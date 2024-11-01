CC = clang
CJSON_OBJ = cJSON.o
PIC_FLAGS = -fPIC
R_CFLAGS = $(PIC_FLAGS) -pedantic -Wall -Werror -Wstrict-prototypes -Wwrite-strings -Wshadow -Winit-self \
						-Wcast-align -Wformat=2 -Wmissing-prototypes -Wstrict-overflow=2 -Wcast-qual \
						-Wc++-compat -Wundef -Wswitch-default -Wconversion \

CJSON_LIBNAME = libcjson
CJSON_STATIC = $(CJSON_LIBNAME).a

all: $(CJSON_STATIC)

$(CJSON_STATIC): $(CJSON_OBJ)
	ar rcs $@ $<


$(CJSON_OBJ): cJSON.c cJSON.h
	$(CC) -std=c89 -c $(R_CFLAGS) cJSON.c -o $@

clean:
	rm -f $(CJSON_OBJ)
