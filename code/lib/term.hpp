#ifndef _H_TERMINAL_
#define _H_TERMINAL_

class Term {
  private:
  Term();
  ~Term() { }

  public:
  Term(const Term&) = delete;
  Term(Term&&) = delete;
  Term& operator=(const Term&) = delete;
  Term& operator=(Term&&) = delete;

  static Term &instance() {
    static Term INSTANCE;
    return INSTANCE;
  }

  void tx(char c);

  void rx_irq(void);
  void tx_dma_irq(void);

  void print(const char *str);
  void print(int i);
  template <typename T> void println(T val);
  void flush(void) { Term::instance().need_flush = true; }
  void idle_flush(void);
  bool flush_needed(void) { return need_flush; }

  private:

  void error(const char *msg);

  static const int RX_BUFSIZE = 80;
  static const int TX_BUFSIZE = 128;

  char rx_buff[RX_BUFSIZE];
  int rx_pos = 0;

  char tx_buff1[TX_BUFSIZE], tx_buff2[TX_BUFSIZE];
  char *tx_buffs[2] = {tx_buff1, tx_buff2};
  int tx_buff_idx = 0;
  char *tx_buff = tx_buffs[0];
  int tx_size = 0;
  volatile bool tx_sending = false;
  bool tx_error = false;
  volatile bool need_flush = false;
};

extern Term &term;

#endif
