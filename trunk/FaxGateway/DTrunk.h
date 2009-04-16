#ifndef _DTRUNK_H_
#define _DTRUNK_H_
void    DrawInterface_LinkDev(INTERFACE_STRUCT* pOneInterface);
void    DrawInterface_CallInfo(INTERFACE_STRUCT* pOneInterface);
void    DrawInterf_LineState(INTERFACE_STRUCT* pOneInterface);
void	OpenInterfaceDevice (INTERFACE_STRUCT* pOneTrunk);
void	CloseInterfaceDevice (INTERFACE_STRUCT* pOneTrunk);
int     PSTNToVOIPCallout(char* pCallerNum, char* pCalledNum, DeviceID_t* pVoipDev, INTERFACE_STRUCT* pOneInterface);
void    InterfaceWork(INTERFACE_STRUCT* pOneInterface, Acs_Evt_t* pAcsEvt);

void    My_InitIntfDtmfBuf(INTERFACE_STRUCT* pOneInterf);
void    My_AddIntfDtmfBuf(INTERFACE_STRUCT* pOneInterf, char TmpDtmf);
char    My_GetIntfDtmfCode(Acs_Evt_t* pAcsEvt);


int	    CalDispRow(int iSeqID);
void    InitListDTrunk(void);
void    DrawDTrunk_LineState(INTERFACE_STRUCT* pOneTrunk);
void    DrawDTrunk_State(INTERFACE_STRUCT* pOneTrunk);
void    DrawDTrunk_CallInfo(INTERFACE_STRUCT* pOneTrunk);
void    DrawDTrunk_Reason(INTERFACE_STRUCT* pOneTrunk, DJ_S32 s32ErrCode, DJ_S32 s32ClearCause);
void    DrawDTrunk_LinkDev(INTERFACE_STRUCT* pOneTrunk);
void	InitDTrunkChannel(INTERFACE_STRUCT* pOneTrunk );
void    Change_DTrunkState(INTERFACE_STRUCT* pOneTrunk, DTRK_STATE NewState);
void    ResetTrunk(INTERFACE_STRUCT* pOneTrunk, Acs_Evt_t* pAcsEvt);
void    DTrunkWork(INTERFACE_STRUCT* pOneTrunk, Acs_Evt_t* pAcsEvt);

int checkrule(char *rule);
int find (char *str, char a);
int Rule(char *a, PtrRule pDialTab, char *convertStr);

void	WriteStatusInfo();

#endif