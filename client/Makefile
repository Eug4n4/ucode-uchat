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

# GTK Flags
GTK_FLAGS = `pkg-config --cflags gtk+-3.0`
GTK_LIBS = `pkg-config --libs gtk+-3.0`

# Source and Object Files
SRCFILES = $(wildcard $(SRCDIR)/*.c)
GUIFILES = $(wildcard $(GUIDIR)/*.c)
ALL_SRCFILES = $(SRCFILES) $(GUIFILES)
OBJFILES = $(patsubst %.c, $(OBJDIR)/%.o, $(notdir $(ALL_SRCFILES)))
INCFLAGS = -I$(INCDIR) -I$(CJSON_DIR) -I$(OPENSSL_INCDIR) $(GTK_FLAGS)
LDFLAGS = -L$(CJSON_DIR) -L$(OPENSSL_DIR) -L$(CJSON_DIR)
LDLIBS = -lssl -lcrypto -lcjson -lpthread -ldl $(GTK_LIBS)
BIN = uchat

# Default target
all: $(BIN)

# Build the executable
$(BIN): $(OBJFILES)
	$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS)

# Compile each source file to an object file
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(INCFLAGS)

$(OBJDIR)/%.o: $(GUIDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(INCFLAGS)

# Ensure the object directory exists
$(OBJDIR):
	@mkdir -p $@

# Clean build files
clean:
	@rm -rf $(OBJDIR) $(BIN)
