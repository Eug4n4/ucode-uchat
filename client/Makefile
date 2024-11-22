CC = clang
CFLAGS = -Wall -Wextra -g
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

BIN = client

# Default target
all: $(BIN)

# Build the executable
$(BIN): $(OBJFILES)
	$(CC) -o $@ $^ -I$(CJSON_DIR) -L$(CJSON_DIR) -L$(OPENSSL_DIR) $(CJSON_DIR)/libcjson.a -lssl -lcrypto -lpthread -ldl $(GTK_LIBS)

# Compile each source file to an object file
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -I$(INCDIR) -I$(CJSON_DIR) -I$(OPENSSL_INCDIR) $(GTK_FLAGS)

$(OBJDIR)/%.o: $(GUIDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -I$(INCDIR) -I$(CJSON_DIR) -I$(OPENSSL_INCDIR) $(GTK_FLAGS)

# Ensure the object directory exists
$(OBJDIR):
	@mkdir -p $@

# Clean build files
clean:
	@rm -rf $(OBJDIR) $(BIN)
