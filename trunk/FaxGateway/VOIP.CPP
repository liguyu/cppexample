#include "StdAfx.h"
#include "globe.h"
#include "VOIP.H"
#include "DTrunk.h"
#include "XMS_String.h"
#include "SysConfig.h"
#include "PBXGetWay.h"
#include "PBXGetWayDlg.h"
#include "DJAcsISUPDef.h"

extern CPBXGetWayDlg  *pdlg;

extern ACSHandle_t g_acsHandle;
extern FILE * g_logfp ;

extern TYPE_XMS_DSP_DEVICE_RES	AllDeviceRes[MAX_DSP_MODULE_NUM];
extern int						g_iTotalVOIP;
extern int						g_iTotalVOIPOpened;
extern TYPE_CHANNEL_MAP_TABLE	MapTable_VOIP[MAX_VOIP_NUM];
extern	int pubSendRTPDTMFType;
extern long		pubCallInCount;
extern long		pubNormalClearCallCount;
extern long		pubErrorClearCallCount;
extern long		pubIpSS7ConnectCount;
extern long		pubSS7IpConnectCount;
extern long		pubIpSS7ErrorCount;
extern long		pubSS7IpErrorCount;
int             g_bSendFax = 0;
int             g_bRecvFax = 0;

void InitListVOIP(void)
{

	_ASSERT(pdlg != NULL);


	LV_COLUMN	lvc;
	DWORD dwExStyle;
	
	dwExStyle = LVS_EX_FULLROWSELECT | /*LVS_EX_GRIDLINES | LVS_EX_SUBITEMIMAGES |*/
						LVS_EX_HEADERDRAGDROP;// | LVS_EX_TRACKSELECT;
	pdlg->m_ListVOIP.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LPARAM(dwExStyle));	

	lvc.mask =  LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	lvc.iSubItem = 0;
	lvc.pszText = "ID" ;
	lvc.cx = 30;
	pdlg->m_ListVOIP.InsertColumn(0, &lvc); 

	lvc.iSubItem = 1;
	lvc.pszText = "Mod,Ch" ;
	lvc.cx = 50;
	pdlg->m_ListVOIP.InsertColumn(1, &lvc); 

	lvc.iSubItem = 2;
	lvc.pszText = "Type" ;
	lvc.cx = 60;
	pdlg->m_ListVOIP.InsertColumn(2, &lvc); 

	lvc.iSubItem = 3;
	lvc.pszText = "LineState" ;
	lvc.cx = 70;
	pdlg->m_ListVOIP.InsertColumn(3, &lvc); 

	lvc.iSubItem = 4;
	lvc.pszText = "State" ;
	lvc.cx = 70;
	pdlg->m_ListVOIP.InsertColumn(4, &lvc); 

	lvc.iSubItem = 5;
	lvc.pszText = "Protocol" ;
	lvc.cx = 60;
	pdlg->m_ListVOIP.InsertColumn(5, &lvc); 

	lvc.iSubItem = 6;
	lvc.pszText = "Caller/Callee Address";
	lvc.cx = 60;
	pdlg->m_ListVOIP.InsertColumn(6, &lvc); 

	lvc.iSubItem = 7;
	lvc.pszText = "Caller/Callee UserID";
	lvc.cx = 60;
	pdlg->m_ListVOIP.InsertColumn(7, &lvc);

	lvc.iSubItem = 8;
	lvc.pszText = "Caller/Callee TelID";
	lvc.cx = 60;
	pdlg->m_ListVOIP.InsertColumn(8, &lvc);

	lvc.iSubItem = 9;
	lvc.pszText = "DTMF";
	lvc.cx = 60;
	pdlg->m_ListVOIP.InsertColumn(9, &lvc);

	lvc.iSubItem = 10;
	lvc.pszText = "LinkDev" ;
	lvc.cx = 70;
	pdlg->m_ListVOIP.InsertColumn(10, &lvc); 

	lvc.iSubItem = 11;
	lvc.pszText = "VocInfo" ;
	lvc.cx = 70;
	pdlg->m_ListVOIP.InsertColumn(11, &lvc); 

	lvc.iSubItem = 12;
	lvc.pszText = "Fail Reason";
	lvc.cx = 60;
	pdlg->m_ListVOIP.InsertColumn(12, &lvc);	
	
	return;
}

void DrawVOIP_LineState(VOIP_STRUCT* pOneVOIP)
{
	char StateStr[100] = {0};
	int	 iDispRow;


	_ASSERT(pOneVOIP != NULL);
	_ASSERT(pdlg != NULL);


	iDispRow = pOneVOIP->iSeqID; 

	GetString_LineState ( StateStr, pOneVOIP->iLineState );
	pdlg->m_ListVOIP.SetItemText ( iDispRow, 3, StateStr ); 

	return;
}

void DrawVOIP_State(VOIP_STRUCT* pOneVOIP)
{
	char StateStr[100] = {0};
	int	 iDispRow;


	_ASSERT(pOneVOIP != NULL);
	_ASSERT(pdlg != NULL);


	iDispRow = pOneVOIP->iSeqID; 
	
	switch( pOneVOIP->State ) 
	{		
	case VOIP_WAITOPEN:
		strcpy(StateStr,"Wait Open"); 
		break;

	case VOIP_FREE:		
		strcpy(StateStr,"空闲"); 
		break ;	
	
	case VOIP_WAIT_ANSWERCALL:
		strcpy(StateStr,"等摘机完成");
		break;
	case VOIP_ALERT:
		strcpy(StateStr, "振铃");
		break;

	case VOIP_CALLOUT:
		strcpy(StateStr, "呼出");
		break;
	case VOIP_CONNECT:
		strcpy(StateStr, "连通");
		break;

	case VOIP_HANGUP:
		strcpy(StateStr, "挂机");
		break;
	case VOIP_CALLIN:
		strcpy(StateStr,"呼叫进入");
		break;
	case VOIP_WAIT_CALL_TRUNK:
		strcpy(StateStr,"等待呼出");
		break;
	case VOIP_EXCHANGE:
		strcpy(StateStr,"转接7号");
		break;
	default:
		strcpy(StateStr,"other");
		break;
	}

	pdlg->m_ListVOIP.SetItemText ( iDispRow, 4, StateStr ); 
	
	return;
}

void DrawVOIP_CallInfo(VOIP_STRUCT* pOneVOIP)
{
	char StateStr[100] = {0};
	int	 iDispRow;


	_ASSERT(pOneVOIP != NULL);
	_ASSERT(pdlg != NULL);


	iDispRow = pOneVOIP->iSeqID; 

	if (pOneVOIP->iProtocol == 0)
	{
		sprintf(StateStr, "%s", "");
	}
    else if (pOneVOIP->iProtocol == 1)
		sprintf(StateStr, "%s", "H323");
	else 
		sprintf(StateStr, "%s", "SIP");
	pdlg->m_ListVOIP.SetItemText(iDispRow, 5, StateStr);

	if (strcmp(pOneVOIP->CallerAddr, "\0") == 0)
	{
		strcpy(pOneVOIP->CallerAddr, "NULL");
	}

	if (strcmp(pOneVOIP->CalleeAddr, "\0") == 0)
	{
		strcpy(pOneVOIP->CalleeAddr, "NULL");
	}
	
	sprintf(StateStr,"%s:%d/%s:%d", pOneVOIP->CallerAddr, pOneVOIP->iCallerPort,
			pOneVOIP->CalleeAddr, pOneVOIP->iCalleePort);
	pdlg->m_ListVOIP.SetItemText ( iDispRow, 6, StateStr ); 


	if (strcmp(pOneVOIP->CallerUserID, "\0") == 0)
	{
		strcpy(pOneVOIP->CallerUserID, "NULL");
	}

	if (strcmp(pOneVOIP->CalleeUserID, "\0") == 0)
	{
		strcpy(pOneVOIP->CalleeUserID, "NULL");
	}

	sprintf(StateStr,"%s/%s", pOneVOIP->CallerUserID, pOneVOIP->CalleeUserID);
	pdlg->m_ListVOIP.SetItemText ( iDispRow, 7, StateStr );

	if (strcmp(pOneVOIP->CallerNum, "\0") == 0)
	{
		strcpy(pOneVOIP->CallerNum, "NULL");
	}

	if (strcmp(pOneVOIP->CalleeNum, "\0") == 0)
	{
		strcpy(pOneVOIP->CalleeNum, "NULL");
	}
	
	sprintf(StateStr,"%s/%s", pOneVOIP->CallerNum, pOneVOIP->CalleeNum);
	pdlg->m_ListVOIP.SetItemText(iDispRow, 8, StateStr);
	
	return;
}

void DrawVOIP_DTMF(VOIP_STRUCT* pOneVOIP)
{
	char StateStr[100] = {0};
	int	 iDispRow;


	_ASSERT(pOneVOIP != NULL);
	_ASSERT(pdlg != NULL);


	iDispRow = pOneVOIP->iSeqID; 
	if ( iDispRow < 0 )	
		return;

	sprintf(StateStr,"Time:%03d Dtmf:%s",pOneVOIP->TimeOutCount,pOneVOIP->DtmfBuf);
	pdlg->m_ListVOIP.SetItemText (iDispRow, 9, StateStr ); 

	return;
}

void DrawVOIP_LinkDev(VOIP_STRUCT* pOneVOIP)
{
	char StateStr[100] = {0};
	int	 iDispRow;


	_ASSERT(pOneVOIP != NULL);
	_ASSERT(pdlg != NULL);


	iDispRow = pOneVOIP->iSeqID; 
	if ( iDispRow < 0 )	
		return;

	if ( pOneVOIP->LinkDevID.m_s16DeviceMain != 0 )
		sprintf(StateStr, "%d,%d", pOneVOIP->LinkDevID.m_s8ModuleID, pOneVOIP->LinkDevID.m_s16ChannelID);
	else
		sprintf(StateStr, "");
	pdlg->m_ListVOIP.SetItemText(iDispRow,10, StateStr); 

	return;
}

void DrawVOIP_VocInfo(VOIP_STRUCT* pOneVOIP)
{
	char StateStr[100] = {0};
	int	 iDispRow;
	DeviceID_t	*pDev = NULL;


	_ASSERT(pOneVOIP != NULL);
	_ASSERT(pdlg != NULL);


	iDispRow = pOneVOIP->iSeqID; 

	if (pOneVOIP->VocDevID.m_s16DeviceMain != 0)
	{
		pDev = &pOneVOIP->VocDevID;
		sprintf(StateStr, "%d,%d", pDev->m_s8ModuleID, pDev->m_s16ChannelID); 
	}
	else
		strcpy(StateStr, "");

	pdlg->m_ListVOIP.SetItemText(iDispRow, 11, StateStr); 

	return;
}

void DrawVOIP_Reason(VOIP_STRUCT* pOneVOIP, DJ_S32 s32ErrCode, DJ_S32 s32ClearCause)
{
	char StateStr[100] = {0};
	int	 iDispRow;


	_ASSERT(pOneVOIP != NULL);
	_ASSERT(pdlg != NULL);

	
	iDispRow = CalDispRow(pOneVOIP->iSeqID); 
	if(iDispRow < 0)
		return;

	sprintf(StateStr, "Err=%d,Cause=%d", s32ErrCode, s32ClearCause); 
	pdlg->m_ListVOIP.SetItemText(iDispRow, 12, StateStr);

	return;
}

void OpenVOIPDevice(VOIP_STRUCT* pOneVOIP)
{
	RetCode_t	r;
	char        TmpStr[500] = {0};


	_ASSERT(pOneVOIP != NULL);


	pOneVOIP->State= VOIP_WAITOPEN;

	r = XMS_ctsOpenDevice ( g_acsHandle, &pOneVOIP->deviceID, NULL );
	if ( r < 0 )
	{
		sprintf(TmpStr, "XMS_ctsOpenDevice Fail in OpenVOIPDevice()! ret=%d", r);			
	}
	
	return;
}

void CloseVOIPDevice(VOIP_STRUCT* pOneVOIP)
{
	RetCode_t	r;
	char        TmpStr[500] = {0};


	_ASSERT(pOneVOIP != NULL);

		
	pOneVOIP->State= VOIP_WAITOPEN;

	r = XMS_ctsCloseDevice ( g_acsHandle, &pOneVOIP->deviceID, NULL );
	if ( r < 0 )
	{
		sprintf(TmpStr, "XMS_ctsCloseDevice Fail in CloseVOIPDevice()!", r);		
	}
	
	return;
}

void InitVOIPChannel(VOIP_STRUCT* pOneVOIP)
{
	Change_VOIPState ( pOneVOIP, VOIP_FREE );
	My_InitDtmfBuf(pOneVOIP);

	pOneVOIP->bSendFax = 0;
	pOneVOIP->bRecvFax = 0;
	pOneVOIP->bStartFax = false;

	pOneVOIP->iCalleePort = 0;
	pOneVOIP->iCallerPort = 0;
	pOneVOIP->iProtocol = 0;

	pOneVOIP->TimeOutCount = 0;
	//此语音资源用来录音，供测试用
	memset(&pOneVOIP->pVoiceDevRecord, 0, sizeof(pOneVOIP->pVoiceDevRecord));

	//初始化主被叫信息
	memset(pOneVOIP->CallerAddr, 0, sizeof(pOneVOIP->CallerAddr));
	memset(pOneVOIP->CallerUserID, 0, sizeof(pOneVOIP->CallerUserID));
	
	memset(pOneVOIP->CalleeAddr, 0, sizeof(pOneVOIP->CalleeAddr));
	memset(pOneVOIP->CalleeUserID, 0, sizeof(pOneVOIP->CalleeUserID));

	memset(pOneVOIP->CallerNum, 0, sizeof(pOneVOIP->CallerNum));
	memset(pOneVOIP->CalleeNum, 0, sizeof(pOneVOIP->CalleeNum));

///	memset ( &pOneVOIP->VocDevID, 0, sizeof(DeviceID_t) );		// -1表示还没有分配Voc Device
//	memset ( &pOneVOIP->LinkDevID, 0, sizeof(DeviceID_t) );
	DrawVOIP_VocInfo( pOneVOIP );

	return;
}

void Change_VOIPState(VOIP_STRUCT* pOneVOIP, VOIP_STATE NewState)
{

	_ASSERT(pOneVOIP != NULL);


	pOneVOIP->State = NewState;
	DrawVOIP_State(pOneVOIP);

	return;
}

DJ_S32	PlayDTMF(DeviceID_t* pVocDevID, char *DtmfStr)
{
	DJ_U32           i = 0, len;
	DJ_U16           u16IoType = 0;
	DJ_U16           u16IoLen = 0;
	char		 	 IoDataBuf[MAX_SEND_IODATA_DTMF_LEN]={0};
	RetCode_t		 r;

	if (DtmfStr == NULL)	// Stop Play Dtmf
	{
		u16IoLen = 0;
	}
	else
	{
		u16IoType = XMS_IO_TYPE_DTMF;

		len = strlen(DtmfStr);
		if (len > MAX_SEND_IODATA_DTMF_LEN)
			len = MAX_SEND_IODATA_DTMF_LEN;
		u16IoLen = (DJ_U16) len;

		memcpy(IoDataBuf, DtmfStr, len);
	}

	r = XMS_ctsSendIOData(g_acsHandle, pVocDevID, u16IoType,u16IoLen,IoDataBuf);

	return	r;

}

DJ_S32 StopPlayDTMF(DeviceID_t* pVocDevID)
{
	return PlayDTMF ( pVocDevID, NULL );
}

void My_InitDtmfBuf(VOIP_STRUCT* pOneVOIP)
{
	pOneVOIP->DtmfCount = 0;
	pOneVOIP->DtmfBuf[0] = 0; 
}

void My_AddDtmfBuf(VOIP_STRUCT* pOneVOIP, char TmpDtmf)
{
	if (pOneVOIP->DtmfCount < (32-1))
	{
		pOneVOIP->DtmfBuf[pOneVOIP->DtmfCount] = TmpDtmf; 
		pOneVOIP->DtmfBuf[pOneVOIP->DtmfCount+1] = 0; 
		pOneVOIP->DtmfCount++;
	}
}

char My_GetDtmfCode(Acs_Evt_t *pAcsEvt)
{
	Acs_VoIP_H245UII *h245Uii = NULL;
	Acs_VoIP_SIPINFO *sipInfo = NULL;
	Acs_VoIP_RTPDTMF *rtpDtmf = NULL;
	char    p;

	Acs_IO_Data *pIOEvt = (Acs_IO_Data *)FetchEventData(pAcsEvt);
	
	switch (pIOEvt->m_u16IoType)
	{
	case XMS_IO_TYPE_VOIP_H245UII:
		if (pIOEvt->m_u16IoDataLen == sizeof(Acs_VoIP_H245UII))
		{
			h245Uii = (Acs_VoIP_H245UII *)FetchIOData(pAcsEvt);
			
			return  p = strlen(h245Uii->m_s8String) == 0 ? h245Uii->m_s8Signal : h245Uii->m_s8String[0];
		}
		break;

	case XMS_IO_TYPE_VOIP_SIPINFO:
		if (pIOEvt->m_u16IoDataLen == sizeof(Acs_VoIP_SIPINFO))
		{
			sipInfo = (Acs_VoIP_SIPINFO *)FetchIOData(pAcsEvt);
			if (strcmp(sipInfo->m_s8ContentType, "application/dtmf") == 0)
				return p = sipInfo->m_s8Body[0];		
		}
		break;

	case XMS_IO_TYPE_VOIP_RTPDTMF:
		if (pIOEvt->m_u16IoDataLen == sizeof(Acs_VoIP_RTPDTMF))
		{
			rtpDtmf = (Acs_VoIP_RTPDTMF *)FetchIOData(pAcsEvt);
			return p = rtpDtmf->m_s8EventChar;
		}
		break;
	}

	return -1;
}

void ResetVOIP(VOIP_STRUCT* pOneVOIP, Acs_Evt_t* pAcsEvt)
{

	_ASSERT(pOneVOIP != NULL);
	_ASSERT(pAcsEvt != NULL);

	InitVOIPChannel(pOneVOIP);

	return;
}

void VOIPWork(VOIP_STRUCT *pOneVOIP, Acs_Evt_t *pAcsEvt)
{
	Acs_CallControl_Data   *pCallControl = NULL;
	VoIPCallPrivate_t      *voipCallPrivate = NULL;	
	DeviceID_t              FreeInterfaceDev;
	INTERFACE_STRUCT       *pOneInterface = NULL;
	int                     i;
	RetCode_t               ret;
	char                    TmpDtmf[256] = {0};
	char                    callerNum[20] = {0};
	char                    calledNum[20] = {0};

	VOICE_STRUCT *pOneVoice = NULL;

try
{
	if(pAcsEvt->m_s32EventType == XMS_EVT_CLEARCALL )	/*拆线事件*/
	{	
		//处理录音
		if (pOneVOIP->pVoiceDevRecord.m_s8ModuleID == pOneVOIP->deviceID.m_s8ModuleID)
			RecordEndProcess(pOneVOIP);	
		
		if (g_logfp != NULL)
		{
			char	mLog[512]={0};
			char	mCurrTime[50]={0};
			getCurrTime(mCurrTime);
			
			sprintf(mLog,"%s VOIP receive ClearCall:Dsp:%d Ch:%d GlobalID:%d CallerNum:%s CalledNum:%s\n\r",
				mCurrTime,
				pOneVOIP->deviceID.m_s8ModuleID,
				pOneVOIP->deviceID.m_s16ChannelID,
				pOneVOIP->iGlobeID,
				pOneVOIP->CallerNum,
				pOneVOIP->CalleeNum);
			fwrite(mLog,strlen(mLog),1,g_logfp);
			fflush(g_logfp);
		}

		if (pOneVOIP->bSendFax == 1)
		{			
			XMS_ctsStopSendFax(g_acsHandle, &pOneVOIP->pFaxDeviceID, NULL);			
			pOneVOIP->bSendFax = 0;			
			pOneVOIP->bStartFax = false;
		}

		if (pOneVOIP->bRecvFax == 1)
		{
			XMS_ctsStopReceiveFax(g_acsHandle, &pOneVOIP->pFaxDeviceID, NULL);
			pOneVOIP->bRecvFax = 0;
			pOneVOIP->bStartFax = false;
		}	

		if(pOneVOIP->mExchangeType == VOIP_SS7)
		{
			pOneInterface = &M_OneInterface(pOneVOIP->pTrunkDeviceID);
			if (pOneInterface->iLineState == DCS_FREE || pOneInterface->iLineState == DCS_WAIT_FREE || pOneInterface->iLineState == DCS_DISCONNECT)
			{
				pubNormalClearCallCount++;
				ResetVOIP(pOneVOIP, pAcsEvt);
				return ; 
			}
		
			if ((pOneInterface->iLineState != DCS_FREE ) && 
				(pOneInterface->bcallFailFalg == false))
			{
				ret = XMS_ctsClearCall(g_acsHandle, &pOneInterface->deviceID, 0, NULL);	

				if (g_logfp != NULL)
				{
					char	mLog[512]={0};
					char	mCurrTime[50]={0};
					char    StateStr[50] = {0};
					getCurrTime(mCurrTime);
					GetString_LineState(StateStr, pOneInterface->iLineState);
					sprintf(mLog,"%s Call Direct: voip callin and VOIP receive clearCall, clear SS7.voip-ss7: (IPDsp:%d IPCh %d SS7Dsp:%d SS7Ch:%d)(CallerNum:%s CalledNum:%s), ss7 state: %s\n\r",
					mCurrTime,
					pOneVOIP->deviceID.m_s8ModuleID,
					pOneVOIP->deviceID.m_s16ChannelID,
					pOneVOIP->pTrunkDeviceID.m_s8ModuleID,
					pOneVOIP->pTrunkDeviceID.m_s16ChannelID,					
					pOneVOIP->CallerNum,
					pOneVOIP->CalleeNum,
					StateStr);
					fwrite(mLog,strlen(mLog),1,g_logfp);
					fflush(g_logfp);
				}
				TRACE("6 ClearCall SS7\n");
			}

			pOneInterface->bcallFailFalg = false;		
		}
		
		long mDspID=pOneVOIP->pTrunkDeviceID.m_s8ModuleID;
		long mChannelID=pOneVOIP->pTrunkDeviceID.m_s16ChannelID;

		if(AllDeviceRes[mDspID].pInterf[mChannelID].mExchangeType == SS7_VOIP)
		{
			pOneInterface = &M_OneInterface(pOneVOIP->pTrunkDeviceID);

			if (pOneInterface->iLineState == DCS_FREE || pOneInterface->iLineState == DCS_WAIT_FREE || pOneInterface->iLineState == DCS_DISCONNECT)
			{
				pubNormalClearCallCount++;
				ResetVOIP(pOneVOIP, pAcsEvt);
				return ; 
			}
			
			if ((pOneInterface->iLineState != DCS_FREE))
			{
				ret = XMS_ctsClearCall(g_acsHandle, &pOneInterface->deviceID, 0, NULL);
				
				if (g_logfp != NULL)
				{
					char	mLog[512]={0};
					char	mCurrTime[50]={0};
					char    StateStr[50]={0};
					getCurrTime(mCurrTime);
					
					GetString_LineState(StateStr, pOneInterface->iLineState);
					sprintf(mLog,"%s Call Direct: SS7 callin and voip receive ClearCall, clear SS7.voip-ss7: (IPDsp:%d IPCh:%d SS7Dsp:%d"\
						 "SS7Ch:%d)(CallerNum:%s CalledNum:%s) ss7 state:%s\n\r",
						mCurrTime,
						pOneVOIP->deviceID.m_s8ModuleID,
						pOneVOIP->deviceID.m_s16ChannelID,
						pOneVOIP->pTrunkDeviceID.m_s8ModuleID,
						pOneVOIP->pTrunkDeviceID.m_s16ChannelID,					
						pOneVOIP->CallerNum,
						pOneVOIP->CalleeNum,
						StateStr);
					fwrite(mLog,strlen(mLog),1,g_logfp);
					fflush(g_logfp);
				}
				
		     	TRACE("7 ClearCall SS7\n");
			}
		}		
		
		pubNormalClearCallCount++;

		ResetVOIP(pOneVOIP, pAcsEvt);
		return ; 
	}

	switch(pOneVOIP->State)
	{
	case VOIP_FREE:
		/*call in from voip*/
		if (pAcsEvt->m_s32EventType == XMS_EVT_CALLIN)	/*呼入事件*/
		{		
			//不做修改
			pCallControl = (Acs_CallControl_Data *)FetchEventData(pAcsEvt);
		
			Change_VOIPState(pOneVOIP,VOIP_CALLIN);

			pOneVOIP->mExchangeType = VOIP_SS7;		//方向

			pubCallInCount++;		//统计呼入的个数

			ASSERT(pCallControl->m_PrivData.m_u32DataSize == sizeof(VoIPCallPrivate_t));
			voipCallPrivate = (VoIPCallPrivate_t *)&(pCallControl->m_PrivData);	
			
			pOneVOIP->iProtocol = voipCallPrivate->m_s32Protocol;
			
			//Caller Info caller主叫  callee被叫
			strncpy(pOneVOIP->CallerAddr, voipCallPrivate->m_s8CallerAddress, sizeof(pOneVOIP->CallerAddr));			
			strncpy(pOneVOIP->CallerUserID, voipCallPrivate->m_s8CallerUserID, sizeof(pOneVOIP->CallerUserID));
			strncpy ( pOneVOIP->CallerNum, pCallControl->m_s8CallingNum, sizeof(pOneVOIP->CallerNum));
			pOneVOIP->iCallerPort = (unsigned short)voipCallPrivate->m_s32CallerPort;
		
			
			//Callee Infoli
			strncpy(pOneVOIP->CalleeAddr, voipCallPrivate->m_s8CalleeAddress, sizeof(pOneVOIP->CalleeAddr));			
			strncpy(pOneVOIP->CalleeUserID, voipCallPrivate->m_s8CalleeUserID, sizeof(pOneVOIP->CalleeUserID));
			strncpy(pOneVOIP->CalleeNum, pCallControl->m_s8CalledNum, sizeof(pOneVOIP->CalleeNum));			
			pOneVOIP->iCalleePort = (unsigned short)voipCallPrivate->m_s32CalleePort;			
			
			if (g_logfp != NULL)
			{
				char	mLog[512]={0};
				char	mCurrTime[50]={0};
				getCurrTime(mCurrTime);
				
				sprintf(mLog,"%s voip callin:Dsp:%d Ch:%d GlobalID:%d CallerNum:%s CalledNum:%s\n\r",
					mCurrTime, 
					pOneVOIP->deviceID.m_s8ModuleID,
					pOneVOIP->deviceID.m_s16ChannelID,
					pOneVOIP->iGlobeID,
					pOneVOIP->CallerNum,
					pOneVOIP->CalleeNum);
				
				fwrite(mLog,strlen(mLog),1,g_logfp);
				fflush(g_logfp);
			}

			DrawVOIP_CallInfo(pOneVOIP);
			
			if (IsValidIP(voipCallPrivate->m_s8CallerAddress))
			{		
				i=1;
				FreeInterfaceDev=pOneVOIP->pTrunkDeviceID;
				if (i >= 0)
				{
					strncpy(callerNum, pOneVOIP->CallerNum, 20);
					strncpy(calledNum, pOneVOIP->CalleeNum, 20);
					pOneInterface = &AllDeviceRes[pOneVOIP->pTrunkDeviceID.m_s8ModuleID].pInterf[pOneVOIP->pTrunkDeviceID.m_s16ChannelID];
					
					//中继为空闲状态
				    if(pOneInterface->iLineState== DCS_FREE && pOneInterface->DTrkState == TRK_FREE)
					{
						if (g_logfp != NULL)
						{
							char	mLog[512]={0};
							char	mCurrTime[50]={0};
							getCurrTime(mCurrTime);
							
							sprintf(mLog,"%s voip callin2:Dsp:%d Ch:%d GlobalID:%d CallerNum:%s CalledNum:%s\n\r",
								mCurrTime, 
								pOneVOIP->deviceID.m_s8ModuleID,
								pOneVOIP->deviceID.m_s16ChannelID,
								pOneVOIP->iGlobeID,
								callerNum,
								pOneVOIP->CalleeNum);
							
							fwrite(mLog,strlen(mLog),1,g_logfp);
							fflush(g_logfp);
						}
						ret = IPToPSTNCallOut(callerNum,pOneVOIP->CalleeNum,&AllDeviceRes[pOneInterface->deviceID.m_s8ModuleID].pInterf[pOneInterface->deviceID.m_s16ChannelID].deviceID, pOneVOIP); 
						if (ret > 0)
						{
							DrawVOIP_LinkDev (pOneVOIP);						

							strcpy(pOneInterface->CallerCode, pCallControl->m_s8CallingNum);
							strcpy(pOneInterface->CalleeCode, pCallControl->m_s8CalledNum);
							DrawInterface_CallInfo(pOneInterface);
							DrawInterface_LinkDev(pOneInterface);						
							Change_DTrunkState(pOneInterface,TRK_CALLOUT);
							Change_VOIPState(pOneVOIP,VOIP_EXCHANGE);
						}
						else
						{	
							pubErrorClearCallCount++;			//异常						

							XMS_ctsClearCall(g_acsHandle, &pOneVOIP->deviceID, 0, NULL);
							Change_VOIPState(pOneVOIP, VOIP_HANGUP);						

							if (g_logfp != NULL)
							{
								char	mTemp[512]={0};
								char	mCurrTime[50]={0};
								getCurrTime(mCurrTime);
								
								sprintf(mTemp,"%s (first) voip call in and find out call SS7 function error,so clear voip:(voip<->ss7)voipDsp:%d voipCh:%d ss7Dsp:%d ss7Ch:%d(CallerNum:%s CalledNum:%s)\n\r",
									mCurrTime,
									pOneVOIP->deviceID.m_s8ModuleID,
									pOneVOIP->deviceID.m_s16ChannelID,
									pOneVOIP->pTrunkDeviceID.m_s8ModuleID,
									pOneVOIP->pTrunkDeviceID.m_s16ChannelID,
									pOneVOIP->CallerNum,
									pOneVOIP->CalleeNum);
								
								fwrite(mTemp,strlen(mTemp),1,g_logfp);
								fflush(g_logfp);
							}
						}
					}
					else
					{
						pOneVOIP->TimeOutCount = 0;				
						XMS_ctsSetDevTimer(g_acsHandle,&pOneVOIP->deviceID,700);
						Change_VOIPState(pOneVOIP,VOIP_WAIT_CALL_TRUNK);
						DrawVOIP_DTMF(pOneVOIP);
					}
				}
				else
				{
					XMS_ctsClearCall(g_acsHandle, &pOneVOIP->deviceID, 0, NULL);
					Change_VOIPState(pOneVOIP, VOIP_HANGUP);

					if (g_logfp != NULL)
					{
						char	mLog[512]={0};
						char	mCurrTime[50]={0};
						getCurrTime(mCurrTime);
						
						sprintf(mLog,"%s voip CallIn and not ss7 device,so clear voip. voipDsp:%d voipCh:%d CallerNum:%s CalledNum:%s\n\r",
							mCurrTime,
							pOneVOIP->deviceID.m_s8ModuleID,
							pOneVOIP->deviceID.m_s16ChannelID,							
							pOneVOIP->CallerNum,
							pOneVOIP->CalleeNum);

						fwrite(mLog,strlen(mLog),1,g_logfp);
						fflush(g_logfp);
					}
										
				}			
			}
			else
			{
				XMS_ctsClearCall(g_acsHandle, &pOneVOIP->deviceID, 0, NULL);
				Change_VOIPState(pOneVOIP, VOIP_HANGUP);

				if (g_logfp != NULL)
				{
					char	mLog[512]={0};
					char	mCurrTime[50]={0};
					getCurrTime(mCurrTime);
					
					sprintf(mLog,"%s voip callin and clear voip  because caller Ip is rejected :voipDsp:%d voipCh:%d CallerNum:%s CalledNum:%s\n\r",
						mCurrTime,
						pOneVOIP->deviceID.m_s8ModuleID,
						pOneVOIP->deviceID.m_s16ChannelID,							
						pOneVOIP->CallerNum,
						pOneVOIP->CalleeNum);

					fwrite(mLog,strlen(mLog),1,g_logfp);
					fflush(g_logfp);
				}				
			}
		}
	
		break;

	case VOIP_WAIT_ANSWERCALL:		
		if ( pAcsEvt->m_s32EventType == XMS_EVT_ANSWERCALL )	
		{		
			Change_VOIPState(pOneVOIP, VOIP_CONNECT);		
		}
		break;

	/*call out from voip*/
	case VOIP_WAIT_CALL_TRUNK:
		{
			if(pOneVOIP->iLineState == DCS_FREE )
			{
				if (pOneVOIP->mExchangeType == VOIP_SS7)
				{
					long	mDspID = pOneVOIP->deviceID.m_s8ModuleID;
					INTERFACE_STRUCT *pOneTrunk = & AllDeviceRes[mDspID].pInterf[pOneVOIP->pTrunkDeviceID.m_s16ChannelID];
					if (pOneTrunk->iLineState != DCS_FREE)			
					{						
						XMS_ctsClearCall(g_acsHandle, &pOneTrunk->deviceID, 0, NULL);					
						DrawInterface_LinkDev(pOneTrunk);

						if (g_logfp != NULL)
						{
							char	mLog[512]={0};
							char	mCurrTime[50]={0};
							getCurrTime(mCurrTime);
							
							sprintf(mLog,"%s when voip wait call ss7 and the voip linestate is free,so clear SS7.(voip<->ss7)voipDsp:%d voipCh:%d ss7Dsp:%d ss7Ch:%d(CallerNum:%s CalledNum:%s)\n\r",
								mCurrTime,
								pOneVOIP->deviceID.m_s8ModuleID,
								pOneVOIP->deviceID.m_s16ChannelID,
								pOneVOIP->pTrunkDeviceID.m_s8ModuleID,
								pOneVOIP->pTrunkDeviceID.m_s16ChannelID,
								pOneVOIP->CallerNum,
								pOneVOIP->CalleeNum);
							
							fwrite(mLog,strlen(mLog),1,g_logfp);
							fflush(g_logfp);
						}
					}
				}
				
				ResetVOIP(pOneVOIP,pAcsEvt);
			}

			if(pAcsEvt->m_s32EventType == XMS_EVT_DEV_TIMER)
			{
				pOneVOIP->TimeOutCount++;
				DrawVOIP_DTMF(pOneVOIP);

				if(pOneVOIP->TimeOutCount == 4 )
				{
					XMS_ctsClearCall(g_acsHandle,&pOneVOIP->deviceID,0,NULL);
					pOneVOIP->TimeOutCount = 0;
					
					if (g_logfp != NULL)
					{
						char	mLog[512]={0};
						char	mCurrTime[50]={0};
						getCurrTime(mCurrTime);
						
						sprintf(mLog,"%s voip callin and wait timer overtime, so clear voip.(voip<->ss7)voipDsp:%d voipCh:%d ss7Dsp:%d ss7Ch:%d(CallerNum:%s CalledNum:%s)\n\r",
							mCurrTime,
							pOneVOIP->deviceID.m_s8ModuleID,
							pOneVOIP->deviceID.m_s16ChannelID,
							pOneVOIP->pTrunkDeviceID.m_s8ModuleID,
							pOneVOIP->pTrunkDeviceID.m_s16ChannelID,
							pOneVOIP->CallerNum,
							pOneVOIP->CalleeNum);
						
						fwrite(mLog,strlen(mLog),1,g_logfp);
						fflush(g_logfp);
					}
					return;
				}

				strncpy(callerNum, pOneVOIP->CallerNum, 20);
				strncpy(calledNum, pOneVOIP->CalleeNum, 20);
				pOneInterface = &AllDeviceRes[pOneVOIP->deviceID.m_s8ModuleID].pInterf[pOneVOIP->pTrunkDeviceID.m_s16ChannelID];

				if(pOneInterface->iLineState == DCS_FREE && pOneInterface->DTrkState == TRK_FREE)
				{
					ret = IPToPSTNCallOut(callerNum, 
						                  calledNum, 
										  &AllDeviceRes[pOneInterface->deviceID.m_s8ModuleID].pInterf[pOneInterface->deviceID.m_s16ChannelID].deviceID,
										  pOneVOIP); 				
					if (ret > 0)
					{
						DrawVOIP_LinkDev (pOneVOIP);						

						DrawInterface_CallInfo(pOneInterface);
						DrawInterface_LinkDev(pOneInterface);						
						Change_DTrunkState(pOneInterface,TRK_CALLOUT);
					}
					else
					{
						XMS_ctsClearCall(g_acsHandle, &pOneVOIP->deviceID, 0, NULL);
						Change_VOIPState(pOneVOIP, VOIP_HANGUP);						

						if (g_logfp != NULL)
						{
							char	mLog[512]={0};
							char	mCurrTime[50]={0};
							getCurrTime(mCurrTime);
							
							sprintf(mLog,"%s second voip call in and find out call SS7 function SS7 error,so clear voip:(voip<->ss7)voipDsp:%d voipCh:%d ss7Dsp:%d ss7Ch:%d CallerNum:%s CalledNum:%s\n\r",
								mCurrTime,
								pOneVOIP->deviceID.m_s8ModuleID,
								pOneVOIP->deviceID.m_s16ChannelID,
								pOneVOIP->pTrunkDeviceID.m_s8ModuleID,
								pOneVOIP->pTrunkDeviceID.m_s16ChannelID,
								pOneVOIP->CallerNum,
								pOneVOIP->CalleeNum);
							
							fwrite(mLog,strlen(mLog),1,g_logfp);
							fflush(g_logfp);
						}
					}
				}
				else
				{
					XMS_ctsSetDevTimer(g_acsHandle,&pOneVOIP->deviceID,200);
					Change_VOIPState(pOneVOIP,VOIP_WAIT_CALL_TRUNK);
					return;
				}

			}
		}
		break;
	case VOIP_CALLOUT:
		{
			pOneInterface = &AllDeviceRes[pOneVOIP->pTrunkDeviceID.m_s8ModuleID].pInterf[pOneVOIP->pTrunkDeviceID.m_s16ChannelID];
			
			if (pAcsEvt->m_s32EventType == XMS_EVT_CALLOUT)
			{
				pCallControl = (Acs_CallControl_Data *)FetchEventData(pAcsEvt);			
				
				if (pCallControl->m_s32AcsEvtState == 1)	// 呼出成功，对方摘机
				{				
					pubSS7IpConnectCount++;

					if (g_logfp != NULL)
					{
						char	mLog[512]={0};
						char	mCurrTime[50]={0};
						getCurrTime(mCurrTime);
						
						sprintf(mLog,"%s voip callout success :Dsp:%d Ch:%d GlobalID:%d CallerNum:%s CalledNum:%s \n\r",
							mCurrTime,
							pOneVOIP->deviceID.m_s8ModuleID,
							pOneVOIP->deviceID.m_s16ChannelID,
							pOneVOIP->iGlobeID,
							pOneVOIP->CallerNum,
							pOneVOIP->CalleeNum);
						fwrite(mLog,strlen(mLog),1,g_logfp);
						fflush(g_logfp);
					}
				
					DrawVOIP_VocInfo(pOneVOIP);					

					/*Trunk*/
					if ((pOneInterface->deviceID.m_s16DeviceSub != XMS_DEVSUB_ANALOG_USER))
					{	
						XMS_ctsAnswerCallIn(g_acsHandle, &pOneInterface->deviceID, NULL);						
					}

					Change_VOIPState(pOneVOIP,VOIP_CONNECT);
				}
				else		// 呼出失败
				{			
					pubSS7IpErrorCount++;

					DrawVOIP_VocInfo(pOneVOIP);
					
					DrawVOIP_Reason(pOneVOIP, pCallControl->m_s32AcsEvtErrCode, pCallControl->m_s32CallClearCause );
					Change_VOIPState(pOneVOIP, VOIP_FREE);	
					DrawVOIP_LinkDev(pOneVOIP);
				
					pOneInterface = &M_OneInterface(pOneVOIP->pTrunkDeviceID);				
					
					ret = XMS_ctsClearCall(g_acsHandle, &pOneInterface->deviceID, 0, NULL);	
					TRACE("8 ClearCall SS7\n");;

					if (pOneVOIP->VocDevID.m_s8ModuleID > 0)
						My_DualUnlink(&pOneVOIP->LinkDevID, &pOneVOIP->pVoiceDeviceID);

					if (g_logfp != NULL)
					{
						char	mTemp[512]={0};
						char	mCurrTime[50]={0};
						getCurrTime(mCurrTime);
						
						sprintf(mTemp,"%s Direct SS7 callin, ip call out failed  so  clear ss7!! voip:(voip<->ss7)voipDsp:%d voipCh:%d ss7Dsp:%d ss7Ch:%d(CallerNum:%s CalledNum:%s) ErrorCode:%d Reason:%d\n\r",
							mCurrTime,
							pOneVOIP->deviceID.m_s8ModuleID,
							pOneVOIP->deviceID.m_s16ChannelID,
							pOneVOIP->pTrunkDeviceID.m_s8ModuleID,
							pOneVOIP->pTrunkDeviceID.m_s16ChannelID,
							pOneVOIP->CallerNum,
							pOneVOIP->CalleeNum,
							pCallControl->m_s32AcsEvtErrCode,
							pCallControl->m_s32CallClearCause);
						
						fwrite(mTemp,strlen(mTemp),1,g_logfp);
						fflush(g_logfp);
					}
				}
			}
		}
		break;

	/*voip is connected*/
	case VOIP_CONNECT:
		{	
			if(pOneVOIP->iLineState == DCS_FREE )
			{
				if (pOneVOIP->mExchangeType == VOIP_SS7)
				{
					long	mDspID = pOneVOIP->deviceID.m_s8ModuleID;
					INTERFACE_STRUCT *pOneTrunk = & AllDeviceRes[mDspID].pInterf[pOneVOIP->pTrunkDeviceID.m_s16ChannelID];
					if (pOneTrunk->iLineState != DCS_FREE)			
					{
						XMS_ctsClearCall(g_acsHandle, &pOneTrunk->deviceID, 0, NULL);						
						DrawInterface_LinkDev(pOneTrunk);

						if (g_logfp != NULL)
						{
							char	mLog[512]={0};
							char	mCurrTime[50]={0};
							getCurrTime(mCurrTime);
							
							sprintf(mLog,"%s voip UserState is CONNECTED but LineState is free, so clear ss7 call :Dsp:%d Ch:%d GlobalID:%d CallerNum:%s CalledNum:%d\n\r",
								mCurrTime,
								pOneVOIP->deviceID.m_s8ModuleID,
								pOneVOIP->deviceID.m_s16ChannelID,
								pOneVOIP->iGlobeID,
								pOneVOIP->CallerNum,
								pOneVOIP->CalleeNum);

							fwrite(mLog,strlen(mLog),1,g_logfp);
							fflush(g_logfp);
						}
					}
				}	
				ResetVOIP(pOneVOIP,pAcsEvt);
			}

			//T38 GATWAY
			T38GatWayWork(pOneVOIP, pAcsEvt);

		//	FaxEndProcess(pOneVOIP, pAcsEvt);
		
			TmpDtmf[0] = My_GetDtmfCode ( pAcsEvt );
			if ( TmpDtmf[0] != -1 )								//DTMF按键
			{	
				if(pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_VOIP )
				{
					PlayDTMF(&pOneVOIP->pVoiceDeviceID, TmpDtmf);
					My_AddDtmfBuf(pOneVOIP, TmpDtmf[0]);
					DrawVOIP_DTMF(pOneVOIP);					

					if (pOneVOIP->DtmfCount > 8)
					{	
						DrawVOIP_DTMF(pOneVOIP);
						My_InitDtmfBuf(pOneVOIP);					
					}
				}
			}	
		}
		break;
	}
}catch(CException &e)
{
	char	mTemp[512]={0};
	char	mCurrTime[50]={0};
	if (g_logfp != NULL)
	{
		getCurrTime(mCurrTime);
		
		sprintf(mTemp,"%s VOIPWORK RUN ERROR\n\r",mCurrTime);
		e.ReportError();
		fwrite(mTemp,strlen(mTemp),1,g_logfp);
		fflush(g_logfp);
	}
}
return;
}

int IPToPSTNCallOut(char* callerNum, char* calledNum, DeviceID_t* pInterfaceDev, VOIP_STRUCT* pOneVOIP)
{
	RetCode_t ret = -1;
	INTERFACE_STRUCT *pOneInterface = NULL;
	DeviceID_t       FreeVocDeviceID = {0};
	MixerControlParam_t     VocCmdP = {0};
	CmdParamData_Voice_t    VocMixParam = {0};
	VoIPCallPrivate_t       voipCallParam = {0};

	if (g_logfp != NULL)
	{
		char	mTemp[512]={0};
		char	mCurrTime[50]={0};
		getCurrTime(mCurrTime);
		sprintf(mTemp,"%s call pstn: m_s16DeviceMain=%d,m_s16DeviceSub=%d,m_s8ModuleID=%d,m_s8MachineID=%d,m_s16ChannelID=%d,m_s32FlowType=%d,m_s32FlowChannel=%d\n\r",
			mCurrTime,
			pInterfaceDev->m_s16DeviceMain,
			pInterfaceDev->m_s16DeviceSub,
			pInterfaceDev->m_s8ModuleID,
			pInterfaceDev->m_s8MachineID,
			pInterfaceDev->m_s16ChannelID,
			pInterfaceDev->m_CallID.m_s32FlowType,
			pInterfaceDev->m_CallID.m_s32FlowChannel);
		fwrite(mTemp,strlen(mTemp),1,g_logfp);
		fflush(g_logfp);
	}

	ISUP_spCallingPartNumber callingPartNumber = {0};
	callingPartNumber.m_u8NatureOfAddressIndicator = 1;
	callingPartNumber.m_u8OddEvenIndicator = 0;
	callingPartNumber.m_u8Screening = 3;
	callingPartNumber.m_u8AddressPresentationRestrictedIndicator = 0;
	callingPartNumber.m_u8NumberingPlanIndicator = 1;
	callingPartNumber.m_u8NumberIncompleteIndicator = 0;
	strncpy(callingPartNumber.m_s8AddressSignal, callerNum, sizeof(callingPartNumber.m_s8AddressSignal));
	
	if (XMS_ctsSetParam(g_acsHandle, pInterfaceDev, ISUP_SP_CallingPartNumber,sizeof(callingPartNumber), &callingPartNumber) < 0)
	{
		if (g_logfp != NULL)
		{
			char	mTemp[512]={0};
			char	mCurrTime[50]={0};
			getCurrTime(mCurrTime);

			sprintf(mTemp,"%s Set callingpartNumber error: m_s16DeviceMain=%d,m_s16DeviceSub=%d,m_s8ModuleID=%d,m_s8MachineID=%d,m_s16ChannelID=%d,m_s32FlowType=%d,m_s32FlowChannel=%d\n\r",
				mCurrTime,
				pInterfaceDev->m_s16DeviceMain,
				pInterfaceDev->m_s16DeviceSub,
				pInterfaceDev->m_s8ModuleID,
				pInterfaceDev->m_s8MachineID,
				pInterfaceDev->m_s16ChannelID,
				pInterfaceDev->m_CallID.m_s32FlowType,
				pInterfaceDev->m_CallID.m_s32FlowChannel);
			fwrite(mTemp,strlen(mTemp),1,g_logfp);
			fflush(g_logfp);
		}
		
		return ret;
	}
		
	ret = XMS_ctsMakeCallOut(g_acsHandle, pInterfaceDev, callerNum, calledNum,  NULL);
	if(ret > 0)
	{			
		//* hw for test	

		pOneInterface = &M_OneInterface(*pInterfaceDev);
			//发起呼叫，防止乱拆线
	//	pOneInterface->bcallFailFalg = true;		
		{			
			VocCmdP.m_u8SRC1_Ctrl = XMS_MIXER_FROM_INPUT;							
			VocCmdP.m_u16SRC_ChID1 = pOneVOIP->pVoiceDeviceID.m_s16ChannelID;
		//如果不屏蔽这两行，会造成DTMF播放死循环
		//	VocCmdP.m_u8SRC2_Ctrl = XMS_MIXER_FROM_PLAY;
		//	VocCmdP.m_u16SRC_ChID2 = pOneVOIP->pVoiceDeviceID.m_s16ChannelID;
			ret = XMS_ctsSetParam(g_acsHandle, &pOneVOIP->deviceID,
				VOIP_PARAM_RTPMIXER, sizeof(VocCmdP), &VocCmdP);
			
		/*	VocMixParam.m_u8InputCtrlValid=1;             //使能输入控制参数
			VocMixParam.m_VocInputControl.m_u8AgcEnable=0;// inputContol使能自动增益
			VocMixParam.m_VocInputControl.m_u8AgcMode = 0 ;//设为ALS模式
			VocMixParam.m_VocInputControl.m_u16FixGain=1024;
			VocMixParam.m_VocInputControl.m_u8EcEnable=0;//禁止回声抑制
			VocMixParam.m_VocInputControl.m_u8TadEnable=1;   //启动语音TONE音抑制使能
			VocMixParam.m_VocInputControl.m_u8EcRefType=0;*/

			VocMixParam.m_u8GtdCtrlValid = 1;
			VocMixParam.m_VocGtdControl.m_u8DTMFEnable = 1;
			VocMixParam.m_VocGtdControl.m_u8FSKEnable = 1;
			VocMixParam.m_VocGtdControl.m_u8ChannelEnable = 1;
			VocMixParam.m_VocGtdControl.m_u8GTDEnable = 1;
			unsigned char gtdID[] = {"GHIJKLM"};
			memcpy(VocMixParam.m_VocGtdControl.m_u8GTDID, gtdID, sizeof(VocMixParam.m_VocGtdControl.m_u8GTDID));
		//	EnableGtg(&pOneVOIP->pVoiceDeviceID);			
			
			VocMixParam.m_u8OutputCtrlValid = 1;
			VocMixParam.m_VocOutputControl.m_u8OutputType = XMS_VOC_OUTPUT_FROM_MIXER;				
			VocMixParam.m_VocOutputControl.m_u16FixGain = 1024;
			VocMixParam.m_VocOutputControl.m_MixerControl.m_u8SRC1_Ctrl = XMS_MIXER_FROM_PLAY;
			VocMixParam.m_VocOutputControl.m_MixerControl.m_u16SRC_ChID1 = pOneVOIP->pVoiceDeviceID.m_s16ChannelID;
			VocMixParam.m_VocOutputControl.m_MixerControl.m_u8SRC2_Ctrl = XMS_MIXER_FROM_IP;
			VocMixParam.m_VocOutputControl.m_MixerControl.m_u16SRC_ChID2 = pOneVOIP->deviceID.m_s16ChannelID;
			ret = XMS_ctsSetParam(g_acsHandle, &pOneVOIP->pVoiceDeviceID,VOC_PARAM_UNIPARAM, sizeof(VocMixParam), &VocMixParam);

			My_DualLink(&pOneVOIP->pTrunkDeviceID, &pOneVOIP->pVoiceDeviceID);	
			
			XMS_ctsAlertCall(g_acsHandle,&pOneVOIP->deviceID, NULL);
		
			if(ret < 0)
			{
				printf("");
			}
			
			DrawVOIP_VocInfo(pOneVOIP);			
		}
		//*/
	}

	return ret;
}

int T38GatWayWork(VOIP_STRUCT *pOneVOIP, Acs_Evt_t *pAcsEvt)
{	
	FaxPrivate_t fax;
	Acs_IO_Data *pIOEvt = NULL;
	RetCode_t ret = -1;	

	_ASSERT(pOneVOIP != NULL);
	_ASSERT(pAcsEvt != NULL);

	if (pOneVOIP->deviceID.m_s8ModuleID > 0)
	{
		if (pAcsEvt->m_s32EventType == XMS_EVT_RECVIODATA)
		{		
			pIOEvt = (Acs_IO_Data *)FetchEventData(pAcsEvt);
			
			if ((pIOEvt->m_u16IoType == XMS_IO_TYPE_VOIP_T38START) &&
				(pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_VOIP))
			{
				memset(&fax, 0, sizeof(FaxPrivate_t));
				fax.m_PrivateData.m_u32DataSize = sizeof(FaxPrivate_t);
				fax.m_u8WorkMode = XMS_FAX_WORKMODE_GATEWAY;
				fax.m_VoIPDevice = pOneVOIP->deviceID;	
				
				MixerControlParam_t paramMixer;
				memset(&paramMixer, 0, sizeof(MixerControlParam_t));
				
				ret = XMS_ctsSetParam(g_acsHandle, 
				                      &pOneVOIP->deviceID, 
					                  VOIP_PARAM_RTPMIXER,
				                      sizeof(paramMixer),
									  (MixerControlParam_t *)&paramMixer);	
				
				if(pOneVOIP->bSendFax == 1) // send fax
				{
					ret = XMS_ctsSendFax(g_acsHandle, 
						                 &pOneVOIP->pFaxDeviceID, 
										 &pOneVOIP->pVoiceDeviceID, 
										 "c:\\temp\\aa.tif","1234", 
										 (PrivateData_t *)&fax);//					
				}
				
				if (pOneVOIP->bRecvFax == 1) //recv fax
				{
					ret = XMS_ctsReceiveFax(g_acsHandle, 
						                    &pOneVOIP->pFaxDeviceID, 
											&pOneVOIP->pVoiceDeviceID, 
											"c:\\temp\\aa.tif","1234", 
											(PrivateData_t *)&fax);
				}

				if (g_logfp != NULL)
				{
					char	mLog[512]={0};
					char	mCurrTime[50]={0};
					getCurrTime(mCurrTime);
					
					sprintf(mLog,"%s voip-fax-voc :VoipDsp:%d VoipCh:%d GlobalID:%d faxDsp:%d faxCh:%d VocDsp:%d VocCh:%d, sendfax:%d, recvfax:%d, ret=%d, faxCallID=(%d,%d)\n\r",
						mCurrTime,
						pOneVOIP->deviceID.m_s8ModuleID,
						pOneVOIP->deviceID.m_s16ChannelID,
						pOneVOIP->iGlobeID,
						pOneVOIP->pFaxDeviceID.m_s8ModuleID,
						pOneVOIP->pFaxDeviceID.m_s16ChannelID,
						pOneVOIP->pVoiceDeviceID.m_s8ModuleID,
						pOneVOIP->pVoiceDeviceID.m_s16ChannelID,
						pOneVOIP->bSendFax,
						pOneVOIP->bRecvFax,						
						ret,
						pOneVOIP->pFaxDeviceID.m_CallID.m_s32FlowType,
						pOneVOIP->pFaxDeviceID.m_CallID.m_s32FlowChannel);

					fwrite(mLog,strlen(mLog),1,g_logfp);
					fflush(g_logfp);
				}				
			}
		}
	}
	
	return ret;
}

void FaxEndProcess(VOIP_STRUCT *pOneVOIP, Acs_Evt_t *pAcsEvt)
{
	if (pAcsEvt->m_s32EventType == XMS_EVT_SENDFAX || 
		pAcsEvt->m_s32EventType == XMS_EVT_RECVFAX)
	{
		if (pAcsEvt->m_s32EventType == XMS_EVT_SENDFAX)
		{
			pOneVOIP->bSendFax = 0;
		}
		else
		{
			pOneVOIP->bRecvFax = 0;
		}
		pOneVOIP->bStartFax = false;

		XMS_ctsClearCall(g_acsHandle, &pOneVOIP->deviceID, 0, NULL);

		if (g_logfp != NULL)
		{
			char	mLog[512]={0};
			char	mCurrTime[50]={0};
			getCurrTime(mCurrTime);
			
			sprintf(mLog,"%s fax end and clear VOIP call :Dsp:%d Ch:%d GlobalID:%d CallerNum:%s CalledNum:%d\n\r",
				mCurrTime,
				pOneVOIP->deviceID.m_s8ModuleID,
				pOneVOIP->deviceID.m_s16ChannelID,
				pOneVOIP->iGlobeID,
				pOneVOIP->CallerNum,
				pOneVOIP->CalleeNum);
			
			fwrite(mLog,strlen(mLog),1,g_logfp);
			fflush(g_logfp);
		}
	}
}

void RecordEndProcess(VOIP_STRUCT *pOneVOIP)
{
	VOICE_STRUCT *pOneVoice = NULL;
	RetCode_t		ret = -1;
	ControlRecord_t	controlRecord;

	ASSERT(pOneVOIP != NULL);	
	pOneVoice = &AllDeviceRes[pOneVOIP->pVoiceDevRecord.m_s8ModuleID].pVoice[pOneVOIP->pVoiceDevRecord.m_s16ChannelID];
	ASSERT(pOneVoice != NULL);	
	
	if(pOneVoice->State == VOC_USED)
	{
		//停止录音
		pOneVoice->State = VOC_FREE;
		
		controlRecord.m_u32ControlType = XMS_STOP_RECORD;
		ret = XMS_ctsControlRecord (g_acsHandle, &pOneVoice->deviceID, &controlRecord, NULL );
		//记录停止录音情况

		ret = XMS_ctsUnlinkDevice(g_acsHandle, &pOneVOIP->pVoiceDeviceID, &pOneVOIP->pVoiceDevRecord, NULL);
		//记录断开录音资源CT_BUS连接情况
	
		memset(&pOneVOIP->pVoiceDevRecord, 0, sizeof(DeviceID_t));
	
		((CButton *)pdlg->GetDlgItem(IDC_BUTTONRecord))->EnableWindow(true);
	}
}


