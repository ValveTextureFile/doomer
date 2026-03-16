CC ?= gcc
CFLAGS ?= -Wall -Wextra -O2
CPPFLAGS ?= -Iinclude
LDFLAGS ?=
LDLIBS ?=

USE_SYSTEM_CONFUSE ?= 0
VENDOR_CONFUSE_DIR := vendor/confuse-3.3
VENDOR_CONFUSE_LIB := $(VENDOR_CONFUSE_DIR)/src/.libs/libconfuse.a

PREFIX ?= /usr/local
DESTDIR ?=

TARGET ?= $(notdir $(CURDIR))
OBJDIR := build/obj

SRCS := $(shell find src -type f -name '*.c' -print)
OBJS := $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

.PHONY: all clean distclean vendor install

ifeq ($(USE_SYSTEM_CONFUSE),0)
CPPFLAGS += -I$(VENDOR_CONFUSE_DIR)/src
LDLIBS += $(VENDOR_CONFUSE_LIB)
else
LDLIBS += -lconfuse
endif

all: $(TARGET)

ifeq ($(USE_SYSTEM_CONFUSE),0)
$(TARGET): $(VENDOR_CONFUSE_LIB)
endif

$(TARGET): $(OBJS)
	$(CC) $^ $(LDFLAGS) $(LDLIBS) -o $@

$(VENDOR_CONFUSE_LIB):
	$(MAKE) -C $(VENDOR_CONFUSE_DIR)

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)

clean:
	rm -rf $(OBJDIR)

vendor:
	$(MAKE) -C $(VENDOR_CONFUSE_DIR)

install: all
	install -d $(DESTDIR)$(PREFIX)/bin
	install -m 755 $(TARGET) $(DESTDIR)$(PREFIX)/bin/
	if [ "$(USE_SYSTEM_CONFUSE)" = "0" ]; then \
		$(MAKE) -C $(VENDOR_CONFUSE_DIR) install prefix=$(PREFIX) DESTDIR=$(DESTDIR); \
	fi

distclean: clean
	rm -f $(TARGET)