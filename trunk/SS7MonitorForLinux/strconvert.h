#ifndef _STR_CONVERT_H
#define _STR_CONVERT_H
char* strlwr(char* str);
char* strupr(char *str);
int convertStrMsgISUP_SM_RSC(char *tmpStr, char *DPC,char *OPC,char *CIC);
int convertStrMsgISUP_SM_GRS(char *tmpStr, char *DPC,char *OPC,char *CIC);
int convertStrMsgISUP_SM_BLO(char *tmpStr, char *DPC,char *OPC,char *CIC);
int convertStrMsgISUP_SM_CGB(char *tmpStr, char *DPC,char *OPC,char *CIC);
int convertStrMsgISUP_SM_CGU(char *tmpStr, char *DPC,char *OPC,char *CIC);
int convertStrMsgISUP_SM_UBL(char *tmpStr, char *DPC,char *OPC,char *CIC);


#endif
