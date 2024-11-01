CC = clang

CFLAGS = -Wall -pthread

CLIENT_BIN = client/client
SERVER_BIN = server/server

CLIENT_SRC = client/client.c
SERVER_SRC = server/server.c

all: $(CLIENT_BIN) $(SERVER_BIN)

$(CLIENT_BIN): $(CLIENT_SRC)
	$(CC) $(CFLAGS) -o $(CLIENT_BIN) $(CLIENT_SRC)

$(SERVER_BIN): $(SERVER_SRC)
	$(CC) $(CFLAGS) -o $(SERVER_BIN) $(SERVER_SRC)

clean:
	rm -f $(CLIENT_BIN) $(SERVER_BIN)
