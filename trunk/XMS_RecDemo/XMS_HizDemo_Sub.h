#ifndef _XMS_HIZDEMO_SUB_H_
#define _XMS_HIZDEMO_SUB_H_

#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"

#define	MAX_FILE_NAME_LEN		256
#define	MAX_APPNAME_LEN			30
#define	MAX_MONITOR_GROUP_NUM	100

enum TRUNK_STATE {
	TRK_WAITOPEN,
	TRK_FREE,	
	TRK_CALL_GENERATE,
	TRK_CONNECT,
//	TRK_ACCOUNT,
//	TRK_PASSWORD,
//	TRK_SELECT,		
//	TRK_PLAYRESULT,
	TRK_RECORDFILE,
//	TRK_PLAYRECORD,	
	TRK_DISCONNECT,

	// new add for XMS
//	TRK_WAIT_ANSWERCALL,
//	TRK_WAIT_LINKOK,

	TRK_WAIT_REMOVE,
};

typedef struct
{
	// ----------------
	DeviceID_t	deviceID;
	int			iSeqID;
	int			iModSeqID;
	int			iLineState;

	DeviceID_t	VocDevID;
	DJ_U8		u8PlayTag;
	DJ_U8       u8IsRecordFlag;
    DJ_U8       u8RecordCounter;   //record voice file number

	// -----------------
	DJ_U8       u8IsBGFlag;        //TRUE: this channel is the BG channel, FALSE: this channel is not the BG channel
	DJ_U8       u8IsBGTxFlag;      //TRUE: this channel is the Tx BG channel, FALSE: this channel is not the Tx BG channel
	DeviceID_t	BGDevID;           //the other deviceID of a BG 

	// -----------------
	TRUNK_STATE	State;             //the state appointed the Tx channel's

	int		DtmfCount;
	char	DtmfBuf[32];

	char CallerCode[20];
	char CalleeCode[20];

} TRUNK_STRUCT;

enum VOICE_STATE {
	VOC_WAITOPEN,
	VOC_FREE,
	VOC_USED,

	VOC_WAIT_REMOVE
} ;

typedef struct
{
	// ----------------
	DeviceID_t	deviceID;
	int			iSeqID;

	DeviceID_t	UsedDevID;

	// ----------------
	VOICE_STATE	State;


} VOICE_STRUCT;

typedef struct
{
	// ----------------
	DeviceID_t	deviceID;
	int			iSeqID;
	bool		bOpenFlag;		// flag of OpenDevice OK

	//
	DJ_U8		u8E1Type;
	DJ_S32		s32AlarmVal;
} PCM_STRUCT;

typedef  struct 
{
	DJ_U8       m_MonitorFirstDspModuleID;		//the dsp ID of Monitor Group
	DJ_U8       m_MonitorFirstE1;				//the first E1 number
	DJ_U8       m_MonitorSecondModuleID;		//the dsp ID of Monitor Group
	DJ_U8       m_MonitorSecondE1;				//the second E1 number
	DJ_S8		MonitorSS7DPCOrOPC[10];			//高阻七号监听时有效，第n个监听组所监听E1的DPCOrOPC，十六进制；前六个字符有用例如：014E91
	DJ_S8		MonitorSS7OPCOrDPC[10];         //高阻七号监听时有效，第n个监听组所监听E1的OPCOrDPC，十六进制；与MonitorSS7DPCOrOPC[n]相对应
	DJ_U8		MonitorSS7_PCM;                 // CIC_Hig7
}MonitorGroupInfo;


// ----- declare function -----
bool	InitSystem(void);
void	ExitSystem(void); 
void	AddMsg ( char *str);
void	OpenAllDevice_Dsp ( DJ_S8 s8DspModID );
void	CloseAllDevice_Dsp ( DJ_S8 s8DspModID );
void	ReOpen_AllDevice (void);
void	HandleDevState ( Acs_Evt_t *pAcsEvt );
void	InitTrunkChannel ( TRUNK_STRUCT *pOneTrunk );
void	ReDrawAll (void);
void	OpenDeviceOK ( DeviceID_t *pDevice );
void	CloseDeviceOK ( DeviceID_t *pDevice );
void	Remove_OneDsp(void);
void	My_BuildIndex ( DeviceID_t	*pVocDevID );
void	Change_State ( TRUNK_STRUCT *pOneTrunk, TRUNK_STATE NewState );
void	Change_Voc_State ( VOICE_STRUCT *pOneVoice, VOICE_STATE NewState );
void	PrepareForCallerID ( TRUNK_STRUCT *pOneTrunk );
void	TrunkWork_Analog ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt );//模拟高阻
void	TrunkWork_Digital ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt );//数字话机
void	TrunkWork_ISDN ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt );//ISDN
void	TrunkWork_SS7 ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt );//SS7

DJ_S32	PlayFile ( DeviceID_t	*pVocDevID, DJ_S8 *s8FileName, DJ_U8 u8PlayTag, bool bIsQueue = false );
DJ_S32	PlayIndex ( DeviceID_t	*pVocDevID, DJ_U16 u16Index, DJ_U8 u8PlayTag, bool bIsQueue );
DJ_S32	StopRecordFile ( DeviceID_t	*pVocDevID );

char	My_GetDtmfCode ( Acs_Evt_t *pAcsEvt );
char*	My_GetFskCode ( Acs_Evt_t *pAcsEvt );
int		SearchOneFreeVoice (  TRUNK_STRUCT *pOneTrunk, DeviceID_t *pFreeVocDeviceID );
int		InitMGNode( TRUNK_STRUCT * pOneTrunk );

#endif