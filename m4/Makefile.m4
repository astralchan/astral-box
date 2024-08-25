dnl Copyright (C) astral
dnl See COPYING for details.
include(`m4/forloop.m4')
include(`m4/utils.m4')
include(`Makefile.in')

all: \
	src/astral-box \
	src/astral-box \
dnl	src/{util} \

src/astral-box: \
	src/astral-box.o \
dnl	src/{util}.o \
	$(CC) $(CFLAGS) -o $@ \
	src/astral-box.o \
dnl	src/{util}.o \
	$(LDFLAGS)

src/astral-box.o: src/astral-box.c
dnl {util}.o: src/{util}.c

dnl {util}: src/astral-box
dnl	ln -sf astral-box src/{util} 

dnl TODO use forloop to generate target for links

clean:
	@rm -f src/astral-box.o astral-box
dnl	@rm -f src/{util}.o src/{util}

dnl	@rm -f astral-box-*

check: check-all

check-all: \
	check-astral-box \
dnl 	check-{util} \

dnl check-{util}:
dnl 	@echo "Checking {util}..."
dnl 	@$(SHELL) test/{util}/check.sh

install:
	@mkdir -p $(BINDIR)
	@mkdir -p $(MANDIR)/man1
	@echo "Installing binaries..."
	@cp src/astral-box $(BINDIR)
dnl	@ln -sf astral-box $(BINDIR)/{util}
	@echo "Installing manpages..."
dnl	@cp doc/{util}.1 $(MANDIR)/man1
	@chmod 644 $(MANDIR)/man1/astral-box.1
