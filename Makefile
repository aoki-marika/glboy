CC = gcc
CFLAGS = -Wall -Iinclude/glboy
MKDIR_P = mkdir -p
RM = rm

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
	$(RM) $(OBJ) $(OUTPUT)