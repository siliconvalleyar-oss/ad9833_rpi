CC      := g++
CFLAGS  := -std=c++17 -Wall -Wextra -MMD -MP
INCLUDES:= -Iinclude
LDLIBS  := -lbcm2835

SRCDIR  := src
OBJDIR  := obj
BINDIR  := bin
TARGET  := $(BINDIR)/app_spi

SRCS := $(shell find $(SRCDIR) -name '*.cpp')
OBJS := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $^ $(LDLIBS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

-include $(DEPS)

clean:
	rm -rf $(OBJDIR) $(TARGET)

.PHONY: all clean
