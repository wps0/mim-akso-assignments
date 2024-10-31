CC        = gcc
CFLAGS    = -Wall -Wextra -Wno-implicit-fallthrough -std=gnu17 -O2 -fPIC #-fsanitize=address,undefined

LMALLOCF  = -Wl,--wrap=malloc -Wl,--wrap=calloc -Wl,--wrap=realloc -Wl,--wrap=reallocarray \
            -Wl,--wrap=free -Wl,--wrap=strdup -Wl,--wrap=strndup
OBJECTS   = trie.o groups.o utils.o seq.o groups_tests.o trie_tests.o memory_tests.o \
            seq_example.o
BINARIES  = seq_example groups_tests trie_tests
LIBS      = libseq.so libseq.a
OTHEROBJ  = seq.tar.gz
BUILD_DIR = out

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

# Compilation of .o files
# https://www.gnu.org/software/make/manual/make.html#Static-Pattern
$(OBJECTS): %.o: %.c

# Create the build directory if it doesn't exist
$(OBJECTS): | $(BUILD_DIR)

all: debug libseq.so libseq.a seq.tar.gz
debug: seq_example groups_tests trie_tests

libseq.so: seq.o trie.o groups.o utils.o memory_tests.o
	gcc -shared $(LMALLOCF) -o $@ $^

libseq.a: seq.o trie.o groups.o utils.o memory_tests.o
	ar -rcs $@ $^

# Tests
trie_tests: trie_tests.o trie.o groups.o utils.o
groups_tests: groups_tests.o groups.o utils.o
seq_example: seq_example.o libseq.so
	gcc -L. -o $@ $^ -lseq

seq.tar.gz:
	tar -czvf $@ Makefile seq.c trie.c trie.h groups.c groups.h utils.c utils.h


$(BUILD_DIR):
	mkdir $(BUILD_DIR)

clean:
	rm -f $(addprefix $(BUILD_DIR)/,$(OBJECTS)) $(OBJECTS)
	rm -f $(BINARIES) $(LIBS) $(OTHEROBJ)
	-rmdir $(BUILD_DIR)

.PHONY: all debug clean
