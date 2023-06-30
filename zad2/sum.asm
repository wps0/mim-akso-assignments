extern sum

section .text

; Parameters:
;  %1, %2 - upper and lower part of the first number
;  %3, %4 - upper and lower part of the second number
%macro add_128b 4
    add %2, %4
    adc %1, %3
%endmacro

; Parameters:
;  %1 - the upper part of the number
;  %2 - the lower part of the number
; Modifies: %1, %2
%macro shift_128b 2
    shrd %2, %1, 1
    sar %1, 1
%%shift_end:
%endmacro

; Calculates floor(64 * i * i / n) and stores the result in rax.
; Parameters:
;  rax - i
;  %1 - n
; Modifies: rax, rdx
%macro calc_shift 1
    mul rax                         ; rax = i^2
    shl rax, 6                      ; rax = 2^6 * i^2

    ; rdx is set to 0 by mul (under the given constraints,
    ;  rdx <= 2^64)
    div %1                          ; rax = 2^6 * i^2 / n
%endmacro

sum:
    xor r11, r11                    ; r11 - loop counter
    xor r8, r8                      ; r9, r8 - buffer; lower bits are in r8
    xor r9, r9

    jmp .loop_condition

; Loops through the values in x array. At the beginning of each iteration,
;  the buffer is shifted by the "current shift".
.loop:
    xor rax, rax
    xchg rax, [rdi+r11*8]            ; loads the next x[i] and sets x[i] to 0

    cqo                              ; extends a 64b number stored in rax to 128b
    add_128b r9, r8, rdx, rax

    mov rax, r11
    calc_shift rsi
    mov r10, rax                    ; r10 - shift_loop counter

    inc r11                         ; increasing the loop counter now
                                    ;  saves instructions later
    jmp .shift_loop_condition

; saves, in x[], the bits from the buffer that won't change
.shift_loop:
    test r8b, 0x1
    jz .shift_loop_end

    mov rax, r10
    cqo                             ; sets rdx to 0 by extending a positive int from rax
    mov ecx, 64
    div rcx                         ; x[rax] - the memory into which to insert the bit
                                    ;    rdx - the offset in x[rax]

    bts [rdi+rax*8], rdx

.shift_loop_end:
    shift_128b r9, r8
    inc r10

.shift_loop_condition:
    mov rax, r11
    calc_shift rsi
    
    cmp rax, r10
    jne .shift_loop

.loop_condition:
    cmp r11, rsi
    jb .loop

    ret
