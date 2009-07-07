#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <unistd.h>
#include <stddef.h>
   
#include "KeygoeEvent.h"
#include "mycur.h"

list *g_pPcmFirst = NULL;
list *g_pPcmLast = NULL;
list *g_pLinkFirst = NULL;
list *g_pLinkLast = NULL;

ACSHandle_t g_acsHandle = -1;

void ldebug(int debug)
{
  ldebug_flag = debug;
}

static list *insert_new_entry(int len, DJ_S8 s8DspModID,list **first, list **last)
{
  tagDEVICE *new_DevTab;
  list *new_entry;  

  new_entry = (list *)malloc(sizeof(list) + sizeof(tagDEVICE)*len);
  if (new_entry == NULL)
    return NULL;
  new_DevTab = (tagDEVICE *)((char *)new_entry + sizeof(list));
  new_entry->entry = new_DevTab;  
  new_entry->next = NULL;
  new_entry->m_s8DspMod = s8DspModID;
  new_entry->m_lDevNum = len;
  
  if (ldebug_flag)
    printstr(NULL,"new_entry entry=%d, m_s8DspMod=%d, m_lDevNum=%d\n", new_DevTab, s8DspModID, len);
  
  /*add lock*/
  if (*last == NULL)
    *first = new_entry;
  else
    (*last)->next = new_entry;
    
  *last = new_entry;
  /*leave lock*/
  
  return new_entry;
}

static int get_length(list *first)
{
  int len = 0;
  list *current;
  
  current = first;
  /*add lock*/
  while (current != NULL)
  {
    len += current->m_lDevNum;
    current = current->next;
  }
  /*leave lock*/
  return len;
}

long find(list *first, DeviceID_t dev, list **local)
{
  list *current = NULL;
  int i;
  
  *local = NULL;
  
  /*add lock*/
  current = first;
  
  while (current != NULL)
  {
    if (current->m_s8DspMod == dev.m_s8ModuleID)
    {
      for(i = 0; i < current->m_lDevNum; i++)
      {
        if (current->entry[i].m_dev.m_s16ChannelID == dev.m_s16ChannelID)
        {
          *local = current;
          /*leave lock*/
          
          return i; 
        }           
      }      
     /*leave lock*/
     return -1;
    }
    current = current->next;
  }
  /*leave lock*/
    
  return -1;
}

static void remove(list *first, DJ_S8 s8DspModID)
{
  list *current;
  list *free_entry;
  
  /*add lock*/
  current = first;
  while (current != NULL)
  {   
    if (current->m_s8DspMod == s8DspModID)
    {
      free_entry = current;
      current = current->next;
      free(free_entry);
      /*leave lock*/
      return;
    }
    current = current->next;
  }
  /*leave lock*/
}

static void clear(list *first, list *last)
{
  list *current;
  list *free_entry;
  
  /*add local*/
  current = first;
  while (current != NULL)
  {
    free_entry = current;
    current = current->next;
    free(free_entry);
  }
  
  first = NULL;
  last = NULL;
  /*leave lock*/  
}

void exitsystem(ACSHandle_t acsHandle)
{
  XMS_acsCloseStream(acsHandle, NULL);
  XMS_acsDllLUnInit();
  clear(g_pPcmFirst, g_pPcmLast);
  clear(g_pLinkFirst, g_pLinkLast); 
}

int initsystem(ServerID_t serverID, ACSHandle_t *acsHandlep)
{
  RetCode_t ret = 0;
  DJ_U8 u8UnitID;
  WINDOW *win;
  win = stdscr;
  int height, width;
  
  ret = XMS_acsDllInit();
  if (ret < 0)
  {
  	getyx(win, height, width);
    mvprintw(height, 0, "XMS_acsDllInit(IP:%s, port:%d) fail. ret=%d\n",
           serverID.m_s8ServerIp, serverID.m_u32ServerPort, ret);
    return ret;
  }
  
  u8UnitID = 128;
  ret = XMS_acsOpenStream(acsHandlep, &serverID,
                          u8UnitID, 64, 64, 0, NULL);
  if ( ret < 0)
  {
    if (ldebug_flag)
    {
      getyx(win, height, width);
      mvprintw(height, 0, "XMS_acsOpenStream(IP:%s, port:%d) fail. ret=%d\n",
              serverID.m_s8ServerIp, serverID.m_u32ServerPort, ret);
    }
    return ret;
  }
  else
  {
    if (ldebug_flag)
      printstr(NULL,"XMS_acsOpenStream(IP:%s,port:%d)OK!\n",
             serverID.m_s8ServerIp, serverID.m_u32ServerPort, ret);
  }  
  
  ret = XMS_acsSetESR(*acsHandlep, (EsrFunc)EvtHandler, 0, 1);
  if(ret < 0)
  {
    if (ldebug_flag)
      printstr(NULL,"XMS_acsSetESR() fial! ret=%d\n", ret);
    return ret;   
  }
  else
  {
    if (ldebug_flag)
      printstr(NULL,"XMS_acsSetESR() OK!\n");
  }
  
  XMS_acsGetDeviceList(*acsHandlep, NULL); 
  
  g_acsHandle = *acsHandlep;
  
  return 0;
}
static void AddDeviceRes_Pcm(DJ_S8 s8DspModID, Acs_Dev_List_Head_t *pAcsDevList)
{
  DJ_S32  s32Num; 
  int i;
  tagDEVICE *pdev = NULL;
  DeviceID_t *pdevice = NULL;
  list *new_entry;
    
  s32Num = pAcsDevList->m_s32DeviceNum;   
  pdev = (tagDEVICE *)malloc(sizeof(tagDEVICE)*s32Num);
  if (pdev == NULL)
  {
    if (ldebug_flag)
      printstr(NULL,"pdev malloc error\n");
    return;
  } 
  memset(pdev, 0, sizeof(tagDEVICE)*s32Num);
  if (ldebug_flag)
    printstr(NULL,"memset pdev 0 AddDeviceRes_Pcm\n");
    
  pdevice = (DeviceID_t *)((unsigned char *)pAcsDevList + sizeof(Acs_Dev_List_Head_t));
  if (pdevice != NULL)
  {
    if (ldebug_flag)
      printstr(NULL,"pdevice memory address=%d, s32Num=%d\n", pdevice, s32Num);
  }
  
  for(i = 0; i < s32Num; i++)
  {   
    if (ldebug_flag)
      printstr(NULL,"add pdev[%d].mdev(%d,%d) to pcm list\n", i, pdevice[i].m_s8ModuleID, pdevice[i].m_s16ChannelID);
    pdev[i].m_dev = pdevice[i];     
  }
  
  new_entry = insert_new_entry(s32Num, s8DspModID, &g_pPcmFirst, &g_pPcmLast);
  (void)memcpy(new_entry->entry, pdev, sizeof(tagDEVICE)*s32Num);

  free(pdev);
}

static void AddDeviceRes_SS7Link(DJ_S8 s8DspModID, Acs_Dev_List_Head_t *pAcsDevList)
{ 
  DJ_S32 s32Num, ss7LinkNum, s32Type;
  int i;
  tagDEVICE *pdev = NULL;
  DeviceID_t *dev = NULL;
  s32Num = pAcsDevList->m_s32DeviceNum;
  list *new_entry; 
    
  pdev = (tagDEVICE *)malloc(sizeof(tagDEVICE)*s32Num);
  if (pdev == NULL)
  {
    if (ldebug_flag)
      printstr(NULL,"pdev malloc error\n");
    return;
  } 
  
  memset(pdev, 0, sizeof(tagDEVICE)*s32Num);
  ss7LinkNum = 0;
  dev = (DeviceID_t *)((unsigned char *)pAcsDevList + sizeof(Acs_Dev_List_Head_t));
  for(i = 0; i < s32Num; i++)
  {     
    s32Type = dev[i].m_s16DeviceSub;
    
    if (s32Type == XMS_DEVSUB_SS7_LINK)
    {
      if (ldebug_flag)
        printstr(NULL,"add pdev[%d].mdev(%d,%d) to ss7link list\n", i, dev[i].m_s8ModuleID, dev[i].m_s16ChannelID);
      pdev[ss7LinkNum].m_dev = dev[i];      
      ss7LinkNum++; 
    }
  }
          
  new_entry = insert_new_entry(ss7LinkNum, s8DspModID, &g_pLinkFirst, &g_pLinkLast);
  memcpy(new_entry->entry, pdev, sizeof(tagDEVICE)*ss7LinkNum);

  free(pdev);
}
static void AddDeviceRes(Acs_Dev_List_Head_t *pAcsDevList)
{
  DJ_S32 s32Type;
  DJ_S8 s8DspModID;
  
  s32Type = pAcsDevList->m_s32DeviceMain; 
  s8DspModID = (DJ_S8) pAcsDevList->m_s32ModuleID;
  if ((s8DspModID < 0))
    return;
    
  switch(s32Type)
  {
  case XMS_DEVMAIN_DIGITAL_PORT:
    AddDeviceRes_Pcm(s8DspModID, pAcsDevList);
    break;
  case XMS_DEVMAIN_INTERFACE_CH: 
    AddDeviceRes_SS7Link(s8DspModID, pAcsDevList);
    break;
  case XMS_DEVMAIN_SS7_LINK: break;
  case XMS_DEVMAIN_VOICE: break;
  case XMS_DEVMAIN_FAX: break;    
  case XMS_DEVMAIN_DSS1_LINK: break;
  case XMS_DEVMAIN_BOARD: break;
  case XMS_DEVMAIN_CTBUS_TS: break;
  case XMS_DEVMAIN_VOIP: break;
  case XMS_DEVMAIN_CONFERENCE: break;
  case XMS_DEVMAIN_VIDEO: break;
  default: break;
  }
}

static void OpenAllDevice_Dsp(DJ_S8 s8DspModID)
{
  int i;  
  RetCode_t ret;
  DeviceID_t dev;
  
  list *current = NULL;
    
  /*add lock lock pcmlist*/
  current = g_pPcmFirst;
  if (current == NULL)
    if (ldebug_flag)
      printstr(NULL,"g_pPcmFirst=NULL\n");    
  
  while(current != NULL)
  {
    for (i = 0; i < current->m_lDevNum; i++)
    {     
      dev = current->entry[i].m_dev;
      if (ldebug_flag)
        printstr(NULL,"open dev(main=%d,sub=%d,%d, %d)\n", dev.m_s16DeviceMain, dev.m_s16DeviceSub,
               dev.m_s8ModuleID, dev.m_s16ChannelID);
      ret = XMS_ctsOpenDevice(g_acsHandle, &dev, NULL );
      if (ret < 0)
      {
        if (ldebug_flag)
        {
          printstr(NULL,"open devType(%d,%d) dev(%d %d) callID(%d,%d)\n ", dev.m_s16DeviceMain,
                 dev.m_s16DeviceSub, dev.m_s8ModuleID, dev.m_s16ChannelID); 
          printstr(NULL,"pcm XMS_ctsOpenDevice Fail. ret=%d\n", ret);
        }
      }       
    }
        
    current = current->next;      
  }
  /*leave lock*/
  
  /*add lock lock Linklist*/
  current = g_pLinkFirst;
  while(current != NULL)
  {
    for (i = 0; i < current->m_lDevNum; i++)
    {
      dev = current->entry[i].m_dev;
      ret = XMS_ctsOpenDevice(g_acsHandle, &dev, NULL);
      if (ret < 0)
      {
        if (ldebug_flag)
        {
          printstr(NULL,"open devType(%d,%d) dev(%d %d) callID(%d,%d): ", dev.m_s16DeviceMain,
                 dev.m_s16DeviceSub, dev.m_s8ModuleID, dev.m_s16ChannelID); 
          printstr(NULL,"XMS_ctsOpenDevice Fail. ret=%d\n", ret);
        }         
      }     
    }
    current = current->next;
  }
  /*leave lock*/
}

static void HandleDevState(Acs_Evt_t *pAcsEvt)
{ 
  list *current = NULL;
  long i;
  Acs_GeneralProc_Data *pGeneralData = NULL;      

  pGeneralData = (Acs_GeneralProc_Data *)FetchEventData(pAcsEvt);
  if ((pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH) && 
      (pAcsEvt->m_DeviceID.m_s16DeviceSub == XMS_DEVSUB_SS7_LINK))      
  {
    if ((i = find(g_pLinkFirst, pAcsEvt->m_DeviceID, &current)) >= 0)
    {
      current->entry[i].u_info.m_LinkInfo.m_GeneralData = *pGeneralData;
    } 
    if (ldebug_flag)
       printstr(NULL,"get ss7link state\n");
  }
}

static void OpenDeviceOK(DeviceID_t *pDevice)
{ 
  list *current = NULL;
  int i;  
  
  if ((pDevice->m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH) &&
      (pDevice->m_s16DeviceSub == XMS_DEVSUB_SS7_LINK))
  {
    if ((i = find(g_pLinkFirst, *pDevice, &current)) >= 0)
    {
      /*add lock*/
      if (current == NULL)
        if (ldebug_flag)
          printstr(NULL,"openDeviceOk find link=NULL\n");
          
      current->entry[i].m_dev.m_CallID = pDevice->m_CallID;
      
      if (ldebug_flag)
      {
        printstr(NULL,"open devType(%d,%d) dev(%d %d) callID(%d,%d)\n ", current->entry[i].m_dev.m_s16DeviceMain,
               current->entry[i].m_dev.m_s16DeviceSub, current->entry[i].m_dev.m_s8ModuleID, current->entry[i].m_dev.m_s16ChannelID, 
               current->entry[i].m_dev.m_CallID.m_s32FlowType, current->entry[i].m_dev.m_CallID.m_s32FlowChannel);        
      }
      /*leave lock*/
      XMS_ctsResetDevice(g_acsHandle, pDevice, NULL);
      XMS_ctsGetDevState(g_acsHandle, pDevice, NULL);     
    }   
  }   

  if (pDevice->m_s16DeviceMain == XMS_DEVMAIN_DIGITAL_PORT)
  {
    if ((i = find(g_pPcmFirst, *pDevice, &current)) >= 0)
    {
      /*add lock*/
      if (current == NULL)
        if (ldebug_flag)
          printstr(NULL,"openDeviceOk find pcm=NULL\n");
          
      current->entry[i].m_dev.m_CallID = pDevice->m_CallID;
      if (ldebug_flag)
      {
        printstr(NULL,"open devType(%d,%d) dev(%d %d) callID(%d,%d)\n ", current->entry[i].m_dev.m_s16DeviceMain,
               current->entry[i].m_dev.m_s16DeviceSub, current->entry[i].m_dev.m_s8ModuleID, 
               current->entry[i].m_dev.m_s16ChannelID, current->entry[i].m_dev.m_CallID.m_s32FlowType, 
               current->entry[i].m_dev.m_CallID.m_s32FlowChannel);        
      }
      /*leave lock*/
      XMS_ctsResetDevice(g_acsHandle, pDevice, NULL);
      XMS_ctsGetDevState(g_acsHandle, pDevice, NULL);   
    } 
  }
}

DJ_Void EvtHandler(DJ_U32 esrParam)
{
  Acs_Evt_t *pAcsEvt;
  Acs_Dev_List_Head_t *pAcsDevList;
  ACS_Digital_PortState_Data *pDigitState = NULL;
  Acs_UniFailure_Data *pUniFailData = NULL;
  WINDOW *win;
  int height, width;
  
  win = stdscr;
  
  pAcsEvt= (Acs_Evt_t *)esrParam;
  if (ldebug_flag)
  {
    /*print event*/
  }
  
  switch(pAcsEvt->m_s32EventType)
  {
    case XMS_EVT_QUERY_DEVICE:
      pAcsDevList = (Acs_Dev_List_Head_t *)FetchEventData(pAcsEvt);
      AddDeviceRes(pAcsDevList);
      break;
    case XMS_EVT_QUERY_ONE_DSP_END:
      {
        /*open all device*/
        OpenAllDevice_Dsp(pAcsEvt->m_DeviceID.m_s8ModuleID);
      }
      break;
    case XMS_EVT_QUERY_REMOVE_ONE_DSP_END:
      {
        /*delete Dsp from device table*/
        remove(g_pPcmFirst, pAcsEvt->m_DeviceID.m_s8ModuleID);
        remove(g_pLinkFirst, pAcsEvt->m_DeviceID.m_s8ModuleID);       
      }
      break;
    case XMS_EVT_QUERY_DEVICE_END:
      break;
    case XMS_EVT_OPEN_DEVICE:
      OpenDeviceOK(&pAcsEvt->m_DeviceID);
      break;
    case XMS_EVT_CLOSE_DEVICE:
    //  CloseDeviceOK(&pAcsEvt->m_DeviceID);
      break;
    case XMS_EVT_DEVICESTATE:
      HandleDevState(pAcsEvt);
      break;
    case XMS_EVT_UNIFAILURE:
      if (ldebug_flag)
      { 
        pUniFailData = (Acs_UniFailure_Data *)FetchEventData(pAcsEvt);
        if (pUniFailData->m_s32AcsEvtState=-1)
        {
          if (pUniFailData->m_s32AcsEvtErrCode == ACSERR_LINKBROKEN)
          {
            getyx(win, height, width);
            mvprintw(height, 0,"flow connection has been disconnected\n");
          }
          else if (pUniFailData->m_s32AcsEvtErrCode == ACSERR_GETRESFAIL)
          {
            getyx(win, height, width);
            mvprintw(height, 0, "OpenDevic get device error,deviceType(%d,%d) ch(%d,%d)\n",
                  pAcsEvt->m_DeviceID.m_s16DeviceMain, pAcsEvt->m_DeviceID.m_s16DeviceSub,
                  pAcsEvt->m_DeviceID.m_s8ModuleID, pAcsEvt->m_DeviceID.m_s16ChannelID);
          }
          else if (pUniFailData->m_s32AcsEvtErrCode == ACSERR_DEVICEERR)
          {
            getyx(win, height, width);
            mvprintw(height, 0, "api transfer device error\n");
          }
        }
        break;
     case XMS_EVT_CLASS_UNIFAILURE:
        if (ldebug_flag)       
        {
          pUniFailData = (Acs_UniFailure_Data *)FetchEventData(pAcsEvt);
          if (pUniFailData->m_s32AcsEvtState == -1)
          {
            if(pUniFailData->m_s32AcsEvtErrCode == ACSERR_FAILSEND)
            {
              getyx(win, height, width);
              mvprintw(height, 0, "send command to function module error\n");
            } 
          }         
        } 
      } 
      break;
   case XMS_EVT_E1STATE:
      pDigitState = (ACS_Digital_PortState_Data *)FetchEventData(pAcsEvt);
      saveDigitState(pAcsEvt, *pDigitState);      
      
      if (ldebug_flag)
      {
       //  showDebugPCMState(pAcsEvt, *pDigitState); 
      } 
      break;
   case XMS_EVT_SS7SendRawFrame:   
    if (ldebug_flag)
    {
    	printstr(NULL, "recv send raw evt\n");    	
    }  
    break;
   case XMS_EVT_SS7RawFrame:
     //if (ldebug_flag)
     {
      Acs_SS7RawFrame_Data *raw_data = NULL;
      raw_data = (Acs_SS7RawFrame_Data *)FetchEventData(pAcsEvt);
     }
    break;
   default: 
      break;        
  } 
}

void saveDigitState(Acs_Evt_t *pAcsEvt, ACS_Digital_PortState_Data digitStatep)
{
  int i;
  
 // if (ldebug_flag)
//    printstr(NULL,"run to saveDigitState\n"); 
    
  list *current = NULL;
  if ((i = find(g_pPcmFirst, pAcsEvt->m_DeviceID, &current)) >= 0)
  {
    /*add lock*/
    current->entry[i].u_info.m_pcmInfo.m_digitState = digitStatep;
    /*leave lock*/
  }   
}

char *getLSTStr(Acs_GeneralProc_Data *pData)
{
  switch(pData->m_s32DeviceState)
  {
  case DES_SS7_LST_CONNECTING:
    return "Connecting";    

  case DES_SS7_LST_ACTIVE:
    return "Active";  

  case DES_SS7_LST_FAILED:
    return  "Failed";   

  case DES_SS7_LST_WAITCON:
    return "WaitConn";  

  case DES_SS7_LST_SUSPEND:
    return "Suspend"; 
  }
  
  return "Unknow";
}

//void showSS7LinkState(int xrol, int ycol)
void showSS7LinkState()
{
  WINDOW* win;
  int height, width;
  int i;    
  list *current = NULL;   
  char chIndex[6][20] = {"dev", "LinkState"};  
  
  win = stdscr;
  clear();
  
  getyx(win, height, width); 
  mvprintw(height, width, chIndex[0]);
  
  getyx(win, height, width); 
  mvprintw(height, width + 3, chIndex[1]);
 
  printw("\n");
  refresh();
 
  current = g_pLinkFirst; 
  while (current != NULL)
  {
    for (i = 0; i < current->m_lDevNum; i++)
    {
      getyx(win, height, width);
      mvprintw(height, width, "%d-%d", current->entry[i].m_dev.m_s8ModuleID,
           current->entry[i].m_dev.m_s16ChannelID);
      getyx(win, height, width); 
      mvprintw(height, width + 3, "%s\n", getLSTStr(&(current->entry[i].u_info.m_LinkInfo.m_GeneralData))); 
      refresh();    
    }     
    current = current->next;
  }
}

char *getAlarmStr(int value)
{
  if (value == 1)
    return "Yes";
  else if (value == 0)
    return "No";
  else
    return "?";
}

void showPCMState()
{
  WINDOW* win;
  int height, width;
  int i; 
  list *current = NULL; 
//  char alarm[10] = {0};
  char chIndex[12][20] = {"dev", "Fas", "Mf", "Crc4",
                 "RmtAlrm", "RmtMFAlrm", "runTm",
                 "RSLIPSc", "TSLIPSc", "LCVRCnt", "FOSCnt"};    
  clear();  
  win = stdscr; 
  
  getyx(win, height, width); 
  mvprintw(height, width, chIndex[0]);
  for (i = 1; i < 11; i++)
  {
    getyx(win, height, width); 
    mvprintw(height, width + 2, "%s", chIndex[i]);
  } 

  getyx(win, height, width);
  mvprintw(height, width, "\n");
  refresh(); 

  current = g_pPcmFirst;
  while (current != NULL)
  { 
    for (i = 0; i < current->m_lDevNum; i++)
    {
        getyx(win, height, width);
        mvprintw(height , width, "%d-%d", current->entry[i].m_dev.m_s8ModuleID,
           current->entry[i].m_dev.m_s16ChannelID);
        getyx(win, height, width);
        width = strlen(chIndex[0]) + 2;
        mvprintw(height, width, 
                 getAlarmStr(current->entry[i].u_info.m_pcmInfo.m_digitState.m_u8Fas_align));
        getyx(win, height, width);
        width = strlen(chIndex[0]) + strlen(chIndex[1]) + 4;
        mvprintw(height, width, 
                 getAlarmStr(current->entry[i].u_info.m_pcmInfo.m_digitState.m_u8Mf_align));
        getyx(win, height, width);
        width = strlen(chIndex[0]) + strlen(chIndex[1]) + strlen(chIndex[2]) + 6;
        mvprintw(height, width, 
                getAlarmStr(current->entry[i].u_info.m_pcmInfo.m_digitState.m_u8Crc4_align));
        getyx(win, height, width);
        width = strlen(chIndex[0]) + strlen(chIndex[1]) + strlen(chIndex[2]) + strlen(chIndex[3]) + 8;
        mvprintw(height, width, 
                getAlarmStr(current->entry[i].u_info.m_pcmInfo.m_digitState.m_u8Remote_alarm));
        getyx(win, height, width);
        width = strlen(chIndex[0]) + strlen(chIndex[1]) + strlen(chIndex[2]) + 
                strlen(chIndex[3]) + strlen(chIndex[4]) + 10;
        mvprintw(height, width,
                getAlarmStr(current->entry[i].u_info.m_pcmInfo.m_digitState.m_u8Remote_MF_alarm));   
        getyx(win, height, width);
        width = strlen(chIndex[0]) + strlen(chIndex[1]) + strlen(chIndex[2]) + 
                strlen(chIndex[3]) + strlen(chIndex[4]) + strlen(chIndex[5]) + 12;
        mvprintw(height, width, "%d", current->entry[i].u_info.m_pcmInfo.m_digitState.m_u32total_seconds);
        getyx(win, height, width);
        width = strlen(chIndex[0]) + strlen(chIndex[1]) + strlen(chIndex[2]) + 
                strlen(chIndex[3]) + strlen(chIndex[4]) + strlen(chIndex[5]) + strlen(chIndex[6]) + 14;
        mvprintw(height, width, "%d", current->entry[i].u_info.m_pcmInfo.m_digitState.m_u32RSLIP_seconds);
        getyx(win, height, width);
        width = strlen(chIndex[0]) + strlen(chIndex[1]) + strlen(chIndex[2]) + 
                strlen(chIndex[3]) + strlen(chIndex[4]) + strlen(chIndex[5]) + 
                strlen(chIndex[6]) + strlen(chIndex[7]) + 16; 
        mvprintw(height, width, "%d", current->entry[i].u_info.m_pcmInfo.m_digitState.m_u32TSLIP_seconds);
        
        getyx(win, height, width);
        width = strlen(chIndex[0]) + strlen(chIndex[1]) + strlen(chIndex[2]) + 
                strlen(chIndex[3]) + strlen(chIndex[4]) + strlen(chIndex[5]) + 
                strlen(chIndex[6]) + strlen(chIndex[7]) + strlen(chIndex[8]) + 18; 
        mvprintw(height, width, "%d", current->entry[i].u_info.m_pcmInfo.m_digitState.m_u32LCVCR_count);
        
        getyx(win, height, width);
        width = strlen(chIndex[0]) + strlen(chIndex[1]) + strlen(chIndex[2]) + 
                strlen(chIndex[3]) + strlen(chIndex[4]) + strlen(chIndex[5]) + 
                strlen(chIndex[6]) + strlen(chIndex[7]) + strlen(chIndex[8]) + strlen(chIndex[9]) + 20; 
        mvprintw(height, width, "%d\n", current->entry[i].u_info.m_pcmInfo.m_digitState.m_u32FOSCR_count);
       
        refresh();  
        
        XMS_ctsGetDevState(g_acsHandle, &current->entry[i].m_dev, NULL);
    }
  
    current = current->next;
  } 
}

void showDebugPCMState(Acs_Evt_t *pAcsEvt, ACS_Digital_PortState_Data digitStatep)
{
 // system("clear");  
  
  printstr(NULL,"%-8s%-11s%-10s%-12s%-14s%-17s\n", "device", "Fas_align", "Mf_align",
         "Crc4_align", "Remote_alarm", "Remote_MF_alarm");    
  
  printstr(NULL,"%-3s(%d,%d)%-13s%-10s%-12s%-14d%-17d\n\n", " ", pAcsEvt->m_DeviceID.m_s8ModuleID,
        pAcsEvt->m_DeviceID.m_s16ChannelID, 
        getAlarmStr(digitStatep.m_u8Fas_align),
        getAlarmStr(digitStatep.m_u8Mf_align),
        getAlarmStr(digitStatep.m_u8Crc4_align),
        getAlarmStr(digitStatep.m_u8Remote_alarm),
        getAlarmStr(digitStatep.m_u8Remote_MF_alarm));    
  
  printstr(NULL,"System running time\n");
  printstr(NULL,"%-15s%-15s\n", "device", "running time");
  
  printstr(NULL,"%-3s(%d,%d)%-18d\n\n",  " " ,pAcsEvt->m_DeviceID.m_s8ModuleID,
        pAcsEvt->m_DeviceID.m_s16ChannelID, 
        digitStatep.m_u32total_seconds);    
  
  printstr(NULL,"Slip and error count\n");
  printstr(NULL,"%-8s%-15s%-15s%-12s%-13s%-13s\n", "device", "RSLIP_Seconds", "TSLIP_Seconds", "LCVR_Count",
         "PCVCR_Count", "FOSCR_Count");
  
 printstr(NULL,"%-3s(%d,%d)%-15d%-15d%-12d%-13d%-13d", " ", pAcsEvt->m_DeviceID.m_s8ModuleID,
        pAcsEvt->m_DeviceID.m_s16ChannelID,
        digitStatep.m_u32RSLIP_seconds,
        digitStatep.m_u32TSLIP_seconds,
        digitStatep.m_u32LCVCR_count,
        digitStatep.m_u32PCVCR_count,
        digitStatep.m_u32FOSCR_count);  
}


 