
CC = clang
CFLAGS =
SRCDIR := src
BUILDDIR = target

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))
SRC=$(call rwildcard,$(SRCDIR),*.c)

.PHONY: all
all: $(BUILDDIR)/snake
$(BUILDDIR)/snake: $(SRC)
	@ echo !==== COMPILING $^
	@ mkdir -p $(@D)
	$(CC) $(CFLAGS) $(SRC) -o $@
clean:
	rm $(BUILDDIR) -rfv
run: all
	$(BUILDDIR)/snake
