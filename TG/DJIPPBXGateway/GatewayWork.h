// GatewayWork.h: interface for the CGatewayWork class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GATEWAYWORK_H__52660541_9373_4818_BF46_D8F4B38E8F7F__INCLUDED_)
#define AFX_GATEWAYWORK_H__52660541_9373_4818_BF46_D8F4B38E8F7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum VOIPCALLOUTTYPE
{
	VCT_DEFAULT,
		VCT_CIRCULATION
};

enum VOIPDTMFTYPE
{
	VDT_H245UII,
		VDT_SIPINFO,
		VDT_RTPDTMF
};

enum ITF_STATE
{
	ITF_FREE = 0,
	ITF_CALLOUT,
	ITF_WAIT_ANSWERCALL,
	ITF_HANGUP
};

// 数字线路状态
enum DTRK_STATE 
{
	TRK_WAITOPEN = 0,
	TRK_FREE,	
	TRK_CALLOUT,
	TRK_CALLIN,
	TRK_NOTHANDLE,
	TRK_WAIT_ANSERCALL,
	TRK_CONNECT,
	TRK_HANGUP
};

//enum USER_STATE
//{
//	USER_WAITOPEN,
//	USER_FREE,
//	USER_OFFHOOK,
//	USER_CALLIN,
//	USER_CALLOUT,
//	USER_WAITHANGUP,
//	USER_CONNECT
////	USER_HANGUP
//};
//
//enum ATRK_STATE
//{
//	ATRK_WAITOPEN,
//	ATRK_FREE,
//	ATRK_OFFHOOK,
//	ATRK_CALLIN,
//	ATRK_CALLOUT,
//	ATRK_HANGUP
//};

const int MAX_STATE_SUM = 10;

const int CALL_NUMBER_MAX_LENGTH = 20;

typedef struct tagINTERFACE_STRUCT		// 1 CH
{
	// ----------------
	bool        bUsed;			// 
	DeviceID_t	deviceID;

	int			iSeqID;			// 编号
	int			iModSeqID;		// 可用的接口通道 去掉0 and 16 (0-31)

	int			iLineState;		// 设备状态

	DeviceID_t	LinkDevID;		//
	
	//DeviceID_t  VocDevID;		// 模拟用户采用
	char CallerNum[CALL_NUMBER_MAX_LENGTH];
	char CalleeNum[CALL_NUMBER_MAX_LENGTH];

	// -----------------
	int iState;					// 用户状态
	
	char DtmfBuf[32];
	int  DtmfCount;

	tagINTERFACE_STRUCT()
	{
		memset(this, 0, sizeof(tagINTERFACE_STRUCT));
	}
} INTERFACE_STRUCT;

enum VOIP_STATE
{
	VOIP_WAITOPEN,
	VOIP_FREE,
	VOIP_HANGUP,
	VOIP_CALLOUT,
	VOIP_CONNECT,
	// new add for XMS
	VOIP_WAIT_ANSWERCALL,
};

extern CString VOIPSTATE_STRING[6];

enum CALLDRE
{
	CALLDRE_NOTCALL,
	CALLDRE_CALLOUT,
		CALLDRE_CALLIN
};

extern CString CALLDRE_STRING[3];

typedef struct tagVOIP_STRUCT		// 1 CH
{
	// ----------------
	DeviceID_t	deviceID;
	int			iSeqID;
	int			iLineState;

	// -----------------
	VOIP_STATE	iState;
	int			iCallDre;

	DeviceID_t  VocDevID;
	DeviceID_t  LinkDevID;

	char CallerNum[CALL_NUMBER_MAX_LENGTH];
	char CalleeNum[CALL_NUMBER_MAX_LENGTH];

	char CallerNumNow[CALL_NUMBER_MAX_LENGTH];	// 经过号码转换后主叫
	char CalleeNumNow[CALL_NUMBER_MAX_LENGTH];	// 经过号码转换后被叫

	int  iProtocol;			// VOIP资源呼叫IPPBXVOIP座席时 IPPBX使用的协议 通过事件返回的VoIPCallPrivate_t获取

	char CallerAddr[20];
	char CallerUserID[20];

	char CalleeAddr[20];
	char CalleeUserID[20];
	
	char DtmfBuf[32];
	int  DtmfCount;

	unsigned short iCallerPort;
	unsigned short iCalleePort;

	tagVOIP_STRUCT()
	{
		memset(this, 0, sizeof(tagVOIP_STRUCT));
	}
} VOIP_STRUCT;

enum VOICE_STATE 
{
	VOC_WAITOPEN,
	VOC_FREE,
	VOC_USED
} ;

typedef struct tagVOICE_STRUCT		// 1 VOC CH
{	
	DeviceID_t	deviceID;
	//int			iSeqID;

	DeviceID_t	UsedDevID;

	VOICE_STATE	iState; // 将每个用VOIP的DSP上的前30各VOC和VOIP绑定 只需要表示没有打开以及打开状态

	tagVOICE_STRUCT()
	{
		memset(this, 0, sizeof(tagVOICE_STRUCT));
	}
} VOICE_STRUCT;

typedef struct tagPCM_STRUCT		// 1 E1
{
	// ----------------
	DeviceID_t	deviceID;
	int			iSeqID;
	bool		bOpenFlag;		// OpenDevice成功的标志

	//
	DJ_U8		u8E1Type;
	DJ_S32		s32AlarmVal;

	tagPCM_STRUCT()
	{
		memset(this, 0, sizeof(tagPCM_STRUCT));
	}
} PCM_STRUCT;

// 定义结构：单个DSP可以提供的设备资源
typedef	struct tagTYPE_XMS_DSP_DEVICE_RES
{
	short				lFlag;				// 本DSP是否存在, 0：不存在，1：存在
	short				lUsed;				// 本DSP是否可用, 0：不可用，1：可用

	DeviceID_t			deviceID;			// 本DSP模块的deviceID
	//int 				iSeqID;				// 本DSP模块的顺序号
	bool 				bOpenFlag;			// OpenDevice成功的标志
	bool 				bErrFlag;			// 发生过CloseDevice事件的标志

	long				lPcmNum;			// 该DSP上的ACS_DEVMAIN_DIGITAL_PORT数量
	long				lPcmOpened;			// 该DSP上OpenDevice成功的Pcm数量
	PCM_STRUCT			*pPcm;				// 根据需要，分配相应的结构

	long				lInterfNum;			// 该DSP上的ACS_DEVMAIN_INTERFACE_CH数量
	long				lInterfOpened;		// 该DSP上OpenDevice成功的Trunk数量
	INTERFACE_STRUCT	*pInterf;			// 根据需要，分配相应的结构

	long				lVocNum;			// 该DSP上的ACS_DEVMAIN_VOICE数量
	long				lVocOpened;			// 该DSP上OpenDevice成功的VOICE数量
	//long				lVocFreeNum;		// 该DSP上可用的VOICE数量	
	VOICE_STRUCT		*pVoice;			// 根据需要，分配相应的结构

	long				lVOIPNum;			// 该DSP上的ACS_DEVMAIN_VOIP_CH数量
	long				lVOIPOpened;		// 该DSP上OpenDevice成功的VOIP数量
	VOIP_STRUCT			*pVOIP;				// 根据需要，分配相应的结构

	tagTYPE_XMS_DSP_DEVICE_RES()			// 构造函数 初始状态所有DSP均为不存在
	{
		memset(this, 0, sizeof(tagTYPE_XMS_DSP_DEVICE_RES));
	}
} TYPE_XMS_DSP_DEVICE_RES;

typedef struct tagPBXADDRESS
{
	char m_chIPAddr[20];
	unsigned int m_uiPort;
	unsigned short m_ushProtocol;

	tagPBXADDRESS()
	{
		memset(this, 0, sizeof(tagPBXADDRESS));
	}
} PBXADDRESS, *LPPBXADDRESS;

typedef struct tagROUTINGCFG
{
	int iRoutingSum;
	CString *pStrRoutingString;

	tagROUTINGCFG()
	{
		memset(this, 0, sizeof(tagROUTINGCFG));
	}
} ROUTINGCFG, *LPROUTINGCFG;

typedef struct tagCHANGECALLERRULECFG
{
	int iRuleSum;
	CString *pStrRuleString;

	tagCHANGECALLERRULECFG()
	{
		memset(this, 0, sizeof(tagCHANGECALLERRULECFG));
	}
} CHANGECALLERRULECFG, *LPCHANGECALLERRULECFG;

typedef struct tagVOIPROUTINGCFG
{
	int iRoutingSum;
	CString *pStrRoutingString;
	PBXADDRESS *pAddress;

	tagVOIPROUTINGCFG()
	{
		memset(this, 0, sizeof(tagVOIPROUTINGCFG));
	}
} VOIPROUTINGCFG, *LPVOIPROUTINGCFG;

const int MAX_DSP_E1_SUM	= 4;							// One DSP E1 SUM
const int MAX_DSP_SUM		= 256;							// DSP MAX
const int MAX_PCM_NUM		= 4*MAX_DSP_SUM;				// PCM MAX
const int MAX_INTERFACE_NUM	= 32*MAX_PCM_NUM;				// ALLTYPECH MAX
const int MAX_VOICE_NUM		= 32*MAX_PCM_NUM;				// VOICE MAX
const int MAX_VOIP_SUM		= 30*MAX_DSP_SUM;				// VOIP MAX
const int USE_ROUTING_NORMAL= MAX_PCM_NUM+1;				// NORMAL ROUTING
const int MAX_SEND_IODATA_DTMF_LEN = 32;					// Send DTMF Max Length

extern ACSHandle_t 	g_acsHandle;							// API HANDLE
extern ServerID_t	g_cfgServerID;							// Config Struct ServerID Used By FLow
extern DJ_U8		g_cfgAppID;								// APP ID
extern DJ_U32		g_cfgRevQueSize;						// API Receive Queue Size
extern DJ_U32		g_cfgSndQueSize;						// API Send Queue Size
extern DJ_S32		g_cfgDebugOn;							// App Debug Flag

extern TYPE_XMS_DSP_DEVICE_RES	g_AllDeviceRes[MAX_DSP_SUM];// DSP 资源信息
extern int			g_cfgiVoipDspID[MAX_DSP_SUM];			// Voip Dsp ID
extern int			g_cfgiDigiDspID[MAX_DSP_SUM][MAX_DSP_E1_SUM];// Digital Dsp ID (Routing)
extern int			g_cfgAppIsLog;							// App Is Write LogFile
extern CString		g_strCfgIni;							// Configer File String
extern PBXADDRESS	g_cfgPBXADDRESS;						// IP PBX Config Info
extern ROUTINGCFG	g_cfgOutRoutingString;					// Out Routing
extern CHANGECALLERRULECFG g_cfgCCRuleString;				// Change Caller Rule VOIP->PSTN CallOut
extern CHANGECALLERRULECFG g_cfgCCRuleStringEx;				// Change Caller Rule PSTN->VOIP CallOut
extern VOIPROUTINGCFG	g_cfgVOIPRouting;					// VOIP Routing
extern int				g_cfgVOIPCallOutRuleType;			// VOIP CallOut RuleType
extern int				g_cfgVOIPDTMFType;					// VOIP DTMF TYPE
extern int				g_cfgVOIPVOCVALUE;					// VOIP VOC Value 混音音量
extern CString		DTRKSTATE_STRING[MAX_STATE_SUM];		// DTRUNK STATE STRING

extern CLogInfo		*g_pLogInfo;							// 日志信息类
extern CShowInfo	*g_pShowInfo;							// 显示信息类

void		ReadConfig(char *pIniFilePath = NULL);					// 读取配置信息
int			InitDJKeygoe();											// 初始化系统
DJ_Void		XMSEventProc(DJ_S32 esrParam);							// Keygoe系统事件回调函数
void		ExitDJKeygoe();											// 退出系统

void		ShowEventInfo(Acs_Evt_t *pAcsEvt);						// 显示事件信息
char*		GetEventTypeString(EventType_t EvtType);				// 获取事件类型字符串
char*		GetDeviceMainString(DJ_S32 s32DeviceMain);				// 获取设备主类型字符串
char*		GetErrorCodeString(DJ_S32 s32ErrorCode);				// 获取事件错误信息字符串

void		AddDeviceRes(Acs_Dev_List_Head_t *pAcsDevList);			// 将资源纳入程序
void		AddDeviceBoardRes(DJ_S8 s8DspModID, Acs_Dev_List_Head_t *pAcsDevList);
void		AddDevicePcmRes(DJ_S8 s8DspModID, Acs_Dev_List_Head_t *pAcsDevList);
void		AddDeviceInterfaceRes(DJ_S8 s8DspModID, Acs_Dev_List_Head_t *pAcsDevList);
void		AddDeviceVOIPRes(DJ_S8 s8DspModID, Acs_Dev_List_Head_t *pAcsDevList);
void		AddDeviceVoiceRes(DJ_S8 s8DspModID, Acs_Dev_List_Head_t *pAcsDevList);

BOOL		CfgIsUseDsp(DJ_S8 s8ModID);
BOOL		CfgIsUsePcm(DJ_S8 s8ModID, DJ_S8 sPcmID);
BOOL		CfgIsUseDigi(DJ_S8 s8ModID, int iDspE1Index);
BOOL		CfgIsUseVOIP(DJ_S8 s8ModID);
void		OpenOneDspAllDevice(DJ_S8 s8DspModID);					// 打开MODEID为s8DspModID的DSP上的用于本程序的Dev

void		OpenDeviceSuccess(DeviceID_t *pDevice);					// OpenDev成功事件处理函数
void		CloseDeviceSuccess(DeviceID_t* pDevice);				// CloseDev成功事件处理函数

void		AddDevLineState(Acs_Evt_t *pAcsEvt);					// 记录线路状态

void		ProcVOIPWork(VOIP_STRUCT *pOneVOIP, Acs_Evt_t *pAcsEvt);// VOIP Work Proc

void		FreeVOIPDev(VOIP_STRUCT* pOneVOIP);						// 清空VOIP结构中的缓存
void		FreeInterfDev(INTERFACE_STRUCT* pOneInterf);			// 清空INTERF结构中的缓存

char		GetVoipDtmfCode(Acs_Evt_t *pAcsEvt);					// 获取DTMF码
char		GetIntfDtmfCode(Acs_Evt_t *pAcsEvt);					// 获取DTMF码

void		ProcInterfaceWork(INTERFACE_STRUCT* pOneInterface, Acs_Evt_t* pAcsEvt);	// Interf Work Proc
void		ProcDTrunkWork(INTERFACE_STRUCT* pOneTrunk, Acs_Evt_t* pAcsEvt);		// DTrunk Work Proc

int			GetRoutingNumAndChangeCalleeNum(VOIP_STRUCT *pOneVOIP);	// 获取当前呼叫路由并且根据号码规则转换被叫号码
void		ChangeCallerNum(VOIP_STRUCT *pOneVOIP);					// 根据号码规则转换主叫号码
int			SearchOneFreeIntf(VOIP_STRUCT *pOneVOIP, DeviceID_t &pFreeIntfDeviceID);// 找到当前一个空闲的Intf资源
int			CallOutVOIPToPSTN(char* callerNum, char* calledNum, DeviceID_t* pInterfaceDev, VOIP_STRUCT* pOneVOIP);

int			GetVOIPRoutingAndChangeCalleeNum(VOIP_STRUCT *pOneVOIP);// 获取当前呼叫VOIP路由并且根据号码规则转换被叫号码
void		ChangeCallerNumEx(VOIP_STRUCT *pOneVOIP);				// 根据号码规则转换主叫号码
int			SearchOneFreeVOIP(DeviceID_t &pFreeVOIPDeviceID);		// 找到当前一个空闲的VOIP资源
int			CalloutPSTNToVOIP(char* pCallerNum, char* pCalledNum,  DeviceID_t* pVoipDev, INTERFACE_STRUCT* pOneInterface);

#endif // !defined(AFX_GATEWAYWORK_H__52660541_9373_4818_BF46_D8F4B38E8F7F__INCLUDED_)
