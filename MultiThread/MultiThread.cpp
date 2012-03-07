#include <windows.h>
#include <iostream.h>

DWORD WINAPI Fun1Proc(  LPVOID lpParameter  );
DWORD WINAPI Fun2Proc(  LPVOID lpParameter  );

int index=0;
int tickets=100;
HANDLE hMutex;

void main()
{
	HANDLE hThread1;
	HANDLE hThread2;
	hMutex=CreateMutex(NULL,0,"tickets");
	if(hMutex)
	{
		if(ERROR_ALREADY_EXISTS==GetLastError())
		{
			cout<<"only instance can run!"<<endl;
			return;
		}
	}
	hThread1=CreateThread(NULL,0,Fun1Proc,NULL,0,NULL);
	hThread2=CreateThread(NULL,0,Fun2Proc,NULL,0,NULL);
	CloseHandle(hThread1);
	CloseHandle(hThread2);
	/*while(index++<1000)
		cout<<"main thread is running"<<endl;*/
//	hMutex=CreateMutex(NULL,0,"tickets");


// 	WaitForSingleObject(hMutex,INFINITE);
// 	ReleaseMutex(hMutex);
// 	ReleaseMutex(hMutex);
	Sleep(10000);
//	Sleep(10);
}

DWORD WINAPI Fun1Proc(  LPVOID lpParameter )
{
	/*while(index++<1000)
		cout<<"thread1 is running"<<endl;*/
	
	while(TRUE)
	{
		//ReleaseMutex(hMutex);
		WaitForSingleObject(hMutex,INFINITE);
		if(tickets>0)
		{
			Sleep(1000);
			cout<<"thread1 sell ticket : "<<tickets--<<endl;
		}
		ReleaseMutex(hMutex);
	}

// 	WaitForSingleObject(hMutex,INFINITE);
// 	cout<<"thread1 is running"<<endl;
	return 0;
}

DWORD WINAPI Fun2Proc(  LPVOID lpParameter  )
{
	
	while(TRUE)
	{
		//ReleaseMutex(hMutex);
		WaitForSingleObject(hMutex,INFINITE);
		if(tickets>0)
		{
			Sleep(1000);
			cout<<"thread2 sell ticket : "<<tickets--<<endl;
		}
		ReleaseMutex(hMutex);
	}
// 	WaitForSingleObject(hMutex,INFINITE);
// 	cout<<"thread2 is running"<<endl;
	return 0;
}