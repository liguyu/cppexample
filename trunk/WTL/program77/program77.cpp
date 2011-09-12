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
// �洢��������Ľṹ
struct Thunk
{
    BYTE    m_jmp;          // jmpָ��Ĳ�����	
    DWORD   m_relproc;      // ���jmp	
};
#pragma pack(pop)

class C
{
public:
    Thunk    m_thunk;
	
    void Init(pFUN pFun, void* pThis)
    {
		// ��תָ��Ĳ�����
        m_thunk.m_jmp = 0xe9;
        // ��Ӧ�����ĵ�ַ
        m_thunk.m_relproc = (int)pFun - ((int)this+sizeof(Thunk));
		//Flushes the instruction cache for the specified process.
        FlushInstructionCache(GetCurrentProcess(), &m_thunk, sizeof(m_thunk));
    }
	
    // ���ǻص�����
    static void CallBackFun()
    {
        C* pC = g_pC;
		
        // ��ʼ��thunk
        pC->Init(StaticFun, pC);
		
        // ���thunk�����ַ
        pFUN pFun = (pFUN)&(pC->m_thunk);
		
        // ��ʼִ��thunk���룬����StaticFun
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
