#ifndef _XMS_DEMO_EVENT_H_
#define _XMS_DEMO_EVENT_H_


#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"

//
#define		MAX_DSP_MODULE_NUMBER_OF_XMS		256

#define		MAX_PCM_NUM_IN_THIS_DEMO			64
#define		MAX_TRUNK_NUM_IN_THIS_DEMO			(32*MAX_PCM_NUM_IN_THIS_DEMO)

#define		M_OneVoice(DevID)		AllDeviceRes[(DevID).m_s8ModuleID].pVoice[(DevID).m_s16ChannelID]
#define		M_OnePcm(DevID)			AllDeviceRes[(DevID).m_s8ModuleID].pPcm[(DevID).m_s16ChannelID]
#define		M_OneTrunk(DevID)		AllDeviceRes[(DevID).m_s8ModuleID].pTrunk[(DevID).m_s16ChannelID]

enum	REMOVE_STATE
{
	DSP_REMOVE_STATE_NONE	=	0,		// Do not remove the DSP hardware
	DSP_REMOVE_STATE_START	=	1,		// Ready to remove DSP hardware, wait all the Devices' Resource release
	DSP_REMOVE_STATE_READY	=	2,		// All the Devices' Resource have released, start delete DSP hardware
};

// --------------------------------------------------------------------------------
// define the structure: Single DSP's available Device Resource
typedef	struct
{
	long	lFlag;				// If this DSP exist, 0: not exist, 1: exist

	DeviceID_t	deviceID;		// this DSP's deviceID
	int			iSeqID;			// this DSP's Sequence ID
	bool		bOpenFlag;		// flag of OpenDevice OK
	bool		bErrFlag;		// flag of CloseDevice Event
	REMOVE_STATE	RemoveState;	// the state of stop DSP hardware

	long	lVocNum;			// the XMS_DEVMAIN_VOICE number in this DSP
	long	lVocOpened;			// the VOICE number opened by OpenDevice()
	long	lVocFreeNum;		// the free voice number in this DSP
	VOICE_STRUCT	*pVoice;	// the structer of voice, alloc as need

	long	lPcmNum;			// the XMS_DEVMAIN_DIGITAL_PORT number in this DPS
	long	lPcmOpened;			// the PCM number opened by OpenDevice()
	PCM_STRUCT		*pPcm;		// the structer of PCM, alloc as need

	long	lTrunkNum;			// the XMS_DEVMAIN_INTERFACE_CH number in this DSP
	long	lTrunkOpened;		// the Trunk number opened by OpenDevice()
	TRUNK_STRUCT	*pTrunk;	// the structer of Trunk, alloc as need

} TYPE_XMS_DSP_DEVICE_RES_DEMO;

// define the structer: use this, you can search the ModuleID & ChannelID
//           Warning: don't change this structer
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
