#ifndef _XMS_FAXDEMO_EVENT_H_
#define _XMS_FAXDEMO_EVENT_H_


#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"

//
#define		MAX_DSP_MODULE_NUMBER_OF_XMS		256

#define		MAX_PCM_NUM_IN_THIS_DEMO		64
#define		MAX_TRUNK_NUM_IN_THIS_DEMO		(32*MAX_PCM_NUM_IN_THIS_DEMO)

#define		M_OneVoice(DevID)		AllDeviceRes[(DevID).m_s8ModuleID].pVoice[(DevID).m_s16ChannelID]
#define		M_OnePcm(DevID)			AllDeviceRes[(DevID).m_s8ModuleID].pPcm[(DevID).m_s16ChannelID]
#define		M_OneTrunk(DevID)		AllDeviceRes[(DevID).m_s8ModuleID].pTrunk[(DevID).m_s16ChannelID]
#define		M_OneFax(DevID)			AllDeviceRes[(DevID).m_s8ModuleID].pFax[(DevID).m_s16ChannelID]
// add by chenwy 2009-02-18
#define		M_OneVoip(DevID)		AllDeviceRes[(DevID).m_s8ModuleID].pVoip[(DevID).m_s16ChannelID]
// add end
// --------------------------------------------------------------------------------
// 定义结构：单个DSP可以提供的设备资源
typedef	struct
{
	long	lFlag;				// 本DSP是否存在, 0：不存在，1：存在

	DeviceID_t	deviceID;		// 本DSP模块的deviceID
	int		iSeqID;			// 本DSP模块的顺序号

	long	lVocNum;			// 该DSP上的ACS_DEVMAIN_VOICE数量
	long	lVocOpened;			// 该DSP上OpenDevice成功的VOICE数量
	long	lVocFreeNum;			// 该DSP上可用的VOICE数量
	VOICE_STRUCT	*pVoice;		// 根据需要，分配相应的结构

	long	lPcmNum;			// 该DSP上的ACS_DEVMAIN_DIGITAL_PORT数量
	long	lPcmOpened;			// 该DSP上OpenDevice成功的Pcm数量
	PCM_STRUCT		*pPcm;		// 根据需要，分配相应的结构
/*
	long	lTrunkNum;			// 该DSP上的ACS_DEVMAIN_INTERFACE_CH数量
	long	lTrunkOpened;			// 该DSP上OpenDevice成功的Trunk数量
	TRUNK_STRUCT	*pTrunk;		// 根据需要，分配相应的结构
*/
	long    lFaxNum;			//  该DSP上的ACS_DEVMAIN_FAX数量
	long    lFaxOpened;			//  该DSP上OpenDevice成功的Fax数量
	long    lFaxFreeNum;			//  该DSP上可用的FAX数量
	FAX_STRUCT    *pFax;
	// add by chenwy 2009-02-18
	long	lVoipNum;			// 该DSP上的voip数量
	long	lVoipOpened;			// 该DSP上打开的Voip数量
	long	lVoipFreeNum;			// 该DSP上可用的Voip数量
	VOIP_STRUCT	*pVoip;			// VOIP结构数组
	// add end

} TYPE_XMS_DSP_DEVICE_RES_DEMO;

// 定义结构：从通道号，可以方便地查找到模块和通道
//           不要更改此结构，也不要将状态或者变量放入此结构中
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