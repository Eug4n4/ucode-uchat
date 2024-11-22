CC = clang
CFLAGS = -Wall -Wextra -Wpedantic
LIBDIR = libs
INCDIR = inc
SRCDIR = src
OBJDIR = obj
SERVER_INCDIR = ../server/$(INCDIR)

CJSON = cjson
CJSON_DIR = $(LIBDIR)/$(CJSON)

SQLITE = sqlite
SQLITE_DIR = $(LIBDIR)/$(SQLITE)

OPENSSL = openssl
OPENSSL_DIR = $(LIBDIR)/$(OPENSSL)
OPENSSL_INCDIR = $(LIBDIR)/$(OPENSSL)/$(INCDIR)

SRCFILES = $(wildcard $(SRCDIR)/*.c)
OBJFILES = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCFILES))

INCFLAGS = -I$(INCDIR) -I$(SERVER_INCDIR) -I../$(CJSON_DIR) -I../$(SQLITE_DIR) -I../$(OPENSSL_DIR)

all: $(OBJFILES)
	

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $^ -o $@ $(INCFLAGS)


$(OBJDIR):
	@mkdir $@


clean:
	@rm -rf $(OBJDIR)