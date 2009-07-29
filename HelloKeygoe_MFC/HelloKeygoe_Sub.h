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

enum TRUNK_STATE{	//中继通道状态
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
	
	DeviceID_t	VocDevID;	//关联的语音设备ID
	DJ_U8		u8PlayTag;
	
	TRUNK_STATE	State;
	int		DtmfCount;
	char	DtmfBuf[32];
	
	char CallerCode[20];
	char CalledCode[20];
} TRUNK_STRUCT;

enum VOICE_STATE {//语音逻辑状态
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
	long lFlag; // 本DSP是否存在, 0：不存在，1：存在
	DeviceID_t deviceID; // 本DSP模块的deviceID 
	int iSeqID; // 本DSP模块的顺序号
	bool bOpenFlag; // OpenDevice成功的标志
	bool bErrFlag; // 发生过CloseDevice事件的标志
	REMOVE_STATE RemoveState; // 停止DSP硬件的标志
	long lVocNum; // 该DSP上ACS_DEVMAIN_VOICE数量
	long lVocOpened; // 该DSP上OpenDevice成功的VOICE数量
	long lVocFreeNum; // 该DSP上可用的VOICE数量
	VOICE_STRUCT *pVoice; // 根据需要，动态分配相应的结构
	long lTrunkNum; //该DSP上ACS_DEVMAIN_INTERFACE_CH数量
	long lTrunkOpened; // 该DSP上OpenDevice成功的Trunk数量
	TRUNK_STRUCT *pTrunk; // 根据需要，动态分配相应的结构 
	
} TYPE_XMS_DSP_DEVICE_RES_DEMO; // 定义结构：单个DSP可以提供的设备资源
//Keygoe最多可以有256个DSP单元
#define MAX_DSP_MODULE_NUMBER_OF_XMS 256 

bool InitSystem(void);
void InitListTrunk(void);
void OpenDeviceOK ( DeviceID_t *pDevice );
DJ_Void EvtHandler(DJ_U32 esrParam);
void UserWork( TRUNK_STRUCT *pOneUser, Acs_Evt_t *pAcsEvt );
void	ExitSystem();

#endif
