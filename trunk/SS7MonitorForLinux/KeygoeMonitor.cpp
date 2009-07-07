#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <unistd.h>
#include <stddef.h>

#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"
#include "DJAcsDevState.h"
#include "KeygoeEvent.h"
#include "KeygoeMonitor.h"
#include "strconvert.h"
#include "mycur.h"

#define MAX_BUFFER 512
#define QUIT_STRING "q"
#define HELP_STRING "h"

extern list *g_pPcmFirst;
extern list *g_pLinkFirst;


int makeargv(const char *s, const char *delimiters, char ***argvp)
{
  int error;
  int i;
  int numtokens;
  const char *snew;
  char *t;
  
  if ((s == NULL) || (delimiters == NULL) || (argvp == NULL))
  {
    errno = EINVAL;
    return -1;
  }
  *argvp = NULL;
  snew = s + strspn(s, delimiters); /*snew is real start of string */
  if ((t = (char *)malloc(strlen(snew) + 1)) == NULL)
    return -1;
  strcpy(t, snew);
  numtokens = 0;
  if (strtok(t, delimiters) != NULL) /*count the number of tokens in s */
    for (numtokens = 1; strtok(NULL, delimiters) != NULL; numtokens++);
  
  if ((*argvp = (char **)malloc((numtokens + 1) * sizeof(char *))) == NULL)
  {
    error = errno;
    free(t);
    errno = error;
    return -1;
  }
  
  if (numtokens == 0)
    free(t);
  else
  {
    strcpy(t, snew);
    **argvp = strtok(t, delimiters);
    for (i = 1; i < numtokens; i++)
       *((*argvp) + i) = strtok(NULL, delimiters);       
  }
  *((*argvp) + numtokens) = NULL;  /*put in final NULL pointer*/  
  return numtokens;  
}

void freemakeargv(char **argv)
{
  if (argv == NULL)
    return;
  if (*argv != NULL)
    free(*argv);
  free(argv);
}

int monitorProc(char **chargvp, ACSHandle_t acsHandle, int numtokens)
{
  char *chargv1;
  char *chargv2;  
  if (ldebug_flag)
  {
    printstr(NULL,"monitor paramiter number is %d\n", numtokens);
  }
    
  if (numtokens >= 3)
  {
    chargv1 = strupr(chargvp[1]);   
    chargv2 = strupr(chargvp[2]); 
    if ((strcmp(chargv1, "-O") == 0) && (strcmp(chargv2, "SS7LINK") == 0))    
    {
      if ((chargvp[3] != NULL) && (strcmp(chargvp[3], "-t") == 0))
      {
        for (; ;)
        {
          showSS7LinkState();
          usleep(30000*100);
        }
       }
       else
       {
          showSS7LinkState();
       }            
      return 0;
    }
    else if ((strcmp(chargv1, "-O") == 0) && (strcmp(chargv2, "PCM") == 0))
    {
      if ((chargvp[3] != NULL) && (strcmp(chargvp[3], "-t") == 0))
      {
        for(; ;)
        {
          showPCMState();
          usleep(30000*100);
        }
      }
      else
      {       
        showPCMState();
      }
      return 0;
    }
    else
    {
      printstr(NULL,"Input monitor parameter has been omittem,please input again(q exit, h help)\n");
    }     
  }
  else
  {
    printstr(NULL,"Input monitor parameter has been omittem,please input again(q exit, h help)\n");
  }
  
  return 1;
}

int maintainProc(char **chargvp, ACSHandle_t acsHandle, int numtokens)
{
  int i;
  int j;  
  char *chargv1;
  char *chargv2;
  Acs_SS7RawFrame_Data   raw_data = {0};
  int Byte_cnt = 0;
  unsigned char Byte_val[512] = {0};
  char rawVal[512] = {0};
  int iLinkCh = -1;
  DeviceID_t linkDev = {0}; 
  list *current = NULL;
  RetCode_t r = -1;
  char DPC[7] = {0};
  char OPC[7] = {0};
  char CIC[5] = {0}; 
  char *token = NULL; 
  int  iCnt = 0;
//  int  bChina = 0;
  
  if (numtokens >= 10)
  { 
    chargv1 = strlwr(chargvp[3]);
    iLinkCh = atoi(chargvp[4]);
    
    if (strcmp(chargv1, "-ch") == 0)
    {
    	token = strtok(chargvp[4], "-");
    	while (token)
    	{
    		if (iCnt == 0)
    			linkDev.m_s8ModuleID = atoi(token);
    		else if (iCnt == 1)
    			linkDev.m_s16ChannelID = atoi(token);
    		token = strtok(NULL, "-");
    		iCnt++;
    	}    
     }
     else
     {
       printstr(NULL,"Input ss7 link channel error, please input again(q exit, h help)\n");
       return 1;
     }
    
    chargv1 = strlwr(chargvp[1]);
    chargv2 = strlwr(chargvp[2]);
    
    if (strlen(chargvp[7]) == 4 && strlen(chargvp[8]) == 4)
    {
    //	bChina = 0;
    	raw_data.m_u8LinkType = XMS_SS7_LINK_TYPE_ITU;
    }
    else if (strlen(chargvp[7]) == 6 && strlen(chargvp[8]) == 6)
    {
    //	bChina = 1;
    	raw_data.m_u8LinkType = XMS_SS7_LINK_TYPE_CHINA;   
    }  
    else
    {
    	printstr(NULL, "Input DPC or OPC error, please input again(q exit, h help)\n");
    }
     
    raw_data.m_u8InfoType  = XMS_SS7_MSG_DATA; // Must set to data // 
    if ((strcmp(chargv1, "-u") == 0) && (strcmp(chargv2, "tup")) ==0)
    {
      raw_data.m_u8UserType = XMS_SS7_USER_TYPE_TUP;      
    }
    else if((strcmp(chargv1, "-u") == 0) && (strcmp(chargv2, "isup")) == 0)
    {
      raw_data.m_u8UserType = XMS_SS7_USER_TYPE_ISUP;     
    }
    else
    {
      printstr(NULL,"Input user type error, please input again(q exit, h help)\n");
      return 1;
    }  
    
    chargv1 = strlwr(chargvp[5]);
    chargv2 = strlwr(chargvp[6]);
    if (strcmp(chargv1, "-cmd") == 0)
    {
      strncpy(DPC, chargvp[7], 6);
      strncpy(OPC, chargvp[8], 6);
      strncpy(CIC, chargvp[9], 4);
      if (strcmp(chargv2, "grs") == 0)
      {
      	convertStrMsgISUP_SM_GRS(rawVal, DPC, OPC, CIC);
      }
      else if (strcmp(chargv2, "blo") ==0)
      {
      	convertStrMsgISUP_SM_BLO(rawVal, DPC, OPC, CIC);
      }
      else if (strcmp(chargv2, "ubl") == 0)
      {
      	convertStrMsgISUP_SM_UBL(rawVal, DPC, OPC, CIC);
      }
      else if (strcmp(chargv2, "cgb") == 0)
      {
      	convertStrMsgISUP_SM_CGB(rawVal, DPC, OPC, CIC);
      }
      else if (strcmp(chargv2, "cgu") == 0)
      {
      	convertStrMsgISUP_SM_CGU(rawVal, DPC, OPC, CIC);
      }
      else if (strcmp(chargv2, "rsc") == 0)
      {
        convertStrMsgISUP_SM_RSC(rawVal, DPC, OPC, CIC);
      }
      else
      {
        printstr(NULL,"Input cmd type error, please input again(q exit, h help)\n");
        return 1;
      }       
     
      for (i = 0, j = 0; i <= strlen(rawVal); i+=2, j++)
      {       
        char temp[3] = {0};
        memcpy(temp, rawVal+i, 2);
        sscanf(temp, "%x", &Byte_val[j]);             
      } 
      Byte_cnt = strlen(rawVal)/2;        
    }
    else
    {
      printstr(NULL,"Input cmd error, please input again(q exit, h help)\n");
      return 1;
    }       
   
    if ((j = find(g_pLinkFirst, linkDev, &current)) >= 0)
    {
      linkDev = current->entry[j].m_dev;
      raw_data.m_u8InfoLen  = Byte_cnt;
      memcpy(raw_data.m_u8Info, Byte_val, Byte_cnt);              
          
      r = XMS_ctsSendRawFrame(acsHandle, &linkDev,                     
                  raw_data.m_u8InfoLen + 8,
                  &raw_data);
      printstr(NULL, "dev %d-%d send ss7 raw data, ret=%d\n",
                linkDev.m_s8ModuleID, linkDev.m_s16ChannelID, r);  
      printstr(NULL, "Data:");
      for (i = 0; i < Byte_cnt; i++)
          printstr(NULL, "%02x ", Byte_val[i]);
      printstr(NULL, "\n");      
    }
    else
    {
      printstr(NULL,"dev %d-%d Can't find this device, please input again(q exit, h help)\n", 
               linkDev.m_s8ModuleID, linkDev.m_s16ChannelID);
      return 1;     
    } 
        
  } 
  else
  {
    printstr(NULL,"Input monitor paramter has been omittem, please input again(q exit, h help)\n");
    return 1;
  }
 }

/*
int maintainProc(char **chargvp, ACSHandle_t acsHandle, int numtokens)
{
  Acs_SS7RawFrame_Data   raw_data = {0};
  int Byte_cnt = 0;
  
  raw_data.m_u8LinkType = XMS_SS7_LINK_TYPE_CHINA;      
  raw_data.m_u8InfoType  = XMS_SS7_MSG_DATA; // Must set to data // 
  int iStart = 0, iLast = 0;
  int i;
  int j;
  unsigned short int usDspMod = 0, usDspChnl = 0;
  DeviceID_t pDev = {0};
  unsigned char Byte_val[512] = {0};
  char *chargv1;
  char *chargv2;
        
  if (numtokens >= 6)
  {
    chargv1 = strupr(chargvp[1]);   
    chargv2 = strupr(chargvp[2]); 
    if ((strcmp(chargv1, "-O") == 0) && (strcmp(chargv2, "ISUP") == 0))
    {   
      raw_data.m_u8UserType = XMS_SS7_USER_TYPE_ISUP;       
    }
    else if ((strcmp(chargv1, "-O") == 0) && (strcmp(chargv2, "TUP") == 0))
    {
      raw_data.m_u8UserType = XMS_SS7_USER_TYPE_TUP;      
    }
    else
    {
      printstr(NULL,"Input maintain operator error\n");
      return 1;
    } 
    
    chargv1 = strupr(chargvp[3]);
    chargv2 = strupr(chargvp[5]);
    if ((strcmp(chargv1, "-F") == 0) )
    {
      iStart = atoi(chargvp[4]);      
    }
    else
    {
      printstr(NULL,"Input maintain paramter -F <num> error, please input again(q exit, h help)\n");
      return 1;
    }
    
    if ((strcmp(chargv2, "-L") == 0) )
    {
      iLast = atoi(chargvp[6]);
    }
    else
    {
      printstr(NULL,"Input maintain paramter -L <num> error, please input again(q exit, h help)\n");
      return 1;
    }   
    
    if ((Byte_cnt = numtokens - 7) < (1+3+3+2+1+1))
    {
      printstr(NULL,"ss7 raw frame data is not entire,please input again(q exit, h help)\n");
      return 1;
    } 
    else
    {       
      for (i = 7; chargvp[i] != NULL; i++)
      {       
          sscanf(chargvp[i], "%x", &Byte_val[i - 7]);
      }
      
      for (i = iStart; i <= iLast; i++)
      {     
        pDev.m_s8ModuleID = i/(g_pLinkFirst->m_lDevNum) + 1;
        //pDev.m_s16ChannelID = i % (g_pLinkFirst->m_lDevNum) + 16;   
        pDev.m_s16ChannelID = 16; //slot = 16   
        list *current = NULL;
        if ((j = find(g_pLinkFirst, pDev, &current)) >= 0)
        {
          pDev = current->entry[j].m_dev;
          raw_data.m_u8InfoLen  = Byte_cnt;
          memcpy(raw_data.m_u8Info, Byte_val, Byte_cnt);    
            
          
          RetCode_t r = XMS_ctsSendRawFrame(acsHandle, &pDev,                      
                  raw_data.m_u8InfoLen + 8,
                  &raw_data);
          printstr(NULL, "dev %d-%d send ss7 raw data, ret=%d\n",
                   pDev.m_s8ModuleID, pDev.m_s16ChannelID, r);          
        }
        else
        {
          printstr(NULL,"dev %d-%d Can't find this device, please input again(q exit, h help)\n", 
                   pDev.m_s8ModuleID, pDev.m_s16ChannelID);
          continue;
        } 
      }
    }   
  }
 else
  {
    printstr(NULL,"Input monitor paramter has been omittem, please input again(q exit, h help)\n");
    return 1;
  }
  
  return 0;
}*/

void helpProc()
{
  WINDOW *win;
  int height, width;
  win = stdscr;
  getyx(win, height, width);
  mvprintw(height, 0, "connect server:\n");
  getyx(win, height, width);
  mvprintw(height, 2, "IP <host> PORT <portnum>\n");
  getyx(win, height, width);
  mvprintw(height, 0, "monitor pcm only once:\n");
  getyx(win, height, width);
  mvprintw(height, 2, "monitor -o pcm\n");
  getyx(win, height, width);
  mvprintw(height, 0, "monitor pcm periodicity:\n");
  getyx(win, height, width);
  mvprintw(height, 2, "monitor -o pcm -t\n");
  getyx(win, height, width);
  mvprintw(height, 0, "monitor ss7 link only once:\n");
  getyx(win, height, width);
  mvprintw(height, 2, "monitor -o ss7link\n");
  getyx(win, height, width);
  mvprintw(height, 0, "monitor ss7 link periodicity:\n");
  getyx(win, height, width);
  mvprintw(height, 2, "monitor -o ss7link -t\n");
  getyx(win, height, width);
  mvprintw(height, 0, "maintain TUP ss7 link:\n");
  getyx(win, height, width);
  mvprintw(height, 2, "maintain -u tup -ch <DSPID-CHID> -cmd <cmd type> <DPC> <OPC> <CIC>\n");
  getyx(win, height, width);
  mvprintw(height, 0, "maintain isup ss7 link:\n");
  getyx(win, height, width);
  mvprintw(height, 2, "maintain -u isup -ch <DSPID-CHID> -cmd <cmd type> <DPC> <OPC> <CIC>\n");
  refresh();    
}

int main(void)
{
  char delim[] = " \t";
  char inbuf[MAX_BUFFER];
  char **chargv;    
  ServerID_t    serverID;
  ACSHandle_t   acsHandle;
  int i, numtokens;
  char *chInput;
  char *chargv0;
  //DJ_U8     g_u8UnitID = 255; 
  int row, col;
  int curRow, curCol;
  char *chargv1;
  char *chargv2;  
  
  initCursel(&row, &col);  
 
  printstr(NULL, "This is a monitor tool to SS7\n");
  printstr(NULL, "press q or ctrl+c exit programmer\n");  
  printstr(NULL, "Input service IP and port(q exit, h help):\n");
  for (; ;)
  {  
  	getyx(stdscr, curRow, curCol);
  	if (curRow == row -1)
  		clear();
  		 	
    getstr(inbuf);  
  
    if (ldebug_flag)
      printstr(NULL,"inbuf:%s\n", inbuf);
    
    chInput = strlwr(inbuf);  
    if (strcmp(chInput, QUIT_STRING) == 0) 
    {  
    	exitwin(); 
      return 0;
    }
    else if(strcmp(chInput, HELP_STRING) == 0)
    {      
      helpProc();
    }
    else 
    {
      if ((numtokens = makeargv(inbuf, delim, &chargv)) >= 4)
      {
        if (ldebug_flag)
        {
          for (i = 0; i < numtokens; i++)
             printstr(NULL,"%d:%s\n", i, chargv[i]);
        }
        chargv1 = strlwr(chargv[0]);
        chargv2 = strlwr(chargv[2]);
        if ((strcmp(chargv1, "ip") == 0) && (strcmp(chargv[1], "\0") != 0))
        {                 
          strncpy(serverID.m_s8ServerIp, chargv[1], sizeof(serverID.m_s8ServerIp));
          if ((strcmp(chargv2, "port") == 0)&& (strcmp(chargv[3], "\0") != 0))
          {
            serverID.m_u32ServerPort = atoi(chargv[3]);
            if (initsystem(serverID, &acsHandle) >= 0)
            {            	
              getyx(stdscr, curRow, curCol);
              mvprintw(curRow, 0,"Service has been  monitored\n");
              usleep(500);
              break;  
            }
            else  
              printstr(NULL,"Please check IP and Port, and please input again(q exit, h help)\n");          
          }
          else
          {
            printstr(NULL,"Input Port error, please input again(q exit, h help)\n");
          }
        }
        else
        {
          printstr(NULL,"Input IP error,please input again(q exit, h help)\n");         
        }       
      }
      else
      {
        printstr(NULL,"Input IP and Port has been omittem,please input again(q exit, h help)\n"); 
      }
    }
  } 
  
  for(; ;)
  {  
  	getyx(stdscr, curRow, curCol);
  	if (curRow == row -1)
  		clear(); 
  		  	
    getstr(inbuf);
   
    chInput = strlwr(inbuf);
    if (ldebug_flag)
      printstr(NULL,"inbuf:%s\n", chInput);
    if (strcmp(chInput, QUIT_STRING) == 0)
    {
      exitsystem(acsHandle);
      exitwin();
      exit(0);
    }
    else if(strcmp(chInput, HELP_STRING) == 0)
    {
      helpProc();   
    } 
    else
    {
      if ((numtokens = makeargv(inbuf, delim, &chargv)) > 0)
      {
        chargv0 = strlwr(chargv[0]);
        if (strcmp(chargv0, "monitor") == 0)
        {
          if (ldebug_flag)
            printstr(NULL,"monitor paramiter number is %d\n", numtokens);
          monitorProc(chargv, acsHandle, numtokens);    
          printstr(NULL,"Input command again if operator(q exit, h help)\n");       
        }
        else if(strcmp(chargv0, "maintain") == 0)
        {
          maintainProc(chargv, acsHandle, numtokens);
          printstr(NULL,"Input command again if operator(q exit, h help)\n");;    
        } 
        else
        {   
          printstr(NULL,"Input command error, please input again(q exit, h help)\n");
        } 
      }
      free(chargv);
    } 
    usleep(30);   
  }
  
  exitwin();
  return 0;
}




  

