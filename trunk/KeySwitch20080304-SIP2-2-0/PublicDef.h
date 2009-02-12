/*本系统所有函数和接口的定义以及申明****************************************/

#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"
#include "StdAfx.h"
#include "KeySwitch.h"
#include "KeySwitchDlg.h"



/*全局常量定义*************************************************************/
#define SYSTEM_INI_FILE		"\\KeygoeSwitch.ini"
#define APPLICATION_ID		12
#define DEBUG_TYPE			0
#define ID_TIME             1002				/*扫描各通道DTMF码定时器编码*/	
#define MAX_DSP_COUNT		3					/*最多支持256个DSP---正式发布时候进行调整*/
#define MAX_VOICE_COUNT		MAX_DSP_COUNT*125	/*最多语音设备数量*/
#define MAX_VOIP_COUNT		MAX_DSP_COUNT*30	/*最多VOIP设备数量*/
#define MAX_INTERFACE_COUNT	MAX_DSP_COUNT*128	/*最多通道数目：包括voice,trunk*/
#define	MAX_SEND_IODATA_DTMF_LEN	16


/*声音文件路径常量定义*****************************************************/
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
#define	HAVE_LONG_NOTICE_VOC		"E:\\Music\\HaveCallLong.pcm"		/*具有长途功能语音提示*/
#define NOT_HAVE_LONG_NOTICE_VOC	"E:\\Music\\NotHaveCallLong.pcm"	/*关闭长途功能语音提示*/
#define	SETTING_FINISH_VOC			"E:\\Music\\finishsetting.pcm"	/*设置完成*/
#define CALL_NUMBER_NOT_EXIT_VOC	"E:\\Music\\nonumber.pcm"			/*呼叫的号码不存在*/
#define	IS_BUSY_NOW_VOC				"E:\\Music\\istalkrow.pcm"			/*对方正在通话中*/
#define	MENU_VOC					"E:\\Music\\menu.pcm"			/*对方正在通话中*/
#define	ENABLE_THE_CALL_VOC			"E:\\Music\\enablethephone.pcm"	/*开启电话*/
#define	DISABLE_THE_CALL_VOC		"E:\\Music\\disablethephone.pcm"	/*关闭电话*/
#define NO_TRUNK_FOR_DIAL_VOC		"E:\\Music\\notrunkfordial.pcm"	/*无空闲外线通道*/
#define CALL_SELF_VOC				"E:\\Music\\callself.pcm"			/*给自己打电话的提示音*/
#define	BACKGROUND_MUSIC			"E:\\Music\\music.pcm"			/*背景音乐*/
#define BACK_SIGN_MUSIC				"E:\\Music\\SIGRING.pcm"

/*语音设备状态定义(VOICE)**************************************************/
enum	VOICE_STATUS
{
		VOC_WAIT_OPEN,							/*等待打开*/
		VOC_WAIT_WORK,							/*等待进行初始化绑定*/
		VOC_FREE,								/*空闲*/
		VOC_OFFHOOK,							/*摘机*/
		VOC_WAITHANGUP,							/*等待挂机*/
		VOC_WAIT_DIAL,							/*等待拨号*/	
		VOC_DTMF_INPUT,							/*正在收号*/
		VOC_CALL_OUT,							/*呼出中*/
		VOC_CALL_OUT_FAILE,						/*呼叫失败*/	
		VOC_CALL_OUT_SUCCESS,					/*呼叫成功*/
		VOC_CONNECTED,							/*通话中*/
		VOC_CONNECT_DTMF,						/*通话中用户按键*/
		VOC_ONHOOK,								/*挂机*/
		VOC_FLASH,								/*拍叉簧*/
		VOC_FLASH_WAIT_DIAL,					/*拍叉簧等待拨号*/
		VOC_FLASH_CONNECT,						/*拍叉簧连接*/
		VOC_CALL_IN,							/*有电话呼入*/
		VOC_RINGING,							/*振铃中*/	
		VOC_RING_TIMEOUT,						/*振铃超时*/
		VOC_FETCH_DIAL,							/*抢接拨号*/
		VOC_FETCH_CONNECT,						/*抢接连接*/
		VOC_SYS_USE,							/*底层使用*/
		VOC_ERROR_CALL_LONG,					/*非法呼叫长途播放错误音状态*/
		VOC_PLAY_NOTICE,						/*播放语音提示*/
		VOC_PLAY_NUMBER,						/*播放本机号码*/
		VOC_PLAY_WELCOME,						/*播放欢迎语音提示*/
		VOC_ANSWER_CALL,						/*ANSWER CALL*/
		VOC_PLAY_VOIP_ERROR,					/*VOIP播放错误音*/
		VOC_HUIXIAN_TURN,						/*汇线通抢接*/
		VOC_WAIT_MAIN,							/*抢接发码等待*/
		VOC_WAIT_FIRST_DIAL_TONE,				/*等待第一个拨号音*/
		VOC_WAIT_SECOND_DIAL_TONE,				/*等待第二个拨号音*/
		VOC_PLAY_DIAL_WRONG						/*呼叫无效号码*/
};

/*VOIP设备状态定义(VOIP)**************************************************/
enum	VOIP_STATUS
{
		VOIP_WAIT_OPEN,							/*等待打开*/
		VOIP_WAIT_WORK,							/*等待进行初始化绑定*/
		VOIP_FREE,								/*空闲*/
		VOIP_CALL_IN,							/*有电话呼入*/
		VOIP_WAIT_CONNECT,						/*内线连接中*/	
		VOIP_CALL_OUT_FAILE,					/*内线呼叫失败*/
		VOIP_CALL_OUT_SUCCESS,					/*内线呼叫成功*/
		VOIP_CONNECTED,							/*通话中*/	
		VOIP_CONNECT_DTMF,						/*通话中用户按键*/
		VOIP_ONHOOK,							/*挂机*/
		VOIP_CALL_VOIP,							/*有电话呼出*/
		VOIP_CALL_VOIP_FAILE,					/*呼叫外线失败*/
		VOIP_PLAY_FILE,							/*VOIP放音*/
		VOIP_WAITHUNGUP,						/*等待挂机*/
		VOIP_CALL_VOIP_SUCCESS					/*呼叫外线成功*/
};

/*设备关联的类型定义******************************************************/
enum	LINK_TYPE
{
		LINK_SLOT,								/*时隙连接*/
		LINK_BIND								/*绑定连接*/
};

/*设备类型定义************************************************************/
enum	DEVICE_TYPE
{
		DEVICE_VOICE,							/*语音设备*/
		DEVICE_VOIP,							/*VOIP设备*/
		DEVICE_USER,							/*坐席设备*/
		DEVICE_TRUNK,							/*外线设备*/
		DEVICE_INTERFACE,						/*接口设备*/
		DEVICE_DSP								/*DSP设备*/
};

/*接口设备类型定义************************************************************/
enum	LINE_TYPE
{
		LINE_IN,								/*内线*/
		LINE_OUT,								/*外线*/
		LINE_NULL								/*空线*/
};

/*信号音类型定义*************************************************************/
enum TONE_TYPE
{
	DIAL_TONE,									/*拨号音*/
	BACK_TONE,									/*回铃音*/
	BUSY_TONE,									/*忙音*/
	STOP_TONE									/*停止播放*/
};

/*VOICE设备状态全局结构体定义*********************************************/
typedef struct
{
	long			fChannelNo;					/*全局内部标识符*/
	long			fIsExit;					/*该通道是否存在,1存在,0不存在*/
	DeviceID_t		fDeviceID;					/*东进内部设备编号*/			
	DEVICE_TYPE		fDeviceType;				/*设备类型*/
	bool			fIsOpen;					/*OpenDeivce成功标志*/
	bool			fErrorFlag;					/*是否执行过CloseDevice*/
	char			fCallOutNumber[20];			/*被叫号码*/
	char			fCallInNumber[20];			/*主叫号码*/
	char			fLocalNumber[20];			/*本机号码*/
	VOICE_STATUS	fStatusCode;				/*当前状态码*/
	bool			fEnabled;					/*通道是否启用*/
	bool			fEnabledCallLong;			/*是否允许呼叫长途*/
	char			fDtmfBuff[20];				/*DTMF缓冲区*/
	int				fDtmfCount;					/*DTMF码长度*/
	CTime			fDtmfRefreshTime;			/*DTMF缓冲区最新更新时间*/
	int				fDtmfNeedLen;				/*DTMF触发流程需要的长度*/
	VOICE_STATUS	fTurnStatusCode;			/*DTMF触发后，跳转的状态码*/
	VOICE_STATUS	fDtmfTimeOutStatusCode;		/*DTMF超时所需要跳转的状态*/
	long			fLinkTrunkChannelNo;		/*关联的设备全局号*/
	LINK_TYPE		fLinkTrunkType;				/*关联类型*/
	DeviceID_t		fLinkTrunkDeviceID;			/*关联设备编号*/
	bool			fIsLinkTrunk;				/*是否已经建立连接*/
	long			fLinkVoipChannelNo;			/*关联的VOIP设备全局号*/
	LINK_TYPE		fLinkVoipType;				/*VOIP关联类型*/
	DeviceID_t		fLinkVoipDeviceID;			/*关联VOIP设备编号*/
	bool			fIsLinkVoip;				/*是否已经建立连接*/
	int				fLineState;					/*线路状态*/

}TYPE_VOICE_DATA;


/*VOIP设备连接ip地址子类型*********************************************/
typedef struct XMS_VOIP_ADDR_TAG
{
	TCHAR	tszAddr[20];
	USHORT	port;
	TCHAR	tszUserID[20];
	TCHAR	tszTelNo[20];
} XMS_VOIP_ADDR;


/*VOIP设备状态全局结构体定义*********************************************/
typedef struct
{
	long			fChannelNo;					/*全局内部标识符*/
	long			fIsExit;					/*该通道是否存在,1存在,0不存在*/
	DeviceID_t		fDeviceID;					/*东进内部设备编号*/			
	DEVICE_TYPE		fDeviceType;				/*设备类型*/
	int				fLineState;					/*线路物理状态*/
	bool			fIsOpen;					/*OpenDeivce成功标志*/
	bool			fErrorFlag;					/*是否执行过CloseDevice*/
	int				fProtocolType;				/*VOIP协议类型*/	
	XMS_VOIP_ADDR	fSourceAddr;				/*主叫地址*/
	XMS_VOIP_ADDR	fTargetAddr;				/*被叫地址*/
	char			fCallOutNumber[20];			/*被叫号码*/
	char			fCallInNumber[20];			/*主叫号码*/
	char			fLocalNumber[20];			/*本机号码*/
	VOIP_STATUS		fStatusCode;				/*当前状态码*/
	bool			fEnabled;					/*通道是否启用*/
	char			fDtmfBuff[20];				/*dtmf缓冲区*/
	int				fDtmfCount;					/*当前通道DTMF码长度*/
	CTime			fDtmfRefreshTime;			/*dtmf缓冲区最新更新时间*/
	int				fDtmfNeedLen;				/*Dtmf触发流程需要的长度*/
	VOIP_STATUS		fTurnStatusCode;			/*Dtmf触发后，跳转的状态码*/
	VOIP_STATUS		fDtmfTimeOutStatusCode;		/*DTMF超时所需要跳转的状态*/
	int				fLinkVoiceChannelNo;				/*关联的设备全局号*/
	LINK_TYPE		fLinkVoiceType;					/*关联类型*/
	DeviceID_t		fLinkVoiceDeviceID;				/*关联设备编号*/
	bool			fIsLinkVoice;					/*是否已经建立连接*/
	DeviceID_t		fLinkTrunkChannelID;			/*关联的接口设备*/
	long			fLinkTrunkChannelNo;			/*设备全局编号*/
	LINE_TYPE		fLinkTrunkLineType;				/*线类型*/
	bool			fIsLinkTrunk;					/*是否连接接口设备*/
}TYPE_VOIP_DATA;



/*接口设备状态全局结构体定义*********************************************/
typedef struct
{
	long			fChannelNo;					/*全局内部标识符*/
	long			fIsExit;					/*该通道是否存在,1存在,0不存在*/
	int				fLineState;					/*线路状态*/	
	DeviceID_t		fDeviceID;					/*东进内部设备编号*/			
	DEVICE_TYPE		fDeviceType;				/*设备类型*/
	bool			fIsOpen;					/*OpenDeivce成功标志*/
	bool			fErrorFlag;					/*是否执行过CloseDevice*/
	LINE_TYPE		fLineType;					/*线路类型，是内线还是外线*/
	char			fCallOutNumber[20];			/*被叫号码*/
	char			fCallInNumber[20];			/*主叫号码*/
	char			fLocalNumber[20];			/*本机号码*/
    char            fSpeedDial[20];             /*定义对应号码的速拨号键*/
	char			fTargetUserNo[20];			/*转向分机号码*/
	VOICE_STATUS	fStatusCode;				/*当前状态码*/
	bool			fEnabled;					/*通道是否启用*/
	bool			fEnabledCallLong;			/*是否允许呼叫长途*/
	char			fDtmfBuff[20];				/*DTMF缓冲区*/
	int				fDtmfCount;					/*DTMF码长度*/
	CTime			fDtmfRefreshTime;			/*DTMF缓冲区最新更新时间*/
	int				fDtmfNeedLen;				/*DTMF触发流程需要的长度*/
	VOICE_STATUS	fTurnStatusCode;			/*DTMF触发后，跳转的状态码*/
	VOICE_STATUS	fDtmfTimeOutStatusCode;		/*DTMF超时所需要跳转的状态*/
	long			fLinkVoiceChannelNo;			/*关联的设备全局号*/
	DeviceID_t		fLinkVoiceDeviceID;				/*关联语音设备编号*/
	bool			fIsLinkVoice;					/*是否已经建立连接*/
	long			fLinkTrunkChannelNo;			/*关联的接口设备全局编号*/
	DeviceID_t		fLinkTrunkDeviceID;				/*关联的接口设备编号*/
	bool			fisLinkVoipVoice;				/*是否关联了VOIP所对应的voice设备*/
	DeviceID_t		fLinkVoipVoiceDeviceID;			/*关联的VOIP所对应的VOICE设备编号*/
	long			fLinkVoipVoiceDeviceNo;			/*关联的VOIP所的应对的VOICE设备全局编号*/
	long			fLinkVoipDeviceNo;				/*关联的VOIP通道，仅用于IP呼叫座席，拆线*/
	DeviceID_t		fLinkVoipDeviceID;				/*关联的VOIP通道，仅用于IP呼叫座席，拆线*/
	bool			fIsLinkTrunk;					/*是否建立连接*/
	int				fChaHuanCount;					/*拍叉簧次数*/
	long			fPrevTrunkChannelNo;			/*接回的中继通道编号*/
	DeviceID_t		fPrevTrunkDeviceID;				/*接回的中继设备编号*/
	long			fPrevVoipChannelNo;				/*接回的VOIP设备编号*/
	DeviceID_t		fPrevVoipDeviceID;				/*接回的VOIP设备编号*/
	int				fPrevDeviceType;				/*需要回接的设备类型。1:为trunk 2:为voip，-1为默认值*/
	bool			fIsChaHuan;						/*是否是拍叉簧*/
	bool			fChaHuanIsReset;				/*叉簧呼叫过程中是否挂机*/
	bool			fChaHuanIsCallStatus;			/*叉簧呼叫过程中是否已经成功*/
	long			fChaHuanLinkChannenlNo;			/*叉簧中间转接的用户通道编号*/
	char			fDescription[30];				/*收取主被叫*/
}TYPE_INTERFACE_DATA;


/*DSP设备状态全局结构体定义*********************************************/
typedef struct
{
	long			fDspNo;						/*DSP系统全局编号*/
	DJ_S32			fDspID;						/*DSP编号，特别注意：该编号是东进底层编号*/
	int				fIsExits;					/*标记该DSP设备是否存在,1存在,0不存在*/
	bool			fIsOpen;					/*标记该DSP是否打开*/
	DeviceID_t		fDeviceID;					/*东进内部设备编号*/			
	DEVICE_TYPE		fDeviceType;				/*设备类型*/
	long			fVoiceCount;				/*语音设备总数*/
	long			fVoiceOpenCount;			/*成功打开的语音设备总数*/
	TYPE_VOICE_DATA	*pVoiceDevice;				/*所有的语音设备指针*/
	long			fTrunkCount;				/*接口设备总数*/
	long			fTrunkOpenCount;			/*打开的接口设备总数*/
	TYPE_INTERFACE_DATA	*pTrunkDevice;			/*所有的接口设备指针*/
	long			fVoipCount;					/*VOIP设备总数*/
	long			fVoipOpenCount;				/*打开的VOIP设备总数*/
	TYPE_VOIP_DATA	*pVoipDevice;				/*所有的VOIP设备指针*/	
}TYPE_DSP_DATA;


/*通道设备全局检索结构体定义********************************************/
typedef struct
{
	long			fChannelNo;					/*通道唯一编号*/
	long			fDspNo;						/*DSP系统全局编号，也就是DSP ID*/
	long			fIndex;						/*数组的下标号*/
	ChannelID_t		fChannelID_t;				/*东进内部系统编号*/
	DEVICE_TYPE		fDeviceType;				/*设备类型*/
	long			fRowNo;						/*在listcontrol中的行号*/
}TYPE_INTERFACE_INDEX;


/*全局函数定义**********************************************************/
bool	getConfigFromIni();										/*从INI文件中获取全局变量*/
bool	InitSystem();											/*系统初始化函数*/
void	InitStatusControl(CListCtrl *pControl);					/*初始化列表控件*/
DJ_Void XMSEventHandler(DJ_U32 esrParam);						/*XMS事件回调函数*/
VOID CALLBACK TimerProc(HWND hwnd,UINT uMsg,UINT idEvent,DWORD dwTime);		/*定时器回调函数，用于扫描DTMF缓冲区*/
bool	getXMSDeviceList();										/*获取交换机设备列表*/
void	CloseXMSConnect();										/*断开与XMS的连接*/
long	getDeviceNo();											/*获取一个全局的设备编码*/	
long	getVoiceDeviceNo();										/*获取语音设备的全局设备编号*/
long	getVoipDeviceNo();										/*获取VOIP设备的全局设备编号*/
long	getTrunkDeviceNo();										/*获取接口设备的全局设备编号*/
bool	InsertDeviceToUse(DJ_S32	mDspID,Acs_Dev_List_Head_t *pDevList);		/*增加一个设备*/
bool	JudgeDspInit(DJ_S32 mDspID);								/*判断该DSP是否已经初始化过*/			
bool	OpenAllDevice();										/*打开所有设备*/
bool	OpenDeviceSuccess(DJ_S32 mDspID,DeviceID_t	*pDeviceID);			/*打开设备成功*/
long	FindIndexByChannelID(DJ_S32 mDspID,ChannelID_t mChannelID,DEVICE_TYPE mDeviceType);	/*通用设备数组下标查找*/
long	FindArrayIndexByChannelID(DJ_S32 mDspID,ChannelID_t mChannelID,DEVICE_TYPE mDeviceType);	/*通用设备数组下标查找*/
char *	GetVoiceStatusString(VOICE_STATUS mStatus);				/*根据语音设备状态，获取相应的描述字符串*/
char *	GetVoipStatusString(VOIP_STATUS mStatus);				/*根据VOIP设备状态，获取相应的描述字符串*/
char *	GetLineStatusString(int mLineState);					/*获取线路状态字符串*/
void	ResetChannel(DJ_S32  mDspID,DeviceID_t	*pDevice);		/*通道打开后的初始化*/
void	DrawAllItem();											/*显示所有设备状态*/
void	DrawOneItem(DJ_S32 mDspNo,ChannelID_t mChannelNo,DEVICE_TYPE	mDeviceType);		/*根据全局设备编号，显示相应的信息*/
bool	LinkInitVoice();										/*进行通道的默认邦定，将接口设备与语音设备进行捆绑*/	
bool	InitLineDbRecord(long mDeviceNo,DJ_S32 mDspID,DeviceID_t *pDevice);		/*将通道信息写入数据库，用于第一次初始化*/
void	CloseAllDevice();										/*关闭所有设备，程序退出时候调用*/
bool	getTrunkInfoFromDb(long mDeviceNo,DJ_S32 mDspNo,ChannelID_t mChannelID,LINE_TYPE mLineType);	/*从数据库中读取各个线路的号码*/	
void	getInterfaceMainType(short mType,char *pType);/*根据类型，获取相应的类型字符串*/
void	getInterfaceSubType(short mType,char *pType);	/*根据子类型，获取相应的类型字符串*/
long	getDeviceNoByChannelID(long mDspNo,ChannelID_t	mChannelID);/*根据设备编号和通道号从数据库中检测该设备是否已经初始化*/
bool	UpdateLineStatus(long mDeviceNo,long mDspNo,ChannelID_t mChannelID,short mType);/*将内外线数据写入数据库，并进行同步*/
void	ProcessDeviceStatusEvt(DeviceID_t	*pDeviceID,int fLineState);	/*处理底层自动发送的线路状态信息*/		
bool	OpenAllDeviceInOneDsp(long	mDspNo);					/*打开一个dsp的所有设备*/
long	FindIndexByChannelNo(DJ_S32 mDspNo,long mDeviceNo,DEVICE_TYPE mDeviceType);	/*根据全局设备编号，获取相应的index数组下标*/
void	DisplayStatusBar();										/*动态显示状态栏的状态*/	
bool	JudgeCanEdit(int	mItemindex,DEVICE_TYPE *pDeviceType,long	*pDeviceNo,LINE_TYPE *pLineType,DJ_S32 *pDspNo);/*判断是否能进行编辑*/
bool	RefreshDeviceGUI(long mDspNo,long mDeviceNo,LINE_TYPE  mLineType);/*从数据库中读取新的号码，刷新界面*/
void	DisplayStatusBar(CString pInfo);
long	FindFreeVoiceDevice(long mDspNo);						/*查找一个空闲的语音通道*/
bool	SetTrunkLineInitParameter(DeviceID_t *pTrunkDevice,DeviceID_t *pVoiceDevice);	/*设置外线通道初始化工作参数*/
bool	SetVoiceGTD(DeviceID_t *pVoiceDevice);					/*设置语音设备的GTD参数*/
DJ_S32	PlayTone(DeviceID_t	*pVoiceDevice,TONE_TYPE mToneType);	/*播放或者停止播放各种信号音*/
void	SetTrunkDeviceState(DeviceID_t	*pDevice,VOIP_STATUS mStatus);	/*设置接口设备的通道状态*/
void	SetVoiceDeviceState(DeviceID_t	*pDevice,VOICE_STATUS mStatus);	/*设置语音设备的通道状态*/
void	SetVoipDeviceState(DeviceID_t	*pDevice,VOIP_STATUS mStatus);	/*射住VOIP设备的通道状态*/
long	FindFreeTrunkToUse();											/*查找一个可用的外线通道*/
void	SetLink (DeviceID_t	*pSourceDev,DeviceID_t *pTargetDev);		/*建立两个设备之间的连接*/
void	DeleteLink(DeviceID_t	*pSourceDev,DeviceID_t	*pTargetDev);	/*清除两个设备之间的连接*/
void	ResetTrunkDevice(DeviceID_t *pDevice,Acs_Evt_t	*pEvt);			/*设置外线通道为初始化状态*/
void	ResetUserDevice(DeviceID_t *pDevice,Acs_Evt_t	*pEvt);			/*设置内线通道为初始化状态*/
long	FindChannelNoByPhoneNumber(char	*	mPhoneNumber);				/*根据电话号码查找所在的设备全局通道号*/
void	UserDeviceWork(DeviceID_t	*pDevice,Acs_Evt_t	*pEvt);			/*内线工作函数*/
void	TrunkDeviceWork(DeviceID_t	*pDevice,Acs_Evt_t	*pEvt);			/*外线工作函数*/
void	InsertDtmfToDevice(long mDspNo,long mIndex,DEVICE_TYPE mDeviceType,char * pDtmf);	/*更新dtmf缓冲区*/
void	InsertDtmfToDeviceLmy(long mDspNo,long mIndex,DEVICE_TYPE mDeviceType,char TmpDtmf);  /*lmy更改*/
bool	CallUser(long mDspNo,long mIndex);								/*实现内线呼叫内线*/		
long	FindTargetIndexByChannelNo(long	mChannelNo);					/*根据全局设备编号，查找他所在的index数组所在的下标*/
void	CallTrunk(long mDspNo,long mIndex,bool isCallLong);				/*外线呼叫处理*/
bool	PlayFile(DeviceID_t *pDevice,DJ_S8 *pFileName,bool isStop);					/*文件放音*/
bool	JudgePhoneNumIsValid(char * pPhoneNumber);						/*判断用户输入的号码中是否有非法号码*/
void	VoipDeviceWork(DeviceID_t	*pDevice,Acs_Evt_t	*pEvt);			/*VOIP设备事件处理函数*/
long	FindFreeVoipToUse(DJ_S32 mDspNo);								/*查找一个可用的VOIP设备进行使用*/
void	CallVoip(long mDspNo,long mIndex,bool isCallLong);				/*利用voip设备进行呼叫*/
void	ResetVoipDevice(DeviceID_t *pDevice,Acs_Evt_t	*pEvt);			/*复位VOIP设备*/
void	CallUserByVoip(long mDspNo,long mIndex);						/*实现VOIP呼叫坐席的应答*/
void	CallUserByTrunk(long mDspNo,long mIndex);						/*实现外线呼叫内线*/
bool	PlayPhoneNumber(DJ_S32	mDspNo,long	mIndex);					/*播放本机号码*/
bool	QueryCallLongStatus(DJ_S32	mDspNo,long	mIndex);				/*查询电话是否开通长途功能*/
bool	SetEnableCallLong(DJ_S32	mDspNo,long	mIndex);				/*设置启用长途功能*/
bool	SetDisEnableCallLong(DJ_S32	mDspNo,long	mIndex);				/*取消长途功能*/
bool	PlayMenu(DJ_S32	mDspNo,long	mIndex);							/*播放菜单*/
bool	SetEnableCall(DJ_S32	mDspNo,long	mIndex);					/*开启电话*/
bool	SetDisableCall(DJ_S32	mDspNo,long	mIndex);					/*关闭电话*/
void	ChaHuanProcess(DJ_S32	mDspNo,long mIndex);					/*拍叉簧处理*/
long	FindHuiXianFreeTrunkToUse();									/*寻找一个通道去抢接汇线通号码*/
long    FindChannelNoBySpeedDial(char * mSpeedDial);					/*根据速拨号查找所在的设备全局通道号*/
void	CallTrunkByVoip(long mDspNo,long mIndex);								/*利用IP呼叫外线*/
bool	SetUserDeviceParam(DeviceID_t *pUserDevice,DeviceID_t *pVoiceDevice);    /*设置user设备param*/
bool	PlayFileCircly(DeviceID_t *pDevice,DJ_S8 *pFileName,bool isStop);					/*文件放音*/
char    My_GetDtmfCode ( Acs_Evt_t *pAcsEvt );							/*放回DTMF码*/


/*************************************************************************************/
void	DebugOutput();				//调试函数，显示所有的内存变量
void	AddMsg(char * mInfo);
void	VerifyData();
void	DisplayVoipStatus();
void	DisplayEvt(Acs_Evt_t *pEvt);