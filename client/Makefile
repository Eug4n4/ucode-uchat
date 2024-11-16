CC = clang
CFLAGS = -Wall -Wextra -g
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
BIN = client

all: $(BIN)

$(BIN): $(OBJFILES)
	$(CC) -o $@ $^ -I../$(CJSON_DIR) -L../$(CJSON_DIR) -L../$(OPENSSL_DIR) -l$(CJSON) -lssl -lcrypto -lpthread -ldl


$(OBJFILES): | $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $^ -o $@ -I$(INCDIR) -I../$(CJSON_DIR)  -I../$(OPENSSL_INCDIR)

$(OBJDIR):
	@mkdir $@


clean:
	@rm -rf $(OBJDIR)
