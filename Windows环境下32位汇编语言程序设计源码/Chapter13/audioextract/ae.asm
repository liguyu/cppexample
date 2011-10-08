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
szArg1		db	128 dup(?)
szArg2		db	128 dup(?)
szSWFFileName	db	128 dup(?)
szOutput	db	1024 dup (?)
szBuffer	db	1024 dup (?)

.const
szErrorStr1	db	0dh,0ah,'Error, arguments number is wrong!',0
szErrorStr2	db	0dh,0ah,'Error, file not found!',0
szComExample	db	0dh,0ah,'The right command line format:',0dh,0ah,'    ae 1.swf 1.mp3',0dh,0ah,0
szFormat1	db	'arg[1]£º%s',0dh,0ah,'arg[2]£º%s',0dh,0ah,0

.code
include		_Cmdline.asm
start:
	invoke	_argc						;count the command line arg
	.if (eax != 3)	
		invoke StdOut, offset szErrorStr1
		jmp inputerror
	.endif

	invoke	ArgClC, 1, offset szArg1			;place arg1 into the buffer szArg1
	invoke	ArgClC, 2, offset szArg2			;place arg1 into the buffer szArg2

	invoke	GetPathOnly, offset szArg1,  offset szBuffer
	invoke	StrLen, offset szBuffer
	.if (eax == 0 )						;only file name
		invoke	GetAppPath, offset szSWFFileName		
	.endif
	
	invoke  szCatStr, offset szSWFFileName,  offset szArg1
	invoke	exist,offset szSWFFileName			;the swf file is exist?
	.if (eax == 0 )
		invoke StdOut, offset szSWFFileName		;swf file absolute path
		invoke StdOut, offset szErrorStr2
		jmp inputerror
	.endif	

	jmp inputpass
	
inputerror:	
	invoke  StdOut, offset szComExample
	invoke	ExitProcess,NULL

inputpass:
	invoke	wsprintf,addr szOutput,addr szFormat1,addr szArg1,addr szArg2
	invoke	StdOut, offset szOutput
	invoke	ExitProcess,NULL

end start