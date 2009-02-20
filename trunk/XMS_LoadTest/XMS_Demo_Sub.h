#ifndef _XMS_DEMO_SUB_H_
#define _XMS_DEMO_SUB_H_

#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"

#define	MAX_FILE_NAME_LEN		256

enum TRUNK_STATE {
	TRK_WAITOPEN,
	TRK_FREE,
	TRK_NOTHANDLE,
	TRK_RECORDFILE,
	TRK_PLAYRECORD,	
	TRK_WAITHANGUP,

	// new add for XMS
	TRK_WAIT_ANSWERCALL,
	TRK_WAIT_LINKOK,
};


typedef struct
{
	// ----------------
	DeviceID_t	deviceID;
	int			iSeqID;
	int			iModSeqID;
	int			iLineState;
	
	DeviceID_t	VocDevID;
	DJ_U8		u8PlayTag;	

	// -----------------
	TRUNK_STATE	State;

	int		DtmfCount;
	char	DtmfBuf[32];

	char CallerCode[20];
	char CalleeCode[20];

} TRUNK_STRUCT;

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

void	InitTrunkChannel ( TRUNK_STRUCT *pOneTrunk );
void	ReDrawAll (void);
void	OpenDeviceOK ( DeviceID_t *pDevice );

void	My_BuildIndex ( DeviceID_t	*pVocDevID );

DJ_S32	 PlayFile ( DeviceID_t	*pVocDevID, DJ_S8 *s8FileName, DJ_U8 u8PlayTag, bool bIsQueue = false );
DJ_S32	 PlayIndex ( DeviceID_t	*pVocDevID, DJ_U16 u16Index, DJ_U8 u8PlayTag, bool bIsQueue );

void	TrunkWork ( TRUNK_STRUCT *pOneTrunk, Acs_Evt_t *pAcsEvt );

void   CreateFileDirector(DJ_S8 s8DspModID);

void   WriteMsgLog(char *pMsg);

#endif