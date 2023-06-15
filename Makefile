

CFLAGS=-g -Wall -Wextra -pedantic -pedantic-errors -O3 -std=c11 -D_POSIX_C_SOURCE=200112L -D__BSD_VISIBLE=1 -Wno-unused-parameter -Wno-implicit-fallthrough  -fsanitize=address

LDFLAGS=

LIB_HEADERS=$(wildcard lib/*/*.h)

SOURCES=$(wildcard *.c pop3/*.c pop3/commands/*.c pop3/commands/monitor/transaction/*.c    pop3/commands/monitor/authorization/*.c   pop3/commands/server/authorization/*.c  pop3/commands/server/update/*.c  pop3/commands/server/transaction/*.c pop3/states/monitor/*.c pop3/states/*.c pop3/states/server/*.c pop3/new_connection/*.c  ) $(wildcard lib/args/*.c lib/buffer/*.c lib/parser/*.c lib/selector/*.c lib/stm/*.c lib/metrics/*.c) 

OBJECTS=$(SOURCES:src/%.c=obj/%.o)

OUTPUT_FOLDER=./bin
OBJECTS_FOLDER=./obj

OUTPUT_FILE=$(OUTPUT_FOLDER)/main

all: server

server: $(OUTPUT_FILE)

$(OUTPUT_FILE):
	mkdir -p $(@D)
	$(CC) $(CFLAGS)  $(LDFLAGS) $(OBJECTS) -o $(OUTPUT_FILE)


clean:
	rm -rf $(OUTPUT_FOLDER)
	rm -rf $(OBJECTS_FOLDER)

.PHONY: all server clean
