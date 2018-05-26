CC = gcc
CFLAGS = -I ../include #todo: glboy/*.h
LDFLAGS = -L ../bin
LDLIBS = -lglboy -lSDL2
MKDIR_P = mkdir -p
RM = rm

ifeq ($(shell uname -s), Darwin)
	LDLIBS += -framework OpenGL
else
	LDLIBS += -lGL
endif

BINDIR = bin
SRC = $(wildcard *.c)

all: libglboy $(BINDIR) $(SRC:.c=)

.PHONY: libglboy
libglboy:
	$(MAKE) -C ../

.c:
	$(CC) $^ -o $(BINDIR)/$@ $(CFLAGS) $(LDFLAGS) $(LDLIBS)

$(BINDIR)/:
	$(MKDIR_P) $@

clean:
	$(RM) $(BINDIR)/*
