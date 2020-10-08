CC = g++

INCLUDE = -I include/
FLAGS = -Wall -Wextra -Wshadow -pedantic -std=c++2a -O2 $(INCLUDE)
# 
LIB_FLAGS = -lglut -lGL -lGLU -lGLEW -lIL -lm
SRCEXT = cpp

#############################################################

BUILDDIR = build

########################### All #############################

all: generator renderer

########################## Common ###########################

COMMON_BUILDDIR = build/common
COMMON_SRCDIR = src/common

COMMON_SRC = $(shell find $(COMMON_SRCDIR) -type f -name *.$(SRCEXT))
COMMON_OBJ = $(patsubst $(COMMON_SRCDIR)/%,$(COMMON_BUILDDIR)/%,$(COMMON_SRC:.$(SRCEXT)=.o))

$(COMMON_BUILDDIR)/%.o: $(COMMON_SRCDIR)/%.$(SRCEXT) 
	$(CC) $(FLAGS) -c -o $@ $<

######################### Generator #########################

GEN_SRCDIR = src/generator
GEN_BUILDDIR = build/generator
GEN_TARGET = bin/generator

GEN_SRC = $(shell find $(GEN_SRCDIR) -type f -name *.$(SRCEXT))
GEN_OBJ = $(patsubst $(GEN_SRCDIR)/%,$(GEN_BUILDDIR)/%,$(GEN_SRC:.$(SRCEXT)=.o))

generator: generator_directories $(GEN_TARGET)

$(GEN_TARGET): $(COMMON_OBJ) $(GEN_OBJ) 
	$(CC) $^ -o $(GEN_TARGET) $(LIB_FLAGS)

$(GEN_BUILDDIR)/%.o: $(GEN_SRCDIR)/%.$(SRCEXT)
	$(CC) $(FLAGS) -c -o $@ $<

######################### Renderer ##########################

RENDERER_SRCDIR = src/renderer
RENDERER_BUILDDIR = build/renderer
RENDERER_TARGET = bin/renderer

RENDERER_SRC = $(shell find $(RENDERER_SRCDIR) -type f -name *.$(SRCEXT))
RENDERER_OBJ = $(patsubst $(RENDERER_SRCDIR)/%,$(RENDERER_BUILDDIR)/%,$(RENDERER_SRC:.$(SRCEXT)=.o))

GEN_OBJ_NO_MAIN = $(filter-out $(GEN_BUILDDIR)/main.o,$(GEN_OBJ))

renderer: renderer_directories $(RENDERER_TARGET)

$(RENDERER_TARGET): $(RENDERER_OBJ) $(COMMON_OBJ) $(GEN_OBJ_NO_MAIN) 
	$(CC) $^ -o $(RENDERER_TARGET) $(LIB_FLAGS)

$(RENDERER_BUILDDIR)/%.o: $(RENDERER_SRCDIR)/%.$(SRCEXT)
	$(CC) $(FLAGS) -c -o $@ $<

#############################################################

generator_directories:
	@mkdir -p $(BUILDDIR)/generator
	@mkdir -p $(BUILDDIR)/common
	@mkdir -p bin

renderer_directories:
	@mkdir -p $(BUILDDIR)/renderer
	@mkdir -p $(BUILDDIR)/common
	@mkdir -p bin

clean:
	rm -rf $(BUILDDIR) bin/* models

########################### Tests ###########################

test: all
	./$(GEN_TARGET) sphere 1 20 20 sphere.3d
	./$(RENDERER_TARGET) scenes/test.xml

icecream: all
	./$(GEN_TARGET) cone 1.7 5.5 20 20 cone1.3d
	./$(GEN_TARGET) sphere 2 20 20 sphere2.3d
	./$(GEN_TARGET) sphere 1.8 20 20 sphere1.3d
	./$(GEN_TARGET) box 1 1 1 20 box.3d
	./$(RENDERER_TARGET) scenes/icecream.xml
#
heart: all
	./$(GEN_TARGET) cone 1.7 2.0 20 20 cone.3d
	./$(GEN_TARGET) sphere 1 20 20 sphere1.3d
	./$(RENDERER_TARGET) scenes/heart.xml

humanoid: all
	./$(GEN_TARGET) sphere 0.2 20 20 sphere.3d
	./$(GEN_TARGET) cylinder 0.3 0.6 20 cylinder.3d
	./$(GEN_TARGET) cylinder 0.1 0.8 20 cylinder2.3d
	./$(RENDERER_TARGET) scenes/humanoid.xml

testss: all
	./$(GEN_TARGET) sphere 1 20 20 sphere.3d
	./$(RENDERER_TARGET) scenes/solar_system.xml

#############################################################