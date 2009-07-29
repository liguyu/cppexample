#ifndef _HELLO_KEYGOE_SUB_H_
#define _HELLO_KEYGOE_SUB_H_

#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"

enum	REMOVE_STATE
{
	DSP_REMOVE_STATE_NONE	=	0,		// Do not remove the DSP hardware
	DSP_REMOVE_STATE_START	=	1,		// Ready to remove DSP hardware, wait all the Devices' Resource release
	DSP_REMOVE_STATE_READY	=	2,		// All the Devices' Resource have released, start delete DSP hardware
};

enum TRUNK_STATE{	//�м�ͨ��״̬
	TRK_WAITOPEN,
	TRK_FREE,	
	USER_OFFHOOK,
	TRK_WELCOME,
};

typedef struct{
	DeviceID_t	deviceID;
	int			iSeqID;
	int			iModSeqID;
	int			iLineState;
	
	DeviceID_t	VocDevID;	//�����������豸ID
	DJ_U8		u8PlayTag;
	
	TRUNK_STATE	State;
	int		DtmfCount;
	char	DtmfBuf[32];
	
	char CallerCode[20];
	char CalledCode[20];
} TRUNK_STRUCT;

enum VOICE_STATE {//�����߼�״̬
	VOC_WAITOPEN,
	VOC_FREE,
	VOC_USED,
	VOC_WAIT_REMOVE
};

typedef struct{
	DeviceID_t	deviceID;
	int			iSeqID;
	DeviceID_t	UsedDevID;
	VOICE_STATE	State;
}VOICE_STRUCT;

typedef struct 
{ 
	long lFlag; // ��DSP�Ƿ����, 0�������ڣ�1������
	DeviceID_t deviceID; // ��DSPģ���deviceID 
	int iSeqID; // ��DSPģ���˳���
	bool bOpenFlag; // OpenDevice�ɹ��ı�־
	bool bErrFlag; // ������CloseDevice�¼��ı�־
	REMOVE_STATE RemoveState; // ֹͣDSPӲ���ı�־
	long lVocNum; // ��DSP��ACS_DEVMAIN_VOICE����
	long lVocOpened; // ��DSP��OpenDevice�ɹ���VOICE����
	long lVocFreeNum; // ��DSP�Ͽ��õ�VOICE����
	VOICE_STRUCT *pVoice; // ������Ҫ����̬������Ӧ�Ľṹ
	long lTrunkNum; //��DSP��ACS_DEVMAIN_INTERFACE_CH����
	long lTrunkOpened; // ��DSP��OpenDevice�ɹ���Trunk����
	TRUNK_STRUCT *pTrunk; // ������Ҫ����̬������Ӧ�Ľṹ 
	
} TYPE_XMS_DSP_DEVICE_RES_DEMO; // ����ṹ������DSP�����ṩ���豸��Դ
//Keygoe��������256��DSP��Ԫ
#define MAX_DSP_MODULE_NUMBER_OF_XMS 256 

bool InitSystem(void);
void InitListTrunk(void);
void OpenDeviceOK ( DeviceID_t *pDevice );
DJ_Void EvtHandler(DJ_U32 esrParam);
void UserWork( TRUNK_STRUCT *pOneUser, Acs_Evt_t *pAcsEvt );
void	ExitSystem();

#endif
