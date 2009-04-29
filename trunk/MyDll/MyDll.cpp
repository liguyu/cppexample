
void CALLBACK TimerProc(HWND hWnd,UINT nMsg,UINT nTimerid,DWORD dwTime)
{
	
}

My_SetESR((EsrFunc) *TimerProc)
{
	int count = 0;
	DWORD dwThreadID = 0;
	HANDLE hThread;
	
	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ThreadFunc, NULL, 0, &dwThreadID);  
	
	if (hThread != NULL)
	{		
		CloseHandle(hThread);
	}
}

DWORD WINAPI ThreadFunc(VOID) 
{ 
	SetTimer(1,1000,TimerProc);
	return 0; 
}  