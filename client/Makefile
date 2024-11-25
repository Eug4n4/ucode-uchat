CC = clang
CFLAGS = -Wall -Wextra -Wpedantic -g
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

# GTK Flags
GTK_LDFLAGS = -L$(LIBDIR)/gtk
GTK_LDLIBS = -lgtk-3 -lgdk-3 -lpangocairo-1.0 -lpango-1.0 -lharfbuzz -latk-1.0 -lcairo-gobject -lcairo -lgdk_pixbuf-2.0 -lgio-2.0 -lgobject-2.0 -lglib-2.0
GTK_INCFLAGS = $(addprefix -I$(LIBDIR)/gtk/inc/, gtk-3.0 at-spi2-atk/2.0 at-spi-2.0 include cairo pango-1.0 harfbuzz fribidi atk-1.0 pixman-1 freetype2 gdk-pixbuf-2.0 libpng16 glib-2.0) $(addprefix -I/usr/include/, dbus-1.0 uuid x86_64-linux-gnu blkid libmount gio-unix-2.0)

# Source and Object Files
SRCFILES = $(wildcard $(SRCDIR)/*.c)
GUIFILES = $(wildcard $(GUIDIR)/*.c)
ALL_SRCFILES = $(SRCFILES) $(GUIFILES)
OBJFILES = $(patsubst %.c, $(OBJDIR)/%.o, $(notdir $(ALL_SRCFILES)))
INCFLAGS = -I$(INCDIR) -I$(CJSON_DIR) -I$(OPENSSL_INCDIR) -I$(COMMON_INCDIR) $(GTK_INCFLAGS)
LDFLAGS = -L$(CJSON_DIR) -L$(OPENSSL_DIR) -L$(CJSON_DIR) $(GTK_LDFLAGS)
LDLIBS = -lssl -lcrypto -lcjson -lpthread -ldl $(GTK_LDLIBS)
BIN = uchat

# Default target
all: $(BIN)

# Build the executable
$(BIN): $(OBJFILES) $(COMMON_OBJFILES)
	$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS)

# Compile each source file to an object file
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(INCFLAGS)

$(OBJDIR)/%.o: $(GUIDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(INCFLAGS)

$(COMMON_OBJFILES):


# Ensure the object directory exists
$(OBJDIR):
	@mkdir -p $@

# Clean build files
clean:
	@rm -rf $(OBJDIR)
