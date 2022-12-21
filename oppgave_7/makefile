INCLDIR = ./include
DEBUGFOLDER = ../debugger
CC = gcc
CFLAGS = -O2
CFLAGS += -Wall -g -Werror
CFLAGS += -I$(INCLDIR)
CFLAGS += -I$(DEBUGFOLDER)/include
LINKEXTRA = -lpthread

OBJDIR = obj

NAME = main
DEBUG_LIB= debugger.a

_DEPS = fileConverter.h
DEPS = $(patsubst %,$(INCLDIR)/%,$(_DEPS))

_OBJS = $(NAME).o $(addsuffix .o,$(basename $(_DEPS)))
OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))


$(OBJDIR)/%.o: %.c $(DEPS) | $(OBJDIR)
	$(CC) -c -o $@ $< $(CFLAGS) 

all: $(NAME)

$(NAME): $(OBJS) $(DEBUG_LIB)
	$(CC) -o $@ $^ $(CFLAGS) $(LINKEXTRA)

$(OBJDIR):
	mkdir -p $@

$(DEBUG_LIB):
	$(MAKE) -C $(DEBUGFOLDER)/. all
	cp $(DEBUGFOLDER)/$(DEBUG_LIB) .

.PHONY: clean
clean: 
	rm -f $(OBJDIR)/*.o *~ core $(INCLDIR)/*~ *.a debug_*.txt $(NAME) *_beautified.c
