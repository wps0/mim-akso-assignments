global _start

section .rodata

SYS_OPEN             equ 2
SYS_CLOSE            equ 3
SYS_READ             equ 0
SYS_WRITE            equ 1
SYS_EXIT             equ 60

O_RDONLY             equ 0
O_WRONLY             equ 1
O_CREAT		         equ 64

ERR_CONST            equ 0xfffffffffffff000                     ; Syscall errors are greater than this value.
WRITE_MASK           equ 644o                                   ; Permissions given to newly-created files.
READ_MASK            equ 400o

READ_BUFFER_SIZE     equ 262144
WRITE_BUFFER_SIZE    equ 131072                                 ; Has to be greater than 3.
OPENED_FILES_COUNT   equ 2
FILE_NOT_OPENED      equ -1

section .data

opened_files: times OPENED_FILES_COUNT dq FILE_NOT_OPENED       ; Descriptors of opened files.

section .bss

read_buffer: resb READ_BUFFER_SIZE
write_buffer: resb WRITE_BUFFER_SIZE

section .text

; Parameters:
;  %1 - Filename.
;  %2 - Flags.
;  %3 - Mode.
;  %4 - The index in opened_files table.
; Returns:
;  rax - A file descriptor.
%macro open_file 4
    mov rdi, %1
    mov rsi, %2
    mov rdx, %3

    mov rax, SYS_OPEN
    syscall
    mov [opened_files + %4*8], rax                              ; Store the descriptor in the list of files
                                                                ;  to be closed.
%endmacro

; Parameters:
;  %1 - A register in which there's an error code.
;  %2 - A label to which to jump.
%macro jmp_on_error 2
    cmp %1, ERR_CONST
    ja %2
%endmacro

_start:
    push rbp
    lea rbp, [rsp+8]
    push r15                                                    ; Program status.

    mov rcx, qword [rbp]                                        ; Checks if argument count is equal to 3.
    cmp rcx, 0x3
    jne .exit_error

    open_file [rbp+2*8], O_RDONLY, READ_MASK, 0x0               ; Opens the input file.
    jmp_on_error rax, .exit_error

    open_file [rbp+3*8], O_WRONLY | O_CREAT, WRITE_MASK, 0x1    ; Opens the output file.
    jmp_on_error rax, .exit_error

    call perform_scopy                                          ; Performs the scopy procedure.
    test rax, rax
    jne .exit_error

.exit_success:
    xor r15, r15
    jmp .exit_close_files

.exit_error:
    mov r15, 1
    jmp .exit_close_files
    
; Closes the files on exit.
.exit_close_files:
    lea rcx, [OPENED_FILES_COUNT-1]

.exit_close_files_loop:
    mov r9, [opened_files+rcx*8]
    cmp r9, ERR_CONST                                           ; Makes sure the file has been opened.
    ja .exit_close_files_condition

    push rcx
    mov rdi, r9
    mov rax, SYS_CLOSE
    syscall
    pop rcx

    cmp rax, ERR_CONST
    jb .exit_close_files_condition                              ; Everything went ok.

    mov r15, 1                                                  ; Sys_close failed.

.exit_close_files_condition:
    dec rcx
    js .exit
    jmp .exit_close_files_loop
    
.exit:
    mov rax, SYS_EXIT
    mov rdi, r15

    pop r15
    pop rbp

    syscall

; Parameters: none
; Returns:
;  rax - 0 on success, 1 otherwise.
; Modifies: rax, rsi, rdi, rdx, r10, r8, r9
perform_scopy:
    push r15                                                    ; r15 - The number of consecutive bytes
                                                                ;  that do not contain 's' or 'S'.
    push r14                                                    ; r14 - The last memory address to be read.
    push r13                                                    ; r13 - Write buffer offset.
    push r12                                                    ; r12 - Temporary register.

    cld

.loop_reading:
    mov rax, SYS_READ
    mov rdi, [opened_files]
    mov rsi, read_buffer
    mov rdx, READ_BUFFER_SIZE
    syscall
    jmp_on_error rax, .exit_error

    or rax, rax                                                 ; If 0 bytes were read, EOF was reached.
    jz .exit_success

    mov rsi, read_buffer                                        ; Setup rsi for lodsb.
    mov r14, rsi
    add r14, rax

.loop_processing:
    lodsb

    cmp al, 's'
    je .write_char
    cmp al, 'S'
    je .write_char

    inc r15

.loop_processing_condition:
    cmp rsi, r14
    jb .loop_processing
    jmp .loop_reading

.write_char:
    mov r12, rsi
    call write_char
    test rax, rax
    jnz .exit_error
    mov rsi, r12

    jmp .loop_processing_condition

.exit_success:
    xor rax, rax
    call write_char
    test rax, rax
    jz .exit

.exit_error:
    mov rax, 1

.exit:
    pop r12
    pop r13
    pop r14
    pop r15
    ret

; DOES NOT FOLLOW ABI!
; Writes <a maximum consecutive sequence of bytes that do not
;  contain 's' or 'S'><'s' or 'S'> to the buffer and flushes
;  it, if necessary.
; Parameters:
;  al  - The character to be written. If al != 's' && al != 'S', then only
;         r15w will be written (if it is non-zero) and then the buffer will be flushed.
;  r15 - The number of consecutive bytes that do not contain 's' or 'S'.
;  r13 - Write buffer offset.
; Returns:
;  rax - 0 on success, 1 otherwise.
; Modifies: rax, rsi, rdi, rdx, r10, r9, r8, r13, r15
write_char:
    push r12

    test r15, r15                                               ; Ignores empty consecutive sequences
    jz .jmp_to_write_al                                         ;  without 's' or 'S'.

    mov [write_buffer+r13], r15w                                ; Writes r15w.
    add r13, 2

.jmp_to_write_al:
    cmp al, 's'
    je .write_al
    cmp al, 'S'
    je .write_al

    jmp .before_flush                                           ; write_char was called to write the length
                                                                ;  of the last suffix.

.write_al:
    mov [write_buffer+r13], al
    inc r13

    cmp r13, WRITE_BUFFER_SIZE-3                                ; Determines if the buffer needs flushing.
    jb .end_success

.before_flush:
    xor r12, r12                                                ; r12 - the length of the prefix of the buffer
                                                                ;  that was flushed.

.flush:
    mov rax, SYS_WRITE
    mov rdi, [opened_files+8]
    lea rsi, [write_buffer+r12]
    mov rdx, r13
    syscall
    jmp_on_error rax, .end

    add r12, rax                                                ; In case not all bytes were written to the file,
    sub r13, rax                                                ;  loops until writing succeeds.
    jnz .flush

    xor r13, r13                                                ; Resets the write buffer offset.

.end_success:
    xor rax, rax
    jmp .end

.end_error:
    push 1
    pop rax

.end:
    xor r15, r15                                                ; r15 was saved to the buffer in .write_position
    pop r12
    ret
