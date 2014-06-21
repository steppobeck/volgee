
SRCDIR = ./
BINDIR = ../bin
CXX = c++
CXXFLAGS = -I. -I/opt/boost/boost_1_55_0/include -DLINUX -Wall -O3
LDDFLAGS = -lSDL -lSDL_image -lSDL_mixer -lGL -lGLU -lglut -lGLEW -O3

TARGET := $(BINDIR)/volgee


SOURCES = $(wildcard $(SRCDIR)*.cpp)
OBJECTS = $(patsubst $(SRCDIR)%.cpp, %.o, $(SOURCES))
HEADERS = $(wildcard $(SRCDIR)*.h)



default: $(TARGET)
	@echo built $(TARGET)


$(TARGET): $(OBJECTS) $(HEADERS) Makefile
	$(CXX) $(OBJECTS) $(LDDFLAGS) -o $@

%.o: $(SRCDIR)%.cpp Makefile
	$(CXX) -c $< $(CXXFLAGS)


clean:
	@rm -f $(OBJECTS)
	@echo cleaned

realclean: clean
	@rm -f $(TARGET) *~
	@echo realcleaned

cleandefault: clean
cleanDebug: clean
Debug: default
Release: default



