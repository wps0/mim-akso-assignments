CC        = gcc
AS        = nasm
CFLAGS    = -c -Wall -Wextra -std=c17 -g
ASFLAGS   = -f elf64 -w+all -w+error -g

C_OBJ     = 
ASM_OBJ   = scopy.o
BINARIES  = scopy

all: $(BINARIES)

%.o: %.asm
	$(AS) $(ASFLAGS) -o $@ $^
%.o: %.c
	$(CC) $(CFLAGS) -o $@ $^

# Compilation of .o files
# https://www.gnu.org/software/make/manual/make.html#Static-Pattern
# $(addprefix $(BUILD_DIR)/,$(C_OBJ)): %.o: %.c
# $(addprefix $(BUILD_DIR)/,$(ASM_OBJ)): %.o: %.asm

scopy.o: scopy.asm

scopy: scopy.o
	ld --fatal-warnings -o scopy $^

# sum_example: sum_example.o sum.o
# 	gcc -z noexecstack -o $@ $^

clean:
	rm -f $(C_OBJ) $(ASM_OBJ) $(BINARIES) *.o

.PHONY: clean all
