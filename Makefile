# build a program

#CC = gcc
CC = clang
#CFLAGS = -g -O3 -fsanitize=address -fsanitize=memory -fsanitize=cfi
CFLAGS = -g -O3

SRC=./src
SRC_TESTS=$(SRC)/tests
SRC_UTILS=$(SRC)/utils
SRC_SEGS=$(SRC)/segments

default_target: program
.PHONY : default_target

#####################################################################################
# program
BIN_PROGRAM=./bin/powerlinec
program/build: clean_program
	mkdir -p ./bin
	$(CC) $(CFLAGS) -o $(BIN_PROGRAM) \
		$(SRC)/*.c \
		$(SRC_SEGS)/*.c \
		$(SRC_UTILS)/*.c
.PHONY: program/build

program: program/build
.PHONY: program

test_program: program/build
	chmod +x $(BIN_PROGRAM)
	time $(BIN_PROGRAM) ./config.conf
.PHONY: test_program

#####################################################################################
# hashtable
BIN_HASHTABLE=./bin/test/hashtable
test_hashtable/build: clean_hashtable
	mkdir -p ./bin/test
	$(CC) $(CFLAGS) -o $(BIN_HASHTABLE) \
		$(SRC_TESTS)/test_hashtable.c \
		$(SRC_UTILS)/hashtable.c \
		$(SRC_UTILS)/string.c \
		$(SRC_UTILS)/list.c
.PHONY: test_hashtable/build

test_hashtable: test_hashtable/build
	chmod +x $(BIN_HASHTABLE)
	$(BIN_HASHTABLE)
.PHONY: test_hashtable

#####################################################################################
# list
BIN_LIST=./bin/test/list
test_list/build: clean_list
	mkdir -p ./bin/test
	$(CC) $(CFLAGS) -o $(BIN_LIST) \
		$(SRC_TESTS)/test_list.c \
		$(SRC_UTILS)/list.c \
		$(SRC_UTILS)/string.c
.PHONY: test_list/build

test_list: test_list/build
	chmod +x $(BIN_LIST)
	$(BIN_LIST)
.PHONY: test_list

#####################################################################################
# string
BIN_STRING=./bin/test/string
test_string/build: clean_string
	mkdir -p ./bin/test
	$(CC) $(CFLAGS) -o $(BIN_STRING) \
		$(SRC_TESTS)/test_string.c \
		$(SRC_UTILS)/list.c \
		$(SRC_UTILS)/string.c
.PHONY: test_string/build

test_string: test_string/build
	chmod +x $(BIN_STRING)
	$(BIN_STRING)
.PHONY: test_string

#####################################################################################
# gits status
BIN_GIT_STATUS=./bin/test/git_status
test_git/build: clean_git
	mkdir -p ./bin/test
	$(CC) $(CFLAGS) -o $(BIN_GIT_STATUS) \
		$(SRC_TESTS)/test_git_status.c \
		$(SRC_UTILS)/list.c \
		$(SRC_UTILS)/string.c \
		$(SRC_UTILS)/file.c \
		$(SRC_UTILS)/git_status.c
.PHONY: test_git/build

test_git: test_git/build
	chmod +x $(BIN_GIT_STATUS)
	$(BIN_GIT_STATUS)
.PHONY: test_git

#####################################################################################
# config
BIN_CONFIG=./bin/test/config
TEST_CONF=/tmp/test_config.conf
test_config/build: clean_config
	mkdir -p ./bin/test
	$(CC) $(CFLAGS) -o $(BIN_CONFIG) \
		$(SRC_TESTS)/test_config.c \
		$(SRC)/config.c \
		$(SRC_UTILS)/*.c
.PHONY: test_config/build

test_config: test_config/build
	chmod +x $(BIN_CONFIG)
	rm -f $(TEST_CONF)
	$(BIN_CONFIG) $(TEST_CONF)
	rm -f $(TEST_CONF)
.PHONY: test_config

#####################################################################################
# segment
BIN_SEGMENT=./bin/test/segment
test_segment/build: clean_segment
	mkdir -p ./bin/test
	$(CC) $(CFLAGS) -o $(BIN_SEGMENT) \
		$(SRC_TESTS)/test_segment.c \
		$(SRC)/config.c \
		$(SRC_SEGS)/*.c \
		$(SRC_UTILS)/*.c
.PHONY: test_segment/build

test_segment: test_segment/build
	chmod +x $(BIN_SEGMENT)
	$(BIN_SEGMENT) ./config.conf
.PHONY: test_segment

#####################################################################################
# tests
test: clean
	make test_list
	make test_string
	make test_hashtable
	make test_config
	make test_git
	make test_segment
	make test_program
.PHONY: test

#####################################################################################
PREFIX=/usr/local
# install
install: program
	rm -f $(PREFIX)/bin/powerlinec
	rm -f $(PREFIX)/etc/powerlinec.conf
	mkdir -p $(PREFIX)/bin
	mkdir -p $(PREFIX)/etc
	cp -f $(BIN_PROGRAM) $(PREFIX)/bin/
	cp -f ./config.conf $(PREFIX)/etc/powerlinec.conf
	chmod +x $(PREFIX)/bin/powerlinec
.PHONY: install

#####################################################################################
# uninstall
uninstall:
	rm -f $(PREFIX)/bin/powerlinec
	rm -f $(PREFIX)/etc/powerlinec.conf
.PHONY: uninstall


#####################################################################################
# clean
clean_program:
	rm -f $(BIN_PROGRAM)
.PHONY: clean_program

clean_list:
	rm -f $(BIN_LIST)
.PHONY: clean_list

clean_string:
	rm -f $(BIN_STRING)
.PHONY: clean_string

clean_git:
	rm -f $(BIN_GIT_STATUS)
.PHONY: clean_git

clean_hashtable:
	rm -f $(BIN_HASHTABLE)
.PHONY: clean_hashtable

clean_config:
	rm -f $(BIN_CONFIG)
.PHONY: clean_config

clean_segment:
	rm -f $(BIN_SEGMENT)
.PHONY: clean_segment

clean: clean_program \
		clean_list \
		clean_string \
		clean_git \
		clean_hashtable \
		clean_config \
		clean_segment
.PHONY: clean

#####################################################################################
# help
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... program"
	@echo "... test"
	@echo "... test_program"
	@echo "... test_list"
	@echo "... test_string"
	@echo "... test_git"
	@echo "... test_hashtable"
	@echo "... test_config"
	@echo "... test_segment"
	@echo "... install"
	@echo "... uninstall"
	@echo "... clean"
	@echo "... help"
.PHONY : help
