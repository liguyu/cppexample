#ifndef _XMS_DEMO_SUB_H_
#define _XMS_DEMO_SUB_H_

#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"

#define	MAX_FILE_NAME_LEN		256

enum VOIP_STATE {
	VOIP_WAITOPEN,
	VOIP_FREE,
	VOIP_NOTHANDLE,
	VOIP_RECORDFILE,
	VOIP_PLAYRECORD,	
	VOIP_WAITHANGUP,
	VOIP_WAITBINDOK,

	// new add for XMS
	VOIP_WAIT_ANSWERCALL,
};


typedef struct
{
	// ----------------
	DeviceID_t	deviceID;
	int			iSeqID;
	int			iLineState;
	
	DeviceID_t	VocDevID;
	DJ_U8		u8PlayTag;	

	// -----------------
	VOIP_STATE	State;

	int		DtmfCount;
	char	DtmfBuf[32];

	char CallerNum[20];
	char CalleeNum[20];	

	int  iProtocol;

	char CallerAddr[20];
	char CallerUserID[20];	

	char CalleeAddr[20];
	char CalleeUserID[20];	
	
	unsigned short iCallerPort;
	unsigned short iCalleePort;	

} VOIP_STRUCT;

enum VOICE_STATE 
{
	VOC_WAITOPEN,
	VOC_FREE,
	VOC_USED
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

	//
	DJ_U8		u8E1Type;
	DJ_S32		s32AlarmVal;
} PCM_STRUCT;

// ----- decalure function -----
bool	InitSystem(void);
void	ExitSystem(void); 

void	AddMsg ( char *str);

void	OpenAllDevice_Dsp ( DJ_S8 s8DspModID );
void	CloseAllDevice_Dsp ( DJ_S8 s8DspModID );

void	HandleDevState ( Acs_Evt_t *pAcsEvt );

void	InitVOIPChannel ( VOIP_STRUCT *pOneVOIP );
void	ReDrawAll (void);
void	OpenDeviceOK ( DeviceID_t *pDevice );

void	My_BuildIndex ( DeviceID_t	*pVocDevID );

DJ_S32	 PlayFile ( DeviceID_t	*pVocDevID, DJ_S8 *s8FileName, DJ_U8 u8PlayTag, bool bIsQueue = false );
DJ_S32	 PlayIndex ( DeviceID_t	*pVocDevID, DJ_U16 u16Index, DJ_U8 u8PlayTag, bool bIsQueue );

void	VOIPWork ( VOIP_STRUCT *pOneVOIP, Acs_Evt_t *pAcsEvt );

void   CreateFileDirector(DJ_S8 s8DspModID);

void   WriteMsgLog(char *pMsg);

#endif