SOURCE_PATH = /home/nick/Dev/chess/src

CFLAGS = -I$(SOURCE_PATH)

SOURCE = \
src/main.c \
src/rules.c \
src/data.c \
src/ai.c \
src/utility.c

debug:
	gcc $(CFLAGS) -D DEBUG_LOGGING -o bin/debug/Chess.sh $(SOURCE)

release:
	gcc $(CFLAGS) -O3 -o bin/release/Chess.sh $(SOURCE)

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
