/*
 *ҵ���ܴ�������Ҫ�Ĺ��ܴ��룩
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
//�м�ͨ��״̬
enum TRUNK_STATE { 
	TRK_WAITOPEN,//��ʼͨ��״̬
	TRK_FREE,//��ͨ��״̬		
	TRK_WAIT_ANSWERCALL,//�ȴ�����Ӧ��
	TRK_WAIT_LINKOK,//�м�ͨ���豸������ͨ���豸 ʱ϶���ӽ����ɹ�");
	TRK_WELCOME,//��ӭ��
	TRK_EXTNUM, //�ֻ���
	TRK_CONNECT_FAILURE,//����ʧ��״̬	
	TRK_WAITCONNECT,//�ȴ�����
	TRK_CONNECT,//�м̴�������״̬
	TRK_CALLOUT,//ͨ�����״̬
	TRK_NOTHANDLE,	
	TRK_WAIT_REMOVE,
};
//��ϯͨ��״̬
enum USER_STATE {
	USER_WAITOPEN,//��ʼ״̬
	USER_FREE,//��ͨ��״̬
	USER_OFFHOOK,//��ϯժ��״̬
	USER_CALLOUT,//��ϯ���״̬
	USER_RING,//��ϯ����״̬
	USER_CONNECT,//��ϯ����	
	USER_CONNECT_FAILURE,//����ʧ��״̬
};
typedef struct{  //�ӿ�ͨ���豸
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

typedef struct{  //����ͨ���豸
	int			iSeqID;
	DeviceID_t	deviceID;
	DeviceID_t	UsedDevID;
	VOICE_STATE	State;
}VOICE_STRUCT;

typedef struct{ //E1�˿��豸
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
void InitListDSPRes(void);//��ʼ����DSP ģ��Ϊ��λ����Դ�������ʾ�б��
void InitListChnRes(void);//�ӿ�ͨ����Դ����ʾ
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
void ExitSystem(void);	//�˳�ϵͳ
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