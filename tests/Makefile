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

all: $(SRC:.c=)

.c: | $(BINDIR)
	# todo: fixme
	$(MKDIR_P) $(BINDIR)
	$(CC) $^ -o $(BINDIR)/$@ $(CFLAGS) $(LDFLAGS) $(LDLIBS)

$(BINDIR)/:
	$(MKDIR_P) $@

clean:
	$(RM) $(BINDIR)/*
