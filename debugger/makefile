INCLDIR = ./include
CC = gcc
CFLAGS = -O2
CFLAGS += -Wall -g -Werror
CFLAGS += -I$(INCLDIR)
LINKEXTRA = -lpthread

OBJDIR = obj

NAME = debugger.a

_DEPS = debugger.h
DEPS = $(patsubst %,$(INCLDIR)/%,$(_DEPS))

_OBJS = $(addsuffix .o,$(basename $(_DEPS)))
OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))

$(OBJDIR)/%.o: %.c $(DEPS) | $(OBJDIR)
	$(CC) -c -o $@ $< $(CFLAGS) 

$(NAME): $(OBJS) $(DEPS)
	ar rcs $@ $^

all: $(NAME)

$(OBJDIR):
	mkdir -p $@

.PHONY: clean
clean: 
	rm -f $(OBJDIR)/*.o *~ core $(INCLDIR)/*~ *.a debug_*.txt $(NAME)
