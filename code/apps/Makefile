SUBDIRS = \
  systick-blinky \
  systick-usart-counter \
  shell-blinky \
  shell-pwm \
  terminal-demo

all:
	@for subdir in $(SUBDIRS); do \
	  echo "Making in $${subdir}"; \
	  $(MAKE) -C "$${subdir}" \
    || exit 1; \
	done

.PHONY: clean
clean:
	@for subdir in $(SUBDIRS); do \
	  echo "Making clean in $${subdir}"; \
	  $(MAKE) -C "$${subdir}" clean \
    || exit 1; \
	done
	$(RM) -fr lib-build
