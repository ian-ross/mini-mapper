#include "bsp-generic.h"

#include "terminal.hpp"

//----------------------------------------------------------------------
//
//  TESTS
//

#ifdef TEST

#include <iostream>
#include "doctest.h"
#include "doctest/trompeloeil.hpp"
#include "events_mock.hpp"
#include "usart_mock.hpp"

void show_check(std::string ch) {
  std::cout << "check = '";
  for (int i = 0; i < ch.length(); ++i) {
    if (ch[i] == '\r')
      std::cout << 'N';
    else if (ch[i] == '\b')
      std::cout << 'X';
    else
      std::cout << ch[i];
    }
  std::cout << "'" << std::endl;
}

TEST_CASE("Terminal") {
  using trompeloeil::_;

  MockEventWaiter waiter;
  Events::Manager ev(MockEventWaiter::wait_for_event);
  MockUSART usart;
  Terminal terminal(usart);
  MockEventConsumer consumer;
  ev += usart;
  ev += terminal;
  ev += consumer;

  SUBCASE("non-interactive printing (string)") {
    std::string check;
    ALLOW_CALL(usart, tx(_)).LR_SIDE_EFFECT(check += _1);
    REQUIRE_CALL(usart, flush());
    terminal.print("test");
    terminal.flush();
    REQUIRE(check == "test");
  }

  SUBCASE("non-interactive printing (integer)") {
    std::string check;
    ALLOW_CALL(usart, tx(_)).LR_SIDE_EFFECT(check += _1);
    REQUIRE_CALL(usart, flush());
    terminal.print(123);
    terminal.flush();
    REQUIRE(check == "123");
  }

  SUBCASE("non-interactive printing (println)") {
    std::string check;
    ALLOW_CALL(usart, tx(_)).LR_SIDE_EFFECT(check += _1);
    REQUIRE_CALL(usart, flush());
    terminal.print("val=");
    terminal.println(64);
    REQUIRE(check == "val=64\r\n");
  }

  SUBCASE("non-interactive printing (error)") {
    std::string check;
    ALLOW_CALL(usart, tx(_)).LR_SIDE_EFFECT(check += _1);
    REQUIRE_CALL(usart, flush());
    terminal.error("UROR");
    REQUIRE(check == "\r\n!UROR\r\n");
  }

  SUBCASE("initial prompt in interactive mode") {
    std::string check;
    ALLOW_CALL(usart, tx(_)).LR_SIDE_EFFECT(check += _1);
    REQUIRE_CALL(usart, flush()).TIMES(AT_LEAST(1));
    terminal.set_interactive(true);
    REQUIRE(check == "> ");
  }

  SUBCASE("line event generated correctly (non-interactive)") {
    ALLOW_CALL(usart, dispatch(_)).RETURN(false);
    REQUIRE_CALL(consumer, dispatch(Events::Event{Events::TERMINAL_LINE_RECEIVED,0}))
      .RETURN(true);
    for (auto ch : "test\r") {
      if (ch != '\0') ev.post(Events::USART_RX_CHAR, ch);
      ev.drain();
    }
    CHECK(ev.pending_count() == 0);
    CHECK(strcmp(terminal.buffer(TERMINAL_BUFFER_0), "test") == 0);
  }

  SUBCASE("RX buffer overflow detected correctly (non-interactive)") {
    ALLOW_CALL(usart, dispatch(_)).RETURN(false);
    REQUIRE_CALL(consumer, dispatch(Events::Event{Events::TERMINAL_RX_OVERFLOW,0}))
      .RETURN(true);
    for (int i = 0; i < 81; ++i) {
      ev.post(Events::USART_RX_CHAR, 'x');
      ev.drain();
    }
  }

  SUBCASE("line event generated correctly (interactive)") {
    ALLOW_CALL(usart, dispatch(_)).RETURN(false);
    std::string check;
    ALLOW_CALL(usart, tx(_)).LR_SIDE_EFFECT(check += _1);
    ALLOW_CALL(usart, flush());
    REQUIRE_CALL(consumer,
                 dispatch(Events::Event{Events::TERMINAL_LINE_RECEIVED,
                                        TERMINAL_BUFFER_0}))
      .RETURN(true);

    terminal.set_interactive(true);
    for (auto ch : "test\r") {
      if (ch != '\0') ev.post(Events::USART_RX_CHAR, ch);
      ev.drain();
    }
    CHECK(ev.pending_count() == 0);

    // Line contents should be passed to consumer.
    CHECK(strcmp(terminal.buffer(TERMINAL_BUFFER_0), "test") == 0);

    // Input should be echoed (and prompt displayed).
    CHECK(check == "> test\r\n");
  }

  SUBCASE("backspace handled (interactive)") {
    ALLOW_CALL(usart, dispatch(_)).RETURN(false);
    std::string check;
    ALLOW_CALL(usart, tx(_)).LR_SIDE_EFFECT(check += _1);
    ALLOW_CALL(usart, flush());
    REQUIRE_CALL(consumer,
                 dispatch(Events::Event{Events::TERMINAL_LINE_RECEIVED,
                                        TERMINAL_BUFFER_0}))
      .RETURN(true);

    terminal.set_interactive(true);
    for (auto ch : "tex\bst\r") {
      if (ch != '\0') ev.post(Events::USART_RX_CHAR, ch);
      ev.drain();
    }
    CHECK(ev.pending_count() == 0);

    // Edited line contents should be passed to consumer.
    CHECK(strcmp(terminal.buffer(TERMINAL_BUFFER_0), "test") == 0);

    // Input should be echoed including backspace (and prompt displayed).
    CHECK(check == "> tex\b \bst\r\n");
  }

  SUBCASE("response output and prompt (interactive)") {
    ALLOW_CALL(usart, dispatch(_)).RETURN(false);
    std::string check;
    ALLOW_CALL(usart, tx(_)).LR_SIDE_EFFECT(check += _1);
    ALLOW_CALL(usart, flush());
    REQUIRE_CALL(consumer,
                 dispatch(Events::Event{Events::TERMINAL_LINE_RECEIVED,
                                        TERMINAL_BUFFER_0}))
      .LR_SIDE_EFFECT(terminal.println("OK"))
      .LR_SIDE_EFFECT(ev.post(Events::TERMINAL_LINE_PROCESSED))
      .RETURN(true);

    terminal.set_interactive(true);
    for (auto ch : "cmd\r") {
      if (ch != '\0') ev.post(Events::USART_RX_CHAR, ch);
      ev.drain();
    }
    CHECK(ev.pending_count() == 0);

    // Echoed input, command response and prompts.
    CHECK(check == "> cmd\r\nOK\r\n> ");
  }

  SUBCASE("unsolicited output, input line erase and redraw (interactive)") {
    ALLOW_CALL(usart, dispatch(_)).RETURN(false);
    std::string check;
    ALLOW_CALL(usart, tx(_)).LR_SIDE_EFFECT(check += _1);
    ALLOW_CALL(usart, flush());
    REQUIRE_CALL(consumer,
                 dispatch(Events::Event{Events::TERMINAL_LINE_RECEIVED,
                                        TERMINAL_BUFFER_0}))
      .LR_SIDE_EFFECT(terminal.println("OK"))
      .LR_SIDE_EFFECT(ev.post(Events::TERMINAL_LINE_PROCESSED))
      .RETURN(true);

    terminal.set_interactive(true);
    ev.post(Events::USART_RX_CHAR, 'c');
    ev.drain();
    ev.post(Events::USART_RX_CHAR, 'm');
    ev.drain();
    terminal.println("unsolicited");
    ev.post(Events::USART_RX_CHAR, 'd');
    ev.drain();
    ev.post(Events::USART_RX_CHAR, '\r');
    ev.drain();
    CHECK(ev.pending_count() == 0);

    // Echoed input, unsolicited output with line erase and redraw,
    // command response and prompts.
    CHECK(check == "> cm\b \b\b \b\b \b\b \bunsolicited\r\n> cmd\r\nOK\r\n> ");
  }
}

#endif
