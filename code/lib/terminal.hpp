#ifndef _H_TERMINAL_
#define _H_TERMINAL_

#include "events.hpp"
#include "usart.hpp"

// Class representing a serial terminal connected to a USART. (USART3
// is connected to the ST-Link on the STM32F767ZI Nucleo board.)
//
// Characters received from the USART are collected into an input
// buffer until and end-of-line is received. At that point, a
// TERMINAL_LINE_RECEIVED event is posted. This event must be consumed
// by an object that processes input lines. When the input line
// processor is done with processing the line and has generated all
// relevant response output, it should post a TERMINAL_LINE_PROCESSED
// event.
//
// A terminal can be interactive, in which case:
//
//  - Received characters are echoed.
//  - Backspace ('\b') characters are handled, deleting the last
//    character in the input line.
//  - A prompt ("> ") is printed before each input line.
//  - A distinction is made between unsolicited output and response
//    output. Unsolicited output is output generated while the input
//    prompt is displayed. Response output is output generated while a
//    line is being processed, i.e. between the posting of a
//    TERMINAL_LINE_RECEIVED event and its matching
//    TERMINAL_LINE_PROCESSED event.
//  - When a line of unsolicited output is flushed, the input line is
//    deleted and redrawn.
//
// In non-interactive mode:
//
//  - Received characters are not echoed.
//  - No prompt is printed.
//  - Backspace characters are not processed.
//  - No distinction is made between unsolicited output and response
//    output.

enum TerminalRXBuffer {
  TERMINAL_BUFFER_0 = 0,
  TERMINAL_BUFFER_1
};

template<typename USART>
class Terminal : public Events::Consumer {
public:

  enum State { INITIAL, INPUT, PROCESSING };

  Terminal(USART &usart): Events::Consumer("Terminal"), usart(usart) { }

  // Control.
  void set_interactive(bool inter);

  // Buffer access.
  const char *buffer(TerminalRXBuffer i) { return rx_buffs[i].data(); }

  // I/O functions.
  void print(const char *str);
  void print(int i);
  template <typename T> void println(T val);
  void flush(void);
  void error(const char *msg);

  bool dispatch(const Events::Event &e) override;

private:

  void tx(char ch);
  void prompt(bool f) {
    print("> ");
    if (f) flush();
    if (state == INITIAL) state = INPUT;
  }
  void process_rx_char(char ch);
  void erase_input(void);
  void redraw_input(void);

  USART &usart;
  bool interactive = false;
  State state = INITIAL;

  // RX buffer handling: two input buffers, one active at a time. The
  // non-active buffer is "loaned out" for processing of a complete
  // input line.
  static const int RX_BUFSIZE = 80;
  std::array<char, RX_BUFSIZE> rx_buffs[2];
  int rx_pos = 0;
  bool rx_other_buffer_on_loan = false;
  std::array<char, RX_BUFSIZE> *rx_buff = &rx_buffs[0];
  TerminalRXBuffer rx_buff_idx = TERMINAL_BUFFER_0;

  char tx_buff[USART_TX_BUFSIZE];
  int tx_size = 0;
};

template <typename USART> void Terminal<USART>::tx(char ch) {
  if (tx_size >= USART_TX_BUFSIZE) {
    tx_size = 0;
    mgr->post(Events::USART_TX_OVERFLOW);
    return;
  }
  tx_buff[tx_size++] = ch;
}

template<typename USART>
template<typename T> void Terminal<USART>::println(T val) {
  print(val);
  tx('\r');
  tx('\n');
  flush();
}

template<typename USART>
void Terminal<USART>::print(const char *str) {
  while (*str) {
    tx(*str);
    ++str;
  }
}

template<typename USART>
void Terminal<USART>::print(int i) {
  const int MAX_DIGITS = 16;
  char buff[MAX_DIGITS + 1];
  buff[MAX_DIGITS] = '\0';
  int idx = MAX_DIGITS;
  do {
    buff[--idx] = '0' + i % 10;
    i /= 10;
  } while (i > 0);
  while (buff[idx])
    tx(buff[idx++]);
}

template<typename USART>
void Terminal<USART>::error(const char *msg) {
  tx('\r');
  tx('\n');
  print("!");
  println(msg);
}

template<typename USART>
void Terminal<USART>::flush(void) {
  if (interactive && state == INPUT) erase_input();
  for (int i = 0; i < tx_size; ++i) usart.tx(tx_buff[i]);
  tx_size = 0;
  usart.flush();
  if (interactive && state == INPUT) redraw_input();
}

template<typename USART>
void Terminal<USART>::set_interactive(bool inter) {
  interactive = inter;
  if (interactive) prompt(true);
}

template <typename USART>
bool Terminal<USART>::dispatch(const Events::Event &e) {
  switch (e.tag) {
  case Events::USART_RX_CHAR:
    process_rx_char(e.param);
    return true;

  case Events::TERMINAL_LINE_PROCESSED:
    if (interactive) prompt(true);
    rx_other_buffer_on_loan = false;
    state = INPUT;
    return true;

  default:
    return false;
  }
}

template <typename USART>
void Terminal<USART>::process_rx_char(char ch) {
  switch (ch) {
  case '\r':
    // End of line signalled just by carriage return...
    if (interactive) {
      usart.tx('\r');
      usart.tx('\n');
      usart.flush();
    }

    // Terminate line and post "line received" event with indicator of
    // which buffer the line is in.
    (*rx_buff)[rx_pos] = '\0';
    mgr->post(Events::TERMINAL_LINE_RECEIVED, rx_buff_idx);

    // Switch RX buffers and mark that the other buffer is in use for
    // line processing.
    rx_buff_idx = rx_buff_idx == TERMINAL_BUFFER_0 ?
      TERMINAL_BUFFER_1 : TERMINAL_BUFFER_0;
    rx_buff = &rx_buffs[rx_buff_idx];
    rx_buff->fill('\0');
    rx_pos = 0;
    rx_other_buffer_on_loan = true;
    state = PROCESSING;

    break;

  case '\b':
    if (rx_pos > 0) {
      --rx_pos;
      usart.tx('\b'); usart.tx(' '); usart.tx('\b'); usart.flush();
    }
    break;

  default:
    // The size limit here is to leave space for the string
    // terminating null at the end of line.
    if (rx_pos < RX_BUFSIZE - 1) {
      (*rx_buff)[rx_pos++] = ch;
      // In interactive mode, echo the received character.
      if (interactive) {
        usart.tx(ch);
        usart.flush();
      }
    }  else {
      // On an overflow, just reset the buffer and signal the error
      // with an event.
      mgr->post(Events::TERMINAL_RX_OVERFLOW);
      rx_pos = 0;
    }
  }
}

template <typename USART> void Terminal<USART>::erase_input(void) {
  for (int i = 0; i < rx_pos + 2; ++i) {
    usart.tx('\b');
    usart.tx(' ');
    usart.tx('\b');
  }
  usart.flush();
}

template <typename USART> void Terminal<USART>::redraw_input(void) {
  usart.tx('>');
  usart.tx(' ');
  for (int i = 0; i < rx_pos; ++i) usart.tx((*rx_buff)[i]);
  usart.flush();
}

#endif
