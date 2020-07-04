SOURCE_PATH = /home/nick/Dev/chess/src

CFLAGS = -I$(SOURCE_PATH)

LFLAGS = -lraylib

SOURCE = \
src/main.c \
src/rules.c \
src/data.c \
src/ai.c \
src/utility.c \
src/GUI.c

debug:
	gcc $(CFLAGS) -g -o bin/debug/Chess.sh $(SOURCE) $(LFLAGS)

release:
	gcc $(CFLAGS) -O3 -o bin/release/Chess.sh $(SOURCE) $(LFLAGS)

.PHONY: test clean

test:
	cd bin/debug; \
	./Chess.sh

run:
	cd bin/release; \
	./Chess.sh

debug_test: debug test

release_run: release run

all: debug release

clean:
	rm -f bin/debug/Chess.sh
	rm -f bin/release/Chess.sh
