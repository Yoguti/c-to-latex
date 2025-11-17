CC = gcc
CFLAGS = -Iinclude -Wextra -Wall -Wshadow -Wcast-align -Wstrict-prototypes -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations -Wnested-externs
BINDIR = bin
TARGET = $(BINDIR)/program
OBJDIR = build
SRCDIR = src
INCLUDEDIR = include
HEADERS = $(wildcard $(INCLUDEDIR)/*.h)
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))

FORMATTER = clang-format -style="{BasedOnStyle: llvm, BreakBeforeBraces: WebKit, IndentWidth: 4}" -i

.PHONY: all clean format

all: $(OBJDIR) $(BINDIR) $(TARGET)

format:
	@echo "Formatting source and headers..."
	$(FORMATTER) $(SOURCES) $(HEADERS)

$(BINDIR):
	mkdir -p $(BINDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CC) $(LDFLAGS) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -I$(INCLUDEDIR) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(BINDIR) $(TARGET)