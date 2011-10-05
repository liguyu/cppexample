;----------------------------------------------------------------
; ae.asm
; audio extract tool
;----------------------------------------------------------------
.386
.model flat, stdcall
option casemap:none

include		windows.inc
include		masm32.inc
include		kernel32.inc
include		user32.inc
includelib	user32.lib
includelib	masm32.lib
includelib	kernel32.lib

.data?
szFileName1	db	128 dup(?)
szFileName2	db	128 dup(?)
szOutput	db	1024 dup (?)

.const
szErrorStr	db	0dh,0ah,'Input Error! The right command line format:',0dh,0ah,'    ae example.swf example.mp3',0dh,0ah,0
szFormat2	db	'可执行文件名称：',0dh,0ah,'%s',0dh,0ah,0ah
		db	'参数总数：%d',0dh,0ah,0
szFormat1	db	'arg[1]：%s',0dh,0ah,'arg[2]：%s',0dh,0ah,0

.code
include		_Cmdline.asm
start:
	invoke	_argc						;count the command line arg
	.if (eax != 3)						
		invoke StdOut, offset szErrorStr
		jmp finish
	.endif

	invoke	ArgClC, 1, offset szFileName1			;place arg1 into the buffer szFileName1
	invoke	ArgClC, 2, offset szFileName2			;place arg1 into the buffer szFileName2

	invoke	wsprintf,addr szOutput,addr szFormat1,addr szFileName1,addr szFileName2
	invoke	StdOut, offset szOutput
	
finish:	
	invoke	ExitProcess,NULL

end start