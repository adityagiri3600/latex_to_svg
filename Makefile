# Directories
SRCDIR = src
BUILDDIR = build

# Output binary
TARGET = $(BUILDDIR)/l2s

# Source files
SOURCES = $(SRCDIR)/main.c $(SRCDIR)/ast.c
GENERATED_SOURCES = $(BUILDDIR)/lexer.c $(BUILDDIR)/parser.c
OBJECTS = $(BUILDDIR)/main.o $(BUILDDIR)/ast.o $(BUILDDIR)/lexer.o $(BUILDDIR)/parser.o $(BUILDDIR)/svg_letters.o

# Generated headers
PARSER_HEADER = $(BUILDDIR)/parser.h

# Default target
all: $(TARGET)

# Create build directory if it doesn't exist
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# Main target
$(TARGET): $(BUILDDIR) $(OBJECTS)
	gcc -o $(TARGET) $(OBJECTS)

# Object files
$(BUILDDIR)/main.o: $(SRCDIR)/main.c $(SRCDIR)/ast.h $(PARSER_HEADER)
	gcc -c $(SRCDIR)/main.c -I$(SRCDIR) -I$(BUILDDIR) -o $(BUILDDIR)/main.o

$(BUILDDIR)/ast.o: $(SRCDIR)/ast.c $(SRCDIR)/ast.h
	gcc -c $(SRCDIR)/ast.c -I$(SRCDIR) -o $(BUILDDIR)/ast.o

$(BUILDDIR)/svg_letters.o: $(SRCDIR)/svg_letters.c $(SRCDIR)/svg_letters.h
	gcc -c $(SRCDIR)/svg_letters.c -I$(SRCDIR) -o $(BUILDDIR)/svg_letters.o

$(BUILDDIR)/lexer.o: $(BUILDDIR)/lexer.c $(PARSER_HEADER)
	gcc -c $(BUILDDIR)/lexer.c -I$(SRCDIR) -I$(BUILDDIR) -o $(BUILDDIR)/lexer.o

$(BUILDDIR)/parser.o: $(BUILDDIR)/parser.c
	gcc -c $(BUILDDIR)/parser.c -I$(SRCDIR) -I$(BUILDDIR) -o $(BUILDDIR)/parser.o

# Generated source files
$(BUILDDIR)/lexer.c: $(SRCDIR)/lexer.l $(PARSER_HEADER) | $(BUILDDIR)
	flex -o $(BUILDDIR)/lexer.c $(SRCDIR)/lexer.l

$(BUILDDIR)/parser.c $(PARSER_HEADER): $(SRCDIR)/parser.y | $(BUILDDIR)
	bison -d -o $(BUILDDIR)/parser.c $(SRCDIR)/parser.y

# Clean up
clean:
	rm -rf $(BUILDDIR)

# Phony targets
.PHONY: all clean
