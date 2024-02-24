
CC = gcc
MINGWCC = x86_64-w64-mingw32-gcc
CFLAGS =
SRCDIR := src
BUILDDIR = target

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))
SRC=$(call rwildcard,$(SRCDIR),*.c)

.PHONY: all
all: $(BUILDDIR)/snake $(BUILDDIR)/snake.exe
$(BUILDDIR)/snake: $(SRC)
	@ echo !==== COMPILING $^
	@ mkdir -p $(@D)
	$(CC) $(CFLAGS) $(SRC) -lm -o $@
	
$(BUILDDIR)/snake.exe: $(SRC)
	@ echo !==== COMPILING $^
	@ mkdir -p $(@D)
	$(MINGWCC) $(CFLAGS) $(SRC) -lm -o $@
clean:
	rm $(BUILDDIR) -rfv
run: $(BUILDDIR)/snake
	$(BUILDDIR)/snake
run-windows: $(BUILDDIR)/snake.exe
	wine $(BUILDDIR)/snake.exe
