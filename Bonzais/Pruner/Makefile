# Squeezer Makefile
# Author: Ph. Gras CEA/IRFU Saclay
# Jan. 3, 11

.PHONY: all clean

SOURCES=$(shell ls *.cc)
OBJS=$(addsuffix .o, $(basename $(SOURCES)))
DEPS=$(addsuffix .d, $(basename $(SOURCES)))

CPPFLAGS+=-MD -Wall
CXXFLAGS+=$(shell root-config --cflags) -I.
CXXFLAGS+=-g -O0
LDFLAGS+=$(shell root-config --libs --ldflags)
LINK.o = $(CXX) $(LDFLAGS) $(TARGET_ARCH)
ROOTCINT=rootcint
CINT_INCDIR=$(shell root-config --incdir)
EXE=pruner
all: $(EXE)

clean:
	-rm $(EXE) $(OBJS) $(DEPS)

$(EXE): $(OBJS)
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@

%Dict.o: %Dict.cc

%Dict.cc : %.h  %LinkDef.h
	$(ROOTCINT) -f $@ -c -I. -I$(CINT_INCDIR) $(DEFINES) $^

echo_%:
	@echo $* = $($*)

-include $(DEPS)

#ignore missing header file to prevent 'make' error after a header file
 #removal:
%.h:;
