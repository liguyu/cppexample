#ifndef _KEYGOE_EVENT_H
#define _KEYGOE_EVENT_H

#include "ItpDataDefine.h"
#include "ITPCom.h"
#include "DJAcsSignalMonitor.h"

#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"
#include "DJAcsDevState.h"

//#define LDEBUGGING	
#ifdef LDEBUGGING
static int ldebug_flag = 1;
#else
static int ldebug_flag = 0;
#endif

typedef struct
{
	ACS_Digital_PortState_Data  m_digitState;	
}tagPcmInfo;

typedef struct
{
	Acs_GeneralProc_Data  m_GeneralData;	
}tagLinkInfo;

typedef struct 
{	
	DeviceID_t m_dev;
	
	union
	{
			tagPcmInfo m_pcmInfo;
			tagLinkInfo m_LinkInfo;
  }u_info;
  
	long m_lSep;	
		
}tagDEVICE;

typedef struct node
{
	tagDEVICE *entry;
	DJ_S8 m_s8DspMod;
	long m_lDevNum;			
	struct node *next;
}list;

void ldebug(int debug);
DJ_Void EvtHandler(DJ_U32 esrParam);
void exitsystem(ACSHandle_t acsHandle);
int initsystem(ServerID_t serverID, ACSHandle_t *acsHandlep);
void showSS7LinkState();
void showPCMState();
void saveDigitState(Acs_Evt_t *pAcsEvt, ACS_Digital_PortState_Data digitStatep);
void showDebugPCMState(Acs_Evt_t *pAcsEvt, ACS_Digital_PortState_Data digitStatep);
char *getLSTStr(Acs_GeneralProc_Data *pData);
long find(list *first, DeviceID_t dev, list **local);

#endif
