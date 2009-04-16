#ifndef _XMS_RUN_H_
#define _XMS_RUN_H_

void	RefreshMapTable(void);
void	DispEventInfo(Acs_Evt_t *pAcsEvt);
void	AddDeviceRes(Acs_Dev_List_Head_t *pAcsDevList);
void	HandleDevState(Acs_Evt_t *pAcsEvt);
DJ_Void EvtHandler(DJ_U32 esrParam);
void    StartT38Fax(DeviceID_t *pDev);

#endif