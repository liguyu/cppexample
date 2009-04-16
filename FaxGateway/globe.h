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
	bool        bcallFailFalg; //标记是否是7号呼叫失败

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

	long		iGlobeID;			//全局通道号


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
	bool		bOpenFlag;		// OpenDevice成功的标志

	//
	DJ_U8		u8E1Type;
	DJ_S32		s32AlarmVal;
} PCM_STRUCT;

// --------------------------------------------------------------------------------
// 定义结构：单个DSP可以提供的设备资源
typedef	struct
{
	long	lFlag;				// 本DSP是否存在, 0：不存在，1：存在

	DeviceID_t	deviceID;		// 本DSP模块的deviceID
	int			iSeqID;			// 本DSP模块的顺序号
	bool		bOpenFlag;		// OpenDevice成功的标志
	bool		bErrFlag;		// 发生过CloseDevice事件的标志

	long	lPcmNum;			// 该DSP上的ACS_DEVMAIN_DIGITAL_PORT数量
	long	lPcmOpened;			// 该DSP上OpenDevice成功的Pcm数量
	PCM_STRUCT		*pPcm;		// 根据需要，分配相应的结构

	long	lInterfNum;			// 该DSP上的ACS_DEVMAIN_INTERFACE_CH数量
	long	lInterfOpened;		// 该DSP上OpenDevice成功的Trunk数量
	INTERFACE_STRUCT	*pInterf;			// 根据需要，分配相应的结构

	long	lVocNum;			// 该DSP上的ACS_DEVMAIN_VOICE数量
	long	lVocOpened;			// 该DSP上OpenDevice成功的VOICE数量
	long	lVocFreeNum;		// 该DSP上可用的VOICE数量	
	VOICE_STRUCT	*pVoice;	// 根据需要，分配相应的结构

	long	lVOIPNum;			// 该DSP上的ACS_DEVMAIN_INTERFACE_CH数量
	long	lVOIPOpened;		// 该DSP上OpenDevice成功的Trunk数量
	VOIP_STRUCT	*pVOIP;			// 根据需要，分配相应的结构

	long    lFaxNum;
	long    lFaxOpened;
	long	lFaxFreeNum;
	FAX_STRUCT *pFax;   

} TYPE_XMS_DSP_DEVICE_RES;

// 定义结构：从通道号，可以方便地查找到模块和通道
//           不要更改此结构，也不要将状态或者变量放入此结构中
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

//建立拨号表
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
//建立拨号表 end


int CreateDailRule(PtrRule *ppDailRule);
void DeledtTab(PtrRule *pDailRule);	

void EnableGtg(DeviceID_t *pVocDev);
void DisableGtg(DeviceID_t *pVocDev);



#endif