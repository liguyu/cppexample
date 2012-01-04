#include <stdio.h>
//#include <tchar.h>
#include <windows.h>

char comList[256]={0};
void GetComList() 
{
	char strCom[10];
	int nCom = 0;
	int count = 0;
	HANDLE hCom;
	do {
		nCom++;
		sprintf(strCom,"COM%d", nCom);
		hCom = CreateFile(strCom, 0, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (INVALID_HANDLE_VALUE == hCom)
		{
			break;
		}
		printf("%s\n",strCom);
		CloseHandle(hCom);
	} while (1);
} 
int main(void)
{
	printf("Valid Com List:\n");
	GetComList() ;
	return 0;
}