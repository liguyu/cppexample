// SampleDLL.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#define EXPORTING_DLL
#include "SampleDLL.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

void HelloWorld()
{
	MessageBox( NULL, TEXT("Hello World!"), TEXT("This is My First DLL Example"), MB_OK);
}
