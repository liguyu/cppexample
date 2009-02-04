#include "stdafx.h"
#include "publicdef.h"
#include "CAS_Common_Code\CAS_Common_Cfg.h"		
#include "DBProcess.h"
#include "DJAcsDevState.h"


/*外部全局变量定义******************************************************/
CKeySwitchDlg			*pMainDlg;		//主对话框
extern	CKeySwitchApp	theApp;			//应用程序对象

/*全局变量定义**********************************************************/
ACSHandle_t		pFlowHandle;							/*全局流程句柄*/
ServerID_t		pServerID;								/*流程服务器所在的IP*/
DJ_U8			pAppID;									/*应用程序ID*/
DJ_S32			mDebugOn;								/*是否启用调式模式，用于控制XMS底层*/
TYPE_VOICE_DATA	*pVoiceData[];							/*全局设备结构数组指针*/
CmdParamData_CAS_t g_Param_CAS;							/*中国一号信令配置参数*/	
char			pDbConnStr[150];						/*数据库连接字符串*/
long			mPubDeviceNo;							/*全局设备当前编号*/
long			mPubDspNo;								/*全局DSP当前编号*/
long			mDspCount;								/*有效的DSP数量从0开始计数*/
long			mOpenDspCount;							/*已经打开的DSP数量*/
long			mVoiceCount;							/*语音设备总数从0开始计数*/
long			mOpenVoiceCount;						/*已经打开的语音设备总数*/
long			mTrunkCount;							/*接口设备总数从0开始计数*/
long			mOpenTrunkCount;						/*已经打开的接口设备总数*/
long			mVoipCount;								/*voip设备总数从0开始计数*/
long			mOpenVoipCount;							/*已经打开的voip设备总数*/
long			pRowNum;								/*listctrl当前的行号*/	
long			mPubVoiceNo;							/*voice设备的全局编号*/		
long			mPubVoipNo;								/*voip设备的全局编号*/		
long			mPubTrunkNo;							/*trunk设备的全局编号*/		
long			mPlayTaskID;							/*放音全局任务编号*/
char			mFirstPhone[20];						/*汇线通首选号码*/
char			mTurnCode[20];							/*汇线通转接代码*/
char			mPrevCode[20];							/*汇线通前导号码*/


/*VOIP模块的全局变量定义************************************************/
char			pVoipServerIp[20];						/*对方voip服务器所在服务器ip*/
UINT			pVoipServerPort;						/*对方服务器端口*/
int				pVoipProtocol;							/*协议类型*/
char			pVoipUserName[20];						/*主叫用户名*/
char			pLocalVoipServerIP[20];					/*本地VOIP服务器地址*/
UINT			pLocalVoipServerPort;					/*本地VOIP服务器断口*/
int				pLocalVoipProtocol;							/*本地VOIP协议*/
char			pLocalVoipUserName[20];					/*本地VOIP用户名*/		
DJ_S32			pVoipDspNo;								/*VOIP所使用的DSP*/
char			pQueryPhoneNumber[10];					/*查号所指定的分机号码*/
bool			fIsRun;									/*是否已经运行*/


/*设备全局数组**********************************************************/
TYPE_DSP_DATA			aryDspData[MAX_DSP_COUNT];				/*DSP设备全局数组*/
TYPE_INTERFACE_INDEX	aryVoiceIndex[MAX_VOICE_COUNT];		/*全局语音设备索引数组*/
TYPE_INTERFACE_INDEX	aryTrunkIndex[MAX_INTERFACE_COUNT];		/*全局通道设备索引数组*/
TYPE_INTERFACE_INDEX	aryVoipIndex[MAX_VOIP_COUNT];			/*全局VOIP设备索引数组*/
bool			mUpdateDb;										/*初始化时，是否更新数据库*/

/***********************************************************************
功能：从INI文件中获取全局变量，读取的变量包括：
pServerID结构体，具体包括：服务器ip,port,username,password
***********************************************************************/
bool	getConfigFromIni()
{
	char	pFileName[200];						/*路径深度*/
	int		pCreateDb;
	GetCurrentDirectory(200,pFileName);
	strcat(pFileName,SYSTEM_INI_FILE);
	GetPrivateProfileString("Config","ServerIP","",pServerID.m_s8ServerIp,sizeof(pServerID.m_s8ServerIp),pFileName);
	pServerID.m_u32ServerPort=GetPrivateProfileInt("Config","Port",9001,pFileName);
	GetPrivateProfileString("Config","UserName","",pServerID.m_s8UserName,sizeof(pServerID.m_s8UserName),pFileName);
	GetPrivateProfileString("Config","Password","",pServerID.m_s8UserPwd,sizeof(pServerID.m_s8UserPwd),pFileName);
	GetPrivateProfileString("DB Config","ConnStr","",pDbConnStr,200,pFileName);
	pCreateDb=GetPrivateProfileInt("SYS","CreateDb",0,pFileName);
	if(pCreateDb==1)
		mUpdateDb=true;
	else
		mUpdateDb=false;
	/*对方VOIP服务器信息初始化*/
	GetPrivateProfileString("VOIP","ServerIP","",pVoipServerIp,200,pFileName);
	pVoipProtocol=GetPrivateProfileInt("VOIP","ProtocolType",0,pFileName);	
	pVoipServerPort=GetPrivateProfileInt("VOIP","ServerPort",0,pFileName);	
	GetPrivateProfileString("VOIP","UserName","",pVoipUserName,200,pFileName);
	/*读取本地的VOIP服务器信息*/
	GetPrivateProfileString("VOIP","LocalServerIP","",pLocalVoipServerIP,200,pFileName);
	pLocalVoipProtocol=GetPrivateProfileInt("VOIP","LocalProtocolType",0,pFileName);	
	pLocalVoipServerPort=GetPrivateProfileInt("VOIP","LocalServerPort",0,pFileName);	
	GetPrivateProfileString("VOIP","UserName","",pLocalVoipUserName,200,pFileName);
	GetPrivateProfileString("TELEPHONE","QUERY_NO","",pQueryPhoneNumber,200,pFileName);
	/*读取汇线通转接信息*/
	GetPrivateProfileString("HUIXIANTONG","FirstPhone","",mFirstPhone,200,pFileName);
	GetPrivateProfileString("HUIXIANTONG","TurnCode","",mTurnCode,200,pFileName);
	GetPrivateProfileString("HUIXIANTONG","PrevCode","",mPrevCode,200,pFileName);
	return true;
}

/***********************************************************************
功能：调用板卡初始化函数
1.读取INI文件配置，为pServerID初始化
2.调用XMS_acsOpenStream建立与XMS服务器的连接。注意：此处并不进行事件处理
3.设置事件回调函数
4.设置定时器，用于检测各个通道的DTMF
5.启动获取设备列表函数，触发相应事件
6.设置mPubDeviceNo=-1，全局当前设备编码为-1
7.设置mPubDspNo=-1,设置DSP全局编号为-1
8.设置mPubValidDspCount=-1,设置全局有效的DSP数量为-1.
***********************************************************************/
bool InitSystem()
{
	RetCode_t		rtCode;	
	char			MsgStr[160];
	int				i;
	pMainDlg=(CKeySwitchDlg *)theApp.m_pMainWnd;
	fIsRun=false;		/*全局标记，标记程序是否已经运行*/
	getConfigFromIni();	
	/*对中国一号信令进行参数初始化*/
	if ( CAS_Common_Cfg_ReadCfg ( &g_Param_CAS ) != 0 )
	{
		sprintf ( MsgStr, "中国一号信令初始化失败!请检查XMS_CAS_Cfg.INI文件是否存在于CAS_Common_Code目录!" );
		MessageBox(NULL, MsgStr, "初始化", MB_OK ) ;
	}
	/*连接XMS中的流程模块服务器*/ 
	pAppID=APPLICATION_ID;
	mDebugOn=0;		/*采用调式模式初始化底层*/
	rtCode=XMS_acsOpenStream(&pFlowHandle,&pServerID,pAppID,32,32,mDebugOn,NULL);	
	if(rtCode==-ACSERR_BADPARAMETER)
	{
		sprintf ( MsgStr, "初始化时参数非法!" );
		MessageBox(NULL, MsgStr, "初始化", MB_OK ) ;
		return false;
	}
	if(rtCode==-ACSERR_NOSERVER)
	{
		sprintf ( MsgStr, "XMS服务器没有启动或者XMS服务器IP或者Port参数错误!" );
		MessageBox(NULL, MsgStr, "初始化", MB_OK ) ;
		return false;
	}
	if(rtCode==-ACSERR_MAXCONN_EXCEED)
	{
		sprintf ( MsgStr, "与本XMS服务器的应用程序连接数目超过128!" );
		MessageBox(NULL, MsgStr, "初始化", MB_OK ) ;
		return false;
	}
	/*设置事件回调函数*/
	rtCode=XMS_acsSetESR(pFlowHandle,(EsrFunc)XMSEventHandler,0,1);
	if(rtCode!=ACSPOSITIVE_ACK)
	{
		sprintf ( MsgStr, "设置事件回调函数失败!" );
		MessageBox(NULL, MsgStr, "初始化", MB_OK ) ;
		return false;
	}


	/*获取设备列表*/
	if(!getXMSDeviceList())	
	{
		CloseXMSConnect();
		return false;
	}

	/*初始化DSP结构数组*/
	for(i=0;i<MAX_DSP_COUNT;i++)
	{
		aryDspData[i].fDspNo=-1;
		aryDspData[i].fDspID=-1;
		aryDspData[i].fDeviceType=DEVICE_DSP;
		aryDspData[i].fIsExits=0;					/*表示存在*/
		aryDspData[i].fIsOpen=false;				/*表示未打开*/
		aryDspData[i].fTrunkCount=-1;				/*特别注意：此处计数都是从0开始*/
		aryDspData[i].fTrunkOpenCount=-1;
		aryDspData[i].fVoiceCount=-1;
		aryDspData[i].fVoiceOpenCount=-1;
		aryDspData[i].fVoipCount=-1;
		aryDspData[i].fVoipOpenCount=-1;
		aryDspData[i].pTrunkDevice=NULL;
		aryDspData[i].pVoiceDevice=NULL;
		aryDspData[i].pVoipDevice=NULL;
	}

	/*全局当前设备编码初始化*/
	mPubDeviceNo=-1;	
	mPubVoiceNo=-1;
	mPubVoipNo=-1;
	mPubTrunkNo=-1;
	/*DSP全局当前编号初始化*/
	mPubDspNo=-1;	
	/*有效的DSP数量初始化*/
	mDspCount=-1;
	mOpenDspCount=-1;
	/*语音设备数量初始化*/
	mVoiceCount=-1;
	mOpenVoiceCount=-1;
	/*接口设备数量初始化*/
	mTrunkCount=-1;
	mOpenTrunkCount=-1;
	/*VOIP设备数量初始化*/
	mVoipCount=-1;
	mOpenVoipCount=-1;
	pRowNum=-1;
	/*放音任务全局编号*/
	mPlayTaskID=-1;
	return true;
}


/***********************************************************************
功能：根据语音设备状态，获取相应的描述字符串
***********************************************************************/
char *	GetVoiceStatusString(VOICE_STATUS mStatus)
{
	switch(mStatus)
	{
	case	VOC_WAIT_OPEN:		
		{
			return "等待打开";
		}
	case	VOC_WAIT_WORK:
		{
			return "等待初始化";
		}
	case	VOC_FREE:								
		{
			return "空闲";
		}
	case	VOC_OFFHOOK:							
		{
			return "摘机";
		}
	case	VOC_WAITHANGUP:
		{
			return "等待挂机";
		}
	case	VOC_WAIT_DIAL:							
		{
			return "等待拨号";
		}
	case	VOC_DTMF_INPUT:							
		{
			return "正在收号";
		}
	case	VOC_CALL_OUT:							
		{
			return "呼出中";
		}
	case	VOC_CALL_OUT_FAILE:						
		{
			return "呼叫失败";
		}
	case	VOC_CALL_OUT_SUCCESS:					
		{
			return "呼叫成功";
		}
	case	VOC_CONNECTED:							
		{
			return "通话中";
		}
	case	VOC_CONNECT_DTMF:						
		{
			return "通话中用户按键";
		}
	case	VOC_ONHOOK:								
		{
			return "挂机";
		}
	case	VOC_FLASH:								
		{
			return "拍叉簧";
		}
	case	VOC_FLASH_WAIT_DIAL:					
		{
			return "拍叉簧等待拨号";
		}
	case	VOC_FLASH_CONNECT:						
		{
			return "拍叉簧已连接";
		}
	case	VOC_CALL_IN:							
		{
			return "有电话呼入";
		}
	case	VOC_RINGING:							
		{
			return "振铃中";
		}
	case	VOC_RING_TIMEOUT:						
		{
			return "振铃超时";
		}
	case	VOC_FETCH_DIAL:							
		{
			return "抢接拨号";
		}
	case	VOC_FETCH_CONNECT:						
		{
			return "抢接连接";
		}
	case	VOC_SYS_USE:
		{
			return "系统保留";
		}
	case	VOC_ERROR_CALL_LONG:
		{
			return "呼出中";
		}
	case	VOC_PLAY_NUMBER:
		{
			return	"播放本机号码";
		}
	case	VOC_PLAY_NOTICE:
		{
			return	"播放语音提示";
		}
	case	VOC_PLAY_DIAL_WRONG:
		{
			return	"播放语音提示";
		}
	case	VOC_PLAY_WELCOME:
		{
			return	"播放欢迎语音";
		}
	case	VOC_ANSWER_CALL:
		{
			return	"应答电话";
		}
	case	VOC_PLAY_VOIP_ERROR:
		{
			return	"播放VOIP错误音";
		}
	case	VOC_HUIXIAN_TURN:
		{
			return	"汇线通抢接";
		}
	case	VOC_WAIT_MAIN:
		{
			return	"等待汇线通反馈";
		}
	case	VOC_WAIT_FIRST_DIAL_TONE:
		{
			return "等待汇线局送拨号音";
		}
	case	VOC_WAIT_SECOND_DIAL_TONE:
		{
			return "等待局方送拨号音";
		}
	}
	return "不可用";
}

/***********************************************************************
功能:根据VOIP设备状态，获取相应的描述字符串
***********************************************************************/
char *	GetVoipStatusString(VOIP_STATUS mStatus)
{
	switch(mStatus)
	{
	case	VOIP_WAIT_OPEN:							/*等待打开*/
		{
			return	"等待打开";
		}
	case	VOIP_WAIT_WORK:							/*等待初始化*/
		{
			return "等待初始化";
		}
	case	VOIP_FREE:								/*空闲*/
		{
			return "空闲";
		}
	case	VOIP_CALL_IN:							/*有电话呼入*/
		{
			return "有电话呼入";
		}
	case	VOIP_WAIT_CONNECT:						/*内线连接中*/	
		{
			return "内线连接中";
		}
	case	VOIP_CALL_OUT_FAILE:					/*内线呼叫失败*/
		{
			return "内线呼叫失败";
		}
	case	VOIP_CALL_OUT_SUCCESS:					/*内线呼叫成功*/
		{
			return "内线呼叫成功";
		}
	case	VOIP_CONNECTED:							/*通话中*/	
		{
			return "通话中";
		}
	case	VOIP_CONNECT_DTMF:						/*通话中用户按键*/
		{
			return "通话中用户按键";
		}
	case	VOIP_ONHOOK:							/*挂机*/
		{
			return "挂机";
		}
	case	VOIP_CALL_VOIP:							/*有电话呼出*/
		{
			return "呼出电话";
		}
	case	VOIP_CALL_VOIP_FAILE:					/*呼叫外线失败*/
		{
			return "呼叫外线失败";
		}
	case	VOIP_CALL_VOIP_SUCCESS:					/*呼叫外线成功*/
		{
			return "呼叫外线成功";
		}
	case	VOIP_PLAY_FILE:
		{
			return	"放音";
		}
	case	VOIP_WAITHUNGUP:
		{
			return	"等待挂机";
		}
	}
	return "不可用";
}


/***********************************************************************
功能：初始化一个列表控件。只是初始化列标题。不填充数据
***********************************************************************/
void	InitStatusControl(CListCtrl *pControl)				
{
	char	pTitle[11][20];		
	int		pWidth[11];
	int		i;

	strcpy(pTitle[0],"全局设备号");
	strcpy(pTitle[1],"设备状态");
	strcpy(pTitle[2],"线路状态");
	strcpy(pTitle[3],"DSP编号");
	strcpy(pTitle[4],"通道号");
	strcpy(pTitle[5],"设备类型");
	strcpy(pTitle[6],"分配号码");
	strcpy(pTitle[7],"主叫号码");
	strcpy(pTitle[8],"被叫号码");
	strcpy(pTitle[9],"DTMF码");
	strcpy(pTitle[10],"描述");
	pWidth[0]=100;
	pWidth[1]=100;
	pWidth[2]=100;
	pWidth[3]=80;
	pWidth[4]=100;
	pWidth[5]=80;
	pWidth[6]=100;
	pWidth[7]=150;
	pWidth[8]=150;
	pWidth[9]=150;
	pWidth[10]=300;
	pControl->DeleteAllItems();
	for(i=0;i<11;i++)
	{
		pControl->InsertColumn(i,pTitle[i],LVCFMT_LEFT,pWidth[i]);
	}
	return;
}

/***********************************************************************
功能：显示所有的设备状态
***********************************************************************/
void	DrawAllItem()
{
	int		i;		
	int		mIndex;
	int		mRow;	
	DJ_S32	mDspNo;
	char	mInfo[50];
	char	mTemp[20];
	CListCtrl *pControl;

	pControl=(CListCtrl *)pMainDlg->GetDlgItem(IDC_LIST_STATUS);

	/*显示所有的接口设备状态*/
	pControl->DeleteAllItems();
	mRow=0; 
	for(i=0;i<=mTrunkCount;i++)
	{
		mDspNo=aryTrunkIndex[i].fDspNo;
		pControl->InsertItem(mRow,_bstr_t(aryTrunkIndex[i].fChannelNo));
		mIndex=FindIndexByChannelID(aryTrunkIndex[i].fDspNo,aryTrunkIndex[i].fChannelID_t,DEVICE_TRUNK);
		strcpy(mInfo,GetVoiceStatusString(aryDspData[mDspNo].pTrunkDevice[mIndex].fStatusCode));
		pControl->SetItemText(mRow,1,mInfo);
		strcpy(mTemp,GetLineStatusString(aryDspData[mDspNo].pTrunkDevice[mIndex].fLineState));
		pControl->SetItemText(mRow,2,mTemp);
		itoa(aryTrunkIndex[i].fDspNo,mTemp,10);
		pControl->SetItemText(mRow,3,mTemp);
		itoa(aryTrunkIndex[i].fChannelID_t,mTemp,10);
		pControl->SetItemText(mRow,4,mTemp);
		if(aryDspData[mDspNo].pTrunkDevice[mIndex].fLineType==LINE_IN)
			pControl->SetItemText(mRow,5,"内线");
		if(aryDspData[mDspNo].pTrunkDevice[mIndex].fLineType==LINE_OUT)
			pControl->SetItemText(mRow,5,"外线");
		if(aryDspData[mDspNo].pTrunkDevice[mIndex].fLineType==LINE_NULL)
			pControl->SetItemText(mRow,5,"空置");

		pControl->SetItemText(mRow,6,aryDspData[mDspNo].pTrunkDevice[mIndex].fLocalNumber);
		pControl->SetItemText(mRow,7,aryDspData[mDspNo].pTrunkDevice[mIndex].fCallOutNumber);
		pControl->SetItemText(mRow,8,aryDspData[mDspNo].pTrunkDevice[mIndex].fCallInNumber);
		pControl->SetItemText(mRow,9,aryDspData[mDspNo].pTrunkDevice[mIndex].fDtmfBuff);
		aryTrunkIndex[i].fRowNo=mRow;
		DisplayStatusBar();
		mRow=mRow+1;
	}
	/*显示所有语音设备的状态*/
	for(i=0;i<=mVoiceCount;i++)
	{
		mDspNo=aryVoiceIndex[i].fDspNo;
		pControl->InsertItem(mRow,_bstr_t(aryVoiceIndex[i].fChannelNo));
		mIndex=FindIndexByChannelID(aryVoiceIndex[i].fDspNo,aryVoiceIndex[i].fChannelID_t,DEVICE_VOICE);
		strcpy(mInfo,GetVoiceStatusString(aryDspData[mDspNo].pVoiceDevice[mIndex].fStatusCode));
		pControl->SetItemText(mRow,1,mInfo);
		strcpy(mTemp,GetLineStatusString(aryDspData[mDspNo].pVoiceDevice[mIndex].fLineState));
		pControl->SetItemText(mRow,2,mTemp);
		itoa(aryVoiceIndex[i].fDspNo,mTemp,10);
		pControl->SetItemText(mRow,3,mTemp);
		itoa(aryVoiceIndex[i].fChannelID_t,mTemp,10);
		pControl->SetItemText(mRow,4,mTemp);
		pControl->SetItemText(mRow,5,"语音设备");
		pControl->SetItemText(mRow,6,aryDspData[mDspNo].pVoiceDevice[mIndex].fLocalNumber);
		pControl->SetItemText(mRow,7,aryDspData[mDspNo].pVoiceDevice[mIndex].fCallOutNumber);
		pControl->SetItemText(mRow,8,aryDspData[mDspNo].pVoiceDevice[mIndex].fCallInNumber);
		pControl->SetItemText(mRow,9,aryDspData[mDspNo].pVoiceDevice[mIndex].fDtmfBuff);
		aryVoiceIndex[i].fRowNo=mRow;
		DisplayStatusBar();
		mRow=mRow+1;
	}
	/*显示所有VOIP的状态*/
	for(i=0;i<=mVoipCount;i++)
	{
		mDspNo=aryVoipIndex[i].fDspNo;
		pControl->InsertItem(mRow,_bstr_t(aryVoipIndex[i].fChannelNo));
		mIndex=FindIndexByChannelID(aryVoipIndex[i].fDspNo,aryVoipIndex[i].fChannelID_t,DEVICE_VOIP);
		strcpy(mInfo,GetVoipStatusString(aryDspData[mDspNo].pVoipDevice[mIndex].fStatusCode));
		pControl->SetItemText(mRow,1,mInfo);
		itoa(aryVoipIndex[i].fDspNo,mTemp,10);
		pControl->SetItemText(mRow,3,mTemp);
		itoa(aryVoipIndex[i].fChannelID_t,mTemp,10);
		pControl->SetItemText(mRow,4,mTemp);
		pControl->SetItemText(mRow,5,"VOIP设备");
		pControl->SetItemText(mRow,6,aryDspData[mDspNo].pVoipDevice[mIndex].fLocalNumber);
		pControl->SetItemText(mRow,7,aryDspData[mDspNo].pVoipDevice[mIndex].fCallOutNumber);
		pControl->SetItemText(mRow,8,aryDspData[mDspNo].pVoipDevice[mIndex].fCallInNumber);
		pControl->SetItemText(mRow,9,aryDspData[mDspNo].pVoipDevice[mIndex].fDtmfBuff);
		aryVoipIndex[i].fRowNo=mRow;
		DisplayStatusBar();
		mRow=mRow+1;
	}
}

/***********************************************************************
功能：XMS事件回调函数。本系统的流程源头
***********************************************************************/
DJ_Void XMSEventHandler(DJ_U32 esrParam)
{
	Acs_Evt_t		*pEvt=NULL;
	Acs_Dev_List_Head_t		*pHeadData=NULL;
	DJ_S32					mDspID;					/*DSP编号*/
	DJ_S32					mDeviceNumber;			/*本次获取的设备总数*/

	pEvt=(Acs_Evt_t *)esrParam;
	/*char	mTemp[100];
	itoa(pEvt->m_s32EventType,mTemp,10);
	strcat(mTemp," Event");
	AddMsg(mTemp);
	*/
	switch(pEvt->m_s32EventType)
	{
	case XMS_EVT_QUERY_DEVICE:							/*获取设备列表事件*/
		{
			//DisplayStatusBar("正在获取设备列表....");
			DisplayEvt(pEvt);
			if(fIsRun)	return;
			pHeadData=(Acs_Dev_List_Head_t *)FetchEventData(pEvt);
			mDspID=(DJ_S32)pHeadData->m_s32ModuleID;			/*获取DSP编号*/
			mDeviceNumber=pHeadData->m_s32DeviceNum;		/*获取设备数量*/
			if(mDspID<0 || mDspID>MAX_DSP_COUNT)	return;	/*对于超过最大DSP数量的DSP不处理*/
			/*本程序只处理VOICE设备，VOIP设备和Interface设备*/
			if(pHeadData->m_s32DeviceMain==XMS_DEVMAIN_VOICE || pHeadData->m_s32DeviceMain==XMS_DEVMAIN_INTERFACE_CH
				|| pHeadData->m_s32DeviceMain==XMS_DEVMAIN_VOIP || pHeadData->m_s32DeviceMain==XMS_DEVMAIN_BOARD)
			{
				if(!InsertDeviceToUse(mDspID,pHeadData))
				{
					MessageBox(NULL,"系统获取设备列表失败!!","Error",MB_OK);
					CloseXMSConnect();
					PostQuitMessage(0);
					return;
				}
			}
		}
		break;
	case XMS_EVT_QUERY_ONE_DSP_END:
		{
			//DisplayStatusBar("准备获取下一个DSP设备列表.....");
		}
		break;
	case XMS_EVT_QUERY_DEVICE_END:
		/*打开所有设备*/
		//DisplayStatusBar("设备列表获取完毕!开始进行初始化...........");
		DisplayEvt(pEvt);
		if(fIsRun)	return;
		DrawAllItem();
		if(!OpenAllDevice())
		{
			MessageBox(NULL,"系统打开设备失败!!","Error",MB_OK);
			return;
		}
		break;
	case XMS_EVT_OPEN_DEVICE:
		/*该设备已经打开*/
		//DisplayStatusBar("正在打开设备.......");
		pHeadData=(Acs_Dev_List_Head_t *)FetchEventData(pEvt);
		mDspID=pEvt->m_DeviceID.m_s8ModuleID;	/*特别注意：应该用此参数*/
		if(fIsRun)	return;
		/*对底层返回的打开的数据进行初始化*/
		if(!OpenDeviceSuccess(mDspID,&pEvt->m_DeviceID))
		{
			MessageBox(NULL,"打开设备失败!!","Error",MB_OK);
			return;
		}
		break;
	case XMS_EVT_CLOSE_DEVICE:
		{
			DisplayEvt(pEvt);	
		}
		break;
	case XMS_EVT_RESET_DEVICE:
		{
			DisplayEvt(pEvt);
		}
		break;
	case XMS_EVT_DEVICESTATE:
		/*动态显示底层返回的设备状态*/
		{
			Acs_GeneralProc_Data *pGeneralData = NULL;
			pGeneralData=(Acs_GeneralProc_Data *)FetchEventData(pEvt);
			ProcessDeviceStatusEvt(&pEvt->m_DeviceID,pGeneralData->m_s32DeviceState);
		}
		break;
	case XMS_EVT_LINKDEVICE:
		/*有连接建立*/
		{
			Acs_CallControl_Data	*pContrlData;
			pContrlData=(Acs_CallControl_Data *)FetchEventData(pEvt);
/*			if(pContrlData->m_s32AcsEvtState==1)
				AddMsg("建立连接成功");
			else
				AddMsg("建立连接失败");
*/
		}
		break;
	case	XMS_EVT_SETPARAM:
		{
			Acs_ParamProc_Data	*pData=NULL;
			pData=(Acs_ParamProc_Data *)FetchEventData(pEvt);
			if(pData->m_s32AcsEvtState!=1)
				AddMsg("设置失败");
			else
				AddMsg("设置成功");
			char	mTemp[20];
			itoa(pEvt->m_DeviceID.m_s16ChannelID,mTemp,10);
			AddMsg(mTemp);
		}
		break;
	default:
		if(pEvt->m_DeviceID.m_s16DeviceMain==XMS_DEVMAIN_INTERFACE_CH)
		{
			/*接口通道事件的处理*/
			if(pEvt->m_DeviceID.m_s16DeviceSub==XMS_DEVSUB_ANALOG_TRUNK)
			{
				/*外线事件处理*/
				TrunkDeviceWork(&pEvt->m_DeviceID,pEvt);
			}
			if(pEvt->m_DeviceID.m_s16DeviceSub==XMS_DEVSUB_ANALOG_USER)
			{
				/*内线事件处理*/
				UserDeviceWork(&pEvt->m_DeviceID,pEvt);
			}
		}
		if(pEvt->m_DeviceID.m_s16DeviceMain==XMS_DEVMAIN_VOICE)
		{
			/*语音通道事件的处理*/
			DJ_S32	mDspNo;
			ChannelID_t	mChannelID;
			long	mIndex;
			DeviceID_t		*pDevice;
			mDspNo=pEvt->m_DeviceID.m_s8ModuleID;
			mChannelID=pEvt->m_DeviceID.m_s16ChannelID;
			mIndex=FindIndexByChannelID(mDspNo,mChannelID,DEVICE_VOICE);
			mIndex=aryVoiceIndex[mIndex].fIndex;
			pDevice=&aryDspData[mDspNo].pVoiceDevice[mIndex].fLinkTrunkDeviceID;
			if(pDevice->m_s16DeviceSub==XMS_DEVSUB_ANALOG_TRUNK)
			{
				/*外线事件处理*/
				TrunkDeviceWork(pDevice,pEvt);
			}
			if(pDevice->m_s16DeviceSub==XMS_DEVSUB_ANALOG_USER)
			{
				/*内线事件处理*/
				UserDeviceWork(pDevice,pEvt);
			}
		}
		if(pEvt->m_DeviceID.m_s16DeviceMain==XMS_DEVMAIN_VOIP)
		{
			/*VOIP通道事件的处理*/
			VoipDeviceWork(&pEvt->m_DeviceID,pEvt);

		}
	}
}


/***********************************************************************
功能：将一个设备添加到可用的设备队列中，此函数在XMS_EVT_QUEYR_DEVICE事件中调用
注意：本函数将统一处理VOICE设备，INTERFACE设备和VOIP设备，本函数流程如下：
1.判断该DSP是否初始化过，如果没有，则将该DSP信息写入数组
2.如果当前设备是语音设备，则加入语音设备数组
3.如果当前设备是接口设备，则加入接口设备数组
4.如果当前设备是VOIP设备，则加入VOIP设备数组
***********************************************************************/
bool	InsertDeviceToUse(DJ_S32	mDspID,Acs_Dev_List_Head_t *pDevList)
{
	DJ_S32		mDeviceNum;
	int			i;
	long		mDspNo;
	long		mDeviceNo;				/*本系统用的全局设备编号*/
	DeviceID_t	 *pDev;
	
	/*判断该DSP是否已经初始化过，如果没有，则先初始化DSP信息*/
	if(pDevList->m_s32DeviceMain==XMS_DEVMAIN_BOARD)
	{
		/*该DSP还没有初始化*/
		mDspNo=mDspID;							/*DSPID即为DSPNO号码*/
		pDev=(DeviceID_t *)((BYTE *)pDevList + sizeof(Acs_Dev_List_Head_t));
		aryDspData[mDspID].fDeviceID=pDev[0];
		aryDspData[mDspID].fIsExits=1;
		aryDspData[mDspID].fDspID=mDspID;
		mDspCount=mDspCount+1;							/*累加有效的DSP数量*/
	}

	mDspNo=mDspID;										/*将DSPNO设置为DSPID*/
	if(mDspNo<0)										
	{
		MessageBox(NULL,"系统内部错误!!","Error",MB_OK);
		return false;
	}
	/*如果返回的是语音设备，则对语音设备进行初始化*/
	if(pDevList->m_s32DeviceMain==XMS_DEVMAIN_VOICE)
	{
		mDeviceNum=pDevList->m_s32DeviceNum;
		if(mDeviceNum>0)
		{
			aryDspData[mDspNo].fVoiceCount=aryDspData[mDspNo].fVoiceCount+mDeviceNum;
			aryDspData[mDspNo].pVoiceDevice=new TYPE_VOICE_DATA[mDeviceNum];			//分配内存
			if(!aryDspData[mDspNo].pVoiceDevice)
			{
				MessageBox(NULL,"系统为语音设备分配内存失败!!","Error",MB_OK);
				return false;
			}
			memset(aryDspData[mDspNo].pVoiceDevice,0,sizeof(TYPE_VOICE_DATA)*mDeviceNum);
			DeviceID_t		*pDeviceID;
			/*特别注意：pDeviceID底层返回的是所有的设备ID的指针*/
			pDeviceID=(DeviceID_t *)((BYTE *)pDevList+sizeof(Acs_Dev_List_Head_t));	
			mVoiceCount=mVoiceCount+mDeviceNum;						/*累加语音设备数量*/

			for(i=0;i<mDeviceNum;i++)
			{
				mDeviceNo=getVoiceDeviceNo();						/*专门的语音设备全局编号*/
				aryDspData[mDspNo].pVoiceDevice[i].fChannelNo=mDeviceNo;
				aryDspData[mDspNo].pVoiceDevice[i].fDeviceID=pDeviceID[i];
				aryDspData[mDspNo].pVoiceDevice[i].fDeviceType=DEVICE_VOICE;
				aryDspData[mDspNo].pVoiceDevice[i].fStatusCode=VOC_WAIT_OPEN;
				aryDspData[mDspNo].pVoiceDevice[i].fIsExit=1;		/*表示该设备存在*/
				aryDspData[mDspNo].pVoiceDevice[i].fIsOpen=false;	/*表示未打开*/
				aryDspData[mDspNo].pVoiceDevice[i].fLinkTrunkChannelNo=-1;
				aryDspData[mDspNo].pVoiceDevice[i].fIsLinkTrunk=false;
				aryDspData[mDspNo].pVoiceDevice[i].fLinkVoipChannelNo=-1;
				aryDspData[mDspNo].pVoiceDevice[i].fIsLinkVoip=false;
				aryDspData[mDspNo].pVoiceDevice[i].fLineState=-1;


				/*全局索引数组的建立*/
				aryVoiceIndex[mDeviceNo].fChannelID_t=pDeviceID[i].m_s16ChannelID;	/*东进系统内部通道号*/
				aryVoiceIndex[mDeviceNo].fChannelNo=mDeviceNo;	/*全局设备编号*/	
				aryVoiceIndex[mDeviceNo].fIndex=i;				/*记下数组下标*/
				aryVoiceIndex[mDeviceNo].fDspNo=mDspNo;
				aryVoiceIndex[mDeviceNo].fDeviceType=DEVICE_VOICE;
			}
		}		
	}
	/*如果返回的是接口设备，则对接口设备进行初始化*/
	if(pDevList->m_s32DeviceMain==XMS_DEVMAIN_INTERFACE_CH)
	{
		mDeviceNum=pDevList->m_s32DeviceNum;
		if(mDeviceNum>0)
		{
			aryDspData[mDspNo].fTrunkCount=aryDspData[mDspNo].fTrunkCount+mDeviceNum;
			aryDspData[mDspNo].pTrunkDevice=new TYPE_INTERFACE_DATA[mDeviceNum];
			if(!aryDspData[mDspNo].pTrunkDevice)
			{
				MessageBox(NULL,"系统为接口设备分配内存失败!!","Error",MB_OK);
				return false;
			}
			memset(aryDspData[mDspNo].pTrunkDevice,0,sizeof(TYPE_INTERFACE_DATA)*mDeviceNum);
			DeviceID_t		*pDeviceID;
			pDeviceID=(DeviceID_t *)((BYTE *)pDevList+sizeof(Acs_Dev_List_Head_t));
			mTrunkCount=mTrunkCount+mDeviceNum;						/*接口设备数量累加*/
			for(i=0;i<mDeviceNum;i++)
			{
				/*基本数据提取*/
				aryDspData[mDspNo].pTrunkDevice[i].fDeviceID=pDeviceID[i];
				aryDspData[mDspNo].pTrunkDevice[i].fDeviceType=DEVICE_TRUNK;
				aryDspData[mDspNo].pTrunkDevice[i].fStatusCode=VOC_WAIT_OPEN;
				if(aryDspData[mDspNo].pTrunkDevice[i].fDeviceID.m_s16DeviceSub==XMS_DEVSUB_ANALOG_TRUNK || aryDspData[mDspNo].pTrunkDevice[i].fDeviceID.m_s16DeviceSub==XMS_DEVSUB_ANALOG_USER)
				{
					aryDspData[mDspNo].pTrunkDevice[i].fIsExit=1;		/*表示该设备存在*/		
					aryDspData[mDspNo].pTrunkDevice[i].fIsOpen=false;	/*表示未打开*/	
					if(pDeviceID[i].m_s16DeviceSub==XMS_DEVSUB_ANALOG_USER) aryDspData[mDspNo].pTrunkDevice[i].fLineType=LINE_IN;
					if(pDeviceID[i].m_s16DeviceSub==XMS_DEVSUB_ANALOG_TRUNK) aryDspData[mDspNo].pTrunkDevice[i].fLineType=LINE_OUT;
				}
				else
				{
					aryDspData[mDspNo].pTrunkDevice[i].fIsExit=0;		/*表示该设备没有存在*/
					aryDspData[mDspNo].pTrunkDevice[i].fIsOpen=false;	/*表示未打开*/	
					aryDspData[mDspNo].pTrunkDevice[i].fLineType=LINE_NULL;	/*表示空线*/

				}
				aryDspData[mDspNo].pTrunkDevice[i].fLineState=-1;			/*表示线路正在等待打开中*/
				aryDspData[mDspNo].pTrunkDevice[i].fLinkVoiceChannelNo=-1;	/*表示没有连接任何设备*/	
				aryDspData[mDspNo].pTrunkDevice[i].fIsLinkVoice=false;
				aryDspData[mDspNo].pTrunkDevice[i].fLinkTrunkChannelNo=-1;
				aryDspData[mDspNo].pTrunkDevice[i].fIsLinkTrunk=false;
				aryDspData[mDspNo].pTrunkDevice[i].fisLinkVoipVoice=false;
				aryDspData[mDspNo].pTrunkDevice[i].fLinkVoipVoiceDeviceNo=-1;
				/*拍叉簧数据初始化*/
				aryDspData[mDspNo].pTrunkDevice[i].fPrevDeviceType=-1;
				aryDspData[mDspNo].pTrunkDevice[i].fPrevTrunkChannelNo=-1;
				aryDspData[mDspNo].pTrunkDevice[i].fPrevVoipChannelNo=-1;
				aryDspData[mDspNo].pTrunkDevice[i].fChaHuanCount=0;
				aryDspData[mDspNo].pTrunkDevice[i].fIsChaHuan=false;
				aryDspData[mDspNo].pTrunkDevice[i].fChaHuanIsCallStatus=false;
				aryDspData[mDspNo].pTrunkDevice[i].fChaHuanIsReset=false;
				aryDspData[mDspNo].pTrunkDevice[i].fChaHuanLinkChannenlNo=-1;
				strcpy(aryDspData[mDspNo].pTrunkDevice[i].fSpeedDial,"");		//缩位拨号
				strcpy(aryDspData[mDspNo].pTrunkDevice[i].fTargetUserNo,"");

				/*处理底层保留的通道*/
				if(i==0)	aryDspData[mDspNo].pTrunkDevice[i].fStatusCode=VOC_SYS_USE;
				if((i % 16)==0)aryDspData[mDspNo].pTrunkDevice[i].fStatusCode=VOC_SYS_USE;
				/*判断该设备是否已经初始化*/
				long	mTempDeviceNo;
				mTempDeviceNo=getDeviceNoByChannelID(mDspNo,pDeviceID[i].m_s16ChannelID);
				if(mTempDeviceNo==-1)	return false;
				if(mTempDeviceNo==-2)
					mDeviceNo=getTrunkDeviceNo();	//获取trunk专用的全局设备编号
				else
				{
					mDeviceNo=mTempDeviceNo;
					mPubTrunkNo=mDeviceNo;
				}
				/*全局索引数组的建立*/
				aryDspData[mDspNo].pTrunkDevice[i].fChannelNo=mDeviceNo;
				aryTrunkIndex[mDeviceNo].fChannelID_t=pDeviceID[i].m_s16ChannelID;	/*东进系统内部通道号*/
				aryTrunkIndex[mDeviceNo].fChannelNo=mDeviceNo;	/*全局设备编号*/	
				aryTrunkIndex[mDeviceNo].fIndex=i;				/*记下数组下标*/
				aryTrunkIndex[mDeviceNo].fDspNo=mDspNo;
				aryTrunkIndex[mDeviceNo].fDeviceType=DEVICE_TRUNK;

				/*判断是否需要进行数据处理*/
				if(mTempDeviceNo==-2)
				{
					if(InitLineDbRecord(mDeviceNo,mDspNo,&aryDspData[mDspNo].pTrunkDevice[i].fDeviceID)==false)	return false;
				}
				/*内外线的处理，用于处理当有内线，外线配置变更的情况下进行,充分保证数据与硬件的同步*/
				if(aryDspData[mDspNo].pTrunkDevice[i].fDeviceID.m_s16DeviceSub==XMS_DEVSUB_ANALOG_TRUNK || aryDspData[mDspNo].pTrunkDevice[i].fDeviceID.m_s16DeviceSub==XMS_DEVSUB_ANALOG_USER)
				{
					if(!UpdateLineStatus(mDeviceNo,mDspNo,aryDspData[mDspNo].pTrunkDevice[i].fDeviceID.m_s16ChannelID,aryDspData[mDspNo].pTrunkDevice[i].fDeviceID.m_s16DeviceSub)) return false;
					if(getTrunkInfoFromDb(mDeviceNo,mDspNo,aryDspData[mDspNo].pTrunkDevice[i].fDeviceID.m_s16ChannelID,
						aryDspData[mDspNo].pTrunkDevice[i].fLineType)==false)	return false;
				}
			}
		}
	}
	if(pDevList->m_s32DeviceMain==XMS_DEVMAIN_VOIP)
	{
		mDeviceNum=pDevList->m_s32DeviceNum;
		if(mDeviceNum>0)
		{
			aryDspData[mDspNo].fVoipCount=aryDspData[mDspNo].fVoipCount+mDeviceNum;
			aryDspData[mDspNo].pVoipDevice=new TYPE_VOIP_DATA[mDeviceNum];
			if(!aryDspData[mDspNo].pVoipDevice)
			{
				MessageBox(NULL,"系统为VOIP设备分配内存失败!!","Error",MB_OK);
				return false;
			}
			memset(aryDspData[mDspNo].pVoipDevice,0,sizeof(TYPE_VOIP_DATA)*mDeviceNum);
			DeviceID_t		*pDeviceID;
			pDeviceID=(DeviceID_t *)((BYTE *)pDevList+sizeof(Acs_Dev_List_Head_t));
			mVoipCount=mVoipCount+mDeviceNum;					/*VOIP设备数量累加*/
			for(i=0;i<mDeviceNum;i++)
			{
				mDeviceNo=getVoipDeviceNo();					/*获取全局的voip设备编号*/
				aryDspData[mDspNo].pVoipDevice[i].fChannelNo=mDeviceNo;
				aryDspData[mDspNo].pVoipDevice[i].fDeviceID=pDeviceID[i];
				aryDspData[mDspNo].pVoipDevice[i].fDeviceType=DEVICE_VOIP;
				aryDspData[mDspNo].pVoipDevice[i].fStatusCode=VOIP_WAIT_OPEN;
				aryDspData[mDspNo].pVoipDevice[i].fIsExit=1;	/*表示该设备存在*/		
				aryDspData[mDspNo].pVoipDevice[i].fIsOpen=false;	/*表示未打开*/	
				aryDspData[mDspNo].pVoipDevice[i].fLinkVoiceChannelNo=-1;	/*表示没有连接任何设备*/	
				aryDspData[mDspNo].pVoipDevice[i].fIsLinkVoice=false;

				/*新写代码，用于实现ip主动拆内线*/
				aryDspData[mDspNo].pVoipDevice[i].fLinkTrunkChannelNo=-1;
				aryDspData[mDspNo].pVoipDevice[i].fLinkTrunkLineType=LINE_NULL;		//空线
				aryDspData[mDspNo].pVoipDevice[i].fIsLinkTrunk=false;


				/*全局索引数组的建立*/
				aryVoipIndex[mDeviceNo].fChannelID_t=pDeviceID[i].m_s16ChannelID;	/*东进系统内部通道号*/
				aryVoipIndex[mDeviceNo].fChannelNo=mDeviceNo;	/*全局设备编号*/	
				aryVoipIndex[mDeviceNo].fIndex=i;				/*记下数组下标*/
				aryVoipIndex[mDeviceNo].fDspNo=mDspNo;
				aryVoipIndex[mDeviceNo].fDeviceType=DEVICE_VOIP;

				/*对VOIP数据进行初始化*/
				if(pLocalVoipProtocol==1)
					aryDspData[mDspNo].pVoipDevice[i].fProtocolType=XMS_VOIP_PROTOCOL_H323;
				else
					aryDspData[mDspNo].pVoipDevice[i].fProtocolType=XMS_VOIP_PROTOCOL_SIP;
				aryDspData[mDspNo].pVoipDevice[i].fSourceAddr.port=pLocalVoipServerPort;
				strcpy(aryDspData[mDspNo].pVoipDevice[i].fSourceAddr.tszAddr,pLocalVoipServerIP);
				strcpy(aryDspData[mDspNo].pVoipDevice[i].fSourceAddr.tszUserID,pLocalVoipUserName);
				aryDspData[mDspNo].pVoipDevice[i].fTargetAddr.port=pVoipServerPort;
				strcpy(aryDspData[mDspNo].pVoipDevice[i].fTargetAddr.tszAddr,pVoipServerIp);
				strcpy(aryDspData[mDspNo].pVoipDevice[i].fTargetAddr.tszUserID,pVoipUserName);
			}
		}		
	}
	return true;
}

/***********************************************************************
功能：定时器回调函数，用于扫描各个通道的dtmf缓冲区的状况
***********************************************************************/
VOID CALLBACK TimerProc(HWND hwnd,UINT uMsg,UINT idEvent,DWORD dwTime)
{
}

/***********************************************************************
功能：获取交换机设备列表
***********************************************************************/
bool	getXMSDeviceList()
{
	RetCode_t	rtCode;
	rtCode=XMS_acsGetDeviceList(pFlowHandle,NULL);
	if(rtCode==-ACSERR_BADHDL || rtCode==-ACSERR_FAILSEND)
	{
		MessageBox(NULL,"系统调用获取设备列表函数失败!!","ERROR",MB_OK);
		return false;
	}
	return true;
}

/***********************************************************************
功能：断开与XMS服务的连接
***********************************************************************/
void	CloseXMSConnect()
{
	XMS_acsCloseStream(pFlowHandle,NULL);
}


/***********************************************************************
功能：获取一个全局的设备编码
***********************************************************************/
long	getDeviceNo()
{
	mPubDeviceNo=mPubDeviceNo+1;
	return mPubDeviceNo;
}

/***********************************************************************
功能：判断该DSP是否已经初始化过，如果已经初始化，则返回True，否则返回false
***********************************************************************/
bool	JudgeDspInit(DJ_S32 mDspID)
{
	bool	mResult=false;
	int		i;
	i=0;
	while(i<MAX_DSP_COUNT && !mResult)
	{
		if(aryDspData[i].fDspID==mDspID && aryDspData[i].fIsExits==1) mResult=true;
		i++;
	}
	return mResult;
}


/***********************************************************************
功能：调用XMS SDK中的OpenDevice打开所有设备。注：程序的跳转从系统返回的事件中进行
***********************************************************************/
bool	OpenAllDevice()
{
	int		i;
	int		j;	
	RetCode_t		rtCode;
	
	/*先打开所有的DSP*/
	for(i=0;i<=MAX_DSP_COUNT;i++)
	{
		/*如果不存在，则不需要进行打开*/
		if(aryDspData[i].fIsExits==1)		
		{
			rtCode=XMS_ctsOpenDevice(pFlowHandle,&aryDspData[i].fDeviceID,NULL);
			if(rtCode==-ACSERR_BADHDL)
			{
				MessageBox(NULL,"调用OpenDevice时,流程句柄非法!!","Error",MB_OK);
				return false;
			}
			if(rtCode==-ACSERR_FAILSEND)
			{
				MessageBox(NULL,"调用OpenDevice时,发送命令失败!!请检查流程服务器的命令最大队列数!!","Error",MB_OK);
				return false;
			}
			if(rtCode==-ACSERR_BADPARAMETER)
			{
				MessageBox(NULL,"调用OpenDevice时,设备编号为空!!","Error",MB_OK);
				return false;
			}
			/*打开语音设备*/
			for(j=0;j<=aryDspData[i].fVoiceCount;j++)
			{
				rtCode=XMS_ctsOpenDevice(pFlowHandle,&aryDspData[i].pVoiceDevice[j].fDeviceID,NULL);
				if(rtCode==-ACSERR_BADHDL)
				{
					MessageBox(NULL,"调用OpenDevice时,流程句柄非法!!","Error",MB_OK);
					return false;
				}
				if(rtCode==-ACSERR_FAILSEND)
				{
					MessageBox(NULL,"调用OpenDevice时,发送命令失败!!请检查流程服务器的命令最大队列数!!","Error",MB_OK);
					return false;
				}
				if(rtCode==-ACSERR_BADPARAMETER)
				{
					MessageBox(NULL,"调用OpenDevice时,设备编号为空!!","Error",MB_OK);
					return false;
				}
			}
			/*打开接口设备*/
			for(j=0;j<=aryDspData[i].fTrunkCount;j++)
			{
				rtCode=XMS_ctsOpenDevice(pFlowHandle,&aryDspData[i].pTrunkDevice[j].fDeviceID,NULL);
				if(rtCode==-ACSERR_BADHDL)
				{
					MessageBox(NULL,"调用OpenDevice时,流程句柄非法!!","Error",MB_OK);
					return false;
				}
				if(rtCode==-ACSERR_FAILSEND)
				{
					MessageBox(NULL,"调用OpenDevice时,发送命令失败!!请检查流程服务器的命令最大队列数!!","Error",MB_OK);
					return false;
				}
				if(rtCode==-ACSERR_BADPARAMETER)
				{
					MessageBox(NULL,"调用OpenDevice时,设备编号为空!!","Error",MB_OK);
					return false;
				}
			}
			/*打开VOIP设备*/
			for(j=0;j<=aryDspData[i].fVoipCount;j++)
			{
				rtCode=XMS_ctsOpenDevice(pFlowHandle,&aryDspData[i].pVoipDevice[j].fDeviceID,NULL);
				if(rtCode==-ACSERR_BADHDL)
				{
					MessageBox(NULL,"调用OpenDevice时,流程句柄非法!!","Error",MB_OK);
					return false;
				}
				if(rtCode==-ACSERR_FAILSEND)
				{
					MessageBox(NULL,"调用OpenDevice时,发送命令失败!!请检查流程服务器的命令最大队列数!!","Error",MB_OK);
					return false;
				}
				if(rtCode==-ACSERR_BADPARAMETER)
				{
					MessageBox(NULL,"调用OpenDevice时,设备编号为空!!","Error",MB_OK);
					return false;
				}
			}
		}
	}
	return true;
}


/***********************************************************************
功能：打开设备成功时调用
***********************************************************************/
bool	OpenDeviceSuccess(DJ_S32 mDspID,DeviceID_t	*pDeviceID)
{
	/*如果是DSP*/
	long	mDspNo;					/*下标*/
	long	mDeviceIndex;	

	if(pDeviceID->m_s16DeviceMain==XMS_DEVMAIN_BOARD)		
	{
			mDspNo=pDeviceID->m_s8ModuleID;									/*根据DSPid获取他的全局编号*/
			aryDspData[mDspNo].fDeviceID.m_CallID=pDeviceID->m_CallID;		/*底层必须的设置，不可缺少*/
			aryDspData[mDspNo].fIsOpen=true;									
			aryDspData[mDspNo].fDspID=mDspID;
			aryDspData[mDspNo].fDspNo=mDspNo;
			mOpenDspCount=mOpenDspCount+1;									/*累加全局打开的DSP数量*/
	}
	/*如果是语音通道*/
	if(pDeviceID->m_s16DeviceMain==XMS_DEVMAIN_VOICE)
	{
			long		mVoiceIndex;
			mDspNo=pDeviceID->m_s8ModuleID;
			mVoiceIndex=FindArrayIndexByChannelID(mDspNo,pDeviceID->m_s16ChannelID,DEVICE_VOICE);
			mDeviceIndex=aryVoiceIndex[mVoiceIndex].fIndex;
			if(mVoiceIndex!=-1)
			{
					aryDspData[mDspNo].pVoiceDevice[mDeviceIndex].fDeviceID.m_CallID=pDeviceID->m_CallID;	/*底层必须的设置，不可缺少*/
					aryDspData[mDspNo].pVoiceDevice[mDeviceIndex].fIsOpen=true;
					mOpenVoiceCount=mOpenVoiceCount+1;
					aryDspData[mDspNo].pVoiceDevice[mDeviceIndex].fStatusCode=VOC_WAIT_WORK;
			}
			DrawOneItem(mDspNo,aryVoiceIndex[mVoiceIndex].fChannelID_t,DEVICE_VOICE);
			ResetChannel(mDspNo,pDeviceID);
	}
	/*如果是接口设备*/
	if(pDeviceID->m_s16DeviceMain==XMS_DEVMAIN_INTERFACE_CH)
	{
			long		mTrunkIndex;
			mDspNo=pDeviceID->m_s8ModuleID;
			mTrunkIndex=FindArrayIndexByChannelID(mDspNo,pDeviceID->m_s16ChannelID,DEVICE_TRUNK);
			if(mTrunkIndex!=-1)
			{
				mDeviceIndex=aryTrunkIndex[mTrunkIndex].fIndex;
				aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID.m_CallID=pDeviceID->m_CallID;	/*底层必须的设置，不可缺少*/
				aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsOpen=true;
				aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode=VOC_WAIT_WORK;
				mOpenTrunkCount=mOpenTrunkCount+1;
				/*内线子类型的判断*/
				if(pDeviceID->m_s16DeviceSub==XMS_DEVSUB_ANALOG_USER)aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLineType=LINE_IN;
				/*外线子类型的判断*/
				if(pDeviceID->m_s16DeviceSub==XMS_DEVSUB_ANALOG_TRUNK)aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLineType=LINE_OUT;
				DrawOneItem(mDspNo,aryTrunkIndex[mTrunkIndex].fChannelID_t,DEVICE_TRUNK);
				ResetChannel(mDspNo,pDeviceID);
			}
	}
	/*如果是VOIP设备*/
	if(pDeviceID->m_s16DeviceMain==XMS_DEVMAIN_VOIP)
	{
		long		mVoipIndex;
		mDspNo=pDeviceID->m_s8ModuleID;
		mVoipIndex=FindArrayIndexByChannelID(mDspNo,pDeviceID->m_s16ChannelID,DEVICE_VOIP);
		if(mVoipIndex!=-1)
		{
			mDeviceIndex=aryVoipIndex[mVoipIndex].fIndex;
			/*底层必须的设置，不可缺少*/
			aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fDeviceID.m_CallID=pDeviceID->m_CallID;	
			aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fIsOpen=true;
			aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fStatusCode=VOIP_WAIT_WORK;
			mOpenVoipCount=mOpenVoipCount+1;
			DrawOneItem(mDspNo,aryVoipIndex[mVoipIndex].fChannelID_t,DEVICE_VOIP);
			ResetChannel(mDspNo,pDeviceID);
		}
	}
	return true;
}

/***********************************************************************
功能：通用设备数组下标查找，根据dspid和channelid_t以及设备类型，利用设备全局索引数组进行查找
***********************************************************************/
long	FindIndexByChannelID(DJ_S32 mDspID,ChannelID_t mChannelID,DEVICE_TYPE mDeviceType)
{
	long			mDspNo;
	int				i;
	bool			mFind;
	long			mResult;
	long			mMaxDeviceCount;
	mDspNo=mDspID;
	mFind=false;
	i=0;
	mResult=-1;
	mMaxDeviceCount=0;
	if(mDeviceType==DEVICE_VOIP) mMaxDeviceCount=MAX_VOIP_COUNT;
	if(mDeviceType==DEVICE_TRUNK) mMaxDeviceCount=MAX_INTERFACE_COUNT;
	if(mDeviceType==DEVICE_VOICE) mMaxDeviceCount=MAX_VOICE_COUNT;
	while(i<=mMaxDeviceCount && mFind==false)
	{
		if(mDeviceType==DEVICE_VOICE)
		{
			if(aryVoiceIndex[i].fDspNo==mDspNo && aryVoiceIndex[i].fChannelID_t==mChannelID && aryVoiceIndex[i].fDeviceType==mDeviceType)	
			{
				mResult=aryVoiceIndex[i].fIndex;
				mFind=true;
			}
		}
		if(mDeviceType==DEVICE_TRUNK)
		{
			if(aryTrunkIndex[i].fDspNo==mDspNo && aryTrunkIndex[i].fChannelID_t==mChannelID && aryTrunkIndex[i].fDeviceType==mDeviceType)	
			{
				mResult=aryTrunkIndex[i].fIndex;
				mFind=true;
			}
		}
		if(mDeviceType==DEVICE_VOIP)
		{
			if(aryVoipIndex[i].fDspNo==mDspNo && aryVoipIndex[i].fChannelID_t==mChannelID && aryVoipIndex[i].fDeviceType==mDeviceType)	
			{
				mResult=aryVoipIndex[i].fIndex;
				mFind=true;
			}
		}
		i++;
	}
	return mResult;
}


/***********************************************************************
功能：通道打开后初始化，同时还对系统数据进行初始化
***********************************************************************/
void	ResetChannel(DJ_S32  mDspID,DeviceID_t	*pDevice)
{
	long	mIndex;
	DEVICE_TYPE		mDeviceType;
	if(pDevice->m_s16DeviceMain==XMS_DEVMAIN_VOICE)
	{
		mDeviceType=DEVICE_VOICE;
		mIndex=FindIndexByChannelID(mDspID,pDevice->m_s16ChannelID,mDeviceType);
		mDspID=aryVoiceIndex[mIndex].fDspNo;
		mIndex=aryVoiceIndex[mIndex].fIndex;
		strcpy(aryDspData[mDspID].pVoiceDevice[mIndex].fDtmfBuff,"");
		aryDspData[mDspID].pVoiceDevice[mIndex].fStatusCode=VOC_WAIT_WORK;
		XMS_ctsResetDevice(pFlowHandle,pDevice,NULL);
		XMS_ctsGetDevState(pFlowHandle,pDevice,NULL);
	}
	if(pDevice->m_s16DeviceMain==XMS_DEVMAIN_INTERFACE_CH)
	{
		mDeviceType=DEVICE_TRUNK;
		mIndex=FindIndexByChannelID(mDspID,pDevice->m_s16ChannelID,mDeviceType);
		mIndex=aryTrunkIndex[mIndex].fIndex;
		mDspID=aryTrunkIndex[mIndex].fDspNo;
		strcpy(aryDspData[mDspID].pTrunkDevice[mIndex].fDtmfBuff,"");
		aryDspData[mDspID].pTrunkDevice[mIndex].fStatusCode=VOC_WAIT_WORK;
		XMS_ctsResetDevice(pFlowHandle,pDevice,NULL);
		XMS_ctsGetDevState(pFlowHandle,pDevice,NULL);
	}
	if(pDevice->m_s16DeviceMain==XMS_DEVMAIN_VOIP)
	{
		mDeviceType=DEVICE_VOIP;
		mIndex=FindIndexByChannelID(mDspID,pDevice->m_s16ChannelID,mDeviceType);
		mIndex=aryVoipIndex[mIndex].fIndex;
		mDspID=aryVoipIndex[mIndex].fDspNo;
		strcpy(aryDspData[mDspID].pVoipDevice[mIndex].fDtmfBuff,"");
		aryDspData[mDspID].pVoipDevice[mIndex].fStatusCode=VOIP_WAIT_WORK;
		XMS_ctsResetDevice(pFlowHandle,&aryDspData[mDspID].pVoipDevice[mIndex].fDeviceID,NULL);
		XMS_ctsGetDevState(pFlowHandle,&aryDspData[mDspID].pVoipDevice[mIndex].fDeviceID,NULL);
	}
	return;
}

/***********************************************************************
功能：实现对语音设备、通道设备以及VOIP设备进行默认绑定
***********************************************************************/
bool	LinkInitVoice()
{
	int		mDspCount1;
	int		i;
	char	pFileName[200];
	char	pDspCount[10];
	char	mDspType[10];
	char	mDspIndex[10];
	char	mTemp[20];
	int		j;
	RetCode_t	rtCode;
	long	mVoiceIndex;
	char	mInfo[200];


	GetCurrentDirectory(200,pFileName);
	strcat(pFileName,SYSTEM_INI_FILE);
	GetPrivateProfileString("DSP","DspCount","",pDspCount,sizeof(pDspCount),pFileName);
	mDspCount1=atoi(pDspCount);
	for(i=0;i<=mDspCount1;i++)
	{
		strcpy(mDspIndex,"DSP");
		itoa(i,mTemp,10);
		strcat(mDspIndex,mTemp);

		GetPrivateProfileString("DSP",mDspIndex,"",mDspType,sizeof(mDspType),pFileName);
		if(aryDspData[i].fIsExits==1)
		{
		if(strcmp(mDspType,"INTERFACE")==0)				//相等
		{
			for(j=0;j<=aryDspData[i].fTrunkCount;j++)
			{
				/*为语音设备和接口设备建立双向的时隙连接*/
				if(aryDspData[i].pTrunkDevice[j].fIsOpen==true)		/*该设备正常打开*/
				{
					mVoiceIndex=FindFreeVoiceDevice(aryDspData[i].fDspNo);
					//mVoiceIndex=j;		/*固定为他的index*/
					itoa(i,mTemp,10);
					strcpy(mInfo,"DSP:");
					strcat(mInfo,mTemp);
					strcat(mInfo,"   Trunk Channel:");
					itoa(j,mTemp,10);
					strcat(mInfo,mTemp);
					strcat(mInfo," link-->");
					itoa(j,mTemp,10);
					strcat(mInfo,mTemp);
					//AddMsg(mInfo);
					if(mVoiceIndex!=-1)
					{
						
						rtCode=XMS_ctsLinkDevice(pFlowHandle,&aryDspData[i].pTrunkDevice[j].fDeviceID,&aryDspData[i].pVoiceDevice[mVoiceIndex].fDeviceID,NULL);
						if(rtCode<0)
						{
							MessageBox(NULL,"系统底层错误!请检查硬件设备是否正常!!","Error",MB_OK);
							return false;
						}
						rtCode=XMS_ctsLinkDevice(pFlowHandle,&aryDspData[i].pVoiceDevice[mVoiceIndex].fDeviceID,&aryDspData[i].pTrunkDevice[j].fDeviceID,NULL);
						if(rtCode<0)
						{
							MessageBox(NULL,"系统底层错误!请检查硬件设备是否正常!!","Error",MB_OK);
							return false;
						}
						
						/*连接完成后，开始进行数据的同步*/
						strcpy(aryDspData[i].pVoiceDevice[mVoiceIndex].fLocalNumber,aryDspData[i].pTrunkDevice[j].fLocalNumber);
						aryDspData[i].pVoiceDevice[mVoiceIndex].fEnabled=aryDspData[i].pTrunkDevice[j].fEnabled;
						aryDspData[i].pVoiceDevice[mVoiceIndex].fEnabledCallLong=aryDspData[i].pTrunkDevice[j].fEnabledCallLong;
						strcpy(aryDspData[i].pVoiceDevice[mVoiceIndex].fDtmfBuff,"");
						strcpy(aryDspData[i].pTrunkDevice[j].fDtmfBuff,"");
						aryDspData[i].pVoiceDevice[mVoiceIndex].fDtmfNeedLen=0;
						aryDspData[i].pVoiceDevice[mVoiceIndex].fLinkTrunkType=LINK_SLOT;
						aryDspData[i].pVoiceDevice[mVoiceIndex].fStatusCode=VOC_FREE;
						aryDspData[i].pTrunkDevice[j].fStatusCode=VOC_FREE;
						
						aryDspData[i].pTrunkDevice[j].fIsLinkVoice=true;
						aryDspData[i].pVoiceDevice[mVoiceIndex].fIsLinkTrunk=true;
						
						aryDspData[i].pVoiceDevice[mVoiceIndex].fLinkTrunkChannelNo=aryDspData[i].pTrunkDevice[j].fChannelNo;
						aryDspData[i].pTrunkDevice[j].fLinkVoiceChannelNo=aryDspData[i].pVoiceDevice[mVoiceIndex].fChannelNo;
						
						aryDspData[i].pVoiceDevice[mVoiceIndex].fLinkTrunkDeviceID=aryDspData[i].pTrunkDevice[j].fDeviceID;
						aryDspData[i].pTrunkDevice[j].fLinkVoiceDeviceID=aryDspData[i].pVoiceDevice[mVoiceIndex].fDeviceID;

						aryDspData[i].pTrunkDevice[j].fLinkVoipDeviceNo=-1;		/*后来加的代码，专门用于处理IP的拆线*/
						
						strcpy(aryDspData[i].pVoiceDevice[mVoiceIndex].fCallInNumber,"");
						strcpy(aryDspData[i].pVoiceDevice[mVoiceIndex].fCallOutNumber,"");
						strcpy(aryDspData[i].pTrunkDevice[j].fCallInNumber,"");
						strcpy(aryDspData[i].pTrunkDevice[j].fCallOutNumber,"");
						strcpy(aryDspData[i].pTrunkDevice[j].fDescription,"");
						
						/*对于外线，设置初始化工作参数*/
						if(aryDspData[i].pTrunkDevice[j].fDeviceID.m_s16DeviceSub==XMS_DEVSUB_ANALOG_TRUNK)
						{
							if(!SetTrunkLineInitParameter(&aryDspData[i].pTrunkDevice[j].fDeviceID,&aryDspData[i].pVoiceDevice[mVoiceIndex].fDeviceID))
							{
								MessageBox(NULL,"系统设置外线工作参数失败!!","Error",MB_OK);
								return false;
							}
						}
					}
				}
			}
		}
		if(strcmp(mDspType,"VOIP")==0)
		{
			if(aryDspData[i].fIsExits==1) pVoipDspNo=i;			/*记录可用的voip DSP*/
			if(aryDspData[i].fIsExits==1)
			for(j=0;j<=aryDspData[i].fVoipCount;j++)
			{
				/*建立语音设备和VOIP设备之间的时隙连接*/
				if(aryDspData[i].pVoipDevice[j].fIsOpen==true)
				{
					//mVoiceIndex=FindFreeVoiceDevice(aryDspData[i].fDspNo);
					mVoiceIndex=j;
					itoa(i,mTemp,10);
					strcpy(mInfo,"DSP:");
					strcat(mInfo,mTemp);
					strcat(mInfo,"   Voip Channel:");
					itoa(j,mTemp,10);
					strcat(mInfo,mTemp);
					strcat(mInfo," linke-->");
					itoa(j,mTemp,10);
					strcat(mInfo,mTemp);
					//AddMsg(mInfo);

					if(mVoiceIndex!=-1)
					{
/*						rtCode=XMS_ctsLinkDevice(pFlowHandle,&aryDspData[i].pVoiceDevice[mVoiceIndex].fDeviceID,&aryDspData[i].pVoipDevice[j].fDeviceID,NULL);
						if(rtCode<0)       
						{
							MessageBox(NULL,"系统底层错误!请检查硬件设备是否正常!!","Error",MB_OK);
							return false;
						}
*/
						rtCode=XMS_ctsLinkDevice(pFlowHandle,&aryDspData[i].pVoipDevice[j].fDeviceID,&aryDspData[i].pVoiceDevice[mVoiceIndex].fDeviceID,NULL);
						if(rtCode<0)
						{
							MessageBox(NULL,"系统底层错误!请检查硬件设备是否正常!!","Error",MB_OK);
							return false;
						}
						/*连接完成后，开始进行数据同步*/
						strcpy(aryDspData[i].pVoiceDevice[mVoiceIndex].fLocalNumber,aryDspData[i].pVoipDevice[j].fLocalNumber);
						aryDspData[i].pVoiceDevice[mVoiceIndex].fEnabled=aryDspData[i].pVoipDevice[j].fEnabled;
						strcpy(aryDspData[i].pVoiceDevice[mVoiceIndex].fDtmfBuff,"");
						strcpy(aryDspData[i].pVoipDevice[j].fDtmfBuff,"");
						aryDspData[i].pVoiceDevice[mVoiceIndex].fDtmfNeedLen=0;
						aryDspData[i].pVoiceDevice[mVoiceIndex].fLinkVoipDeviceID=aryDspData[i].pVoipDevice[j].fDeviceID;
						aryDspData[i].pVoipDevice[j].fLinkVoiceDeviceID=aryDspData[i].pVoiceDevice[mVoiceIndex].fDeviceID;
						aryDspData[i].pVoiceDevice[mVoiceIndex].fLinkVoipType=LINK_SLOT;
						aryDspData[i].pVoipDevice[j].fLinkVoiceType=LINK_SLOT;
						aryDspData[i].pVoiceDevice[mVoiceIndex].fStatusCode=VOC_FREE;
						aryDspData[i].pVoipDevice[j].fStatusCode=VOIP_FREE;
						strcpy(aryDspData[i].pVoiceDevice[mVoiceIndex].fCallInNumber,"");
						strcpy(aryDspData[i].pVoiceDevice[mVoiceIndex].fCallOutNumber,"");
						aryDspData[i].pVoiceDevice[mVoiceIndex].fIsLinkVoip=true;
						aryDspData[i].pVoipDevice[j].fIsLinkVoice=true;
						aryDspData[i].pVoiceDevice[mVoiceIndex].fLinkVoipChannelNo=aryDspData[i].pVoipDevice[j].fChannelNo;
						aryDspData[i].pVoipDevice[j].fLinkVoiceChannelNo=aryDspData[i].pVoiceDevice[mVoiceIndex].fChannelNo;
						SetVoipDeviceState(&aryDspData[i].pVoipDevice[j].fDeviceID,VOIP_FREE);
						XMS_ctsResetDevice(pFlowHandle,&aryDspData[i].pVoipDevice[j].fDeviceID,NULL);
						XMS_ctsGetDevState(pFlowHandle,&aryDspData[i].pVoipDevice[j].fDeviceID,NULL);
					}
				}
			}
		}
		}
	}
	MessageBox(pMainDlg->m_hWnd,"启动服务成功!!!","Info",MB_OK);
	DisplayStatusBar("          系统工作中.服务已启动");
	fIsRun=true;
	return true;
}



/***********************************************************************
功能：将通道信息写入数据库，用于第一次初始化
***********************************************************************/
bool	InitLineDbRecord(long mDeviceNo,DJ_S32 mDspID,DeviceID_t *pDevice)
{
	char		mSQL[500];
	char		mTemp[10];
	CDBProcess	objDb;
	char 			mMainType[30];
	char 			mSubType[30];

	if(!objDb.ConnectDB())
	{
		MessageBox(NULL,"系统连接数据库失败!!","Error",MB_OK);
		return false;
	}


	strcpy(mMainType,"");
	strcpy(mSubType,"");
	getInterfaceMainType(pDevice->m_s16DeviceMain,mMainType);
	getInterfaceSubType(pDevice->m_s16DeviceSub,mSubType);

	strcpy(mSQL,"insert into objTrunk(fdeviceno,fdspno,fchannelno,fmaintype,fsubtype) values(");
	strcat(mSQL,"'");
	itoa(mDeviceNo,mTemp,10);
	strcat(mSQL,mTemp);
	strcat(mSQL,"','");
	itoa(mDspID,mTemp,10);
	strcat(mSQL,mTemp);
	strcat(mSQL,"','");
	itoa(pDevice->m_s16ChannelID,mTemp,10);
	strcat(mSQL,mTemp);
	strcat(mSQL,"','");
	strcat(mSQL,mMainType);
	strcat(mSQL,"','");
	strcat(mSQL,mSubType);
	strcat(mSQL,"')");
	if(!objDb.ExcueteSQL(mSQL))
	{
		MessageBox(NULL,"系统更新数据失败!!","Error",MB_OK);
		return false;
	}
	return true;
}

/***********************************************************************
功能：关闭所有设备，程序退出时候调用
***********************************************************************/
void	CloseAllDevice()
{
	int		i;
	int		j;
	for(i=0;i<=mDspCount;i++)
	{
		/*关闭voip设备*/
		if(aryDspData[i].fIsExits==1)
		{
			for(j=0;j<=aryDspData[i].fVoipCount;j++)
			{
				XMS_ctsResetDevice(pFlowHandle,&aryDspData[i].pVoipDevice[j].fDeviceID,NULL);
				XMS_ctsCloseDevice(pFlowHandle,&aryDspData[i].pVoipDevice[j].fDeviceID,NULL);
			}
			delete []aryDspData[i].pVoipDevice;
			
			/*关闭接口设备*/
			for(j=0;j<=aryDspData[i].fTrunkCount;j++)
			{
				XMS_ctsResetDevice(pFlowHandle,&aryDspData[i].pTrunkDevice[j].fDeviceID,NULL);	
				XMS_ctsCloseDevice(pFlowHandle,&aryDspData[i].pTrunkDevice[j].fDeviceID,NULL);
			}
			delete []aryDspData[i].pTrunkDevice;
			
			/*关闭语音设备*/
			for(j=0;j<=aryDspData[i].fVoiceCount;j++)
			{
				XMS_ctsResetDevice(pFlowHandle,&aryDspData[i].pVoiceDevice[j].fDeviceID,NULL);
				XMS_ctsCloseDevice(pFlowHandle,&aryDspData[i].pVoiceDevice[j].fDeviceID,NULL);
			}
			delete []aryDspData[i].pVoiceDevice;
		}
	}
	return;
}

/***********************************************************************
功能：从数据库中读取各个线路的配置信息，包括：号码，是否开通长途等
注意：本函数是根据通道的全局设备码和类型进行读取
***********************************************************************/
bool	getTrunkInfoFromDb(long mDeviceNo,DJ_S32 mDspNo,ChannelID_t mChannelID,LINE_TYPE mLineType)
{
	char	mSQL[300];
	CDBProcess	objDb;
	_RecordsetPtr	objList;
	int		mIndex;
	char	mTemp[10];
	long	mChannelNo;

	/*读取外线数据*/
	if(mLineType==LINE_OUT)
	{
		strcpy(mSQL,"select * from objTrunkLine where fLineNo='");
		itoa(mDeviceNo,mTemp,10);
		strcat(mSQL,mTemp);
		strcat(mSQL,"'");
		strcat(mSQL,"order by fDspNo,fChannelNo");
		objList.CreateInstance(__uuidof(Recordset));
		if(!objDb.ConnectDB())
		{
			MessageBox(NULL,"系统读取数据库失败!!","Error",MB_OK);
			return false;
		}
		if(!objDb.ExcueteQuery(mSQL))
		{
			MessageBox(NULL,"系统读取数据库失败!!","Error",MB_OK);
			objDb.CloseDB();
			return false;
		}
		if(!objDb.pRs->EndOfFile)
		{
			mIndex=FindIndexByChannelNo(mDspNo,mChannelID,DEVICE_TRUNK);
			mDspNo=aryTrunkIndex[mIndex].fDspNo;
			mIndex=aryTrunkIndex[mIndex].fIndex;
			_variant_t 		mTemp;
			char			mValue[20];
			mTemp=(_variant_t)objDb.pRs->GetCollect("fLineNo");
			if(mTemp.vt!=VT_NULL)
				strcpy(mValue,(char *)_bstr_t(mTemp));
			else
				strcpy(mValue,"");
			if(strcmp(mValue,"")!=0)
				mChannelNo=atoi(mValue);
			else
				mChannelNo=-1;
			mIndex=FindIndexByChannelNo(mDspNo,mChannelNo,DEVICE_TRUNK);
			mDspNo=aryTrunkIndex[mIndex].fDspNo;
			mIndex=aryTrunkIndex[mIndex].fIndex;

			mTemp=(_variant_t)objDb.pRs->GetCollect("fPhoneNumber");
			if(mTemp.vt!=VT_NULL)
				strcpy(mValue,(char *)_bstr_t(mTemp));
			else
				strcpy(mValue,"");
			strcpy(aryDspData[mDspNo].pTrunkDevice[mIndex].fLocalNumber,mValue);
			mTemp=(_variant_t)objDb.pRs->GetCollect("fIsEnable");
			if(mTemp.vt!=VT_NULL)
				strcpy(mValue,(char *)_bstr_t(mTemp));
			else
				strcpy(mValue,"");
			if(strcmp(mValue,"启用")==0)
				aryDspData[mDspNo].pTrunkDevice[mIndex].fEnabled=true;
			else
				aryDspData[mDspNo].pTrunkDevice[mIndex].fEnabled=false;
		}

		objDb.CloseDB();
	}
	/*读取内线数据*/
	if(mLineType==LINE_IN)
	{
		strcpy(mSQL,"select * from objuserline where fLineNo=");
		strcat(mSQL,"'");
		itoa(mDeviceNo,mTemp,10);
		strcat(mSQL,mTemp);
		strcat(mSQL,"'");
		if(!objDb.ConnectDB())
		{
			MessageBox(NULL,"系统读取数据库失败!!","Error",MB_OK);
			return false;
		}
		if(!objDb.ExcueteQuery(mSQL))
		{
			MessageBox(NULL,"系统读取数据库失败!!","Error",MB_OK);
			objDb.CloseDB();
			return false;
		}
		if(!objDb.pRs->EndOfFile)
		{
			_variant_t 		mTemp;
			char			mValue[20];
			mTemp=(_variant_t)objDb.pRs->GetCollect("fLineNo");
			if(mTemp.vt!=VT_NULL)
				strcpy(mValue,(char *)_bstr_t(mTemp));
			else
				strcpy(mValue,"");
			if(strcmp(mValue,"")!=0)
				mChannelNo=atoi(mValue);
			else
				mChannelNo=-1;
			mIndex=FindIndexByChannelNo(mDspNo,mChannelNo,DEVICE_TRUNK);
			mDspNo=aryTrunkIndex[mIndex].fDspNo;
			mIndex=aryTrunkIndex[mIndex].fIndex;
			mTemp=(_variant_t)objDb.pRs->GetCollect("fPhoneNumber");
			if(mTemp.vt!=VT_NULL)
				strcpy(mValue,(char *)_bstr_t(mTemp));
			else
				strcpy(mValue,"");

			strcpy(aryDspData[mDspNo].pTrunkDevice[mIndex].fLocalNumber,mValue);
			mTemp=(_variant_t)objDb.pRs->GetCollect("fIsEnable");
			if(mTemp.vt!=VT_NULL)
				strcpy(mValue,(char *)_bstr_t(mTemp));
			else
				strcpy(mValue,"");
			if(strcmp(mValue,"启用")==0)
				aryDspData[mDspNo].pTrunkDevice[mIndex].fEnabled=true;
			else
				aryDspData[mDspNo].pTrunkDevice[mIndex].fEnabled=false;
			mTemp=(_variant_t)objDb.pRs->GetCollect("fLongEnable");
			if(mTemp.vt!=VT_NULL)
				strcpy(mValue,(char *)_bstr_t(mTemp));
			else
				strcpy(mValue,"");
			if(strcmp(mValue,"开通")==0)
				aryDspData[mDspNo].pTrunkDevice[mIndex].fEnabledCallLong=true;
			else
				aryDspData[mDspNo].pTrunkDevice[mIndex].fEnabledCallLong=false;

			mTemp=(_variant_t)objDb.pRs->GetCollect("fSpeedDial");
			if(mTemp.vt!=VT_NULL)
				strcpy(mValue,(char *)_bstr_t(mTemp));
			else
				strcpy(mValue,"");
			if(strcmp(mValue,"")!=0)
				strcpy(aryDspData[mDspNo].pTrunkDevice[mIndex].fSpeedDial,mValue);
			else
				strcpy(aryDspData[mDspNo].pTrunkDevice[mIndex].fSpeedDial,"");

			mTemp=(_variant_t)objDb.pRs->GetCollect("fTargetUser");
			if(mTemp.vt!=VT_NULL)
				strcpy(mValue,(char *)_bstr_t(mTemp));
			else
				strcpy(mValue,"");
			if(strcmp(mValue,"")!=0)
				strcpy(aryDspData[mDspNo].pTrunkDevice[mIndex].fTargetUserNo,mValue);
			else
				strcpy(aryDspData[mDspNo].pTrunkDevice[mIndex].fTargetUserNo,"");
		}

		objDb.CloseDB();
	}
	return true;
}

/***********************************************************************
功能：根据全局设备编号，显示相应的信息 
注意：mChannelNo为全局设备编号
***********************************************************************/
void	DrawOneItem(DJ_S32 mDspNo,ChannelID_t mChannelNo,DEVICE_TYPE	mDeviceType)
{
	long	mRow;		/*在列表控件中的行号*/
	long	mIndex;		/*索引数组的下标*/
	long	mDeviceIndex;	/*相应的设备结构数组下标*/
	char    mInfo[100];
	char	mTemp[100];
	CListCtrl *pControl;

	pControl=(CListCtrl *)pMainDlg->GetDlgItem(IDC_LIST_STATUS);
	if(mDeviceType==DEVICE_VOICE)
	{
		mIndex=FindArrayIndexByChannelID(mDspNo,mChannelNo,mDeviceType)	;
		if(mIndex==-1)	return;
		mRow=aryVoiceIndex[mIndex].fRowNo;
		mDspNo=aryVoiceIndex[mIndex].fDspNo;
		mDeviceIndex=aryVoiceIndex[mIndex].fIndex;
		strcpy(mInfo,GetVoiceStatusString(aryDspData[mDspNo].pVoiceDevice[mDeviceIndex].fStatusCode));
		itoa(aryVoiceIndex[mIndex].fChannelNo,mTemp,10);
		pControl->SetItemText(mRow,0,mTemp);
		pControl->SetItemText(mRow,1,mInfo);
		itoa(aryDspData[mDspNo].pVoiceDevice[mDeviceIndex].fLineState,mTemp,10);
		pControl->SetItemText(mRow,2,mTemp);
		itoa(aryVoiceIndex[mIndex].fDspNo,mTemp,10);
		pControl->SetItemText(mRow,3,mTemp);
		itoa(aryVoiceIndex[mIndex].fChannelID_t,mTemp,10);
		pControl->SetItemText(mRow,4,mTemp);
		pControl->SetItemText(mRow,5,"语音设备");
		pControl->SetItemText(mRow,6,(LPCSTR)aryDspData[mDspNo].pVoiceDevice[mDeviceIndex].fLocalNumber);
		pControl->SetItemText(mRow,8,(LPCSTR)aryDspData[mDspNo].pVoiceDevice[mDeviceIndex].fCallOutNumber);
		pControl->SetItemText(mRow,7,(LPCSTR)aryDspData[mDspNo].pVoiceDevice[mDeviceIndex].fCallInNumber);
		pControl->SetItemText(mRow,9,(LPCSTR)aryDspData[mDspNo].pVoiceDevice[mDeviceIndex].fDtmfBuff);
	}

	if(mDeviceType==DEVICE_TRUNK)
	{
		mIndex=FindArrayIndexByChannelID(mDspNo,mChannelNo,mDeviceType);
		if(mIndex==-1)return;
		mRow=aryTrunkIndex[mIndex].fRowNo;
		mDspNo=aryTrunkIndex[mIndex].fDspNo;
		mDeviceIndex=aryTrunkIndex[mIndex].fIndex;
		strcpy(mInfo,"DSP:");
		itoa(mDspNo,mTemp,10);
		strcat(mInfo,mTemp);
		strcat(mInfo,"    TRUNK:");
		itoa(aryTrunkIndex[mIndex].fChannelNo,mTemp,10);
		strcat(mInfo,mTemp);
		strcat(mInfo," SysID:");
		itoa(aryTrunkIndex[mIndex].fChannelID_t,mTemp,10);
		strcat(mInfo,mTemp);
		strcat(mInfo," LineState:");
		strcpy(mTemp,GetLineStatusString(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLineState));
		strcat(mInfo,mTemp);
//		AddMsg(mInfo);
		strcpy(mInfo,GetVoiceStatusString(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode));
		itoa(aryTrunkIndex[mIndex].fChannelNo,mTemp,10);
		pControl->SetItemText(mRow,0,mTemp);
		pControl->SetItemText(mRow,1,mInfo);
		strcpy(mTemp,GetLineStatusString(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLineState));
		pControl->SetItemText(mRow,2,mTemp);
		itoa(aryTrunkIndex[mIndex].fDspNo,mTemp,10);
		pControl->SetItemText(mRow,3,mTemp);
		itoa(aryTrunkIndex[mIndex].fChannelID_t,mTemp,10);
		pControl->SetItemText(mRow,4,mTemp);
		if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLineType==LINE_IN)
			pControl->SetItemText(mRow,5,"内线");
		if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLineType==LINE_OUT)
			pControl->SetItemText(mRow,5,"外线");
		if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLineType==LINE_NULL)
			pControl->SetItemText(mRow,5,"空置");
		pControl->SetItemText(mRow,6,(LPCSTR)aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLocalNumber);
		pControl->SetItemText(mRow,7,(LPCSTR)aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fCallOutNumber);
		pControl->SetItemText(mRow,8,(LPCSTR)aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fCallInNumber);
		pControl->SetItemText(mRow,9,(LPCSTR)aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff);
		pControl->SetItemText(mRow,10,(LPCSTR)aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDescription);
	}

	if(mDeviceType==DEVICE_VOIP)
	{
		mIndex=FindArrayIndexByChannelID(mDspNo,mChannelNo,mDeviceType);
		if(mIndex==-1)return;
		mRow=aryVoipIndex[mIndex].fRowNo;
		mDspNo=aryVoipIndex[mIndex].fDspNo;
		mDeviceIndex=aryVoipIndex[mIndex].fIndex;
		strcpy(mInfo,GetVoipStatusString(aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fStatusCode));
		itoa(aryVoipIndex[mIndex].fChannelNo,mTemp,10);
		pControl->SetItemText(mRow,0,mTemp);
		pControl->SetItemText(mRow,1,mInfo);
		strcpy(mInfo,GetLineStatusString(aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fLineState));
		pControl->SetItemText(mRow,2,mInfo);
		itoa(aryVoipIndex[mIndex].fDspNo,mTemp,10);
		pControl->SetItemText(mRow,3,mTemp);
		itoa(aryVoipIndex[mIndex].fChannelID_t,mTemp,10);
		pControl->SetItemText(mRow,4,mTemp);
		pControl->SetItemText(mRow,5,"VOIP设备");
		pControl->SetItemText(mRow,6,(LPCSTR)aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fLocalNumber);
		pControl->SetItemText(mRow,7,(LPCSTR)aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fCallOutNumber);
		pControl->SetItemText(mRow,8,(LPCSTR)aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fCallInNumber);
		pControl->SetItemText(mRow,9,(LPCSTR)aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fDtmfBuff);
	}
	/*刷新状态栏*/
	DisplayStatusBar();
	return;
}

/***********************************************************************
功能:根据类型，获取相应的类型字符串
***********************************************************************/
void	getInterfaceMainType(short mType,char *pType)
{
	char	mMainType[30];
	switch(mType)
	{
	case	XMS_DEVMAIN_VOICE:
		strcpy(mMainType,"DEVMAIN_VOICE");
		break;
	case	XMS_DEVMAIN_FAX:
		strcpy(mMainType,"DEVMAIN_FAX");
		break;
	case	XMS_DEVMAIN_DIGITAL_PORT:
		strcpy(mMainType,"DEVMAIN_DIGITAL_PORT");
		break;
	case	XMS_DEVMAIN_INTERFACE_CH:
		strcpy(mMainType,"DEVMAIN_INTERFACE_CH");
		break;
	case	XMS_DEVMAIN_DSS1_LINK:
		strcpy(mMainType,"DEVMAIN_DSS1_LINK");
		break;
	case	XMS_DEVMAIN_SS7_LINK:
		strcpy(mMainType,"DEVMAIN_SS7_LINK");
		break;
	case	XMS_DEVMAIN_BOARD:
		strcpy(mMainType,"DEVMAIN_BOARD");
		break;
	case	XMS_DEVMAIN_CTBUS_TS:
		strcpy(mMainType,"DEVMAIN_CTBUS_TS");
		break;
	case	XMS_DEVMAIN_VOIP:
		strcpy(mMainType,"DEVMAIN_VOIP");
		break;
	case	XMS_DEVMAIN_CONFERENCE:
		strcpy(mMainType,"DEVMAIN_CONFERENCE");
		break;
	case	XMS_DEVMAIN_VIDEO:
		strcpy(mMainType,"DEVMAIN_VIDEO");
		break;
	}
	strcpy(pType,mMainType);
	return;

}

/***********************************************************************
功能:根据子类型，获取相应的类型字符串
***********************************************************************/
void	getInterfaceSubType(short mType,char *pType)
{
	char	mSubType[30];
	switch(mType)
	{
	case XMS_DEVSUB_BASE:
		strcpy(mSubType,"DEVSUB_BASE");
		break;
	case XMS_DEVSUB_ANALOG_TRUNK:
		strcpy(mSubType,"DEVSUB_ANALOG_TRUNK");
		break;
	case XMS_DEVSUB_ANALOG_USER:
		strcpy(mSubType,"DEVSUB_ANALOG_USER");
		break;
	case XMS_DEVSUB_ANALOG_HIZ:
		strcpy(mSubType,"DEVSUB_ANALOG_HIZ");
		break;
	case XMS_DEVSUB_ANALOG_EMPTY:
		strcpy(mSubType,"DEVSUB_ANALOG_EMPTY");
		break;
	case XMS_DEVSUB_E1_PCM:
		strcpy(mSubType,"DEVSUB_E1_PCM");
		break;
	case XMS_DEVSUB_E1_CAS:
		strcpy(mSubType,"DEVSUB_E1_CAS");
		break;
	case XMS_DEVSUB_E1_DSS1:
		strcpy(mSubType,"DEVSUB_E1_DSS1");
		break;
	case XMS_DEVSUB_E1_SS7_TUP:
		strcpy(mSubType,"DEVSUB_E1_SS7_TUP");
		break;
	case XMS_DEVSUB_E1_SS7_ISUP:
		strcpy(mSubType,"DEVSUB_E1_SS7_ISUP");
		break;
	case XMS_DEVSUB_ANALOG_VOC2W:
		strcpy(mSubType,"DEVSUB_ANALOG_VOC2W");
		break;
	case XMS_DEVSUB_ANALOG_VOC4W:
		strcpy(mSubType,"DEVSUB_ANALOG_VOC4W");
		break;
	case XMS_DEVSUB_ANALOG_EM:
		strcpy(mSubType,"DEVSUB_ANALOG_EM");
		break;
	case XMS_DEVSUB_ANALOG_MAG:
		strcpy(mSubType,"DEVSUB_ANALOG_MAC");
		break;
	case XMS_DEVSUB_E1_DCH:
		strcpy(mSubType,"DEVSUB_E1_DCH");
		break;
	case XMS_DEVSUB_E1_BCH:
		strcpy(mSubType,"DEVSUB_E1_BCH");
		break;
	case XMS_DEVSUB_UNUSABLE:
		strcpy(mSubType,"DEVSUB_UNUSABLE");
		break;
	}
	strcpy(pType,mSubType);
	return;
}


/***********************************************************************
功能：根据DSP编号和ChannelID来获取该设备的全局设备编号，如果存在，则表示已经初始化
过，如果不存在，则表示要更新到数据库
***********************************************************************/
long	getDeviceNoByChannelID(long mDspNo,ChannelID_t	mChannelID)
{
	char 		mSQL[300];
	char 		mTemp[10];
	CDBProcess	objDb;
	long		mDeviceNo;

	mDeviceNo=-1;
	strcpy(mSQL,"select * from objTrunk where fdspno='");
	itoa(mDspNo,mTemp,10);
	strcat(mSQL,mTemp);
	strcat(mSQL,"' and fchannelno='");
	itoa(mChannelID,mTemp,10);
	strcat(mSQL,mTemp);
	strcat(mSQL,"'");
	if(!objDb.ConnectDB())
	{
		MessageBox(NULL,"系统连接数据库失败!!","Error",MB_OK);
		return mDeviceNo;
	}
	if(!objDb.ExcueteQuery(mSQL))
	{
		MessageBox(NULL,"系统查询数据失败!!","Error",MB_OK);
		return mDeviceNo;
	}
	if(!objDb.pRs->EndOfFile)
	{
		try
		{
			mDeviceNo=atoi((_bstr_t)(objDb.pRs->GetCollect("fdeviceno")));
		}
		catch (_com_error e)
		{
			CString errormessage; 
			errormessage.Format("连接数据库失败!\r\n错误信息:%s",e.ErrorMessage()); 
			AfxMessageBox(errormessage);
		}
	}
	else
		mDeviceNo=-2;
	objDb.CloseDB();
	return mDeviceNo;
}

/***********************************************************************
功能：将内外线数据写入数据库，并进行同步
1:查询该通道是否已经存在于数据库
2:如果不存在则插入，存在则更新他的状态
***********************************************************************/
bool	UpdateLineStatus(long mDeviceNo,long mDspNo,ChannelID_t mChannelID,short mType)
{
	char		mSQL[200];
	char		mTemp[10];
	CDBProcess	objDb;

	/*外线的处理*/
	if(mType==XMS_DEVSUB_ANALOG_TRUNK)
	{
		strcpy(mSQL,"");
		strcpy(mSQL,"select * from objTrunkLine where fdspno='");
		itoa(mDspNo,mTemp,10);
		strcat(mSQL,mTemp);
		strcat(mSQL,"' and fchannelno='");
		itoa(mChannelID,mTemp,10);
		strcat(mSQL,mTemp);
		strcat(mSQL,"'");
		if(!objDb.ConnectDB())
		{
			MessageBox(NULL,"系统连接数据库失败!!","Error",MB_OK);
			return false;
		}
		if(!objDb.ExcueteQuery(mSQL))
		{
			MessageBox(NULL,"系统连接数据库失败!!","Error",MB_OK);
			return false;
		}
		if (objDb.pRs->EndOfFile)
		{
			strcpy(mSQL,"Insert into objTrunkLine(fLineNo,fDspNo,fChannelNo) values(");
			strcat(mSQL,"'");
			itoa(mDeviceNo,mTemp,10);
			strcat(mSQL,mTemp);
			strcat(mSQL,"','");
			itoa(mDspNo,mTemp,10);
			strcat(mSQL,mTemp);
			strcat(mSQL,"','");
			itoa(mChannelID,mTemp,10);
			strcat(mSQL,mTemp);
			strcat(mSQL,"')");
			if(!objDb.ExcueteSQL(mSQL))
			{
				MessageBox(NULL,"系统更新数据失败!!","Error",MB_OK);
				return false;
		}
		}
	}
	/*内线的处理*/
	if(mType==XMS_DEVSUB_ANALOG_USER)
	{
		strcpy(mSQL,"select * from objUserLine where fdspno='");
		itoa(mDspNo,mTemp,10);
		strcat(mSQL,mTemp);
		strcat(mSQL,"' and fchannelno='");
		itoa(mChannelID,mTemp,10);
		strcat(mSQL,mTemp);
		strcat(mSQL,"'");
		if(!objDb.ConnectDB())
		{
			MessageBox(NULL,"系统连接数据库失败!!","Error",MB_OK);
			return false;
		}
		if(!objDb.ExcueteQuery(mSQL))
		{
			MessageBox(NULL,"系统连接数据库失败!!","Error",MB_OK);
			return false;
		}
		if(objDb.pRs->EndOfFile)
		{
			strcpy(mSQL,"Insert into objUserLine(fLineNo,fDspNo,fChannelNo) values(");
			strcat(mSQL,"'");
			itoa(mDeviceNo,mTemp,10);
			strcat(mSQL,mTemp);
			strcat(mSQL,"','");
			itoa(mDspNo,mTemp,10);
			strcat(mSQL,mTemp);
			strcat(mSQL,"','");
			itoa(mChannelID,mTemp,10);
			strcat(mSQL,mTemp);
			strcat(mSQL,"')");
			if(!objDb.ExcueteSQL(mSQL))
			{
				MessageBox(NULL,"系统更新数据失败!!","Error",MB_OK);
				return false;
			}
		}
	}
	return true;
}

/***********************************************************************
功能：调试函数，显示所有的内存变量
***********************************************************************/
void	DebugOutput()
{
	char	mSQL[1000];
	long	i;
	char	mTemp[30];
	CDBProcess	objDb;
	long	j;

	if(!objDb.ConnectDB()) return;
	/*输出trunkindex*/
	for(i=0;i<MAX_INTERFACE_COUNT;i++)
	{
		strcpy(mSQL,"insert into objDebugIndex(fchannelno,fdspno,findex,fchannel_id_t,fdevicetype,");
		strcat(mSQL,"frowno,ftype) values('");
		itoa(aryTrunkIndex[i].fChannelNo,mTemp,10);
		strcat(mSQL,mTemp);
		strcat(mSQL,"','");
		itoa(aryTrunkIndex[i].fDspNo,mTemp,10);
		strcat(mSQL,mTemp);
		strcat(mSQL,"','");
		itoa(aryTrunkIndex[i].fIndex,mTemp,10);
		strcat(mSQL,mTemp);
		strcat(mSQL,"','");
		itoa(aryTrunkIndex[i].fChannelID_t,mTemp,10);
		strcat(mSQL,mTemp);
		strcat(mSQL,"','");
		strcat(mSQL,"DEVICE_TRUNK','");
		itoa(aryTrunkIndex[i].fRowNo,mTemp,10);
		strcat(mSQL,mTemp);
		strcat(mSQL,"','TRUNK')");
		if(!objDb.ExcueteSQL(mSQL))	return ;
	}

	/*输出voice index*/
	for(i=0;i<MAX_VOICE_COUNT;i++)
	{
		strcpy(mSQL,"insert into objDebugIndex(fchannelno,fdspno,findex,fchannel_id_t,fdevicetype,");
		strcat(mSQL,"frowno,ftype) values('");
		itoa(aryVoiceIndex[i].fChannelNo,mTemp,10);
		strcat(mSQL,mTemp);
		strcat(mSQL,"','");
		itoa(aryVoiceIndex[i].fDspNo,mTemp,10);
		strcat(mSQL,mTemp);
		strcat(mSQL,"','");
		itoa(aryVoiceIndex[i].fIndex,mTemp,10);
		strcat(mSQL,mTemp);
		strcat(mSQL,"','");
		itoa(aryVoiceIndex[i].fChannelID_t,mTemp,10);
		strcat(mSQL,mTemp);
		strcat(mSQL,"','");
		strcat(mSQL,"DEVICE_VOICE','");
		itoa(aryVoiceIndex[i].fRowNo,mTemp,10);
		strcat(mSQL,mTemp);
		strcat(mSQL,"','VOICE')");
		if(!objDb.ExcueteSQL(mSQL))	return ;
	}

	/*输出voip index*/
	for(i=0;i<MAX_VOIP_COUNT;i++)
	{
		strcpy(mSQL,"insert into objDebugIndex(fchannelno,fdspno,findex,fchannel_id_t,fdevicetype,");
		strcat(mSQL,"frowno,ftype) values('");
		itoa(aryVoipIndex[i].fChannelNo,mTemp,10);
		strcat(mSQL,mTemp);
		strcat(mSQL,"','");
		itoa(aryVoipIndex[i].fDspNo,mTemp,10);
		strcat(mSQL,mTemp);
		strcat(mSQL,"','");
		itoa(aryVoipIndex[i].fIndex,mTemp,10);
		strcat(mSQL,mTemp);
		strcat(mSQL,"','");
		itoa(aryVoipIndex[i].fChannelID_t,mTemp,10);
		strcat(mSQL,mTemp);
		strcat(mSQL,"','");
		strcat(mSQL,"DEVICE_VOIP','");
		itoa(aryVoipIndex[i].fRowNo,mTemp,10);
		strcat(mSQL,mTemp);
		strcat(mSQL,"','VOIP')");
		if(!objDb.ExcueteSQL(mSQL))	return ;
	}
	
	/*输出dsp以及各个通道的数据*/
	for(i=0;i<MAX_DSP_COUNT;i++)
	{
		/*输出dsp数据*/
		if(aryDspData[i].fIsExits==1)
		{
			strcpy(mSQL,"insert into objDebugDsp(fdspno,fdspid,fisexits,fisopen,fdeviceid,fdevicetype,");
			strcat(mSQL,"fvoicecount,ftrunkcount,fvoiceopencount,ftrunkopencount,fvoipcount,fvoipopencount) values('");
			itoa(aryDspData[i].fDspNo,mTemp,10);
			strcat(mSQL,mTemp);
			strcat(mSQL,"','");
			itoa(aryDspData[i].fDspID,mTemp,10);
			strcat(mSQL,mTemp);
			strcat(mSQL,"','");
			if(aryDspData[i].fIsExits==1)
				strcat(mSQL,"存在");
			else
				strcat(mSQL,"不存在");
			strcat(mSQL,"','");
			if(aryDspData[i].fIsOpen==1)
				strcat(mSQL,"打开");
			else
				strcat(mSQL,"未打开");
			strcat(mSQL,"','");
			itoa(aryDspData[i].fDeviceID.m_s16ChannelID,mTemp,10);
			strcat(mSQL,mTemp);
			strcat(mSQL,"',");
			strcat(mSQL,"'DSP','");
			itoa(aryDspData[i].fVoiceCount,mTemp,10);
			strcat(mSQL,mTemp);
			strcat(mSQL,"','");
			itoa(aryDspData[i].fTrunkCount,mTemp,10);
			strcat(mSQL,mTemp);
			strcat(mSQL,"','");
			itoa(aryDspData[i].fVoiceOpenCount,mTemp,10);
			strcat(mSQL,mTemp);
			strcat(mSQL,"','");
			itoa(aryDspData[i].fTrunkOpenCount,mTemp,10);
			strcat(mSQL,mTemp);
			strcat(mSQL,"','");
			itoa(aryDspData[i].fVoipCount,mTemp,10);
			strcat(mSQL,mTemp);
			strcat(mSQL,"','");
			itoa(aryDspData[i].fVoipOpenCount,mTemp,10);
			strcat(mSQL,mTemp);
			strcat(mSQL,"')");
			if(!objDb.ExcueteSQL(mSQL))	return;
		}
		/*输出每个通道的数据*/
		if(aryDspData[i].fIsExits==1)
		{
			for(j=0;j<=aryDspData[i].fTrunkCount;j++)
			{
				strcpy(mSQL,"insert into objDebugTrunk(fchannelno,fisexit,fdeviceid,fdevicetype,fisopen,ferrorflag,");
				strcat(mSQL,"flinetype,fcalloutnumber,fcallinnumber,flocalnumber,fstatuscode,fenabled,fenablecalllong,");
				strcat(mSQL,"fdtmfbuff,fdtmfneedlen,fturnstatuscode,fdtmftimeoutstatuscode,flinkchannelno,flinktype,flinkdeviceid,flinkdevicetype) values(");
				itoa(aryDspData[i].pTrunkDevice[j].fChannelNo,mTemp,10);
				strcat(mSQL,"'");
				strcat(mSQL,mTemp);
				strcat(mSQL,"','");
				if(aryDspData[i].pTrunkDevice[j].fIsExit==1)
					strcat(mSQL,"存在','");
				else
					strcat(mSQL,"不存在','");
				itoa(aryDspData[i].pTrunkDevice[j].fDeviceID.m_s16ChannelID,mTemp,10);
				strcat(mSQL,mTemp);
				strcat(mSQL,"','接口','");
				if(aryDspData[i].pTrunkDevice[j].fIsOpen)
					strcat(mSQL,"已打开','");
				else
					strcat(mSQL,"未打开','");
				if(aryDspData[i].pTrunkDevice[j].fErrorFlag)
					strcat(mSQL,"出现异常','");
				else
					strcat(mSQL,"未见异常','");
				if(aryDspData[i].pTrunkDevice[j].fLineType==LINE_IN)
					strcat(mSQL,"内线','");
				if(aryDspData[i].pTrunkDevice[j].fLineType==LINE_OUT)
					strcat(mSQL,"外线','");
				if(aryDspData[i].pTrunkDevice[j].fLineType==LINE_NULL)
					strcat(mSQL,"空线','");
				strcat(mSQL,aryDspData[i].pTrunkDevice[j].fCallOutNumber);
				strcat(mSQL,"','");
				strcat(mSQL,aryDspData[i].pTrunkDevice[j].fCallInNumber);
				strcat(mSQL,"','");
				strcat(mSQL,aryDspData[i].pTrunkDevice[j].fLocalNumber);
				strcat(mSQL,"','");
				strcat(mSQL,GetVoiceStatusString(aryDspData[i].pTrunkDevice[j].fStatusCode));
				strcat(mSQL,"','");
				if(aryDspData[i].pTrunkDevice[j].fEnabled)
					strcat(mSQL,"允许','");
				else
					strcat(mSQL,"禁止','");
				if(aryDspData[i].pTrunkDevice[j].fEnabledCallLong)
					strcat(mSQL,"允许','");
				else
					strcat(mSQL,"禁止','");
				strcat(mSQL,aryDspData[i].pTrunkDevice[j].fDtmfBuff);
				strcat(mSQL,"','");
				itoa(aryDspData[i].pTrunkDevice[j].fDtmfNeedLen,mTemp,10);
				strcat(mSQL,mTemp);
				strcat(mSQL,"','");
				strcat(mSQL,GetVoiceStatusString(aryDspData[i].pTrunkDevice[j].fTurnStatusCode));
				strcat(mSQL,"','");
				strcat(mSQL,GetVoiceStatusString(aryDspData[i].pTrunkDevice[j].fDtmfTimeOutStatusCode));
				strcat(mSQL,"','");
				itoa(aryDspData[i].pTrunkDevice[j].fLinkTrunkChannelNo,mTemp,10);
				strcat(mSQL,mTemp);
				strcat(mSQL,"','");
				strcat(mSQL,"时隙','");
				itoa(aryDspData[i].pTrunkDevice[j].fLinkTrunkDeviceID.m_s16ChannelID,mTemp,10);
				strcat(mSQL,mTemp);
				strcat(mSQL,"','");
				strcat(mSQL,"接口')");
				if(!objDb.ExcueteSQL(mSQL))	return ;
			}
			/*输出语音设备*/
			for(j=0;j<=aryDspData[i].fVoiceCount;j++)
			{
				strcpy(mSQL,"insert into objDebugVoice(fchannelno,fisexit,fdeviceid,fdevicetype,fisopen,ferrorflag,");
				strcat(mSQL,"fcalloutnumber,fcallinnumber,flocalnumber,fstatuscode,fenable,fenablecalllong,");
				strcat(mSQL,"fdtmfbuff,fdtmfneedlen,fdtmftimeoutstatuscode,flinkchannelno,flinktype,flinkdeviceid,flinkdevicetype) values(");
				itoa(aryDspData[i].pVoiceDevice[j].fChannelNo,mTemp,10);
				strcat(mSQL,"'");
				strcat(mSQL,mTemp);
				strcat(mSQL,"','");
				if(aryDspData[i].pVoiceDevice[j].fIsExit==1)
					strcat(mSQL,"存在','");
				else
					strcat(mSQL,"不存在','");
				itoa(aryDspData[i].pVoiceDevice[j].fDeviceID.m_s16ChannelID,mTemp,10);
				strcat(mSQL,mTemp);
				strcat(mSQL,"','语音','");
				if(aryDspData[i].pVoiceDevice[j].fIsOpen)
					strcat(mSQL,"已打开','");
				else
					strcat(mSQL,"未打开','");
				if(aryDspData[i].pVoiceDevice[j].fErrorFlag)
					strcat(mSQL,"出现异常','");
				else
					strcat(mSQL,"未见异常','");
				strcat(mSQL,aryDspData[i].pVoiceDevice[j].fCallOutNumber);
				strcat(mSQL,"','");
				strcat(mSQL,aryDspData[i].pVoiceDevice[j].fCallInNumber);
				strcat(mSQL,"','");
				strcat(mSQL,aryDspData[i].pVoiceDevice[j].fLocalNumber);
				strcat(mSQL,"','");
				strcat(mSQL,GetVoiceStatusString(aryDspData[i].pTrunkDevice[j].fStatusCode));
				strcat(mSQL,"','");
				if(aryDspData[i].pVoiceDevice[j].fEnabled)
					strcat(mSQL,"允许','");
				else
					strcat(mSQL,"禁止','");
				if(aryDspData[i].pVoiceDevice[j].fEnabledCallLong)
					strcat(mSQL,"允许','");
				else
					strcat(mSQL,"禁止','");
				strcat(mSQL,aryDspData[i].pVoiceDevice[j].fDtmfBuff);
				strcat(mSQL,"','");
				itoa(aryDspData[i].pVoiceDevice[j].fDtmfNeedLen,mTemp,10);
				strcat(mSQL,mTemp);
				strcat(mSQL,"','");
				strcat(mSQL,GetVoiceStatusString(aryDspData[i].pVoiceDevice[j].fDtmfTimeOutStatusCode));
				strcat(mSQL,"','");
				itoa(aryDspData[i].pVoiceDevice[j].fLinkTrunkChannelNo,mTemp,10);
				strcat(mSQL,mTemp);
				strcat(mSQL,"','");
				strcat(mSQL,"时隙','");
				itoa(aryDspData[i].pVoiceDevice[j].fLinkTrunkDeviceID.m_s16ChannelID,mTemp,10);
				strcat(mSQL,mTemp);
				strcat(mSQL,"','");
				strcat(mSQL,"接口')");
				if(!objDb.ExcueteSQL(mSQL))	return ;
			}
			/*输出voip设备*/
			for(j=0;j<=aryDspData[i].fVoipCount;j++)
			{
				strcpy(mSQL,"insert into objDebugVoip(fchannelno,fisexit,fdeviceid,fdevicetype,fisopen,ferrorflag,");
				strcat(mSQL,"fprotocaltype,fsourceaddr,ftargetaddr,fcalloutnumber,fcallinnumber,flocalnumber,fstatuscode,fenabled,");
				strcat(mSQL,"fdtmfbuff,fdtmfneedlen,fturnstatuscode,fdtmftimeoutstatuscode,flinkchannelno,flinktype,flinkdeviceid,flinkdevicetype) values(");
				itoa(aryDspData[i].pVoipDevice[j].fChannelNo,mTemp,10);
				strcat(mSQL,"'");
				strcat(mSQL,mTemp);
				strcat(mSQL,"','");
				if(aryDspData[i].pVoipDevice[j].fIsExit==1)
					strcat(mSQL,"存在','");
				else
					strcat(mSQL,"不存在','");
				itoa(aryDspData[i].pVoipDevice[j].fDeviceID.m_s16ChannelID,mTemp,10);
				strcat(mSQL,mTemp);
				strcat(mSQL,"','VOIP','");
				if(aryDspData[i].pVoipDevice[j].fIsOpen)
					strcat(mSQL,"已打开','");
				else
					strcat(mSQL,"未打开','");
				if(aryDspData[i].pVoipDevice[j].fErrorFlag)
					strcat(mSQL,"出现异常','");
				else
					strcat(mSQL,"未见异常','");
				if(aryDspData[i].pVoipDevice[j].fProtocolType==1)
					strcat(mSQL,"SIP','");
				else
					strcat(mSQL,"H.323','");
				strcat(mSQL,aryDspData[i].pVoipDevice[j].fSourceAddr.tszAddr);
				strcat(mSQL,"','");
				strcat(mSQL,aryDspData[i].pVoipDevice[j].fTargetAddr.tszAddr);
				strcat(mSQL,"','");
				strcat(mSQL,aryDspData[i].pVoipDevice[j].fCallOutNumber);
				strcat(mSQL,"','");
				strcat(mSQL,aryDspData[i].pVoipDevice[j].fCallInNumber);
				strcat(mSQL,"','");
				strcat(mSQL,aryDspData[i].pVoipDevice[j].fLocalNumber);
				strcat(mSQL,"','");
				strcat(mSQL,GetVoipStatusString(aryDspData[i].pVoipDevice[j].fStatusCode));
				strcat(mSQL,"','");
				if(aryDspData[i].pTrunkDevice[j].fEnabled)
					strcat(mSQL,"允许','");
				else
					strcat(mSQL,"禁止','");
				strcat(mSQL,aryDspData[i].pVoipDevice[j].fDtmfBuff);
				strcat(mSQL,"','");
				itoa(aryDspData[i].pVoipDevice[j].fDtmfNeedLen,mTemp,10);
				strcat(mSQL,mTemp);
				strcat(mSQL,"','");
				strcat(mSQL,GetVoipStatusString(aryDspData[i].pVoipDevice[j].fTurnStatusCode));
				strcat(mSQL,"','");
				strcat(mSQL,GetVoipStatusString(aryDspData[i].pVoipDevice[j].fDtmfTimeOutStatusCode));
				strcat(mSQL,"','");
				itoa(aryDspData[i].pVoipDevice[j].fLinkVoiceChannelNo,mTemp,10);
				strcat(mSQL,mTemp);
				strcat(mSQL,"','");
				strcat(mSQL,"时隙','");
				itoa(aryDspData[i].pVoipDevice[j].fLinkVoiceDeviceID.m_s16ChannelID,mTemp,10);
				strcat(mSQL,mTemp);
				strcat(mSQL,"','");
				strcat(mSQL,"语音')");
				pMainDlg->SetDlgItemText(IDC_EDIT1,mSQL);
				if(!objDb.ExcueteSQL(mSQL))	return ;
			}

		}
	}
	MessageBox(NULL,"输出调试信息完毕!!","Info",MB_OK);
	return;
}


void	AddMsg(char * mInfo)
{
	if(pMainDlg->m_ListInfo.GetCount()>1000)
		pMainDlg->m_ListInfo.ResetContent();
	pMainDlg->m_ListInfo.AddString(mInfo);
}


/***********************************************************************
功能：获取语音设备的全局编号
***********************************************************************/
long	getVoiceDeviceNo()
{
	mPubVoiceNo=mPubVoiceNo+1;
	return mPubVoiceNo;
}

/***********************************************************************
功能：获取VOIP设备的全局编号
***********************************************************************/
long	getVoipDeviceNo()
{
	mPubVoipNo=mPubVoipNo+1;
	return mPubVoipNo;
}

/***********************************************************************
功能：获取interface设备的全局设备编号
***********************************************************************/
long	getTrunkDeviceNo()
{
	mPubTrunkNo=mPubTrunkNo+1;
	return mPubTrunkNo;
}

/***********************************************************************
功能：获取线路状态字符串
***********************************************************************/
char *	GetLineStatusString(int mLineState)
{
	switch(mLineState)
	{
	case DCS_FREE:							return "空闲";			break;
	case DCS_LOCAL_BLOCK:					return "本地阻塞";		break;
	case DCS_REMOTE_BLOCK:					return "远端组塞";		break;
	case DCS_BLOCK:							return "本地和远端阻塞";break;
	case DCS_UNAVAILABLE:					return "不可用";		break;
	case DCS_CALLOUT:						return "开始呼出";		break;
	case DCS_CALLIN:						return "呼叫进入";		break;
	case DCS_ALERTING:						return "地址收齐对用户振铃";	break;
	case DCS_CONNECT:						return "通话中";		break;
	case DCS_DISCONNECT:					return "收到拆线";		break;
	case DCS_WAIT_FREE:						return "对拆线的响应";	break;
	default:
		return	"不可用";
	}
}

/***********************************************************************
功能：响应底层的DeviceState事件
***********************************************************************/
void	ProcessDeviceStatusEvt(DeviceID_t	*pDeviceID,int fLineState)
{
	long	mTrunkIndex;
	long	mDspNo;					/*下标*/
	long	mDeviceIndex;

	/*如果是接口通道*/
	if(pDeviceID->m_s16DeviceMain==XMS_DEVMAIN_INTERFACE_CH)
	{
		mDspNo=pDeviceID->m_s8ModuleID;
		mTrunkIndex=FindIndexByChannelID(mDspNo,pDeviceID->m_s16ChannelID,DEVICE_TRUNK);
		mDeviceIndex=aryTrunkIndex[mTrunkIndex].fIndex;
		aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLineState=fLineState;
		if(fLineState==DCS_FREE)
			aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode=VOC_FREE;
		if(pDeviceID->m_s16ChannelID%16==0)
		{
			aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode=VOC_SYS_USE;
			aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLineState=DCS_UNAVAILABLE;
		}
		//mOpenTrunkCount=mOpenTrunkCount+1;
		aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsOpen=true;
		DrawOneItem(mDspNo,aryTrunkIndex[mTrunkIndex].fChannelID_t,DEVICE_TRUNK);
	}
	/*如果设备是语音通道*/
	if(pDeviceID->m_s16DeviceMain==XMS_DEVMAIN_VOICE)
	{
		mDspNo=pDeviceID->m_s8ModuleID;
		mTrunkIndex=FindIndexByChannelID(mDspNo,pDeviceID->m_s16ChannelID,DEVICE_VOICE);
		mDeviceIndex=aryVoiceIndex[mTrunkIndex].fIndex;
		aryDspData[mDspNo].pVoiceDevice[mDeviceIndex].fStatusCode=VOC_FREE;
		aryDspData[mDspNo].pVoiceDevice[mDeviceIndex].fIsOpen=true;
		aryDspData[mDspNo].pVoiceDevice[mDeviceIndex].fLineState=fLineState;
		//mOpenVoiceCount=mOpenVoiceCount+1;
		DrawOneItem(mDspNo,aryVoiceIndex[mTrunkIndex].fChannelID_t,DEVICE_VOICE);
	}
	/*如果是VOIP设备*/
	if(pDeviceID->m_s16DeviceMain==XMS_DEVMAIN_VOIP)
	{
		mDspNo=pDeviceID->m_s8ModuleID;
		mTrunkIndex=FindIndexByChannelID(mDspNo,pDeviceID->m_s16ChannelID,DEVICE_VOIP);
		mDeviceIndex=aryVoipIndex[mTrunkIndex].fIndex;
		aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fIsOpen=true;
		aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fLineState=fLineState;
		//if(fLineState==DCS_FREE)
		//	aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fStatusCode=VOIP_FREE;
		//mOpenVoipCount=mOpenVoipCount+1;
		DrawOneItem(mDspNo,aryVoipIndex[mTrunkIndex].fChannelID_t,DEVICE_VOIP);
	}
	return;
}


/***********************************************************************
功能：打开一个DSP的所有设备
***********************************************************************/
bool	OpenAllDeviceInOneDsp(long	mDspNo)
{
	int		i;
	int		j;	
	RetCode_t		rtCode;

	i=mDspNo;
	/*先打开DSP*/
	if(aryDspData[mDspNo].fIsExits==0) return true;		//设备不存在则直接退出
	rtCode=XMS_ctsOpenDevice(pFlowHandle,&aryDspData[mDspNo].fDeviceID,NULL);
	if(rtCode==-ACSERR_BADHDL)
	{
		MessageBox(NULL,"调用OpenDevice时,流程句柄非法!!","Error",MB_OK);
		return false;
	}
	if(rtCode==-ACSERR_FAILSEND)
	{
		MessageBox(NULL,"调用OpenDevice时,发送命令失败!!请检查流程服务器的命令最大队列数!!","Error",MB_OK);
		return false;
	}
	if(rtCode==-ACSERR_BADPARAMETER)
	{
		MessageBox(NULL,"调用OpenDevice时,设备编号为空!!","Error",MB_OK);
		return false;
	}
	/*打开语音设备*/
	for(j=0;j<=aryDspData[i].fVoiceCount;j++)
	{
		rtCode=XMS_ctsOpenDevice(pFlowHandle,&aryDspData[i].pVoiceDevice[j].fDeviceID,NULL);
		if(rtCode==-ACSERR_BADHDL)
		{
			MessageBox(NULL,"调用OpenDevice时,流程句柄非法!!","Error",MB_OK);
			return false;
		}
		if(rtCode==-ACSERR_FAILSEND)
		{
			MessageBox(NULL,"调用OpenDevice时,发送命令失败!!请检查流程服务器的命令最大队列数!!","Error",MB_OK);
			return false;
		}
		if(rtCode==-ACSERR_BADPARAMETER)
		{
			MessageBox(NULL,"调用OpenDevice时,设备编号为空!!","Error",MB_OK);
			return false;
		}
	}
	/*打开接口设备*/
	for(j=0;j<=aryDspData[i].fTrunkCount;j++)
	{
		rtCode=XMS_ctsOpenDevice(pFlowHandle,&aryDspData[i].pTrunkDevice[j].fDeviceID,NULL);
		if(rtCode==-ACSERR_BADHDL)
		{
			MessageBox(NULL,"调用OpenDevice时,流程句柄非法!!","Error",MB_OK);
			return false;
		}
		if(rtCode==-ACSERR_FAILSEND)
		{
			MessageBox(NULL,"调用OpenDevice时,发送命令失败!!请检查流程服务器的命令最大队列数!!","Error",MB_OK);
			return false;
		}
		if(rtCode==-ACSERR_BADPARAMETER)
		{
			MessageBox(NULL,"调用OpenDevice时,设备编号为空!!","Error",MB_OK);
			return false;
		}
	}
	/*打开VOIP设备*/
	for(j=0;j<=aryDspData[i].fVoipCount;j++)
	{
		rtCode=XMS_ctsOpenDevice(pFlowHandle,&aryDspData[i].pVoipDevice[j].fDeviceID,NULL);
		if(rtCode==-ACSERR_BADHDL)
		{
			MessageBox(NULL,"调用OpenDevice时,流程句柄非法!!","Error",MB_OK);
			return false;
		}
		if(rtCode==-ACSERR_FAILSEND)
		{
			MessageBox(NULL,"调用OpenDevice时,发送命令失败!!请检查流程服务器的命令最大队列数!!","Error",MB_OK);
			return false;
		}
		if(rtCode==-ACSERR_BADPARAMETER)
		{
			MessageBox(NULL,"调用OpenDevice时,设备编号为空!!","Error",MB_OK);
			return false;
		}
	}
	return true;
}


/***********************************************************************
功能：根据全局设备编号，查找相应的index数组的下标
***********************************************************************/
long	FindIndexByChannelNo(DJ_S32 mDspNo,long mDeviceNo,DEVICE_TYPE mDeviceType)
{
	long	mResult;
	bool	mFind;
	long	i;

	mFind=false;
	mResult=-1;
	if(mDeviceType==DEVICE_TRUNK)
	{
		i=0;
		while(i<=MAX_INTERFACE_COUNT && mFind==false)
		{
			if(aryTrunkIndex[i].fChannelNo==mDeviceNo)
			{
				mResult=i;
				mFind=true;
			}
			i++;
		}
	}
	if(mDeviceType==DEVICE_VOICE)
	{
		i=0;
		while(i<=MAX_VOICE_COUNT && mFind==false)
		{
			if(aryVoiceIndex[i].fChannelNo==mDeviceNo)
			{
				mResult=i;
				mFind=true;
			}
			i++;
		}
	}
	if(mDeviceType==DEVICE_VOIP)
	{
		i=0;
		while(i<=MAX_VOIP_COUNT && mFind==false)
		{
			if(aryVoipIndex[i].fChannelNo==mDeviceNo)
			{
				mResult=i;
				mFind=true;
			}
			i++;
		}
	}
	return mResult;
}

/***********************************************************************
功能：调式函数，显示所有的的内存全局变量
***********************************************************************/
void	VerifyData()
{
	long	i;
	char	mTemp[30];
	char	mInfo[200];
	long	mDspNo;
	long	mDeviceIndex;

	itoa(mPubTrunkNo,mTemp,10);
	pMainDlg->m_TrunkList.ResetContent();
	pMainDlg->m_TrunkList.AddString(mTemp);
	for(i=0;i<=mPubTrunkNo;i++)
	{
		itoa(aryTrunkIndex[i].fChannelNo,mTemp,10);
		strcpy(mInfo,"全局号:");
		strcat(mInfo,mTemp);
		itoa(aryTrunkIndex[i].fDspNo,mTemp,10);
		strcat(mInfo,"  DSP:");
		strcat(mInfo,mTemp);
		itoa(aryTrunkIndex[i].fRowNo,mTemp,10);
		strcat(mInfo," Row:");
		strcat(mInfo,mTemp);
		itoa(aryTrunkIndex[i].fIndex,mTemp,10);
		strcat(mInfo," 下标:");
		strcat(mInfo,mTemp);
		itoa(aryTrunkIndex[i].fChannelID_t,mTemp,10);
		strcat(mInfo," SYS ID:");
		strcat(mInfo,mTemp);
		itoa(aryDspData[aryTrunkIndex[i].fDspNo].pTrunkDevice[aryTrunkIndex[i].fIndex].fChannelNo,mTemp,10);
		strcat(mInfo," pNo:");
		strcat(mInfo,mTemp);
		strcat(mInfo," Index:");
		itoa(aryTrunkIndex[i].fIndex,mTemp,10);
		strcat(mInfo,mTemp);
		strcat(mInfo," LineState:");
		strcpy(mTemp,GetLineStatusString(aryDspData[aryTrunkIndex[i].fDspNo].pTrunkDevice[aryTrunkIndex[i].fIndex].fLineState));
		strcat(mInfo,mTemp);
		mDspNo=aryTrunkIndex[i].fDspNo;
		mDeviceIndex=aryTrunkIndex[i].fIndex;
		if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkVoice)
			strcat(mInfo,"  已连接语音设备");
		else
			strcat(mInfo,"  未连接语音设备");
		itoa(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID.m_s16ChannelID,mTemp,10);
		strcat(mInfo," 通道:");
		strcat(mInfo,mTemp);
		strcat(mInfo,"  LocalNumber:");
		strcat(mInfo,aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLocalNumber);

		if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLineType==LINE_IN)
		{
			strcat(mInfo," 缩位号:");
			strcat(mInfo,aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fSpeedDial);
		}
		pMainDlg->m_TrunkList.AddString(mInfo);

	//	DrawOneItem(aryTrunkIndex[i].fDspNo,aryTrunkIndex[i].fChannelNo,DEVICE_TRUNK);
	}

	pMainDlg->m_VoiceList.ResetContent();
	itoa(mPubVoiceNo,mTemp,10);
	pMainDlg->m_VoiceList.AddString(mTemp);
	for(i=0;i<=mPubVoiceNo;i++)
	{
		itoa(aryVoiceIndex[i].fChannelNo,mTemp,10);
		strcpy(mInfo,"全局号:");
		strcat(mInfo,mTemp);
		itoa(aryVoiceIndex[i].fDspNo,mTemp,10);
		strcat(mInfo,"  DSP:");
		strcat(mInfo,mTemp);
		itoa(aryVoiceIndex[i].fRowNo,mTemp,10);
		strcat(mInfo," Row:");
		strcat(mInfo,mTemp);
		itoa(aryVoiceIndex[i].fIndex,mTemp,10);
		strcat(mInfo," 下标:");
		strcat(mInfo,mTemp);
		itoa(aryVoiceIndex[i].fChannelID_t,mTemp,10);
		strcat(mInfo," SYS ID:");
		strcat(mInfo,mTemp);
		itoa(aryDspData[aryVoiceIndex[i].fDspNo].pVoiceDevice[aryVoiceIndex[i].fIndex].fChannelNo,mTemp,10);
		strcat(mInfo," pNo:");
		strcat(mInfo,mTemp);
		strcat(mInfo," Index:");
		itoa(aryVoiceIndex[i].fIndex,mTemp,10);
		strcat(mInfo,mTemp);
		strcat(mInfo," 关联的接口设备:");
		itoa(aryDspData[aryVoiceIndex[i].fDspNo].pVoiceDevice[aryVoiceIndex[i].fIndex].fLinkTrunkChannelNo,mTemp,10);
		strcat(mInfo," pNo:");
		strcat(mInfo,mTemp);
		pMainDlg->m_VoiceList.AddString(mInfo);
	}

	strcpy(mInfo,"30 line is:");
	itoa(aryDspData[1].pVoiceDevice[30].fChannelNo,mTemp,10);
	strcat(mInfo,mTemp);
	pMainDlg->m_VoiceList.AddString(mInfo);

	pMainDlg->m_VoipList.ResetContent();
	itoa(mPubVoipNo,mTemp,10);
	pMainDlg->m_VoipList.AddString(mTemp);
	for(i=0;i<=mPubVoipNo;i++)
	{
		itoa(aryVoipIndex[i].fChannelNo,mTemp,10);
		strcpy(mInfo,"全局号:");
		strcat(mInfo,mTemp);
		itoa(aryVoipIndex[i].fDspNo,mTemp,10);
		strcat(mInfo,"  DSP:");
		strcat(mInfo,mTemp);
		itoa(aryVoipIndex[i].fRowNo,mTemp,10);
		strcat(mInfo," Row:");
		strcat(mInfo,mTemp);
		itoa(aryVoipIndex[i].fIndex,mTemp,10);
		strcat(mInfo," 下标:");
		strcat(mInfo,mTemp);
		itoa(aryVoipIndex[i].fChannelID_t,mTemp,10);
		strcat(mInfo," SYS ID:");
		strcat(mInfo,mTemp);
		itoa(aryDspData[aryVoipIndex[i].fDspNo].pVoipDevice[aryVoipIndex[i].fIndex].fChannelNo,mTemp,10);
		strcat(mInfo," pNo:");
		strcat(mInfo,mTemp);
		strcat(mInfo," Index:");
		itoa(aryVoipIndex[i].fIndex,mTemp,10);
		strcat(mInfo,mTemp);
		if(aryDspData[aryVoipIndex[i].fDspNo].pVoipDevice[aryVoipIndex[i].fIndex].fIsLinkVoice)
			strcat(mInfo,"  已连接语音");
		else
			strcat(mInfo,"  未连接语音");
		itoa(aryDspData[aryVoipIndex[i].fDspNo].pVoipDevice[aryVoipIndex[i].fIndex].fLinkVoiceChannelNo,mTemp,10);
		strcat(mInfo,mTemp);
		pMainDlg->m_VoipList.AddString(mInfo);
	}

}

/***********************************************************************
功能：根据设备底层编号，获取在相应index数组中的数组下标
***********************************************************************/
long	FindArrayIndexByChannelID(DJ_S32 mDspID,ChannelID_t mChannelID,DEVICE_TYPE mDeviceType)
{
		long			mDspNo;
		int				i;
		bool			mFind;
		long			mResult;
		long			mMaxDeviceCount;
		mDspNo=mDspID;
		mFind=false;
		i=0;
		mResult=-1;
		mMaxDeviceCount=0;
		if(mDeviceType==DEVICE_VOIP) mMaxDeviceCount=MAX_VOIP_COUNT;
		if(mDeviceType==DEVICE_TRUNK) mMaxDeviceCount=MAX_INTERFACE_COUNT;
		if(mDeviceType==DEVICE_VOICE) mMaxDeviceCount=MAX_VOICE_COUNT;
		while(i<=mMaxDeviceCount && mFind==false)
		{
			if(mDeviceType==DEVICE_VOICE)
			{
				if(aryVoiceIndex[i].fDspNo==mDspNo && aryVoiceIndex[i].fChannelID_t==mChannelID && aryVoiceIndex[i].fDeviceType==mDeviceType)	
				{
					mResult=i;
					mFind=true;
				}
			}
			if(mDeviceType==DEVICE_TRUNK)
			{
				if(aryTrunkIndex[i].fDspNo==mDspNo && aryTrunkIndex[i].fChannelID_t==mChannelID && aryTrunkIndex[i].fDeviceType==mDeviceType)	
				{
					mResult=i;
					mFind=true;
				}
			}
			if(mDeviceType==DEVICE_VOIP)
			{
				if(aryVoipIndex[i].fDspNo==mDspNo && aryVoipIndex[i].fChannelID_t==mChannelID && aryVoipIndex[i].fDeviceType==mDeviceType)	
				{
					mResult=i;
					mFind=true;
				}
			}
			i++;
		}
	return mResult;
}

/***********************************************************************
功能：动态显示状态栏信息
***********************************************************************/
void	DisplayStatusBar()
{
	char	mTemp[30];
	char	mInfo[300];
	strcpy(mInfo,"DSP数量:");
	itoa(mOpenDspCount+1,mTemp,10);
	strcat(mInfo,mTemp);
	strcat(mInfo,"   接口设备:共");
	itoa(mPubTrunkNo+1,mTemp,10);
	strcat(mInfo,mTemp);
	strcat(mInfo,"  已打开:");
	itoa(mOpenTrunkCount+1,mTemp,10);
	strcat(mInfo,mTemp);
	strcat(mInfo,"   语音设备:共");
	itoa(mPubVoiceNo+1,mTemp,10);
	strcat(mInfo,mTemp);
	strcat(mInfo,"  已打开:");
	itoa(mOpenVoiceCount+1,mTemp,10);
	strcat(mInfo,mTemp);
	strcat(mInfo,"   IP设备:共");
	itoa(mPubVoipNo+1,mTemp,10);
	strcat(mInfo,mTemp);
	strcat(mInfo,"  已打开:");
	itoa(mOpenVoipCount+1,mTemp,10);
	strcat(mInfo,mTemp);
	strcpy(pMainDlg->mUpdateStatusBar,mInfo);
	PostMessage(pMainDlg->m_hWnd,WM_REFRESH_STATUS_BAR,0,0);
	//pMainDlg->m_wndStatusBar.SetPaneText(7,mTemp);	
}					

/***********************************************************************
功能：判断用户双击某一个条目时候，是否能进行修改，并返回用户选择需要编辑的设备全局编号
***********************************************************************/
bool	JudgeCanEdit(int	mItemindex,DEVICE_TYPE *pDeviceType,long	*pDeviceNo,LINE_TYPE *pLineType,DJ_S32 *pDspNo)
{
	long	mDeviceNo;
	bool	mFind;
	long	i;
	long	mDspNo;
	long	mIndex;
	mFind=false;
	mDeviceNo=-1;
	i=0;
	while(i<=MAX_INTERFACE_COUNT && mFind==false)
	{
		if(aryTrunkIndex[i].fRowNo==mItemindex)
		{
			mDeviceNo=aryTrunkIndex[i].fChannelNo;
			*pDeviceType=DEVICE_TRUNK;
			mDspNo=aryTrunkIndex[i].fDspNo;
			mIndex=aryTrunkIndex[i].fIndex;
			*pLineType=aryDspData[mDspNo].pTrunkDevice[mIndex].fLineType;
			*pDspNo=mDspNo;
			mFind=true;
		}
		i++;
	}
	i=0;
	while(i<=MAX_VOIP_COUNT && mFind==false)
	{
		if(aryVoipIndex[i].fRowNo==mItemindex)
		{
			mDeviceNo=aryVoipIndex[i].fChannelNo;
			*pDeviceType=DEVICE_VOIP;
			*pDspNo=aryVoipIndex[i].fDspNo;
			mFind=true;
		}
		i++;
	}
	i=0;
	while(i<=MAX_VOICE_COUNT && mFind==false)
	{
		if(aryVoiceIndex[i].fRowNo==mItemindex)
		{
			mDeviceNo=aryVoiceIndex[i].fChannelNo;
			*pDeviceType=DEVICE_VOICE;
			mFind=true;
		}
		i++;
	}
	if(!mFind)	return false;
	*pDeviceNo=mDeviceNo;
	return true;
	
}

/***********************************************************************
功能：从数据库中读取线路信息，并刷新界面
***********************************************************************/
bool	RefreshDeviceGUI(long mDspNo,long mDeviceNo,LINE_TYPE  mLineType)
{
	char	mSQL[200];
	CDBProcess	objDb;
	char	mTemp[20];
	_variant_t	mValue;
	long		mIndex;
	
	if(mLineType==LINE_IN)
	{
		strcpy(mSQL,"select * from objUserLine where fdspno='");
		itoa(mDspNo,mTemp,10);
		strcat(mSQL,mTemp);
		strcat(mSQL,"' and flineno='");
		itoa(mDeviceNo,mTemp,10);
		strcat(mSQL,mTemp);
		strcat(mSQL,"'");
		if(!objDb.ConnectDB())	return false;
		if(!objDb.ExcueteQuery(mSQL))	return false;
		if(objDb.pRs->EndOfFile)	return false;
		mValue=(_variant_t)objDb.pRs->GetCollect("fphonenumber");
		if(mValue.vt!=VT_NULL)
			strcpy(mTemp,(char *)_bstr_t(mValue));
		else
			strcpy(mTemp,"");
		mIndex=FindIndexByChannelNo(mDspNo,mDeviceNo,DEVICE_TRUNK);
		mIndex=aryTrunkIndex[mIndex].fIndex;
		strcpy(aryDspData[mDspNo].pTrunkDevice[mIndex].fLocalNumber,mTemp);
		mValue=(_variant_t)objDb.pRs->GetCollect("fisenable");
		if(mValue.vt!=VT_NULL)
			strcpy(mTemp,(char *)_bstr_t(mValue));
		else
			strcpy(mTemp,"");
		if(strcmp(mTemp,"启用")==-1)
			aryDspData[mDspNo].pTrunkDevice[mIndex].fEnabled=false;
		else
			aryDspData[mDspNo].pTrunkDevice[mIndex].fEnabled=true;
		mValue=(_variant_t)objDb.pRs->GetCollect("flongenable");
		if(mValue.vt!=VT_NULL)
			strcpy(mTemp,(char *)_bstr_t(mValue));
		else
			strcpy(mTemp,"");
		if(strcmp(mTemp,"开通")==-1)
			aryDspData[mDspNo].pTrunkDevice[mIndex].fEnabledCallLong=false;
		else
			aryDspData[mDspNo].pTrunkDevice[mIndex].fEnabledCallLong=true;
		//
		mValue=(_variant_t)objDb.pRs->GetCollect("fSpeedDial");
		if(mValue.vt!=VT_NULL)
			strcpy(mTemp,(char *)_bstr_t(mValue));
		else
			strcpy(mTemp,"");
		strcpy(aryDspData[mDspNo].pTrunkDevice[mIndex].fSpeedDial,mTemp);

		mValue=(_variant_t)objDb.pRs->GetCollect("fTargetUser");
		if(mValue.vt!=VT_NULL)
			strcpy(mTemp,(char *)_bstr_t(mValue));
		else
			strcpy(mTemp,"");
		strcpy(aryDspData[mDspNo].pTrunkDevice[mIndex].fTargetUserNo,mTemp);

		objDb.CloseDB();
		DrawOneItem(mDspNo,aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID.m_s16ChannelID,DEVICE_TRUNK);
	}
	if(mLineType==LINE_OUT)
	{
		strcpy(mSQL,"select * from objTrunkLine where fdspno='");
		itoa(mDspNo,mTemp,10);
		strcat(mSQL,mTemp);
		strcat(mSQL,"' and flineno='");
		itoa(mDeviceNo,mTemp,10);
		strcat(mSQL,mTemp);
		strcat(mSQL,"'");
		if(!objDb.ConnectDB())	return false;
		if(!objDb.ExcueteQuery(mSQL))	return false;
		if(objDb.pRs->EndOfFile)	return false;
		mValue=(_variant_t)objDb.pRs->GetCollect("fphonenumber");
		if(mValue.vt!=VT_NULL)
			strcpy(mTemp,(char *)_bstr_t(mValue));
		else
			strcpy(mTemp,"");
		mIndex=FindIndexByChannelNo(mDspNo,mDeviceNo,DEVICE_TRUNK);
		mIndex=aryTrunkIndex[mIndex].fIndex;
		strcpy(aryDspData[mDspNo].pTrunkDevice[mIndex].fLocalNumber,mTemp);

		mValue=(_variant_t)objDb.pRs->GetCollect("fisenable");
		if(mValue.vt!=VT_NULL)
			strcpy(mTemp,(char *)_bstr_t(mValue));
		else
			strcpy(mTemp,"禁止");
		if(strcmp(mTemp,"启用")==0)
			aryDspData[mDspNo].pTrunkDevice[mIndex].fEnabled=true;
		else
			aryDspData[mDspNo].pTrunkDevice[mIndex].fEnabled=false;
		objDb.CloseDB();
		DrawOneItem(mDspNo,aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID.m_s16ChannelID,DEVICE_TRUNK);
	}
	return true;
}

/***********************************************************************
功能：动态刷新状态栏
***********************************************************************/
void	DisplayStatusBar(CString pInfo)
{
	char	mTemp[400];
	strcpy(mTemp,pMainDlg->mUpdateStatusBar);
	strcat(mTemp,pInfo);
	strcpy(pMainDlg->mUpdateStatusBar,mTemp);
	PostMessage(pMainDlg->m_hWnd,WM_REFRESH_STATUS_BAR,0,0);
}


/***********************************************************************
功能：查找一个可用的语音通道，并返回该语音通道的数组下标
***********************************************************************/
long	FindFreeVoiceDevice(long mDspNo)
{
	long	mVoiceIndex;
	bool	mFind;
	long	i;
	mFind=false;
	i=0;
	mVoiceIndex=-1;
	while(i<=aryDspData[mDspNo].fVoiceCount && mFind==false)
	{
		if(aryDspData[mDspNo].pVoiceDevice[i].fIsOpen && aryDspData[mDspNo].pVoiceDevice[i].fStatusCode==VOC_FREE && aryDspData[mDspNo].pVoiceDevice[i].fIsLinkTrunk==false && aryDspData[mDspNo].pVoiceDevice[i].fIsLinkVoip==false)
		{
			mVoiceIndex=i;
			mFind=true;
		}
		i++;
	}
	return mVoiceIndex;
}

/***********************************************************************
功能：设置模拟中继外线的工作参数
1.设置与该外线通道相连接的语音通道的GTD参数(通用信号检测)
2.设置外线通道的工作参数
***********************************************************************/
bool	SetTrunkLineInitParameter(DeviceID_t *pTrunkDevice,DeviceID_t *pVoiceDevice)
{
	RetCode_t	rtCode;
	CmdParamData_AnalogTrunk_t	mTrunkParam;
	DJ_S16		m16ParamType;
	DJ_S16		m16ParamSize;
	void		*pTrunkParam=NULL;

	/*设置GTD(通用音频检测)工作参数*/
	//if(!SetVoiceGTD(pVoiceDevice))	return false;

	/*设置外线工作参数*/
	m16ParamType=ANALOGTRUNK_PARAM_UNIPARAM;
	m16ParamSize=sizeof(CmdParamData_AnalogTrunk_t);
	memset(&mTrunkParam,0,sizeof(mTrunkParam));
//	mTrunkParam.m_VocDevID=*pVoiceDevice;										/*指定绑定关系*/
//	mTrunkParam.m_u8CallerIdOption=XMS_ANALOG_TRUNK_CALLERID_FSKORDTMF;			/*号码传输模式*/
//	mTrunkParam.m_u8CallInRingCount=2;											/*响铃次数*/
//	mTrunkParam.m_u8DialtoneDetect=XMS_ANALOG_TRUNK_DIALTONE_DETECT;			/*检测信号音*/
//	mTrunkParam.m_u8CallOutMethod=XMS_ANALOG_TRUNK_CALLOUT_APP_FULLCONTROL;
	mTrunkParam.m_u16CallInRingCount = 2; //ring count before CallIn event
	mTrunkParam.m_u16CallInRingTimeOut = 45;//Unit:ms Ring TimeOut,when ring timed out ,state changed to DCS_Free

	pTrunkParam=(void *) &mTrunkParam;

	rtCode=XMS_ctsSetParam(pFlowHandle,pTrunkDevice,m16ParamType,m16ParamSize,(void *)pTrunkParam);
	if(rtCode<0)	return false;
	return true;
}

/***********************************************************************
功能：设置语音设备的GTD参数
***********************************************************************/
bool	SetVoiceGTD(DeviceID_t *pVoiceDevice)
{
	RetCode_t	rtCode;
	DJ_S16		m16ParamType;
	DJ_S16		m16ParamSize;
	void		*pTrunkParam=NULL;
	CmdParamData_Voice_t	mVoiceParam;
	/*设置GTD(通用音频检测)工作参数*/
	memset(&mVoiceParam,0,sizeof(CmdParamData_Voice_t));
	mVoiceParam.m_u8GtdCtrlValid=1;												/*启用GTD*/
	mVoiceParam.m_VocGtdControl.m_u8ChannelEnable=1;
	mVoiceParam.m_VocGtdControl.m_u8DTMFEnable=1;
	mVoiceParam.m_VocGtdControl.m_u8MR2FEnable = 1;
	mVoiceParam.m_VocGtdControl.m_u8MR2BEnable = 1;
	mVoiceParam.m_VocGtdControl.m_u8GTDEnable = 1;
	mVoiceParam.m_VocGtdControl.m_u8FSKEnable = 1;

	strcpy((char *)&mVoiceParam.m_VocGtdControl.m_u8GTDID[0],("GHIJK"));
	m16ParamType=VOC_PARAM_UNIPARAM;
	m16ParamSize=sizeof(mVoiceParam);
	pTrunkParam=(char *)&mVoiceParam;
	rtCode=XMS_ctsSetParam(pFlowHandle,pVoiceDevice,m16ParamType,m16ParamSize,(void *)pTrunkParam);
	if(rtCode<0)	return false;
	return true;
}

/***********************************************************************
功能：设置user设备的参数
***********************************************************************/
bool	SetUserDeviceParam(DeviceID_t *pUserDevice,DeviceID_t *pVoiceDevice)
{
	RetCode_t	rtCode;
	DJ_S16		m16ParamType;
	DJ_S16		m16ParamSize;
	void		*pUserParam=NULL;
	CmdParamData_AnalogUser_t mUserParam;

	/*设置User设备工作参数*/
	memset(&mUserParam,0,sizeof(CmdParamData_AnalogUser_t));

	mUserParam.m_u8CallerIdOption = 2;//0:disabled; 1:Fsk CallerId; 2:DTMF CallerId;,reference XMS_ANALOG_USER_CALLERID_OPT
	mUserParam.m_u8RingCadenceType = 0;//0: single-pulse ring; 1:double-pulse ring; 2: triple-pulse ring
	mUserParam.m_u8CallOutRingTimeOut = 0;// unit: second ; Ringing TimeOut ,it timetout ,then state return to DCS_FREE


/*	mUserParam.m_VocDevID=*pVoiceDevice;

    mUserParam.m_u8CallerIdOption=2;//0:disabled; 1:Fsk CallerId; 2:DTMF
	mUserParam.m_u8RingCadenceType=0;
    mUserParam.m_u8CallOutRingTimeOut=0;
	mUserParam.m_u8AutoDialtone=0;
	mUserParam.m_u8CallInMethod=0;
	mUserParam.m_u16CalleeLen=0;
	mUserParam.m_u16InterDigitTimeOut=0;
	mUserParam.m_u16FullDialTimeOut=0;*/
	//mUserParam.m_u8Rfu1=1;

    m16ParamType = ANALOGUSER_PARAM_UNIPARAM;
	m16ParamSize = sizeof(CmdParamData_AnalogUser_t);

	pUserParam=(char *)&mUserParam;
	rtCode=XMS_ctsSetParam(pFlowHandle,pUserDevice,m16ParamType,m16ParamSize,(void *)pUserParam);
	//rtCode=XMS_ctsSetParam(pFlowHandle,pVoiceDevice,m16ParamType,m16ParamSize,(void *)pUserParam);
	if(rtCode<0)	return false;
	return true;
}


/***********************************************************************
功能：在相应的语音设备播放拨号音，忙音，回铃音或者停止播放
***********************************************************************/
DJ_S32	PlayTone(DeviceID_t	*pVoiceDevice,TONE_TYPE mToneType)
{
	DJ_U32           i = 0;	
	DJ_U16           u16IoLen = 0;
	DJ_U16           u16IoType = 0;
	RetCode_t		 r;
	char		 	 IoDataBuf[MAX_SEND_IODATA_DTMF_LEN]={0};
	if ( mToneType == STOP_TONE )	
	{
		u16IoLen = 0;
		u16IoType = XMS_IO_TYPE_GTG;
	}
	else
	{
		u16IoType = XMS_IO_TYPE_GTG;
		u16IoLen = 1;
		switch ( mToneType)
		{
		case DIAL_TONE:		
			IoDataBuf[0] = 'G';
			break;
		case BACK_TONE:	
			IoDataBuf[0] = 'H';
			break;
		case BUSY_TONE:
			IoDataBuf[0] = 'I';
			break;
		}
	}
 	r = XMS_ctsSendIOData(pFlowHandle,pVoiceDevice,u16IoType,u16IoLen,IoDataBuf);
	return	r;
}

/***********************************************************************
功能：设置接口设备通道状态
***********************************************************************/
void	SetTrunkDeviceState(DeviceID_t	*pDevice,VOICE_STATUS mStatus)
{
	long	mIndex;
	mIndex=FindIndexByChannelID(pDevice->m_s8ModuleID,pDevice->m_s16ChannelID,DEVICE_TRUNK);
	if(mIndex==-1) return ;
	aryDspData[pDevice->m_s8ModuleID].pTrunkDevice[mIndex].fStatusCode=mStatus;
	DrawOneItem(pDevice->m_s8ModuleID,pDevice->m_s16ChannelID,DEVICE_TRUNK);
}

/***********************************************************************
功能：设置语音设备的通道状态
***********************************************************************/
void	SetVoiceDeviceState(DeviceID_t	*pDevice,VOICE_STATUS mStatus)
{
	long	mIndex;
	mIndex=FindIndexByChannelID(pDevice->m_s8ModuleID,pDevice->m_s16ChannelID,DEVICE_VOICE);
	if(mIndex==-1) return ;
	aryDspData[pDevice->m_s8ModuleID].pVoiceDevice[mIndex].fStatusCode=mStatus;
	DrawOneItem(pDevice->m_s8ModuleID,pDevice->m_s16ChannelID,DEVICE_VOICE);
}

/***********************************************************************
功能：设置VOIP设备的通道状态
***********************************************************************/
void	SetVoipDeviceState(DeviceID_t	*pDevice,VOIP_STATUS mStatus)
{
	long	mIndex;
	mIndex=FindIndexByChannelID(pDevice->m_s8ModuleID,pDevice->m_s16ChannelID,DEVICE_VOIP);
	if(mIndex==-1) return ;
	aryDspData[pDevice->m_s8ModuleID].pVoipDevice[mIndex].fStatusCode=mStatus;
	DrawOneItem(pDevice->m_s8ModuleID,pDevice->m_s16ChannelID,DEVICE_VOIP);
}

/***********************************************************************
功能：查找一个可用的外线通道，本系统采用轮询法则，未找到则返回-1
***********************************************************************/
long	FindFreeTrunkToUse()
{
	bool	mFind;
	long	mResult;
	long	i;
	long	mIndex;
	DJ_S32	mDspNo;

	mFind=false;
	mResult=-1;
	i=0;
	while(i<=mTrunkCount && mFind==false)
	{
		mIndex=aryTrunkIndex[i].fIndex;
		mDspNo=aryTrunkIndex[i].fDspNo;
		if(aryDspData[mDspNo].pTrunkDevice[mIndex].fLineState==DCS_FREE && aryDspData[mDspNo].pTrunkDevice[mIndex].fLineType==LINE_OUT 
		   && aryDspData[mDspNo].pTrunkDevice[mIndex].fEnabled==true && strcmp(aryDspData[mDspNo].pTrunkDevice[mIndex].fLocalNumber,mFirstPhone)!=0)
		{
			mFind=true;
			mResult=i;
		}
		i++;
	}
	return mResult;
}

/***********************************************************************
功能：建立两个设备之间的双向连接
***********************************************************************/
void	SetLink (DeviceID_t	*pSourceDev,DeviceID_t *pTargetDev)
{
	if( pSourceDev->m_s16DeviceMain == 0 ||	pTargetDev->m_s16DeviceMain == 0 )
		return;
	XMS_ctsLinkDevice(pFlowHandle,pSourceDev,pTargetDev,NULL); 
	XMS_ctsLinkDevice(pFlowHandle,pTargetDev,pSourceDev,NULL);
	return;
}

/***********************************************************************
功能：拆除两个设备之间的连接
***********************************************************************/
void	DeleteLink(DeviceID_t	*pSourceDev,DeviceID_t	*pTargetDev)
{
	int r=0; 
	r=XMS_ctsUnlinkDevice(pFlowHandle,pSourceDev,pTargetDev,NULL);
	r=XMS_ctsUnlinkDevice(pFlowHandle,pTargetDev,pSourceDev,NULL);
}

/***********************************************************************
功能：复位接口设备，将其设置为初始化状态(本函数只处理外线)
***********************************************************************/
void	ResetTrunkDevice(DeviceID_t *pDevice,Acs_Evt_t	*pEvt)
{
	long	mDspNo;
	long	mIndex;
	ChannelID_t	mChannelID;
	long	mDeviceIndex;

	mDspNo=pDevice->m_s8ModuleID;
	mChannelID=pDevice->m_s16ChannelID;
	if(pDevice->m_s16DeviceSub!=XMS_DEVSUB_ANALOG_TRUNK)	return;
	mIndex=FindIndexByChannelID(mDspNo,mChannelID,DEVICE_TRUNK);
	mDeviceIndex=aryTrunkIndex[mIndex].fIndex;
	/*如果有连接的接口设备，则将该连接清除*/
	if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkTrunk)
	{
		DeviceID_t		mLinkDeviceID;
		long			mLinkIndex;
		long			mLinkDspNo;
		long			mLinkDeviceIndex;
		mLinkDeviceID=aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkTrunkDeviceID;
		if(mLinkDeviceID.m_s16DeviceMain!=0)
		{
			DeleteLink(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkTrunkDeviceID,pDevice);
			aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkTrunk=false;
			mLinkIndex=FindIndexByChannelID(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkTrunkDeviceID.m_s8ModuleID,aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkTrunkDeviceID.m_s16ChannelID,DEVICE_TRUNK);
			mLinkDeviceIndex=aryTrunkIndex[mLinkIndex].fIndex;
			mLinkDspNo=aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkTrunkDeviceID.m_s8ModuleID;
			/*对与该外线接口的内线接口发出一个拆线函数*/
			XMS_ctsClearCall(pFlowHandle,&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDeviceID,0,NULL);
			aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkTrunkChannelNo=-1;
			aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fIsLinkTrunk=false;
			if(aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fIsLinkVoice==false)
			{
				SetLink(&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDeviceID,
					    &aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkVoiceDeviceID);
				aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fIsLinkVoice=true;
			}
			PlayTone(&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkVoiceDeviceID,BUSY_TONE);
		}
	}

	/*外线拆IP*/
	if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fisLinkVoipVoice)
	{
		DeviceID_t		mLinkDeviceID;
		long			mLinkIndex;
		long			mLinkDspNo;
		long			mLinkDeviceIndex;

		mLinkDeviceID=aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoipDeviceID;
		if(mLinkDeviceID.m_s16DeviceMain!=0)
		{
			DeleteLink(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoipVoiceDeviceID,pDevice);
			aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fisLinkVoipVoice=false;

			mLinkIndex=FindIndexByChannelID(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoipDeviceID.m_s8ModuleID,aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoipVoiceDeviceID.m_s16ChannelID,DEVICE_VOIP);
			mLinkDeviceIndex=aryVoipIndex[mLinkIndex].fIndex;
			mLinkDspNo=aryVoipIndex[mLinkIndex].fDspNo;
			/*IP通道放忙音*/
			DJ_U16		mCommandType;
			MixerControlParam_t	mParam;
			
			mCommandType=VOIP_PARAM_RTPMIXER;
			mParam.m_u8SRC1_Ctrl=XMS_MIXER_FROM_PLAY;
			mParam.m_u16SRC_ChID1=aryDspData[mLinkDspNo].pVoipDevice[mLinkDeviceIndex].fLinkVoiceDeviceID.m_s16ChannelID;
			XMS_ctsSetParam(pFlowHandle,&aryDspData[mLinkDspNo].pVoipDevice[mLinkDeviceIndex].fDeviceID,mCommandType,sizeof(mParam),(DJ_Void *)&mParam);
			aryDspData[mLinkDspNo].pVoipDevice[mLinkDeviceIndex].fIsLinkTrunk=false;
			PlayTone(&aryDspData[mLinkDspNo].pVoipDevice[mLinkDeviceIndex].fLinkVoiceDeviceID,BUSY_TONE);
		}
	}
	/*本通道数据初始化*/
	aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkTrunk=false;
	aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkTrunkChannelNo=-1;
	strcpy(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff,"");		
	strcpy(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fCallInNumber,"");
	strcpy(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fCallOutNumber,"");
	/*拍叉簧数据初始化*/
	aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fPrevDeviceType=-1;
	aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fPrevTrunkChannelNo=-1;
	aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fPrevVoipChannelNo=-1;
	aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fChaHuanCount=0;
	DeleteLink(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,
		       &aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID);
	aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkVoice=false;
	PlayTone(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID,STOP_TONE);
	ControlPlay_t	mTempControl;
	mTempControl.m_u16ControlType=XMS_STOP_PLAY;
	XMS_ctsControlPlay(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID,&mTempControl,NULL);
	XMS_ctsResetDevice(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID,NULL);
	XMS_ctsGetDevState(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID,NULL);

	SetTrunkDeviceState(pDevice,VOC_FREE);
	XMS_ctsResetDevice(pFlowHandle,pDevice,NULL);
	XMS_ctsGetDevState(pFlowHandle,pDevice,NULL);
	return;
}


/***********************************************************************
功能：复位接口设备，将其设置为初始化状态(本函数只处理内线)
***********************************************************************/
void	ResetUserDevice(DeviceID_t *pDevice,Acs_Evt_t	*pEvt)
{
	long	mDspNo;
	long	mIndex;
	long	mDeviceIndex;
	ChannelID_t	mChannelID;

	mDspNo=pDevice->m_s8ModuleID;
	mChannelID=pDevice->m_s16ChannelID;
	if(pDevice->m_s16DeviceSub!=XMS_DEVSUB_ANALOG_USER)	return;
	mIndex=FindIndexByChannelID(mDspNo,mChannelID,DEVICE_TRUNK);
	mDeviceIndex=aryTrunkIndex[mIndex].fIndex;
	/*如果不是在拍叉簧的状态下，则正常处理*/
	if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsChaHuan==false)
	{
		/*如果有连接的接口设备，则将该连接清除*/
		if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkTrunk)
		{
			DeviceID_t		mLinkDeviceID;
			long			mLinkIndex;
			long			mLinkDspNo;
			long			mLinkDeviceIndex;
			mLinkDeviceID=aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkTrunkDeviceID;
			/*如果连接的是内线*/
			if(mLinkDeviceID.m_s16DeviceMain!=0 && mLinkDeviceID.m_s16DeviceSub==XMS_DEVSUB_ANALOG_USER)
			{
				mLinkIndex=FindIndexByChannelNo(mDspNo,aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkTrunkChannelNo,DEVICE_TRUNK);
				mLinkDeviceIndex=aryTrunkIndex[mLinkIndex].fIndex;
				mLinkDspNo=aryTrunkIndex[mLinkIndex].fDspNo;
				DeleteLink(&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkTrunkDeviceID,
					&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID);
				aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fIsLinkTrunk=false;
				/*相关联的接口设备的语音通道放忙音*/
				if(aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fIsLinkVoice==false)
				{
					SetLink(&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDeviceID,
						&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkVoiceDeviceID);
					aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fIsLinkVoice=true;
				}
				/*先停止放音*/
				PlayTone(&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkVoiceDeviceID,BUSY_TONE);
				XMS_ctsClearCall(pFlowHandle,&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDeviceID,0,NULL);
			}
			/*如果连接的是外线，此种情况是内线拨打外线，并且内线先挂机*/
			if(mLinkDeviceID.m_s16DeviceMain!=0 && mLinkDeviceID.m_s16DeviceSub==XMS_DEVSUB_ANALOG_TRUNK)
			{
				mLinkIndex=FindIndexByChannelNo(mDspNo,aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkTrunkChannelNo,DEVICE_TRUNK);
				mLinkDeviceIndex=aryTrunkIndex[mLinkIndex].fIndex;
				mLinkDspNo=aryTrunkIndex[mLinkIndex].fDspNo;
				DeleteLink(&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkTrunkDeviceID,
					&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID);
				aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkTrunkChannelNo=-1;
				aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fIsLinkTrunk=false;
				/*向局方发出拆线信息*/
				XMS_ctsClearCall(pFlowHandle,&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDeviceID,0,NULL);
				XMS_ctsResetDevice(pFlowHandle,&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDeviceID,NULL);
				XMS_ctsGetDevState(pFlowHandle,&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDeviceID,NULL);
				aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fIsLinkVoice=false;
				/*将外线通道设置为空闲状态*/
				SetTrunkDeviceState(&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDeviceID,VOC_FREE);
				strcpy(aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fCallInNumber,"");
				strcpy(aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fCallOutNumber,"");
				strcpy(aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDtmfBuff,"");
			}
		}
		/*如果连接的是VOIP*/
		if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fisLinkVoipVoice)
		{
			long		mLinkVoipVoiceNo;
			long		mLinkVoipVoiceDspNo;
			long		mLinkVoipVoiceIndex;
			long		mVoipDspNo;
			long		mVoipDeviceNo;
			long		mVoipDeviceIndex;
			
			/*首先将本接口通道与VOIP设备的语音通道之间的连接断开*/		
			mLinkVoipVoiceNo=aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoipVoiceDeviceNo;
			mLinkVoipVoiceDspNo=0;
			mLinkVoipVoiceIndex=FindIndexByChannelNo(mLinkVoipVoiceDspNo,mLinkVoipVoiceNo,DEVICE_VOICE);
			mLinkVoipVoiceDspNo=aryVoiceIndex[mLinkVoipVoiceIndex].fDspNo;
			mLinkVoipVoiceIndex=aryVoiceIndex[mLinkVoipVoiceIndex].fIndex;
			DeleteLink(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,
				&aryDspData[mLinkVoipVoiceDspNo].pVoiceDevice[mLinkVoipVoiceIndex].fDeviceID);
			aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fisLinkVoipVoice=false;
			aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoipVoiceDeviceNo=-1;
			aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkVoice=false;
			/*向VOIP通道发起拆线消息*/
			mVoipDeviceNo=aryDspData[mLinkVoipVoiceDspNo].pVoiceDevice[mLinkVoipVoiceIndex].fLinkVoipChannelNo;
			mVoipDspNo=mDspNo;			/*本语句没用*/
			mVoipDeviceIndex=FindIndexByChannelNo(mVoipDspNo,mVoipDeviceNo,DEVICE_VOIP);
			mVoipDspNo=aryVoipIndex[mVoipDeviceIndex].fDspNo;
			mVoipDeviceIndex=aryVoipIndex[mVoipDeviceIndex].fIndex;
			XMS_ctsClearCall(pFlowHandle,&aryDspData[mVoipDspNo].pVoipDevice[mVoipDeviceIndex].fDeviceID,0,NULL);
		}
	}
	else
	{
		/*拍叉簧的处理*/
		if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkTrunk==true)
		{
			DJ_S32		mLinkDspNo;
			long		mLinkDeviceNo;
			long		mLinkDeviceIndex;
			DJ_S32		mPrevDspNo;
			long		mPrevDeviceNo;
			long		mPrevDeviceIndex;

			if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode==VOC_ERROR_CALL_LONG ||
				aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode==VOC_PLAY_NOTICE ||
				aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode==VOC_PLAY_NUMBER ||
				aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode==VOC_PLAY_DIAL_WRONG ||
				aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode==VOC_PLAY_WELCOME)
			{
				ControlPlay_t	mControlPlay;
				mControlPlay.m_u16ControlType=XMS_STOP_PLAY;
				XMS_ctsControlPlay(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID,&mControlPlay,NULL);
				AddMsg("叉簧停止播放");
			}
			/*获取当前连接的接口通道信息*/
			mLinkDeviceNo=aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkTrunkChannelNo;
			mLinkDspNo=-1;
			mLinkDeviceIndex=FindIndexByChannelNo(mLinkDspNo,mLinkDeviceNo,DEVICE_TRUNK);
			mLinkDspNo=aryTrunkIndex[mLinkDeviceIndex].fDspNo;
			mLinkDeviceIndex=aryTrunkIndex[mLinkDeviceIndex].fIndex;
			/*获取要直接连接的通道*/
			/*连接的是接口*/
			if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fPrevDeviceType==1)
			{
				mPrevDeviceNo=aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fPrevTrunkChannelNo;
				mPrevDspNo=-1;
				mPrevDeviceIndex=FindIndexByChannelNo(mPrevDspNo,mPrevDeviceNo,DEVICE_TRUNK);
				mPrevDspNo=aryTrunkIndex[mPrevDeviceIndex].fDspNo;
				mPrevDeviceIndex=aryTrunkIndex[mPrevDeviceIndex].fIndex;
				/*呼叫第三方成功，即和第三方建立通话了。才挂机*/
				if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fChaHuanIsCallStatus==true)
				{
					/*断开该通道与语音通道的接口*/
					/*新加的代码-----wwk*/
					ControlPlay_t	mControlPlay;
					mControlPlay.m_u16ControlType=XMS_STOP_PLAY;
					XMS_ctsControlPlay(pFlowHandle,&aryDspData[mPrevDspNo].pVoipDevice[mPrevDeviceIndex].fLinkVoiceDeviceID,&mControlPlay,NULL);
					AddMsg("叉簧停止播放1");
					DeleteLink(&aryDspData[mPrevDspNo].pTrunkDevice[mPrevDeviceIndex].fDeviceID,
						&aryDspData[mPrevDspNo].pTrunkDevice[mPrevDeviceIndex].fLinkVoiceDeviceID);
					/*断开本通道和前向通道的连接*/
					DeleteLink(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,
						&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkTrunkDeviceID);
					aryDspData[mPrevDspNo].pTrunkDevice[mPrevDeviceIndex].fIsLinkVoice=false;
					aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkVoice=false;
					/*将该通道与目标通道建立双向连接*/	
					SetLink(&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDeviceID,
						&aryDspData[mPrevDspNo].pTrunkDevice[mPrevDeviceIndex].fDeviceID);
					aryDspData[mPrevDspNo].pTrunkDevice[mPrevDeviceIndex].fIsLinkTrunk=true;
					aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fIsLinkTrunk=true;
					aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fChaHuanCount=0;
					aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fIsChaHuan=false;
				}
				else
				{
					//aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fIsChaHuan=true;
					if(aryDspData[mPrevDspNo].pTrunkDevice[mPrevDeviceIndex].fDeviceType==DEVICE_TRUNK)
					{
						aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fPrevDeviceType=1;	
						aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fPrevTrunkDeviceID=aryDspData[mPrevDspNo].pTrunkDevice[mPrevDeviceIndex].fDeviceID;
						aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fPrevTrunkChannelNo=aryDspData[mPrevDspNo].pTrunkDevice[mPrevDeviceIndex].fChannelNo;
					}
					if(aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDeviceType==DEVICE_VOIP)
					{
						aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fPrevDeviceType=2;	
						aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fPrevVoipDeviceID=aryDspData[mPrevDspNo].pTrunkDevice[mPrevDeviceIndex].fDeviceID;
						aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fPrevVoipChannelNo=aryDspData[mPrevDspNo].pTrunkDevice[mPrevDeviceIndex].fChannelNo;
					}
					aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fIsChaHuan=false;
				}
				/*置标记为：已挂机*/
				aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fChaHuanIsReset=true;
				aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkTrunkChannelNo=aryDspData[mPrevDspNo].pTrunkDevice[mPrevDeviceIndex].fChannelNo;
				aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkTrunkDeviceID=aryDspData[mPrevDspNo].pTrunkDevice[mPrevDeviceIndex].fDeviceID;
				aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fChaHuanLinkChannenlNo=aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fChannelNo;
				aryDspData[mPrevDspNo].pTrunkDevice[mPrevDeviceIndex].fLinkTrunkChannelNo=aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fChannelNo;
				aryDspData[mPrevDspNo].pTrunkDevice[mPrevDeviceIndex].fLinkTrunkDeviceID=aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDeviceID;
				SetTrunkDeviceState(&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDeviceID,VOC_WAITHANGUP);
				SetTrunkDeviceState(&aryDspData[mPrevDspNo].pTrunkDevice[mPrevDeviceIndex].fDeviceID,VOC_WAITHANGUP);
			}
			/*连接的是VOIP*/
			if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fPrevDeviceType==2)
			{
				ControlPlay_t		mControlPlay;
				DeviceID_t			mTempDeviceID;
				mPrevDeviceNo=aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fPrevVoipChannelNo;
				mTempDeviceID=aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fPrevVoipDeviceID;

				mPrevDspNo=-1;
				mPrevDeviceIndex=FindIndexByChannelNo(mPrevDspNo,mPrevDeviceNo,DEVICE_VOIP);
				mPrevDspNo=aryVoipIndex[mPrevDeviceIndex].fDspNo;
				mPrevDeviceIndex=aryVoipIndex[mPrevDeviceIndex].fIndex;

				/*断开本通道和前向通道的连接*/
				DeleteLink(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,
					&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkTrunkDeviceID);
				
				/*建立双向连接*/
				SetLink(&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDeviceID,
					    &aryDspData[mPrevDspNo].pVoipDevice[mPrevDeviceIndex].fLinkVoiceDeviceID);
				/*设置混音*/
				MixerControlParam_t paramMixer = {0};
				paramMixer.m_u8SRC1_Ctrl = XMS_MIXER_FROM_PLAY ;
				paramMixer.m_u16SRC_ChID1 = aryDspData[mPrevDspNo].pVoipDevice[mPrevDeviceIndex].fLinkVoiceDeviceID.m_s16ChannelID;
				paramMixer.m_u8SRC2_Ctrl = XMS_MIXER_FROM_INPUT;
				paramMixer.m_u16SRC_ChID2 = aryDspData[mPrevDspNo].pVoipDevice[mPrevDeviceIndex].fLinkVoiceDeviceID.m_s16ChannelID;
				//RetCode_t ret = XMS_ctsSetParam(pFlowHandle, &aryDspData[mPrevDspNo].pVoipDevice[mPrevDeviceIndex].fDeviceID,
				//	VOIP_PARAM_MIXER, sizeof(paramMixer), &paramMixer);
				XMS_ctsLinkDevice(pFlowHandle,&aryDspData[mPrevDspNo].pVoipDevice[mPrevDeviceIndex].fDeviceID,
					              &aryDspData[mPrevDspNo].pVoipDevice[mPrevDeviceIndex].fLinkVoiceDeviceID,NULL);
				/*停止放转接音*/
				mControlPlay.m_u16ControlType=XMS_STOP_PLAY;
				XMS_ctsControlPlay(pFlowHandle,&aryDspData[mPrevDspNo].pVoipDevice[mPrevDeviceIndex].fLinkVoiceDeviceID,&mControlPlay,NULL);
				AddMsg("叉簧停止播放转接音");
				aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fisLinkVoipVoice=true;
				aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fIsLinkTrunk=false;
				aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fIsChaHuan=false;

				aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkVoipVoiceDeviceNo=aryDspData[mPrevDspNo].pVoipDevice[mPrevDeviceIndex].fLinkVoiceChannelNo;
				aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkVoipVoiceDeviceID=aryDspData[mPrevDspNo].pVoipDevice[mPrevDeviceIndex].fLinkVoiceDeviceID;

				strcpy(aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDtmfBuff,"");
				SetTrunkDeviceState(&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDeviceID,VOC_WAITHANGUP);
				SetVoipDeviceState(&aryDspData[mPrevDspNo].pVoipDevice[mPrevDeviceIndex].fDeviceID,VOIP_CONNECTED);
			}
		}
	}
	/*本通道数据初始化*/
	/*仅仅是连接语音通道*/
	//AddMsg(GetVoiceStatusString(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode));
	if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode==VOC_ERROR_CALL_LONG ||
		aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode==VOC_PLAY_NOTICE ||
		aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode==VOC_PLAY_NUMBER ||
		aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode==VOC_PLAY_DIAL_WRONG ||
		aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode==VOC_PLAY_WELCOME)
	{
		/*先停止放音*/
		ControlPlay_t	mPlayControl;
		mPlayControl.m_u16ControlType=XMS_STOP_PLAY;
		XMS_ctsControlPlay(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID,&mPlayControl,NULL);
		AddMsg("单纯播放音停止播放");
		DeleteLink(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID,
			&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID);
		aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkVoice=false;
		SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,VOC_FREE);
		XMS_ctsResetDevice(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID,NULL);
		XMS_ctsGetDevState(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID,NULL);
	}

	aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkTrunk=false;
	aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fisLinkVoipVoice=false;
	aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkVoice=false;
	aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkTrunkChannelNo=-1;
	strcpy(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff,"");		
	strcpy(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fCallInNumber,"");
	strcpy(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fCallOutNumber,"");
	aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsChaHuan=false;
	aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fChaHuanCount=0;
	aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fPrevDeviceType=-1;
	aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fPrevTrunkChannelNo=-1;
	aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fPrevVoipChannelNo=-1;
//	PlayTone(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID,STOP_TONE);
//	ControlPlay_t	mTempControl;
//	mTempControl.m_u16ControlType=XMS_STOP_PLAY;
//	XMS_ctsControlPlay(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID,&mTempControl,NULL);
//	XMS_ctsResetDevice(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID,NULL);
//	XMS_ctsGetDevState(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID,NULL);
	SetTrunkDeviceState(pDevice,VOC_WAIT_WORK);
	XMS_ctsResetDevice(pFlowHandle,pDevice,NULL);
	XMS_ctsGetDevState(pFlowHandle,pDevice,NULL);
	return;
}

/***********************************************************************
功能：根据电话号码查找所对应的内线通道，返回内线通道号的全局设备编号
***********************************************************************/
long	FindChannelNoByPhoneNumber(char	*	mPhoneNumber)
{
	long	mChannelNo;
	long	i;
	long	j;
	bool	mFound;

	mFound=false;
	i=0;
	mChannelNo=-1;
	while(i<=mDspCount+1 && !mFound)
	{
		if(aryDspData[i].fIsExits==1)
		{
			j=0;
			while(j<=aryDspData[i].fTrunkCount && !mFound)
			{
				if(j==145)
				{
					char	mTemp[20];
					strcpy(mTemp,aryDspData[i].pTrunkDevice[j].fLocalNumber);
				}
				if(strcmp(aryDspData[i].pTrunkDevice[j].fLocalNumber,mPhoneNumber)==0)
				{
					mChannelNo=aryDspData[i].pTrunkDevice[j].fChannelNo;
					mFound=true;
				}
				j++;
			}
		}
		i++;
	}
	return mChannelNo;
}


/***********************************************************************
功能 ：内线工作函数--响应和处理内线有关的所有事件
***********************************************************************/
void	UserDeviceWork(DeviceID_t	*pDevice,Acs_Evt_t	*pEvt)
{
	long		mDspNo;
	ChannelID_t	mChannelID;
	long		mIndex;
	long		mDeviceIndex;
	long		mLinkDspNo;
	long		mLinkDeviceIndex;
	long		mLinkChannelNo;

	mDspNo=pDevice->m_s8ModuleID;
	mChannelID=pDevice->m_s16ChannelID;
	mIndex=FindIndexByChannelID(mDspNo,mChannelID,DEVICE_TRUNK);
	if(mIndex==-1) return;
	mDeviceIndex=aryTrunkIndex[mIndex].fIndex;
	if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsOpen==false) return;
	
	DisplayEvt(pEvt);

	switch(pEvt->m_s32EventType)
	{
	case	XMS_EVT_CLEARCALL:					
		/*拆线事件*/
		{
			DisplayEvt(pEvt);
			ResetUserDevice(pDevice,pEvt);
			return;
		}
		break;
	case	XMS_EVT_CALLIN:
		/*用户摘机事件*/
		DisplayEvt(pEvt);
		if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode==VOC_FREE)
		{

			if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkVoice==false)
			{
				SetLink(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,
					&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID);
				aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkVoice=true;
			}
			PlayTone(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID,DIAL_TONE);
			SetTrunkDeviceState(pDevice,VOC_OFFHOOK);
		}
		break;
	case	XMS_EVT_CALLOUT:
		/*呼出事件*/
		{
			Acs_CallControl_Data	*pControlData=NULL;
			DisplayEvt(pEvt);
			pControlData=(Acs_CallControl_Data *)FetchEventData(pEvt);
			/*获取本通道的基本数据*/
			mChannelID=pDevice->m_s16ChannelID;
			mDspNo=pDevice->m_s8ModuleID;
			mIndex=FindIndexByChannelID(mDspNo,mChannelID,DEVICE_TRUNK);
			mDeviceIndex=aryTrunkIndex[mIndex].fIndex;
			/*连接的是接口设备*/
			if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkTrunk==true)
			{
				mLinkChannelNo=aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkTrunkChannelNo;
				mLinkDeviceIndex=FindIndexByChannelNo(mDspNo,mLinkChannelNo,DEVICE_TRUNK);
				mLinkDspNo=aryTrunkIndex[mLinkDeviceIndex].fDspNo;
				mLinkDeviceIndex=aryTrunkIndex[mLinkDeviceIndex].fIndex;
				if(pControlData->m_s32AcsEvtState==1)
				{
					/*呼叫成功*/
					/*将呼叫端接口通道与语音通道之间的连接断开*/
					if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsChaHuan==false)
					{
						PlayTone(&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkVoiceDeviceID,STOP_TONE);
						DeleteLink(&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDeviceID,
							&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkVoiceDeviceID);
						XMS_ctsResetDevice(pFlowHandle,&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkVoiceDeviceID,NULL);
						XMS_ctsGetDevState(pFlowHandle,&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkVoiceDeviceID,NULL);

						aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fIsLinkVoice=false;
						/*建立双向连接*/
						SetLink(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,
							&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDeviceID);
						aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkTrunk=true;
						aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkVoice=false;			/*本地的语音连接也被断开*/
						aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fIsLinkTrunk=true;
						aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fIsLinkVoice=false;	/*原有的VOICE连接已经被断开*/
						/*由于系统底层的错误*/
						aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fChaHuanIsCallStatus=true;
						SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,VOC_CONNECTED);
						SetTrunkDeviceState(&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDeviceID,VOC_CONNECTED);
					}
					else
					{
						/*如果是叉簧，此时，双方的接口通道已经建立时隙连接，所以双方的语音通道已经关闭*/
						long		mPrevChannelNo;
						DJ_S32		mPrevDspNo;
						long		mPrevIndex;
						mPrevChannelNo=aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fChaHuanLinkChannenlNo;
						if(mPrevChannelNo>=0)
						{
							mPrevDspNo=-1;
							mPrevIndex=FindIndexByChannelNo(mPrevDspNo,mPrevChannelNo,DEVICE_TRUNK);
							mPrevDspNo=aryTrunkIndex[mPrevIndex].fDspNo;
							mPrevIndex=aryTrunkIndex[mPrevIndex].fIndex;
							if(aryDspData[mPrevDspNo].pTrunkDevice[mPrevIndex].fChaHuanIsReset==true)
							{
								/*在呼叫成功之前就挂机了。则直接建立外线和内线的连接*/
								ControlPlay_t	mControlPlay;
								DeleteLink(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,
									&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID);
								aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkVoice=false;
								XMS_ctsResetDevice(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID,NULL);
								XMS_ctsGetDevState(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID,NULL);

								mControlPlay.m_u16ControlType=XMS_STOP_PLAY;
								XMS_ctsControlPlay(pFlowHandle,&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkVoiceDeviceID,&mControlPlay,NULL);
								AddMsg("呼叫前挂机停止播放");
								DeleteLink(&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDeviceID,
									&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkVoiceDeviceID);
								XMS_ctsResetDevice(pFlowHandle,&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkVoiceDeviceID,NULL);
								XMS_ctsGetDevState(pFlowHandle,&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkVoiceDeviceID,NULL);
								aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fIsLinkVoice=false;
								SetLink(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,
									&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDeviceID);
								aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkTrunk=true;
								aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fIsLinkTrunk=true;
							}
						}
					}
				}
				else
				{
					/*呼叫失败，首先检查是不是属于拍叉簧流程，不是，则正常处理*/
					if(aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fIsChaHuan==false)
					{
						XMS_ctsResetDevice(pFlowHandle,&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkVoiceDeviceID,NULL);
						XMS_ctsGetDevState(pFlowHandle,&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkVoiceDeviceID,NULL);
						if(aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fIsLinkVoice==false)
						{
							SetLink(&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDeviceID,
								&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkVoiceDeviceID);
							aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fIsLinkVoice=true;
						}
						PlayTone(&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkVoiceDeviceID,BUSY_TONE);
						aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fIsLinkTrunk=false;
						ResetUserDevice(pDevice,pEvt);

					}
					else
					{
						/*拍叉簧时，如果呼叫对方失败，则重新接回来*/
						/*接回原有通道*/
						if(aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fPrevDeviceType==1 && 
						   aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLineState==DCS_CONNECT)
						{	
							/*原通道为接口设备*/
							ControlPlay_t		mPlayControl;
							DJ_S32				mPrevDspNo;
							long				mPrevDeviceNo;
							long				mPrevDeviceIndex;

							mPlayControl.m_u16ControlType=XMS_STOP_PLAY;
							mPrevDeviceNo=aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fPrevTrunkChannelNo;
							mPrevDspNo=-1;
							mPrevDeviceIndex=FindIndexByChannelNo(mPrevDspNo,mPrevDeviceNo,DEVICE_TRUNK);
							mPrevDspNo=aryTrunkIndex[mPrevDeviceIndex].fDspNo;
							mPrevDeviceIndex=aryTrunkIndex[mPrevDeviceIndex].fIndex;
							XMS_ctsControlPlay(pFlowHandle,&aryDspData[mPrevDspNo].pTrunkDevice[mPrevDeviceIndex].fLinkVoiceDeviceID,&mPlayControl,NULL);
							XMS_ctsResetDevice(pFlowHandle,&aryDspData[mPrevDspNo].pTrunkDevice[mPrevDeviceIndex].fLinkVoiceDeviceID,NULL);
							XMS_ctsGetDevState(pFlowHandle,&aryDspData[mPrevDspNo].pTrunkDevice[mPrevDeviceIndex].fLinkVoiceDeviceID,NULL);

							AddMsg("叉簧放音停止");
							SetLink(&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDeviceID,
								    &aryDspData[mPrevDspNo].pTrunkDevice[mPrevDeviceIndex].fDeviceID);
							aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fIsLinkTrunk=true;
							aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fisLinkVoipVoice=false;
							aryDspData[mPrevDspNo].pTrunkDevice[mPrevDeviceIndex].fIsLinkTrunk=true;
							aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fIsLinkVoice=false;
							aryDspData[mPrevDspNo].pTrunkDevice[mPrevDeviceIndex].fIsLinkVoice=false;
							aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkTrunkChannelNo=aryDspData[mPrevDspNo].pTrunkDevice[mPrevDeviceIndex].fChannelNo;
							aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkTrunkDeviceID=aryDspData[mPrevDspNo].pTrunkDevice[mPrevDeviceIndex].fDeviceID;
							strcpy(aryDspData[mPrevDspNo].pTrunkDevice[mPrevDeviceIndex].fDtmfBuff,"");
							strcpy(aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDtmfBuff,"");
							aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fPrevDeviceType=1;
						}
						if(aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fPrevDeviceType==2 &&
						   aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLineState==DCS_CONNECT)
						{
							/*原通道为VOIP设备*/
							ControlPlay_t		mPlayControl;
							DJ_S32				mPrevDspNo;
							long				mPrevDeviceNo;
							long				mPrevDeviceIndex;
							
							mPlayControl.m_u16ControlType=XMS_STOP_PLAY;
							mPrevDeviceNo=aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fPrevVoipChannelNo;
							mPrevDspNo=-1;
							mPrevDeviceIndex=FindIndexByChannelNo(mPrevDspNo,mPrevDeviceNo,DEVICE_VOIP);
							mPrevDspNo=aryTrunkIndex[mPrevDeviceIndex].fDspNo;
							mPrevDeviceIndex=aryTrunkIndex[mPrevDeviceIndex].fIndex;
							XMS_ctsControlPlay(pFlowHandle,&aryDspData[mPrevDspNo].pVoipDevice[mPrevDeviceIndex].fLinkVoiceDeviceID,&mPlayControl,NULL);
							XMS_ctsResetDevice(pFlowHandle,&aryDspData[mPrevDspNo].pVoipDevice[mPrevDeviceIndex].fLinkVoiceDeviceID,NULL);
							XMS_ctsGetDevState(pFlowHandle,&aryDspData[mPrevDspNo].pVoipDevice[mPrevDeviceIndex].fLinkVoiceDeviceID,NULL);

							AddMsg("叉簧VOIP停止放音");
							/*将语音通道与接口通道进行绑定*/
							SetLink(&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDeviceID,
								&aryDspData[mPrevDspNo].pVoipDevice[mPrevDeviceIndex].fLinkVoiceDeviceID);
							aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fIsLinkTrunk=false;
							aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fIsLinkVoice=false;
							aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fisLinkVoipVoice=true;
							aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkTrunkChannelNo=-1;
							aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkVoipVoiceDeviceNo=aryDspData[mPrevDspNo].pVoipDevice[mPrevDeviceIndex].fLinkVoiceChannelNo;
							aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkVoipVoiceDeviceID=aryDspData[mPrevDspNo].pVoipDevice[mPrevDeviceIndex].fLinkVoiceDeviceID;	
							aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fPrevDeviceType=2;
						}
						/*将拍叉簧数据清除*/
						aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fIsChaHuan=false;
						aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fChaHuanCount=0;
						SetTrunkDeviceState(&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDeviceID,VOC_CONNECTED);
					}
				}
			} 
			/*如果连接的是VOIP设备*/
			if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fisLinkVoipVoice==true)
			{
				if(pControlData->m_s32AcsEvtState==1)
				{
					PlayTone(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoipVoiceDeviceID,STOP_TONE);
					/*将VOIP的语音通道与坐席建立双向连接*/
					SetLink(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,
							&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoipVoiceDeviceID);
					SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,VOC_CONNECTED);

					/*新加入代码*/
					DJ_S32		mVoipDspNo;
					long		mVoipDeviceIndex;
					long		mVoipDeviceNo;
					mVoipDeviceNo=aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoipDeviceNo;
					mVoipDspNo=-1;
					mVoipDeviceIndex=FindIndexByChannelNo(mVoipDspNo,mVoipDeviceNo,DEVICE_VOIP);
					mVoipDeviceIndex=aryVoipIndex[mVoipDeviceIndex].fIndex;
					mVoipDspNo=aryVoipIndex[mVoipDeviceIndex].fDspNo;
					aryDspData[mVoipDspNo].pVoipDevice[mVoipDeviceIndex].fIsLinkTrunk=true;
				}
				else
				{
					/*VOIP呼叫座席，座席无应答*/
					long	mVoipDspNo;
					long	mVoipDeviceIndex;
					long	mVoipDeviceNo;
					mVoipDeviceNo=aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoipDeviceNo;
					mVoipDspNo=-1;
					mVoipDeviceIndex=FindIndexByChannelNo(mVoipDspNo,mVoipDeviceNo,DEVICE_VOIP);
					mVoipDspNo=aryVoipIndex[mVoipDeviceIndex].fDspNo;
					mVoipDeviceIndex=aryVoipIndex[mVoipDeviceIndex].fIndex;
					/*向对方发拆线消息*/
					XMS_ctsClearCall(pFlowHandle,&aryDspData[mVoipDspNo].pVoipDevice[mVoipDeviceIndex].fDeviceID,0,NULL);
					ResetVoipDevice(&aryDspData[mVoipDspNo].pVoipDevice[mVoipDeviceIndex].fDeviceID,NULL);
					ResetUserDevice(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,NULL);
				}
			}
		}
		break;
	case	XMS_EVT_LINKDEVICE:
		/*建立连接事件*/
		break;
	case	XMS_EVT_UNLINKDEVICE:
		/*解除连接事件*/
		break;
	case	XMS_EVT_SENDIODATA:
		/*发送数据事件*/
		{
			Acs_IO_Data	 *pSendData;
			pSendData=(Acs_IO_Data *)FetchEventData(pEvt);
			char	mTemp[20];
			itoa(mChannelID,mTemp,10);
			AddMsg(mTemp);
		}
		break;
	case	XMS_EVT_ANALOG_INTERFACE:
		/*拍叉簧事件*/
		{
			Acs_AnalogInterface_Data *pFlashEvt = (Acs_AnalogInterface_Data *)FetchEventData(pEvt);
			DisplayEvt(pEvt);
			if(pFlashEvt->m_u8AnalogInterfaceState==XMS_ANALOG_USER_CH_FLASH)
			{
				if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode==VOC_PLAY_NOTICE ||
				   aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode==VOC_PLAY_NUMBER ||
				   aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode==VOC_PLAY_WELCOME ||
				   aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode==VOC_ERROR_CALL_LONG ||
				   aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode==VOC_PLAY_DIAL_WRONG)	return;

				if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode==VOC_CONNECTED ||
					aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode==VOC_WAITHANGUP)
				{
					aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fChaHuanCount++;
					strcpy(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff,"");
					if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fChaHuanCount>2)
						aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fChaHuanCount=1;
					/*叉簧处理函数*/
					aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsChaHuan=true;
					ChaHuanProcess(mDspNo,mDeviceIndex);
				}
			}
		}
		break;
	case	XMS_EVT_PLAY:
		/*放音完成*/
		{
			Acs_MediaProc_Data		*pMediaData=NULL;
			long			mCode;
			DisplayEvt(pEvt);
			pMediaData=(Acs_MediaProc_Data *)FetchEventData(pEvt);
			mCode=pMediaData->m_s32AcsEvtErrCode;
			if(mCode==0)	AddMsg("正常播放");
			if(mCode==1)	AddMsg("收到应用程序停止指令");
			if(mCode==2)	AddMsg("收到DTMF或者TOne音");
			if(mCode==3)	AddMsg("打开文件失败");
			if(mCode==4)	AddMsg("该语音通道故障");
			if(mCode==5)	AddMsg("与硬件模块网络断开");

			if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode==VOC_FREE)return;
			/*如果当前的状态是播放非法拨长途音，则放完之后，接着放忙音，催挂*/
			if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode==VOC_PLAY_NUMBER 
			   || aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode==VOC_PLAY_NOTICE 
			   || aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode==VOC_ERROR_CALL_LONG
			   || aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode==VOC_PLAY_DIAL_WRONG 
			   || aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode==VOC_PLAY_WELCOME)
			{
				if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkVoice==false)
				{
					SetLink(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,
						&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID);
					aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkVoice=true;
				}
				PlayTone(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID,BUSY_TONE);
				if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode!=VOC_FREE)
					SetTrunkDeviceState(pDevice,VOC_WAITHANGUP);
			}
			if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode==VOC_PLAY_VOIP_ERROR)
			{
				/*播放VOIP错误提示音的特殊处理*/
				XMS_ctsClearCall(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoipDeviceID,0,NULL);
				ResetUserDevice(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,NULL);
				ResetVoipDevice(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoipDeviceID,NULL);
				return;
			}
		}
		break;
	case	XMS_EVT_SETPARAM:
		{
			AddMsg("设置完成");
		}
		break;
	case	XMS_EVT_RECVIODATA:				
		/*接收数据事件--DTMF*/
		{
			char	*pDtmf;
			Acs_IO_Data			*pIoData;
			if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode==VOC_OFFHOOK || 
			   aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode==VOC_FLASH_WAIT_DIAL)
			{
				pIoData=(Acs_IO_Data *)FetchEventData(pEvt);
				if(pIoData->m_u16IoDataLen>0 && pIoData->m_u16IoType==XMS_IO_TYPE_DTMF 
					&& aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode!=VOC_PLAY_NUMBER
					&& aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode!=VOC_PLAY_NOTICE
					&& aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode!=VOC_PLAY_DIAL_WRONG
					&& aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode!=VOC_PLAY_WELCOME
					&& aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode!=VOC_ERROR_CALL_LONG)
				{
					pDtmf=(char *)FetchIOData(pEvt);
					/*停止放拨号音*/
					PlayTone(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID,STOP_TONE);
					InsertDtmfToDevice(mDspNo,mDeviceIndex,DEVICE_TRUNK,pDtmf);
					AddMsg(pDtmf);

				}
				if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff[0]=='9' &&
				   aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode!=VOC_FLASH_WAIT_DIAL)
				{
					/*呼叫外线*/
					strcpy(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff,"");
					CallTrunk(mDspNo,mDeviceIndex,false);
				}
				if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff[0]=='0' &&
				   aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode!=VOC_FLASH_WAIT_DIAL)
				{
					/*呼叫外线,并且是呼叫长途*/
					strcpy(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff,"");
					CallTrunk(mDspNo,mDeviceIndex,true);
				}
				if(strcmp(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff,"##00")==0 && 
				   aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode!=VOC_FLASH_WAIT_DIAL)
				{
					/*查询长途开通状态*/
					strcpy(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff,"");
					QueryCallLongStatus(mDspNo,mDeviceIndex);
					return;
				}
				if(strcmp(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff,"##01")==0 &&
				   aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode!=VOC_FLASH_WAIT_DIAL)
				{
					/*查询本机号码*/
					strcpy(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff,"");
					PlayPhoneNumber(mDspNo,mDeviceIndex);
					return;
				}
				if(strcmp(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff,"##02")==0 && 
				   aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode!=VOC_FLASH_WAIT_DIAL)
				{
					/*开启长途功能*/
					strcpy(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff,"");
					SetEnableCallLong(mDspNo,mDeviceIndex);
					return;
				}
				if(strcmp(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff,"##03")==0 &&
				   aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode!=VOC_FLASH_WAIT_DIAL)
				{
					/*关闭长途功能*/
					strcpy(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff,"");
					SetDisEnableCallLong(mDspNo,mDeviceIndex);
					return;
				}
				if(strcmp(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff,"##07")==0 && 
				   aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode!=VOC_FLASH_WAIT_DIAL)
				{
					/*关闭电话*/
					strcpy(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff,"");
					SetDisableCall(mDspNo,mDeviceIndex);
					return;
				}
				if(strcmp(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff,"##08")==0 &&
				   aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode!=VOC_FLASH_WAIT_DIAL)
				{
					/*开启电话*/
					strcpy(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff,"");
					SetEnableCall(mDspNo,mDeviceIndex);
					return;
				}
				/*播放功能菜单*/					
				/*if(strcmp(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff,"##09")==0 && 
				   aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode!=VOC_FLASH_WAIT_DIAL)
				{
					strcpy(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff,"");
					PlayMenu(mDspNo,mDeviceIndex);
					return;
				}
				*/

				/*缩位拨号*/
				char mSpeedDial[20];
                mSpeedDial[0]=aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff[0];
				mSpeedDial[1]='\0';

				char mTargetUserNumber[20];
				if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff[0]==
					aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fSpeedDial[0])
				{
					if(strlen(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fTargetUserNo)<=0)	return;
					strcpy(mTargetUserNumber,aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fTargetUserNo);
					if(mTargetUserNumber[0]=='7' &&
						aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode!=VOC_FLASH_WAIT_DIAL)
					{
						strcpy(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff,mTargetUserNumber);
						CallVoip(mDspNo,mDeviceIndex,false);
					}else if(mTargetUserNumber[0]=='6' &&
						aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode!=VOC_FLASH_WAIT_DIAL)
					{
						strcpy(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff,mTargetUserNumber);
						CallUser(mDspNo,mDeviceIndex);
					}
				}

				/*
				long mChannelNo=FindChannelNoBySpeedDial(mSpeedDial);
				if(mChannelNo>0)
				{
					if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode!=VOC_FLASH_WAIT_DIAL)
					{
						DJ_S32	mSpeedDspNo;
						long	mSpeedDeviceIndex;
						mSpeedDspNo=-1;
						mSpeedDeviceIndex=FindIndexByChannelNo(mSpeedDspNo,mChannelNo,DEVICE_TRUNK);
						mSpeedDspNo=aryTrunkIndex[mSpeedDeviceIndex].fDspNo;
						mSpeedDeviceIndex=aryTrunkIndex[mSpeedDeviceIndex].fIndex;
						strcpy(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff,
							aryDspData[mSpeedDspNo].pTrunkDevice[mSpeedDeviceIndex].fLocalNumber);
						CallUser(mDspNo,mDeviceIndex);	
					}
				}
				*/
               /*内线拨打IP会议*/
				if(strlen(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff)==5 &&
					aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff[0]=='8' &&
					aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode!=VOC_FLASH_WAIT_DIAL
					)
				{
					/*呼叫总部，走VOIP*/
					/*unsigned int i;
					for(i=0;i<strlen(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff);i++)
						aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff[i]=aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff[i+1];
					aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff[i]='\0';
					*/
					CallVoip(mDspNo,mDeviceIndex,false);
				}

                   /*内线拨打IP*/
				if(strlen(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff)==4 &&
					aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff[0]=='7' &&
					   aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode!=VOC_FLASH_WAIT_DIAL
					)
				{
					/*呼叫总部，走VOIP*/
					CallVoip(mDspNo,mDeviceIndex,false);
				}

				if(strlen(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff)==4)
				{
					/*呼叫内线或者呼叫总部*/

					if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff[0]=='5')
					{
						/*内线呼叫内线*/
						CallUser(mDspNo,mDeviceIndex);	
					}
					else if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff[0]=='7' &&
					   aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode!=VOC_FLASH_WAIT_DIAL)
					{
						/*呼叫总部，走VOIP*/
						//CallVoip(mDspNo,mDeviceIndex,false);
					}
					else
					{
						if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff[0]!='0' && 
						   aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff[0]!='9')
						{
							return;
							if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkVoice==false)
							{
								SetLink(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,
									&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID);
								aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkVoice=true;
							}
							PlayFile(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID,CALL_NUMBER_NOT_EXIT_VOC,false);
							/*将本通道的状态设置为非法呼叫长途状态*/
							SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,VOC_PLAY_NOTICE);
						}
					}
				}
				//异常拨号
				if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff[0]!='0' && 
					aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff[0]!='9' && 
					strlen(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff)>4)
				{
					return;
					if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkVoice==false)
					{
						SetLink(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,
							&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID);
						aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkVoice=true;
					}
					PlayFile(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID,CALL_NUMBER_NOT_EXIT_VOC,false);
					/*将本通道的状态设置为非法呼叫长途状态*/
					SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,VOC_PLAY_NOTICE);
				}

			}
			/*下段代码专门用于收拨外线的号码*/
			if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode==VOC_CALL_OUT &&
			   aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkTrunk==true)
			{
				/*进行收号*/
				long	mLinkChannelNo;
				long	mLinkDspNo;
				long	mLinkDeviceIndex;
				pIoData=(Acs_IO_Data *)FetchEventData(pEvt);
				if(pIoData->m_u16IoDataLen>0 && pIoData->m_u16IoType==XMS_IO_TYPE_DTMF)
				{
					pDtmf=(char *)FetchIOData(pEvt);
					strcpy(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fCallInNumber,aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLocalNumber);
					InsertDtmfToDevice(mDspNo,mDeviceIndex,DEVICE_TRUNK,pDtmf);
					mLinkChannelNo=aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkTrunkChannelNo;
					mLinkDspNo=mDspNo;
					mLinkDeviceIndex=FindIndexByChannelNo(mLinkDspNo,mLinkChannelNo,DEVICE_TRUNK);
					mLinkDspNo=aryTrunkIndex[mLinkChannelNo].fDspNo;
					mLinkDeviceIndex=aryTrunkIndex[mLinkChannelNo].fIndex;
					strcpy(aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fCallInNumber,aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLocalNumber);
					InsertDtmfToDevice(mLinkDspNo,mLinkDeviceIndex,DEVICE_TRUNK,pDtmf);
				}
			}
		}
		break;
	}
	/*
	if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode==VOC_WAITHANGUP)
	{
		if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkVoice==false)
		{
			SetLink(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,
				&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID);
			aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkVoice=true;
		}
		PlayTone(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID,STOP_TONE);
		SetTrunkDeviceState(pDevice,VOC_FREE);
	}*/
	return;
}

/***********************************************************************
功能：外线工作函数--响应和处理外线有关的所有事件
***********************************************************************/
void	TrunkDeviceWork(DeviceID_t	*pDevice,Acs_Evt_t	*pEvt)
{
	long		mDspNo;
	ChannelID_t	mChannelID;
	long		mIndex;
	long		mDeviceIndex;
	long		mLinkDspNo;
	long		mLinkDeviceIndex;
	long		mLinkDeviceNo;
	
 	mDspNo=pDevice->m_s8ModuleID;
	mChannelID=pDevice->m_s16ChannelID;
	mIndex=FindIndexByChannelID(mDspNo,mChannelID,DEVICE_TRUNK);
	if(mIndex==-1) return;
	mDeviceIndex=aryTrunkIndex[mIndex].fIndex;
	if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsOpen==false) return;
	switch(pEvt->m_s32EventType)
	{
	case	XMS_EVT_CLEARCALL:					
		/*拆线事件*/
		{
			DisplayEvt(pEvt);

			ResetTrunkDevice(pDevice,pEvt);
			return;
		}
		break;
	case	XMS_EVT_CAS_MFC_START:
		{
			CmdParamData_CAS_t		g_Param_CAS;
			g_Param_CAS.m_VocDevID = aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID;
			XMS_ctsSetParam ( pFlowHandle, &aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,
				CAS_PARAM_UNIPARAM, sizeof(CmdParamData_CAS_t), (DJ_Void *)&g_Param_CAS );
		}
		break;
	case	XMS_EVT_CAS_MFC_END:
		{

		}
		break;
	case	XMS_EVT_DEV_TIMER:
		{
			long	mLinkVoiceIndex;
			DJ_S32	mLinkVoiceDspNo;
			long	mLinkTrunkIndex;
			DJ_S32	mLinkTrunkDspNo;
			
			if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode==VOC_HUIXIAN_TURN)
			{
					/*汇线通转接*/
					XMS_ctsSendIOData(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID,XMS_IO_TYPE_DTMF,4,"*72#");
					XMS_ctsSetDevTimer(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,1500);
					SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,VOC_WAIT_MAIN);
					return;
			}
			else if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode==VOC_WAIT_MAIN)
			{
					if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkVoice==false)
					{
						SetLink(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,
							&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID);
						aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkVoice=true;
					}
					SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,VOC_PLAY_WELCOME);
					PlayFile(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID,SYS_WELCOME_FILE,true);
			}
			else
			{
				
					if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode==VOC_WAIT_FIRST_DIAL_TONE)
					{
						if(strcmp(mPrevCode,"")!=0)
							XMS_ctsSendIOData(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID,XMS_IO_TYPE_DTMF,strlen(mPrevCode),mPrevCode);
						
						/*查找相连的内线通道*/
						mLinkTrunkIndex=FindIndexByChannelNo(mDspNo,aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkTrunkChannelNo,DEVICE_TRUNK);
						mLinkTrunkDspNo=aryTrunkIndex[mLinkTrunkIndex].fDspNo;
						mLinkTrunkIndex=aryTrunkIndex[mLinkTrunkIndex].fIndex;
						/*查找相连的内线通道绑定的语音通道*/
						mLinkVoiceIndex=FindIndexByChannelID(aryDspData[mLinkTrunkDspNo].pTrunkDevice[mLinkTrunkIndex].fLinkVoiceDeviceID.m_s8ModuleID,aryDspData[mLinkTrunkDspNo].pTrunkDevice[mLinkTrunkIndex].fLinkVoiceDeviceID.m_s16ChannelID,DEVICE_VOICE);
						mLinkVoiceDspNo=aryVoiceIndex[mLinkVoiceIndex].fDspNo;
						mLinkVoiceIndex=aryVoiceIndex[mLinkVoiceIndex].fIndex;
						

						/*断开本通道相连的语音通道连接*/
						DeleteLink(&aryDspData[mLinkTrunkDspNo].pTrunkDevice[mLinkTrunkIndex].fDeviceID,
								   &aryDspData[mLinkTrunkDspNo].pTrunkDevice[mLinkTrunkIndex].fLinkVoiceDeviceID);


						aryDspData[mLinkTrunkDspNo].pTrunkDevice[mLinkTrunkIndex].fIsLinkVoice=false;
						aryDspData[mLinkVoiceDspNo].pVoiceDevice[mLinkVoiceIndex].fIsLinkTrunk=false;
						/*将内线通道和外线通道之间建立时隙连接*/
						SetLink(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
								&aryDspData[mLinkTrunkDspNo].pTrunkDevice[mLinkTrunkIndex].fDeviceID);
						/*修改两个通道的状态*/
						SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,VOC_CALL_OUT);
						SetTrunkDeviceState(&aryDspData[mLinkTrunkDspNo].pTrunkDevice[mLinkTrunkIndex].fDeviceID,VOC_CALL_OUT);

						/*后续处理转到外线呼叫的返回事件*/
						/*为了能够接收到被叫号码，所以将中继通道相连的语音通道建立一个单向的连接*/
						//XMS_ctsLinkDevice(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
						//				  &aryDspData[mLinkVoiceDspNo].pVoiceDevice[mLinkVoiceIndex].fDeviceID,NULL);
						return;
					}
			}
	}
		break;
	case	XMS_EVT_CALLIN:
		/*呼入事件*/
		{
			Acs_CallControl_Data	*pControlData=NULL;
			pControlData=(Acs_CallControl_Data *)FetchEventData(pEvt);			
			/*将主叫收回来*/
			strcpy(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fCallInNumber,aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLocalNumber);
			/*判断是否是汇线通号码转接过来的*/
			if(strcmp(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fCallInNumber,mFirstPhone)==0)
			{
				long					mFreeTrunkIndex;
				mFreeTrunkIndex=FindHuiXianFreeTrunkToUse();
				if(mFreeTrunkIndex>0)
				{
					DJ_S32		mFreeDspNo;
					long		mFreeIndex;
					mFreeDspNo=aryTrunkIndex[mFreeTrunkIndex].fDspNo;
					mFreeIndex=aryTrunkIndex[mFreeTrunkIndex].fIndex;
					DeleteLink(&aryDspData[mFreeDspNo].pTrunkDevice[mFreeIndex].fDeviceID,
						&aryDspData[mFreeDspNo].pTrunkDevice[mFreeIndex].fLinkVoiceDeviceID);
					aryDspData[mFreeDspNo].pTrunkDevice[mFreeIndex].fIsLinkVoice=false;
					
					SetTrunkLineInitParameter(&aryDspData[mFreeDspNo].pTrunkDevice[mFreeIndex].fDeviceID,
							&aryDspData[mFreeDspNo].pTrunkDevice[mFreeIndex].fLinkVoiceDeviceID);
					DJ_S32		mVoiceDspNo;
					long		mVoiceIndex;
					mVoiceDspNo=aryDspData[mFreeDspNo].pTrunkDevice[mFreeIndex].fLinkVoiceDeviceID.m_s8ModuleID;
					mVoiceIndex=FindIndexByChannelID(mVoiceDspNo,aryDspData[mFreeDspNo].pTrunkDevice[mFreeIndex].fLinkVoiceDeviceID.m_s16ChannelID,DEVICE_VOICE);
					mVoiceDspNo=aryVoiceIndex[mVoiceIndex].fDspNo;
					mVoiceIndex=aryVoiceIndex[mVoiceIndex].fIndex;
					aryDspData[mVoiceDspNo].pVoiceDevice[mVoiceIndex].fLinkTrunkDeviceID=aryDspData[mFreeDspNo].pTrunkDevice[mFreeIndex].fDeviceID;
					aryDspData[mVoiceDspNo].pVoiceDevice[mVoiceIndex].fLinkTrunkChannelNo=aryDspData[mFreeDspNo].pTrunkDevice[mFreeIndex].fChannelNo;

					XMS_ctsMakeCallOut(pFlowHandle,&aryDspData[mFreeDspNo].pTrunkDevice[mFreeIndex].fDeviceID,"","",NULL);
					//Sleep(3000);
					//XMS_ctsSendIOData(pFlowHandle,&aryDspData[mFreeDspNo].pTrunkDevice[mFreeIndex].fLinkVoiceDeviceID,XMS_IO_TYPE_DTMF,strlen(mTurnCode),mTurnCode);
					//Sleep(1000);
					SetTrunkDeviceState(&aryDspData[mFreeDspNo].pTrunkDevice[mFreeIndex].fDeviceID,VOC_HUIXIAN_TURN);
					XMS_ctsSetDevTimer(pFlowHandle,&aryDspData[mFreeDspNo].pTrunkDevice[mFreeIndex].fDeviceID,2000);
					/*开始放欢迎音*/
					/*正常的接续*/
					/*
					if(aryDspData[mFreeDspNo].pTrunkDevice[mFreeIndex].fIsLinkVoice==false)
					{
						SetLink(&aryDspData[mFreeDspNo].pTrunkDevice[mFreeIndex].fDeviceID,
							   &aryDspData[mFreeDspNo].pTrunkDevice[mFreeIndex].fLinkVoiceDeviceID);
						aryDspData[mFreeDspNo].pTrunkDevice[mFreeIndex].fIsLinkVoice=true;

					}
					*/
				//	PlayFile(&aryDspData[mFreeDspNo].pTrunkDevice[mFreeIndex].fLinkVoiceDeviceID,SYS_WELCOME_FILE,true);
					return;
				}
			}
		
			/*正常的接续*/
			XMS_ctsAlertCall(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,NULL);
			XMS_ctsAnswerCallIn(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,NULL);
			//SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,VOC_CALL_IN);
			/*等待用户拨号*/
			SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,VOC_ANSWER_CALL);
		}
		break;
	case	XMS_EVT_ANSWERCALL:
		/*外线应答成功*/
		{
			if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode==VOC_ANSWER_CALL)
			{
				ControlPlay_t			mControlPlay;
				mControlPlay.m_u16ControlType=XMS_STOP_PLAY;
				/*正常的接续*/
				XMS_ctsResetDevice(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID,NULL);
				XMS_ctsGetDevState(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID,NULL);
				if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkVoice==false)
				{
					SetLink(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,
						&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID);
					aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkVoice=true;
				}
				//XMS_ctsControlPlay(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID,&mControlPlay,NULL);
				//SetVoiceGTD(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID);
				PlayFile(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID,SYS_WELCOME_FILE,true);
				SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,VOC_PLAY_WELCOME);
			}
		}
		break;
	case	XMS_EVT_CALLOUT:
		/*呼出事件*/
		{
			Acs_CallControl_Data		*pControlData=NULL;
			pControlData=(Acs_CallControl_Data *)FetchEventData(pEvt);
			DisplayEvt(pEvt);
			if(pControlData->m_s32AcsEvtState==1)
			{
				/*外呼成功，不用处理*/
				if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fisLinkVoipVoice)
				{
					SetLink(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,
						&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoipVoiceDeviceID);
				}
			}
			else
			{
				/*呼叫失败*/
				/*第一步：清除内线通道和外线通道之间的连接*/
				mChannelID=pDevice->m_s16ChannelID;
				mDspNo=pDevice->m_s8ModuleID;
				mIndex=FindIndexByChannelID(mDspNo,mChannelID,DEVICE_TRUNK);
				mDeviceIndex=aryTrunkIndex[mDspNo].fIndex;
				mLinkDeviceNo=aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkTrunkChannelNo;
				mLinkDeviceIndex=FindIndexByChannelNo(mDspNo,mLinkDeviceNo,DEVICE_TRUNK);
				if(mLinkDeviceIndex<0)
				{
					/*规避底层错误*/
					return;
				}
				mLinkDspNo=aryTrunkIndex[mLinkDeviceIndex].fDspNo;
				mLinkDeviceIndex=aryTrunkIndex[mLinkDeviceIndex].fIndex;
				DeleteLink(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,
						   &aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDeviceID);
				aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkTrunk=false;
				aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkTrunkChannelNo=-1;
				aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fIsLinkTrunk=false;
				aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkTrunkChannelNo=-1;
				SetTrunkDeviceState(&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDeviceID,VOC_FREE);
				SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,VOC_WAITHANGUP);
				/*第二步：对内线通道放忙音*/
				XMS_ctsResetDevice(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID,NULL);
				XMS_ctsGetDevState(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID,NULL);

				if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkVoice==false)
				{
					SetLink(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,
						&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID);
					aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkVoice=true;
				}
				PlayTone(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID,BUSY_TONE);
			}
		}
		break;
	case	XMS_EVT_LINKDEVICE:
		/*建立连接事件*/
		break;
	case	XMS_EVT_PLAY:
		/*放音事件*/
		{
			/*播放欢迎提示语音完毕后的处理*/
			if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode==VOC_PLAY_WELCOME)
			{
				//if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkVoice==false)
				//{
					SetLink(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,
						&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID);
					aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkVoice=true;
				//}
				PlayTone(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID,DIAL_TONE);
				strcpy(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff,"");
				SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,VOC_WAIT_DIAL);
			}
			/*用户拨号错误，放音完毕后放忙音*/
			if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode==VOC_PLAY_DIAL_WRONG)
			{
				if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkVoice==false)
				{
					SetLink(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,
						&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID);
					aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkVoice=true;
				}
				PlayTone(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID,DIAL_TONE);
				strcpy(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff,"");
			}

		}
		break;
	case	XMS_EVT_UNLINKDEVICE:
		/*解除连接事件*/
		break;
	case	XMS_EVT_SENDIODATA:
		/*发送数据事件*/
		{
			Acs_IO_Data	 *pSendData;
			pSendData=(Acs_IO_Data *)FetchEventData(pEvt);
			if(pSendData->m_u16IoDataLen>0)
				AddMsg("发送成功抢接");
			else
				AddMsg("发送失败抢接");
			char	mTemp[10];
			itoa(mChannelID,mTemp,10);
			AddMsg(mTemp);
			if(pSendData->m_s32AcsEvtState==1)
			{
			}
			long	mLinkVoiceIndex;
			DJ_S32	mLinkVoiceDspNo;
			long	mLinkTrunkIndex;
			DJ_S32	mLinkTrunkDspNo;
			if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode==VOC_WAIT_SECOND_DIAL_TONE)
			{
					/*查找相连的内线通道*/
					//sleep(500)
					mLinkTrunkIndex=FindIndexByChannelNo(mDspNo,aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkTrunkChannelNo,DEVICE_TRUNK);
					mLinkTrunkDspNo=aryTrunkIndex[mLinkTrunkIndex].fDspNo;
					mLinkTrunkIndex=aryTrunkIndex[mLinkTrunkIndex].fIndex;
					/*查找相连的内线通道绑定的语音通道*/
					mLinkVoiceIndex=FindIndexByChannelID(aryDspData[mLinkTrunkDspNo].pTrunkDevice[mLinkTrunkIndex].fLinkVoiceDeviceID.m_s8ModuleID,aryDspData[mLinkTrunkDspNo].pTrunkDevice[mLinkTrunkIndex].fLinkVoiceDeviceID.m_s16ChannelID,DEVICE_VOICE);
					mLinkVoiceDspNo=aryVoiceIndex[mLinkVoiceIndex].fDspNo;
					mLinkVoiceIndex=aryVoiceIndex[mLinkVoiceIndex].fIndex;
					/*断开本通道相连的语音通道连接*/
					DeleteLink(&aryDspData[mLinkTrunkDspNo].pTrunkDevice[mLinkTrunkIndex].fDeviceID,
							   &aryDspData[mLinkTrunkDspNo].pTrunkDevice[mLinkTrunkIndex].fLinkVoiceDeviceID);
					aryDspData[mLinkTrunkDspNo].pTrunkDevice[mLinkTrunkIndex].fIsLinkVoice=false;
					aryDspData[mLinkVoiceDspNo].pVoiceDevice[mLinkVoiceIndex].fIsLinkTrunk=false;
					/*将内线通道和外线通道之间建立时隙连接*/
					SetLink(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
							&aryDspData[mLinkTrunkDspNo].pTrunkDevice[mLinkTrunkIndex].fDeviceID);
					/*修改两个通道的状态*/
					SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,VOC_CALL_OUT);
					SetTrunkDeviceState(&aryDspData[mLinkTrunkDspNo].pTrunkDevice[mLinkTrunkIndex].fDeviceID,VOC_CALL_OUT);

					/*后续处理转到外线呼叫的返回事件*/
					/*为了能够接收到被叫号码，所以将中继通道相连的语音通道建立一个单向的连接*/
/*					XMS_ctsLinkDevice(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
									  &aryDspData[mLinkVoiceDspNo].pVoiceDevice[mLinkVoiceIndex].fDeviceID,NULL);
*/
					return;				
			}

		}
		break;
	case	XMS_EVT_RECVIODATA:
		/*接收数据事件，由于采用模拟信令，因此，对于一些事件的返回，是采用dtmf流实现。*/
		{
			Acs_IO_Data		*pDtmfData=NULL;
			char			*pDtmfBuff;

			pDtmfData=(Acs_IO_Data *)FetchEventData(pEvt);
			if(pDtmfData->m_u16IoType==XMS_IO_TYPE_GTG && pDtmfData->m_u16IoDataLen>0)
			{
				pDtmfBuff=(char *)FetchIOData(pEvt);
				//AddMsg(pDtmfBuff);
/*				if(*pDtmfBuff=='I' || *pDtmfBuff=='J' || *pDtmfBuff=='K' ||
					*pDtmfBuff=='i' || *pDtmfBuff=='j' || *pDtmfBuff=='k' )
				{
					XMS_ctsClearCall(pFlowHandle,pDevice,0,NULL);
					ResetTrunkDevice(pDevice,pEvt);
					return;
				}
*/
				/*信号音的检测*/
				if(*pDtmfBuff=='G' || *pDtmfBuff=='g')	//如果是拨号音
				{
					if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode==VOC_WAIT_FIRST_DIAL_TONE)
					{
						//发送DTMF前导数据
						if(strcmp(mPrevCode,"")!=0)
						{
							if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkVoice==false)
							{
								SetLink(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID);
								aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkVoice=true;
							}
							//SetTrunkLineInitParameter(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID);
							//等待设置参数事件返回
							XMS_ctsSetDevTimer(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,500);
							return;
						}
					}
				}
			}
			if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode==VOC_WAIT_DIAL)
			{
				/*检测，当外线有呼入，则在此处接收用户的按键*/
				if(pDtmfData->m_u16IoType==XMS_IO_TYPE_DTMF && aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode==VOC_WAIT_DIAL
					&& pDtmfData->m_u16IoDataLen>0)
				{
					pDtmfBuff=(char *)FetchIOData(pEvt);
					if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkVoice==false)
					{
						SetLink(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,
							&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID);
						aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkVoice=true;
					}
					PlayTone(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID,STOP_TONE);
					InsertDtmfToDevice(mDspNo,mDeviceIndex,DEVICE_TRUNK,pDtmfBuff);
					if(strcmp(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff,"0")==0)
					{
						/*拨分机号码0*/
						if(strlen(pQueryPhoneNumber)>0)
						{
							strcpy(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff,pQueryPhoneNumber);
						}
					}
					if(strlen(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff)==4 && 
					   aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff[0]=='5')
					{
						strcpy(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fCallOutNumber,aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff);
						CallUserByTrunk(mDspNo,mDeviceIndex);
					}
					//加入外线转IP功能
					if(strlen(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff)==4 && 
					   aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff[0]=='7')
					{
						strcpy(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fCallOutNumber,aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff);
						CallVoip(mDspNo,mDeviceIndex,true);
					}

					if(strlen(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff)==5 &&
						aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff[0]=='8')
					{
						/*呼叫总部，走VOIP*/
						//unsigned int i;
						/*for(i=0;i<strlen(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff);i++)
							aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff[i]=aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff[i+1];
						aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff[i]='\0';
						*/
						CallVoip(mDspNo,mDeviceIndex,false);
					}
				}
			}
		}
		break;
	}
}


/***********************************************************************
功能：将dtmf码加入到某个通道的dtmf缓冲区
***********************************************************************/
void	InsertDtmfToDevice(long mDspNo,long mIndex,DEVICE_TYPE mDeviceType,char * pDtmf)
{
	strcat(aryDspData[mDspNo].pTrunkDevice[mIndex].fDtmfBuff,pDtmf);
//	AddMsg(pDtmf);
	DrawOneItem(mDspNo,aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID.m_s16ChannelID,DEVICE_TRUNK);
	return;
}

/***********************************************************************
功能：实现内线呼叫内线。触发整个事件流程
***********************************************************************/
bool	CallUser(long mDspNo,long mIndex)
{
	char	mLocalNumber[20];
	char	mTargetNumber[20];
	long	mTargetChannelNo;		/*全局设备编号*/
	long	mTargetIndex;
	long	mTargetDspNo;
	long	mTargetDeviceIdIndex;
	DeviceID_t	mTargetDevice;
	RetCode_t	rtCode;

	strcpy(mLocalNumber,aryDspData[mDspNo].pTrunkDevice[mIndex].fLocalNumber);
	strcpy(mTargetNumber,aryDspData[mDspNo].pTrunkDevice[mIndex].fDtmfBuff);
	strcpy(aryDspData[mDspNo].pTrunkDevice[mIndex].fCallInNumber,mLocalNumber);
	strcpy(aryDspData[mDspNo].pTrunkDevice[mIndex].fCallOutNumber,mTargetNumber);
	
	mTargetChannelNo=FindChannelNoByPhoneNumber(mTargetNumber);
	if(mTargetChannelNo<0)
	{
		/*呼叫的号码不存在*/
		if(aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice==false)
		{
			SetLink(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
				&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID);
			aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice=true;
		}
		PlayFile(&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID,CALL_NUMBER_NOT_EXIT_VOC,false);
		SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,VOC_PLAY_NOTICE);
		return false;
	}

	mTargetIndex=FindTargetIndexByChannelNo(mTargetChannelNo);
	mTargetDspNo=aryTrunkIndex[mTargetIndex].fDspNo;
	mTargetDeviceIdIndex=aryTrunkIndex[mTargetIndex].fIndex;
	mTargetDevice=aryDspData[mDspNo].pTrunkDevice[mTargetDeviceIdIndex].fDeviceID;
	
	/*如果该线路被禁止，则也直接放忙音*/
	if(aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fEnabled==false)
	{	
		if(aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice==false)
		{
			SetLink(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
				&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID);
			aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice=true;
		}
		PlayFile(&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID,CALL_NUMBER_NOT_EXIT_VOC,false);
		SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,VOC_PLAY_NOTICE);
		return false;
	}

	/*如果自己给自己打电话，则播放语音提示*/
	if(strcmp(mTargetNumber,mLocalNumber)==0)
	{	
		if(aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice==false)
		{
			SetLink(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
				&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID);
			aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice=true;
		}
		PlayFile(&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID,CALL_SELF_VOC,false);
		SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,VOC_PLAY_NOTICE);
		return false;
	}
	/*对方线路为非空闲状态*/
	if(aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fLineState!=DCS_FREE ||
	   aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fStatusCode!=VOC_FREE  ||
	   aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fDeviceID.m_s16DeviceSub!=XMS_DEVSUB_ANALOG_USER)
	{
		if(aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice==false)
		{
			SetLink(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
				&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID);
			aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice=true;
		}
		//PlayTone(&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID,BUSY_TONE);
		PlayFile(&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID,IS_BUSY_NOW_VOC,false);
		SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,VOC_PLAY_NOTICE);
		return false;
	}
	/*ADD BY TIM,增加对user设备接收主叫的设置*/ 
	if(aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fIsLinkVoice==false)
	{
		//SetLink(&aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fDeviceID,
		//	&aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fLinkVoiceDeviceID);
		//aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fIsLinkVoice=true;
	}
	SetUserDeviceParam(&aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fDeviceID,&aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fLinkVoiceDeviceID);

	/*可以拨号*/
	rtCode=XMS_ctsMakeCallOut(pFlowHandle,&mTargetDevice,mLocalNumber,mTargetNumber,NULL);
	if(rtCode<0)
	{
		if(aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice==false)
		{
			SetLink(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
				&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID);
			aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice=true;
		}
		PlayTone(&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID,BUSY_TONE);
		SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,VOC_WAITHANGUP);
		return false;
	}
	
	/*对方数据初始化*/
	strcpy(aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fCallInNumber,mLocalNumber);
	aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fLinkTrunkDeviceID=aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID;
	aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fLinkTrunkChannelNo=aryDspData[mDspNo].pTrunkDevice[mIndex].fChannelNo;
	aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fIsLinkTrunk=true;

	/*修改本通道数据，并放回铃音*/
	aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkTrunkChannelNo=aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fChannelNo;
	aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkTrunkDeviceID=aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fDeviceID;
	aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkTrunk=true;
	SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,VOC_CALL_OUT);
	if(aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice==false)
	{
		SetLink(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
			&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID);
		aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice=true;
	}
	PlayTone(&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID,BACK_TONE);
	return false;
}


/***********************************************************************
功能：根据接口设备的全局编号。查找他所在的index数组的下标
***********************************************************************/
long	FindTargetIndexByChannelNo(long	mChannelNo)
{
	long	mResult;
	bool	mFind;
	long	i;
	
	mFind=false;
	mResult=-1;
	i=0;
	while(i<=MAX_INTERFACE_COUNT && mFind==false)
	{
		if(aryTrunkIndex[i].fChannelNo==mChannelNo)
		{
			mResult=i;
			mFind=true;
		}
		i++;
	}
	return mResult;
}


/***********************************************************************
功能：实现外线呼叫，响应外线的事件
***********************************************************************/
void	CallTrunk(long mDspNo,long mIndex,bool isCallLong)
{
	char	mLocalNumber[20];
	char	mTargetNumber[20];
	long	mTargetDspNo;
	long	mTargetDeviceIdIndex;
	RetCode_t	rtCode;
	long	mTrunkIndex;
	long		mLinkVoiceDspNo;
	long		mLinkVoiceIndex;
	
	strcpy(mLocalNumber,aryDspData[mDspNo].pTrunkDevice[mIndex].fLocalNumber);
	strcpy(mTargetNumber,"");
	strcpy(aryDspData[mDspNo].pTrunkDevice[mIndex].fCallInNumber,mLocalNumber);
	strcpy(aryDspData[mDspNo].pTrunkDevice[mIndex].fCallOutNumber,mTargetNumber);
	strcpy(aryDspData[mDspNo].pTrunkDevice[mIndex].fDtmfBuff,"");
	/*如果是呼叫长途*/
	if(isCallLong==true)
	{
		/*不允许呼叫长途*/
		if(aryDspData[mDspNo].pTrunkDevice[mIndex].fEnabledCallLong==false)
		{
			/*播放提示音*/
			if(aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice==false)
			{
				SetLink(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
					&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID);
				aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice=true;
			}
			PlayFile(&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID,DIAL_LONG_ERROR_FILE,false);
			/*将本通道的状态设置为非法呼叫长途状态*/
			SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,VOC_ERROR_CALL_LONG);
			return;
		}
	}
	/*查找一个可用的外线通道*/
	mTrunkIndex=FindFreeTrunkToUse();
	if(mTrunkIndex==-1)
	{
		/*没有空闲的外线通道*/
		if(aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice==false)
		{
			SetLink(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
				&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID);
			aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice=true;
		}
		//PlayTone(&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID,BUSY_TONE);
		PlayFile(&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID,NO_TRUNK_FOR_DIAL_VOC,false);
		SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,VOC_PLAY_NOTICE);
		return;		
	}
	mTargetDspNo=aryTrunkIndex[mTrunkIndex].fDspNo;
	mTargetDeviceIdIndex=aryTrunkIndex[mTrunkIndex].fIndex;
	/*开始进行呼叫*/
	rtCode=XMS_ctsMakeCallOut(pFlowHandle,&aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fDeviceID,
		mLocalNumber,mTargetNumber,NULL);
	if(rtCode<0)
	{
		/*呼叫失败*/
		if(aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice==false)
		{
			SetLink(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
				&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID);
			aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice=true;
		}
		PlayTone(&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID,BUSY_TONE);
		SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,VOC_WAITHANGUP);
		return;		
	}
	/*断开本通道相连的语音通道连接*/
	mLinkVoiceIndex=FindIndexByChannelNo(mDspNo,aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceChannelNo,DEVICE_VOICE);
	mLinkVoiceDspNo=aryVoiceIndex[mLinkVoiceIndex].fDspNo;
	mLinkVoiceIndex=aryVoiceIndex[mLinkVoiceIndex].fIndex;
	DeleteLink(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
		&aryDspData[mLinkVoiceDspNo].pVoiceDevice[mLinkVoiceIndex].fDeviceID);
	aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice=false;
	aryDspData[mLinkVoiceDspNo].pVoiceDevice[mLinkVoiceIndex].fIsLinkTrunk=false;
	/*将内线通道和外线通道之间建立时隙连接*/
	SetLink(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
		&aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fDeviceID);
	/*修改本接口数据*/
	aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkTrunk=true;
	aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkTrunkChannelNo=aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fChannelNo;
	aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkTrunkDeviceID=aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fDeviceID;
	aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice=false;
	/*修改连接的外线通道接口数据*/
	aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fIsLinkTrunk=true;
	aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fLinkTrunkChannelNo=aryDspData[mDspNo].pTrunkDevice[mIndex].fChannelNo;
	aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fLinkTrunkDeviceID=aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID;
	aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fIsLinkVoice=false;
	/*修改两个通道的状态*/
	SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,VOC_CALL_OUT);
	SetTrunkDeviceState(&aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fDeviceID,VOC_CALL_OUT);
	/*后续处理转到外线呼叫的返回事件*/
	/*为了能够接收到被叫号码，所以将中继通道相连的语音通道建立一个单向的连接*/
	XMS_ctsLinkDevice(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
		&aryDspData[mLinkVoiceDspNo].pVoiceDevice[mLinkVoiceIndex].fDeviceID,NULL);
	return;	
}


/***********************************************************************
功能：判断号码中是否有长途号码或者非法的号码
***********************************************************************/
bool	JudgePhoneNumIsValid(char * pPhoneNumber)
{
	return true;
}


/***********************************************************************
功能：对指定的语音通道放音，本系统全部采用文件放音
***********************************************************************/
bool	PlayFile(DeviceID_t *pDevice,DJ_S8 *pFileName,bool isStop)
{
	RetCode_t	rtCode;
	PlayProperty_t		mProperty;
	ControlPlay_t		mPlayControl;
	int					i;
       
	mPlayControl.m_u16ControlType=XMS_STOP_PLAY;
	XMS_ctsControlPlay(pFlowHandle,pDevice,&mPlayControl,NULL);
	PlayTone(pDevice,STOP_TONE);
	XMS_ctsCloseDevice(pFlowHandle,pDevice,NULL);
	XMS_ctsOpenDevice(pFlowHandle,pDevice,NULL);
	Sleep(700);
	XMS_ctsResetDevice(pFlowHandle,pDevice,NULL);
	XMS_ctsGetDevState(pFlowHandle,pDevice,NULL);
	for(i=0;i<500;i++)	i++;

	memset(&mProperty,0,sizeof(mProperty));
	mProperty.m_u16PlayType=XMS_PLAY_TYPE_FILE;			/*采用文件放音*/
	if(isStop)
		mProperty.m_u32StopMode=XMS_Any_Code;			/*设置为收到任意一个dtmf码则停止放音*/
	mPlayTaskID=mPlayTaskID+1;
	if(mPlayTaskID>255)	mPlayTaskID=0;					/*全局的播放任务ID*/
	mProperty.m_u8TaskID=(DJ_U8)mPlayTaskID;
	strcpy(mProperty.m_s8PlayContent,pFileName);
	rtCode=XMS_ctsPlay(pFlowHandle,pDevice,&mProperty,NULL);
	if(rtCode<0)	return false;
	return true;
}

/***********************************************************************
功能:VOIP事件的响应和处理
***********************************************************************/
void	VoipDeviceWork(DeviceID_t	*pDevice,Acs_Evt_t	*pEvt)
{
	long		mDspNo;
	ChannelID_t	mChannelID;
	long		mIndex;
	long		mDeviceIndex;
	long		mLinkDspNo;
	long		mLinkDeviceIndex;
	long		mLinkChannelNo;

	mDspNo=pDevice->m_s8ModuleID;
	mChannelID=pDevice->m_s16ChannelID;
	mIndex=FindIndexByChannelID(mDspNo,mChannelID,DEVICE_VOIP);
	if(mIndex==-1) return;
	mDeviceIndex=aryVoipIndex[mIndex].fIndex;
	if(aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fIsOpen==false) return;
	switch(pEvt->m_s32EventType)
	{
	case	XMS_EVT_CLEARCALL:					
		{
			ResetVoipDevice(pDevice,pEvt);
			return;
		}
		break;
	case	XMS_EVT_CALLIN:
		/*用户摘机事件----本段代码还还需要修改*/
		{
			Acs_CallControl_Data		*pControlData=NULL;
			char		mCallerNum[20];						/*被叫号码*/
			char		mCallingNum[20];					/*主叫号码*/
			RetCode_t	rtCode;
			long		mTrunkIndex;
			pControlData=(Acs_CallControl_Data *)FetchEventData(pEvt);
			strcpy(mCallerNum,pControlData->m_s8CalledNum);		/*获取被叫号码*/
			strcpy(mCallingNum,pControlData->m_s8CallingNum);	/*获取主叫号码*/
			/*方便测试，直接指定一个通道*/
			if(mCallerNum[0]!='4')
			{
				mTrunkIndex=FindChannelNoByPhoneNumber(mCallerNum);
				if(mTrunkIndex==-1)
				{
					/*如果呼叫的号码不存在，则直接拆线，到时候修改，放忙音*/
					XMS_ctsClearCall(pFlowHandle,pDevice,0,NULL);
					ResetVoipDevice(pDevice,pEvt);
					//Sleep(3000);
					//aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fLineState=DCS_FREE;
					return;
				}
			}
			strcpy(aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fCallInNumber,mCallingNum);
			strcpy(aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fCallOutNumber,mCallerNum);
			/*应答该呼叫*/
			rtCode=XMS_ctsAlertCall(pFlowHandle,pDevice,NULL);
			if(rtCode<0)
			{
				XMS_ctsClearCall(pFlowHandle,pDevice,0,NULL);
				ResetVoipDevice(pDevice,pEvt);
				return;
			}
			rtCode=XMS_ctsAnswerCallIn(pFlowHandle,pDevice,NULL);
			if(rtCode<0)
			{
				XMS_ctsClearCall(pFlowHandle,pDevice,0,NULL);
				ResetVoipDevice(pDevice,pEvt);
				return;
			}
			/*找到对应的内线通道，则对该中继通道发起呼叫*/
			SetVoipDeviceState(pDevice,VOIP_CALL_IN);
		}
		break;
	case	XMS_EVT_ANSWERCALL:
		{
			DJ_S32		mLinkVoipVoiceDspNo;
			long		mLinkVoipVoiceChannelNo;
			long		mLinkVoipVoiceIndex;
			DJ_S32		mTrunkDspNo;
			long		mTrunkIndex;
			long		mTrunkNo;
			
			SetVoipDeviceState(pDevice,VOIP_CONNECTED);
			//加入IP呼叫外线
			if(aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fCallOutNumber[0]=='4')
			{
				unsigned int	i;
				for(i=0;i<strlen(aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fCallOutNumber);i++)
					aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fCallOutNumber[i]=aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fCallOutNumber[i+1];
				aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fCallOutNumber[i]='\0';
				CallTrunkByVoip(mDspNo,mDeviceIndex);
				return;
			}
			/*根据被叫号码，查找内线通道*/
			mTrunkNo=FindChannelNoByPhoneNumber(aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fCallOutNumber);
			mTrunkDspNo=aryTrunkIndex[mTrunkNo].fDspNo;
			mTrunkIndex=aryTrunkIndex[mTrunkNo].fIndex;
			strcpy(aryDspData[mTrunkDspNo].pTrunkDevice[mTrunkIndex].fCallOutNumber,aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fCallOutNumber);
			strcpy(aryDspData[mTrunkDspNo].pTrunkDevice[mTrunkIndex].fCallInNumber,aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fCallInNumber);

			mLinkVoipVoiceDspNo=mDspNo;
			mLinkVoipVoiceChannelNo=aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fLinkVoiceChannelNo;
			mLinkVoipVoiceIndex=FindIndexByChannelNo(mLinkVoipVoiceDspNo,mLinkVoipVoiceChannelNo,DEVICE_VOICE);
			mLinkVoipVoiceDspNo=aryVoiceIndex[mLinkVoipVoiceIndex].fDspNo;
			mLinkVoipVoiceIndex=aryVoiceIndex[mLinkVoipVoiceIndex].fIndex;
			/*将VOIP通道和他对应的VOICE设备绑定*/
			XMS_ctsLinkDevice(pFlowHandle,&aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fDeviceID,
				&aryDspData[mLinkVoipVoiceDspNo].pVoiceDevice[mLinkVoipVoiceIndex].fDeviceID,NULL);
			MixerControlParam_t paramMixer = {0};
			paramMixer.m_u8SRC1_Ctrl = XMS_MIXER_FROM_PLAY ;
			paramMixer.m_u16SRC_ChID1 = aryDspData[mLinkVoipVoiceDspNo].pVoiceDevice[mLinkVoipVoiceIndex].fDeviceID.m_s16ChannelID;
			paramMixer.m_u8SRC2_Ctrl = XMS_MIXER_FROM_INPUT;
			paramMixer.m_u16SRC_ChID2 = aryDspData[mLinkVoipVoiceDspNo].pVoiceDevice[mLinkVoipVoiceIndex].fDeviceID.m_s16ChannelID;
			RetCode_t ret = XMS_ctsSetParam(pFlowHandle, &aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fDeviceID,
				VOIP_PARAM_RTPMIXER, sizeof(paramMixer), &paramMixer);
			aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fIsLinkVoice=true;

			aryDspData[mTrunkDspNo].pTrunkDevice[mTrunkIndex].fLinkVoipVoiceDeviceID=aryDspData[mLinkVoipVoiceDspNo].pVoiceDevice[mLinkVoipVoiceIndex].fDeviceID;
			aryDspData[mTrunkDspNo].pTrunkDevice[mTrunkIndex].fLinkVoipVoiceDeviceNo=aryDspData[mLinkVoipVoiceDspNo].pVoiceDevice[mLinkVoipVoiceIndex].fChannelNo;
			aryDspData[mTrunkDspNo].pTrunkDevice[mTrunkIndex].fisLinkVoipVoice=false;

			/*对内线通道发起呼叫*/
			/*将VOIP通道信息写入内线通道，以备呼叫不成功，进行拆线*/
			aryDspData[mTrunkDspNo].pTrunkDevice[mTrunkIndex].fLinkVoipDeviceNo=aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fChannelNo;
			aryDspData[mTrunkDspNo].pTrunkDevice[mTrunkIndex].fLinkVoipDeviceID=aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fDeviceID;
			CallUserByVoip(mTrunkDspNo,mTrunkIndex);
			/*利用该通道放回铃音*/
			PlayTone(&aryDspData[mLinkVoipVoiceDspNo].pVoiceDevice[mLinkVoipVoiceIndex].fDeviceID,BACK_TONE);
		}
		break;
	case	XMS_EVT_CALLOUT:
		/*呼出事件*/
		{
			Acs_CallControl_Data	*pControlData=NULL;
			long					mLinkUserDspNo;
			long					mLinkUserDeviceIndex;
			long					mLinkUserDeviceNo;


			pControlData=(Acs_CallControl_Data *)FetchEventData(pEvt);
			/*获取本通道的基本数据*/
			mChannelID=pDevice->m_s16ChannelID;
			mDspNo=pDevice->m_s8ModuleID;
			mIndex=FindIndexByChannelID(mDspNo,mChannelID,DEVICE_VOIP);
			mDeviceIndex=aryVoipIndex[mIndex].fIndex;
			mLinkChannelNo=aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fLinkVoiceChannelNo;
			mLinkDeviceIndex=FindIndexByChannelNo(mDspNo,mLinkChannelNo,DEVICE_VOICE);
			mLinkDspNo=aryVoiceIndex[mLinkDeviceIndex].fDspNo;
			mLinkDeviceIndex=aryVoiceIndex[mLinkDeviceIndex].fIndex;
			/*通过与VOIP相连的语音通道，找到相对应的接口通道*/
			mLinkUserDeviceNo=aryDspData[mLinkDspNo].pVoiceDevice[mLinkDeviceIndex].fLinkTrunkChannelNo;
			mLinkUserDspNo=mDspNo;
			mLinkUserDeviceIndex=FindIndexByChannelNo(mLinkUserDspNo,mLinkUserDeviceNo,DEVICE_TRUNK);
			mLinkUserDspNo=aryTrunkIndex[mLinkUserDeviceIndex].fDspNo;
			mLinkUserDeviceIndex=aryTrunkIndex[mLinkUserDeviceIndex].fIndex;

			ControlPlay_t		mPlayControl;
			
			mPlayControl.m_u16ControlType=XMS_STOP_PLAY;
			XMS_ctsControlPlay(pFlowHandle,&aryDspData[mLinkDspNo].pVoiceDevice[mLinkDeviceIndex].fDeviceID,&mPlayControl,NULL);
			DeleteLink(&aryDspData[mLinkUserDspNo].pTrunkDevice[mLinkUserDeviceIndex].fDeviceID,
					&aryDspData[mLinkUserDspNo].pTrunkDevice[mLinkUserDeviceIndex].fLinkVoipVoiceDeviceID);

			if(pControlData->m_s32AcsEvtState==1)
			{
				/*呼叫成功*/
				aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fIsLinkTrunk=true;

				XMS_ctsLinkDevice(pFlowHandle,&aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fDeviceID,
					&aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fLinkVoiceDeviceID,NULL);
				
				SetLink(&aryDspData[mLinkUserDspNo].pTrunkDevice[mLinkUserDeviceIndex].fDeviceID,
					&aryDspData[mLinkUserDspNo].pTrunkDevice[mLinkUserDeviceIndex].fLinkVoipVoiceDeviceID);

				SetVoipDeviceState(&aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fDeviceID,VOIP_CONNECTED);
				SetTrunkDeviceState(&aryDspData[mLinkUserDspNo].pTrunkDevice[mLinkUserDeviceIndex].fDeviceID,VOC_CONNECTED);
			}
			else
			{
				/*呼叫失败*/
				/*断开接口设备与voip的语音设备之间的连接*/
				DeleteLink(&aryDspData[mLinkUserDspNo].pTrunkDevice[mLinkUserDeviceIndex].fDeviceID,
						   &aryDspData[mLinkDspNo].pVoiceDevice[mLinkDeviceIndex].fDeviceID);
				aryDspData[mLinkDspNo].pVoiceDevice[mLinkDeviceIndex].fIsLinkTrunk=false;
				aryDspData[mLinkDspNo].pVoiceDevice[mLinkDeviceIndex].fLinkTrunkChannelNo=-1;

				/*接口通道放忙音*/
				if(aryDspData[mLinkUserDspNo].pTrunkDevice[mLinkUserDeviceIndex].fIsLinkVoice==false)
				{
					SetLink(&aryDspData[mLinkUserDspNo].pTrunkDevice[mLinkUserDeviceIndex].fDeviceID,
						&aryDspData[mLinkUserDspNo].pTrunkDevice[mLinkUserDeviceIndex].fLinkVoiceDeviceID);
					aryDspData[mLinkUserDspNo].pTrunkDevice[mLinkUserDeviceIndex].fIsLinkVoice=true;
				}
				PlayTone(&aryDspData[mLinkUserDspNo].pTrunkDevice[mLinkUserDeviceIndex].fLinkVoiceDeviceID,BUSY_TONE);
				SetTrunkDeviceState(&aryDspData[mLinkUserDspNo].pTrunkDevice[mLinkUserDeviceIndex].fDeviceID,VOC_WAITHANGUP);
				ResetVoipDevice(pDevice,pEvt);
			}
		}
		break;
	case	XMS_EVT_LINKDEVICE:
		/*建立连接事件*/
		break;
	case	XMS_EVT_UNLINKDEVICE:
		/*解除连接事件*/
		break;
	case	XMS_EVT_SENDIODATA:
		/*发送数据事件*/
		break;
	case	XMS_EVT_PLAY:
		/*放音完成*/

		break;
	case	XMS_EVT_RECVIODATA:				
		/*接收数据事件--DTMF*/
		break;
	}
	return;
}


/***********************************************************************
功能：查找一个可用的VOIP通道，如果没有空闲的，返回-1，必须确保找到的是同一个DSP的语音通道
***********************************************************************/
long	FindFreeVoipToUse(DJ_S32 mDspNo)
{
	bool	mFind;
	long	mResult;
	long	i;
	long	mIndex;

	mFind=false;
	mResult=-1;
	i=mVoipCount;
	while(i>0 && mFind==false)
	{
		mIndex=aryVoipIndex[i].fIndex;
		if(aryVoipIndex[i].fDspNo==mDspNo)
		{
			if(aryDspData[mDspNo].pVoipDevice[mIndex].fLineState==DCS_FREE && aryDspData[mDspNo].pVoipDevice[mIndex].fStatusCode==VOIP_FREE)
			{
				mFind=true;
				mResult=i;
			}
		}
		i--;
	}
	return mResult;
}

/***********************************************************************
功能：实现利用VOIP进行呼叫，触发整个VOIP事件流
***********************************************************************/
void	CallVoip(long mDspNo,long mIndex,bool isCallLong)
{
	char	mLocalNumber[20];
	char	mTargetNumber[20];
	long	mTargetDspNo;
	long	mTargetDeviceIdIndex;
	RetCode_t	rtCode;
	long	mVoipIndex;
	long		mLinkVoiceDspNo;
	long		mLinkVoiceIndex;
	long		mLinkVoiceNo;
	VoIPCallPrivate_t	pCallParam={0};

	strcpy(mLocalNumber,aryDspData[mDspNo].pTrunkDevice[mIndex].fLocalNumber);
	strcpy(mTargetNumber,aryDspData[mDspNo].pTrunkDevice[mIndex].fDtmfBuff);
	strcpy(aryDspData[mDspNo].pTrunkDevice[mIndex].fCallInNumber,mLocalNumber);
	strcpy(aryDspData[mDspNo].pTrunkDevice[mIndex].fCallOutNumber,mTargetNumber);
	strcpy(aryDspData[mDspNo].pTrunkDevice[mIndex].fDtmfBuff,"");

	/*查找一个可用的VOIP通道*/
	mVoipIndex=FindFreeVoipToUse(pVoipDspNo);
//	mVoipIndex=59;
	if(mVoipIndex==-1)
	{
		/*没有空闲的VOIP通道进行呼叫*/
		if(aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice==false)
		{
			SetLink(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
				&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID);
			aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice=true;
		}
		//PlayTone(&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID,BUSY_TONE);
		//SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,VOC_WAITHANGUP);

		PlayFile(&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID,IS_BUSY_NOW_VOC,false);
		SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,VOC_PLAY_NOTICE);

		return;		
	}

	mTargetDspNo=aryVoipIndex[mVoipIndex].fDspNo;
	mTargetDeviceIdIndex=aryVoipIndex[mVoipIndex].fIndex;

	/*设置被叫信息*/
	pCallParam.m_PrivateData.m_u32DataSize=sizeof(VoIPCallPrivate_t);
	pCallParam.m_s32Protocol=aryDspData[mTargetDspNo].pVoipDevice[mTargetDeviceIdIndex].fProtocolType;
	//呼叫323协议不需要填写IP地址以及端口
	if(pCallParam.m_s32Protocol==2)
	{
		pCallParam.m_s32CalleePort=aryDspData[mTargetDspNo].pVoipDevice[mTargetDeviceIdIndex].fTargetAddr.port;
		strcpy(pCallParam.m_s8CalleeAddress,aryDspData[mTargetDspNo].pVoipDevice[mTargetDeviceIdIndex].fTargetAddr.tszAddr);
		strcpy(pCallParam.m_s8CalleeUserID,aryDspData[mTargetDspNo].pVoipDevice[mTargetDeviceIdIndex].fTargetAddr.tszUserID);
	}
	/*设置主叫信息*/
	strcpy(pCallParam.m_s8CallerAddress,aryDspData[mTargetDspNo].pVoipDevice[mTargetDeviceIdIndex].fSourceAddr.tszAddr);
	pCallParam.m_s32CallerPort=aryDspData[mTargetDspNo].pVoipDevice[mTargetDeviceIdIndex].fSourceAddr.port;
	strcpy(pCallParam.m_s8CallerUserID,aryDspData[mTargetDspNo].pVoipDevice[mTargetDeviceIdIndex].fSourceAddr.tszUserID);
	/*开始进行呼叫*/

	rtCode=XMS_ctsMakeCallOut(pFlowHandle,&aryDspData[mTargetDspNo].pVoipDevice[mTargetDeviceIdIndex].fDeviceID,
		mLocalNumber,mTargetNumber,(PrivateData_t *)&pCallParam);
	if(rtCode<0)
	{
		/*呼叫失败*/
		if(aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice==false)
		{
			SetLink(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
				&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID);
			aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice=true;
		}
		PlayTone(&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID,BUSY_TONE);
		SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,VOC_WAITHANGUP);
		return;		
	}
	/*断开用户通道本身相连的语音通道*/
	mLinkVoiceIndex=FindIndexByChannelNo(mDspNo,aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceChannelNo,DEVICE_VOICE);
	mLinkVoiceDspNo=aryVoiceIndex[mLinkVoiceIndex].fDspNo;
	mLinkVoiceIndex=aryVoiceIndex[mLinkVoiceIndex].fIndex;
	if(aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice==true)
	{
		DeleteLink(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
			&aryDspData[mLinkVoiceDspNo].pVoiceDevice[mLinkVoiceIndex].fDeviceID);
		aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice=false;
	}

	/*将voip设备和他相连的语音设备进行绑定*/
	mLinkVoiceNo=aryDspData[mTargetDspNo].pVoipDevice[mTargetDeviceIdIndex].fLinkVoiceChannelNo;
	mLinkVoiceIndex=FindIndexByChannelNo(mLinkVoiceDspNo,mLinkVoiceNo,DEVICE_VOICE);
	mLinkVoiceDspNo=aryVoiceIndex[mLinkVoiceIndex].fDspNo;
	mLinkVoiceIndex=aryVoiceIndex[mLinkVoiceIndex].fIndex;
	XMS_ctsLinkDevice(pFlowHandle,&aryDspData[mTargetDspNo].pVoipDevice[mTargetDeviceIdIndex].fDeviceID,
		&aryDspData[mLinkVoiceDspNo].pVoiceDevice[mLinkVoiceIndex].fDeviceID,NULL);
	aryDspData[mTargetDspNo].pVoipDevice[mTargetDeviceIdIndex].fIsLinkVoice=true;
	aryDspData[mLinkVoiceDspNo].pVoiceDevice[mLinkVoiceIndex].fIsLinkVoip=true;
	aryDspData[mLinkVoiceDspNo].pVoiceDevice[mLinkVoiceIndex].fLinkVoipChannelNo=aryDspData[mTargetDspNo].pVoipDevice[mTargetDeviceIdIndex].fChannelNo;
	aryDspData[mLinkVoiceDspNo].pVoiceDevice[mLinkVoiceIndex].fLinkVoipDeviceID=aryDspData[mTargetDspNo].pVoipDevice[mTargetDeviceIdIndex].fDeviceID;

	/*将接口通道与VOIP绑定的语音通道建立双向的时隙连接*/
	SetLink(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
		    &aryDspData[mLinkVoiceDspNo].pVoiceDevice[mLinkVoiceIndex].fDeviceID);
	aryDspData[mLinkVoiceDspNo].pVoiceDevice[mLinkVoiceIndex].fIsLinkTrunk=true;
	aryDspData[mLinkVoiceDspNo].pVoiceDevice[mLinkVoiceIndex].fLinkTrunkChannelNo=aryDspData[mDspNo].pTrunkDevice[mIndex].fChannelNo;
	aryDspData[mLinkVoiceDspNo].pVoiceDevice[mLinkVoiceIndex].fLinkTrunkDeviceID=aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkTrunkDeviceID;
	aryDspData[mDspNo].pTrunkDevice[mIndex].fisLinkVoipVoice=true;
	aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoipVoiceDeviceID=aryDspData[mLinkVoiceDspNo].pVoiceDevice[mLinkVoiceIndex].fDeviceID;
	aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoipVoiceDeviceNo=aryDspData[mLinkVoiceDspNo].pVoiceDevice[mLinkVoiceIndex].fChannelNo;

	aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoipDeviceID=aryDspData[mTargetDspNo].pVoipDevice[mTargetDeviceIdIndex].fDeviceID;
	aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoipDeviceNo=aryDspData[mTargetDspNo].pVoipDevice[mTargetDeviceIdIndex].fChannelNo;

	aryDspData[mTargetDspNo].pVoipDevice[mTargetDeviceIdIndex].fLinkTrunkChannelID=aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID;
	aryDspData[mTargetDspNo].pVoipDevice[mTargetDeviceIdIndex].fLinkTrunkChannelNo=aryDspData[mDspNo].pTrunkDevice[mIndex].fChannelNo;

	/*放回铃音*/
	PlayFileCircly(&aryDspData[mLinkVoiceDspNo].pVoiceDevice[mLinkVoiceIndex].fDeviceID,BACK_SIGN_MUSIC,false);
	//PlayTone(&aryDspData[mLinkVoiceDspNo].pVoiceDevice[mLinkVoiceIndex].fDeviceID,BACK_TONE);


	/*修改两个通道的状态*/
	SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,VOC_CALL_OUT);
	SetVoipDeviceState(&aryDspData[mTargetDspNo].pVoipDevice[mTargetDeviceIdIndex].fDeviceID,VOIP_CALL_VOIP);
	return;	
}


/***********************************************************************
功能：复位VOIP设备
***********************************************************************/
void	ResetVoipDevice(DeviceID_t *pDevice,Acs_Evt_t	*pEvt)
{
	long	mDspNo;
	long	mIndex;
	long	mDeviceIndex;
	ChannelID_t	mChannelID;

	mDspNo=pDevice->m_s8ModuleID;
	mChannelID=pDevice->m_s16ChannelID;
	if(pDevice->m_s16DeviceMain!=XMS_DEVMAIN_VOIP)	return;
	mIndex=FindIndexByChannelID(mDspNo,mChannelID,DEVICE_VOIP);
	mDeviceIndex=aryVoipIndex[mIndex].fIndex;
	if(aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fStatusCode==VOIP_FREE)	return;
	/*对VOIP相连的语音通道放忙音*/
	if(aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fIsLinkVoice)
	{
		DeviceID_t		mLinkDeviceID;
		long			mLinkIndex;
		long			mLinkDeviceIndex;
		long			mLinkDspNo;
		mLinkDeviceID=aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fLinkVoiceDeviceID;
		if(mLinkDeviceID.m_s16DeviceMain!=0)
		{
			mLinkIndex=FindIndexByChannelNo(mDspNo,aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fLinkVoiceChannelNo,DEVICE_VOICE);
			mLinkDeviceIndex=aryVoiceIndex[mLinkIndex].fIndex;
			mLinkDspNo=aryVoiceIndex[mLinkIndex].fDspNo;
			DeleteLink(pDevice,&aryDspData[mLinkDspNo].pVoiceDevice[mLinkDeviceIndex].fDeviceID);
			aryDspData[mLinkDspNo].pVoiceDevice[mLinkDeviceIndex].fIsLinkVoip=false;
			aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fIsLinkVoice=false;
		}
	}
	//如果连接了接口
	if(aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fIsLinkTrunk)
	{
		DeviceID_t		mLinkDeviceID;
		long			mLinkIndex;
		long			mLinkDeviceIndex;
		long			mLinkDspNo;
		mLinkDeviceID=aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fLinkTrunkChannelID;
		if(mLinkDeviceID.m_s16DeviceMain!=0)
		{
			mLinkIndex=FindIndexByChannelNo(mDspNo,aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fLinkTrunkChannelNo,DEVICE_TRUNK);
			mLinkDeviceIndex=aryTrunkIndex[mLinkIndex].fIndex;
			mLinkDspNo=aryTrunkIndex[mLinkIndex].fDspNo;
			if(aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLineType==LINE_IN)
			{
				DeleteLink(&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDeviceID,
					&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkVoipVoiceDeviceID);
				aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fisLinkVoipVoice=false;
				SetLink(&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDeviceID,
					&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkVoiceDeviceID);
				aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fIsLinkVoice=true;
				PlayTone(&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkVoiceDeviceID,BUSY_TONE);
			}
			if(aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLineType==LINE_OUT)
			{
/*				DeleteLink(&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDeviceID,
					       &aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkVoipVoiceDeviceID);
				aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fisLinkVoipVoice=false;
				SetLink(&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDeviceID,
					    &aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkVoiceDeviceID);
				aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fIsLinkVoice=true;
				PlayTone(&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkVoiceDeviceID,BUSY_TONE);
				SetTrunkDeviceState(&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDeviceID,VOC_WAITHANGUP);
*/
				XMS_ctsClearCall(pFlowHandle,&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDeviceID,0,NULL);
			}
		}
	}
	/*本通道数据初始化*/
	aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fIsLinkVoice=false;
	strcpy(aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fDtmfBuff,"");		
	strcpy(aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fCallInNumber,"");
	strcpy(aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fCallOutNumber,"");
	SetVoipDeviceState(pDevice,VOIP_FREE);
	XMS_ctsResetDevice(pFlowHandle,pDevice,NULL);
	XMS_ctsGetDevState(pFlowHandle,pDevice,NULL);
	return;
}


/***********************************************************************
功能：实现对VOIP呼叫坐席的应答
注意：mdspno,mindex:即表示呼叫那个坐席
***********************************************************************/
void	CallUserByVoip(long mDspNo,long mIndex)
{
	//char	mLocalNumber[20];
	char	mCallingNumber[20];
	char	mTargetNumber[20];
	long	mTargetChannelNo;	
	DJ_S32	mTargetDspNo;
	long	mTargetDeviceIdIndex;
	DeviceID_t	mTargetDevice;
	RetCode_t	rtCode;

	DJ_S32		mVoipDspNo;
	long		mVoipDeviceNo;	
	long		mVoipDeviceIndex;

	mTargetDspNo=-1;
	mTargetDeviceIdIndex=-1;
	//strcpy(mLocalNumber,aryDspData[mDspNo].pTrunkDevice[mIndex].fLocalNumber);
	strcpy(mCallingNumber,aryDspData[mDspNo].pTrunkDevice[mIndex].fCallInNumber);
 	strcpy(mTargetNumber,aryDspData[mDspNo].pTrunkDevice[mIndex].fCallOutNumber);
	mTargetChannelNo=aryDspData[mDspNo].pTrunkDevice[mIndex].fChannelNo;
	mTargetDeviceIdIndex=mIndex;
	mTargetDspNo=mDspNo;
	mTargetDevice=aryDspData[mDspNo].pTrunkDevice[mTargetDeviceIdIndex].fDeviceID;
	
	if(aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fEnabled==false)
	{	
		if(aryDspData[mDspNo].pTrunkDevice[mIndex].fisLinkVoipVoice==false)
		{
			SetLink(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
				&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoipVoiceDeviceID);
			aryDspData[mDspNo].pTrunkDevice[mIndex].fisLinkVoipVoice=true;
		}
		//PlayTone(&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoipVoiceDeviceID,BUSY_TONE);
		//PlayFile(&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoipVoiceDeviceID,CALL_NUMBER_NOT_EXIT_VOC,false);
		//SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,VOC_PLAY_VOIP_ERROR);

		XMS_ctsClearCall(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoipDeviceID,0,NULL);
		ResetVoipDevice(&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoipDeviceID,NULL);
		ResetUserDevice(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,NULL);
		return;
	}

	/*对方线路为非空闲状态*/
	if(aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fLineState!=DCS_FREE ||
	   aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fStatusCode!=VOC_FREE  ||
	   aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fDeviceID.m_s16DeviceSub!=XMS_DEVSUB_ANALOG_USER)
	{
		if(aryDspData[mDspNo].pTrunkDevice[mIndex].fisLinkVoipVoice==false)
		{
			SetLink(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
				&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoipVoiceDeviceID);
			aryDspData[mDspNo].pTrunkDevice[mIndex].fisLinkVoipVoice=true;
		}
		//PlayTone(&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoipVoiceDeviceID,BUSY_TONE);
		//PlayFile(&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoipVoiceDeviceID,IS_BUSY_NOW_VOC,false);
		//SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,VOC_PLAY_VOIP_ERROR);
		XMS_ctsClearCall(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoipDeviceID,0,NULL);
		ResetVoipDevice(&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoipDeviceID,NULL);
		ResetUserDevice(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,NULL);

		return;
	}

	/*ADD BY TIM,增加对user设备接收主叫的设置*/ 
	SetUserDeviceParam(&mTargetDevice,&aryDspData[mDspNo].pTrunkDevice[mTargetDeviceIdIndex].fLinkVoiceDeviceID);

	/*可以拨号*/
	rtCode=XMS_ctsMakeCallOut(pFlowHandle,&mTargetDevice,mCallingNumber,mTargetNumber,NULL);
	if(rtCode<0)
	{
		if(aryDspData[mDspNo].pTrunkDevice[mIndex].fisLinkVoipVoice==false)
		{
			SetLink(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
				&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoipVoiceDeviceID);
			aryDspData[mDspNo].pTrunkDevice[mIndex].fisLinkVoipVoice=true;
		}
		PlayTone(&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoipVoiceDeviceID,BUSY_TONE);
		return;
	}

	/*回写VOIP通道数据*/
	mVoipDspNo=-1;
	mVoipDeviceNo=aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoipDeviceNo;
	mVoipDeviceIndex=FindIndexByChannelNo(mVoipDspNo,mVoipDeviceNo,DEVICE_VOIP);
	mVoipDspNo=aryVoipIndex[mVoipDeviceIndex].fDspNo;
	mVoipDeviceIndex=aryVoipIndex[mVoipDeviceIndex].fIndex;
	aryDspData[mVoipDspNo].pVoipDevice[mVoipDeviceIndex].fLinkTrunkChannelID=aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID;
	aryDspData[mVoipDspNo].pVoipDevice[mVoipDeviceIndex].fLinkTrunkChannelNo=aryDspData[mDspNo].pTrunkDevice[mIndex].fChannelNo;
	aryDspData[mVoipDspNo].pVoipDevice[mVoipDeviceIndex].fLinkTrunkLineType=aryDspData[mDspNo].pTrunkDevice[mIndex].fLineType;
	aryDspData[mVoipDspNo].pVoipDevice[mVoipDeviceIndex].fIsLinkTrunk=false;

	/*此代码很重要*/
	aryDspData[mDspNo].pTrunkDevice[mIndex].fisLinkVoipVoice=true;
	SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,VOC_CALL_IN);
	return;
}


/***********************************************************************
功能：实现外线呼叫内线
***********************************************************************/
void	CallUserByTrunk(long mDspNo,long mIndex)
{
	char	mLocalNumber[20];
	char	mTargetNumber[20];
	long	mTargetChannelNo;		/*全局设备编号*/
	long	mTargetIndex;
	long	mTargetDspNo;
	long	mTargetDeviceIdIndex;
	DeviceID_t	mTargetDevice;
	RetCode_t	rtCode;

	strcpy(mLocalNumber,aryDspData[mDspNo].pTrunkDevice[mIndex].fLocalNumber);
	strcpy(mTargetNumber,aryDspData[mDspNo].pTrunkDevice[mIndex].fDtmfBuff);
	strcpy(aryDspData[mDspNo].pTrunkDevice[mIndex].fCallOutNumber,mTargetNumber);
	
	mTargetChannelNo=FindChannelNoByPhoneNumber(mTargetNumber);
	if(mTargetChannelNo<0)
	{
		/*呼叫的号码不存在*/
		if(aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice==false)
		{
			SetLink(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
				&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID);
			aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice=true;
		}
		PlayFile(&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID,CALL_NUMBER_NOT_EXIT_VOC,false);
		SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,VOC_PLAY_DIAL_WRONG);
		return;
	}

	mTargetIndex=FindTargetIndexByChannelNo(mTargetChannelNo);
	mTargetDspNo=aryTrunkIndex[mTargetIndex].fDspNo;
	mTargetDeviceIdIndex=aryTrunkIndex[mTargetIndex].fIndex;
	mTargetDevice=aryDspData[mDspNo].pTrunkDevice[mTargetDeviceIdIndex].fDeviceID;
	
	/*如果该线路被禁止，则也直接放忙音*/
	if(aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fEnabled==false)
	{	
		if(aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice==false)
		{
			SetLink(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
				&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID);
			aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice=true;
		}
		/*
		PlayFile(&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID,IS_BUSY_NOW_VOC,false);
		SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,VOC_PLAY_DIAL_WRONG);
		*/
		PlayTone(&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID,BUSY_TONE);
		SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,VOC_WAITHANGUP);
		return;
	}
	/*对方线路为非空闲状态*/
	if(aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fLineState!=DCS_FREE ||
	   aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fStatusCode!=VOC_FREE  ||
	   aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fDeviceID.m_s16DeviceSub!=XMS_DEVSUB_ANALOG_USER)
	{
		if(aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice==false)
		{
			SetLink(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
				&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID);
			aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice=true;
		}
		/*
		PlayFile(&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID,IS_BUSY_NOW_VOC,false);
		SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,VOC_PLAY_DIAL_WRONG);
		*/
		PlayTone(&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID,BUSY_TONE);
		SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,VOC_WAITHANGUP);
		return;
	}

	/*ADD BY TIM,增加对user设备接收主叫的设置*/ 
	SetUserDeviceParam(&mTargetDevice,&aryDspData[mDspNo].pTrunkDevice[mTargetDeviceIdIndex].fLinkVoiceDeviceID);

	/*可以拨号*/
	rtCode=XMS_ctsMakeCallOut(pFlowHandle,&mTargetDevice,mLocalNumber,mTargetNumber,NULL);
	if(rtCode<0)
	{
		if(aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice==false)
		{
			SetLink(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
				&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID);
			aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice=true;
		}
		PlayTone(&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID,BUSY_TONE);
		SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,VOC_WAITHANGUP);
		return;
	}
	
	/*对方数据初始化*/
	strcpy(aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fCallInNumber,mLocalNumber);
	aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fLinkTrunkDeviceID=aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID;
	aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fLinkTrunkChannelNo=aryDspData[mDspNo].pTrunkDevice[mIndex].fChannelNo;
	aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fIsLinkTrunk=true;

	/*修改本通道数据，并放回铃音*/
	aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkTrunkChannelNo=aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fChannelNo;
	aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkTrunkDeviceID=aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fDeviceID;
	aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkTrunk=true;
	SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,VOC_CALL_OUT);
	if(aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice==false)
	{
		SetLink(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
			&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID);
		aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice=true;
	}
	PlayTone(&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID,BACK_TONE);
	return;
}


/***********************************************************************
功能：根据用户指定的通道号，自动播放相应的分配的号码文件
***********************************************************************/
bool	PlayPhoneNumber(DJ_S32	mDspNo,long	mIndex)
{
	DJ_S32	mVoiceDspNo;
	long	mVoiceIndex;
	long	mVoiceDeviceNo;
	RetCode_t	rtCode;
	unsigned int			i;
	char		mFileName[300];
	PlayProperty_t		mPlayProperty;

	XMS_ctsCloseDevice(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID,NULL);
	XMS_ctsOpenDevice(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID,NULL);
	Sleep(700);
	XMS_ctsResetDevice(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID,NULL);
	XMS_ctsGetDevState(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID,NULL);

	if(aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice==false)
	{
		SetLink(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
			&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID);
		aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice=true;
	}

	/*开始进行放音处理*/	
	mVoiceDeviceNo=aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceChannelNo;
	mVoiceDspNo=-1;
	mVoiceIndex=FindIndexByChannelNo(mVoiceDspNo,mVoiceDeviceNo,DEVICE_VOICE);
	mVoiceDspNo=aryVoiceIndex[mVoiceIndex].fDspNo;
	mVoiceIndex=aryVoiceIndex[mVoiceIndex].fIndex;
	i=0;
	mPlayTaskID=mPlayTaskID+1;
	if(mPlayTaskID>255)	mPlayTaskID=0;
	memset((PlayProperty_t *)&mPlayProperty,0,sizeof(PlayProperty_t));
	mPlayProperty.m_u8TaskID=(DJ_U8)mPlayTaskID;
	mPlayProperty.m_u16PlayType=XMS_PLAY_TYPE_FILE_QUEUE;	/*采用索引文件放音*/
	strcpy(mPlayProperty.m_s8PlayContent,SYS_LOCALNUMBER_VOC);
	rtCode=XMS_ctsPlay(pFlowHandle,&aryDspData[mVoiceDspNo].pVoiceDevice[mVoiceIndex].fDeviceID,
		&mPlayProperty,NULL);
	if(rtCode<0)	return false;
	while(i<strlen(aryDspData[mDspNo].pTrunkDevice[mIndex].fLocalNumber))
	{
		if(aryDspData[mDspNo].pTrunkDevice[mIndex].fLocalNumber[i]=='0')
			strcpy(mFileName,SYS_NUMBER_0_VOICE);
		if(aryDspData[mDspNo].pTrunkDevice[mIndex].fLocalNumber[i]=='1')
			strcpy(mFileName,SYS_NUMBER_1_VOICE);
		if(aryDspData[mDspNo].pTrunkDevice[mIndex].fLocalNumber[i]=='2')
			strcpy(mFileName,SYS_NUMBER_2_VOICE);
		if(aryDspData[mDspNo].pTrunkDevice[mIndex].fLocalNumber[i]=='3')
			strcpy(mFileName,SYS_NUMBER_3_VOICE);
		if(aryDspData[mDspNo].pTrunkDevice[mIndex].fLocalNumber[i]=='4')
			strcpy(mFileName,SYS_NUMBER_4_VOICE);
		if(aryDspData[mDspNo].pTrunkDevice[mIndex].fLocalNumber[i]=='5')
			strcpy(mFileName,SYS_NUMBER_5_VOICE);
		if(aryDspData[mDspNo].pTrunkDevice[mIndex].fLocalNumber[i]=='6')
			strcpy(mFileName,SYS_NUMBER_6_VOICE);
		if(aryDspData[mDspNo].pTrunkDevice[mIndex].fLocalNumber[i]=='7')
			strcpy(mFileName,SYS_NUMBER_7_VOICE);
		if(aryDspData[mDspNo].pTrunkDevice[mIndex].fLocalNumber[i]=='8')
			strcpy(mFileName,SYS_NUMBER_8_VOICE);
		if(aryDspData[mDspNo].pTrunkDevice[mIndex].fLocalNumber[i]=='9')
			strcpy(mFileName,SYS_NUMBER_9_VOICE);

		mPlayProperty.m_u8TaskID=(DJ_U8)mPlayTaskID;
		mPlayProperty.m_u16PlayType=XMS_PLAY_TYPE_FILE_QUEUE;	/*采用索引文件放音*/
		strcpy(mPlayProperty.m_s8PlayContent,mFileName);
		rtCode=XMS_ctsPlay(pFlowHandle,&aryDspData[mVoiceDspNo].pVoiceDevice[mVoiceIndex].fDeviceID,
				&mPlayProperty,NULL);
		if(rtCode<0)	return false;
		i++;
	}
	SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,VOC_PLAY_NUMBER);
	return true;
}


/***********************************************************************
功能：查询长途是否开通功能
***********************************************************************/
bool	QueryCallLongStatus(DJ_S32	mDspNo,long	mIndex)
{
	
	if(aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice==false)
	{
		SetLink(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
			&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID);
		aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice=true;
	}
	/*不允许呼叫长途*/
	if(aryDspData[mDspNo].pTrunkDevice[mIndex].fEnabledCallLong==false)
	{
		PlayFile(&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID,NOT_HAVE_LONG_NOTICE_VOC,false);
		/*将本通道的状态设置为非法呼叫长途状态*/
		SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,VOC_PLAY_NOTICE);
	}
	/*允许呼叫长途*/
	if(aryDspData[mDspNo].pTrunkDevice[mIndex].fEnabledCallLong==true)
	{
		PlayFile(&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID,HAVE_LONG_NOTICE_VOC,false);
		/*将本通道的状态设置为非法呼叫长途状态*/
		SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,VOC_PLAY_NOTICE);
	}
	return true;
}

/***********************************************************************
功能：开通长途功能
***********************************************************************/
bool	SetEnableCallLong(DJ_S32	mDspNo,long	mIndex)
{
	CDBProcess		objDb;
	char			mSQL[200];
	long			mChannelNo;
	char			mTemp[20];
	PlayProperty_t	mPlayProperty;
	RetCode_t		rtCode;

	mChannelNo=aryDspData[mDspNo].pTrunkDevice[mIndex].fChannelNo;
	strcpy(mSQL,"update objUserLine set flongenable='开通' where fdspno='" );
	itoa(mDspNo,mTemp,10);
	strcat(mSQL,mTemp);
	strcat(mSQL,"' and flineno='");
	itoa(mChannelNo,mTemp,10);
	strcat(mSQL,mTemp);
	strcat(mSQL,"'");
	if(!objDb.ConnectDB())	return false;
	if(!objDb.ExcueteSQL(mSQL))	return false;	
	aryDspData[mDspNo].pTrunkDevice[mIndex].fEnabledCallLong=true;
	mPlayTaskID=mPlayTaskID+1;
	if(mPlayTaskID>255) mPlayTaskID=0;
	/*开始播放语音*/
	if(aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice==false)
	{
		SetLink(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
			&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID);
		aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice=true;
	}

	memset((PlayProperty_t *)&mPlayProperty,0,sizeof(PlayProperty_t));
	mPlayProperty.m_u8TaskID=(DJ_U8)mPlayTaskID;
	strcpy(mPlayProperty.m_s8PlayContent,SETTING_FINISH_VOC);
	mPlayProperty.m_u16PlayType=XMS_PLAY_TYPE_FILE_QUEUE;	/*采用索引文件放音*/
	rtCode=XMS_ctsPlay(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID,
		&mPlayProperty,NULL);
	if(rtCode<0)	return false;

	mPlayProperty.m_u8TaskID=(DJ_U8)mPlayTaskID;
	strcpy(mPlayProperty.m_s8PlayContent,HAVE_LONG_NOTICE_VOC);
	mPlayProperty.m_u16PlayType=XMS_PLAY_TYPE_FILE_QUEUE;	/*采用索引文件放音*/
	rtCode=XMS_ctsPlay(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID,
		&mPlayProperty,NULL);
	if(rtCode<0)	return false;
	/*将本通道的状态设置为非法呼叫长途状态*/
	SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,VOC_PLAY_NOTICE);
	return true;
}

/***********************************************************************
功能：禁止长途功能
***********************************************************************/
bool	SetDisEnableCallLong(DJ_S32	mDspNo,long	mIndex)
{
	CDBProcess		objDb;
	char			mSQL[200];
	long			mChannelNo;
	char			mTemp[20];
	PlayProperty_t	mPlayProperty;
	RetCode_t		rtCode;

	mChannelNo=aryDspData[mDspNo].pTrunkDevice[mIndex].fChannelNo;
	strcpy(mSQL,"update objUserLine set flongenable='禁止' where fdspno='" );
	itoa(mDspNo,mTemp,10);
	strcat(mSQL,mTemp);
	strcat(mSQL,"' and flineno='");
	itoa(mChannelNo,mTemp,10);
	strcat(mSQL,mTemp);
	strcat(mSQL,"'");
	if(!objDb.ConnectDB())	return false;
	if(!objDb.ExcueteSQL(mSQL))	return false;	
	aryDspData[mDspNo].pTrunkDevice[mIndex].fEnabledCallLong=false;
	mPlayTaskID=mPlayTaskID+1;
	if(mPlayTaskID>255) mPlayTaskID=0;
	/*开始播放语音*/
	if(aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice==false)
	{
		SetLink(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
			&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID);
		aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice=true;
	}

	memset((PlayProperty_t *)&mPlayProperty,0,sizeof(PlayProperty_t));
	mPlayProperty.m_u8TaskID=(DJ_U8)mPlayTaskID;
	strcpy(mPlayProperty.m_s8PlayContent,SETTING_FINISH_VOC);
	mPlayProperty.m_u16PlayType=XMS_PLAY_TYPE_FILE_QUEUE;	/*采用索引文件放音*/
	rtCode=XMS_ctsPlay(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID,
		&mPlayProperty,NULL);
	if(rtCode<0)	return false;

	mPlayProperty.m_u8TaskID=(DJ_U8)mPlayTaskID;
	strcpy(mPlayProperty.m_s8PlayContent,NOT_HAVE_LONG_NOTICE_VOC);
	mPlayProperty.m_u16PlayType=XMS_PLAY_TYPE_FILE_QUEUE;	/*采用索引文件放音*/
	rtCode=XMS_ctsPlay(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID,
		&mPlayProperty,NULL);
	if(rtCode<0)	return false;
	/*将本通道的状态设置为非法呼叫长途状态*/
	SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,VOC_PLAY_NOTICE);
	return true;
}


/***********************************************************************
功能：播放菜单声音
***********************************************************************/
bool	PlayMenu(DJ_S32	mDspNo,long	mIndex)
{
	
	if(aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice==false)
	{
		SetLink(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
			&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID);
		aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice=true;
	}
	PlayFile(&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID,MENU_VOC,false);
	/*将本通道的状态设置为非法呼叫长途状态*/
	SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,VOC_PLAY_NOTICE);
	return true;
}


/***********************************************************************
功能：开启电话
***********************************************************************/
bool	SetEnableCall(DJ_S32	mDspNo,long	mIndex)
{
	CDBProcess		objDb;
	char			mSQL[200];
	long			mChannelNo;
	char			mTemp[20];
	PlayProperty_t	mPlayProperty;
	RetCode_t		rtCode;

	mChannelNo=aryDspData[mDspNo].pTrunkDevice[mIndex].fChannelNo;
	strcpy(mSQL,"update objUserLine set fisenable='启用' where fdspno='" );
	itoa(mDspNo,mTemp,10);
	strcat(mSQL,mTemp);
	strcat(mSQL,"' and flineno='");
	itoa(mChannelNo,mTemp,10);
	strcat(mSQL,mTemp);
	strcat(mSQL,"'");
	if(!objDb.ConnectDB())	return false;
	if(!objDb.ExcueteSQL(mSQL))	return false;	
	aryDspData[mDspNo].pTrunkDevice[mIndex].fEnabled=true;
	mPlayTaskID=mPlayTaskID+1;
	if(mPlayTaskID>255) mPlayTaskID=0;
	/*开始播放语音*/
	if(aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice==false)
	{
		SetLink(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
			&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID);
		aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice=true;
	}
	memset((PlayProperty_t *)&mPlayProperty,0,sizeof(PlayProperty_t));
	mPlayProperty.m_u8TaskID=(DJ_U8)mPlayTaskID;
	strcpy(mPlayProperty.m_s8PlayContent,SETTING_FINISH_VOC);
	mPlayProperty.m_u16PlayType=XMS_PLAY_TYPE_FILE_QUEUE;	/*采用索引文件放音*/
	rtCode=XMS_ctsPlay(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID,
		&mPlayProperty,NULL);
	if(rtCode<0)	return false;
	mPlayProperty.m_u8TaskID=(DJ_U8)mPlayTaskID;
	strcpy(mPlayProperty.m_s8PlayContent,ENABLE_THE_CALL_VOC);
	mPlayProperty.m_u16PlayType=XMS_PLAY_TYPE_FILE_QUEUE;	/*采用索引文件放音*/
	rtCode=XMS_ctsPlay(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID,
		&mPlayProperty,NULL);
	if(rtCode<0)	return false;
	/*将本通道的状态设置为非法呼叫长途状态*/
	SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,VOC_PLAY_NOTICE);
	return true;
}


/***********************************************************************
功能：关闭电话
***********************************************************************/
bool	SetDisableCall(DJ_S32	mDspNo,long	mIndex)
{
	CDBProcess		objDb;
	char			mSQL[200];
	long			mChannelNo;
	char			mTemp[20];
	PlayProperty_t	mPlayProperty;
	RetCode_t		rtCode;

	mChannelNo=aryDspData[mDspNo].pTrunkDevice[mIndex].fChannelNo;
	strcpy(mSQL,"update objUserLine set fisenable='禁止' where fdspno='" );
	itoa(mDspNo,mTemp,10);
	strcat(mSQL,mTemp);
	strcat(mSQL,"' and flineno='");
	itoa(mChannelNo,mTemp,10);
	strcat(mSQL,mTemp);
	strcat(mSQL,"'");
	if(!objDb.ConnectDB())	return false;
	if(!objDb.ExcueteSQL(mSQL))	return false;	
	aryDspData[mDspNo].pTrunkDevice[mIndex].fEnabled=false;
	mPlayTaskID=mPlayTaskID+1;
	if(mPlayTaskID>255) mPlayTaskID=0;
	/*开始播放语音*/
	if(aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice==false)
	{
		SetLink(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
			&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID);
		aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice=true;
	}
	memset((PlayProperty_t *)&mPlayProperty,0,sizeof(PlayProperty_t));
	mPlayProperty.m_u8TaskID=(DJ_U8)mPlayTaskID;
	strcpy(mPlayProperty.m_s8PlayContent,SETTING_FINISH_VOC);
	mPlayProperty.m_u16PlayType=XMS_PLAY_TYPE_FILE_QUEUE;	/*采用索引文件放音*/
	rtCode=XMS_ctsPlay(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID,
		&mPlayProperty,NULL);
	if(rtCode<0)	return false;
	mPlayProperty.m_u8TaskID=(DJ_U8)mPlayTaskID;
	strcpy(mPlayProperty.m_s8PlayContent,DISABLE_THE_CALL_VOC);
	mPlayProperty.m_u16PlayType=XMS_PLAY_TYPE_FILE_QUEUE;	/*采用索引文件放音*/
	rtCode=XMS_ctsPlay(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID,
		&mPlayProperty,NULL);
	if(rtCode<0)	return false;
	/*将本通道的状态设置为非法呼叫长途状态*/
	SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,VOC_PLAY_NOTICE);
	return true;
}


/***********************************************************************
功能：拍叉簧处理
***********************************************************************/
void	ChaHuanProcess(DJ_S32	mDspNo,long mIndex)
{
	DJ_S32		mLinkDspNo;
	long		mLinkIndex;
	long		mLinkDeviceIndex;
	long		mLinkChannelNo;

	/*第一次拍叉簧的处理*/
	if(aryDspData[mDspNo].pTrunkDevice[mIndex].fChaHuanCount==1)
	{
		/*如果当前连接的是坐席*/
		if(aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkTrunk==true)
		{ 
			/*将当前连接的通道保存起来*/
			aryDspData[mDspNo].pTrunkDevice[mIndex].fPrevTrunkChannelNo=
				aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkTrunkChannelNo;
			aryDspData[mDspNo].pTrunkDevice[mIndex].fPrevTrunkDeviceID=
				aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkTrunkDeviceID;
			aryDspData[mDspNo].pTrunkDevice[mIndex].fPrevDeviceType=1;			/*表示连接的是Trunk*/
			
			/*断开原通话的通道之间的连接*/
			DeleteLink(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
				&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkTrunkDeviceID);
			/*对通道本身，建立语音连接，并放拨号音*/
			if(aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice==false)
			{
				SetLink(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
					&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID);
				aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice=true;
			}
			PlayTone(&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID,DIAL_TONE);
			/*本通道的状态设置为拍叉簧状态*/
			SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,VOC_FLASH_WAIT_DIAL);
			/*对连接的接口通道，将其与他所对应的语音通道建立连接*/
			mLinkChannelNo=aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkTrunkChannelNo;	
			mLinkDspNo=-1;
			mLinkIndex=FindIndexByChannelNo(mLinkDspNo,mLinkChannelNo,DEVICE_TRUNK);
			mLinkDspNo=aryTrunkIndex[mLinkIndex].fDspNo;
			mLinkDeviceIndex=aryTrunkIndex[mLinkIndex].fIndex;
			if(aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fIsLinkVoice==false)
			{
				SetLink(&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDeviceID,
					&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkVoiceDeviceID);
				aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fIsLinkVoice=true;
			}
			/*对他播放转接音乐*/
			PlayFile(&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkVoiceDeviceID,BACKGROUND_MUSIC,false);
			/*等待本通道拨号即可*/
			aryDspData[mDspNo].pTrunkDevice[mIndex].fChaHuanIsReset=false;
			aryDspData[mDspNo].pTrunkDevice[mIndex].fChaHuanIsCallStatus=false;
		}
		/*如果当前连接的是VOIP*/
		if(aryDspData[mDspNo].pTrunkDevice[mIndex].fisLinkVoipVoice==true)
		{
			long		mLinkVoipVoiceNo;
			DJ_S32		mLinkVoipVoiceDspNo;
			long		mLinkVoipVoiceIndex;

			/*将当前连接的通道保存起来*/
			aryDspData[mDspNo].pTrunkDevice[mIndex].fPrevVoipChannelNo=
				aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoipVoiceDeviceNo;
			aryDspData[mDspNo].pTrunkDevice[mIndex].fPrevVoipDeviceID=
				aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoipVoiceDeviceID;
			aryDspData[mDspNo].pTrunkDevice[mIndex].fPrevDeviceType=2;			/*表示为VOIP*/
			/*获取相连接的VOIP通道信息*/
			mLinkVoipVoiceNo=aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoipVoiceDeviceNo;
			mLinkVoipVoiceDspNo=-1;
			mLinkVoipVoiceIndex=FindIndexByChannelNo(mLinkVoipVoiceDspNo,mLinkVoipVoiceNo,DEVICE_VOICE);
			mLinkVoipVoiceDspNo=aryVoiceIndex[mLinkVoipVoiceIndex].fDspNo;
			mLinkVoipVoiceIndex=aryVoiceIndex[mLinkVoipVoiceIndex].fIndex;
			/*VOIP设备的全局编号*/
			mLinkChannelNo=aryDspData[mLinkVoipVoiceDspNo].pVoiceDevice[mLinkVoipVoiceIndex].fLinkVoipChannelNo;
			mLinkDspNo=-1;
			mLinkDeviceIndex=FindIndexByChannelNo(mLinkDspNo,mLinkChannelNo,DEVICE_VOIP);
			mLinkDspNo=aryVoipIndex[mLinkDeviceIndex].fDspNo;
			mLinkDeviceIndex=aryVoipIndex[mLinkDeviceIndex].fIndex;
			/*断开本中继通道与VOIP通道之间的连接*/
			DeleteLink(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
				&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoipVoiceDeviceID);
			/*对通道本身放拨号音*/
			if(aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice==false)
			{
				SetLink(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
					&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID);
				aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice=true;
			}
			PlayTone(&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID,DIAL_TONE);
			SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,VOC_FLASH_WAIT_DIAL);
			/*对VOIP通道放转接音*/
			if(aryDspData[mLinkDspNo].pVoipDevice[mLinkDeviceIndex].fIsLinkVoice==false)
			{
				XMS_ctsLinkDevice(pFlowHandle,&aryDspData[mLinkDspNo].pVoipDevice[mLinkDeviceIndex].fDeviceID,
					&aryDspData[mLinkDspNo].pVoipDevice[mLinkDeviceIndex].fLinkVoiceDeviceID,NULL);
			}
			/*设置VOIP通道的混音*/
			MixerControlParam_t paramMixer = {0};
			paramMixer.m_u8SRC1_Ctrl = XMS_MIXER_FROM_PLAY ;
			paramMixer.m_u16SRC_ChID1 = aryDspData[mLinkDspNo].pVoipDevice[mLinkDeviceIndex].fLinkVoiceDeviceID.m_s16ChannelID;
			paramMixer.m_u8SRC2_Ctrl = XMS_MIXER_FROM_INPUT;
			paramMixer.m_u16SRC_ChID2 = aryDspData[mLinkDspNo].pVoipDevice[mLinkDeviceIndex].fLinkVoiceDeviceID.m_s16ChannelID;
			RetCode_t ret = XMS_ctsSetParam(pFlowHandle, &aryDspData[mLinkDspNo].pVoipDevice[mLinkDeviceIndex].fDeviceID,
				VOIP_PARAM_RTPMIXER, sizeof(paramMixer), &paramMixer);
			PlayFile(&aryDspData[mLinkDspNo].pVoipDevice[mLinkDeviceIndex].fLinkVoiceDeviceID,BACKGROUND_MUSIC,false);
			aryDspData[mDspNo].pTrunkDevice[mIndex].fChaHuanIsReset=false;
			aryDspData[mDspNo].pTrunkDevice[mIndex].fChaHuanIsCallStatus=false;
		}
	}

	/*第二次拍叉簧，即拍回来*/
	if(aryDspData[mDspNo].pTrunkDevice[mIndex].fChaHuanCount==2)
	{
		DJ_S32		mLinkDspNo;
		long		mLinkDeviceIndex;
		/*接回原有通道*/
		mLinkDspNo=mDspNo;
		mLinkDeviceIndex=mIndex;

		if(aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fPrevDeviceType==1)
		{	
			/*原通道为接口设备*/
			ControlPlay_t		mPlayControl;
			DJ_S32				mPrevDspNo;
			long				mPrevDeviceNo;
			long				mPrevDeviceIndex;
			
			mPlayControl.m_u16ControlType=XMS_STOP_PLAY;
			mPrevDeviceNo=aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fPrevTrunkChannelNo;
			mPrevDspNo=-1;
			mPrevDeviceIndex=FindIndexByChannelNo(mPrevDspNo,mPrevDeviceNo,DEVICE_TRUNK);
			mPrevDspNo=aryTrunkIndex[mPrevDeviceIndex].fDspNo;
			mPrevDeviceIndex=aryTrunkIndex[mPrevDeviceIndex].fIndex;
			XMS_ctsControlPlay(pFlowHandle,&aryDspData[mPrevDspNo].pTrunkDevice[mPrevDeviceIndex].fLinkVoiceDeviceID,&mPlayControl,NULL);
			AddMsg("拍二次停止放音");
			DeleteLink(&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDeviceID,
				&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkVoiceDeviceID);
			aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fIsLinkVoice=false;
			DeleteLink(&aryDspData[mPrevDspNo].pTrunkDevice[mPrevDeviceIndex].fDeviceID,
					   &aryDspData[mPrevDspNo].pTrunkDevice[mPrevDeviceIndex].fLinkVoiceDeviceID);
			aryDspData[mPrevDspNo].pTrunkDevice[mPrevDeviceIndex].fIsLinkVoice=false;
			SetLink(&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDeviceID,
				&aryDspData[mPrevDspNo].pTrunkDevice[mPrevDeviceIndex].fDeviceID);
			aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fIsLinkTrunk=true;
			aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fisLinkVoipVoice=false;
			aryDspData[mPrevDspNo].pTrunkDevice[mPrevDeviceIndex].fIsLinkTrunk=true;
			aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fIsLinkVoice=false;
			aryDspData[mPrevDspNo].pTrunkDevice[mPrevDeviceIndex].fIsLinkVoice=false;
			aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkTrunkChannelNo=aryDspData[mPrevDspNo].pTrunkDevice[mPrevDeviceIndex].fChannelNo;
			aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkTrunkDeviceID=aryDspData[mPrevDspNo].pTrunkDevice[mPrevDeviceIndex].fDeviceID;
			strcpy(aryDspData[mPrevDspNo].pTrunkDevice[mPrevDeviceIndex].fDtmfBuff,"");
			strcpy(aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDtmfBuff,"");
			aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fPrevDeviceType=1;
		}
		if(aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fPrevDeviceType==2)
		{
			/*原通道为VOIP设备*/
			ControlPlay_t		mPlayControl;
			DJ_S32				mPrevDspNo;
			long				mPrevDeviceNo;
			long				mPrevDeviceIndex;
			
			mPlayControl.m_u16ControlType=XMS_STOP_PLAY;
			mPrevDeviceNo=aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fPrevVoipChannelNo;
			mPrevDspNo=-1;
			mPrevDeviceIndex=FindIndexByChannelNo(mPrevDspNo,mPrevDeviceNo,DEVICE_VOICE);
			mPrevDspNo=aryVoiceIndex[mPrevDeviceIndex].fDspNo;
			mPrevDeviceIndex=aryVoiceIndex[mPrevDeviceIndex].fIndex;
			XMS_ctsControlPlay(pFlowHandle,&aryDspData[mPrevDspNo].pVoipDevice[mPrevDeviceIndex].fLinkVoiceDeviceID,&mPlayControl,NULL);
			AddMsg("拍第二次停止放音2");
			/*将语音通道与接口通道进行绑定*/
			SetLink(&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDeviceID,
				&aryDspData[mPrevDspNo].pVoipDevice[mPrevDeviceIndex].fLinkVoiceDeviceID);
			aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fIsLinkTrunk=false;
			aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fIsLinkVoice=false;
			aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fisLinkVoipVoice=true;
			aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkTrunkChannelNo=-1;
			aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkVoipVoiceDeviceNo=aryDspData[mPrevDspNo].pVoipDevice[mPrevDeviceIndex].fLinkVoiceChannelNo;
			aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkVoipVoiceDeviceID=aryDspData[mPrevDspNo].pVoipDevice[mPrevDeviceIndex].fLinkVoiceDeviceID;	
			aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fPrevDeviceType=2;
		}
		/*将拍叉簧数据清除*/
		aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fIsChaHuan=false;
		aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fChaHuanCount=0;
		aryDspData[mDspNo].pTrunkDevice[mIndex].fChaHuanIsReset=false;
		aryDspData[mDspNo].pTrunkDevice[mIndex].fChaHuanIsCallStatus=false;
		SetTrunkDeviceState(&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDeviceID,VOC_CONNECTED);
	}
	return;
}


void	DisplayVoipStatus()
{

}

/*在信息显示框中显示 事件类型、DSP模块号、通道号*/
void	DisplayEvt(Acs_Evt_t *pEvt)
{
	char	mInfo[200];
	strcpy(mInfo,"");
	switch(pEvt->m_s32EventType)
	{
	case XMS_EVT_CLOSE_DEVICE:	
		{
			strcpy(mInfo,"CLOSE DEVICE EVT:");
			break;
		}
	case XMS_EVT_CALLIN:
		{
			strcpy(mInfo,"EVT_CALLIN:");
			break;
		}
	case XMS_EVT_CALLOUT:
		{
			strcpy(mInfo,"EVT_CALLOUT");
			break;
		}
	case XMS_EVT_RECVIODATA: 
		{
			strcpy(mInfo,"EVT_RECVIODATA:");
			break;
		}
	case XMS_EVT_PLAY:
		{
			strcpy(mInfo,"EVT_PLAY:");
			break;
		}
	case XMS_EVT_CLEARCALL:
		{
			strcpy(mInfo,"EVT_CLEARCALL:");
			break;
		}
	case XMS_EVT_QUERY_ONE_DSP_START:
		{
			strcpy(mInfo,"EVT_QUERY_DSP:");
			break;
		}
	case XMS_EVT_QUERY_ONE_DSP_END:
		{
			strcpy(mInfo,"EVT_QUERY_DSP_END:");
			break;
		}
	case XMS_EVT_OPEN_DEVICE:
		{
			strcpy(mInfo,"EVT_OPEN_DEVICE:");
			break;
		}
	case XMS_EVT_RESET_DEVICE:
		{
			strcpy(mInfo,"EVT_RESET_DEVICE:");
			break;
		}
	}
	if(!strcmp(mInfo,"")==0)
	{
		char	mTemp[20];
		itoa(pEvt->m_DeviceID.m_s8ModuleID,mTemp,10);
		strcat(mInfo," DSP:");
		strcat(mInfo,mTemp);
		itoa(pEvt->m_DeviceID.m_s16ChannelID,mTemp,10);
		strcat(mInfo," ChannelID:");
		strcat(mInfo,mTemp);
		AddMsg(mInfo);
	}
}


/***********************************************************************
功能：查找一个可用的抢接汇线通外线号码的外线
***********************************************************************/
long	FindHuiXianFreeTrunkToUse()
{
	bool	mFind;
	long	mResult;
	long	i;
	long	mIndex;
	DJ_S32	mDspNo;

	mFind=false;
	mResult=-1;
	i=0;
	while(i<=mTrunkCount && mFind==false)
	{
		mIndex=aryTrunkIndex[i].fIndex;
		mDspNo=aryTrunkIndex[i].fDspNo;
		if(aryDspData[mDspNo].pTrunkDevice[mIndex].fLineState==DCS_FREE && aryDspData[mDspNo].pTrunkDevice[mIndex].fLineType==LINE_OUT 
		   && aryDspData[mDspNo].pTrunkDevice[mIndex].fEnabled==true && strcmp(aryDspData[mDspNo].pTrunkDevice[mIndex].fLocalNumber,mFirstPhone)!=0)
		{
			mFind=true;
			mResult=i;
		}
		i++;
	}
	return mResult;
}



/***********************************************************************
功能：根据速拨号查找所在的设备全局通道号
***********************************************************************/
long    FindChannelNoBySpeedDial(char * mSpeedDial)
{
	long	mChannelNo;
	long	i;
	long	j;
	bool	mFound;

	mFound=false;
	i=0;
	mChannelNo=-1;
	while(i<=mDspCount+1 && !mFound)
	{
		if(aryDspData[i].fIsExits==1)
		{
			j=0;
			while(j<=aryDspData[i].fTrunkCount && !mFound)
			{
				if(strcmp(aryDspData[i].pTrunkDevice[j].fSpeedDial,mSpeedDial)==0)
				{
					mChannelNo=aryDspData[i].pTrunkDevice[j].fChannelNo;
					mFound=true;
				}
				j++;
			}
		}
		i++;
	}
	return mChannelNo;
}


/************************************************************************/
/* 利用IP呼叫外线                                                                     */
/************************************************************************/
void	CallTrunkByVoip(long mDspNo,long mIndex)
{
	//注意：该函数没有实现汇线通
	char	mLocalNumber[20];
	char	mTargetNumber[20];
	long	mTargetDspNo;
	long	mTargetDeviceIdIndex;
	RetCode_t	rtCode;
	long	mTrunkIndex;

	strcpy(mLocalNumber,aryDspData[mDspNo].pVoipDevice[mIndex].fLocalNumber);
	strcpy(mTargetNumber,"");
	strcpy(mLocalNumber,aryDspData[mDspNo].pVoipDevice[mIndex].fCallInNumber);
	strcpy(mTargetNumber, aryDspData[mDspNo].pVoipDevice[mIndex].fCallOutNumber);
	strcpy(aryDspData[mDspNo].pVoipDevice[mIndex].fDtmfBuff,"");

	/*查找一个可用的外线通道*/
	mTrunkIndex=FindFreeTrunkToUse();
	if(mTrunkIndex==-1)
	{
		/*没有空闲的外线通道*/
		if(aryDspData[mDspNo].pVoipDevice[mIndex].fIsLinkVoice==false)
		{
			XMS_ctsLinkDevice(pFlowHandle,&aryDspData[mDspNo].pVoipDevice[mIndex].fDeviceID,
				&aryDspData[mDspNo].pVoipDevice[mIndex].fLinkVoiceDeviceID,NULL);
			aryDspData[mDspNo].pVoipDevice[mIndex].fIsLinkVoice=true;
		}
		DJ_U16		mCommandType;
		MixerControlParam_t	mParam;

		mCommandType=VOIP_PARAM_RTPMIXER;
		mParam.m_u8SRC1_Ctrl=XMS_MIXER_FROM_PLAY;
		mParam.m_u16SRC_ChID1=aryDspData[mDspNo].pVoipDevice[mIndex].fLinkVoiceDeviceID.m_s16ChannelID;
		XMS_ctsSetParam(pFlowHandle,&aryDspData[mDspNo].pVoipDevice[mIndex].fDeviceID,mCommandType,sizeof(mParam),(DJ_Void *)&mParam);

		PlayFile(&aryDspData[mDspNo].pVoipDevice[mIndex].fLinkVoiceDeviceID,NO_TRUNK_FOR_DIAL_VOC,false);

		SetVoipDeviceState(&aryDspData[mDspNo].pVoipDevice[mIndex].fDeviceID,VOIP_PLAY_FILE);
		return;		
	}

	mTargetDspNo=aryTrunkIndex[mTrunkIndex].fDspNo;
	mTargetDeviceIdIndex=aryTrunkIndex[mTrunkIndex].fIndex;
	
	SetTrunkLineInitParameter(&aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fDeviceID,
		 &aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fLinkVoiceDeviceID);
	
	/*开始进行呼叫*/
	rtCode=XMS_ctsMakeCallOut(pFlowHandle,&aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fDeviceID,
		mLocalNumber,mTargetNumber,NULL);
	Sleep(5000);

	if(rtCode<0)
	{
		/*呼叫失败*/
		if(aryDspData[mDspNo].pVoipDevice[mIndex].fIsLinkVoice==false)
		{
			SetLink(&aryDspData[mDspNo].pVoipDevice[mIndex].fDeviceID,
				&aryDspData[mDspNo].pVoipDevice[mIndex].fLinkVoiceDeviceID);
			aryDspData[mDspNo].pVoipDevice[mIndex].fIsLinkVoice=true;
		}
		DJ_U16		mCommandType;
		MixerControlParam_t	mParam;

		
		mCommandType=VOIP_PARAM_RTPMIXER;
		mParam.m_u8SRC1_Ctrl=XMS_MIXER_FROM_PLAY;
		mParam.m_u16SRC_ChID1=aryDspData[mDspNo].pVoipDevice[mIndex].fLinkVoiceDeviceID.m_s16ChannelID;
		XMS_ctsSetParam(pFlowHandle,&aryDspData[mDspNo].pVoipDevice[mIndex].fDeviceID,mCommandType,sizeof(mParam),(DJ_Void *)&mParam);

		PlayTone(&aryDspData[mDspNo].pVoipDevice[mIndex].fLinkVoiceDeviceID,BUSY_TONE);
		SetVoipDeviceState(&aryDspData[mDspNo].pVoipDevice[mIndex].fDeviceID,VOIP_WAITHUNGUP);
		return;		
	}


	/*将ip的语音与外线建立时隙连接*/
	XMS_ctsLinkDevice(pFlowHandle,&aryDspData[mDspNo].pVoipDevice[mIndex].fDeviceID,
			&aryDspData[mDspNo].pVoipDevice[mIndex].fLinkVoiceDeviceID,NULL);
	//Sleep(1000);
	SetLink(&aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fDeviceID,
		    &aryDspData[mDspNo].pVoipDevice[mIndex].fLinkVoiceDeviceID);

	/*修改外线接口数据*/
	aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fisLinkVoipVoice=true;
	aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fLinkVoipVoiceDeviceID=
		aryDspData[mDspNo].pVoipDevice[mIndex].fLinkVoiceDeviceID;
	aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fLinkVoipVoiceDeviceNo=
		aryDspData[mDspNo].pVoipDevice[mIndex].fLinkVoiceChannelNo;
	aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fLinkVoipDeviceID=aryDspData[mDspNo].pVoipDevice[mIndex].fDeviceID;
	aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fLinkVoipDeviceNo=aryDspData[mDspNo].pVoipDevice[mIndex].fChannelNo;

	/*修改本接口数据*/
	aryDspData[mDspNo].pVoipDevice[mIndex].fIsLinkTrunk=true;
	aryDspData[mDspNo].pVoipDevice[mIndex].fLinkTrunkChannelNo=aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fChannelNo;
	aryDspData[mDspNo].pVoipDevice[mIndex].fLinkTrunkChannelID=aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fDeviceID;

	SetVoipDeviceState(&aryDspData[mDspNo].pVoipDevice[mIndex].fDeviceID,VOIP_CALL_VOIP);
	SetTrunkDeviceState(&aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fDeviceID,VOC_CALL_OUT);
	return;
}


bool	PlayFileCircly(DeviceID_t *pDevice,DJ_S8 *pFileName,bool isStop)					/*文件放音*/
{
	RetCode_t	rtCode;
	PlayProperty_t		mProperty;
	ControlPlay_t		mPlayControl;
	int					i;
	
	mPlayControl.m_u16ControlType=XMS_STOP_PLAY;
	XMS_ctsControlPlay(pFlowHandle,pDevice,&mPlayControl,NULL);
	PlayTone(pDevice,STOP_TONE);
	XMS_ctsCloseDevice(pFlowHandle,pDevice,NULL);
	XMS_ctsOpenDevice(pFlowHandle,pDevice,NULL);
	Sleep(700);
	XMS_ctsResetDevice(pFlowHandle,pDevice,NULL);
	XMS_ctsGetDevState(pFlowHandle,pDevice,NULL);
	for(i=0;i<500;i++)	i++;
	
	memset(&mProperty,0,sizeof(mProperty));
	mProperty.m_u16PlayType=XMS_PLAY_TYPE_FILE;			/*采用文件放音*/
	if(isStop)
		mProperty.m_u32StopMode=XMS_Any_Code;			/*设置为收到任意一个dtmf码则停止放音*/
	mProperty.m_u32MaxTime=999999;
	mPlayTaskID=mPlayTaskID+1;
	if(mPlayTaskID>255)	mPlayTaskID=0;					/*全局的播放任务ID*/
	mProperty.m_u8TaskID=(DJ_U8)mPlayTaskID;
	strcpy(mProperty.m_s8PlayContent,pFileName);
	rtCode=XMS_ctsPlay(pFlowHandle,pDevice,&mProperty,NULL);
	if(rtCode<0)	return false;
	return true;

}
