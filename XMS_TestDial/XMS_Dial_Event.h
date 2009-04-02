#ifndef _XMS_DIAL_EVENT_H_
#define _XMS_DIAL_EVENT_H_


#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"

//
#define		MAX_DSP_MODULE_NUMBER_OF_XMS		256

#define		MAX_PCM_NUM_IN_THIS_DEMO			64
#define		MAX_TRUNK_NUM_IN_THIS_DEMO			(32*MAX_PCM_NUM_IN_THIS_DEMO)

#define		M_OneVoice(DevID)		AllDeviceRes[(DevID).m_s8ModuleID].pVoice[(DevID).m_s16ChannelID]
#define		M_OnePcm(DevID)			AllDeviceRes[(DevID).m_s8ModuleID].pPcm[(DevID).m_s16ChannelID]
#define		M_OneTrunk(DevID)		AllDeviceRes[(DevID).m_s8ModuleID].pTrunk[(DevID).m_s16ChannelID]    

// --------------------------------------------------------------------------------
// ����ṹ������DSP�����ṩ���豸��Դ
typedef	struct
{
	long	lFlag;				// ��DSP�Ƿ����, 0�������ڣ�1������

	DeviceID_t	deviceID;		// ��DSPģ���deviceID
	int			iSeqID;			// ��DSPģ���˳���

	long	lVocNum;			// ��DSP�ϵ�ACS_DEVMAIN_VOICE����
	long	lVocOpened;			// ��DSP��OpenDevice�ɹ���VOICE����
	long	lVocFreeNum;		// ��DSP�Ͽ��õ�VOICE����
	VOICE_STRUCT	*pVoice;	// ������Ҫ��������Ӧ�Ľṹ

	long	lPcmNum;			// ��DSP�ϵ�ACS_DEVMAIN_DIGITAL_PORT����
	long	lPcmOpened;			// ��DSP��OpenDevice�ɹ���Pcm����
	PCM_STRUCT		*pPcm;		// ������Ҫ��������Ӧ�Ľṹ

	long	lTrunkNum;			// ��DSP�ϵ�ACS_DEVMAIN_INTERFACE_CH����
	long	lTrunkOpened;		// ��DSP��OpenDevice�ɹ���Trunk����
	TRUNK_STRUCT	*pTrunk;			// ������Ҫ��������Ӧ�Ľṹ

} TYPE_XMS_DSP_DEVICE_RES_DEMO;

// ����ṹ����ͨ���ţ����Է���ز��ҵ�ģ���ͨ��
//           ��Ҫ���Ĵ˽ṹ��Ҳ��Ҫ��״̬���߱�������˽ṹ��
typedef struct
{
    ModuleID_t      m_s8ModuleID;    /*device module type*/
    ChannelID_t     m_s16ChannelID;  /*device chan id*/
} TYPE_CHANNEL_MAP_TABLE;
// 

DJ_Void EvtHandler(DJ_U32 esrParam);

void	InitAllDeviceRes (void);
void	FreeAllDeviceRes (void);

#endif