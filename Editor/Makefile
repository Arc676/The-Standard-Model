SHELL=/bin/bash
ODIR=obj
SDIR=src
BDIR=bin

CC=gcc
FLAGS=-std=c11 $(patsubst %, -I %, $(shell echo ../StandardModel/src/*)) -I ../SimulatorLib/src
LIB=-L ../StandardModel -L ../SimulatorLib -l standardmodel -l simulator -lm

EXECOUT=$(BDIR)/tsm_editor

ifdef DEBUG
FLAGS+=-g -O0
endif

OBJS=$(shell find $(SDIR) -name '*.c' -printf "%f\n")
_OBJS=$(patsubst %.c, $(ODIR)/%.o, $(OBJS))

simulator: makedir $(_OBJS)
	$(CC) $(_OBJS) $(LIB) -o $(EXECOUT)

makedir:
	mkdir -p $(ODIR)
	mkdir -p $(BDIR)

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) -c $(FLAGS) -o $@ $<

clean:
	rm -f $(EXECOUT) $(ODIR)/*.o
