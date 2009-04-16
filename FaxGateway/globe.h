#ifndef _GLOBE_H_
#define _GLOBE_H_

#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"
#include "DJAcsDevState.h"

#define		MAX_DSP_MODULE_NUM	256

#define		MAX_PCM_NUM			(4*MAX_DSP_MODULE_NUM)
#define     MAX_INTERFACE_NUM   (32*MAX_PCM_NUM)
#define		MAX_VOIP_NUM		(30*MAX_DSP_MODULE_NUM)
#define     MAX_VOICE_NUM       (32*MAX_PCM_NUM)

#define     MAX_SEND_IODATA_DTMF_LEN 32

#define		M_OnePcm(DevID)			AllDeviceRes[(DevID).m_s8ModuleID].pPcm[(DevID).m_s16ChannelID]
#define		M_OneInterface(DevID)	AllDeviceRes[(DevID).m_s8ModuleID].pInterf[(DevID).m_s16ChannelID]
#define		M_OneVoice(DevID)		AllDeviceRes[(DevID).m_s8ModuleID].pVoice[(DevID).m_s16ChannelID]
#define		M_OneVOIP(DevID)		AllDeviceRes[(DevID).m_s8ModuleID].pVOIP[(DevID).m_s16ChannelID]
#define		M_OneFax(DevID)		    AllDeviceRes[(DevID).m_s8ModuleID].pFax[(DevID).m_s16ChannelID]
enum ITF_STATE
{
	ITF_FREE = 0,
	ITF_CALLOUT,
	ITF_WAIT_ANSWERCALL,
	ITF_HANGUP
};

enum DTRK_STATE 
{
	TRK_WAITOPEN = 0,
	TRK_FREE,
	TRK_ALERT,
	TRK_CALLOUT,
	TRK_CALLIN,
	TRK_NOTHANDLE,
	TRK_WAIT_ANSERCALL,
	TRK_CONNECT,
	TRK_HANGUP,
	TRK_WAIT_CALL_VOIP,
};

enum EXCHAGE_TYPE
{
	EXCHANGE_FREE,
	SS7_VOIP,
	VOIP_SS7,
};

enum FAXMODE
{
	BYPASS = 0,
	T38
};

typedef struct
{
	// ----------------
	bool        bUsed;	
	bool        bcallFailFalg; //����Ƿ���7�ź���ʧ��

	DeviceID_t	deviceID;
	int			iSeqID;
	int			iModSeqID;
	int			iLineState;

	DeviceID_t	LinkDevID;
	DeviceID_t  VocDevID;

	DeviceID_t	pVoiceDeviceID;
	DeviceID_t	pVoipDeviceID;	

	char CallerCode[64];
	char CalleeCode[64];
	
	// -----------------
	DTRK_STATE	DTrkState;
	
	char DtmfBuf[32];
	int  DtmfCount;

	int	 TimeOut;	

	EXCHAGE_TYPE	mExchangeType;

} INTERFACE_STRUCT;

enum VOIP_STATE {
	VOIP_WAITOPEN,
	VOIP_FREE,
	VOIP_HANGUP,
	VOIP_ALERT,
	VOIP_CALLOUT,
	VOIP_CONNECT,
	VOIP_CALLIN,
	// new add for XMS
	VOIP_WAIT_ANSWERCALL,	
	VOIP_WAIT_CALL_TRUNK,
	VOIP_EXCHANGE,
};

typedef struct
{
	// ----------------
	int        bSendFax;
	int        bRecvFax;
	bool       bStartFax;	
	
	DeviceID_t	deviceID;
	int			iSeqID;
	int			iLineState;

	// -----------------
	VOIP_STATE	State;

	DeviceID_t  VocDevID;
	DeviceID_t  LinkDevID;

	DeviceID_t  pVoiceDevRecord;
	DeviceID_t	pVoiceDeviceID;
	DeviceID_t	pTrunkDeviceID;
	DeviceID_t  pFaxDeviceID;

	long		iGlobeID;			//ȫ��ͨ����


	char CallerNum[64];
	char CalleeNum[64];	

	int  iProtocol;

	char CallerAddr[20];
	char CallerUserID[20];	

	char CalleeAddr[20];
	char CalleeUserID[20];
	
	char DtmfBuf[32];
	int  DtmfCount;
	
	unsigned short iCallerPort;
	unsigned short iCalleePort;	

	int	 TimeOutCount;

	EXCHAGE_TYPE	mExchangeType;

} VOIP_STRUCT;

enum VOICE_STATE 
{
	VOC_WAITOPEN,
	VOC_FREE,
	VOC_USED
} ;

typedef struct
{	
	DeviceID_t	deviceID;
	int			iSeqID;	

//	DeviceID_t	UsedDevID; 

	DeviceID_t	pTrunkDevice;
	DeviceID_t	pVoipDevice;

	VOICE_STATE	State;

} VOICE_STRUCT;

enum FAX_STATE
{
	FAX_WAITOPEN,
	FAX_FREE,
	FAX_USED
};

typedef struct 
{	
	DeviceID_t  deviceID;
	int         iSeqID;

	DeviceID_t  pVoipDevice;

	FAX_STATE   State;
}FAX_STRUCT;

typedef struct
{
	// ----------------
	DeviceID_t	deviceID;
	int			iSeqID;
	bool		bOpenFlag;		// OpenDevice�ɹ��ı�־

	//
	DJ_U8		u8E1Type;
	DJ_S32		s32AlarmVal;
} PCM_STRUCT;

// --------------------------------------------------------------------------------
// ����ṹ������DSP�����ṩ���豸��Դ
typedef	struct
{
	long	lFlag;				// ��DSP�Ƿ����, 0�������ڣ�1������

	DeviceID_t	deviceID;		// ��DSPģ���deviceID
	int			iSeqID;			// ��DSPģ���˳���
	bool		bOpenFlag;		// OpenDevice�ɹ��ı�־
	bool		bErrFlag;		// ������CloseDevice�¼��ı�־

	long	lPcmNum;			// ��DSP�ϵ�ACS_DEVMAIN_DIGITAL_PORT����
	long	lPcmOpened;			// ��DSP��OpenDevice�ɹ���Pcm����
	PCM_STRUCT		*pPcm;		// ������Ҫ��������Ӧ�Ľṹ

	long	lInterfNum;			// ��DSP�ϵ�ACS_DEVMAIN_INTERFACE_CH����
	long	lInterfOpened;		// ��DSP��OpenDevice�ɹ���Trunk����
	INTERFACE_STRUCT	*pInterf;			// ������Ҫ��������Ӧ�Ľṹ

	long	lVocNum;			// ��DSP�ϵ�ACS_DEVMAIN_VOICE����
	long	lVocOpened;			// ��DSP��OpenDevice�ɹ���VOICE����
	long	lVocFreeNum;		// ��DSP�Ͽ��õ�VOICE����	
	VOICE_STRUCT	*pVoice;	// ������Ҫ��������Ӧ�Ľṹ

	long	lVOIPNum;			// ��DSP�ϵ�ACS_DEVMAIN_INTERFACE_CH����
	long	lVOIPOpened;		// ��DSP��OpenDevice�ɹ���Trunk����
	VOIP_STRUCT	*pVOIP;			// ������Ҫ��������Ӧ�Ľṹ

	long    lFaxNum;
	long    lFaxOpened;
	long	lFaxFreeNum;
	FAX_STRUCT *pFax;   

} TYPE_XMS_DSP_DEVICE_RES;

// ����ṹ����ͨ���ţ����Է���ز��ҵ�ģ���ͨ��
//           ��Ҫ���Ĵ˽ṹ��Ҳ��Ҫ��״̬���߱�������˽ṹ��
typedef struct
{
    ModuleID_t      m_s8ModuleID;    /*device module type*/
    ChannelID_t     m_s16ChannelID;  /*device chan id*/
} TYPE_CHANNEL_MAP_TABLE;

void	ReDrawAll(void);
BOOL	InitSystem (void);
void    ExitSystem(void);
void	InitAllDeviceRes(void);
void	FreeOneDeviceRes(int ID);
void	FreeAllDeviceRes (void);
void	OpenBoardDevice(DeviceID_t* pBoardDevID);
void	CloseBoardDevice(DeviceID_t* pBoardDevID);
void    OpenVoiceDevice(VOICE_STRUCT *pOneVoice);
void    CloseVoiceDevice(VOICE_STRUCT *pOneVoice);
void    OpenFaxDevice(FAX_STRUCT *pOneFax);
void    OpenPcmDevice(PCM_STRUCT *pOnePcm);
void    ClosePcmDevice(PCM_STRUCT *pOnePcm);
void	OpenAllDevice_Dsp(DJ_S8 s8DspModID);
void	CloseAllDevice_DspOK(DJ_S8 s8DspModID);
void	OpenDeviceOK(DeviceID_t* pDevice);
void	CloseDeviceOK(DeviceID_t* pDevice);
void	My_DualLink(DeviceID_t* pDev1, DeviceID_t* pDev2);
void	My_DualUnlink(DeviceID_t* pDev1, DeviceID_t* pDev2);

BOOL    IsDigitMod(DJ_S8 s8ModID);
BOOL    IsUserMod(DJ_S8 s8ModID);
BOOL    IsATrkMod(DJ_S8 s8ModID);
BOOL    IsVoipMod(DJ_S8 s8ModID);
void	DebugAllDevice();
int		SearchOneFreeTrunk(DeviceID_t * pTrunkDevice);
int		JudgeEmptyDsp(DJ_S8 mStartDspID);
void	getCurrTime(char* mTime);

int     setFax1100Gtd_Template(DeviceID_t *pDev);
int     setFax2100Gtd_Template(DeviceID_t *pDev);


typedef struct 
{
	char proxyAddr[20];
	long proxyPort;
}OutBoundAddr;

typedef struct
{
	char phonePreNum[10][10];
	OutBoundAddr outBAddr; 
}OutBoundRout;

void assignmentOutBoundRout(char *outBoundRoutStr, OutBoundRout *outRout);

void My_CloseLog();

void RecordFile(int iChType, int iDspID, int iChnID, int vocDspID, int vocChnID);

//�������ű�
struct Rule_Tab;
typedef struct Rule_Tab ListRule;
typedef struct Rule_Tab *PtrRule;
typedef PtrRule Position;

struct Rule_Tab
{
	char  CodeRule[256];
	PtrRule  Next;
};

int IsLast(PtrRule rule);
void Insert(char *chElement, PtrRule rule, Position p);
void DeleteList(PtrRule rule);
int IsEmpty(PtrRule rule);
//�������ű� end


int CreateDailRule(PtrRule *ppDailRule);
void DeledtTab(PtrRule *pDailRule);	

void EnableGtg(DeviceID_t *pVocDev);
void DisableGtg(DeviceID_t *pVocDev);



#endif