;----------------------------------------------------------------
; console01.asm
; ��һ��Win32������̨�����ʽ StdOut
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
	ret				;ret �������ӳ��򷵻ص�ָ��, ������������ ExitProcess(��û������ Win32 ����ʱ�ǿ��Ե�)
end start