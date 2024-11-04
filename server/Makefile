CC = clang
CFLAGS = -Wall -Wextra
SRCDIR = src
INCDIR = inc
OBJDIR = obj
LIBDIR = libs
CJSON = cjson
CJSON_DIR = $(LIBDIR)/$(CJSON)
SQLITE = sqlite
SQLITE_DIR = $(LIBDIR)/$(SQLITE)
SRCFILES = $(wildcard $(SRCDIR)/*.c)
DB_SRCFILES = $(wildcard db/*.c)
OBJFILES = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCFILES))
DB_OBJFILES = $(patsubst %.c, $(OBJDIR)/%.o, $(notdir $(DB_SRCFILES)))
BIN = server

all: $(BIN)


$(BIN): $(OBJFILES) $(DB_OBJFILES)
	$(CC) -o $@ $^ -I../$(CJSON_DIR) -I../$(SQLITE_DIR) -L../$(CJSON_DIR) -L../$(SQLITE_DIR) -l$(CJSON) -l$(SQLITE)

$(OBJFILES): | $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $^ -o $@ -I$(INCDIR) -I../$(LIBDIR)/$(CJSON) -I../$(LIBDIR)/$(SQLITE)

$(OBJDIR)/%.o: db/%.c
	$(CC) $(CFLAGS) -c $^ -o $@ -I$(INCDIR) -I../$(LIBDIR)/$(CJSON) -I../$(LIBDIR)/$(SQLITE)

$(OBJDIR):
	mkdir $@

clean:
	rm -rf $(OBJDIR)
