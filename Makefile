
# GENERICS
ROOT_DIR := $(ROOT)

CC = gcc

CFLAGS = -Wall -I/usr/include -I./src/include $(EXTRA_CFLAGS)
LDFLAGS = -I/usr/include $(EXTRA_CFLAGS) -lSDL2 -lm -lSDL2_ttf

ALL_SRC := $(wildcard src/*.c) $(wildcard src/*/*.c)
ALL_OBJ := $(addprefix tmp/,$(patsubst src/%.c,%.o,$(ALL_SRC)))
APP_OBJ := $(addprefix tmp/,$(patsubst src/%.c,%.o,$(ALL_SRC)))

dir_guard=@mkdir -p $(@D)

debug:
	@echo "######################"
	@echo "# MAKING DEBUG BUILD #"
	@echo "######################"
	make dingus CFLAGS="-DDEBUG -g"

production:
	@echo "###########################"
	@echo "# MAKING PRODUCTION BUILD #"
	@echo "###########################"
	make clean
	make dingus CFLAGS="-O3"
	strip -s dingus

dingus: $(APP_OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

.intermediate:
	@touch $@

tmp/%.o: src/%.c
	$(dir_guard)
	$(CC) $(CFLAGS) -c -o $@ $<
	
clean:
	rm -rf dingus tmp/*

dbg:
	@echo $(ALL_SRC)
