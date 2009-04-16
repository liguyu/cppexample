#ifndef _VOIP_H_
#define _VOIP_H_

void	InitListVOIP(void);
void    DrawVOIP_LineState(VOIP_STRUCT* pOneVOIP);
void    DrawVOIP_State(VOIP_STRUCT* pOneVOIP);
void    DrawVOIP_CallInfo(VOIP_STRUCT* pOneVOIP);
void    DrawVOIP_DTMF(VOIP_STRUCT* pOneVOIP);
void    DrawVOIP_LinkDev(VOIP_STRUCT* pOneVOIP);
void    DrawVOIP_VocInfo(VOIP_STRUCT* pOneVOIP);
void    DrawVOIP_Reason(INTERFACE_STRUCT* pOneTrunk, DJ_S32 s32ErrCode, DJ_S32 s32ClearCause);
void    OpenVOIPDevice(VOIP_STRUCT* pOneVOIP);
void    CloseVOIPDevice(VOIP_STRUCT* pOneVOIP);
void    InitVOIPChannel(VOIP_STRUCT* pOneVOIP);
char    My_GetDtmfCode(Acs_Evt_t* pAcsEvt);
void    Change_VOIPState(VOIP_STRUCT* pOneVOIP, VOIP_STATE NewState);
void    ResetVOIP(VOIP_STRUCT* pOneVOIP, Acs_Evt_t* pAcsEvt);
int     IPToPSTNCallOut(char* calleeNum, char* calledNum, DeviceID_t* pInterfaceDev, VOIP_STRUCT* pOneVOIP);
void    VOIPWork(VOIP_STRUCT* pOneVOIP, Acs_Evt_t *pAcsEvt);
DJ_S32	PlayDTMF(DeviceID_t* pVocDevID, char *DtmfStr);
DJ_S32	StopPlayDTMF(DeviceID_t* pVocDevID);
void    My_InitDtmfBuf(VOIP_STRUCT* pOneOVIP);
void    My_AddDtmfBuf(VOIP_STRUCT* pOneVOIP, char TmpDtmf);
int     T38GatWayWork(VOIP_STRUCT *pOneVOIP, Acs_Evt_t *pAcsEvt);
void    FaxEndProcess(VOIP_STRUCT *pOneVOIP, Acs_Evt_t *pAcsEvt);
void    RecordEndProcess(VOIP_STRUCT *pOneVoice);
//char    My_GetDtmfCode(Acs_Evt_t* pAcsEvt);
 
#endif