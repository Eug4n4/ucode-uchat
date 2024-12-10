CC = clang
CFLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic
SRCDIR = src
INCDIR = inc
OBJDIR = obj
LIBDIR = ../libs
GUIDIR = gui

CJSON = cjson
CJSON_DIR = $(LIBDIR)/$(CJSON)

OPENSSL = openssl
OPENSSL_DIR = $(LIBDIR)/$(OPENSSL)
OPENSSL_INCDIR = $(LIBDIR)/$(OPENSSL)/$(INCDIR)

COMMON_DIR = ../common
COMMON_SRCFILES = $(wildcard $(COMMON_DIR)/$(SRCDIR)/*.c)
COMMON_OBJFILES = $(patsubst %.c, $(COMMON_DIR)/$(OBJDIR)/%.o, $(notdir $(notdir $(COMMON_SRCFILES))))


COMMON_INCDIR = $(COMMON_DIR)/$(INCDIR)


GTK_INCFLAGS = -pthread -I/usr/include/gtk-3.0 -I/usr/include/at-spi2-atk/2.0 -I/usr/include/at-spi-2.0 -I/usr/include/dbus-1.0 -I/usr/lib/x86_64-linux-gnu/dbus-1.0/include -I/usr/include/gtk-3.0 -I/usr/include/gio-unix-2.0 -I/usr/include/cairo -I/usr/include/pango-1.0 -I/usr/include/fribidi -I/usr/include/harfbuzz -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/pixman-1 -I/usr/include/uuid -I/usr/include/freetype2 -I/usr/include/libpng16 -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/libmount -I/usr/include/blkid -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include
GTK_LDFLAGS = -lgtk-3 -lgdk-3 -lpangocairo-1.0 -lpango-1.0 -lharfbuzz -latk-1.0 -lcairo-gobject -lcairo -lgdk_pixbuf-2.0 -lgio-2.0 -lgobject-2.0 -lglib-2.0 -lX11 -lm


SRCFILES = $(wildcard $(SRCDIR)/*.c)
GUIFILES = $(wildcard $(GUIDIR)/*.c)
ALL_SRCFILES = $(SRCFILES) $(GUIFILES)
OBJFILES = $(patsubst %.c, $(OBJDIR)/%.o, $(notdir $(ALL_SRCFILES)))
INCFLAGS = -I$(INCDIR) -I$(CJSON_DIR) -I$(OPENSSL_INCDIR) -I$(COMMON_INCDIR) $(GTK_INCFLAGS)
LDFLAGS = -L$(CJSON_DIR) -L$(OPENSSL_DIR) -L$(CJSON_DIR) $(GTK_LDFLAGS)
LDLIBS = -lssl -lcrypto -lcjson -lpthread -ldl
BIN = uchat


all: $(BIN)


$(BIN): $(OBJFILES) $(COMMON_OBJFILES)
	$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS)


$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(INCFLAGS)

$(OBJDIR)/%.o: $(GUIDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(INCFLAGS)

$(COMMON_OBJFILES):


$(OBJDIR):
	@mkdir -p $@


clean:
	@rm -rf $(OBJDIR)
