typedef int	BOOL;
#define WINAPI __stdcall

#include "stdio.h"
#include "String.h"
#include "DJAcsAPIDef.h"

enum TRUNK_STATE{	//�м�ͨ���߼�״̬
	TRK_WAITOPEN,	//�м�ͨ���ȴ���
	TRK_FREE,		//�м�ͨ������
	USER_OFFHOOK,	//��ϯժ��
};
enum VOICE_STATE {  //�����豸�߼�״̬
	VOC_WAITOPEN,	//�����豸�ȴ���
	VOC_FREE,		//�����豸����
	VOC_USED,		//�����豸ʹ����
};

typedef struct{				//�м��豸�ṹ��
	DeviceID_t	deviceID;	//�м��豸ID
	int			iLineState;	//�м��豸����״̬
	DeviceID_t	VocDevID;	//�����������豸ID	
	TRUNK_STATE	State;		//�м��豸�߼�״̬
} TRUNK_STRUCT;

typedef struct{				//�����豸�ṹ��
	DeviceID_t	deviceID;	//�����豸ID
	DeviceID_t	UsedDevID;	//ʹ�ô��������豸ID
	VOICE_STATE	State;		//�����豸�߼�״̬
}VOICE_STRUCT;

typedef struct 
{ 
	long		lFlag;				// ��DSP�Ƿ����, 0�������ڣ�1������
	DeviceID_t	deviceID;			// ��DSPģ���deviceID 
	bool		bOpenFlag;			// OpenDevice�ɹ��ı�־
	bool		bErrFlag;			// ������CloseDevice�¼��ı�־
	
	long		lVocNum;			// ��DSP�������豸����
	long		lVocOpened;			// ��DSP�ϴ򿪳ɹ��������豸����
	long		lVocFreeNum;		// ��DSP�Ͽ��õ������豸����
	VOICE_STRUCT *pVoice;			// ������Ҫ����̬������Ӧ�Ľṹ
	
	long		lTrunkNum;			// ��DSP���м��豸����
	long		lTrunkOpened;		// ��DSP�ϴ򿪳ɹ����м��豸����
	TRUNK_STRUCT *pTrunk;			// ������Ҫ����̬������Ӧ�Ľṹ 
	
} TYPE_XMS_DSP_DEVICE_RES_DEMO;		// ����ṹ������DSP�����ṩ���豸��Դ

#define MAX_DSP_MODULE_NUMBER_OF_XMS 256 //Keygoe��������256��DSP��Ԫ
//ȫ�ֱ�������-------------------------------------------
int				cfg_iPartWorkModuleID = 1;	//DSPģ���
RetCode_t		r;							//������
ServerID_t		cfg_ServerID;				//Keygoe��������Ϣ
ACSHandle_t		g_acsHandle = -1;			//�豸���
TYPE_XMS_DSP_DEVICE_RES_DEMO	AllDeviceRes[MAX_DSP_MODULE_NUMBER_OF_XMS];//�������п����豸

//��������-----------------------------------------------
void			OpenDeviceOK ( DeviceID_t *pDevice );	//�ɹ����豸����д���ĺ���
void			UserWork( TRUNK_STRUCT *pOneUser, Acs_Evt_t *pAcsEvt );	//��ϯ�豸���¼�������
void			ResetUser ( TRUNK_STRUCT *pOneUser, Acs_Evt_t *pAcsEvt );	//������ϯ�豸
void			AddDeviceRes ( Acs_Dev_List_Head_t *pAcsDevList );	//���������Ŀ����豸��Դ��ӵ�ȫ������AllDeviceRes[]
void			OpenDeviceRes ( DJ_S8 s8DspModID );	//���豸��Դ
void			ExitSystem();					//�˳�ϵͳ
DJ_Void			EvtHandler(DJ_U32 esrParam);	//�ص�����������Keygoeʱ�䷵�ص��˺���

//�������----------------------------------------
int main(int argc,char **argv)
{
	printf("DONJIN Keygoe HelloKeygoe Sample...Copyright 2009.\n");
	printf("=======================================================\n");
	
	strcpy(cfg_ServerID.m_s8ServerIp,"192.168.11.111");
	cfg_ServerID.m_u32ServerPort = 9000;
	
	r = XMS_acsOpenStream( &g_acsHandle, &cfg_ServerID,2,32,32,	0,NULL);//������Keygoe������	
	if ( r < 0 )
	{
		printf ( "XMS_acsOpenStream(IP Addr : %s, port : %d) FAIL. ret = %d\n", 
			cfg_ServerID.m_s8ServerIp, cfg_ServerID.m_u32ServerPort, r );
		return false;
	}else
	{
		printf ( "XMS_acsOpenStream(%s,%d) OK!\n", cfg_ServerID.m_s8ServerIp, cfg_ServerID.m_u32ServerPort );
	}
	
	r = XMS_acsSetESR(g_acsHandle, (EsrFunc)EvtHandler, 0, 1 );//���ûص�����
	if ( r < 0 )
	{
		printf ( "XMS_acsSetESR() FAIL! ret = %d\n", r );		
		return false;
	}else
	{
		printf ( "XMS_acsSetESR() OK!\n" );
	}
	r = XMS_acsGetDeviceList ( g_acsHandle, NULL );//��ȡ�����豸�б�
	if ( r < 0)
	{
		printf ( "XMS_acsGetDeviceList() FAIL!\n" );
		return false;
	}else
	{
		printf ( "XMS_acsGetDeviceList() OK!\n" );
	}

	printf("��ϯ�绰��ժ��!\n");	
    while ( getchar() != 'q' )    //������ q ���� ctrl+c �����˳�
        printf("press q or ctrl+c exit programmer\n");

	ExitSystem();
	printf("�˳�Ӧ�ó���\n");
	return 0;
}

DJ_Void EvtHandler(DJ_U32 esrParam)
{
	DJ_S8					s8DspModID;
	Acs_Evt_t				*pAcsEvt = NULL;
	Acs_Dev_List_Head_t		*pAcsDevList = NULL;
	Acs_GeneralProc_Data	*pGeneralData = NULL;	
	VOICE_STRUCT			*pOneVoice;
	TRUNK_STRUCT			*pOneTrunk;

	pAcsEvt = (Acs_Evt_t *) esrParam;
	s8DspModID = pAcsEvt->m_DeviceID.m_s8ModuleID;

	switch ( pAcsEvt->m_s32EventType )
	{
	case XMS_EVT_QUERY_DEVICE:			//ÿ�η��ش��¼����ɻ�ȡDSP��һ�����͵Ŀ����豸
		if (s8DspModID == cfg_iPartWorkModuleID)
		{
			pAcsDevList = ( Acs_Dev_List_Head_t *) FetchEventData(pAcsEvt);	
			AddDeviceRes ( pAcsDevList );//��ӿ����豸��ȫ������AllDeviceRes[]
		}	
		break; 
	case XMS_EVT_QUERY_ONE_DSP_END:		//һ��DSP�ϵ���Դ�������
		if (s8DspModID == cfg_iPartWorkModuleID)
		{
			AllDeviceRes[s8DspModID].lFlag = 1;		//��DSP����
			AllDeviceRes[s8DspModID].bErrFlag = false;
			OpenDeviceRes ( s8DspModID );			//������Ҫ�õ����豸��Դ			
		}
		break;
	case XMS_EVT_QUERY_REMOVE_ONE_DSP_END:		
		break;		
	case XMS_EVT_QUERY_DEVICE_END:	// Query Device List End
		break;		
	case XMS_EVT_OPEN_DEVICE:
		OpenDeviceOK(&pAcsEvt->m_DeviceID );	//�豸�򿪳ɹ�
		break;		
	case XMS_EVT_CLOSE_DEVICE:
		break;		
	case XMS_EVT_DEVICESTATE:
		pGeneralData = (Acs_GeneralProc_Data *)FetchEventData(pAcsEvt);		
		if ( pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH )
		{
			pOneTrunk = &AllDeviceRes[pAcsEvt->m_DeviceID.m_s8ModuleID].pTrunk[pAcsEvt->m_DeviceID.m_s16ChannelID];
			pOneTrunk->iLineState = pGeneralData->m_s32DeviceState;
		}
		break;		
	case XMS_EVT_UNIFAILURE:
		break;		
	default:
		if ( pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH &&
			pAcsEvt->m_DeviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_USER)
		{
			pOneTrunk = &AllDeviceRes[pAcsEvt->m_DeviceID.m_s8ModuleID].pTrunk[pAcsEvt->m_DeviceID.m_s16ChannelID];
			UserWork( pOneTrunk, pAcsEvt );
		}else if (pAcsEvt->m_DeviceID.m_s16DeviceMain == XMS_DEVMAIN_VOICE)
		{	
			DeviceID_t	*pDevID;
			pOneVoice = &AllDeviceRes[pAcsEvt->m_DeviceID.m_s8ModuleID].pVoice[pAcsEvt->m_DeviceID.m_s16ChannelID];
			pDevID = &pOneVoice->UsedDevID;
			
			if ( pDevID->m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH && pDevID->m_s16DeviceSub == XMS_DEVSUB_ANALOG_USER)
			{
				pOneTrunk = &AllDeviceRes[pDevID->m_s8ModuleID].pTrunk[pDevID->m_s16ChannelID];
				UserWork ( pOneTrunk, pAcsEvt );
			}			
		}
		break;
	}
}
void ResetUser ( TRUNK_STRUCT *pOneUser, Acs_Evt_t *pAcsEvt )
{
	VOICE_STRUCT * pOneVoice;
	if ( pOneUser->VocDevID.m_s16DeviceMain != 0 )//����������豸����м����ӣ���Ͽ������ͷ�������Դ
	{
		XMS_ctsUnlinkDevice ( g_acsHandle, &pOneUser->VocDevID, &pOneUser->deviceID, NULL ); 
		XMS_ctsUnlinkDevice ( g_acsHandle, &pOneUser->deviceID, &pOneUser->VocDevID, NULL ); 
		pOneVoice = &AllDeviceRes[pOneUser->VocDevID.m_s8ModuleID].pVoice[pOneUser->VocDevID.m_s16ChannelID];
		pOneVoice->State = VOC_FREE;
		AllDeviceRes[pOneVoice->deviceID.m_s8ModuleID].lVocFreeNum++;
	}

	pOneUser->State = TRK_FREE;
	memset( &pOneUser->VocDevID, 0, sizeof(DeviceID_t) );
}

//��ϯ�豸�¼�����------------------------------------------------------------
void UserWork( TRUNK_STRUCT *pOneUser, Acs_Evt_t *pAcsEvt )
{
	RetCode_t				r;
	Acs_GeneralProc_Data	*pGeneral = NULL;
	Acs_CallControl_Data	*pCallControl = NULL;
	VOICE_STRUCT			*pOneVoice = NULL;
	PlayProperty_t			playProperty;	
	
	if( pAcsEvt->m_s32EventType == XMS_EVT_CLEARCALL ){	//���ߴ���
		if(pOneUser->State != TRK_FREE)
		{
			ResetUser(pOneUser,pAcsEvt);
			printf("ResetUser()---(User %d,%d)---��ϯ�һ�\n",
				pOneUser->deviceID.m_s8ModuleID,pOneUser->deviceID.m_s16ChannelID);
			return;
		}
	}
	
	switch(pOneUser->State)
	{
	case TRK_FREE:
		if ( pAcsEvt->m_s32EventType == XMS_EVT_CALLIN ) //��ǰ״̬TRK_FREE,��ϯժ���¼�
		{
			printf("(User %d,%d) OFFHOOK\n",pOneUser->deviceID.m_s8ModuleID,pOneUser->deviceID.m_s16ChannelID);
			pOneVoice = &AllDeviceRes[pOneUser->deviceID.m_s8ModuleID].pVoice[pOneUser->deviceID.m_s16ChannelID];
			if ( pOneVoice->State == VOC_FREE ){
				pOneVoice->State = VOC_USED;
				AllDeviceRes[pOneUser->deviceID.m_s8ModuleID].lVocFreeNum--;
				
				pOneUser->VocDevID = pOneVoice->deviceID;
				pOneVoice->UsedDevID = pOneUser->deviceID;
				
				XMS_ctsLinkDevice ( g_acsHandle, &pOneUser->deviceID, &pOneVoice->deviceID, NULL ); 
				XMS_ctsLinkDevice ( g_acsHandle, &pOneVoice->deviceID, &pOneUser->deviceID, NULL ); 
				
				memset(&playProperty,0,sizeof(playProperty));
				playProperty.m_u16PlayType = XMS_PLAY_TYPE_FILE;	
				strcpy ( playProperty.m_s8PlayContent, "C:\\DJKeygoe\\Samples\\Voc\\Bank.001");
				r = XMS_ctsPlay ( g_acsHandle, &pOneVoice->deviceID, &playProperty, NULL );
				if (r < 0)
				{
					printf("XMS_ctsPlay fail!\n") ;
				}else{
					printf("(VOC %d,%d) play voice: �����Ƕ����绰������ʾϵͳ\n",
						pOneVoice->deviceID.m_s8ModuleID,pOneVoice->deviceID.m_s16ChannelID);
					pOneUser->State = USER_OFFHOOK;					
				}				
			}else{
				printf("No free voice device!\n");
			}			
		}
		break;
		
	case USER_OFFHOOK:
		if ( pAcsEvt->m_s32EventType == XMS_EVT_PLAY ){
			ResetUser(pOneUser,pAcsEvt);
			printf("ResetUser()---(User %d,%d)---��������\n",
				pOneUser->deviceID.m_s8ModuleID,pOneUser->deviceID.m_s16ChannelID);
		}	
		break;		
	}
}

void	OpenDeviceOK ( DeviceID_t *pDevice )
{
	TRUNK_STRUCT *	pOneTrunk;
	VOICE_STRUCT *	pOneVoice;	
	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH )
	{
		pOneTrunk = &AllDeviceRes[(*pDevice).m_s8ModuleID].pTrunk[(*pDevice).m_s16ChannelID];
		
		pOneTrunk->deviceID.m_CallID = pDevice->m_CallID;		// this line is very important, must before all operation
		
		// init this Device: Trunk
		pOneTrunk->State = TRK_FREE;

		memset ( &pOneTrunk->VocDevID, 0, sizeof(DeviceID_t) );		// 0: didn't alloc Voc Device
		
		XMS_ctsResetDevice ( g_acsHandle, pDevice, NULL );
		XMS_ctsGetDevState ( g_acsHandle, pDevice, NULL );
				
		AllDeviceRes[pDevice->m_s8ModuleID].lTrunkOpened ++;		
	}
	
	if ( pDevice->m_s16DeviceMain == XMS_DEVMAIN_VOICE )
	{
		pOneVoice = &AllDeviceRes[(*pDevice).m_s8ModuleID].pVoice[(*pDevice).m_s16ChannelID];
		pOneVoice->deviceID.m_CallID = pDevice->m_CallID;		// this line is very important, must before all operation
		
		// init this Device: Voice
		pOneVoice->State = VOC_FREE;
		
		XMS_ctsResetDevice ( g_acsHandle, pDevice, NULL );
		XMS_ctsGetDevState ( g_acsHandle, pDevice, NULL );	
		
		AllDeviceRes[pDevice->m_s8ModuleID].lVocOpened ++;
		AllDeviceRes[pDevice->m_s8ModuleID].lVocFreeNum ++;	
	}
	
}

void	ExitSystem() 
{
	int			i;
	RetCode_t	r;
	// �ر��м��豸
	for ( i = 0; i < AllDeviceRes[cfg_iPartWorkModuleID].lTrunkNum; i++ )
	{
		XMS_ctsCloseDevice ( g_acsHandle, &AllDeviceRes[cfg_iPartWorkModuleID].pTrunk[i].deviceID, NULL );		
	}	
	// �ر������豸
	for ( i = 0; i < AllDeviceRes[cfg_iPartWorkModuleID].lVocNum; i++ )
	{
		XMS_ctsCloseDevice ( g_acsHandle, &AllDeviceRes[cfg_iPartWorkModuleID].pVoice[i].deviceID, NULL );		
	}	
	
	r = XMS_acsCloseStream ( g_acsHandle, NULL );//�ر���Keygoe������
	
	memset ( AllDeviceRes, 0, sizeof(AllDeviceRes) );
}

void	AddDeviceRes ( Acs_Dev_List_Head_t *pAcsDevList )
{
	DJ_S8	s8DspModID;
	DJ_S32	s32Num;
	
	s32Num = pAcsDevList->m_s32DeviceNum;	
	s8DspModID = (DJ_S8) pAcsDevList->m_s32ModuleID;

	if ( (s8DspModID < 0) || (s8DspModID >= MAX_DSP_MODULE_NUMBER_OF_XMS) )	return;	// �Ƿ�DSP��ԪID
	
	switch ( pAcsDevList->m_s32DeviceMain )
	{
	case XMS_DEVMAIN_VOICE:	
		if ( (AllDeviceRes[s8DspModID].lVocNum == 0) && (s32Num > 0) )		//����������豸
		{
			AllDeviceRes[s8DspModID].pVoice = new VOICE_STRUCT[s32Num];
			if( AllDeviceRes[s8DspModID].pVoice )
			{
				AllDeviceRes[s8DspModID].lVocNum = s32Num;
				AllDeviceRes[s8DspModID].lVocOpened = 0;
				AllDeviceRes[s8DspModID].lVocFreeNum = 0;
				memset ( AllDeviceRes[s8DspModID].pVoice, 0, sizeof(VOICE_STRUCT)*s32Num );
				
				DeviceID_t	 *pDev;
				pDev = (DeviceID_t *)((unsigned char *)pAcsDevList + sizeof(Acs_Dev_List_Head_t));
				
				for ( int i = 0; i < s32Num; i ++ )
				{
					AllDeviceRes[s8DspModID].pVoice[i].deviceID = pDev[i];
					AllDeviceRes[s8DspModID].pVoice[i].State = VOC_WAITOPEN;
				}
			}
		}
		break;			
	case XMS_DEVMAIN_INTERFACE_CH:
		if ( (AllDeviceRes[s8DspModID].lTrunkNum == 0) && (s32Num > 0) )		//������м��豸
		{
			AllDeviceRes[s8DspModID].pTrunk = new TRUNK_STRUCT[s32Num];
			if( AllDeviceRes[s8DspModID].pTrunk )
			{
				AllDeviceRes[s8DspModID].lTrunkNum = s32Num;
				AllDeviceRes[s8DspModID].lTrunkOpened = 0;
				memset ( AllDeviceRes[s8DspModID].pTrunk, 0, sizeof(TRUNK_STRUCT)*s32Num );
				
				DeviceID_t	 *pDev;				
				pDev = (DeviceID_t *)((unsigned char *)pAcsDevList + sizeof(Acs_Dev_List_Head_t));
				
				for (int i = 0; i < s32Num; i ++ )
				{
					AllDeviceRes[s8DspModID].pTrunk[i].deviceID = pDev[i];
					AllDeviceRes[s8DspModID].pTrunk[i].State = TRK_WAITOPEN;
				}
			}
		}	
		break;
	}
}
//���豸��Դ---------------------------------
void	OpenDeviceRes ( DJ_S8 s8DspModID )
{
	VOICE_STRUCT	*pOneVoice;
	TRUNK_STRUCT	*pOneTrunk;
	// �������豸
	for ( int i = 0; i < AllDeviceRes[s8DspModID].lVocNum; i++ )
	{
		pOneVoice = &AllDeviceRes[s8DspModID].pVoice[i];			
		if ( pOneVoice->State == VOC_WAITOPEN )		//�����豸��δ��
		{
			if ( XMS_ctsOpenDevice ( g_acsHandle, &pOneVoice->deviceID, NULL ) < 0 )
			{
				printf("(VOC %d,%d) open fail!\n",pOneVoice->deviceID.m_s8ModuleID,
					pOneVoice->deviceID.m_s16ChannelID) ;
			}
		}
	}			
	// ���м��豸
	for ( int j = 0; j < AllDeviceRes[s8DspModID].lTrunkNum; j++ )
	{
		pOneTrunk = &AllDeviceRes[s8DspModID].pTrunk[j];
		if ( pOneTrunk->State == TRK_WAITOPEN )		//�м��豸��δ��
		{
			if ( XMS_ctsOpenDevice ( g_acsHandle, &pOneTrunk->deviceID, NULL ) < 0 )
			{
				printf("(Trunk %d,%d) open fail!\n",pOneTrunk->deviceID.m_s8ModuleID,
					pOneTrunk->deviceID.m_s16ChannelID) ;
			}
		}		
	}
}
