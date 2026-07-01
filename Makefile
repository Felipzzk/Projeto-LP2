CC      = gcc
CFLAGS  = -Wall -Wextra -std=c99 -g
TARGET  = inventario_rpg
SRCDIR  = src
OBJDIR  = build

SRCS = $(SRCDIR)/main.c \
       $(SRCDIR)/item.c \
       $(SRCDIR)/inventario.c \
       $(SRCDIR)/arquivo.c \
       $(SRCDIR)/eventos.c \
       $(SRCDIR)/historico.c \
       $(SRCDIR)/ui.c \
       $(SRCDIR)/menus.c

OBJS = $(OBJDIR)/main.o \
       $(OBJDIR)/item.o \
       $(OBJDIR)/inventario.o \
       $(OBJDIR)/arquivo.o \
       $(OBJDIR)/eventos.o \
       $(OBJDIR)/historico.o \
       $(OBJDIR)/ui.o \
       $(OBJDIR)/menus.o

all: $(OBJDIR) $(TARGET)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(OBJDIR) $(TARGET)

.PHONY: all run clean
