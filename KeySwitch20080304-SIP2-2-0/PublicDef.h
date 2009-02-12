/*��ϵͳ���к����ͽӿڵĶ����Լ�����****************************************/

#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"
#include "StdAfx.h"
#include "KeySwitch.h"
#include "KeySwitchDlg.h"



/*ȫ�ֳ�������*************************************************************/
#define SYSTEM_INI_FILE		"\\KeygoeSwitch.ini"
#define APPLICATION_ID		12
#define DEBUG_TYPE			0
#define ID_TIME             1002				/*ɨ���ͨ��DTMF�붨ʱ������*/	
#define MAX_DSP_COUNT		3					/*���֧��256��DSP---��ʽ����ʱ����е���*/
#define MAX_VOICE_COUNT		MAX_DSP_COUNT*125	/*��������豸����*/
#define MAX_VOIP_COUNT		MAX_DSP_COUNT*30	/*���VOIP�豸����*/
#define MAX_INTERFACE_COUNT	MAX_DSP_COUNT*128	/*���ͨ����Ŀ������voice,trunk*/
#define	MAX_SEND_IODATA_DTMF_LEN	16


/*�����ļ�·����������*****************************************************/
#define	DIAL_LONG_ERROR_FILE		"E:\\Music\\noldt.pcm"
#define SYS_WELCOME_FILE			"E:\\Music\\welcome.pcm"
#define	SYS_NUMBER_0_VOICE			"E:\\Music\\0.pcm"
#define	SYS_NUMBER_1_VOICE			"E:\\Music\\1.pcm"
#define	SYS_NUMBER_2_VOICE			"E:\\Music\\2.pcm"
#define	SYS_NUMBER_3_VOICE			"E:\\Music\\3.pcm"
#define	SYS_NUMBER_4_VOICE			"E:\\Music\\4.pcm"
#define	SYS_NUMBER_5_VOICE			"E:\\Music\\5.pcm"
#define	SYS_NUMBER_6_VOICE			"E:\\Music\\6.pcm"
#define	SYS_NUMBER_7_VOICE			"E:\\Music\\7.pcm"
#define	SYS_NUMBER_8_VOICE			"E:\\Music\\8.pcm"
#define	SYS_NUMBER_9_VOICE			"E:\\Music\\9.pcm"
#define SYS_LOCALNUMBER_VOC         "E:\\Music\\localnumber.pcm"
#define	HAVE_LONG_NOTICE_VOC		"E:\\Music\\HaveCallLong.pcm"		/*���г�;����������ʾ*/
#define NOT_HAVE_LONG_NOTICE_VOC	"E:\\Music\\NotHaveCallLong.pcm"	/*�رճ�;����������ʾ*/
#define	SETTING_FINISH_VOC			"E:\\Music\\finishsetting.pcm"	/*�������*/
#define CALL_NUMBER_NOT_EXIT_VOC	"E:\\Music\\nonumber.pcm"			/*���еĺ��벻����*/
#define	IS_BUSY_NOW_VOC				"E:\\Music\\istalkrow.pcm"			/*�Է�����ͨ����*/
#define	MENU_VOC					"E:\\Music\\menu.pcm"			/*�Է�����ͨ����*/
#define	ENABLE_THE_CALL_VOC			"E:\\Music\\enablethephone.pcm"	/*�����绰*/
#define	DISABLE_THE_CALL_VOC		"E:\\Music\\disablethephone.pcm"	/*�رյ绰*/
#define NO_TRUNK_FOR_DIAL_VOC		"E:\\Music\\notrunkfordial.pcm"	/*�޿�������ͨ��*/
#define CALL_SELF_VOC				"E:\\Music\\callself.pcm"			/*���Լ���绰����ʾ��*/
#define	BACKGROUND_MUSIC			"E:\\Music\\music.pcm"			/*��������*/
#define BACK_SIGN_MUSIC				"E:\\Music\\SIGRING.pcm"

/*�����豸״̬����(VOICE)**************************************************/
enum	VOICE_STATUS
{
		VOC_WAIT_OPEN,							/*�ȴ���*/
		VOC_WAIT_WORK,							/*�ȴ����г�ʼ����*/
		VOC_FREE,								/*����*/
		VOC_OFFHOOK,							/*ժ��*/
		VOC_WAITHANGUP,							/*�ȴ��һ�*/
		VOC_WAIT_DIAL,							/*�ȴ�����*/	
		VOC_DTMF_INPUT,							/*�����պ�*/
		VOC_CALL_OUT,							/*������*/
		VOC_CALL_OUT_FAILE,						/*����ʧ��*/	
		VOC_CALL_OUT_SUCCESS,					/*���гɹ�*/
		VOC_CONNECTED,							/*ͨ����*/
		VOC_CONNECT_DTMF,						/*ͨ�����û�����*/
		VOC_ONHOOK,								/*�һ�*/
		VOC_FLASH,								/*�Ĳ��*/
		VOC_FLASH_WAIT_DIAL,					/*�Ĳ�ɵȴ�����*/
		VOC_FLASH_CONNECT,						/*�Ĳ������*/
		VOC_CALL_IN,							/*�е绰����*/
		VOC_RINGING,							/*������*/	
		VOC_RING_TIMEOUT,						/*���峬ʱ*/
		VOC_FETCH_DIAL,							/*���Ӳ���*/
		VOC_FETCH_CONNECT,						/*��������*/
		VOC_SYS_USE,							/*�ײ�ʹ��*/
		VOC_ERROR_CALL_LONG,					/*�Ƿ����г�;���Ŵ�����״̬*/
		VOC_PLAY_NOTICE,						/*����������ʾ*/
		VOC_PLAY_NUMBER,						/*���ű�������*/
		VOC_PLAY_WELCOME,						/*���Ż�ӭ������ʾ*/
		VOC_ANSWER_CALL,						/*ANSWER CALL*/
		VOC_PLAY_VOIP_ERROR,					/*VOIP���Ŵ�����*/
		VOC_HUIXIAN_TURN,						/*����ͨ����*/
		VOC_WAIT_MAIN,							/*���ӷ���ȴ�*/
		VOC_WAIT_FIRST_DIAL_TONE,				/*�ȴ���һ��������*/
		VOC_WAIT_SECOND_DIAL_TONE,				/*�ȴ��ڶ���������*/
		VOC_PLAY_DIAL_WRONG						/*������Ч����*/
};

/*VOIP�豸״̬����(VOIP)**************************************************/
enum	VOIP_STATUS
{
		VOIP_WAIT_OPEN,							/*�ȴ���*/
		VOIP_WAIT_WORK,							/*�ȴ����г�ʼ����*/
		VOIP_FREE,								/*����*/
		VOIP_CALL_IN,							/*�е绰����*/
		VOIP_WAIT_CONNECT,						/*����������*/	
		VOIP_CALL_OUT_FAILE,					/*���ߺ���ʧ��*/
		VOIP_CALL_OUT_SUCCESS,					/*���ߺ��гɹ�*/
		VOIP_CONNECTED,							/*ͨ����*/	
		VOIP_CONNECT_DTMF,						/*ͨ�����û�����*/
		VOIP_ONHOOK,							/*�һ�*/
		VOIP_CALL_VOIP,							/*�е绰����*/
		VOIP_CALL_VOIP_FAILE,					/*��������ʧ��*/
		VOIP_PLAY_FILE,							/*VOIP����*/
		VOIP_WAITHUNGUP,						/*�ȴ��һ�*/
		VOIP_CALL_VOIP_SUCCESS					/*�������߳ɹ�*/
};

/*�豸���������Ͷ���******************************************************/
enum	LINK_TYPE
{
		LINK_SLOT,								/*ʱ϶����*/
		LINK_BIND								/*������*/
};

/*�豸���Ͷ���************************************************************/
enum	DEVICE_TYPE
{
		DEVICE_VOICE,							/*�����豸*/
		DEVICE_VOIP,							/*VOIP�豸*/
		DEVICE_USER,							/*��ϯ�豸*/
		DEVICE_TRUNK,							/*�����豸*/
		DEVICE_INTERFACE,						/*�ӿ��豸*/
		DEVICE_DSP								/*DSP�豸*/
};

/*�ӿ��豸���Ͷ���************************************************************/
enum	LINE_TYPE
{
		LINE_IN,								/*����*/
		LINE_OUT,								/*����*/
		LINE_NULL								/*����*/
};

/*�ź������Ͷ���*************************************************************/
enum TONE_TYPE
{
	DIAL_TONE,									/*������*/
	BACK_TONE,									/*������*/
	BUSY_TONE,									/*æ��*/
	STOP_TONE									/*ֹͣ����*/
};

/*VOICE�豸״̬ȫ�ֽṹ�嶨��*********************************************/
typedef struct
{
	long			fChannelNo;					/*ȫ���ڲ���ʶ��*/
	long			fIsExit;					/*��ͨ���Ƿ����,1����,0������*/
	DeviceID_t		fDeviceID;					/*�����ڲ��豸���*/			
	DEVICE_TYPE		fDeviceType;				/*�豸����*/
	bool			fIsOpen;					/*OpenDeivce�ɹ���־*/
	bool			fErrorFlag;					/*�Ƿ�ִ�й�CloseDevice*/
	char			fCallOutNumber[20];			/*���к���*/
	char			fCallInNumber[20];			/*���к���*/
	char			fLocalNumber[20];			/*��������*/
	VOICE_STATUS	fStatusCode;				/*��ǰ״̬��*/
	bool			fEnabled;					/*ͨ���Ƿ�����*/
	bool			fEnabledCallLong;			/*�Ƿ�������г�;*/
	char			fDtmfBuff[20];				/*DTMF������*/
	int				fDtmfCount;					/*DTMF�볤��*/
	CTime			fDtmfRefreshTime;			/*DTMF���������¸���ʱ��*/
	int				fDtmfNeedLen;				/*DTMF����������Ҫ�ĳ���*/
	VOICE_STATUS	fTurnStatusCode;			/*DTMF��������ת��״̬��*/
	VOICE_STATUS	fDtmfTimeOutStatusCode;		/*DTMF��ʱ����Ҫ��ת��״̬*/
	long			fLinkTrunkChannelNo;		/*�������豸ȫ�ֺ�*/
	LINK_TYPE		fLinkTrunkType;				/*��������*/
	DeviceID_t		fLinkTrunkDeviceID;			/*�����豸���*/
	bool			fIsLinkTrunk;				/*�Ƿ��Ѿ���������*/
	long			fLinkVoipChannelNo;			/*������VOIP�豸ȫ�ֺ�*/
	LINK_TYPE		fLinkVoipType;				/*VOIP��������*/
	DeviceID_t		fLinkVoipDeviceID;			/*����VOIP�豸���*/
	bool			fIsLinkVoip;				/*�Ƿ��Ѿ���������*/
	int				fLineState;					/*��·״̬*/

}TYPE_VOICE_DATA;


/*VOIP�豸����ip��ַ������*********************************************/
typedef struct XMS_VOIP_ADDR_TAG
{
	TCHAR	tszAddr[20];
	USHORT	port;
	TCHAR	tszUserID[20];
	TCHAR	tszTelNo[20];
} XMS_VOIP_ADDR;


/*VOIP�豸״̬ȫ�ֽṹ�嶨��*********************************************/
typedef struct
{
	long			fChannelNo;					/*ȫ���ڲ���ʶ��*/
	long			fIsExit;					/*��ͨ���Ƿ����,1����,0������*/
	DeviceID_t		fDeviceID;					/*�����ڲ��豸���*/			
	DEVICE_TYPE		fDeviceType;				/*�豸����*/
	int				fLineState;					/*��·����״̬*/
	bool			fIsOpen;					/*OpenDeivce�ɹ���־*/
	bool			fErrorFlag;					/*�Ƿ�ִ�й�CloseDevice*/
	int				fProtocolType;				/*VOIPЭ������*/	
	XMS_VOIP_ADDR	fSourceAddr;				/*���е�ַ*/
	XMS_VOIP_ADDR	fTargetAddr;				/*���е�ַ*/
	char			fCallOutNumber[20];			/*���к���*/
	char			fCallInNumber[20];			/*���к���*/
	char			fLocalNumber[20];			/*��������*/
	VOIP_STATUS		fStatusCode;				/*��ǰ״̬��*/
	bool			fEnabled;					/*ͨ���Ƿ�����*/
	char			fDtmfBuff[20];				/*dtmf������*/
	int				fDtmfCount;					/*��ǰͨ��DTMF�볤��*/
	CTime			fDtmfRefreshTime;			/*dtmf���������¸���ʱ��*/
	int				fDtmfNeedLen;				/*Dtmf����������Ҫ�ĳ���*/
	VOIP_STATUS		fTurnStatusCode;			/*Dtmf��������ת��״̬��*/
	VOIP_STATUS		fDtmfTimeOutStatusCode;		/*DTMF��ʱ����Ҫ��ת��״̬*/
	int				fLinkVoiceChannelNo;				/*�������豸ȫ�ֺ�*/
	LINK_TYPE		fLinkVoiceType;					/*��������*/
	DeviceID_t		fLinkVoiceDeviceID;				/*�����豸���*/
	bool			fIsLinkVoice;					/*�Ƿ��Ѿ���������*/
	DeviceID_t		fLinkTrunkChannelID;			/*�����Ľӿ��豸*/
	long			fLinkTrunkChannelNo;			/*�豸ȫ�ֱ��*/
	LINE_TYPE		fLinkTrunkLineType;				/*������*/
	bool			fIsLinkTrunk;					/*�Ƿ����ӽӿ��豸*/
}TYPE_VOIP_DATA;



/*�ӿ��豸״̬ȫ�ֽṹ�嶨��*********************************************/
typedef struct
{
	long			fChannelNo;					/*ȫ���ڲ���ʶ��*/
	long			fIsExit;					/*��ͨ���Ƿ����,1����,0������*/
	int				fLineState;					/*��·״̬*/	
	DeviceID_t		fDeviceID;					/*�����ڲ��豸���*/			
	DEVICE_TYPE		fDeviceType;				/*�豸����*/
	bool			fIsOpen;					/*OpenDeivce�ɹ���־*/
	bool			fErrorFlag;					/*�Ƿ�ִ�й�CloseDevice*/
	LINE_TYPE		fLineType;					/*��·���ͣ������߻�������*/
	char			fCallOutNumber[20];			/*���к���*/
	char			fCallInNumber[20];			/*���к���*/
	char			fLocalNumber[20];			/*��������*/
    char            fSpeedDial[20];             /*�����Ӧ������ٲ��ż�*/
	char			fTargetUserNo[20];			/*ת��ֻ�����*/
	VOICE_STATUS	fStatusCode;				/*��ǰ״̬��*/
	bool			fEnabled;					/*ͨ���Ƿ�����*/
	bool			fEnabledCallLong;			/*�Ƿ�������г�;*/
	char			fDtmfBuff[20];				/*DTMF������*/
	int				fDtmfCount;					/*DTMF�볤��*/
	CTime			fDtmfRefreshTime;			/*DTMF���������¸���ʱ��*/
	int				fDtmfNeedLen;				/*DTMF����������Ҫ�ĳ���*/
	VOICE_STATUS	fTurnStatusCode;			/*DTMF��������ת��״̬��*/
	VOICE_STATUS	fDtmfTimeOutStatusCode;		/*DTMF��ʱ����Ҫ��ת��״̬*/
	long			fLinkVoiceChannelNo;			/*�������豸ȫ�ֺ�*/
	DeviceID_t		fLinkVoiceDeviceID;				/*���������豸���*/
	bool			fIsLinkVoice;					/*�Ƿ��Ѿ���������*/
	long			fLinkTrunkChannelNo;			/*�����Ľӿ��豸ȫ�ֱ��*/
	DeviceID_t		fLinkTrunkDeviceID;				/*�����Ľӿ��豸���*/
	bool			fisLinkVoipVoice;				/*�Ƿ������VOIP����Ӧ��voice�豸*/
	DeviceID_t		fLinkVoipVoiceDeviceID;			/*������VOIP����Ӧ��VOICE�豸���*/
	long			fLinkVoipVoiceDeviceNo;			/*������VOIP����Ӧ�Ե�VOICE�豸ȫ�ֱ��*/
	long			fLinkVoipDeviceNo;				/*������VOIPͨ����������IP������ϯ������*/
	DeviceID_t		fLinkVoipDeviceID;				/*������VOIPͨ����������IP������ϯ������*/
	bool			fIsLinkTrunk;					/*�Ƿ�������*/
	int				fChaHuanCount;					/*�Ĳ�ɴ���*/
	long			fPrevTrunkChannelNo;			/*�ӻص��м�ͨ�����*/
	DeviceID_t		fPrevTrunkDeviceID;				/*�ӻص��м��豸���*/
	long			fPrevVoipChannelNo;				/*�ӻص�VOIP�豸���*/
	DeviceID_t		fPrevVoipDeviceID;				/*�ӻص�VOIP�豸���*/
	int				fPrevDeviceType;				/*��Ҫ�ؽӵ��豸���͡�1:Ϊtrunk 2:Ϊvoip��-1ΪĬ��ֵ*/
	bool			fIsChaHuan;						/*�Ƿ����Ĳ��*/
	bool			fChaHuanIsReset;				/*��ɺ��й������Ƿ�һ�*/
	bool			fChaHuanIsCallStatus;			/*��ɺ��й������Ƿ��Ѿ��ɹ�*/
	long			fChaHuanLinkChannenlNo;			/*����м�ת�ӵ��û�ͨ�����*/
	char			fDescription[30];				/*��ȡ������*/
}TYPE_INTERFACE_DATA;


/*DSP�豸״̬ȫ�ֽṹ�嶨��*********************************************/
typedef struct
{
	long			fDspNo;						/*DSPϵͳȫ�ֱ��*/
	DJ_S32			fDspID;						/*DSP��ţ��ر�ע�⣺�ñ���Ƕ����ײ���*/
	int				fIsExits;					/*��Ǹ�DSP�豸�Ƿ����,1����,0������*/
	bool			fIsOpen;					/*��Ǹ�DSP�Ƿ��*/
	DeviceID_t		fDeviceID;					/*�����ڲ��豸���*/			
	DEVICE_TYPE		fDeviceType;				/*�豸����*/
	long			fVoiceCount;				/*�����豸����*/
	long			fVoiceOpenCount;			/*�ɹ��򿪵������豸����*/
	TYPE_VOICE_DATA	*pVoiceDevice;				/*���е������豸ָ��*/
	long			fTrunkCount;				/*�ӿ��豸����*/
	long			fTrunkOpenCount;			/*�򿪵Ľӿ��豸����*/
	TYPE_INTERFACE_DATA	*pTrunkDevice;			/*���еĽӿ��豸ָ��*/
	long			fVoipCount;					/*VOIP�豸����*/
	long			fVoipOpenCount;				/*�򿪵�VOIP�豸����*/
	TYPE_VOIP_DATA	*pVoipDevice;				/*���е�VOIP�豸ָ��*/	
}TYPE_DSP_DATA;


/*ͨ���豸ȫ�ּ����ṹ�嶨��********************************************/
typedef struct
{
	long			fChannelNo;					/*ͨ��Ψһ���*/
	long			fDspNo;						/*DSPϵͳȫ�ֱ�ţ�Ҳ����DSP ID*/
	long			fIndex;						/*������±��*/
	ChannelID_t		fChannelID_t;				/*�����ڲ�ϵͳ���*/
	DEVICE_TYPE		fDeviceType;				/*�豸����*/
	long			fRowNo;						/*��listcontrol�е��к�*/
}TYPE_INTERFACE_INDEX;


/*ȫ�ֺ�������**********************************************************/
bool	getConfigFromIni();										/*��INI�ļ��л�ȡȫ�ֱ���*/
bool	InitSystem();											/*ϵͳ��ʼ������*/
void	InitStatusControl(CListCtrl *pControl);					/*��ʼ���б�ؼ�*/
DJ_Void XMSEventHandler(DJ_U32 esrParam);						/*XMS�¼��ص�����*/
VOID CALLBACK TimerProc(HWND hwnd,UINT uMsg,UINT idEvent,DWORD dwTime);		/*��ʱ���ص�����������ɨ��DTMF������*/
bool	getXMSDeviceList();										/*��ȡ�������豸�б�*/
void	CloseXMSConnect();										/*�Ͽ���XMS������*/
long	getDeviceNo();											/*��ȡһ��ȫ�ֵ��豸����*/	
long	getVoiceDeviceNo();										/*��ȡ�����豸��ȫ���豸���*/
long	getVoipDeviceNo();										/*��ȡVOIP�豸��ȫ���豸���*/
long	getTrunkDeviceNo();										/*��ȡ�ӿ��豸��ȫ���豸���*/
bool	InsertDeviceToUse(DJ_S32	mDspID,Acs_Dev_List_Head_t *pDevList);		/*����һ���豸*/
bool	JudgeDspInit(DJ_S32 mDspID);								/*�жϸ�DSP�Ƿ��Ѿ���ʼ����*/			
bool	OpenAllDevice();										/*�������豸*/
bool	OpenDeviceSuccess(DJ_S32 mDspID,DeviceID_t	*pDeviceID);			/*���豸�ɹ�*/
long	FindIndexByChannelID(DJ_S32 mDspID,ChannelID_t mChannelID,DEVICE_TYPE mDeviceType);	/*ͨ���豸�����±����*/
long	FindArrayIndexByChannelID(DJ_S32 mDspID,ChannelID_t mChannelID,DEVICE_TYPE mDeviceType);	/*ͨ���豸�����±����*/
char *	GetVoiceStatusString(VOICE_STATUS mStatus);				/*���������豸״̬����ȡ��Ӧ�������ַ���*/
char *	GetVoipStatusString(VOIP_STATUS mStatus);				/*����VOIP�豸״̬����ȡ��Ӧ�������ַ���*/
char *	GetLineStatusString(int mLineState);					/*��ȡ��·״̬�ַ���*/
void	ResetChannel(DJ_S32  mDspID,DeviceID_t	*pDevice);		/*ͨ���򿪺�ĳ�ʼ��*/
void	DrawAllItem();											/*��ʾ�����豸״̬*/
void	DrawOneItem(DJ_S32 mDspNo,ChannelID_t mChannelNo,DEVICE_TYPE	mDeviceType);		/*����ȫ���豸��ţ���ʾ��Ӧ����Ϣ*/
bool	LinkInitVoice();										/*����ͨ����Ĭ�ϰ�����ӿ��豸�������豸��������*/	
bool	InitLineDbRecord(long mDeviceNo,DJ_S32 mDspID,DeviceID_t *pDevice);		/*��ͨ����Ϣд�����ݿ⣬���ڵ�һ�γ�ʼ��*/
void	CloseAllDevice();										/*�ر������豸�������˳�ʱ�����*/
bool	getTrunkInfoFromDb(long mDeviceNo,DJ_S32 mDspNo,ChannelID_t mChannelID,LINE_TYPE mLineType);	/*�����ݿ��ж�ȡ������·�ĺ���*/	
void	getInterfaceMainType(short mType,char *pType);/*�������ͣ���ȡ��Ӧ�������ַ���*/
void	getInterfaceSubType(short mType,char *pType);	/*���������ͣ���ȡ��Ӧ�������ַ���*/
long	getDeviceNoByChannelID(long mDspNo,ChannelID_t	mChannelID);/*�����豸��ź�ͨ���Ŵ����ݿ��м����豸�Ƿ��Ѿ���ʼ��*/
bool	UpdateLineStatus(long mDeviceNo,long mDspNo,ChannelID_t mChannelID,short mType);/*������������д�����ݿ⣬������ͬ��*/
void	ProcessDeviceStatusEvt(DeviceID_t	*pDeviceID,int fLineState);	/*����ײ��Զ����͵���·״̬��Ϣ*/		
bool	OpenAllDeviceInOneDsp(long	mDspNo);					/*��һ��dsp�������豸*/
long	FindIndexByChannelNo(DJ_S32 mDspNo,long mDeviceNo,DEVICE_TYPE mDeviceType);	/*����ȫ���豸��ţ���ȡ��Ӧ��index�����±�*/
void	DisplayStatusBar();										/*��̬��ʾ״̬����״̬*/	
bool	JudgeCanEdit(int	mItemindex,DEVICE_TYPE *pDeviceType,long	*pDeviceNo,LINE_TYPE *pLineType,DJ_S32 *pDspNo);/*�ж��Ƿ��ܽ��б༭*/
bool	RefreshDeviceGUI(long mDspNo,long mDeviceNo,LINE_TYPE  mLineType);/*�����ݿ��ж�ȡ�µĺ��룬ˢ�½���*/
void	DisplayStatusBar(CString pInfo);
long	FindFreeVoiceDevice(long mDspNo);						/*����һ�����е�����ͨ��*/
bool	SetTrunkLineInitParameter(DeviceID_t *pTrunkDevice,DeviceID_t *pVoiceDevice);	/*��������ͨ����ʼ����������*/
bool	SetVoiceGTD(DeviceID_t *pVoiceDevice);					/*���������豸��GTD����*/
DJ_S32	PlayTone(DeviceID_t	*pVoiceDevice,TONE_TYPE mToneType);	/*���Ż���ֹͣ���Ÿ����ź���*/
void	SetTrunkDeviceState(DeviceID_t	*pDevice,VOIP_STATUS mStatus);	/*���ýӿ��豸��ͨ��״̬*/
void	SetVoiceDeviceState(DeviceID_t	*pDevice,VOICE_STATUS mStatus);	/*���������豸��ͨ��״̬*/
void	SetVoipDeviceState(DeviceID_t	*pDevice,VOIP_STATUS mStatus);	/*��סVOIP�豸��ͨ��״̬*/
long	FindFreeTrunkToUse();											/*����һ�����õ�����ͨ��*/
void	SetLink (DeviceID_t	*pSourceDev,DeviceID_t *pTargetDev);		/*���������豸֮�������*/
void	DeleteLink(DeviceID_t	*pSourceDev,DeviceID_t	*pTargetDev);	/*��������豸֮�������*/
void	ResetTrunkDevice(DeviceID_t *pDevice,Acs_Evt_t	*pEvt);			/*��������ͨ��Ϊ��ʼ��״̬*/
void	ResetUserDevice(DeviceID_t *pDevice,Acs_Evt_t	*pEvt);			/*��������ͨ��Ϊ��ʼ��״̬*/
long	FindChannelNoByPhoneNumber(char	*	mPhoneNumber);				/*���ݵ绰����������ڵ��豸ȫ��ͨ����*/
void	UserDeviceWork(DeviceID_t	*pDevice,Acs_Evt_t	*pEvt);			/*���߹�������*/
void	TrunkDeviceWork(DeviceID_t	*pDevice,Acs_Evt_t	*pEvt);			/*���߹�������*/
void	InsertDtmfToDevice(long mDspNo,long mIndex,DEVICE_TYPE mDeviceType,char * pDtmf);	/*����dtmf������*/
void	InsertDtmfToDeviceLmy(long mDspNo,long mIndex,DEVICE_TYPE mDeviceType,char TmpDtmf);  /*lmy����*/
bool	CallUser(long mDspNo,long mIndex);								/*ʵ�����ߺ�������*/		
long	FindTargetIndexByChannelNo(long	mChannelNo);					/*����ȫ���豸��ţ����������ڵ�index�������ڵ��±�*/
void	CallTrunk(long mDspNo,long mIndex,bool isCallLong);				/*���ߺ��д���*/
bool	PlayFile(DeviceID_t *pDevice,DJ_S8 *pFileName,bool isStop);					/*�ļ�����*/
bool	JudgePhoneNumIsValid(char * pPhoneNumber);						/*�ж��û�����ĺ������Ƿ��зǷ�����*/
void	VoipDeviceWork(DeviceID_t	*pDevice,Acs_Evt_t	*pEvt);			/*VOIP�豸�¼�������*/
long	FindFreeVoipToUse(DJ_S32 mDspNo);								/*����һ�����õ�VOIP�豸����ʹ��*/
void	CallVoip(long mDspNo,long mIndex,bool isCallLong);				/*����voip�豸���к���*/
void	ResetVoipDevice(DeviceID_t *pDevice,Acs_Evt_t	*pEvt);			/*��λVOIP�豸*/
void	CallUserByVoip(long mDspNo,long mIndex);						/*ʵ��VOIP������ϯ��Ӧ��*/
void	CallUserByTrunk(long mDspNo,long mIndex);						/*ʵ�����ߺ�������*/
bool	PlayPhoneNumber(DJ_S32	mDspNo,long	mIndex);					/*���ű�������*/
bool	QueryCallLongStatus(DJ_S32	mDspNo,long	mIndex);				/*��ѯ�绰�Ƿ�ͨ��;����*/
bool	SetEnableCallLong(DJ_S32	mDspNo,long	mIndex);				/*�������ó�;����*/
bool	SetDisEnableCallLong(DJ_S32	mDspNo,long	mIndex);				/*ȡ����;����*/
bool	PlayMenu(DJ_S32	mDspNo,long	mIndex);							/*���Ų˵�*/
bool	SetEnableCall(DJ_S32	mDspNo,long	mIndex);					/*�����绰*/
bool	SetDisableCall(DJ_S32	mDspNo,long	mIndex);					/*�رյ绰*/
void	ChaHuanProcess(DJ_S32	mDspNo,long mIndex);					/*�Ĳ�ɴ���*/
long	FindHuiXianFreeTrunkToUse();									/*Ѱ��һ��ͨ��ȥ���ӻ���ͨ����*/
long    FindChannelNoBySpeedDial(char * mSpeedDial);					/*�����ٲ��Ų������ڵ��豸ȫ��ͨ����*/
void	CallTrunkByVoip(long mDspNo,long mIndex);								/*����IP��������*/
bool	SetUserDeviceParam(DeviceID_t *pUserDevice,DeviceID_t *pVoiceDevice);    /*����user�豸param*/
bool	PlayFileCircly(DeviceID_t *pDevice,DJ_S8 *pFileName,bool isStop);					/*�ļ�����*/
char    My_GetDtmfCode ( Acs_Evt_t *pAcsEvt );							/*�Ż�DTMF��*/


/*************************************************************************************/
void	DebugOutput();				//���Ժ�������ʾ���е��ڴ����
void	AddMsg(char * mInfo);
void	VerifyData();
void	DisplayVoipStatus();
void	DisplayEvt(Acs_Evt_t *pEvt);