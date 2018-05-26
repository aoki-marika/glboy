CC = gcc
CFLAGS = -Wall -Iinclude
LDLIBS = -lSDL2
MKDIR_P = mkdir -p

ifeq ($(shell uname -s), Darwin)
	LDLIBS += -framework OpenGL
else
	LDLIBS += -lGL
endif

SRCDIR = src
BINDIR = bin
OBJDIR = $(BINDIR)/obj

SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
OUTPUT = $(BINDIR)/libglboy.a

$(OUTPUT):$(OBJ) | $(BINDIR)
	ar ru $@ $^
	ranlib $@

$(OBJ): $(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ $^

$(BINDIR)/:
	$(MKDIR_P) $@

$(OBJDIR)/:
	$(MKDIR_P) $@

clean:
	rm $(OBJ) $(OUTPUT)