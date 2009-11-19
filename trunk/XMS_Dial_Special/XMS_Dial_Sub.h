#ifndef _XMS_DIAL_SUB_H_
#define _XMS_DIAL_SUB_H_

#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"

#define	MAX_FILE_NAME_LEN		256

#define	MAX_SEND_IODATA_DTMF_LEN	16

enum TRUNK_STATE {
	TRK_WAITOPEN,
	TRK_FREE,			
	TRK_NOTHANDLE,
	TRK_CALLOUT,
	TRK_SIM_CALLOUT,
	TRK_SIM_LINK,
	TRK_SIM_ANALOG_OFFHOOK,
	TRK_SIM_ANALOG_DIALING,

	TRK_LINK,
	TRK_PLAYDTMF,
	TRK_HANGUP,
	TRK_FAIL,

	TRK_WAIT_REMOVE,
};

enum USER_STATE {
	USER_WAITOPEN,
	USER_FREE,
	USER_OFFHOOK,
	USER_CALLOUT,
	USER_LINK,
	USER_WAITHANGUP,

	USER_WAIT_REMOVE,

	USER_RING,
};

typedef struct
{
	// ----------------
	DeviceID_t	deviceID;
	int			iSeqID;
	int			iModSeqID;
	int			iLineState;

	DeviceID_t	VocDevID;

	DeviceID_t	LinkDevID;

	// -----------------
	TRUNK_STATE	State;

	// -----------------
	USER_STATE	UserState;
	int			iUserSeqID;

	int		DtmfCount;
	char	DtmfBuf[32];

	char CallerCode[20];
	char CalleeCode[20];

} TRUNK_STRUCT;

enum VOICE_STATE {
	VOC_WAITOPEN,
	VOC_FREE,
	VOC_USED,

	VOC_WAIT_REMOVE,
} ;

typedef struct
{
	DeviceID_t	deviceID;
	int			iSeqID;
	DeviceID_t	UsedDevID;
	VOICE_STATE	State;
} VOICE_STRUCT;

typedef struct
{
	DeviceID_t	deviceID;
	int			iSeqID;
	bool		bOpenFlag;		// flag of OpenDevice OK

	DJ_U8		u8E1Type;
	DJ_S32		s32AlarmVal;
} PCM_STRUCT;

//º¯ÊýÉùÃ÷
bool	InitSystem(void);
void	ExitSystem(void); 
void	AddMsg ( char *str);
void	OpenAllDevice_Dsp ( DJ_S8 s8DspModID );
void	CloseAllDevice_Dsp ( DJ_S8 s8DspModID );
void	ReOpen_AllDevice (void);
void	HandleDevState ( Acs_Evt_t *pAcsEvt );
void	ReDrawAll (void);
void	OpenDeviceOK ( DeviceID_t *pDevice );
void	CloseDeviceOK ( DeviceID_t *pDevice );
void	Remove_OneDsp(void);
void	ChannelWork ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt );
void	Change_State ( TRUNK_STRUCT *pOneTrunk, TRUNK_STATE NewState );
void	InitTrunkChannel ( TRUNK_STRUCT *pOneTrunk );
void	InitUserChannel ( TRUNK_STRUCT *pOneUser );
void	SimulateCallOut(void);
void	Change_UserState ( TRUNK_STRUCT *pOneTrunk, USER_STATE NewState );
void	Change_Voc_State ( VOICE_STRUCT *pOneVoice, VOICE_STATE NewState );
bool	IsTrunk ( DJ_S16 s16DevSub );
bool	IsDigitTrunk ( DJ_S16 s16DevSub );
int		SearchOneFreeVoice (  TRUNK_STRUCT *pOneTrunk, DeviceID_t *pFreeVocDeviceID );

#endif