CC        = gcc
AS        = nasm
CFLAGS    = -c -Wall -Wextra -std=c17 -g
ASFLAGS   = -f elf64 -w+all -w+error -g

C_OBJ     = sum_example.o sum_tests.o
ASM_OBJ   = sum.o
BINARIES  = sum_example sum_tests sum128b_tests sum_example_brute

all: $(BINARIES)

%.o: %.asm
	$(AS) $(ASFLAGS) -o $@ $^
%.o: %.c
	$(CC) $(CFLAGS) -o $@ $^

# Compilation of .o files
# https://www.gnu.org/software/make/manual/make.html#Static-Pattern
# $(addprefix $(BUILD_DIR)/,$(C_OBJ)): %.o: %.c
# $(addprefix $(BUILD_DIR)/,$(ASM_OBJ)): %.o: %.asm

sum_example.o: sum_example.c
sum_tests.o: sum_tests.c
sum.o: sum.asm
brute.o: brute.c

sum_example: sum_example.o sum.o
	gcc -z noexecstack -o $@ $^
sum_tests: sum_tests.o sum.o
	gcc -z noexecstack -o $@ $^

sum_example_brute: sum_example.o brute.o
	gcc -z noexecstack -o $@ $^


128bsum.o: 128bsum.asm
128bsum_tests.o: 128bsum_tests.c

128bsum_tests: 128bsum.o 128bsum_tests.o
	gcc -z noexecstack -o $@ $^


clean:
	rm -f $(C_OBJ) $(ASM_OBJ) $(BINARIES) *.o

.PHONY: clean all
