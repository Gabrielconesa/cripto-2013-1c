CFLAGS=-fno-builtin -I$(SRCDIR) -pedantic -std=c99 -fstrict-aliasing -Wall -Wextra -Wshadow -Wcast-qual \
	-Wwrite-strings -Wpointer-arith -Wcast-align -Wmissing-prototypes \
	-Wmissing-declarations -Wredundant-decls -Wnested-externs -Winline \
	-Wstrict-prototypes -Wunreachable-code -include $(SRCDIR)/global.h

SRCDIR=../src
OBJDIR=../bin
TARGET=$(OBJDIR)/stegobmp
FOLDERS=. $(CHILD_FOLDERS)
CHILD_FOLDERS=$(shell find . -mindepth 1 -type d)
OBJS=$(addprefix $(OBJDIR)/,$(foreach dir,$(FOLDERS),$(patsubst %.c, %.o, $(wildcard $(dir)/*.c)) $(patsubst %.asm, %-asm.o, $(wildcard $(dir)/*.asm))))
INCLUDES=$(addprefix $(SRCDIR)/,$(foreach dir,$(FOLDERS), $(wildcard $(dir)/*.h)))
CC=gcc
LD=gcc

.PHONY: debug release clean all prepare

.DEFAULT: $(TARGET)

$(TARGET): $(OBJS) $(INCLUDES)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c $(CFLAGS) $< -o $@
%.c:

all: release

# Debug flags
debug: override CFLAGS += -O0 -g
debug: $(TARGET)

# Release flags
release: override CFLAGS += -O2 -g
release: $(TARGET)

clean: 
	-rm $(OBJS)
	-cd $(OBJDIR) && rm -rf $(CHILD_FOLDERS)
	-rm $(TARGET)

prepare:
	cd $(OBJDIR) && mkdir -p $(CHILD_FOLDERS)
