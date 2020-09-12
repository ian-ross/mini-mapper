SUBDIRS = \
  systick-blinky \
  systick-usart-counter \
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
