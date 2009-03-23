#ifndef _XMS_SMONITOR_FUNC_H_
#define _XMS_SMONITOR_FUNC_H_

#define DT_Main(i)     (g_pDevRes[i].DevID.m_s16DeviceMain)
#define DT_Sub(i)      (g_pDevRes[i].DevID.m_s16DeviceSub)

enum SMON_DEVICE_STATE
{
	ST_NULL = 0,
	ST_OPEN,
	ST_RESET,
	ST_FREE,
};

typedef struct
{
	char IPAddress[32];
    int  Port;
	
	char Username[32];
	char Password[32];
	int  AppID;
    
} CFG_INFO, *PCFG_INFO;

typedef struct 
{
	DeviceID_t	DevID;
	bool		bOpenFlag;
	
	int         iState;
	int         iEvent;
	
	char        Caller[32];
	char        Called[32];
	
	int         iPos;

} TYPE_MY_DEVICE_STRUCT;

DJ_S32  InitSystem();
DJ_S32  ExitSystem();

DJ_Void Get_ConfigInformation();
DJ_Void Set_ConfigInformation();

DJ_Void EVT_Handler(DJ_U32 esrParam);

DJ_S32  IsHizDevice(DJ_S32 idx);

DJ_Void OpenDevice();
DJ_Void CloseDevice();

DJ_S32  AddDeviceRes(Acs_Dev_List_Head_t *pAcsDevList);
DJ_S32  InitDevice();

DJ_S32  Get_DeviceIndex(DeviceID_t *pDev);

DJ_S32  Ack_OpenDevice(Acs_Evt_t* pAcsEvt);
DJ_S32  Ack_SigMon(Acs_Evt_t* pAcsEvt);

DJ_Void Show_Information(DJ_S32 iPos, DJ_Void* pData);
DJ_Void Show_Data(DJ_Void* pData);

#endif
