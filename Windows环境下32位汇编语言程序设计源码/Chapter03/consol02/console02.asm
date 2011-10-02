;----------------------------------------------------------------
; console02.asm
; 第一种Win32汇编控制台输出方式 API
;----------------------------------------------------------------

.386
.model flat, stdcall
option casemap:none

include windows.inc
include kernel32.inc
includelib kernel32.lib

.data
    szText db 'Hello World!', 0

;定义两个 DWORD 类型的变量, 分别是用于输出句柄和字符串长度
.data?
    hOut dd ?
    len  dd ?

.code
start:
    ; 获取控制台输出设备的句柄, 其返回值会放在 eax 寄存器
    invoke GetStdHandle, STD_OUTPUT_HANDLE
    ; 把获取到的句柄给变量 hOut 
    mov hOut, eax
    ; 通过 lstrlen 函数获取字符串长度, 返回值在 eax                        
    invoke lstrlen, addr szText      
    ; 把获取到的字符串长度给变量 len      
    mov len, eax
    ; 输出到控制台, 参数分别是: 句柄、字符串地址、字符串长度; 后面是两个指针暂用不到                           
    invoke WriteFile, hOut, addr szText, len, NULL, NULL
    ret
end start

; 另外前面用到的 StdOut 也基本就是这样实现的, 源码在: masm32\m32lib\stdout.asm