.386
.model flat, stdcall
option casemap :none

include     windows.inc
include     kernel32.inc
includelib     kernel32.lib
include     user32.inc
includelib     user32.lib
include     masm32.inc
includelib     masm32.lib
.data?
hStdout     HANDLE ?
iLength     DWORD ?
cWritten    DWORD ?

.data
szText     db     'Hello ASM!',0
szMessage db 'Error!',0

.code
_WinMain     proc
    invoke     GetStdHandle,STD_OUTPUT_HANDLE   
    .if        eax==INVALID_HANDLE_VALUE
            invoke     MessageBox,NULL,offset szMessage,offset szText,MB_OK
    .else
            mov     hStdout,eax
    .endif
    invoke     lstrlen,offset szText
    mov         iLength,eax
    invoke     WriteConsole,hStdout,offset szText,iLength,offset cWritten,NULL
   
    .if        eax==NULL
        invoke     MessageBox,NULL,offset szMessage,offset szText,MB_OK
    .endif
   
    ret
_WinMain     endp

start:
    call _WinMain
    invoke StdIn,NULL,NULL
    invoke ExitProcess,NULL
   
    end start