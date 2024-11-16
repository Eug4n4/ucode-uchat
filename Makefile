LIBS_DIR = libs

CLIENT = client_rule
CLIENT_DIR = client

SERVER_DIR = server
SERVER = server_rule

SQLITE = sqlite
SQLITE_DIR = $(LIBS_DIR)/$(SQLITE)

CJSON = cjson
CJSON_DIR = $(LIBS_DIR)/$(CJSON)


all: $(CJSON) $(SQLITE) $(SERVER) $(CLIENT)

$(CJSON):
	make -C $(CJSON_DIR)

$(SQLITE):
	make -C $(SQLITE_DIR)


$(CLIENT):
	make -C $(CLIENT_DIR)

$(SERVER):
	make -C $(SERVER_DIR)

clean:
	make -C $(CJSON_DIR) clean
	make -C $(SQLITE_DIR) clean
	make -C $(SERVER_DIR) clean
	make -C $(CLIENT_DIR) clean

