; Test3_3.asm

.386
.model flat, stdcall

include msvcrt.inc
includelib msvcrt.lib

.data
    szText db 'Hello World!', 0

.code
start:
    invoke crt_printf, addr szText
    ret
end start
