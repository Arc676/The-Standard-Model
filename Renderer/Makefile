SHELL=/bin/bash
ODIR=obj
SDIR=src
BDIR=bin

CC=gcc
FLAGS=-std=c11 $(patsubst %, -I %, $(shell echo ../StandardModel/src/*))
LIB=-L ../StandardModel -l standardmodel -lm -L ../OpenGLRecorder -l openglrecorder

EXECOUT=$(BDIR)/tsm_render

ifdef DEBUG
FLAGS+=-g -O0
endif

OBJS=$(shell find $(SDIR) -name '*.c' -printf "%f\n")
_OBJS=$(patsubst %.c, $(ODIR)/%.o, $(OBJS))

renderer: makedir $(_OBJS)
	$(CC) $(_OBJS) $(LIB) -o $(EXECOUT)

makedir:
	mkdir -p $(ODIR)
	mkdir -p $(BDIR)

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) -c $(FLAGS) -o $@ $<

clean:
	rm -f $(EXECOUT) $(ODIR)/*.o
