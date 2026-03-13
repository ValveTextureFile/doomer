# Makefile for C project (finds .c under project root and include/)
CC ?= gcc
CFLAGS ?= -Wall -Wextra -O2
CPPFLAGS ?= -Iinclude
LDFLAGS ?=

# Vendor libconfuse configuration
USE_SYSTEM_CONFUSE ?= 0
VENDOR_CONFUSE_DIR := include/lib/confuse-3.3
VENDOR_CONFUSE_LIB := $(VENDOR_CONFUSE_DIR)/src/.libs/libconfuse.a

# Install prefix (users can override PREFIX or DESTDIR)
PREFIX ?= /usr/local
DESTDIR ?=

TARGET ?= $(notdir $(CURDIR))
OBJDIR := build/obj

SRCS := $(shell find . -type f -name '*.c' -not -path './include/lib/confuse-3.3/*' -print | sed 's|^\./||')
OBJS := $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

.PHONY: all clean distclean vendor install
all: $(TARGET)

# If not using system libconfuse, ensure the vendored copy is built first
ifeq ($(USE_SYSTEM_CONFUSE),0)
$(TARGET): $(VENDOR_CONFUSE_LIB)
endif

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

# Build vendored libconfuse (invokes the sub-make in the vendored directory)
$(VENDOR_CONFUSE_LIB):
	$(MAKE) -C $(VENDOR_CONFUSE_DIR)

# Add the vendor library to link flags when not using system lib
ifeq ($(USE_SYSTEM_CONFUSE),0)
LDFLAGS += $(VENDOR_CONFUSE_LIB)
CPPFLAGS += -I$(VENDOR_CONFUSE_DIR)/src
else
# Use system library
LDFLAGS += -lconfuse
endif

# compile .c -> .o into build/obj/... preserving source subdirs
$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)

clean:
	rm -rf $(OBJDIR)

vendor:
	$(MAKE) -C $(VENDOR_CONFUSE_DIR)

install: all
	# install the main executable
	install -d $(DESTDIR)$(PREFIX)/bin
	install -m 755 $(TARGET) $(DESTDIR)$(PREFIX)/bin/
	# if we're using the vendored copy, install it into the same prefix
	if [ "$(USE_SYSTEM_CONFUSE)" = "0" ]; then \
		$(MAKE) -C $(VENDOR_CONFUSE_DIR) install prefix=$(PREFIX) DESTDIR=$(DESTDIR); \
	fi

distclean: clean
	rm -f $(TARGET)