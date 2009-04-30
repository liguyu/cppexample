#ifndef _XMS_DIAL_SUB_H_
#define _XMS_DIAL_SUB_H_

#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"

#define	MAX_FILE_NAME_LEN		256

enum VOIP_STATE {
	VOIP_WAITOPEN,
	VOIP_FREE,
	VOIP_CALLOUT,  //带语音检测
	VOIP_WAIT_SETOK,
	VOIP_CALLOUT1, //纯信令接续
	VOIP_PLAY,
	VOIP_GETDTMF,
	VOIP_FAIL,
	VOIP_WAITHANGUP,
};

typedef struct
{
	// ----------------
	DeviceID_t	deviceID;
	int			iSeqID;
	int			iModSeqID;
	int			iLineState;

	DeviceID_t	VocDevID;
	DeviceID_t  VocDevIDForCheck;

	DJ_U8		u8PlayTag;	

	// -----------------
	VOIP_STATE	State;


	int		DtmfCount;
	char	DtmfBuf[32];

	char    CallerCode[20];
	char    CalleeCode[20];

	int     iTotalCnt;
	int     iSucCnt;
	int     iCallErrCnt;
	int     iCheckErrCnt;
	int     iEstablishTime;
	int     iCallOutTime;
} VOIP_STRUCT;

enum VOICE_STATE {
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

void	ReDrawAll (void);
void	OpenDeviceOK ( DeviceID_t *pDevice );

void	VOIPWork ( VOIP_STRUCT *pOneVOIP, Acs_Evt_t *pAcsEvt );

void	Change_State ( VOIP_STRUCT *pOneVOIP, VOIP_STATE NewState );

void	InitVOIPChannel ( VOIP_STRUCT *pOneVOIP );

void    InitVOIPcounter(VOIP_STRUCT *pOneVOIP);

void	SimulateCallOut(void);

void    StopCallOut(void);

void   WriteMsgLog(char *pMsg);

void   CreateFileDirector(DJ_S8 s8DspMod);


#endif