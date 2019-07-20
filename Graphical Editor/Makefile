SHELL=/bin/bash
ODIR=obj
SDIR=src
BDIR=bin

CC=g++
FLAGS=-std=c++11 $(patsubst %, -I %, $(shell echo ../StandardModel/src/*)) -I ../SimulatorLib/src -I ../imgui -I ../imgui/examples
LIB=-L ../StandardModel -l standardmodel -L ../SimulatorLib -l simulator -lm -l GL -l glfw -l GLEW

EXECOUT=$(BDIR)/tsm_editor

ifdef DEBUG
FLAGS+=-g -O0
endif

OBJS=$(shell find $(SDIR) -name '*.cpp' -printf "%f\n")
_OBJS=$(patsubst %.cpp, $(ODIR)/%.o, $(OBJS))

IMGUI_SRC=imgui.cpp imgui_draw.cpp imgui_widgets.cpp examples/imgui_impl_glfw.cpp examples/imgui_impl_opengl3.cpp
IMGUI_OBJ=$(patsubst %.cpp, $(ODIR)/%.o, $(patsubst examples/%, %, $(IMGUI_SRC)))

IMGUI_FLAGS=-I ../imgui -I ../imgui/examples -D IMGUI_IMPL_OPENGL_LOADER_GLEW

editor: makedir $(_OBJS)
	test -s $(ODIR)/imgui.o || make imgui
	$(CC) $(IMGUI_OBJ) $(_OBJS) $(LIB) -o $(EXECOUT)

imgui:
	$(CC) -c $(IMGUI_FLAGS) $(patsubst %.cpp, ../imgui/%.cpp, $(IMGUI_SRC))
	mv *.o $(ODIR)

makedir:
	mkdir -p $(ODIR)
	mkdir -p $(BDIR)

$(ODIR)/%.o: $(SDIR)/%.cpp
	$(CC) -c $(FLAGS) -o $@ $<

clean:
	rm -f $(EXECOUT) $(ODIR)/*.o
