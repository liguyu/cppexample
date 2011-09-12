// program77.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <windows.h>

using namespace std;

class C;
C* g_pC = NULL;

typedef void(*pFUN)();

#pragma pack(push,1)
// 存储机器代码的结构
struct Thunk
{
    BYTE    m_jmp;          // jmp指令的操作码	
    DWORD   m_relproc;      // 相对jmp	
};
#pragma pack(pop)

class C
{
public:
    Thunk    m_thunk;
	
    void Init(pFUN pFun, void* pThis)
    {
		// 跳转指令的操作码
        m_thunk.m_jmp = 0xe9;
        // 相应函数的地址
        m_thunk.m_relproc = (int)pFun - ((int)this+sizeof(Thunk));
		//Flushes the instruction cache for the specified process.
        FlushInstructionCache(GetCurrentProcess(), &m_thunk, sizeof(m_thunk));
    }
	
    // 这是回调函数
    static void CallBackFun()
    {
        C* pC = g_pC;
		
        // 初始化thunk
        pC->Init(StaticFun, pC);
		
        // 获得thunk代码地址
        pFUN pFun = (pFUN)&(pC->m_thunk);
		
        // 开始执行thunk代码，调用StaticFun
        pFun();
		
        cout << "C::CallBackFun" << endl;
    }
	
    static void StaticFun()
    {
        cout << "C::StaticFun" << endl;
    }
};

int main()
{
    C objC;
    g_pC = &objC;
    C::CallBackFun();
    return 0;
}
