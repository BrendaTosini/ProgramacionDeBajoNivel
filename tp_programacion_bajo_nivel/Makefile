# Detectar el sistema operativo
UNAME_S := $(shell uname -s)

# Variables
CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0` -Iinclude -Wall -Wextra -pedantic -std=c99
LDFLAGS = `pkg-config --libs gtk+-3.0` -lcunit
SRCDIR = src
TESTDIR = $(SRCDIR)/test
INCDIR = include
BINDIR = bin
TARGET = $(BINDIR)/main
TEST_TARGETS = $(BINDIR)/testEstudiantes $(BINDIR)/testMaterias

# Encuentra todos los archivos .c en el directorio src y src/test
SRC = $(wildcard $(SRCDIR)/*.c)
TEST_SRC = $(wildcard $(TESTDIR)/*.c)

# Genera los nombres de los archivos objeto (.o)
OBJ = $(SRC:$(SRCDIR)/%.c=$(BINDIR)/%.o)
TEST_OBJ = $(TEST_SRC:$(TESTDIR)/%.c=$(BINDIR)/%.o)

# Comandos específicos por sistema operativo
ifeq ($(UNAME_S), Linux)
    MKDIR = mkdir -p
    RM = rm -f $(BINDIR)/*.o $(TARGET) $(TEST_TARGETS)
endif

ifeq ($(UNAME_S), Darwin)
    MKDIR = mkdir -p
    RM = rm -f $(BINDIR)/*.o $(TARGET) $(TEST_TARGETS)
endif

ifeq ($(OS), Windows_NT)
    CC = gcc
    MKDIR = if not exist $(subst /,\,$(BINDIR)) mkdir $(subst /,\,$(BINDIR))
    RM = del /Q $(subst /,\,$(BINDIR))\\*.o $(subst /,\,$(TARGET)) $(subst /,\,$(TEST_TARGETS))
endif

# Regla por defecto
all: $(TARGET) tests

# Regla para compilar el programa
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

# Regla para compilar las pruebas
tests: $(TEST_TARGETS)

$(BINDIR)/testEstudiantes: $(BINDIR)/testEstudiantes.o $(filter-out $(BINDIR)/main.o, $(OBJ))
	$(CC) $^ -o $@ $(LDFLAGS)

$(BINDIR)/testMaterias: $(BINDIR)/testMaterias.o $(filter-out $(BINDIR)/main.o, $(OBJ))
	$(CC) $^ -o $@ $(LDFLAGS)

# Regla para compilar los archivos objeto
$(BINDIR)/%.o: $(SRCDIR)/%.c | $(BINDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Regla para compilar las pruebas objeto
$(BINDIR)/%.o: $(TESTDIR)/%.c | $(BINDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Crea el directorio bin si no existe
$(BINDIR):
	$(MKDIR)

# Limpiar los archivos compilados
clean:
	$(RM)

.PHONY: all tests clean
