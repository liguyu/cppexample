#ifndef _XMSAPI_TEST_EVENT_H_
#define _XMSAPI_TEST_EVENT_H_

typedef struct 
{
	DeviceID_t	DevID;
	bool		bOpenFlag;
} TYPE_MY_DEVICE_STRUCT;

#define	MAX_DEVICE_ALLOW_OPEN		32

DJ_Void EvtHandler(DJ_U32 esrParam);

void	OpenDeviceOK ( DeviceID_t	*pDev );
void	CloseDeviceOK ( DeviceID_t	*pDev );

#endif