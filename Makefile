CXX = $(shell wx-config --cxx)

PROGRAM = main

ODIR = obj
IDIR = include

_OBJ = main.o mainWindow.o frame.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_DEPS = mainWindow.hpp frame.hpp config.hpp
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CXX) -c `wx-config --cxxflags` -o $@ $<

all: $(PROGRAM)

$(PROGRAM): $(OBJ)
	$(CXX) -o $(PROGRAM) $(OBJ) `wx-config --libs`

clean:
	rm -f $(ODIR)/*.o $(PROGRAM)
