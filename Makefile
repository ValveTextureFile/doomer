# Makefile for C project (finds .c under project root and include/)
CC ?= gcc
CFLAGS ?= -Wall -Wextra -O2
CPPFLAGS ?= -Iinclude
LDFLAGS ?=

TARGET ?= $(notdir $(CURDIR))
OBJDIR := build/obj

SRCS := $(shell find . -type f -name '*.c' -print | sed 's|^\./||')
OBJS := $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

.PHONY: all clean distclean
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

# compile .c -> .o into build/obj/... preserving source subdirs
$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)

clean:
	rm -rf $(OBJDIR)

distclean: clean
	rm -f $(TARGET)