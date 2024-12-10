CC = clang
CJSON_OBJ = cJSON.o


CJSON_LIBNAME = libcjson
CJSON_STATIC = $(CJSON_LIBNAME).a

all: $(CJSON_STATIC)

$(CJSON_STATIC): $(CJSON_OBJ)
	ar rcs $@ $<


$(CJSON_OBJ): cJSON.c cJSON.h
	$(CC) -c cJSON.c -o $@

clean:
	@rm -f $(CJSON_OBJ)
