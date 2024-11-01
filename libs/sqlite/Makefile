CC = clang
OBJ = sqlite3.o
LIB = libsqlite.a

all: $(LIB)

$(LIB): $(OBJ)
	ar rcs $@ $^

$(OBJ): sqlite3.c sqlite3.h
	$(CC) -c sqlite3.c -o $@

clean:
	rm -f $(OBJ)

