include ./Makefile.inc

SERVER_SOURCES=$(wildcard main.c pop3/*.c monitor/*c lib/args/*.c lib/buffer/*.c lib/parser/*.c lib/selector/*.c lib/stm/*.c)

SERVER_OBJECTS=$(SERVER_SOURCES:src/%.c=obj/%.o)

OUTPUT_FOLDER=./bin
OBJECTS_FOLDER=./obj

SERVER_OUTPUT_FILE=$(OUTPUT_FOLDER)/pop3server

all: server

server: $(SERVER_OUTPUT_FILE)

$(SERVER_OUTPUT_FILE): $(SERVER_OBJECTS)
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(LDFLAGS) $(SERVER_OBJECTS) -o $(SERVER_OUTPUT_FILE)


obj/%.o: src/%.c
	mkdir -p $(@D)
	$(CC) $(GCCFLAGS) -c $< -o $@


clean:
	rm -rf $(OUTPUT_FOLDER)
	rm -rf $(OBJECTS_FOLDER)


check:
	mkdir -p check
	cppcheck --quiet --enable=all --force --inconclusive . 2> ./check/cppout.txt

	pvs-studio-analyzer trace -- make
	pvs-studio-analyzer analyze
	plog-converter -a '64:1,2,3;GA:1,2,3;OP:1,2,3' -t tasklist -o ./check/report.tasks ./PVS-Studio.log

	rm PVS-Studio.log
	mv strace_out check

.PHONY: all server clean check