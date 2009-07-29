typedef int	BOOL;
#define WINAPI __stdcall

#include "stdio.h"
#include "String.h"
#include "DJAcsAPIDef.h"

enum TRUNK_STATE{	//中继通道逻辑状态
	TRK_WAITOPEN,	//中继通道等待打开
	TRK_FREE,		//中继通道空闲
	USER_OFFHOOK,	//坐席摘机
};
enum VOICE_STATE {  //语音设备逻辑状态
	VOC_WAITOPEN,	//语音设备等待打开
	VOC_FREE,		//语音设备空闲
	VOC_USED,		//语音设备使用中
};

typedef struct{				//中继设备结构体
	DeviceID_t	deviceID;	//中继设备ID
	int			iLineState;	//中继设备物理状态
	DeviceID_t	VocDevID;	//关联的语音设备ID	
	TRUNK_STATE	State;		//中继设备逻辑状态
} TRUNK_STRUCT;

typedef struct{				//语音设备结构体
	DeviceID_t	deviceID;	//语音设备ID
	DeviceID_t	UsedDevID;	//使用此语音的设备ID
	VOICE_STATE	State;		//语音设备逻辑状态
}VOICE_STRUCT;

typedef struct 
{ 
	long		lFlag;				// 本DSP是否存在, 0：不存在，1：存在
	DeviceID_t	deviceID;			// 本DSP模块的deviceID 
	bool		bOpenFlag;			// OpenDevice成功的标志
	bool		bErrFlag;			// 发生过CloseDevice事件的标志
	
	long		lVocNum;			// 该DSP上语音设备数量
	long		lVocOpened;			// 该DSP上打开成功的语音设备数量
	long		lVocFreeNum;		// 该DSP上可用的语音设备数量
	VOICE_STRUCT *pVoice;			// 根据需要，动态分配相应的结构
	
	long		lTrunkNum;			// 该DSP上中继设备数量
	long		lTrunkOpened;		// 该DSP上打开成功的中继设备数量
	TRUNK_STRUCT *pTrunk;			// 根据需要，动态分配相应的结构 
	
} TYPE_XMS_DSP_DEVICE_RES_DEMO;		// 定义结构：单个DSP可以提供的设备资源

#define MAX_DSP_MODULE_NUMBER_OF_XMS 256 //Keygoe最多可以有256个DSP单元
//全局变量定义-------------------------------------------
int				cfg_iPartWorkModuleID = 1;	//DSP模块号
RetCode_t		r;							//返回码
ServerID_t		cfg_ServerID;				//Keygoe服务器信息
ACSHandle_t		g_acsHandle = -1;			//设备句柄
TYPE_XMS_DSP_DEVICE_RES_DEMO	AllDeviceRes[MAX_DSP_MODULE_NUMBER_OF_XMS];//保存所有可用设备

//函数声明-----------------------------------------------
void			OpenDeviceOK ( DeviceID_t *pDevice );	//成功打开设备后进行处理的函数
void			UserWork( TRUNK_STRUCT *pOneUser, Acs_Evt_t *pAcsEvt );	//坐席设备的事件处理函数
void			ResetUser ( TRUNK_STRUCT *pOneUser, Acs_Evt_t *pAcsEvt );	//重置坐席设备
void			AddDeviceRes ( Acs_Dev_List_Head_t *pAcsDevList );	//将搜索到的可用设备资源添加到全局数组AllDeviceRes[]
void			OpenDeviceRes ( DJ_S8 s8DspModID );	//打开设备资源
void			ExitSystem();					//退出系统
DJ_Void			EvtHandler(DJ_U32 esrParam);	//回调函数，所有Keygoe时间返回到此函数

//程序入口----------------------------------------
int main(int argc,char **argv)
{
	printf("DONJIN Keygoe HelloKeygoe Sample...Copyright 2009.\n");
	printf("=======================================================\n");
	
	strcpy(cfg_ServerID.m_s8ServerIp,"192.168.11.111");
	cfg_ServerID.m_u32ServerPort = 9000;
	
	r = XMS_acsOpenStream( &g_acsHandle, &cfg_ServerID,2,32,32,	0,NULL);//建立与Keygoe的连接	
	if ( r < 0 )
	{
		printf ( "XMS_acsOpenStream(IP Addr : %s, port : %d) FAIL. ret = %d\n", 
			cfg_ServerID.m_s8ServerIp, cfg_ServerID.m_u32ServerPort, r );
		return false;
	}else
	{
		printf ( "XMS_acsOpenStream(%s,%d) OK!\n", cfg_ServerID.m_s8ServerIp, cfg_ServerID.m_u32ServerPort );
	}
	
	r = XMS_acsSetESR(g_acsHandle, (EsrFunc)EvtHandler, 0, 1 );//设置回调函数
	if ( r < 0 )
	{
		printf ( "XMS_acsSetESR() FAIL! ret = %d\n", r );		
		return false;
	}else
	{
		printf ( "XMS_acsSetESR() OK!\n" );
	}
	r = XMS_acsGetDeviceList ( g_acsHandle, NULL );//获取可用设备列表
	if ( r < 0)
	{
		printf ( "XMS_acsGetDeviceList() FAIL!\n" );
		return false;
	}else
	{
		printf ( "XMS_acsGetDeviceList() OK!\n" );
	}

	printf("坐席电话请摘机!\n");	
    while ( getchar() != 'q' )    //当输入 q 或者 ctrl+c 程序退出
        printf("press q or ctrl+c exit programmer\n");

	ExitSystem();
	printf("退出应用程序\n");
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
	case XMS_EVT_QUERY_DEVICE:			//每次返回此事件，可获取DSP上一种类型的可用设备
		if (s8DspModID == cfg_iPartWorkModuleID)
		{
			pAcsDevList = ( Acs_Dev_List_Head_t *) FetchEventData(pAcsEvt);	
			AddDeviceRes ( pAcsDevList );//添加可用设备到全局数组AllDeviceRes[]
		}	
		break; 
	case XMS_EVT_QUERY_ONE_DSP_END:		//一个DSP上的资源搜索完毕
		if (s8DspModID == cfg_iPartWorkModuleID)
		{
			AllDeviceRes[s8DspModID].lFlag = 1;		//此DSP可用
			AllDeviceRes[s8DspModID].bErrFlag = false;
			OpenDeviceRes ( s8DspModID );			//代开需要用到的设备资源			
		}
		break;
	case XMS_EVT_QUERY_REMOVE_ONE_DSP_END:		
		break;		
	case XMS_EVT_QUERY_DEVICE_END:	// Query Device List End
		break;		
	case XMS_EVT_OPEN_DEVICE:
		OpenDeviceOK(&pAcsEvt->m_DeviceID );	//设备打开成功
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
	if ( pOneUser->VocDevID.m_s16DeviceMain != 0 )//如果有语音设备与此中继连接，则断开连接释放语音资源
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

//坐席设备事件处理------------------------------------------------------------
void UserWork( TRUNK_STRUCT *pOneUser, Acs_Evt_t *pAcsEvt )
{
	RetCode_t				r;
	Acs_GeneralProc_Data	*pGeneral = NULL;
	Acs_CallControl_Data	*pCallControl = NULL;
	VOICE_STRUCT			*pOneVoice = NULL;
	PlayProperty_t			playProperty;	
	
	if( pAcsEvt->m_s32EventType == XMS_EVT_CLEARCALL ){	//拆线处理
		if(pOneUser->State != TRK_FREE)
		{
			ResetUser(pOneUser,pAcsEvt);
			printf("ResetUser()---(User %d,%d)---坐席挂机\n",
				pOneUser->deviceID.m_s8ModuleID,pOneUser->deviceID.m_s16ChannelID);
			return;
		}
	}
	
	switch(pOneUser->State)
	{
	case TRK_FREE:
		if ( pAcsEvt->m_s32EventType == XMS_EVT_CALLIN ) //当前状态TRK_FREE,坐席摘机事件
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
					printf("(VOC %d,%d) play voice: 这里是东进电话银行演示系统\n",
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
			printf("ResetUser()---(User %d,%d)---放音结束\n",
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
	// 关闭中继设备
	for ( i = 0; i < AllDeviceRes[cfg_iPartWorkModuleID].lTrunkNum; i++ )
	{
		XMS_ctsCloseDevice ( g_acsHandle, &AllDeviceRes[cfg_iPartWorkModuleID].pTrunk[i].deviceID, NULL );		
	}	
	// 关闭语音设备
	for ( i = 0; i < AllDeviceRes[cfg_iPartWorkModuleID].lVocNum; i++ )
	{
		XMS_ctsCloseDevice ( g_acsHandle, &AllDeviceRes[cfg_iPartWorkModuleID].pVoice[i].deviceID, NULL );		
	}	
	
	r = XMS_acsCloseStream ( g_acsHandle, NULL );//关闭与Keygoe的连接
	
	memset ( AllDeviceRes, 0, sizeof(AllDeviceRes) );
}

void	AddDeviceRes ( Acs_Dev_List_Head_t *pAcsDevList )
{
	DJ_S8	s8DspModID;
	DJ_S32	s32Num;
	
	s32Num = pAcsDevList->m_s32DeviceNum;	
	s8DspModID = (DJ_S8) pAcsDevList->m_s32ModuleID;

	if ( (s8DspModID < 0) || (s8DspModID >= MAX_DSP_MODULE_NUMBER_OF_XMS) )	return;	// 非法DSP单元ID
	
	switch ( pAcsDevList->m_s32DeviceMain )
	{
	case XMS_DEVMAIN_VOICE:	
		if ( (AllDeviceRes[s8DspModID].lVocNum == 0) && (s32Num > 0) )		//新添加语音设备
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
		if ( (AllDeviceRes[s8DspModID].lTrunkNum == 0) && (s32Num > 0) )		//新添加中继设备
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
//打开设备资源---------------------------------
void	OpenDeviceRes ( DJ_S8 s8DspModID )
{
	VOICE_STRUCT	*pOneVoice;
	TRUNK_STRUCT	*pOneTrunk;
	// 打开语音设备
	for ( int i = 0; i < AllDeviceRes[s8DspModID].lVocNum; i++ )
	{
		pOneVoice = &AllDeviceRes[s8DspModID].pVoice[i];			
		if ( pOneVoice->State == VOC_WAITOPEN )		//语音设备尚未打开
		{
			if ( XMS_ctsOpenDevice ( g_acsHandle, &pOneVoice->deviceID, NULL ) < 0 )
			{
				printf("(VOC %d,%d) open fail!\n",pOneVoice->deviceID.m_s8ModuleID,
					pOneVoice->deviceID.m_s16ChannelID) ;
			}
		}
	}			
	// 打开中继设备
	for ( int j = 0; j < AllDeviceRes[s8DspModID].lTrunkNum; j++ )
	{
		pOneTrunk = &AllDeviceRes[s8DspModID].pTrunk[j];
		if ( pOneTrunk->State == TRK_WAITOPEN )		//中继设备尚未打开
		{
			if ( XMS_ctsOpenDevice ( g_acsHandle, &pOneTrunk->deviceID, NULL ) < 0 )
			{
				printf("(Trunk %d,%d) open fail!\n",pOneTrunk->deviceID.m_s8ModuleID,
					pOneTrunk->deviceID.m_s16ChannelID) ;
			}
		}		
	}
}
