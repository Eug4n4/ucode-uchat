LIBS_DIR = libs

CLIENT = client_rule
CLIENT_DIR = client

SERVER_DIR = server
SERVER = server_rule

COMMON = common_rule
COMMON_DIR = common

SQLITE = sqlite
SQLITE_DIR = $(LIBS_DIR)/$(SQLITE)

CJSON = cjson
CJSON_DIR = $(LIBS_DIR)/$(CJSON)


all: $(CJSON) $(SQLITE) $(COMMON) $(SERVER) $(CLIENT)

$(CJSON):
	@make --no-print-directory -C $(CJSON_DIR)

$(SQLITE):
	@make --no-print-directory -C $(SQLITE_DIR)


$(CLIENT):
	@make --no-print-directory -C  $(CLIENT_DIR)

$(SERVER):
	@make --no-print-directory -C $(SERVER_DIR)

$(COMMON):
	@make --no-print-directory -C $(COMMON_DIR)

clean:
	@make --no-print-directory -C $(CJSON_DIR) clean
	@make --no-print-directory -C $(SQLITE_DIR) clean
	@make --no-print-directory -C $(SERVER_DIR) clean
	@make --no-print-directory -C $(CLIENT_DIR) clean
	@make --no-print-directory -C $(COMMON_DIR) clean


