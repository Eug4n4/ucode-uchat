CC = clang
CFLAGS = -Wall -Wextra -Wpedantic -g
SRCDIR = src
INCDIR = inc
OBJDIR = obj
LIBDIR = libs

CJSON = cjson
CJSON_DIR = $(LIBDIR)/$(CJSON)

SQLITE = sqlite
SQLITE_DIR = $(LIBDIR)/$(SQLITE)

OPENSSL = openssl
OPENSSL_DIR = $(LIBDIR)/$(OPENSSL)
OPENSSL_INCDIR = $(LIBDIR)/$(OPENSSL)/$(INCDIR)


SRCFILES = $(wildcard $(SRCDIR)/*.c)

DB_SRCFILES = $(wildcard db/*.c)
OBJFILES = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCFILES))
DB_OBJFILES = $(patsubst %.c, $(OBJDIR)/%.o, $(notdir $(DB_SRCFILES)))

LOG_SRCFILES = $(wildcard logging/*.c)
LOG_OBJFILES = $(patsubst %.c, $(OBJDIR)/%.o, $(notdir $(LOG_SRCFILES)))

INCFLAGS = -I$(INCDIR) -I../$(CJSON_DIR) -I../$(SQLITE_DIR) -I../$(OPENSSL_INCDIR)
LDFLAGS = -L../$(CJSON_DIR) -L../$(SQLITE_DIR) -L../$(OPENSSL_DIR)
LDLIBS = -l$(CJSON) -l$(SQLITE) -lssl -lcrypto -lpthread -ldl

BIN = uchat_server

all: $(BIN)


$(BIN): $(OBJFILES) $(DB_OBJFILES) $(LOG_OBJFILES)
	$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS)

$(OBJFILES): | $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $^ -o $@ $(INCFLAGS)

$(OBJDIR)/%.o: db/%.c
	$(CC) $(CFLAGS) -c $^ -o $@ $(INCFLAGS)

$(OBJDIR)/%.o: logging/%.c
	$(CC) $(CFLAGS) -c $^ -o $@ $(INCFLAGS)

$(OBJDIR):
	@mkdir $@

clean:
	@rm -rf $(OBJDIR)
