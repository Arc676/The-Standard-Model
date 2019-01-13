SHELL=/bin/bash
ODIR=obj
SDIR=src

CC=gcc
FLAGS=-std=c11 $(patsubst %, -I %, $(shell echo $(SDIR)/*))

LIBOUT=libstandardmodel.a

ifdef DEBUG
FLAGS+=-g -O0
endif

OBJS=$(shell find $(SDIR) -name '*.c' -printf "%f\n")
_OBJS=$(patsubst %.c, $(ODIR)/%.o, $(OBJS))

lib: makeodir $(_OBJS)
	ar rcs $(LIBOUT) $(_OBJS)

makeodir:
	mkdir -p $(ODIR)

$(ODIR)/%.o: $(SDIR)/*/%.c
	$(CC) -c $(FLAGS) -o $@ $<

clean:
	rm -f $(LIBOUT) $(ODIR)/*.o
