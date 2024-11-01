CC = clang

CFLAGS = -Wall -pthread

LIBS_DIR = libs

CLIENT_BIN = client/client
SERVER_BIN = server/server

CLIENT_SRC = client/client.c
SERVER_SRC = server/server.c

CJSON = cjson
CJSON_DIR = $(LIBS_DIR)/$(CJSON)

all: $(CJSON) $(CLIENT_BIN) $(SERVER_BIN)

$(CJSON):
	make -C $(CJSON_DIR)

$(CLIENT_BIN): $(CLIENT_SRC)
	$(CC) $(CFLAGS) -o $(CLIENT_BIN) $(CLIENT_SRC) -I$(CSJON_DIR) -L$(CJSON_DIR) -l$(CJSON)

$(SERVER_BIN): $(SERVER_SRC)
	$(CC) $(CFLAGS) -o $(SERVER_BIN) $(SERVER_SRC) -I$(CJSON_DIR) -L$(CJSON_DIR) -l$(CJSON)

clean:
	rm -f $(CLIENT_BIN) $(SERVER_BIN)
