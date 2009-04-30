#include "stdafx.h"
#include "globe.h"
#include "DTrunk.h"
#include "VOIP.H"
#include "XMS_String.h"
#include "PBXGetWay.h"
#include "PBXGetWayDlg.h"

extern FILE * g_logfp;

extern CPBXGetWayDlg  *pdlg;

extern ACSHandle_t g_acsHandle;

extern TYPE_XMS_DSP_DEVICE_RES	AllDeviceRes[MAX_DSP_MODULE_NUM];

extern int						g_iTotalDTrunk;
extern int						g_iTotalDTrunkOpened;
extern TYPE_CHANNEL_MAP_TABLE	MapTable_DTrunk[MAX_INTERFACE_NUM];
extern	int pubSendRTPDTMFType;
extern long		pubCallInCount;
extern long		pubNormalClearCallCount;
extern long		pubErrorClearCallCount;
extern long		pubIpSendClearCallCount;
extern long		pubIpSS7ConnectCount;
extern long		pubSS7IpConnectCount;
extern long		pubIpSS7ErrorCount;
extern long		pubSS7IpErrorCount;

extern char		pubOutVoipAddress[255];
extern int			pubOutVoipPort;

extern PtrRule pDailRuleHead;
extern OutBoundRout *outRout;  //根据号码指向不同代理服务器
extern int cfg_routCnt;


void DrawInterface_LinkDev(INTERFACE_STRUCT* pOneInterface)
{
	switch(pOneInterface->deviceID.m_s16DeviceSub)
	{
	case XMS_DEVSUB_E1_CAS: 
	case XMS_DEVSUB_E1_DSS1:
	case XMS_DEVSUB_E1_SS7_TUP:
	case XMS_DEVSUB_E1_SS7_ISUP:
		DrawDTrunk_LinkDev(pOneInterface);
		break;
	default:
		break;
	}	
	return;
}

void DrawInterface_CallInfo(INTERFACE_STRUCT* pOneInterface)
{
	switch(pOneInterface->deviceID.m_s16DeviceSub)
	{
	case XMS_DEVSUB_E1_CAS: 
	case XMS_DEVSUB_E1_DSS1:
	case XMS_DEVSUB_E1_SS7_TUP:
	case XMS_DEVSUB_E1_SS7_ISUP:
		DrawDTrunk_CallInfo(pOneInterface);
		break;
	default:
		break;
	}
}

void DrawInterf_LineState(INTERFACE_STRUCT* pOneInterface)
{
	switch(pOneInterface->deviceID.m_s16DeviceSub)
	{
	case XMS_DEVSUB_E1_CAS: 
	case XMS_DEVSUB_E1_DSS1:
	case XMS_DEVSUB_E1_SS7_TUP:
	case XMS_DEVSUB_E1_SS7_ISUP:
		DrawDTrunk_LineState(pOneInterface);
		break;
	default:
		break;
	}	
}
void OpenInterfaceDevice(INTERFACE_STRUCT *pOneTrunk)
{
	RetCode_t   r;	

	_ASSERT(pOneTrunk != NULL);
	
	r = XMS_ctsOpenDevice(g_acsHandle, &pOneTrunk->deviceID, NULL);
	if (r<0)
	{
		//AddMsg
	}
	return;
}

void CloseInterfaceDevice(INTERFACE_STRUCT *pOneTrunk)
{
	RetCode_t	r;
	
	_ASSERT(pOneTrunk != NULL);
	
	pOneTrunk->DTrkState= TRK_WAITOPEN;
	
	r = XMS_ctsCloseDevice ( g_acsHandle, &pOneTrunk->deviceID, NULL );
	if ( r < 0 )
	{
		//	AddMsg ( "XMS_ctsCloseDevice Fail in CloseTrunkDevice()!" );
	}
	
	return;
}


void My_InitIntfDtmfBuf(INTERFACE_STRUCT* pOneInterf)
{
	pOneInterf->DtmfCount = 0;
	pOneInterf->DtmfBuf[0] = 0; 
	return;
}

void My_AddIntfDtmfBuf(INTERFACE_STRUCT* pOneInterf, char TmpDtmf)
{
	if ( pOneInterf->DtmfCount < (32-1) )
	{
		pOneInterf->DtmfBuf[pOneInterf->DtmfCount] = TmpDtmf; 
		pOneInterf->DtmfBuf[pOneInterf->DtmfCount+1] = 0; 
		pOneInterf->DtmfCount ++;
	}
	
	return;
}

char My_GetIntfDtmfCode(Acs_Evt_t* pAcsEvt)
{
	Acs_IO_Data				*pIOData = NULL;
	char		*p;	
	
	if ( pAcsEvt->m_s32EventType == XMS_EVT_RECVIODATA )	/*IO Data事件*/
	{
		pIOData = (Acs_IO_Data *)FetchEventData(pAcsEvt);
		
		if ( ( pIOData->m_u16IoType == XMS_IO_TYPE_DTMF ) 
			&& ( pIOData->m_u16IoDataLen > 0 ) )
		{
			p = (char *)FetchIOData(pAcsEvt);
			
			return *p;
		}
	}
	
	return -1;	// not a good DTMF
}
/*Digit Trunk*/
int	CalDispRow(int iSeqID)
{
	int	iRet;
	
	int cfg_iDispChnl = 32;
	
	if ( cfg_iDispChnl == 30 )
	{
		if ( (iSeqID % 16) == 0 )
			return -1;
		
		iRet = (iSeqID / 32) * 30 + (iSeqID % 32);
		iRet --;
		if ( (iSeqID % 32) > 16 )
			iRet --;
		return iRet;
		
	}
	
	if ( cfg_iDispChnl == 31 )
	{
		if ( (iSeqID % 32) == 0 )
			return -1;
		
		iRet = (iSeqID / 32) * 31 + (iSeqID % 32);
		iRet --;
		return iRet;
	}
	
	return iSeqID;
}

void InitListDTrunk(void)
{
	LV_COLUMN	lvc;
	DWORD dwExStyle;
	
	_ASSERT(pdlg != NULL);
	
	dwExStyle = LVS_EX_FULLROWSELECT | /*LVS_EX_GRIDLINES | LVS_EX_SUBITEMIMAGES |*/
		LVS_EX_HEADERDRAGDROP;// | LVS_EX_TRACKSELECT;
	
	pdlg->m_ListDTrunk.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LPARAM(dwExStyle));	
	lvc.mask =  LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	
	lvc.iSubItem = 0;
	lvc.pszText = "ID" ;
	lvc.cx = 30;
	pdlg->m_ListDTrunk.InsertColumn(0, &lvc); 
	
	lvc.iSubItem = 1;
	lvc.pszText = "Mod,Ch" ;
	lvc.cx = 50;
	pdlg->m_ListDTrunk.InsertColumn(1, &lvc); 
	
	lvc.iSubItem = 2;
	lvc.pszText = "Type" ;
	lvc.cx = 60;
	pdlg->m_ListDTrunk.InsertColumn(2, &lvc); 
	
	lvc.iSubItem = 3;
	lvc.pszText = "LineState" ;
	lvc.cx = 70;
	pdlg->m_ListDTrunk.InsertColumn(3, &lvc); 
	
	lvc.iSubItem = 4;
	lvc.pszText = "State" ;
	lvc.cx = 70;
	pdlg->m_ListDTrunk.InsertColumn(4, &lvc); 
	
	lvc.iSubItem = 5;
	lvc.pszText = "Called" ;
	lvc.cx = 60;
	pdlg->m_ListDTrunk.InsertColumn(5, &lvc); 
	
	lvc.iSubItem = 6;
	lvc.pszText = "Calling" ;
	lvc.cx = 60;
	pdlg->m_ListDTrunk.InsertColumn(6, &lvc); 
	
	lvc.iSubItem = 7;
	lvc.pszText = "LinkDev" ;
	lvc.cx = 60;
	pdlg->m_ListDTrunk.InsertColumn(7, &lvc); 
	
	lvc.iSubItem = 8;
	lvc.pszText = "FailReason" ;
	lvc.cx = 75;
	pdlg->m_ListDTrunk.InsertColumn(8, &lvc); 
	
	lvc.iSubItem = 9;
	lvc.pszText = "DTMF";
	lvc.cx = 75;
	pdlg->m_ListDTrunk.InsertColumn(9, &lvc);
	
	return;
}

void DrawDTrunk_DTMF(INTERFACE_STRUCT* pOneTrunk)
{
	char StateStr[100];
	int	 iDispRow;
	

	_ASSERT(pOneTrunk != NULL);
	_ASSERT(pdlg != NULL);
	
	
	iDispRow = CalDispRow(pOneTrunk->iSeqID); 
	if ( iDispRow < 0 )	
		return;	 
	
	sprintf(StateStr,"Time:%d Dtmf:%s",pOneTrunk->TimeOut,pOneTrunk->DtmfBuf);
	pdlg->m_ListDTrunk.SetItemText (iDispRow, 9, StateStr ); 
	
	return;
}

void DrawDTrunk_LineState(INTERFACE_STRUCT *pOneTrunk)
{
	char StateStr[100];
	int	 iDispRow;
	
	_ASSERT(pOneTrunk != NULL);
	_ASSERT(pdlg != NULL);		
	
	iDispRow = CalDispRow(pOneTrunk->iSeqID); 
	if ( iDispRow < 0 )	
		return;	 
	
	GetString_LineState ( StateStr, pOneTrunk->iLineState );
	pdlg->m_ListDTrunk.SetItemText ( iDispRow, 3, StateStr ); 	
	
	return;
}

void DrawDTrunk_State(INTERFACE_STRUCT* pOneTrunk)
{
	char StateStr[100];
	int	 iDispRow;
	
	_ASSERT(pOneTrunk != NULL);
	_ASSERT(pdlg != NULL);
	
	iDispRow = CalDispRow(pOneTrunk->iSeqID); 
	if ( iDispRow < 0 )	return;
	//
	switch( pOneTrunk->DTrkState ) 
	{
	case TRK_WAITOPEN:
		strcpy(StateStr,"Wait Open"); 
		break;
		
	case TRK_FREE:		
		strcpy(StateStr,"空闲"); 
		break;		
		
	case TRK_WAIT_ANSERCALL:
		strcpy(StateStr, "等待应答");
		break;
		
	case TRK_NOTHANDLE:
		strcpy(StateStr,"不处理");
		break ;
		
	case TRK_ALERT:
		strcpy(StateStr, "振铃");
		break;

	case TRK_CALLOUT:	
		strcpy(StateStr,"呼出");
		break;
		
	case TRK_HANGUP:
		strcpy(StateStr, "挂机");
		break;
		
	case TRK_CONNECT:
		strcpy(StateStr, "连通");
		break;
		
	default:
		strcpy(StateStr,"other");
		break;
	}
	
	pdlg->m_ListDTrunk.SetItemText ( iDispRow, 4, StateStr ); 
	
	return;
}

void DrawDTrunk_CallInfo(INTERFACE_STRUCT* pOneTrunk)
{
	char StateStr[100];
	int	 iDispRow;
	
#if DEBUG
	_ASSERT(pOneTrunk != NULL);
	_ASSERT(pdlg != NULL);
#endif
	
	iDispRow = CalDispRow(pOneTrunk->iSeqID); 
	if ( iDispRow < 0 )	
		return;
	
	strcpy ( StateStr, pOneTrunk->CalleeCode );
	pdlg->m_ListDTrunk.SetItemText ( iDispRow, 5, StateStr ); 
	
	strcpy ( StateStr, pOneTrunk->CallerCode );
	pdlg->m_ListDTrunk.SetItemText ( iDispRow, 6, StateStr ); 
	
	return;
}

void DrawDTrunk_Reason(INTERFACE_STRUCT* pOneTrunk, DJ_S32 s32ErrCode, DJ_S32 s32ClearCause)
{
	char StateStr[100];
	int	 iDispRow;
	
#if DEBUG
	_ASSERT(pOneTrunk != NULL);
	_ASSERT(pdlg != NULL);
#endif
	
	iDispRow = CalDispRow(pOneTrunk->iSeqID); 
	if ( iDispRow < 0 )	return;
	
	sprintf ( StateStr, "Err=%d,Cause=%d", s32ErrCode, s32ClearCause ); 
	pdlg->m_ListDTrunk.SetItemText ( iDispRow, 8, StateStr );
	
	return;
}

void DrawDTrunk_LinkDev(INTERFACE_STRUCT *pOneTrunk)
{
	char StateStr[100];
	int	 iDispRow;
	
#if DEBUG
	_ASSERT(pOneTrunk != NULL);
	_ASSERT(pdlg != NULL);
#endif
	
	iDispRow = CalDispRow(pOneTrunk->iSeqID); 
	if ( iDispRow < 0 )
		return;
	
	if ( pOneTrunk->LinkDevID.m_s16DeviceMain != 0 )
		sprintf ( StateStr, "(%d,%d)", pOneTrunk->LinkDevID.m_s8ModuleID, pOneTrunk->LinkDevID.m_s16ChannelID );
	else
		sprintf (StateStr, "");
	
	pdlg->m_ListDTrunk.SetItemText (iDispRow, 7, StateStr); 
	
	return;
}

void InitDTrunkChannel(INTERFACE_STRUCT *pOneTrunk)
{
	_ASSERT(pOneTrunk != NULL);

	Change_DTrunkState(pOneTrunk, TRK_FREE);
    My_InitIntfDtmfBuf(pOneTrunk);
	memset(&pOneTrunk->LinkDevID, 0, sizeof(DeviceID_t));// 0表示还没有分配LinkDevID
	memset(&pOneTrunk->VocDevID, 0, sizeof(DeviceID_t)); // 在中继中此语音为空
	DrawDTrunk_LinkDev(pOneTrunk);
	pOneTrunk->mExchangeType = EXCHANGE_FREE;
	pOneTrunk->bcallFailFalg = false;
	//	XMS_ctsResetDevice(g_acsHandle,&pOneTrunk->deviceID,NULL);
	return;
}

void Change_DTrunkState(INTERFACE_STRUCT *pOneTrunk, DTRK_STATE NewState)
{
	_ASSERT(pOneTrunk != NULL);
	
	pOneTrunk->DTrkState = NewState;
	DrawDTrunk_State(pOneTrunk);
	
	return;
}

void ResetTrunk(INTERFACE_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt)
{
	_ASSERT(pOneTrunk != NULL);
	
	InitDTrunkChannel(pOneTrunk);
	
	return;
}

void DTrunkWork(INTERFACE_STRUCT* pOneTrunk, Acs_Evt_t* pAcsEvt)
{
	DJ_S8                  pCallerNum[64] = {0};
	DJ_S8                  pCalledNum[64] = {0};
	DJ_S8                  pTempcalledNum[64] = {0};
	char                   TmpDtmf[1] = {0};
	Acs_CallControl_Data   *pCallControl = NULL;
	VOIP_STRUCT            *pOneVOIP = NULL;
	int                    i = -1;
	RetCode_t			   ret;
	
try
{	
	if (pAcsEvt->m_s32EventType == XMS_EVT_CLEARCALL)
	{	
		TRACE("Recv ss7 clearCall evt\n");
		
		long mDspID=pOneTrunk->pVoipDeviceID.m_s8ModuleID;
		long mChannelID=pOneTrunk->pVoipDeviceID.m_s16ChannelID;
		
		pOneVOIP=&M_OneVOIP(pOneTrunk->pVoipDeviceID);
		
		if(pOneTrunk->mExchangeType == SS7_VOIP)
		{
			pOneVOIP=&M_OneVOIP(pOneTrunk->pVoipDeviceID);
			if (pOneVOIP->iLineState != DCS_FREE)			
			{
				pubIpSendClearCallCount++;				
				
				XMS_ctsClearCall(g_acsHandle, &pOneVOIP->deviceID, 0, NULL);
				
				if(pOneVOIP->pVoiceDeviceID.m_s8ModuleID>0 )
					My_DualUnlink(&pOneTrunk->deviceID, &pOneVOIP->pVoiceDeviceID);		
				DrawVOIP_LinkDev (pOneVOIP);

				if (g_logfp != NULL)
				{
					char	mLog[512]={0};
					char	mCurrTime[50]={0};
					getCurrTime(mCurrTime);
					sprintf(mLog,"%s Direct SS7 call in: ss7 receive clearCall so clear voip.Dsp:%d Ch:%d GlobalID:%d CallerNum:%s CalledNum:%s\n\r",
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
		
		if(AllDeviceRes[mDspID].pVOIP[mChannelID].mExchangeType == VOIP_SS7 && 
			AllDeviceRes[mDspID].pVOIP[mChannelID].iLineState !=DCS_FREE &&
			AllDeviceRes[mDspID].pVOIP[mChannelID].iLineState !=DCS_CALLIN )
		{
			pOneVOIP=&M_OneVOIP(pOneTrunk->pVoipDeviceID);		
			
			ret = XMS_ctsClearCall(g_acsHandle, &pOneVOIP->deviceID, 0, NULL);	
			if (g_logfp != NULL)
			{
				char	mLog[512]={0};
				char	mCurrTime[50]={0};
				getCurrTime(mCurrTime);
				
				sprintf(mLog,"%s Direct VOIP call in: ss7 receive clearCall so clear voip.Dsp:%d Ch:%d GlobalID:%d CallerNum:%s CalledNum:%s\n\r",
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

		ResetTrunk(pOneTrunk,pAcsEvt);
		return;
	}

	//状态处理
	switch(pOneTrunk->DTrkState)
	{
		case TRK_FREE:	
			/*call in from Digit  Signal */
			if (pAcsEvt->m_s32EventType == XMS_EVT_CALLIN)
			{	
				
				pCallControl = (Acs_CallControl_Data *)((DJ_S8 *)pAcsEvt + sizeof(Acs_Evt_t));
				strncpy(pCallerNum, pCallControl->m_s8CallingNum, sizeof(pCallerNum));
				strncpy(pTempcalledNum, pCallControl->m_s8CalledNum, sizeof(pTempcalledNum));
		
				//hao ma gui ze
				memset(pCalledNum, 0, sizeof(pCalledNum));
				if (Rule(pTempcalledNum, pDailRuleHead, pCalledNum) == 0)		
					strcpy(pCalledNum, pTempcalledNum); 
				
				pOneTrunk->mExchangeType = SS7_VOIP;
				
				pOneVOIP=&M_OneVOIP(pOneTrunk->pVoipDeviceID);

				if (g_logfp != NULL)
				{
					char	mLog[512]={0};
					char	mCurrTime[50]={0};
					getCurrTime(mCurrTime);
					
					sprintf(mLog,"%s SS7 RECEVICE CallIn:Dsp:%d Ch:%d GlobalID:%d CallerNum:%s CalledNum:%s\n\r",
						mCurrTime,
						pOneVOIP->deviceID.m_s8ModuleID,
						pOneVOIP->deviceID.m_s16ChannelID,
						pOneVOIP->iGlobeID,
						pCallerNum,
						pCalledNum);
					fwrite(mLog,strlen(mLog),1,g_logfp);
					fflush(g_logfp);
				}
				
				if(pOneVOIP->iLineState==DCS_FREE && pOneVOIP->State == VOIP_FREE)
				{
					pOneVOIP=&M_OneVOIP(pOneTrunk->pVoipDeviceID);
					
					if(PSTNToVOIPCallout(pCallerNum, pCalledNum, &pOneVOIP->deviceID, pOneTrunk) < 0)
					{	
						XMS_ctsClearCall(g_acsHandle, &pOneTrunk->deviceID, 0, NULL);
						TRACE("3 ClearCall SS7\n");
						Change_DTrunkState(pOneTrunk, TRK_HANGUP);

						if (g_logfp != NULL)
						{
							char	mLog[512]={0};
							char	mCurrTime[50]={0};
							getCurrTime(mCurrTime);
							
							sprintf(mLog,"%s First Direct SS7 callin: voip function error,  so  clear ss7 :Dsp:%d Ch:%d GlobalID:%d CallerNum:%s CalledNum:%s\n\r",
								mCurrTime,
								pOneVOIP->deviceID.m_s8ModuleID,
								pOneVOIP->deviceID.m_s16ChannelID,
								pOneVOIP->iGlobeID,
								pOneVOIP->CallerNum,
								pOneVOIP->CalleeNum);
							
							fwrite(mLog,strlen(mLog),1,g_logfp);
							fflush(g_logfp);
						}
						break;
					}	
					DrawVOIP_LinkDev(pOneVOIP);	
					DrawVOIP_CallInfo(pOneVOIP);
					Change_VOIPState(pOneVOIP, VOIP_CALLOUT);

					strncpy(pOneVOIP->CalleeNum,pCalledNum,20);
					strncpy(pOneVOIP->CallerNum,pCallerNum,20);

					strncpy(pOneTrunk->CalleeCode, pCalledNum, 20);
					strncpy(pOneTrunk->CallerCode, pCallerNum, 20);
					DrawDTrunk_CallInfo(pOneTrunk);
					DrawDTrunk_LinkDev(pOneTrunk);			
					Change_DTrunkState(pOneTrunk, TRK_WAIT_ANSERCALL);	
				}
				else
				{
					XMS_ctsSetDevTimer(g_acsHandle,&pOneTrunk->deviceID,400);
					pOneTrunk->TimeOut = 0;
					DrawDTrunk_DTMF(pOneTrunk);
					Change_DTrunkState(pOneTrunk,TRK_WAIT_CALL_VOIP);
				}				
			}
		break;	

		case TRK_WAIT_CALL_VOIP:
		{
			if(pOneTrunk->iLineState == DCS_FREE )
			{
				if (pOneTrunk->mExchangeType == SS7_VOIP)
				{
					pOneVOIP=&M_OneVOIP(pOneTrunk->pVoipDeviceID);
					if (pOneVOIP->iLineState != DCS_FREE)			
					{
						XMS_ctsClearCall(g_acsHandle, &pOneVOIP->deviceID, 0, NULL);
						
						if(pOneVOIP->pVoiceDeviceID.m_s8ModuleID>0 )
							My_DualUnlink(&pOneTrunk->deviceID, &pOneVOIP->pVoiceDeviceID);		
						DrawVOIP_LinkDev (pOneVOIP);
						
						if (g_logfp != NULL)
						{
							char	mLog[512]={0};
							char	mCurrTime[50]={0};
							getCurrTime(mCurrTime);
							
							sprintf(mLog,"%s Direct SS7 call in: wait call IP,then SS7 Linestate is free so clear voip:Dsp:%d Ch:%d GlobalID:%d CallerNum:%s CalledNum:%s\n\r",
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
				ResetTrunk(pOneTrunk,pAcsEvt);
			}
			
			if(pAcsEvt->m_s32EventType == XMS_EVT_DEV_TIMER && pOneTrunk->iLineState != DCS_FREE)
			{
				pOneTrunk->TimeOut++;
				DrawDTrunk_DTMF(pOneTrunk);
				
				if(pOneTrunk->TimeOut >4 )
				{
					pOneTrunk->TimeOut = 0;
					pOneVOIP=&M_OneVOIP(pOneTrunk->pVoipDeviceID);
					XMS_ctsClearCall(g_acsHandle, &pOneTrunk->deviceID, 0, NULL);
					TRACE("4 ClearCall SS7\n");

					if (g_logfp != NULL)
					{
						char	mLog[512]={0};
						char	mCurrTime[50]={0};
						getCurrTime(mCurrTime);
						
						sprintf(mLog,"%s Direct SS7 call in: voip wait time is timeover,so clear ss7 :Dsp:%d Ch:%d GlobalID:%d CallerNum:%s CalledNum:%d\n\r",
							mCurrTime,
							pOneVOIP->deviceID.m_s8ModuleID,
							pOneVOIP->deviceID.m_s16ChannelID,
							pOneVOIP->iGlobeID,
							pOneVOIP->CallerNum,
							pOneVOIP->CalleeNum);

						fwrite(mLog,strlen(mLog),1,g_logfp);
						fflush(g_logfp);
					}
					
					break;
				}
				
				pOneVOIP=&M_OneVOIP(pOneTrunk->pVoipDeviceID);
				
				if(pOneVOIP->iLineState==DCS_FREE && pOneVOIP->State == VOIP_FREE)
				{
					if(PSTNToVOIPCallout(pCallerNum, pCalledNum, &pOneVOIP->deviceID, pOneTrunk) < 0)
					{	
						XMS_ctsClearCall(g_acsHandle, &pOneTrunk->deviceID, 0, NULL);
						TRACE("5 ClearCall SS7\n");;
						Change_DTrunkState(pOneTrunk, TRK_HANGUP);

						if (g_logfp != NULL)
						{
							char	mLog[512]={0};
							char	mCurrTime[50]={0};
							getCurrTime(mCurrTime);
							
							sprintf(mLog,"%s Second Direct SS7 call in: Voip call function error, so  clear ss7:Dsp:%d Ch:%d GlobalID:%d CallerNum:%s CalledNum:%s\n\r",
								mCurrTime,
								pOneVOIP->deviceID.m_s8ModuleID,
								pOneVOIP->deviceID.m_s16ChannelID,
								pOneVOIP->iGlobeID,
								pOneVOIP->CallerNum,
								pOneVOIP->CalleeNum);
							
							fwrite(mLog,strlen(mLog),1,g_logfp);
							fflush(g_logfp);
						}
					
						break;
					}				
					
					DrawVOIP_LinkDev(pOneVOIP);	
					DrawVOIP_CallInfo(pOneVOIP);
					Change_VOIPState(pOneVOIP, VOIP_CALLOUT);
					
					strncpy(pOneTrunk->CalleeCode, pCalledNum, 20);
					strncpy(pOneTrunk->CallerCode, pCallerNum, 20);
					DrawDTrunk_CallInfo(pOneTrunk);
					DrawDTrunk_LinkDev(pOneTrunk);			
					Change_DTrunkState(pOneTrunk, TRK_WAIT_ANSERCALL);	
				}
				else
				{
					XMS_ctsSetDevTimer(g_acsHandle,&pOneTrunk->deviceID,400);
					Change_DTrunkState(pOneTrunk,TRK_WAIT_CALL_VOIP);
				}
			}
		}
		break;
	
		case TRK_WAIT_ANSERCALL:		
			if(pOneTrunk->iLineState == DCS_FREE)
			{
				if (pOneTrunk->mExchangeType == SS7_VOIP)
				{
					pOneVOIP=&M_OneVOIP(pOneTrunk->pVoipDeviceID);
					if (pOneVOIP->iLineState != DCS_FREE)			
					{
						XMS_ctsClearCall(g_acsHandle, &pOneVOIP->deviceID, 0, NULL);
						
						if(pOneVOIP->pVoiceDeviceID.m_s8ModuleID>0 )
							My_DualUnlink(&pOneTrunk->deviceID, &pOneVOIP->pVoiceDeviceID);		
						DrawVOIP_LinkDev (pOneVOIP);
						
						if (g_logfp != NULL)
						{
							char	mLog[512]={0};
							char	mCurrTime[50]={0};
							getCurrTime(mCurrTime);
							
							sprintf(mLog,"%s SS7 wait  ansercall but SS7 Linestate is free so clear VOIP: Dsp:%d Ch:%d GlobalID:%d CallerNum:%s CalledNum:%s\n\r",
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
				ResetTrunk(pOneTrunk,pAcsEvt);
			}
			if(pAcsEvt->m_s32EventType == XMS_EVT_ANSWERCALL)
			{		

				Change_DTrunkState(pOneTrunk, TRK_CONNECT);
			}
			break;
	
		case TRK_CALLOUT:
			if ( pAcsEvt->m_s32EventType == XMS_EVT_CALLOUT )	
			{	
				INTERFACE_STRUCT *pOneInterface = NULL;			
				
				pOneVOIP = &M_OneVOIP(pOneTrunk->pVoipDeviceID);
				pCallControl = (Acs_CallControl_Data *)((DJ_S8 *)pAcsEvt + sizeof(Acs_Evt_t));
				if ( pCallControl->m_s32AcsEvtState == 1 )	// 呼出成功，对方摘机
				{	
				//	pOneTrunk->bcallFailFalg = false;

					if (g_logfp != NULL)
					{
						char	mLog[512]={0};
						char	mCurrTime[50]={0};
						getCurrTime(mCurrTime);
						
						sprintf(mLog,"%s SS7 CallOut Success:Dsp:%d Ch:%d GlobalID:%d CallerNum:%s CalledNum:%s\n\r",
							mCurrTime,
							pOneVOIP->deviceID.m_s8ModuleID,
							pOneVOIP->deviceID.m_s16ChannelID,
							pOneVOIP->iGlobeID,
							pOneVOIP->CallerNum,
							pOneVOIP->CalleeNum);
						
						fwrite(mLog,strlen(mLog),1,g_logfp);
						fflush(g_logfp);
					}				

					pubIpSS7ConnectCount++;					
					
					Change_DTrunkState(pOneTrunk, TRK_CONNECT);
					Change_VOIPState(pOneVOIP, VOIP_WAIT_ANSWERCALL);					
					XMS_ctsAnswerCallIn(g_acsHandle, &pOneVOIP->deviceID, NULL);		
				}
				else		// 呼出失败
				{				
					pOneTrunk->bcallFailFalg = true;
					DrawDTrunk_Reason(pOneTrunk, pCallControl->m_s32AcsEvtErrCode, pCallControl->m_s32CallClearCause);
					Change_DTrunkState(pOneTrunk, TRK_FREE);	
					pubIpSS7ErrorCount++;
					pubErrorClearCallCount++;				

					//带原因值的拆线--ADD by WWK
					int		mCallErrorCode = 0;
					if(pCallControl->m_s32AcsEvtErrCode == 17 ) mCallErrorCode = 486;		//被叫忙
					if(pCallControl->m_s32AcsEvtErrCode == 28 ) mCallErrorCode = 404;		//号码不存在
					if(pCallControl->m_s32AcsEvtErrCode == 19 ) mCallErrorCode = 408;		//被叫关机或者无应答

					XMS_ctsClearCall(g_acsHandle, &pOneVOIP->deviceID, mCallErrorCode, NULL);
					Change_VOIPState(pOneVOIP, VOIP_HANGUP);

					if (g_logfp != NULL)
					{
						char	mTemp[512]={0};
						char	mCurrTime[50]={0};
						getCurrTime(mCurrTime);
						
						sprintf(mTemp,"%s SS7 call out failed, so clear voip!! DSP:%d Ch:%d ErrorCode:%d Reason:%d, bcallFailFalg:%d\n\r",
							mCurrTime,
							pOneTrunk->deviceID.m_s8ModuleID,
							pOneTrunk->deviceID.m_s16ChannelID,
							pCallControl->m_s32AcsEvtErrCode,
							pCallControl->m_s32CallClearCause,
							pOneTrunk->bcallFailFalg);

						fwrite(mTemp,strlen(mTemp),1,g_logfp);
						fflush(g_logfp);
					}									
				}
			}
			break;

		case TRK_CONNECT:
			if(pOneTrunk->iLineState == DCS_FREE )
			{
				if (pOneTrunk->mExchangeType == SS7_VOIP)
				{
					pOneVOIP=&M_OneVOIP(pOneTrunk->pVoipDeviceID);
					if (pOneVOIP->iLineState != DCS_FREE)			
					{
						XMS_ctsClearCall(g_acsHandle, &pOneVOIP->deviceID, 0, NULL);
						
						if(pOneVOIP->pVoiceDeviceID.m_s8ModuleID>0 )
							My_DualUnlink(&pOneTrunk->deviceID, &pOneVOIP->pVoiceDeviceID);		
						DrawVOIP_LinkDev (pOneVOIP);

						if (g_logfp != NULL)
						{
							char	mLog[512]={0};
							char	mCurrTime[50]={0};
							getCurrTime(mCurrTime);
							
							sprintf(mLog,"%s SS7 Connected and Line State Is Free So Clear VOIP CALL :Dsp:%d Ch:%d GlobalID:%d CallerNum:%s CalledNum:%s\n\r",
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
				
				ResetTrunk(pOneTrunk, pAcsEvt);
				return;
			}
		
			TmpDtmf[0] = My_GetIntfDtmfCode(pAcsEvt);
			if ( TmpDtmf[0] != -1 )								//DTMF按键
			{				
				My_AddIntfDtmfBuf(pOneTrunk, TmpDtmf[0]);
				DrawDTrunk_DTMF(pOneTrunk);					
				
				if(pOneTrunk->DtmfCount > 0 )
				{					
					DeviceID_t	*pOneVOIP = &AllDeviceRes[pOneTrunk->deviceID.m_s8ModuleID].pInterf[pOneTrunk->deviceID.m_s16ChannelID].pVoipDeviceID;
					switch(pubSendRTPDTMFType)
					{
					case 1:
						{
							Acs_VoIP_H245UII h245Uii = {0};
							h245Uii.m_s8Signal = pOneTrunk->DtmfBuf[0]; // dtmf char in "0123456789#*ABCD!"
							h245Uii.m_u32Duration = 200; // milliseconds
							XMS_ctsSendIOData(g_acsHandle,pOneVOIP,XMS_IO_TYPE_VOIP_H245UII,sizeof(h245Uii),&h245Uii);
						}
						break;
					case 2:
						{
							Acs_VoIP_SIPINFO sipInfo = {0};
							strcpy(sipInfo.m_s8ContentType, "application/dtmf");
							sprintf(sipInfo.m_s8Body, "dtmf=%c\nduration=%d", pOneTrunk->DtmfBuf[0], 200);
							XMS_ctsSendIOData(g_acsHandle,pOneVOIP,XMS_IO_TYPE_VOIP_SIPINFO,sizeof(sipInfo),&sipInfo);
						}
						break;
					case 3:
						{
							Acs_VoIP_RTPDTMF rtpDtmf = {0};
							rtpDtmf.m_s8EventChar	= pOneTrunk->DtmfBuf[0];	// rtp dtmf event char
							rtpDtmf.m_s8Volume		= -32;				// volume (in dbm0 from -63 to 0)
							rtpDtmf.m_u16Duration	= 200;				// duration (in timestamp units
							XMS_ctsSendIOData(g_acsHandle,pOneVOIP,XMS_IO_TYPE_VOIP_RTPDTMF,sizeof(rtpDtmf),&rtpDtmf);
						}
						break;
					default:
						{
							Acs_VoIP_H245UII h245Uii = {0};
							h245Uii.m_s8Signal = pOneTrunk->DtmfBuf[0]; // dtmf char in "0123456789#*ABCD!"
							h245Uii.m_u32Duration = 200; // milliseconds
							XMS_ctsSendIOData(g_acsHandle,pOneVOIP,XMS_IO_TYPE_VOIP_H245UII,sizeof(h245Uii),&h245Uii);
						}
						break;
					}
					My_InitIntfDtmfBuf(pOneTrunk);					
				}
			}
		break;
		}
}catch(...)
{
	if (g_logfp != NULL)
	{
		char	mTemp[255]={0};
		char	mCurrTime[50]={0};
		getCurrTime(mCurrTime);
		
		sprintf(mTemp,"%s DT Trunk Run Error\n\r",mCurrTime);
		fwrite(mTemp,strlen(mTemp),1,g_logfp);
		fflush(g_logfp);
	}
}
	return;
}

int  PSTNToVOIPCallout(char* pCallerNum, char* pCalledNum,  DeviceID_t* pVoipDev, INTERFACE_STRUCT* pOneInterface)
{
	RetCode_t ret = -1;
	VOIP_STRUCT *pOneVOIP = NULL;
	DeviceID_t   FreeVocDeviceID;
	MixerControlParam_t     VocCmdP = {0};
	CmdParamData_Voice_t    VocMixParam = {0};
	VoIPCallPrivate_t       voipCallParam = {0};
	int iPreCnt = 0;	
	int iRoutCnt = 0;

	_ASSERT(pCallerNum != NULL);
	_ASSERT(pCalledNum != NULL);
	_ASSERT(pVoipDev != NULL);
	_ASSERT(pOneInterface != NULL);

	
	voipCallParam.m_PrivateData.m_u32DataSize = sizeof(VoIPCallPrivate_t);
	voipCallParam.m_s32Protocol =  XMS_VOIP_PROTOCOL_SIP;
	strncpy(voipCallParam.m_s8CallerUserID, pCallerNum, sizeof(voipCallParam.m_s8CallerUserID));

	char cfg_MapIP[255] = {0};
	if (outRout == NULL)
	{		
		sprintf(cfg_MapIP,"%s",pubOutVoipAddress);
		strncpy(voipCallParam.m_s8CalleeAddress, cfg_MapIP, sizeof(voipCallParam.m_s8CallerAddress));
		
		voipCallParam.m_s32CalleePort = pubOutVoipPort;
	}
	else
	{
		int bRout = 0;
		for (iRoutCnt = 1; iRoutCnt <= cfg_routCnt; iRoutCnt++)
		{
			if (bRout == 1)
				break;

			for (iPreCnt = 0; iPreCnt < 10; iPreCnt++)
			{
				if (strlen(outRout[iRoutCnt].phonePreNum[iPreCnt]) > 0)
				{			
					if (strncmp(outRout[iRoutCnt].phonePreNum[iPreCnt], pCalledNum, strlen(outRout[iRoutCnt].phonePreNum[iPreCnt])) == 0)
					{
						strncpy(voipCallParam.m_s8CalleeAddress, outRout[iRoutCnt].outBAddr.proxyAddr, sizeof(voipCallParam.m_s8CallerAddress));
						
						voipCallParam.m_s32CalleePort = outRout[iRoutCnt].outBAddr.proxyPort;
						
						bRout = 1;
						break;
					}
				}
			}
		}

		if (bRout == 0)
		{
			sprintf(cfg_MapIP,"%s",pubOutVoipAddress);
			strncpy(voipCallParam.m_s8CalleeAddress, cfg_MapIP, sizeof(voipCallParam.m_s8CallerAddress));
			
			voipCallParam.m_s32CalleePort = pubOutVoipPort;			
		}
	}

	if (g_logfp != NULL)
	{
		char	mTemp[512]={0};
		char	mCurrTime[50]={0};
		getCurrTime(mCurrTime);
		
		sprintf(mTemp,"%s IPCALLOUT In PSTNToVOIPCallOut: DSP:%d Ch:%d AcsHandle:%d, FlowType:%d FlowChannelID:%d CalleAddress:%s Port:%d Caller:%s Called:%s \n\r",
			mCurrTime,
			pVoipDev->m_s8ModuleID,
			pVoipDev->m_s16ChannelID,
			g_acsHandle,
			pVoipDev->m_CallID.m_s32FlowType,
			pVoipDev->m_CallID.m_s32FlowChannel,
			voipCallParam.m_s8CalleeAddress,
			voipCallParam.m_s32CalleePort,
			pCallerNum,
			pCalledNum);
		fwrite(mTemp,strlen(mTemp),1,g_logfp);
		fflush(g_logfp);
	}


	pOneVOIP = &M_OneVOIP(*pVoipDev);
		
	ret = XMS_ctsMakeCallOut(g_acsHandle, pVoipDev, pCallerNum, pCalledNum, (PrivateData_t *)&voipCallParam);
	if(ret > 0)
	{		
		
		FreeVocDeviceID=pOneVOIP->pVoiceDeviceID;			
		
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
		VocMixParam.m_VocInputControl.m_u8EcRefType=0;	*/
	
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
	
		My_DualLink(&pOneInterface->deviceID, &pOneInterface->pVoiceDeviceID);		
		
		XMS_ctsAlertCall(g_acsHandle,&pOneInterface->deviceID,NULL);			
		
		DrawVOIP_VocInfo(pOneVOIP);	
		
	}
/*	else
	{
		char	mTemp[255]={0};
		char	mCurrTime[50]={0};

		if (g_logfp != NULL)
		{
			getCurrTime(mCurrTime);
			
			sprintf(mTemp,"%s,PSTNToVOIP Faile!!,m_s16DeviceMain=%d,m_s16DeviceSub=%d,m_s8ModuleID=%d,m_s8MachineID=%d,m_s16ChannelID=%d,m_s32FlowType=%d,m_s32FlowChannel=%d\r",
				mCurrTime,
				pVoipDev->m_s16DeviceMain,pVoipDev->m_s16DeviceSub,pVoipDev->m_s8ModuleID,
				pVoipDev->m_s8MachineID,pVoipDev->m_s16ChannelID,
				pVoipDev->m_CallID.m_s32FlowType,pVoipDev->m_CallID.m_s32FlowChannel);
			fwrite(mTemp,strlen(mTemp),1,g_logfp);
		}

	//	return ret;
	
	}*/
	return ret;
}

void InterfaceWork(INTERFACE_STRUCT* pOneInterface, Acs_Evt_t* pAcsEvt)
{
	_ASSERT(pOneInterface != NULL);
	_ASSERT(pAcsEvt != NULL);
	DTrunkWork(pOneInterface, pAcsEvt);
}


//拨号规则代码
int find (char *str, char a)
{
	int i = -1;
	for(i = 0; str[i] != '\0'; i ++)
	{
		if (str[i] == a)
			return i;
	}
	return i = -1;
}

int Rule(char *oriCalledNum, PtrRule pDialTab, char *objCalledNum)
{	
	
	char *p = NULL;
	char plustemp[50] = {0};
	char minuttemp[50] = {0};
	char preAndMidd[100] = {0};
	char TempCalledNum[64] = {0};
	//char convertStr[256] = {0};

	int i = 0;

	if (pDialTab == NULL)
		return NULL;

	if (oriCalledNum == NULL || strcmp(oriCalledNum, "\0") == 0)
		return NULL;

	if (objCalledNum == NULL)
		return NULL;	
	
	sprintf(TempCalledNum, "0%s", oriCalledNum);
	while(!IsLast(pDialTab))
	{
		pDialTab = pDialTab->Next;
		TRACE("DialPlan=%s\n", pDialTab->CodeRule);
		
		if (checkrule(pDialTab->CodeRule) != 1)
			continue;
		
		for (i = 0; pDialTab->CodeRule[i] != '+';  i++)
		{
			if (pDialTab->CodeRule[i] == '+')
				break;
			plustemp[i] = pDialTab->CodeRule[i];
		}

		strncat(preAndMidd, plustemp, sizeof(preAndMidd));
		
		i++;
		
		for(int j = 0; pDialTab->CodeRule[i] != '|';  i++, j++)
		{
			if (pDialTab->CodeRule[j] == '|')
				break;
			
			minuttemp[j] = pDialTab->CodeRule[i];		
		}	

		if((sizeof(preAndMidd)-strlen(preAndMidd)-1) >= strlen(minuttemp))
			strncat(preAndMidd, minuttemp, sizeof(preAndMidd)-strlen(preAndMidd)-1);
		else
			continue;
		
		if (strlen(preAndMidd) > 0 && strncmp(preAndMidd, TempCalledNum, strlen(preAndMidd)) == 0)
		{
			strncat(objCalledNum, plustemp, strlen(plustemp));
			strncat(objCalledNum, TempCalledNum+strlen(preAndMidd), strlen(TempCalledNum) - strlen(preAndMidd));
			return strlen(objCalledNum);
		}
		
		memset(preAndMidd, 0, sizeof(preAndMidd));
	}			

	return  (0);
}

int checkrule(char *rule)
{
	int bValid = 0; //语法合法标志

	int orFlag = -1; //|在字符串中的位置
	int plusFlag = -1; //+在字符串中的位置
	int pointFlag = -1; //.在字符串中的位置

	if (rule == NULL)
		return 0;

	
	orFlag = find(rule, '|');
	plusFlag = find(rule, '+');
	pointFlag = find(rule, '.');


	if (orFlag >= 0 && plusFlag >= 0)
	{
		if (plusFlag < orFlag )
			bValid = 1;
	}

	if (pointFlag >= 0)
	{
		if (orFlag >= 0 )
		{
			if (pointFlag < orFlag )
				bValid = 0;			
		}

		if (plusFlag >= 0)
		{
			if (pointFlag < plusFlag)
				bValid = 0;
		}		
	}	

	return bValid;
}

//拨号规则 end

