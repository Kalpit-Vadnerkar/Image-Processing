# Object files
OFILES = base/ImgProc.o \
         base/ImageProcessing.o \
         base/Fractals.o  \
         base/StarterViewer.o \
         base/imgviewer.o

# Directories
ROOTDIR = .
BINDIR = $(ROOTDIR)/bin
LIBDIR = $(ROOTDIR)/lib

# Name of the static library
LIBNAME = kvadner.a
LIB = $(LIBDIR)/$(LIBNAME)

# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -g -O2 -fPIC -fopenmp -std=c++14
INCLUDES = -I./include/ -I../build/include -I/usr/local/include/ -I/usr/include/

# OpenGL and other libraries flags
GLLDFLAGS = -lglut -lGL -lm -lGLU -L../build/lib -lOpenImageIO -lOpenImageIO_Util

# Pattern rule for object files
%.o: %.C
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Default rule
.PHONY: all
all: $(BINDIR)/imgviewer

# Create the static library
$(LIB): $(OFILES)
	ar rcs $@ $^

# imgviewer executable
$(BINDIR)/imgviewer: $(OFILES) $(LIB)
	$(CXX) $(CXXFLAGS) $^ $(GLLDFLAGS) -o $@

# Clean rule
.PHONY: clean
clean:
	rm -rf $(BINDIR)/imgviewer $(OFILES) $(LIB)

# Documentation rule
.PHONY: doc
doc:
	cd doc; doxygen Doxyfile

