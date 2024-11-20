CC = clang
CFLAGS = -Wall -Wextra -Wpedantic -g
SRCDIR = src
INCDIR = inc
OBJDIR = obj
LIBDIR = libs

CJSON = cjson
CJSON_DIR = $(LIBDIR)/$(CJSON)

OPENSSL = openssl
OPENSSL_DIR = $(LIBDIR)/$(OPENSSL)
OPENSSL_INCDIR = $(LIBDIR)/$(OPENSSL)/$(INCDIR)

SRCFILES = $(wildcard $(SRCDIR)/*.c)
OBJFILES = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCFILES))
INCFLAGS = -I$(INCDIR) -I../$(CJSON_DIR) -I../$(OPENSSL_INCDIR)
LDFLAGS = -L../$(CJSON_DIR) -L../$(OPENSSL_DIR)
LDLIBS = -l$(CJSON) -lssl -lcrypto -lpthread -ldl
BIN = uchat

all: $(BIN)

$(BIN): $(OBJFILES)
	$(CC) -o $@ $^ $(INCFLAGS) $(LDFLAGS) $(LDLIBS)


$(OBJFILES): | $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $^ -o $@ $(INCFLAGS)

$(OBJDIR):
	@mkdir $@


clean:
	@rm -rf $(OBJDIR)
