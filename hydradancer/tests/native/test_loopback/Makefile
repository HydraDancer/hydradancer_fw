VERSION = "0.1.0 06-Aug-2023"

# Install paths
PREFIX = /usr/local

# Flags
CPPFLAGS = -DVERSION=\"$(VERSION)\"

ifeq ($(OS), Windows_NT)
CFLAGS = -Wall -O2 `pkg-config --cflags libusb-1.0` $(INCLUDES)
LDFLAGS = -L/mingw64/lib -I/mingw64/include/libusb-1.0 -lusb-1.0
else
CFLAGS = -Wall -O2 `pkg-config --cflags libusb-1.0` $(INCLUDES)
LDFLAGS = `pkg-config --libs libusb-1.0`
endif

LIBUSB_PORTABLE_DIR = ../libusb_portable
CARGS_PORTABLE_DIR = ../../../../submodules/cargs

SRC = test_loopback.c $(LIBUSB_PORTABLE_DIR)/libusb_portable.c $(CARGS_PORTABLE_DIR)/src/cargs.c
HDR = $(LIBUSB_PORTABLE_DIR)/libusb_portable.h $(CARGS_PORTABLE_DIR)/include/cargs.h
OBJ = $(SRC:.c=.o)
BIN = test_loopback
DISTFILES = $(SRC) $(HDR) Makefile


INCLUDES = \
  -I$(LIBUSB_PORTABLE_DIR)\
  -I$(CARGS_PORTABLE_DIR)/include

all: $(BIN)

$(OBJ): $(SRC)

$(BIN): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

install:
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f $(BIN) $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin/$(BIN)

uninstall:
	rm -vf $(DESTDIR)$(PREFIX)/bin/$(BIN)

dist:
	mkdir -p $(BIN)-$(VERSION)
	cp $(DISTFILES) $(BIN)-$(VERSION)
	tar -cf $(BIN)-$(VERSION).tar $(BIN)-$(VERSION)
	gzip $(BIN)-$(VERSION).tar
	rm -rf $(BIN)-$(VERSION)

clean:
	rm -f $(OBJ) $(BIN)

.PHONY: all install uninstall dist clean
