#ifndef _SYSCONFIG_H_
#define _SYSCONFIG_H_

#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"

#define MAX_PREFIX   30
#define MAX_SEC      30
#define MAX_FILE_NAME_LEN 256



int     My_IpPoolItem(int iItemCnt);
void    My_DeleteIpPoolItem(int iItemCnt);
int     My_IpPool(int iItemCnt);
void    My_DeleteIpPool(int iItemCnt);
int     My_DigitGrpItem(int  iItemCnt);
void    My_DeleteDigitGrpItem(int iItemCnt);
void    My_DeleteAllMem();
void    My_DeleteRout();
int     SplitStr2Int(DJ_S8* str, DJ_S8* sep, int buf[]);
void    ReadVOIPConfig(void);
void	InitTextBox(void);
BOOL    IsATrkMod(DJ_S8 s8ModID);
BOOL    IsValidIP(char* IPAddr);


#endif