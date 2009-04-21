#ifndef _XMS_SS7MONITOR_SUB_H_
#define _XMS_SS7MONITOR_SUB_H_

#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"

#define	MAX_FILE_NAME_LEN		256

enum TRUNK_STATE {
	TRK_WAITOPEN,
	TRK_FREE,	
	TRK_WAIT_REMOVE,
};

typedef struct
{
	DeviceID_t	deviceID;
	int			iSeqID;
	int			iModSeqID;
	int			iLineState;

	DeviceID_t	VocDevID;
	DJ_U8		u8PlayTag;
	TRUNK_STATE	State;
	char		CallerCode[20];
	char		CalleeCode[20];

} TRUNK_STRUCT;

typedef struct
{
	DeviceID_t	deviceID;
	int			iSeqID;
	bool		bOpenFlag;		// flag of OpenDevice OK
	DJ_U8		u8E1Type;
	DJ_S32		s32AlarmVal;
} PCM_STRUCT;

// ----- Declare Function -----
bool	InitSystem(void);
void	ExitSystem(void); 
void	AddMsg ( char *str);
void	OpenAllDevice_Dsp ( DJ_S8 s8DspModID );
void	CloseAllDevice_Dsp ( DJ_S8 s8DspModID );
void	ReOpen_AllDevice (void);
void	HandleDevState ( Acs_Evt_t *pAcsEvt );
void	InitTrunkChannel ( TRUNK_STRUCT *pOneTrunk );
void	ReDrawAll (void);
void	OpenDeviceOK ( DeviceID_t *pDevice );
void	CloseDeviceOK ( DeviceID_t *pDevice );
void	Remove_OneDsp(void);
void	SendDataToTrunk(void);
void	TrunkWork ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt );
void	Change_State ( TRUNK_STRUCT *pOneTrunk, TRUNK_STATE NewState );

#endif