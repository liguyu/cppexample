#ifndef _XMS_DEMO_EVENT_H_
#define _XMS_DEMO_EVENT_H_


#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"

//
#define		MAX_DSP_MODULE_NUMBER_OF_XMS		256

#define		MAX_PCM_NUM_IN_THIS_DEMO			64
#define		MAX_VOIP_NUM_IN_THIS_DEMO			(32*MAX_PCM_NUM_IN_THIS_DEMO)

#define		M_OneVoice(DevID)		AllDeviceRes[(DevID).m_s8ModuleID].pVoice[(DevID).m_s16ChannelID]
#define		M_OneVOIP(DevID)		AllDeviceRes[(DevID).m_s8ModuleID].pVOIP[(DevID).m_s16ChannelID]

// --------------------------------------------------------------------------------
// 定义结构：单个DSP可以提供的设备资源
typedef	struct
{
	long	lFlag;				// 本DSP是否存在, 0：不存在，1：存在

	DeviceID_t	deviceID;		// 本DSP模块的deviceID
	int			iSeqID;			// 本DSP模块的顺序号

	long	lVocNum;			// 该DSP上的ACS_DEVMAIN_VOICE数量
	long	lVocOpened;			// 该DSP上OpenDevice成功的VOICE数量
	long	lVocFreeNum;		// 该DSP上可用的VOICE数量
	VOICE_STRUCT	*pVoice;	// 根据需要，分配相应的结构


	long	lVOIPNum;			// 该DSP上的ACS_DEVMAIN_INTERFACE_CH数量
	long	lVOIPOpened;		// 该DSP上OpenDevice成功的Trunk数量
	VOIP_STRUCT	*pVOIP;			// 根据需要，分配相应的结构

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