INCLDIR = ./include
CC = gcc
CFLAGS = -O2
CFLAGS += -Wall -g -Werror
CFLAGS += -I$(INCLDIR)
LINKEXTRA = -lpthread

OBJDIR = obj

NAME = main

_DEPS = debugger.h list.h
DEPS = $(patsubst %,$(INCLDIR)/%,$(_DEPS))

_OBJS = $(NAME).o $(addsuffix .o,$(basename $(_DEPS))) list.o
OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))


$(OBJDIR)/%.o: %.c $(DEPS) | $(OBJDIR)
	$(CC) -c -o $@ $< $(CFLAGS) 

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LINKEXTRA)

$(OBJDIR):
	mkdir -p $@

.PHONY: clean
clean: 
	rm -f $(OBJDIR)/*.o *~ core $(INCLDIR)/*~ *.a debug_*.txt $(NAME)
