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

// Enumeration used for identifying which terminal RX buffer contains
// the latest line to be processed.

enum TerminalRXBuffer {
  TERMINAL_BUFFER_0 = 0,
  TERMINAL_BUFFER_1
};


// Interface representing terminal functionality. Used to allow for
// easy mocking for testing.

class TerminalInterface {
public:
  TerminalInterface() { }
  virtual void set_interactive(bool inter) = 0;
  virtual char *buffer(TerminalRXBuffer i) = 0;
  virtual void print(const char *str) = 0;
  virtual void print(int i) = 0;
  virtual void println(const char *str) = 0;
  virtual void println(int i) = 0;
  virtual void flush(void) = 0;
  virtual void error(const char *msg) = 0;
};


// The receive buffer size is set to a "normal" line length.

static const int TERMINAL_RX_BUFSIZE = 80;



template<typename USART>
class Terminal : public TerminalInterface, public Events::Consumer {
public:

  // The state of the Terminal indicates whether we have an input line
  // (and prompt) displayed in interactive mode (in that case, we need
  // to erase and redraw the input line to output lines of unsolicited
  // output).
  enum State { INITIAL, INPUT, PROCESSING };

  Terminal(USART &usart): Events::Consumer("Terminal"), usart(usart) { }

  // Control of interactive mode.
  void set_interactive(bool inter) override;

  // Buffer access.
  char *buffer(TerminalRXBuffer i) override { return rx_buffs[i].data(); }

  // I/O functions.
  void print(const char *str) override;
  void print(int i) override;
  void println(const char *str) override;
  void println(int i) override;
  void flush(void) override;
  void error(const char *msg) override;

  // Event handling.
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
  std::array<char, TERMINAL_RX_BUFSIZE> rx_buffs[2];
  int rx_pos = 0;
  std::array<char, TERMINAL_RX_BUFSIZE> *rx_buff = &rx_buffs[0];
  TerminalRXBuffer rx_buff_idx = TERMINAL_BUFFER_0;

  char tx_buff[USART_TX_BUFSIZE];
  int tx_size = 0;
};


// Save a single character for transmission. We do this rather than
// sending characters directly to the USART so that we can erase and
// redraw input lines in interactive mode.

template <typename USART> void Terminal<USART>::tx(char ch) {
  if (tx_size >= USART_TX_BUFSIZE) {
    tx_size = 0;
    mgr->post(Events::USART_TX_OVERFLOW);
    return;
  }
  tx_buff[tx_size++] = ch;
}


// Variants of output functions with new line and flush.

template<typename USART>
void Terminal<USART>::println(const char *str) {
  print(str);
  tx('\r');
  tx('\n');
  flush();
}

template<typename USART>
void Terminal<USART>::println(int i) {
  print(i);
  tx('\r');
  tx('\n');
  flush();
}


// Output a string.

template<typename USART>
void Terminal<USART>::print(const char *str) {
  while (*str) {
    tx(*str);
    ++str;
  }
}


// Output an integer.

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


// Output an error message. These are all short codes preceded by a
// '!' character.

template<typename USART>
void Terminal<USART>::error(const char *msg) {
  print("!");
  println(msg);
}


// Flush output: in interactive mode, we need to erase and redraw the
// input line to avoid corruption in the serial terminal application
// connected to our USART.

template<typename USART>
void Terminal<USART>::flush(void) {
  if (interactive && state == INPUT) erase_input();
  for (int i = 0; i < tx_size; ++i) usart.tx(tx_buff[i]);
  tx_size = 0;
  usart.flush();
  if (interactive && state == INPUT) redraw_input();
}


// Switch between interactive and the default non-interactive mode.

template<typename USART>
void Terminal<USART>::set_interactive(bool inter) {
  interactive = inter;
  if (interactive) prompt(true);
}


// The events that we need to deal with are character input from the
// USART, and signalling from downstream consumers of line input that
// they are done with processing the last line that was received.

template <typename USART>
bool Terminal<USART>::dispatch(const Events::Event &e) {
  switch (e.tag) {
  case Events::USART_RX_CHAR:
    process_rx_char(e.param);
    return true;

  case Events::TERMINAL_LINE_PROCESSED:
    if (interactive) prompt(true);
    state = INPUT;
    return true;

  default:
    return false;
  }
}


// Process a single character received from the USART.

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
    state = PROCESSING;

    break;

  case '\b':
    // Backspace deletes a single character backwards.
    if (rx_pos > 0) {
      --rx_pos;
      // In interactive mode, we need to erase the character in the
      // serial terminal view as well as in the buffer.
      if (interactive) {
        usart.tx('\b'); usart.tx(' '); usart.tx('\b'); usart.flush();
      }
    }
    break;

  default:
    // The size limit here is to leave space for the string
    // terminating null at the end of line.
    if (rx_pos < TERMINAL_RX_BUFSIZE - 1) {
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


// The following two methods are used when it's necessary to write an
// output line when an input line is being edited: the input line is
// erased, the output line is written, then the input line is redrawn.

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
