#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include "strconvert.h"

char* strlwr(char* str)   
{   
  char* orig = str;
  for ( ; *str != '\0'; str++)   
     *str   =   tolower(*str);   
  return   orig;   
}  

char* strupr(char *str)
{
	char* orig = str;
	for (; *str != '\0'; str++)
	  *str = toupper(*str);
	return orig;
}


int convertStrMsgISUP_SM_BLO(char *tmpStr, char *DPC,char *OPC,char *CIC){
	int pos = 0;
	int i = 0;
	char tmpChar;
	unsigned int i1 = 0;
	unsigned int i2 = 0;
	char temp[3] = {0};
	int len = 100;
	
	if (6 == (int)strlen(DPC) && 6 == (int)strlen(OPC) && 4 == (int)strlen(CIC))
	{
		tmpStr[pos++] = '8';
		tmpStr[pos++] = '5';
		
		tmpStr[pos++] = DPC[4];
		tmpStr[pos++] = DPC[5];
		tmpStr[pos++] = DPC[2];
		tmpStr[pos++] = DPC[3];
		tmpStr[pos++] = DPC[0];
		tmpStr[pos++] = DPC[1];
		
		tmpStr[pos++] = OPC[4];
		tmpStr[pos++] = OPC[5];
		tmpStr[pos++] = OPC[2];
		tmpStr[pos++] = OPC[3];
		tmpStr[pos++] = OPC[0];
		tmpStr[pos++] = OPC[1];
		
		//SLS
		tmpStr[pos++] = '0';
		tmpStr[pos++] = CIC[1];
					
	}else if (4 == (int)strlen(DPC) && 4 == (int)strlen(OPC) && 4 == (int)strlen(CIC))
	{
		tmpStr[pos++] = 'C';
		tmpStr[pos++] = '5';
		
		tmpStr[pos++] = DPC[2];
		tmpStr[pos++] = DPC[3];
			
        memcpy(temp, DPC, 2);
        sscanf(temp, "%x", &i1);
		i1 = i1 & 63;
		//printf("i1: %x\n",i1);
		
		memcpy(temp, OPC+2, 2);
        sscanf(temp, "%x", &i2);
		//printf("i2: %x\n",i2);
		i2 = i2<<6;	
		//printf("i2: %x\n",i2);

		i1 = i1|i2;
		//printf("i1: %x\n",i1);
		i2 = i1&0x00f0;
		sprintf( &tmpChar,"%x", i2);
		tmpStr[pos++] = tmpChar;

		i2 = i1&0x000f;
		sprintf( &tmpChar,"%x", i2);
		tmpStr[pos++] = tmpChar;		
			
		memcpy(temp, OPC, 4);
        sscanf(temp, "%x", &i1);
		i1 = i1>>2;
		i2 = i1&0x00f0;
		sprintf( &tmpChar,"%x", i2);
		tmpStr[pos++] = tmpChar;		
		
		i2 = i1&0x000f;
		sprintf( &tmpChar,"%x", i2);
		tmpStr[pos++] = tmpChar;

		tmpStr[pos++] = CIC[1];

		i2 = i1&0x0f00;
		sprintf( &tmpChar,"%x", i2);
		//printf("tmpChar: %c\n",tmpChar);
		tmpStr[pos++] = tmpChar;		

	}else{
		//printf("input format error");
		return 0;
	}	
	//CIC
	for (i = 0; i < (int)strlen(CIC); i ++ )
	{
		tmpStr[pos++] = CIC[i];
	}
	tmpStr[pos++] = '1';
	tmpStr[pos++] = '3';	
	tmpStr[pos++] = '\0';	

	return pos;
}

int convertStrMsgISUP_SM_RSC(char *tmpStr, char *DPC,char *OPC,char *CIC){
	int pos = 0;
	int i = 0;
	char tmpChar;
	unsigned int i1 = 0;
	unsigned int i2 = 0;
	char temp[3] = {0};
	int len = 100;
	
	if (6 == (int)strlen(DPC) && 6 == (int)strlen(OPC) && 4 == (int)strlen(CIC))
	{
		tmpStr[pos++] = '8';
		tmpStr[pos++] = '5';
		
		tmpStr[pos++] = DPC[4];
		tmpStr[pos++] = DPC[5];
		tmpStr[pos++] = DPC[2];
		tmpStr[pos++] = DPC[3];
		tmpStr[pos++] = DPC[0];
		tmpStr[pos++] = DPC[1];
		
		tmpStr[pos++] = OPC[4];
		tmpStr[pos++] = OPC[5];
		tmpStr[pos++] = OPC[2];
		tmpStr[pos++] = OPC[3];
		tmpStr[pos++] = OPC[0];
		tmpStr[pos++] = OPC[1];
		
		//SLS
		tmpStr[pos++] = '0';
		tmpStr[pos++] = CIC[1];
				
	}else if (4 == (int)strlen(DPC) && 4 == (int)strlen(OPC) && 4 == (int)strlen(CIC))
	{
		tmpStr[pos++] = 'C';
		tmpStr[pos++] = '5';
		
		tmpStr[pos++] = DPC[2];
		tmpStr[pos++] = DPC[3];
			
        memcpy(temp, DPC, 2);
        sscanf(temp, "%x", &i1);
		i1 = i1 & 63;
		//printf("i1: %x\n",i1);
		
		memcpy(temp, OPC+2, 2);
        sscanf(temp, "%x", &i2);
		//printf("i2: %x\n",i2);
		i2 = i2<<6;	
		//printf("i2: %x\n",i2);

		i1 = i1|i2;
		//printf("i1: %x\n",i1);
		i2 = i1&0x00f0;
		sprintf( &tmpChar,"%x", i2);
		tmpStr[pos++] = tmpChar;

		i2 = i1&0x000f;
		sprintf( &tmpChar,"%x", i2);
		tmpStr[pos++] = tmpChar;
		
			
		memcpy(temp, OPC, 4);
        sscanf(temp, "%x", &i1);
		i1 = i1>>2;
		i2 = i1&0x00f0;
		sprintf( &tmpChar,"%x", i2);
		tmpStr[pos++] = tmpChar;	
		
		i2 = i1&0x000f;
		sprintf( &tmpChar,"%x", i2);
		tmpStr[pos++] = tmpChar;

		tmpStr[pos++] = CIC[1];

		i2 = i1&0x0f00;
		sprintf( &tmpChar,"%x", i2);
		//printf("tmpChar: %c\n",tmpChar);
		tmpStr[pos++] = tmpChar;
	}else{
		//printf("input format error");
		return 0;
	}	

	//CIC
	for (i = 0; i < (int)strlen(CIC); i ++ )
	{
		tmpStr[pos++] = CIC[i];
	}
	tmpStr[pos++] = '1';
	tmpStr[pos++] = '2';	
	tmpStr[pos++] = '\0';
	return pos;
}

int convertStrMsgISUP_SM_UBL(char *tmpStr, char *DPC,char *OPC,char *CIC){
	int pos = 0;
	int i = 0;
	char tmpChar;
	unsigned int i1 = 0;
	unsigned int i2 = 0;
	char temp[3] = {0};
	int len = 100;
	
	if (6 == (int)strlen(DPC) && 6 == (int)strlen(OPC) && 4 == (int)strlen(CIC))
	{
		tmpStr[pos++] = '8';
		tmpStr[pos++] = '5';
		
		tmpStr[pos++] = DPC[4];
		tmpStr[pos++] = DPC[5];
		tmpStr[pos++] = DPC[2];
		tmpStr[pos++] = DPC[3];
		tmpStr[pos++] = DPC[0];
		tmpStr[pos++] = DPC[1];
		
		tmpStr[pos++] = OPC[4];
		tmpStr[pos++] = OPC[5];
		tmpStr[pos++] = OPC[2];
		tmpStr[pos++] = OPC[3];
		tmpStr[pos++] = OPC[0];
		tmpStr[pos++] = OPC[1];
		
		//SLS
		tmpStr[pos++] = '0';
		tmpStr[pos++] = CIC[1];
		
	}else if (4 == (int)strlen(DPC) && 4 == (int)strlen(OPC) && 4 == (int)strlen(CIC))
	{
		tmpStr[pos++] = 'C';
		tmpStr[pos++] = '5';
		
		tmpStr[pos++] = DPC[2];
		tmpStr[pos++] = DPC[3];
		
        memcpy(temp, DPC, 2);
        sscanf(temp, "%x", &i1);
		i1 = i1 & 63;
		//printf("i1: %x\n",i1);
		
		memcpy(temp, OPC+2, 2);
        sscanf(temp, "%x", &i2);
		//printf("i2: %x\n",i2);
		i2 = i2<<6;	
		//printf("i2: %x\n",i2);
		
		i1 = i1|i2;
		//printf("i1: %x\n",i1);
		i2 = i1&0x00f0;
		sprintf( &tmpChar,"%x", i2);
		tmpStr[pos++] = tmpChar;
		
		i2 = i1&0x000f;
		sprintf( &tmpChar,"%x", i2);
		tmpStr[pos++] = tmpChar;
		
		
		memcpy(temp, OPC, 4);
        sscanf(temp, "%x", &i1);
		i1 = i1>>2;
		i2 = i1&0x00f0;
		sprintf( &tmpChar,"%x", i2);
		tmpStr[pos++] = tmpChar;	
		
		i2 = i1&0x000f;
		sprintf( &tmpChar,"%x", i2);
		tmpStr[pos++] = tmpChar;
		
		tmpStr[pos++] = CIC[1];
		
		i2 = i1&0x0f00;
		sprintf( &tmpChar,"%x", i2);
		//printf("tmpChar: %c\n",tmpChar);
		tmpStr[pos++] = tmpChar;		
		
	}else{
		//printf("input format error");
		return 0;
	}	
	
	//CIC
	for (i = 0; i < (int)strlen(CIC); i ++ )
	{
		tmpStr[pos++] = CIC[i];
	}
	tmpStr[pos++] = '1';
	tmpStr[pos++] = '4';	
	tmpStr[pos++] = '\0';
	return pos;
}

int convertStrMsgISUP_SM_CGB(char *tmpStr, char *DPC,char *OPC,char *CIC){
	int pos = 0;
	int i = 0;
	char tmpChar;
	unsigned int i1 = 0;
	unsigned int i2 = 0;
	char temp[3] = {0};
	int len = 100;
	
	if (6 == (int)strlen(DPC) && 6 == (int)strlen(OPC) && 4 == (int)strlen(CIC))
	{
		tmpStr[pos++] = '8';
		tmpStr[pos++] = '5';
		
		tmpStr[pos++] = DPC[4];
		tmpStr[pos++] = DPC[5];
		tmpStr[pos++] = DPC[2];
		tmpStr[pos++] = DPC[3];
		tmpStr[pos++] = DPC[0];
		tmpStr[pos++] = DPC[1];
		
		tmpStr[pos++] = OPC[4];
		tmpStr[pos++] = OPC[5];
		tmpStr[pos++] = OPC[2];
		tmpStr[pos++] = OPC[3];
		tmpStr[pos++] = OPC[0];
		tmpStr[pos++] = OPC[1];
		
		//SLS
		tmpStr[pos++] = '0';
		tmpStr[pos++] = CIC[1];
		
	}else if (4 == (int)strlen(DPC) && 4 == (int)strlen(OPC) && 4 == (int)strlen(CIC))
	{
		tmpStr[pos++] = 'C';
		tmpStr[pos++] = '5';
		
		tmpStr[pos++] = DPC[2];
		tmpStr[pos++] = DPC[3];
		
        memcpy(temp, DPC, 2);
        sscanf(temp, "%x", &i1);
		i1 = i1 & 63;
		//printf("i1: %x\n",i1);
		
		memcpy(temp, OPC+2, 2);
        sscanf(temp, "%x", &i2);
		//printf("i2: %x\n",i2);
		i2 = i2<<6;	
		//printf("i2: %x\n",i2);
		
		i1 = i1|i2;
		//printf("i1: %x\n",i1);
		i2 = i1&0x00f0;
		sprintf( &tmpChar,"%x", i2);
		tmpStr[pos++] = tmpChar;
		
		i2 = i1&0x000f;
		sprintf( &tmpChar,"%x", i2);
		tmpStr[pos++] = tmpChar;		
		
		memcpy(temp, OPC, 4);
        sscanf(temp, "%x", &i1);
		i1 = i1>>2;
		i2 = i1&0x00f0;
		sprintf( &tmpChar,"%x", i2);
		tmpStr[pos++] = tmpChar;		
		
		i2 = i1&0x000f;
		sprintf( &tmpChar,"%x", i2);
		tmpStr[pos++] = tmpChar;
		
		tmpStr[pos++] = CIC[1];
		
		i2 = i1&0x0f00;
		sprintf( &tmpChar,"%x", i2);
		//printf("tmpChar: %c\n",tmpChar);
		tmpStr[pos++] = tmpChar;		
		
	}else{
		//printf("input format error");
		return 0;
	}	
	
	//CIC
	for (i = 0; i < (int)strlen(CIC); i ++ )
	{
		tmpStr[pos++] = CIC[i];
	}
	tmpStr[pos++] = '1';
	tmpStr[pos++] = '8';
	
	tmpStr[pos++] = '0';
	tmpStr[pos++] = '0';
	tmpStr[pos++] = '0';
	tmpStr[pos++] = '1';
	
	tmpStr[pos++] = '0';
	tmpStr[pos++] = '3';
	tmpStr[pos++] = '0';
	tmpStr[pos++] = 'e';
	tmpStr[pos++] = 'f';
	tmpStr[pos++] = 'f';
	tmpStr[pos++] = '7';
	tmpStr[pos++] = 'f';
	tmpStr[pos++] = '\0';
	
	return pos;
}
int convertStrMsgISUP_SM_CGU(char *tmpStr, char *DPC,char *OPC,char *CIC){
	int pos = 0;
	int i = 0;
	char tmpChar;
	unsigned int i1 = 0;
	unsigned int i2 = 0;
	char temp[3] = {0};
	int len = 100;
	
	if (6 == (int)strlen(DPC) && 6 == (int)strlen(OPC) && 4 == (int)strlen(CIC))
	{
		tmpStr[pos++] = '8';
		tmpStr[pos++] = '5';
		
		tmpStr[pos++] = DPC[4];
		tmpStr[pos++] = DPC[5];
		tmpStr[pos++] = DPC[2];
		tmpStr[pos++] = DPC[3];
		tmpStr[pos++] = DPC[0];
		tmpStr[pos++] = DPC[1];
		
		tmpStr[pos++] = OPC[4];
		tmpStr[pos++] = OPC[5];
		tmpStr[pos++] = OPC[2];
		tmpStr[pos++] = OPC[3];
		tmpStr[pos++] = OPC[0];
		tmpStr[pos++] = OPC[1];
		
		//SLS
		tmpStr[pos++] = '0';
		tmpStr[pos++] = CIC[1];
		
	}else if (4 == (int)strlen(DPC) && 4 == (int)strlen(OPC) && 4 == (int)strlen(CIC))
	{
		tmpStr[pos++] = 'C';
		tmpStr[pos++] = '5';
		
		tmpStr[pos++] = DPC[2];
		tmpStr[pos++] = DPC[3];
		
        memcpy(temp, DPC, 2);
        sscanf(temp, "%x", &i1);
		i1 = i1 & 63;
		//printf("i1: %x\n",i1);
		
		memcpy(temp, OPC+2, 2);
        sscanf(temp, "%x", &i2);
		//printf("i2: %x\n",i2);
		i2 = i2<<6;	
		//printf("i2: %x\n",i2);
		
		i1 = i1|i2;
		//printf("i1: %x\n",i1);
		i2 = i1&0x00f0;
		sprintf( &tmpChar,"%x", i2);
		tmpStr[pos++] = tmpChar;
		
		i2 = i1&0x000f;
		sprintf( &tmpChar,"%x", i2);
		tmpStr[pos++] = tmpChar;
		
		
		memcpy(temp, OPC, 4);
        sscanf(temp, "%x", &i1);
		i1 = i1>>2;
		i2 = i1&0x00f0;
		sprintf( &tmpChar,"%x", i2);
		tmpStr[pos++] = tmpChar;
		
		
		i2 = i1&0x000f;
		sprintf( &tmpChar,"%x", i2);
		tmpStr[pos++] = tmpChar;
		
		tmpStr[pos++] = CIC[1];
		
		i2 = i1&0x0f00;
		sprintf( &tmpChar,"%x", i2);
		//printf("tmpChar: %c\n",tmpChar);
		tmpStr[pos++] = tmpChar;		
		
	}else{
		//printf("input format error");
		return 0;
	}	
	
	//CIC
	for (i = 0; i < (int)strlen(CIC); i ++ )
	{
		tmpStr[pos++] = CIC[i];
	}
	tmpStr[pos++] = '1';
	tmpStr[pos++] = '9';
	tmpStr[pos++] = '0';
	tmpStr[pos++] = '0';
	tmpStr[pos++] = '0';
	tmpStr[pos++] = '1';
	
	tmpStr[pos++] = '0';
	tmpStr[pos++] = '3';
	tmpStr[pos++] = '0';
	tmpStr[pos++] = 'e';
	tmpStr[pos++] = 'f';
	tmpStr[pos++] = 'f';
	tmpStr[pos++] = '7';
	tmpStr[pos++] = 'f';
	tmpStr[pos++] = '\0';
	
	return pos;
}
int convertStrMsgISUP_SM_GRS(char *tmpStr, char *DPC,char *OPC,char *CIC){
	int pos = 0;
	int i = 0;
	char tmpChar;
	unsigned int i1 = 0;
	unsigned int i2 = 0;
	char temp[3] = {0};
	int len = 100;
	
	if (6 == (int)strlen(DPC) && 6 == (int)strlen(OPC) && 4 == (int)strlen(CIC))
	{
		tmpStr[pos++] = '8';
		tmpStr[pos++] = '5';
		
		tmpStr[pos++] = DPC[4];
		tmpStr[pos++] = DPC[5];
		tmpStr[pos++] = DPC[2];
		tmpStr[pos++] = DPC[3];
		tmpStr[pos++] = DPC[0];
		tmpStr[pos++] = DPC[1];
		
		tmpStr[pos++] = OPC[4];
		tmpStr[pos++] = OPC[5];
		tmpStr[pos++] = OPC[2];
		tmpStr[pos++] = OPC[3];
		tmpStr[pos++] = OPC[0];
		tmpStr[pos++] = OPC[1];
		
		//SLS
		tmpStr[pos++] = '0';
		tmpStr[pos++] = CIC[1];
		
	}else if (4 == (int)strlen(DPC) && 4 == (int)strlen(OPC) && 4 == (int)strlen(CIC))
	{
		tmpStr[pos++] = 'C';
		tmpStr[pos++] = '5';
		
		tmpStr[pos++] = DPC[2];
		tmpStr[pos++] = DPC[3];
		
        memcpy(temp, DPC, 2);
        sscanf(temp, "%x", &i1);
		i1 = i1 & 63;
		//printf("i1: %x\n",i1);
		
		memcpy(temp, OPC+2, 2);
        sscanf(temp, "%x", &i2);
		//printf("i2: %x\n",i2);
		i2 = i2<<6;	
		//printf("i2: %x\n",i2);
		
		i1 = i1|i2;
		//printf("i1: %x\n",i1);
		i2 = i1&0x00f0;
		sprintf( &tmpChar,"%x", i2);
		tmpStr[pos++] = tmpChar;
		
		i2 = i1&0x000f;
		sprintf( &tmpChar,"%x", i2);
		tmpStr[pos++] = tmpChar;		
		
		memcpy(temp, OPC, 4);
        sscanf(temp, "%x", &i1);
		i1 = i1>>2;
		i2 = i1&0x00f0;
		sprintf( &tmpChar,"%x", i2);
		tmpStr[pos++] = tmpChar;		
		
		i2 = i1&0x000f;
		sprintf( &tmpChar,"%x", i2);
		tmpStr[pos++] = tmpChar;
		
		tmpStr[pos++] = CIC[1];
		
		i2 = i1&0x0f00;
		sprintf( &tmpChar,"%x", i2);
		//printf("tmpChar: %c\n",tmpChar);
		tmpStr[pos++] = tmpChar;	
		
	}else{
		//printf("input format error");
		return 0;
	}	
	
	//CIC
	for (i = 0; i < (int)strlen(CIC); i ++ )
	{
		tmpStr[pos++] = CIC[i];
	}
	tmpStr[pos++] = '1';
	tmpStr[pos++] = '7';
	tmpStr[pos++] = '0';
	tmpStr[pos++] = '1';
	tmpStr[pos++] = '0';
	tmpStr[pos++] = '1';
	tmpStr[pos++] = '0';
	tmpStr[pos++] = 'e';
	tmpStr[pos++] = '\0';
	
	return pos;
}
