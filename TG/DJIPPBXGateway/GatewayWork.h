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

// ������·״̬
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

	int			iSeqID;			// ���
	int			iModSeqID;		// ���õĽӿ�ͨ�� ȥ��0 and 16 (0-31)

	int			iLineState;		// �豸״̬

	DeviceID_t	LinkDevID;		//
	
	//DeviceID_t  VocDevID;		// ģ���û�����
	char CallerNum[CALL_NUMBER_MAX_LENGTH];
	char CalleeNum[CALL_NUMBER_MAX_LENGTH];

	// -----------------
	int iState;					// �û�״̬
	
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

	char CallerNumNow[CALL_NUMBER_MAX_LENGTH];	// ��������ת��������
	char CalleeNumNow[CALL_NUMBER_MAX_LENGTH];	// ��������ת���󱻽�

	int  iProtocol;			// VOIP��Դ����IPPBXVOIP��ϯʱ IPPBXʹ�õ�Э�� ͨ���¼����ص�VoIPCallPrivate_t��ȡ

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

	VOICE_STATE	iState; // ��ÿ����VOIP��DSP�ϵ�ǰ30��VOC��VOIP�� ֻ��Ҫ��ʾû�д��Լ���״̬

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
	bool		bOpenFlag;		// OpenDevice�ɹ��ı�־

	//
	DJ_U8		u8E1Type;
	DJ_S32		s32AlarmVal;

	tagPCM_STRUCT()
	{
		memset(this, 0, sizeof(tagPCM_STRUCT));
	}
} PCM_STRUCT;

// ����ṹ������DSP�����ṩ���豸��Դ
typedef	struct tagTYPE_XMS_DSP_DEVICE_RES
{
	short				lFlag;				// ��DSP�Ƿ����, 0�������ڣ�1������
	short				lUsed;				// ��DSP�Ƿ����, 0�������ã�1������

	DeviceID_t			deviceID;			// ��DSPģ���deviceID
	//int 				iSeqID;				// ��DSPģ���˳���
	bool 				bOpenFlag;			// OpenDevice�ɹ��ı�־
	bool 				bErrFlag;			// ������CloseDevice�¼��ı�־

	long				lPcmNum;			// ��DSP�ϵ�ACS_DEVMAIN_DIGITAL_PORT����
	long				lPcmOpened;			// ��DSP��OpenDevice�ɹ���Pcm����
	PCM_STRUCT			*pPcm;				// ������Ҫ��������Ӧ�Ľṹ

	long				lInterfNum;			// ��DSP�ϵ�ACS_DEVMAIN_INTERFACE_CH����
	long				lInterfOpened;		// ��DSP��OpenDevice�ɹ���Trunk����
	INTERFACE_STRUCT	*pInterf;			// ������Ҫ��������Ӧ�Ľṹ

	long				lVocNum;			// ��DSP�ϵ�ACS_DEVMAIN_VOICE����
	long				lVocOpened;			// ��DSP��OpenDevice�ɹ���VOICE����
	//long				lVocFreeNum;		// ��DSP�Ͽ��õ�VOICE����	
	VOICE_STRUCT		*pVoice;			// ������Ҫ��������Ӧ�Ľṹ

	long				lVOIPNum;			// ��DSP�ϵ�ACS_DEVMAIN_VOIP_CH����
	long				lVOIPOpened;		// ��DSP��OpenDevice�ɹ���VOIP����
	VOIP_STRUCT			*pVOIP;				// ������Ҫ��������Ӧ�Ľṹ

	tagTYPE_XMS_DSP_DEVICE_RES()			// ���캯�� ��ʼ״̬����DSP��Ϊ������
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

extern TYPE_XMS_DSP_DEVICE_RES	g_AllDeviceRes[MAX_DSP_SUM];// DSP ��Դ��Ϣ
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
extern int				g_cfgVOIPVOCVALUE;					// VOIP VOC Value ��������
extern CString		DTRKSTATE_STRING[MAX_STATE_SUM];		// DTRUNK STATE STRING

extern CLogInfo		*g_pLogInfo;							// ��־��Ϣ��
extern CShowInfo	*g_pShowInfo;							// ��ʾ��Ϣ��

void		ReadConfig(char *pIniFilePath = NULL);					// ��ȡ������Ϣ
int			InitDJKeygoe();											// ��ʼ��ϵͳ
DJ_Void		XMSEventProc(DJ_S32 esrParam);							// Keygoeϵͳ�¼��ص�����
void		ExitDJKeygoe();											// �˳�ϵͳ

void		ShowEventInfo(Acs_Evt_t *pAcsEvt);						// ��ʾ�¼���Ϣ
char*		GetEventTypeString(EventType_t EvtType);				// ��ȡ�¼������ַ���
char*		GetDeviceMainString(DJ_S32 s32DeviceMain);				// ��ȡ�豸�������ַ���
char*		GetErrorCodeString(DJ_S32 s32ErrorCode);				// ��ȡ�¼�������Ϣ�ַ���

void		AddDeviceRes(Acs_Dev_List_Head_t *pAcsDevList);			// ����Դ�������
void		AddDeviceBoardRes(DJ_S8 s8DspModID, Acs_Dev_List_Head_t *pAcsDevList);
void		AddDevicePcmRes(DJ_S8 s8DspModID, Acs_Dev_List_Head_t *pAcsDevList);
void		AddDeviceInterfaceRes(DJ_S8 s8DspModID, Acs_Dev_List_Head_t *pAcsDevList);
void		AddDeviceVOIPRes(DJ_S8 s8DspModID, Acs_Dev_List_Head_t *pAcsDevList);
void		AddDeviceVoiceRes(DJ_S8 s8DspModID, Acs_Dev_List_Head_t *pAcsDevList);

BOOL		CfgIsUseDsp(DJ_S8 s8ModID);
BOOL		CfgIsUsePcm(DJ_S8 s8ModID, DJ_S8 sPcmID);
BOOL		CfgIsUseDigi(DJ_S8 s8ModID, int iDspE1Index);
BOOL		CfgIsUseVOIP(DJ_S8 s8ModID);
void		OpenOneDspAllDevice(DJ_S8 s8DspModID);					// ��MODEIDΪs8DspModID��DSP�ϵ����ڱ������Dev

void		OpenDeviceSuccess(DeviceID_t *pDevice);					// OpenDev�ɹ��¼�������
void		CloseDeviceSuccess(DeviceID_t* pDevice);				// CloseDev�ɹ��¼�������

void		AddDevLineState(Acs_Evt_t *pAcsEvt);					// ��¼��·״̬

void		ProcVOIPWork(VOIP_STRUCT *pOneVOIP, Acs_Evt_t *pAcsEvt);// VOIP Work Proc

void		FreeVOIPDev(VOIP_STRUCT* pOneVOIP);						// ���VOIP�ṹ�еĻ���
void		FreeInterfDev(INTERFACE_STRUCT* pOneInterf);			// ���INTERF�ṹ�еĻ���

char		GetVoipDtmfCode(Acs_Evt_t *pAcsEvt);					// ��ȡDTMF��
char		GetIntfDtmfCode(Acs_Evt_t *pAcsEvt);					// ��ȡDTMF��

void		ProcInterfaceWork(INTERFACE_STRUCT* pOneInterface, Acs_Evt_t* pAcsEvt);	// Interf Work Proc
void		ProcDTrunkWork(INTERFACE_STRUCT* pOneTrunk, Acs_Evt_t* pAcsEvt);		// DTrunk Work Proc

int			GetRoutingNumAndChangeCalleeNum(VOIP_STRUCT *pOneVOIP);	// ��ȡ��ǰ����·�ɲ��Ҹ��ݺ������ת�����к���
void		ChangeCallerNum(VOIP_STRUCT *pOneVOIP);					// ���ݺ������ת�����к���
int			SearchOneFreeIntf(VOIP_STRUCT *pOneVOIP, DeviceID_t &pFreeIntfDeviceID);// �ҵ���ǰһ�����е�Intf��Դ
int			CallOutVOIPToPSTN(char* callerNum, char* calledNum, DeviceID_t* pInterfaceDev, VOIP_STRUCT* pOneVOIP);

int			GetVOIPRoutingAndChangeCalleeNum(VOIP_STRUCT *pOneVOIP);// ��ȡ��ǰ����VOIP·�ɲ��Ҹ��ݺ������ת�����к���
void		ChangeCallerNumEx(VOIP_STRUCT *pOneVOIP);				// ���ݺ������ת�����к���
int			SearchOneFreeVOIP(DeviceID_t &pFreeVOIPDeviceID);		// �ҵ���ǰһ�����е�VOIP��Դ
int			CalloutPSTNToVOIP(char* pCallerNum, char* pCalledNum,  DeviceID_t* pVoipDev, INTERFACE_STRUCT* pOneInterface);

#endif // !defined(AFX_GATEWAYWORK_H__52660541_9373_4818_BF46_D8F4B38E8F7F__INCLUDED_)
