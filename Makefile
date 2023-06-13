include ./Makefile.inc

LIB_SOURCES=$(wildcard lib/args/*.c lib/buffer/*.c lib/parser/*.c lib/selector/*.c lib/stm/*.c lib/metrics/*.c)
MAIN_SOURCES=$(wildcard main.c pop3/*.c pop3/commands/*.c  pop3/commands/authorization/*.c  pop3/commands/update/*.c  pop3/commands/transaction/*.c pop3/states/*.c pop3/new_connection/*.c  )
TEST_SOURCES=testingMain.c

LIB_OBJECTS=$(LIB_SOURCES:%.c=obj/%.o)
MAIN_OBJECTS=$(MAIN_SOURCES:%.c=obj/%.o)
TEST_OBJECTS=$(TEST_SOURCES:%.c=obj/%.o)

OUTPUT_FOLDER=./bin
OBJECTS_FOLDER=./obj

MAIN_OUTPUT_FILE=$(OUTPUT_FOLDER)/main
TEST_OUTPUT_FILE=$(OUTPUT_FOLDER)/testingMain

all: main test

main: $(MAIN_OUTPUT_FILE)
test: $(TEST_OUTPUT_FILE)

$(MAIN_OUTPUT_FILE): $(LIB_OBJECTS) $(MAIN_OBJECTS)
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIB_OBJECTS) $(MAIN_OBJECTS) -o $(MAIN_OUTPUT_FILE)

$(TEST_OUTPUT_FILE): $(LIB_OBJECTS) $(TEST_OBJECTS)
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIB_OBJECTS) $(TEST_OBJECTS) -o $(TEST_OUTPUT_FILE)

obj/%.o: %.c
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

.PHONY: all main test clean check

