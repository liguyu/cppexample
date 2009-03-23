#ifndef _XMSAPI_TEST_SUB_H_
#define _XMSAPI_TEST_SUB_H_

//#define	DEBUG_IN_HOME_NOTEBOOK

#define	MSG_TYPE_ALL			0
#define	MSG_TYPE_FUNCTION		1
#define	MSG_TYPE_EVENT			2
#define	MSG_TYPE_OTHER			3

#define	MAX_FILE_NAME_LEN		256

#define	MAX_PARAMETER_TYPE		13
//16


// ----------------------------------------------------------
void	AddMsg ( int	iMsgType, char *str);
bool	InitSystem(void);
void	ExitSystem(void);

void	InitDeviceInfo (void);
void	SelectOpenDevice(void);
void	SelectOpenedDevice(void);

void	ExchangeDeviceIndex(void);
void	SelectComboDevice(void);
void	SelectComboMsg(void);

void	SelectSendFaxFile (void);
void	SelectReceiveFaxFile (void);

void	SelectPlayFile (void);
void	SelectRecordFile (int nEdit);
void	SelectBuildIndexFile (void);

void	My_SelectFile ( char *FullFileName );

#endif