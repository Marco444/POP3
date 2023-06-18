CFLAGS=-g -Wall -pthread -Wextra -pedantic -pedantic-errors -O3 -std=c11 -D_POSIX_C_SOURCE=200112L -D__BSD_VISIBLE=1 -Wno-unused-parameter -Wno-implicit-fallthrough  -fsanitize=address

LDFLAGS=

LIB_HEADERS=$(wildcard lib/*/*.h)

SERVER_SOURCES=$(wildcard lib/logger/*.c lib/sockets/*.c *.c pop3/*.c pop3/commands/*.c pop3/commands/monitor/transaction/*.c    pop3/commands/monitor/authorization/*.c   pop3/commands/server/authorization/*.c  pop3/commands/server/update/*.c  pop3/commands/server/transaction/*.c pop3/states/monitor/*.c pop3/states/*.c pop3/states/server/*.c pop3/new_connection/*.c  ) $(wildcard lib/args/*.c lib/buffer/*.c lib/parser/*.c lib/selector/*.c lib/stm/*.c lib/metrics/*.c) 

CLIENT_SOURCES=$(wildcard client/*.c lib/client/*.c)

SERVER_OBJECTS=$(SERVER_SOURCES:src/%.c=obj/%.o)
CLIENT_OBJECTS=$(CLIENT_SOURCES:src/%.c=obj/%.o)

OUTPUT_FOLDER=./bin
OBJECTS_FOLDER=./obj

SERVER_OUTPUT_FILE=$(OUTPUT_FOLDER)/server
CLIENT_OUTPUT_FILE=$(OUTPUT_FOLDER)/client

all: server client

server: $(SERVER_OUTPUT_FILE)

client: $(CLIENT_OUTPUT_FILE)

$(SERVER_OUTPUT_FILE):
	mkdir -p $(@D)
	$(CC) $(CFLAGS)  $(LDFLAGS) $(SERVER_OBJECTS) -o $(SERVER_OUTPUT_FILE)

$(CLIENT_OUTPUT_FILE):
	mkdir -p $(@D)
	$(CC) $(CFLAGS)  $(LDFLAGS) $(CLIENT_OBJECTS) -o $(CLIENT_OUTPUT_FILE)

clean:
	rm -rf $(OUTPUT_FOLDER)
	rm -rf $(OBJECTS_FOLDER)

.PHONY: all server client clean
