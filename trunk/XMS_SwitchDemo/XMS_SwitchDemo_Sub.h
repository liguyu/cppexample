/*
 *业务功能处理（最主要的功能代码）
 */
#ifndef _XMS_SWITCHDEMO_SUB_H_
#define _XMS_SWITCHDEMO_SUB_H_

#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"

#define	MAX_FILE_NAME_LEN		    256
#define	MAX_SEND_IODATA_DTMF_LEN	16

enum VOICE_STATE {
	VOC_WAITOPEN,
	VOC_FREE,
	VOC_USED,
	VOC_WAIT_REMOVE
};
//中继通道状态
enum TRUNK_STATE { 
	TRK_WAITOPEN,//初始通道状态
	TRK_FREE,//打开通道状态		
	TRK_WAIT_ANSWERCALL,//等待呼叫应答
	TRK_WAIT_LINKOK,//中继通道设备与语音通道设备 时隙连接建立成功");
	TRK_WELCOME,//欢迎语
	TRK_EXTNUM, //分机号
	TRK_CONNECT_FAILURE,//连接失败状态	
	TRK_WAITCONNECT,//等待连接
	TRK_CONNECT,//中继处于连接状态
	TRK_CALLOUT,//通道外呼状态
	TRK_NOTHANDLE,	
	TRK_WAIT_REMOVE,
};
//坐席通道状态
enum USER_STATE {
	USER_WAITOPEN,//初始状态
	USER_FREE,//打开通道状态
	USER_OFFHOOK,//坐席摘机状态
	USER_CALLOUT,//坐席外呼状态
	USER_RING,//坐席振铃状态
	USER_CONNECT,//坐席连接	
	USER_CONNECT_FAILURE,//连接失败状态
};
typedef struct{  //接口通道设备
	DeviceID_t	deviceID;
	int			iSeqID;
	int			iModSeqID;
	int			iLineState;

	DeviceID_t	VocDevID;
	DJ_U8		u8PlayTag;

	TRUNK_STATE	State;
	USER_STATE  UserState;
	int			iUserSeqID;
	DeviceID_t	LinkDevID;
	int			DtmfCount;
	char		DtmfBuf[32];
	char		CallerCode[20];
	char		CalledCode[20];

}TRUNK_STRUCT;

typedef struct{  //语音通道设备
	int			iSeqID;
	DeviceID_t	deviceID;
	DeviceID_t	UsedDevID;
	VOICE_STATE	State;
}VOICE_STRUCT;

typedef struct{ //E1端口设备
	DeviceID_t	deviceID;
	int			iSeqID;
	bool		bOpenFlag;		// flag of OpenDevice OK
	DJ_U8		u8E1Type;
	DJ_S32		s32AlarmVal;
}PCM_STRUCT;


// ----- declare function -----
bool InitSystem(void);
bool CheckPlayEnd ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt );
bool	IsTrunk ( DJ_S16 s16DevSub );

void AddMsg(char *str);
void Change_State ( TRUNK_STRUCT *pOneTrunk, TRUNK_STATE NewState );
void CheckRemoveReady ( DJ_S8 s8DspModID );
void Change_Voc_State ( VOICE_STRUCT *pOneVoice, VOICE_STATE NewState );
void Change_UserState ( TRUNK_STRUCT *pOneTrunk, USER_STATE NewState );
void CloseDeviceOK ( DeviceID_t *pDevice );
void ReadFromConfig(void);
void InitListDSPRes(void);//初始化以DSP 模块为单位的资源总体的显示列表框
void InitListChnRes(void);//接口通道资源的显示
void InitListPCM(void);
void InitListAnalogUser(void);
void InitListMsg(void);
void InitTextBox(void);
void InitTrunkChannel ( TRUNK_STRUCT *pOneTrunk );
void InitUserChannel ( TRUNK_STRUCT *pOneUser );
void OpenAllDevice_Dsp(DJ_S8 s8DspModID);
void OpenDeviceOK( DeviceID_t *pDevice );
void PrepareForCallerID ( TRUNK_STRUCT *pOneTrunk );
void ReDrawAll(void);
void ResetUser( TRUNK_STRUCT *pOneUser, Acs_Evt_t *pAcsEvt );
void SetGTD_ToneParam( DeviceID_t *pDevice );
void SetGtD_AnalogTrunk(DeviceID_t* pDevId);
void SpecialForCas ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt );
void HandleDevState ( Acs_Evt_t *pAcsEvt );
void TrunkWork ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt );
void UserWork ( TRUNK_STRUCT *pOneUser, Acs_Evt_t *pAcsEvt );
void My_AddDtmfBuf ( TRUNK_STRUCT *pOneTrunk, char TmpDtmf );
char My_GetGtdCode ( Acs_Evt_t *pAcsEvt );
char My_GetDtmfCode ( Acs_Evt_t *pAcsEvt );
void My_DualUnlink ( DeviceID_t *pDev1, DeviceID_t *pDev2 );
void My_BuildIndex ( DeviceID_t	*pVocDevID );
void My_InitDtmfBuf ( TRUNK_STRUCT *pOneTrunk );
void My_DualLink ( DeviceID_t *pDev1, DeviceID_t *pDev2 );
void ExitSystem(void);	//退出系统
void FetchFromText(void);
void WriteToConfig(void);

int  FreeOneFreeVoice (  DeviceID_t *pFreeVocDeviceID );
int	 GetOutUserID (  char *pDtmf, DeviceID_t *pUserDeviceID );
int	 SearchOneFreeTrunk ( DeviceID_t *pFreeTrkDeviceID );
int  SearchOneFreeVoice (  TRUNK_STRUCT *pOneTrunk, DeviceID_t *pFreeVocDeviceID );
DJ_S32   StopPlayFile ( DeviceID_t	*pVocDevID );
DJ_S32	PlayTone ( DeviceID_t	*pVocDevID, int iPlayType );
DJ_S32	 PlayFile ( DeviceID_t	*pVocDevID, DJ_S8 *s8FileName, DJ_U8 u8PlayTag, bool bIsQueue );
unsigned char *My_GetFskCode ( Acs_Evt_t *pAcsEvt );



#endif