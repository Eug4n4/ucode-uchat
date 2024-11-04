CC = clang

CFLAGS = -Wall -pthread

LIBS_DIR = libs

CLIENT_BIN = client/client
SERVER_BIN = server/server

CLIENT_SRC = client/client.c
#SERVER_SRC = server/server.c

SERVER_DIR = server
SERVER = server_rule
SQLITE = sqlite
SQLITE_DIR = $(LIBS_DIR)/$(SQLITE)

CJSON = cjson
CJSON_DIR = $(LIBS_DIR)/$(CJSON)

all: $(CJSON) $(SQLITE) $(SERVER) $(CLIENT_BIN)

$(CJSON):
	make -C $(CJSON_DIR)

$(SQLITE):
	make -C $(SQLITE_DIR)

$(CLIENT_BIN): $(CLIENT_SRC)
	$(CC) $(CFLAGS) -o $(CLIENT_BIN) $(CLIENT_SRC) -I$(CJSON_DIR) -L$(CJSON_DIR) -l$(CJSON)

$(SERVER):
	make -C $(SERVER_DIR)

clean:
	make -C $(CJSON_DIR) clean
	make -C $(SQLITE_DIR) clean
	make -C $(SERVER_DIR) clean


