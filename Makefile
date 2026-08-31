CC      := g++
CFLAGS  := -std=c++17 -Wall -Wextra -MMD -MP
INCLUDES:= -Iinclude

SRCDIR  := src
OBJDIR  := obj
BINDIR  := bin
TARGET  := $(BINDIR)/app_spi

SRCS := $(shell find $(SRCDIR) -name '*.cc')
OBJS := $(patsubst $(SRCDIR)/%.cc,$(OBJDIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cc
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

-include $(DEPS)

clean:
	rm -rf $(OBJDIR) $(TARGET)

.PHONY: all clean
