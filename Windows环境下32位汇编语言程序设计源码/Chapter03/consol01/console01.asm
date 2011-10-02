;----------------------------------------------------------------
; console01.asm
; 第一种Win32汇编控制台输出方式 StdOut
;----------------------------------------------------------------
.386
.model flat, stdcall

include		masm32.inc
include		kernel32.inc
includelib	masm32.lib
includelib	kernel32.lib

.data
	szText db "Hello World!", 0

.code
start:
	invoke StdOut, addr szText
	ret				;ret 是用于子程序返回的指令, 这里用它代替 ExitProcess(在没有生成 Win32 窗口时是可以的)
end start