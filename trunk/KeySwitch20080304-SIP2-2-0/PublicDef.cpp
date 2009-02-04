#include "stdafx.h"
#include "publicdef.h"
#include "CAS_Common_Code\CAS_Common_Cfg.h"		
#include "DBProcess.h"
#include "DJAcsDevState.h"


/*�ⲿȫ�ֱ�������******************************************************/
CKeySwitchDlg			*pMainDlg;		//���Ի���
extern	CKeySwitchApp	theApp;			//Ӧ�ó������

/*ȫ�ֱ�������**********************************************************/
ACSHandle_t		pFlowHandle;							/*ȫ�����̾��*/
ServerID_t		pServerID;								/*���̷��������ڵ�IP*/
DJ_U8			pAppID;									/*Ӧ�ó���ID*/
DJ_S32			mDebugOn;								/*�Ƿ����õ�ʽģʽ�����ڿ���XMS�ײ�*/
TYPE_VOICE_DATA	*pVoiceData[];							/*ȫ���豸�ṹ����ָ��*/
CmdParamData_CAS_t g_Param_CAS;							/*�й�һ���������ò���*/	
char			pDbConnStr[150];						/*���ݿ������ַ���*/
long			mPubDeviceNo;							/*ȫ���豸��ǰ���*/
long			mPubDspNo;								/*ȫ��DSP��ǰ���*/
long			mDspCount;								/*��Ч��DSP������0��ʼ����*/
long			mOpenDspCount;							/*�Ѿ��򿪵�DSP����*/
long			mVoiceCount;							/*�����豸������0��ʼ����*/
long			mOpenVoiceCount;						/*�Ѿ��򿪵������豸����*/
long			mTrunkCount;							/*�ӿ��豸������0��ʼ����*/
long			mOpenTrunkCount;						/*�Ѿ��򿪵Ľӿ��豸����*/
long			mVoipCount;								/*voip�豸������0��ʼ����*/
long			mOpenVoipCount;							/*�Ѿ��򿪵�voip�豸����*/
long			pRowNum;								/*listctrl��ǰ���к�*/	
long			mPubVoiceNo;							/*voice�豸��ȫ�ֱ��*/		
long			mPubVoipNo;								/*voip�豸��ȫ�ֱ��*/		
long			mPubTrunkNo;							/*trunk�豸��ȫ�ֱ��*/		
long			mPlayTaskID;							/*����ȫ��������*/
char			mFirstPhone[20];						/*����ͨ��ѡ����*/
char			mTurnCode[20];							/*����ͨת�Ӵ���*/
char			mPrevCode[20];							/*����ͨǰ������*/


/*VOIPģ���ȫ�ֱ�������************************************************/
char			pVoipServerIp[20];						/*�Է�voip���������ڷ�����ip*/
UINT			pVoipServerPort;						/*�Է��������˿�*/
int				pVoipProtocol;							/*Э������*/
char			pVoipUserName[20];						/*�����û���*/
char			pLocalVoipServerIP[20];					/*����VOIP��������ַ*/
UINT			pLocalVoipServerPort;					/*����VOIP�������Ͽ�*/
int				pLocalVoipProtocol;							/*����VOIPЭ��*/
char			pLocalVoipUserName[20];					/*����VOIP�û���*/		
DJ_S32			pVoipDspNo;								/*VOIP��ʹ�õ�DSP*/
char			pQueryPhoneNumber[10];					/*�����ָ���ķֻ�����*/
bool			fIsRun;									/*�Ƿ��Ѿ�����*/


/*�豸ȫ������**********************************************************/
TYPE_DSP_DATA			aryDspData[MAX_DSP_COUNT];				/*DSP�豸ȫ������*/
TYPE_INTERFACE_INDEX	aryVoiceIndex[MAX_VOICE_COUNT];		/*ȫ�������豸��������*/
TYPE_INTERFACE_INDEX	aryTrunkIndex[MAX_INTERFACE_COUNT];		/*ȫ��ͨ���豸��������*/
TYPE_INTERFACE_INDEX	aryVoipIndex[MAX_VOIP_COUNT];			/*ȫ��VOIP�豸��������*/
bool			mUpdateDb;										/*��ʼ��ʱ���Ƿ�������ݿ�*/

/***********************************************************************
���ܣ���INI�ļ��л�ȡȫ�ֱ�������ȡ�ı���������
pServerID�ṹ�壬���������������ip,port,username,password
***********************************************************************/
bool	getConfigFromIni()
{
	char	pFileName[200];						/*·�����*/
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
	/*�Է�VOIP��������Ϣ��ʼ��*/
	GetPrivateProfileString("VOIP","ServerIP","",pVoipServerIp,200,pFileName);
	pVoipProtocol=GetPrivateProfileInt("VOIP","ProtocolType",0,pFileName);	
	pVoipServerPort=GetPrivateProfileInt("VOIP","ServerPort",0,pFileName);	
	GetPrivateProfileString("VOIP","UserName","",pVoipUserName,200,pFileName);
	/*��ȡ���ص�VOIP��������Ϣ*/
	GetPrivateProfileString("VOIP","LocalServerIP","",pLocalVoipServerIP,200,pFileName);
	pLocalVoipProtocol=GetPrivateProfileInt("VOIP","LocalProtocolType",0,pFileName);	
	pLocalVoipServerPort=GetPrivateProfileInt("VOIP","LocalServerPort",0,pFileName);	
	GetPrivateProfileString("VOIP","UserName","",pLocalVoipUserName,200,pFileName);
	GetPrivateProfileString("TELEPHONE","QUERY_NO","",pQueryPhoneNumber,200,pFileName);
	/*��ȡ����ͨת����Ϣ*/
	GetPrivateProfileString("HUIXIANTONG","FirstPhone","",mFirstPhone,200,pFileName);
	GetPrivateProfileString("HUIXIANTONG","TurnCode","",mTurnCode,200,pFileName);
	GetPrivateProfileString("HUIXIANTONG","PrevCode","",mPrevCode,200,pFileName);
	return true;
}

/***********************************************************************
���ܣ����ð忨��ʼ������
1.��ȡINI�ļ����ã�ΪpServerID��ʼ��
2.����XMS_acsOpenStream������XMS�����������ӡ�ע�⣺�˴����������¼�����
3.�����¼��ص�����
4.���ö�ʱ�������ڼ�����ͨ����DTMF
5.������ȡ�豸�б�����������Ӧ�¼�
6.����mPubDeviceNo=-1��ȫ�ֵ�ǰ�豸����Ϊ-1
7.����mPubDspNo=-1,����DSPȫ�ֱ��Ϊ-1
8.����mPubValidDspCount=-1,����ȫ����Ч��DSP����Ϊ-1.
***********************************************************************/
bool InitSystem()
{
	RetCode_t		rtCode;	
	char			MsgStr[160];
	int				i;
	pMainDlg=(CKeySwitchDlg *)theApp.m_pMainWnd;
	fIsRun=false;		/*ȫ�ֱ�ǣ���ǳ����Ƿ��Ѿ�����*/
	getConfigFromIni();	
	/*���й�һ��������в�����ʼ��*/
	if ( CAS_Common_Cfg_ReadCfg ( &g_Param_CAS ) != 0 )
	{
		sprintf ( MsgStr, "�й�һ�������ʼ��ʧ��!����XMS_CAS_Cfg.INI�ļ��Ƿ������CAS_Common_CodeĿ¼!" );
		MessageBox(NULL, MsgStr, "��ʼ��", MB_OK ) ;
	}
	/*����XMS�е�����ģ�������*/ 
	pAppID=APPLICATION_ID;
	mDebugOn=0;		/*���õ�ʽģʽ��ʼ���ײ�*/
	rtCode=XMS_acsOpenStream(&pFlowHandle,&pServerID,pAppID,32,32,mDebugOn,NULL);	
	if(rtCode==-ACSERR_BADPARAMETER)
	{
		sprintf ( MsgStr, "��ʼ��ʱ�����Ƿ�!" );
		MessageBox(NULL, MsgStr, "��ʼ��", MB_OK ) ;
		return false;
	}
	if(rtCode==-ACSERR_NOSERVER)
	{
		sprintf ( MsgStr, "XMS������û����������XMS������IP����Port��������!" );
		MessageBox(NULL, MsgStr, "��ʼ��", MB_OK ) ;
		return false;
	}
	if(rtCode==-ACSERR_MAXCONN_EXCEED)
	{
		sprintf ( MsgStr, "�뱾XMS��������Ӧ�ó���������Ŀ����128!" );
		MessageBox(NULL, MsgStr, "��ʼ��", MB_OK ) ;
		return false;
	}
	/*�����¼��ص�����*/
	rtCode=XMS_acsSetESR(pFlowHandle,(EsrFunc)XMSEventHandler,0,1);
	if(rtCode!=ACSPOSITIVE_ACK)
	{
		sprintf ( MsgStr, "�����¼��ص�����ʧ��!" );
		MessageBox(NULL, MsgStr, "��ʼ��", MB_OK ) ;
		return false;
	}


	/*��ȡ�豸�б�*/
	if(!getXMSDeviceList())	
	{
		CloseXMSConnect();
		return false;
	}

	/*��ʼ��DSP�ṹ����*/
	for(i=0;i<MAX_DSP_COUNT;i++)
	{
		aryDspData[i].fDspNo=-1;
		aryDspData[i].fDspID=-1;
		aryDspData[i].fDeviceType=DEVICE_DSP;
		aryDspData[i].fIsExits=0;					/*��ʾ����*/
		aryDspData[i].fIsOpen=false;				/*��ʾδ��*/
		aryDspData[i].fTrunkCount=-1;				/*�ر�ע�⣺�˴��������Ǵ�0��ʼ*/
		aryDspData[i].fTrunkOpenCount=-1;
		aryDspData[i].fVoiceCount=-1;
		aryDspData[i].fVoiceOpenCount=-1;
		aryDspData[i].fVoipCount=-1;
		aryDspData[i].fVoipOpenCount=-1;
		aryDspData[i].pTrunkDevice=NULL;
		aryDspData[i].pVoiceDevice=NULL;
		aryDspData[i].pVoipDevice=NULL;
	}

	/*ȫ�ֵ�ǰ�豸�����ʼ��*/
	mPubDeviceNo=-1;	
	mPubVoiceNo=-1;
	mPubVoipNo=-1;
	mPubTrunkNo=-1;
	/*DSPȫ�ֵ�ǰ��ų�ʼ��*/
	mPubDspNo=-1;	
	/*��Ч��DSP������ʼ��*/
	mDspCount=-1;
	mOpenDspCount=-1;
	/*�����豸������ʼ��*/
	mVoiceCount=-1;
	mOpenVoiceCount=-1;
	/*�ӿ��豸������ʼ��*/
	mTrunkCount=-1;
	mOpenTrunkCount=-1;
	/*VOIP�豸������ʼ��*/
	mVoipCount=-1;
	mOpenVoipCount=-1;
	pRowNum=-1;
	/*��������ȫ�ֱ��*/
	mPlayTaskID=-1;
	return true;
}


/***********************************************************************
���ܣ����������豸״̬����ȡ��Ӧ�������ַ���
***********************************************************************/
char *	GetVoiceStatusString(VOICE_STATUS mStatus)
{
	switch(mStatus)
	{
	case	VOC_WAIT_OPEN:		
		{
			return "�ȴ���";
		}
	case	VOC_WAIT_WORK:
		{
			return "�ȴ���ʼ��";
		}
	case	VOC_FREE:								
		{
			return "����";
		}
	case	VOC_OFFHOOK:							
		{
			return "ժ��";
		}
	case	VOC_WAITHANGUP:
		{
			return "�ȴ��һ�";
		}
	case	VOC_WAIT_DIAL:							
		{
			return "�ȴ�����";
		}
	case	VOC_DTMF_INPUT:							
		{
			return "�����պ�";
		}
	case	VOC_CALL_OUT:							
		{
			return "������";
		}
	case	VOC_CALL_OUT_FAILE:						
		{
			return "����ʧ��";
		}
	case	VOC_CALL_OUT_SUCCESS:					
		{
			return "���гɹ�";
		}
	case	VOC_CONNECTED:							
		{
			return "ͨ����";
		}
	case	VOC_CONNECT_DTMF:						
		{
			return "ͨ�����û�����";
		}
	case	VOC_ONHOOK:								
		{
			return "�һ�";
		}
	case	VOC_FLASH:								
		{
			return "�Ĳ��";
		}
	case	VOC_FLASH_WAIT_DIAL:					
		{
			return "�Ĳ�ɵȴ�����";
		}
	case	VOC_FLASH_CONNECT:						
		{
			return "�Ĳ��������";
		}
	case	VOC_CALL_IN:							
		{
			return "�е绰����";
		}
	case	VOC_RINGING:							
		{
			return "������";
		}
	case	VOC_RING_TIMEOUT:						
		{
			return "���峬ʱ";
		}
	case	VOC_FETCH_DIAL:							
		{
			return "���Ӳ���";
		}
	case	VOC_FETCH_CONNECT:						
		{
			return "��������";
		}
	case	VOC_SYS_USE:
		{
			return "ϵͳ����";
		}
	case	VOC_ERROR_CALL_LONG:
		{
			return "������";
		}
	case	VOC_PLAY_NUMBER:
		{
			return	"���ű�������";
		}
	case	VOC_PLAY_NOTICE:
		{
			return	"����������ʾ";
		}
	case	VOC_PLAY_DIAL_WRONG:
		{
			return	"����������ʾ";
		}
	case	VOC_PLAY_WELCOME:
		{
			return	"���Ż�ӭ����";
		}
	case	VOC_ANSWER_CALL:
		{
			return	"Ӧ��绰";
		}
	case	VOC_PLAY_VOIP_ERROR:
		{
			return	"����VOIP������";
		}
	case	VOC_HUIXIAN_TURN:
		{
			return	"����ͨ����";
		}
	case	VOC_WAIT_MAIN:
		{
			return	"�ȴ�����ͨ����";
		}
	case	VOC_WAIT_FIRST_DIAL_TONE:
		{
			return "�ȴ����߾��Ͳ�����";
		}
	case	VOC_WAIT_SECOND_DIAL_TONE:
		{
			return "�ȴ��ַ��Ͳ�����";
		}
	}
	return "������";
}

/***********************************************************************
����:����VOIP�豸״̬����ȡ��Ӧ�������ַ���
***********************************************************************/
char *	GetVoipStatusString(VOIP_STATUS mStatus)
{
	switch(mStatus)
	{
	case	VOIP_WAIT_OPEN:							/*�ȴ���*/
		{
			return	"�ȴ���";
		}
	case	VOIP_WAIT_WORK:							/*�ȴ���ʼ��*/
		{
			return "�ȴ���ʼ��";
		}
	case	VOIP_FREE:								/*����*/
		{
			return "����";
		}
	case	VOIP_CALL_IN:							/*�е绰����*/
		{
			return "�е绰����";
		}
	case	VOIP_WAIT_CONNECT:						/*����������*/	
		{
			return "����������";
		}
	case	VOIP_CALL_OUT_FAILE:					/*���ߺ���ʧ��*/
		{
			return "���ߺ���ʧ��";
		}
	case	VOIP_CALL_OUT_SUCCESS:					/*���ߺ��гɹ�*/
		{
			return "���ߺ��гɹ�";
		}
	case	VOIP_CONNECTED:							/*ͨ����*/	
		{
			return "ͨ����";
		}
	case	VOIP_CONNECT_DTMF:						/*ͨ�����û�����*/
		{
			return "ͨ�����û�����";
		}
	case	VOIP_ONHOOK:							/*�һ�*/
		{
			return "�һ�";
		}
	case	VOIP_CALL_VOIP:							/*�е绰����*/
		{
			return "�����绰";
		}
	case	VOIP_CALL_VOIP_FAILE:					/*��������ʧ��*/
		{
			return "��������ʧ��";
		}
	case	VOIP_CALL_VOIP_SUCCESS:					/*�������߳ɹ�*/
		{
			return "�������߳ɹ�";
		}
	case	VOIP_PLAY_FILE:
		{
			return	"����";
		}
	case	VOIP_WAITHUNGUP:
		{
			return	"�ȴ��һ�";
		}
	}
	return "������";
}


/***********************************************************************
���ܣ���ʼ��һ���б�ؼ���ֻ�ǳ�ʼ���б��⡣���������
***********************************************************************/
void	InitStatusControl(CListCtrl *pControl)				
{
	char	pTitle[11][20];		
	int		pWidth[11];
	int		i;

	strcpy(pTitle[0],"ȫ���豸��");
	strcpy(pTitle[1],"�豸״̬");
	strcpy(pTitle[2],"��·״̬");
	strcpy(pTitle[3],"DSP���");
	strcpy(pTitle[4],"ͨ����");
	strcpy(pTitle[5],"�豸����");
	strcpy(pTitle[6],"�������");
	strcpy(pTitle[7],"���к���");
	strcpy(pTitle[8],"���к���");
	strcpy(pTitle[9],"DTMF��");
	strcpy(pTitle[10],"����");
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
���ܣ���ʾ���е��豸״̬
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

	/*��ʾ���еĽӿ��豸״̬*/
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
			pControl->SetItemText(mRow,5,"����");
		if(aryDspData[mDspNo].pTrunkDevice[mIndex].fLineType==LINE_OUT)
			pControl->SetItemText(mRow,5,"����");
		if(aryDspData[mDspNo].pTrunkDevice[mIndex].fLineType==LINE_NULL)
			pControl->SetItemText(mRow,5,"����");

		pControl->SetItemText(mRow,6,aryDspData[mDspNo].pTrunkDevice[mIndex].fLocalNumber);
		pControl->SetItemText(mRow,7,aryDspData[mDspNo].pTrunkDevice[mIndex].fCallOutNumber);
		pControl->SetItemText(mRow,8,aryDspData[mDspNo].pTrunkDevice[mIndex].fCallInNumber);
		pControl->SetItemText(mRow,9,aryDspData[mDspNo].pTrunkDevice[mIndex].fDtmfBuff);
		aryTrunkIndex[i].fRowNo=mRow;
		DisplayStatusBar();
		mRow=mRow+1;
	}
	/*��ʾ���������豸��״̬*/
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
		pControl->SetItemText(mRow,5,"�����豸");
		pControl->SetItemText(mRow,6,aryDspData[mDspNo].pVoiceDevice[mIndex].fLocalNumber);
		pControl->SetItemText(mRow,7,aryDspData[mDspNo].pVoiceDevice[mIndex].fCallOutNumber);
		pControl->SetItemText(mRow,8,aryDspData[mDspNo].pVoiceDevice[mIndex].fCallInNumber);
		pControl->SetItemText(mRow,9,aryDspData[mDspNo].pVoiceDevice[mIndex].fDtmfBuff);
		aryVoiceIndex[i].fRowNo=mRow;
		DisplayStatusBar();
		mRow=mRow+1;
	}
	/*��ʾ����VOIP��״̬*/
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
		pControl->SetItemText(mRow,5,"VOIP�豸");
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
���ܣ�XMS�¼��ص���������ϵͳ������Դͷ
***********************************************************************/
DJ_Void XMSEventHandler(DJ_U32 esrParam)
{
	Acs_Evt_t		*pEvt=NULL;
	Acs_Dev_List_Head_t		*pHeadData=NULL;
	DJ_S32					mDspID;					/*DSP���*/
	DJ_S32					mDeviceNumber;			/*���λ�ȡ���豸����*/

	pEvt=(Acs_Evt_t *)esrParam;
	/*char	mTemp[100];
	itoa(pEvt->m_s32EventType,mTemp,10);
	strcat(mTemp," Event");
	AddMsg(mTemp);
	*/
	switch(pEvt->m_s32EventType)
	{
	case XMS_EVT_QUERY_DEVICE:							/*��ȡ�豸�б��¼�*/
		{
			//DisplayStatusBar("���ڻ�ȡ�豸�б�....");
			DisplayEvt(pEvt);
			if(fIsRun)	return;
			pHeadData=(Acs_Dev_List_Head_t *)FetchEventData(pEvt);
			mDspID=(DJ_S32)pHeadData->m_s32ModuleID;			/*��ȡDSP���*/
			mDeviceNumber=pHeadData->m_s32DeviceNum;		/*��ȡ�豸����*/
			if(mDspID<0 || mDspID>MAX_DSP_COUNT)	return;	/*���ڳ������DSP������DSP������*/
			/*������ֻ����VOICE�豸��VOIP�豸��Interface�豸*/
			if(pHeadData->m_s32DeviceMain==XMS_DEVMAIN_VOICE || pHeadData->m_s32DeviceMain==XMS_DEVMAIN_INTERFACE_CH
				|| pHeadData->m_s32DeviceMain==XMS_DEVMAIN_VOIP || pHeadData->m_s32DeviceMain==XMS_DEVMAIN_BOARD)
			{
				if(!InsertDeviceToUse(mDspID,pHeadData))
				{
					MessageBox(NULL,"ϵͳ��ȡ�豸�б�ʧ��!!","Error",MB_OK);
					CloseXMSConnect();
					PostQuitMessage(0);
					return;
				}
			}
		}
		break;
	case XMS_EVT_QUERY_ONE_DSP_END:
		{
			//DisplayStatusBar("׼����ȡ��һ��DSP�豸�б�.....");
		}
		break;
	case XMS_EVT_QUERY_DEVICE_END:
		/*�������豸*/
		//DisplayStatusBar("�豸�б��ȡ���!��ʼ���г�ʼ��...........");
		DisplayEvt(pEvt);
		if(fIsRun)	return;
		DrawAllItem();
		if(!OpenAllDevice())
		{
			MessageBox(NULL,"ϵͳ���豸ʧ��!!","Error",MB_OK);
			return;
		}
		break;
	case XMS_EVT_OPEN_DEVICE:
		/*���豸�Ѿ���*/
		//DisplayStatusBar("���ڴ��豸.......");
		pHeadData=(Acs_Dev_List_Head_t *)FetchEventData(pEvt);
		mDspID=pEvt->m_DeviceID.m_s8ModuleID;	/*�ر�ע�⣺Ӧ���ô˲���*/
		if(fIsRun)	return;
		/*�Եײ㷵�صĴ򿪵����ݽ��г�ʼ��*/
		if(!OpenDeviceSuccess(mDspID,&pEvt->m_DeviceID))
		{
			MessageBox(NULL,"���豸ʧ��!!","Error",MB_OK);
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
		/*��̬��ʾ�ײ㷵�ص��豸״̬*/
		{
			Acs_GeneralProc_Data *pGeneralData = NULL;
			pGeneralData=(Acs_GeneralProc_Data *)FetchEventData(pEvt);
			ProcessDeviceStatusEvt(&pEvt->m_DeviceID,pGeneralData->m_s32DeviceState);
		}
		break;
	case XMS_EVT_LINKDEVICE:
		/*�����ӽ���*/
		{
			Acs_CallControl_Data	*pContrlData;
			pContrlData=(Acs_CallControl_Data *)FetchEventData(pEvt);
/*			if(pContrlData->m_s32AcsEvtState==1)
				AddMsg("�������ӳɹ�");
			else
				AddMsg("��������ʧ��");
*/
		}
		break;
	case	XMS_EVT_SETPARAM:
		{
			Acs_ParamProc_Data	*pData=NULL;
			pData=(Acs_ParamProc_Data *)FetchEventData(pEvt);
			if(pData->m_s32AcsEvtState!=1)
				AddMsg("����ʧ��");
			else
				AddMsg("���óɹ�");
			char	mTemp[20];
			itoa(pEvt->m_DeviceID.m_s16ChannelID,mTemp,10);
			AddMsg(mTemp);
		}
		break;
	default:
		if(pEvt->m_DeviceID.m_s16DeviceMain==XMS_DEVMAIN_INTERFACE_CH)
		{
			/*�ӿ�ͨ���¼��Ĵ���*/
			if(pEvt->m_DeviceID.m_s16DeviceSub==XMS_DEVSUB_ANALOG_TRUNK)
			{
				/*�����¼�����*/
				TrunkDeviceWork(&pEvt->m_DeviceID,pEvt);
			}
			if(pEvt->m_DeviceID.m_s16DeviceSub==XMS_DEVSUB_ANALOG_USER)
			{
				/*�����¼�����*/
				UserDeviceWork(&pEvt->m_DeviceID,pEvt);
			}
		}
		if(pEvt->m_DeviceID.m_s16DeviceMain==XMS_DEVMAIN_VOICE)
		{
			/*����ͨ���¼��Ĵ���*/
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
				/*�����¼�����*/
				TrunkDeviceWork(pDevice,pEvt);
			}
			if(pDevice->m_s16DeviceSub==XMS_DEVSUB_ANALOG_USER)
			{
				/*�����¼�����*/
				UserDeviceWork(pDevice,pEvt);
			}
		}
		if(pEvt->m_DeviceID.m_s16DeviceMain==XMS_DEVMAIN_VOIP)
		{
			/*VOIPͨ���¼��Ĵ���*/
			VoipDeviceWork(&pEvt->m_DeviceID,pEvt);

		}
	}
}


/***********************************************************************
���ܣ���һ���豸��ӵ����õ��豸�����У��˺�����XMS_EVT_QUEYR_DEVICE�¼��е���
ע�⣺��������ͳһ����VOICE�豸��INTERFACE�豸��VOIP�豸���������������£�
1.�жϸ�DSP�Ƿ��ʼ���������û�У��򽫸�DSP��Ϣд������
2.�����ǰ�豸�������豸������������豸����
3.�����ǰ�豸�ǽӿ��豸�������ӿ��豸����
4.�����ǰ�豸��VOIP�豸�������VOIP�豸����
***********************************************************************/
bool	InsertDeviceToUse(DJ_S32	mDspID,Acs_Dev_List_Head_t *pDevList)
{
	DJ_S32		mDeviceNum;
	int			i;
	long		mDspNo;
	long		mDeviceNo;				/*��ϵͳ�õ�ȫ���豸���*/
	DeviceID_t	 *pDev;
	
	/*�жϸ�DSP�Ƿ��Ѿ���ʼ���������û�У����ȳ�ʼ��DSP��Ϣ*/
	if(pDevList->m_s32DeviceMain==XMS_DEVMAIN_BOARD)
	{
		/*��DSP��û�г�ʼ��*/
		mDspNo=mDspID;							/*DSPID��ΪDSPNO����*/
		pDev=(DeviceID_t *)((BYTE *)pDevList + sizeof(Acs_Dev_List_Head_t));
		aryDspData[mDspID].fDeviceID=pDev[0];
		aryDspData[mDspID].fIsExits=1;
		aryDspData[mDspID].fDspID=mDspID;
		mDspCount=mDspCount+1;							/*�ۼ���Ч��DSP����*/
	}

	mDspNo=mDspID;										/*��DSPNO����ΪDSPID*/
	if(mDspNo<0)										
	{
		MessageBox(NULL,"ϵͳ�ڲ�����!!","Error",MB_OK);
		return false;
	}
	/*������ص��������豸����������豸���г�ʼ��*/
	if(pDevList->m_s32DeviceMain==XMS_DEVMAIN_VOICE)
	{
		mDeviceNum=pDevList->m_s32DeviceNum;
		if(mDeviceNum>0)
		{
			aryDspData[mDspNo].fVoiceCount=aryDspData[mDspNo].fVoiceCount+mDeviceNum;
			aryDspData[mDspNo].pVoiceDevice=new TYPE_VOICE_DATA[mDeviceNum];			//�����ڴ�
			if(!aryDspData[mDspNo].pVoiceDevice)
			{
				MessageBox(NULL,"ϵͳΪ�����豸�����ڴ�ʧ��!!","Error",MB_OK);
				return false;
			}
			memset(aryDspData[mDspNo].pVoiceDevice,0,sizeof(TYPE_VOICE_DATA)*mDeviceNum);
			DeviceID_t		*pDeviceID;
			/*�ر�ע�⣺pDeviceID�ײ㷵�ص������е��豸ID��ָ��*/
			pDeviceID=(DeviceID_t *)((BYTE *)pDevList+sizeof(Acs_Dev_List_Head_t));	
			mVoiceCount=mVoiceCount+mDeviceNum;						/*�ۼ������豸����*/

			for(i=0;i<mDeviceNum;i++)
			{
				mDeviceNo=getVoiceDeviceNo();						/*ר�ŵ������豸ȫ�ֱ��*/
				aryDspData[mDspNo].pVoiceDevice[i].fChannelNo=mDeviceNo;
				aryDspData[mDspNo].pVoiceDevice[i].fDeviceID=pDeviceID[i];
				aryDspData[mDspNo].pVoiceDevice[i].fDeviceType=DEVICE_VOICE;
				aryDspData[mDspNo].pVoiceDevice[i].fStatusCode=VOC_WAIT_OPEN;
				aryDspData[mDspNo].pVoiceDevice[i].fIsExit=1;		/*��ʾ���豸����*/
				aryDspData[mDspNo].pVoiceDevice[i].fIsOpen=false;	/*��ʾδ��*/
				aryDspData[mDspNo].pVoiceDevice[i].fLinkTrunkChannelNo=-1;
				aryDspData[mDspNo].pVoiceDevice[i].fIsLinkTrunk=false;
				aryDspData[mDspNo].pVoiceDevice[i].fLinkVoipChannelNo=-1;
				aryDspData[mDspNo].pVoiceDevice[i].fIsLinkVoip=false;
				aryDspData[mDspNo].pVoiceDevice[i].fLineState=-1;


				/*ȫ����������Ľ���*/
				aryVoiceIndex[mDeviceNo].fChannelID_t=pDeviceID[i].m_s16ChannelID;	/*����ϵͳ�ڲ�ͨ����*/
				aryVoiceIndex[mDeviceNo].fChannelNo=mDeviceNo;	/*ȫ���豸���*/	
				aryVoiceIndex[mDeviceNo].fIndex=i;				/*���������±�*/
				aryVoiceIndex[mDeviceNo].fDspNo=mDspNo;
				aryVoiceIndex[mDeviceNo].fDeviceType=DEVICE_VOICE;
			}
		}		
	}
	/*������ص��ǽӿ��豸����Խӿ��豸���г�ʼ��*/
	if(pDevList->m_s32DeviceMain==XMS_DEVMAIN_INTERFACE_CH)
	{
		mDeviceNum=pDevList->m_s32DeviceNum;
		if(mDeviceNum>0)
		{
			aryDspData[mDspNo].fTrunkCount=aryDspData[mDspNo].fTrunkCount+mDeviceNum;
			aryDspData[mDspNo].pTrunkDevice=new TYPE_INTERFACE_DATA[mDeviceNum];
			if(!aryDspData[mDspNo].pTrunkDevice)
			{
				MessageBox(NULL,"ϵͳΪ�ӿ��豸�����ڴ�ʧ��!!","Error",MB_OK);
				return false;
			}
			memset(aryDspData[mDspNo].pTrunkDevice,0,sizeof(TYPE_INTERFACE_DATA)*mDeviceNum);
			DeviceID_t		*pDeviceID;
			pDeviceID=(DeviceID_t *)((BYTE *)pDevList+sizeof(Acs_Dev_List_Head_t));
			mTrunkCount=mTrunkCount+mDeviceNum;						/*�ӿ��豸�����ۼ�*/
			for(i=0;i<mDeviceNum;i++)
			{
				/*����������ȡ*/
				aryDspData[mDspNo].pTrunkDevice[i].fDeviceID=pDeviceID[i];
				aryDspData[mDspNo].pTrunkDevice[i].fDeviceType=DEVICE_TRUNK;
				aryDspData[mDspNo].pTrunkDevice[i].fStatusCode=VOC_WAIT_OPEN;
				if(aryDspData[mDspNo].pTrunkDevice[i].fDeviceID.m_s16DeviceSub==XMS_DEVSUB_ANALOG_TRUNK || aryDspData[mDspNo].pTrunkDevice[i].fDeviceID.m_s16DeviceSub==XMS_DEVSUB_ANALOG_USER)
				{
					aryDspData[mDspNo].pTrunkDevice[i].fIsExit=1;		/*��ʾ���豸����*/		
					aryDspData[mDspNo].pTrunkDevice[i].fIsOpen=false;	/*��ʾδ��*/	
					if(pDeviceID[i].m_s16DeviceSub==XMS_DEVSUB_ANALOG_USER) aryDspData[mDspNo].pTrunkDevice[i].fLineType=LINE_IN;
					if(pDeviceID[i].m_s16DeviceSub==XMS_DEVSUB_ANALOG_TRUNK) aryDspData[mDspNo].pTrunkDevice[i].fLineType=LINE_OUT;
				}
				else
				{
					aryDspData[mDspNo].pTrunkDevice[i].fIsExit=0;		/*��ʾ���豸û�д���*/
					aryDspData[mDspNo].pTrunkDevice[i].fIsOpen=false;	/*��ʾδ��*/	
					aryDspData[mDspNo].pTrunkDevice[i].fLineType=LINE_NULL;	/*��ʾ����*/

				}
				aryDspData[mDspNo].pTrunkDevice[i].fLineState=-1;			/*��ʾ��·���ڵȴ�����*/
				aryDspData[mDspNo].pTrunkDevice[i].fLinkVoiceChannelNo=-1;	/*��ʾû�������κ��豸*/	
				aryDspData[mDspNo].pTrunkDevice[i].fIsLinkVoice=false;
				aryDspData[mDspNo].pTrunkDevice[i].fLinkTrunkChannelNo=-1;
				aryDspData[mDspNo].pTrunkDevice[i].fIsLinkTrunk=false;
				aryDspData[mDspNo].pTrunkDevice[i].fisLinkVoipVoice=false;
				aryDspData[mDspNo].pTrunkDevice[i].fLinkVoipVoiceDeviceNo=-1;
				/*�Ĳ�����ݳ�ʼ��*/
				aryDspData[mDspNo].pTrunkDevice[i].fPrevDeviceType=-1;
				aryDspData[mDspNo].pTrunkDevice[i].fPrevTrunkChannelNo=-1;
				aryDspData[mDspNo].pTrunkDevice[i].fPrevVoipChannelNo=-1;
				aryDspData[mDspNo].pTrunkDevice[i].fChaHuanCount=0;
				aryDspData[mDspNo].pTrunkDevice[i].fIsChaHuan=false;
				aryDspData[mDspNo].pTrunkDevice[i].fChaHuanIsCallStatus=false;
				aryDspData[mDspNo].pTrunkDevice[i].fChaHuanIsReset=false;
				aryDspData[mDspNo].pTrunkDevice[i].fChaHuanLinkChannenlNo=-1;
				strcpy(aryDspData[mDspNo].pTrunkDevice[i].fSpeedDial,"");		//��λ����
				strcpy(aryDspData[mDspNo].pTrunkDevice[i].fTargetUserNo,"");

				/*����ײ㱣����ͨ��*/
				if(i==0)	aryDspData[mDspNo].pTrunkDevice[i].fStatusCode=VOC_SYS_USE;
				if((i % 16)==0)aryDspData[mDspNo].pTrunkDevice[i].fStatusCode=VOC_SYS_USE;
				/*�жϸ��豸�Ƿ��Ѿ���ʼ��*/
				long	mTempDeviceNo;
				mTempDeviceNo=getDeviceNoByChannelID(mDspNo,pDeviceID[i].m_s16ChannelID);
				if(mTempDeviceNo==-1)	return false;
				if(mTempDeviceNo==-2)
					mDeviceNo=getTrunkDeviceNo();	//��ȡtrunkר�õ�ȫ���豸���
				else
				{
					mDeviceNo=mTempDeviceNo;
					mPubTrunkNo=mDeviceNo;
				}
				/*ȫ����������Ľ���*/
				aryDspData[mDspNo].pTrunkDevice[i].fChannelNo=mDeviceNo;
				aryTrunkIndex[mDeviceNo].fChannelID_t=pDeviceID[i].m_s16ChannelID;	/*����ϵͳ�ڲ�ͨ����*/
				aryTrunkIndex[mDeviceNo].fChannelNo=mDeviceNo;	/*ȫ���豸���*/	
				aryTrunkIndex[mDeviceNo].fIndex=i;				/*���������±�*/
				aryTrunkIndex[mDeviceNo].fDspNo=mDspNo;
				aryTrunkIndex[mDeviceNo].fDeviceType=DEVICE_TRUNK;

				/*�ж��Ƿ���Ҫ�������ݴ���*/
				if(mTempDeviceNo==-2)
				{
					if(InitLineDbRecord(mDeviceNo,mDspNo,&aryDspData[mDspNo].pTrunkDevice[i].fDeviceID)==false)	return false;
				}
				/*�����ߵĴ������ڴ��������ߣ��������ñ��������½���,��ֱ�֤������Ӳ����ͬ��*/
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
				MessageBox(NULL,"ϵͳΪVOIP�豸�����ڴ�ʧ��!!","Error",MB_OK);
				return false;
			}
			memset(aryDspData[mDspNo].pVoipDevice,0,sizeof(TYPE_VOIP_DATA)*mDeviceNum);
			DeviceID_t		*pDeviceID;
			pDeviceID=(DeviceID_t *)((BYTE *)pDevList+sizeof(Acs_Dev_List_Head_t));
			mVoipCount=mVoipCount+mDeviceNum;					/*VOIP�豸�����ۼ�*/
			for(i=0;i<mDeviceNum;i++)
			{
				mDeviceNo=getVoipDeviceNo();					/*��ȡȫ�ֵ�voip�豸���*/
				aryDspData[mDspNo].pVoipDevice[i].fChannelNo=mDeviceNo;
				aryDspData[mDspNo].pVoipDevice[i].fDeviceID=pDeviceID[i];
				aryDspData[mDspNo].pVoipDevice[i].fDeviceType=DEVICE_VOIP;
				aryDspData[mDspNo].pVoipDevice[i].fStatusCode=VOIP_WAIT_OPEN;
				aryDspData[mDspNo].pVoipDevice[i].fIsExit=1;	/*��ʾ���豸����*/		
				aryDspData[mDspNo].pVoipDevice[i].fIsOpen=false;	/*��ʾδ��*/	
				aryDspData[mDspNo].pVoipDevice[i].fLinkVoiceChannelNo=-1;	/*��ʾû�������κ��豸*/	
				aryDspData[mDspNo].pVoipDevice[i].fIsLinkVoice=false;

				/*��д���룬����ʵ��ip����������*/
				aryDspData[mDspNo].pVoipDevice[i].fLinkTrunkChannelNo=-1;
				aryDspData[mDspNo].pVoipDevice[i].fLinkTrunkLineType=LINE_NULL;		//����
				aryDspData[mDspNo].pVoipDevice[i].fIsLinkTrunk=false;


				/*ȫ����������Ľ���*/
				aryVoipIndex[mDeviceNo].fChannelID_t=pDeviceID[i].m_s16ChannelID;	/*����ϵͳ�ڲ�ͨ����*/
				aryVoipIndex[mDeviceNo].fChannelNo=mDeviceNo;	/*ȫ���豸���*/	
				aryVoipIndex[mDeviceNo].fIndex=i;				/*���������±�*/
				aryVoipIndex[mDeviceNo].fDspNo=mDspNo;
				aryVoipIndex[mDeviceNo].fDeviceType=DEVICE_VOIP;

				/*��VOIP���ݽ��г�ʼ��*/
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
���ܣ���ʱ���ص�����������ɨ�����ͨ����dtmf��������״��
***********************************************************************/
VOID CALLBACK TimerProc(HWND hwnd,UINT uMsg,UINT idEvent,DWORD dwTime)
{
}

/***********************************************************************
���ܣ���ȡ�������豸�б�
***********************************************************************/
bool	getXMSDeviceList()
{
	RetCode_t	rtCode;
	rtCode=XMS_acsGetDeviceList(pFlowHandle,NULL);
	if(rtCode==-ACSERR_BADHDL || rtCode==-ACSERR_FAILSEND)
	{
		MessageBox(NULL,"ϵͳ���û�ȡ�豸�б���ʧ��!!","ERROR",MB_OK);
		return false;
	}
	return true;
}

/***********************************************************************
���ܣ��Ͽ���XMS���������
***********************************************************************/
void	CloseXMSConnect()
{
	XMS_acsCloseStream(pFlowHandle,NULL);
}


/***********************************************************************
���ܣ���ȡһ��ȫ�ֵ��豸����
***********************************************************************/
long	getDeviceNo()
{
	mPubDeviceNo=mPubDeviceNo+1;
	return mPubDeviceNo;
}

/***********************************************************************
���ܣ��жϸ�DSP�Ƿ��Ѿ���ʼ����������Ѿ���ʼ�����򷵻�True�����򷵻�false
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
���ܣ�����XMS SDK�е�OpenDevice�������豸��ע���������ת��ϵͳ���ص��¼��н���
***********************************************************************/
bool	OpenAllDevice()
{
	int		i;
	int		j;	
	RetCode_t		rtCode;
	
	/*�ȴ����е�DSP*/
	for(i=0;i<=MAX_DSP_COUNT;i++)
	{
		/*��������ڣ�����Ҫ���д�*/
		if(aryDspData[i].fIsExits==1)		
		{
			rtCode=XMS_ctsOpenDevice(pFlowHandle,&aryDspData[i].fDeviceID,NULL);
			if(rtCode==-ACSERR_BADHDL)
			{
				MessageBox(NULL,"����OpenDeviceʱ,���̾���Ƿ�!!","Error",MB_OK);
				return false;
			}
			if(rtCode==-ACSERR_FAILSEND)
			{
				MessageBox(NULL,"����OpenDeviceʱ,��������ʧ��!!�������̷�������������������!!","Error",MB_OK);
				return false;
			}
			if(rtCode==-ACSERR_BADPARAMETER)
			{
				MessageBox(NULL,"����OpenDeviceʱ,�豸���Ϊ��!!","Error",MB_OK);
				return false;
			}
			/*�������豸*/
			for(j=0;j<=aryDspData[i].fVoiceCount;j++)
			{
				rtCode=XMS_ctsOpenDevice(pFlowHandle,&aryDspData[i].pVoiceDevice[j].fDeviceID,NULL);
				if(rtCode==-ACSERR_BADHDL)
				{
					MessageBox(NULL,"����OpenDeviceʱ,���̾���Ƿ�!!","Error",MB_OK);
					return false;
				}
				if(rtCode==-ACSERR_FAILSEND)
				{
					MessageBox(NULL,"����OpenDeviceʱ,��������ʧ��!!�������̷�������������������!!","Error",MB_OK);
					return false;
				}
				if(rtCode==-ACSERR_BADPARAMETER)
				{
					MessageBox(NULL,"����OpenDeviceʱ,�豸���Ϊ��!!","Error",MB_OK);
					return false;
				}
			}
			/*�򿪽ӿ��豸*/
			for(j=0;j<=aryDspData[i].fTrunkCount;j++)
			{
				rtCode=XMS_ctsOpenDevice(pFlowHandle,&aryDspData[i].pTrunkDevice[j].fDeviceID,NULL);
				if(rtCode==-ACSERR_BADHDL)
				{
					MessageBox(NULL,"����OpenDeviceʱ,���̾���Ƿ�!!","Error",MB_OK);
					return false;
				}
				if(rtCode==-ACSERR_FAILSEND)
				{
					MessageBox(NULL,"����OpenDeviceʱ,��������ʧ��!!�������̷�������������������!!","Error",MB_OK);
					return false;
				}
				if(rtCode==-ACSERR_BADPARAMETER)
				{
					MessageBox(NULL,"����OpenDeviceʱ,�豸���Ϊ��!!","Error",MB_OK);
					return false;
				}
			}
			/*��VOIP�豸*/
			for(j=0;j<=aryDspData[i].fVoipCount;j++)
			{
				rtCode=XMS_ctsOpenDevice(pFlowHandle,&aryDspData[i].pVoipDevice[j].fDeviceID,NULL);
				if(rtCode==-ACSERR_BADHDL)
				{
					MessageBox(NULL,"����OpenDeviceʱ,���̾���Ƿ�!!","Error",MB_OK);
					return false;
				}
				if(rtCode==-ACSERR_FAILSEND)
				{
					MessageBox(NULL,"����OpenDeviceʱ,��������ʧ��!!�������̷�������������������!!","Error",MB_OK);
					return false;
				}
				if(rtCode==-ACSERR_BADPARAMETER)
				{
					MessageBox(NULL,"����OpenDeviceʱ,�豸���Ϊ��!!","Error",MB_OK);
					return false;
				}
			}
		}
	}
	return true;
}


/***********************************************************************
���ܣ����豸�ɹ�ʱ����
***********************************************************************/
bool	OpenDeviceSuccess(DJ_S32 mDspID,DeviceID_t	*pDeviceID)
{
	/*�����DSP*/
	long	mDspNo;					/*�±�*/
	long	mDeviceIndex;	

	if(pDeviceID->m_s16DeviceMain==XMS_DEVMAIN_BOARD)		
	{
			mDspNo=pDeviceID->m_s8ModuleID;									/*����DSPid��ȡ����ȫ�ֱ��*/
			aryDspData[mDspNo].fDeviceID.m_CallID=pDeviceID->m_CallID;		/*�ײ��������ã�����ȱ��*/
			aryDspData[mDspNo].fIsOpen=true;									
			aryDspData[mDspNo].fDspID=mDspID;
			aryDspData[mDspNo].fDspNo=mDspNo;
			mOpenDspCount=mOpenDspCount+1;									/*�ۼ�ȫ�ִ򿪵�DSP����*/
	}
	/*���������ͨ��*/
	if(pDeviceID->m_s16DeviceMain==XMS_DEVMAIN_VOICE)
	{
			long		mVoiceIndex;
			mDspNo=pDeviceID->m_s8ModuleID;
			mVoiceIndex=FindArrayIndexByChannelID(mDspNo,pDeviceID->m_s16ChannelID,DEVICE_VOICE);
			mDeviceIndex=aryVoiceIndex[mVoiceIndex].fIndex;
			if(mVoiceIndex!=-1)
			{
					aryDspData[mDspNo].pVoiceDevice[mDeviceIndex].fDeviceID.m_CallID=pDeviceID->m_CallID;	/*�ײ��������ã�����ȱ��*/
					aryDspData[mDspNo].pVoiceDevice[mDeviceIndex].fIsOpen=true;
					mOpenVoiceCount=mOpenVoiceCount+1;
					aryDspData[mDspNo].pVoiceDevice[mDeviceIndex].fStatusCode=VOC_WAIT_WORK;
			}
			DrawOneItem(mDspNo,aryVoiceIndex[mVoiceIndex].fChannelID_t,DEVICE_VOICE);
			ResetChannel(mDspNo,pDeviceID);
	}
	/*����ǽӿ��豸*/
	if(pDeviceID->m_s16DeviceMain==XMS_DEVMAIN_INTERFACE_CH)
	{
			long		mTrunkIndex;
			mDspNo=pDeviceID->m_s8ModuleID;
			mTrunkIndex=FindArrayIndexByChannelID(mDspNo,pDeviceID->m_s16ChannelID,DEVICE_TRUNK);
			if(mTrunkIndex!=-1)
			{
				mDeviceIndex=aryTrunkIndex[mTrunkIndex].fIndex;
				aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID.m_CallID=pDeviceID->m_CallID;	/*�ײ��������ã�����ȱ��*/
				aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsOpen=true;
				aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode=VOC_WAIT_WORK;
				mOpenTrunkCount=mOpenTrunkCount+1;
				/*���������͵��ж�*/
				if(pDeviceID->m_s16DeviceSub==XMS_DEVSUB_ANALOG_USER)aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLineType=LINE_IN;
				/*���������͵��ж�*/
				if(pDeviceID->m_s16DeviceSub==XMS_DEVSUB_ANALOG_TRUNK)aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLineType=LINE_OUT;
				DrawOneItem(mDspNo,aryTrunkIndex[mTrunkIndex].fChannelID_t,DEVICE_TRUNK);
				ResetChannel(mDspNo,pDeviceID);
			}
	}
	/*�����VOIP�豸*/
	if(pDeviceID->m_s16DeviceMain==XMS_DEVMAIN_VOIP)
	{
		long		mVoipIndex;
		mDspNo=pDeviceID->m_s8ModuleID;
		mVoipIndex=FindArrayIndexByChannelID(mDspNo,pDeviceID->m_s16ChannelID,DEVICE_VOIP);
		if(mVoipIndex!=-1)
		{
			mDeviceIndex=aryVoipIndex[mVoipIndex].fIndex;
			/*�ײ��������ã�����ȱ��*/
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
���ܣ�ͨ���豸�����±���ң�����dspid��channelid_t�Լ��豸���ͣ������豸ȫ������������в���
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
���ܣ�ͨ���򿪺��ʼ����ͬʱ����ϵͳ���ݽ��г�ʼ��
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
���ܣ�ʵ�ֶ������豸��ͨ���豸�Լ�VOIP�豸����Ĭ�ϰ�
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
		if(strcmp(mDspType,"INTERFACE")==0)				//���
		{
			for(j=0;j<=aryDspData[i].fTrunkCount;j++)
			{
				/*Ϊ�����豸�ͽӿ��豸����˫���ʱ϶����*/
				if(aryDspData[i].pTrunkDevice[j].fIsOpen==true)		/*���豸������*/
				{
					mVoiceIndex=FindFreeVoiceDevice(aryDspData[i].fDspNo);
					//mVoiceIndex=j;		/*�̶�Ϊ����index*/
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
							MessageBox(NULL,"ϵͳ�ײ����!����Ӳ���豸�Ƿ�����!!","Error",MB_OK);
							return false;
						}
						rtCode=XMS_ctsLinkDevice(pFlowHandle,&aryDspData[i].pVoiceDevice[mVoiceIndex].fDeviceID,&aryDspData[i].pTrunkDevice[j].fDeviceID,NULL);
						if(rtCode<0)
						{
							MessageBox(NULL,"ϵͳ�ײ����!����Ӳ���豸�Ƿ�����!!","Error",MB_OK);
							return false;
						}
						
						/*������ɺ󣬿�ʼ�������ݵ�ͬ��*/
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

						aryDspData[i].pTrunkDevice[j].fLinkVoipDeviceNo=-1;		/*�����ӵĴ��룬ר�����ڴ���IP�Ĳ���*/
						
						strcpy(aryDspData[i].pVoiceDevice[mVoiceIndex].fCallInNumber,"");
						strcpy(aryDspData[i].pVoiceDevice[mVoiceIndex].fCallOutNumber,"");
						strcpy(aryDspData[i].pTrunkDevice[j].fCallInNumber,"");
						strcpy(aryDspData[i].pTrunkDevice[j].fCallOutNumber,"");
						strcpy(aryDspData[i].pTrunkDevice[j].fDescription,"");
						
						/*�������ߣ����ó�ʼ����������*/
						if(aryDspData[i].pTrunkDevice[j].fDeviceID.m_s16DeviceSub==XMS_DEVSUB_ANALOG_TRUNK)
						{
							if(!SetTrunkLineInitParameter(&aryDspData[i].pTrunkDevice[j].fDeviceID,&aryDspData[i].pVoiceDevice[mVoiceIndex].fDeviceID))
							{
								MessageBox(NULL,"ϵͳ�������߹�������ʧ��!!","Error",MB_OK);
								return false;
							}
						}
					}
				}
			}
		}
		if(strcmp(mDspType,"VOIP")==0)
		{
			if(aryDspData[i].fIsExits==1) pVoipDspNo=i;			/*��¼���õ�voip DSP*/
			if(aryDspData[i].fIsExits==1)
			for(j=0;j<=aryDspData[i].fVoipCount;j++)
			{
				/*���������豸��VOIP�豸֮���ʱ϶����*/
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
							MessageBox(NULL,"ϵͳ�ײ����!����Ӳ���豸�Ƿ�����!!","Error",MB_OK);
							return false;
						}
*/
						rtCode=XMS_ctsLinkDevice(pFlowHandle,&aryDspData[i].pVoipDevice[j].fDeviceID,&aryDspData[i].pVoiceDevice[mVoiceIndex].fDeviceID,NULL);
						if(rtCode<0)
						{
							MessageBox(NULL,"ϵͳ�ײ����!����Ӳ���豸�Ƿ�����!!","Error",MB_OK);
							return false;
						}
						/*������ɺ󣬿�ʼ��������ͬ��*/
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
	MessageBox(pMainDlg->m_hWnd,"��������ɹ�!!!","Info",MB_OK);
	DisplayStatusBar("          ϵͳ������.����������");
	fIsRun=true;
	return true;
}



/***********************************************************************
���ܣ���ͨ����Ϣд�����ݿ⣬���ڵ�һ�γ�ʼ��
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
		MessageBox(NULL,"ϵͳ�������ݿ�ʧ��!!","Error",MB_OK);
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
		MessageBox(NULL,"ϵͳ��������ʧ��!!","Error",MB_OK);
		return false;
	}
	return true;
}

/***********************************************************************
���ܣ��ر������豸�������˳�ʱ�����
***********************************************************************/
void	CloseAllDevice()
{
	int		i;
	int		j;
	for(i=0;i<=mDspCount;i++)
	{
		/*�ر�voip�豸*/
		if(aryDspData[i].fIsExits==1)
		{
			for(j=0;j<=aryDspData[i].fVoipCount;j++)
			{
				XMS_ctsResetDevice(pFlowHandle,&aryDspData[i].pVoipDevice[j].fDeviceID,NULL);
				XMS_ctsCloseDevice(pFlowHandle,&aryDspData[i].pVoipDevice[j].fDeviceID,NULL);
			}
			delete []aryDspData[i].pVoipDevice;
			
			/*�رսӿ��豸*/
			for(j=0;j<=aryDspData[i].fTrunkCount;j++)
			{
				XMS_ctsResetDevice(pFlowHandle,&aryDspData[i].pTrunkDevice[j].fDeviceID,NULL);	
				XMS_ctsCloseDevice(pFlowHandle,&aryDspData[i].pTrunkDevice[j].fDeviceID,NULL);
			}
			delete []aryDspData[i].pTrunkDevice;
			
			/*�ر������豸*/
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
���ܣ������ݿ��ж�ȡ������·��������Ϣ�����������룬�Ƿ�ͨ��;��
ע�⣺�������Ǹ���ͨ����ȫ���豸������ͽ��ж�ȡ
***********************************************************************/
bool	getTrunkInfoFromDb(long mDeviceNo,DJ_S32 mDspNo,ChannelID_t mChannelID,LINE_TYPE mLineType)
{
	char	mSQL[300];
	CDBProcess	objDb;
	_RecordsetPtr	objList;
	int		mIndex;
	char	mTemp[10];
	long	mChannelNo;

	/*��ȡ��������*/
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
			MessageBox(NULL,"ϵͳ��ȡ���ݿ�ʧ��!!","Error",MB_OK);
			return false;
		}
		if(!objDb.ExcueteQuery(mSQL))
		{
			MessageBox(NULL,"ϵͳ��ȡ���ݿ�ʧ��!!","Error",MB_OK);
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
			if(strcmp(mValue,"����")==0)
				aryDspData[mDspNo].pTrunkDevice[mIndex].fEnabled=true;
			else
				aryDspData[mDspNo].pTrunkDevice[mIndex].fEnabled=false;
		}

		objDb.CloseDB();
	}
	/*��ȡ��������*/
	if(mLineType==LINE_IN)
	{
		strcpy(mSQL,"select * from objuserline where fLineNo=");
		strcat(mSQL,"'");
		itoa(mDeviceNo,mTemp,10);
		strcat(mSQL,mTemp);
		strcat(mSQL,"'");
		if(!objDb.ConnectDB())
		{
			MessageBox(NULL,"ϵͳ��ȡ���ݿ�ʧ��!!","Error",MB_OK);
			return false;
		}
		if(!objDb.ExcueteQuery(mSQL))
		{
			MessageBox(NULL,"ϵͳ��ȡ���ݿ�ʧ��!!","Error",MB_OK);
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
			if(strcmp(mValue,"����")==0)
				aryDspData[mDspNo].pTrunkDevice[mIndex].fEnabled=true;
			else
				aryDspData[mDspNo].pTrunkDevice[mIndex].fEnabled=false;
			mTemp=(_variant_t)objDb.pRs->GetCollect("fLongEnable");
			if(mTemp.vt!=VT_NULL)
				strcpy(mValue,(char *)_bstr_t(mTemp));
			else
				strcpy(mValue,"");
			if(strcmp(mValue,"��ͨ")==0)
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
���ܣ�����ȫ���豸��ţ���ʾ��Ӧ����Ϣ 
ע�⣺mChannelNoΪȫ���豸���
***********************************************************************/
void	DrawOneItem(DJ_S32 mDspNo,ChannelID_t mChannelNo,DEVICE_TYPE	mDeviceType)
{
	long	mRow;		/*���б�ؼ��е��к�*/
	long	mIndex;		/*����������±�*/
	long	mDeviceIndex;	/*��Ӧ���豸�ṹ�����±�*/
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
		pControl->SetItemText(mRow,5,"�����豸");
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
			pControl->SetItemText(mRow,5,"����");
		if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLineType==LINE_OUT)
			pControl->SetItemText(mRow,5,"����");
		if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLineType==LINE_NULL)
			pControl->SetItemText(mRow,5,"����");
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
		pControl->SetItemText(mRow,5,"VOIP�豸");
		pControl->SetItemText(mRow,6,(LPCSTR)aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fLocalNumber);
		pControl->SetItemText(mRow,7,(LPCSTR)aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fCallOutNumber);
		pControl->SetItemText(mRow,8,(LPCSTR)aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fCallInNumber);
		pControl->SetItemText(mRow,9,(LPCSTR)aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fDtmfBuff);
	}
	/*ˢ��״̬��*/
	DisplayStatusBar();
	return;
}

/***********************************************************************
����:�������ͣ���ȡ��Ӧ�������ַ���
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
����:���������ͣ���ȡ��Ӧ�������ַ���
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
���ܣ�����DSP��ź�ChannelID����ȡ���豸��ȫ���豸��ţ�������ڣ����ʾ�Ѿ���ʼ��
������������ڣ����ʾҪ���µ����ݿ�
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
		MessageBox(NULL,"ϵͳ�������ݿ�ʧ��!!","Error",MB_OK);
		return mDeviceNo;
	}
	if(!objDb.ExcueteQuery(mSQL))
	{
		MessageBox(NULL,"ϵͳ��ѯ����ʧ��!!","Error",MB_OK);
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
			errormessage.Format("�������ݿ�ʧ��!\r\n������Ϣ:%s",e.ErrorMessage()); 
			AfxMessageBox(errormessage);
		}
	}
	else
		mDeviceNo=-2;
	objDb.CloseDB();
	return mDeviceNo;
}

/***********************************************************************
���ܣ�������������д�����ݿ⣬������ͬ��
1:��ѯ��ͨ���Ƿ��Ѿ����������ݿ�
2:�������������룬�������������״̬
***********************************************************************/
bool	UpdateLineStatus(long mDeviceNo,long mDspNo,ChannelID_t mChannelID,short mType)
{
	char		mSQL[200];
	char		mTemp[10];
	CDBProcess	objDb;

	/*���ߵĴ���*/
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
			MessageBox(NULL,"ϵͳ�������ݿ�ʧ��!!","Error",MB_OK);
			return false;
		}
		if(!objDb.ExcueteQuery(mSQL))
		{
			MessageBox(NULL,"ϵͳ�������ݿ�ʧ��!!","Error",MB_OK);
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
				MessageBox(NULL,"ϵͳ��������ʧ��!!","Error",MB_OK);
				return false;
		}
		}
	}
	/*���ߵĴ���*/
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
			MessageBox(NULL,"ϵͳ�������ݿ�ʧ��!!","Error",MB_OK);
			return false;
		}
		if(!objDb.ExcueteQuery(mSQL))
		{
			MessageBox(NULL,"ϵͳ�������ݿ�ʧ��!!","Error",MB_OK);
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
				MessageBox(NULL,"ϵͳ��������ʧ��!!","Error",MB_OK);
				return false;
			}
		}
	}
	return true;
}

/***********************************************************************
���ܣ����Ժ�������ʾ���е��ڴ����
***********************************************************************/
void	DebugOutput()
{
	char	mSQL[1000];
	long	i;
	char	mTemp[30];
	CDBProcess	objDb;
	long	j;

	if(!objDb.ConnectDB()) return;
	/*���trunkindex*/
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

	/*���voice index*/
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

	/*���voip index*/
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
	
	/*���dsp�Լ�����ͨ��������*/
	for(i=0;i<MAX_DSP_COUNT;i++)
	{
		/*���dsp����*/
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
				strcat(mSQL,"����");
			else
				strcat(mSQL,"������");
			strcat(mSQL,"','");
			if(aryDspData[i].fIsOpen==1)
				strcat(mSQL,"��");
			else
				strcat(mSQL,"δ��");
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
		/*���ÿ��ͨ��������*/
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
					strcat(mSQL,"����','");
				else
					strcat(mSQL,"������','");
				itoa(aryDspData[i].pTrunkDevice[j].fDeviceID.m_s16ChannelID,mTemp,10);
				strcat(mSQL,mTemp);
				strcat(mSQL,"','�ӿ�','");
				if(aryDspData[i].pTrunkDevice[j].fIsOpen)
					strcat(mSQL,"�Ѵ�','");
				else
					strcat(mSQL,"δ��','");
				if(aryDspData[i].pTrunkDevice[j].fErrorFlag)
					strcat(mSQL,"�����쳣','");
				else
					strcat(mSQL,"δ���쳣','");
				if(aryDspData[i].pTrunkDevice[j].fLineType==LINE_IN)
					strcat(mSQL,"����','");
				if(aryDspData[i].pTrunkDevice[j].fLineType==LINE_OUT)
					strcat(mSQL,"����','");
				if(aryDspData[i].pTrunkDevice[j].fLineType==LINE_NULL)
					strcat(mSQL,"����','");
				strcat(mSQL,aryDspData[i].pTrunkDevice[j].fCallOutNumber);
				strcat(mSQL,"','");
				strcat(mSQL,aryDspData[i].pTrunkDevice[j].fCallInNumber);
				strcat(mSQL,"','");
				strcat(mSQL,aryDspData[i].pTrunkDevice[j].fLocalNumber);
				strcat(mSQL,"','");
				strcat(mSQL,GetVoiceStatusString(aryDspData[i].pTrunkDevice[j].fStatusCode));
				strcat(mSQL,"','");
				if(aryDspData[i].pTrunkDevice[j].fEnabled)
					strcat(mSQL,"����','");
				else
					strcat(mSQL,"��ֹ','");
				if(aryDspData[i].pTrunkDevice[j].fEnabledCallLong)
					strcat(mSQL,"����','");
				else
					strcat(mSQL,"��ֹ','");
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
				strcat(mSQL,"ʱ϶','");
				itoa(aryDspData[i].pTrunkDevice[j].fLinkTrunkDeviceID.m_s16ChannelID,mTemp,10);
				strcat(mSQL,mTemp);
				strcat(mSQL,"','");
				strcat(mSQL,"�ӿ�')");
				if(!objDb.ExcueteSQL(mSQL))	return ;
			}
			/*��������豸*/
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
					strcat(mSQL,"����','");
				else
					strcat(mSQL,"������','");
				itoa(aryDspData[i].pVoiceDevice[j].fDeviceID.m_s16ChannelID,mTemp,10);
				strcat(mSQL,mTemp);
				strcat(mSQL,"','����','");
				if(aryDspData[i].pVoiceDevice[j].fIsOpen)
					strcat(mSQL,"�Ѵ�','");
				else
					strcat(mSQL,"δ��','");
				if(aryDspData[i].pVoiceDevice[j].fErrorFlag)
					strcat(mSQL,"�����쳣','");
				else
					strcat(mSQL,"δ���쳣','");
				strcat(mSQL,aryDspData[i].pVoiceDevice[j].fCallOutNumber);
				strcat(mSQL,"','");
				strcat(mSQL,aryDspData[i].pVoiceDevice[j].fCallInNumber);
				strcat(mSQL,"','");
				strcat(mSQL,aryDspData[i].pVoiceDevice[j].fLocalNumber);
				strcat(mSQL,"','");
				strcat(mSQL,GetVoiceStatusString(aryDspData[i].pTrunkDevice[j].fStatusCode));
				strcat(mSQL,"','");
				if(aryDspData[i].pVoiceDevice[j].fEnabled)
					strcat(mSQL,"����','");
				else
					strcat(mSQL,"��ֹ','");
				if(aryDspData[i].pVoiceDevice[j].fEnabledCallLong)
					strcat(mSQL,"����','");
				else
					strcat(mSQL,"��ֹ','");
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
				strcat(mSQL,"ʱ϶','");
				itoa(aryDspData[i].pVoiceDevice[j].fLinkTrunkDeviceID.m_s16ChannelID,mTemp,10);
				strcat(mSQL,mTemp);
				strcat(mSQL,"','");
				strcat(mSQL,"�ӿ�')");
				if(!objDb.ExcueteSQL(mSQL))	return ;
			}
			/*���voip�豸*/
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
					strcat(mSQL,"����','");
				else
					strcat(mSQL,"������','");
				itoa(aryDspData[i].pVoipDevice[j].fDeviceID.m_s16ChannelID,mTemp,10);
				strcat(mSQL,mTemp);
				strcat(mSQL,"','VOIP','");
				if(aryDspData[i].pVoipDevice[j].fIsOpen)
					strcat(mSQL,"�Ѵ�','");
				else
					strcat(mSQL,"δ��','");
				if(aryDspData[i].pVoipDevice[j].fErrorFlag)
					strcat(mSQL,"�����쳣','");
				else
					strcat(mSQL,"δ���쳣','");
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
					strcat(mSQL,"����','");
				else
					strcat(mSQL,"��ֹ','");
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
				strcat(mSQL,"ʱ϶','");
				itoa(aryDspData[i].pVoipDevice[j].fLinkVoiceDeviceID.m_s16ChannelID,mTemp,10);
				strcat(mSQL,mTemp);
				strcat(mSQL,"','");
				strcat(mSQL,"����')");
				pMainDlg->SetDlgItemText(IDC_EDIT1,mSQL);
				if(!objDb.ExcueteSQL(mSQL))	return ;
			}

		}
	}
	MessageBox(NULL,"���������Ϣ���!!","Info",MB_OK);
	return;
}


void	AddMsg(char * mInfo)
{
	if(pMainDlg->m_ListInfo.GetCount()>1000)
		pMainDlg->m_ListInfo.ResetContent();
	pMainDlg->m_ListInfo.AddString(mInfo);
}


/***********************************************************************
���ܣ���ȡ�����豸��ȫ�ֱ��
***********************************************************************/
long	getVoiceDeviceNo()
{
	mPubVoiceNo=mPubVoiceNo+1;
	return mPubVoiceNo;
}

/***********************************************************************
���ܣ���ȡVOIP�豸��ȫ�ֱ��
***********************************************************************/
long	getVoipDeviceNo()
{
	mPubVoipNo=mPubVoipNo+1;
	return mPubVoipNo;
}

/***********************************************************************
���ܣ���ȡinterface�豸��ȫ���豸���
***********************************************************************/
long	getTrunkDeviceNo()
{
	mPubTrunkNo=mPubTrunkNo+1;
	return mPubTrunkNo;
}

/***********************************************************************
���ܣ���ȡ��·״̬�ַ���
***********************************************************************/
char *	GetLineStatusString(int mLineState)
{
	switch(mLineState)
	{
	case DCS_FREE:							return "����";			break;
	case DCS_LOCAL_BLOCK:					return "��������";		break;
	case DCS_REMOTE_BLOCK:					return "Զ������";		break;
	case DCS_BLOCK:							return "���غ�Զ������";break;
	case DCS_UNAVAILABLE:					return "������";		break;
	case DCS_CALLOUT:						return "��ʼ����";		break;
	case DCS_CALLIN:						return "���н���";		break;
	case DCS_ALERTING:						return "��ַ������û�����";	break;
	case DCS_CONNECT:						return "ͨ����";		break;
	case DCS_DISCONNECT:					return "�յ�����";		break;
	case DCS_WAIT_FREE:						return "�Բ��ߵ���Ӧ";	break;
	default:
		return	"������";
	}
}

/***********************************************************************
���ܣ���Ӧ�ײ��DeviceState�¼�
***********************************************************************/
void	ProcessDeviceStatusEvt(DeviceID_t	*pDeviceID,int fLineState)
{
	long	mTrunkIndex;
	long	mDspNo;					/*�±�*/
	long	mDeviceIndex;

	/*����ǽӿ�ͨ��*/
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
	/*����豸������ͨ��*/
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
	/*�����VOIP�豸*/
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
���ܣ���һ��DSP�������豸
***********************************************************************/
bool	OpenAllDeviceInOneDsp(long	mDspNo)
{
	int		i;
	int		j;	
	RetCode_t		rtCode;

	i=mDspNo;
	/*�ȴ�DSP*/
	if(aryDspData[mDspNo].fIsExits==0) return true;		//�豸��������ֱ���˳�
	rtCode=XMS_ctsOpenDevice(pFlowHandle,&aryDspData[mDspNo].fDeviceID,NULL);
	if(rtCode==-ACSERR_BADHDL)
	{
		MessageBox(NULL,"����OpenDeviceʱ,���̾���Ƿ�!!","Error",MB_OK);
		return false;
	}
	if(rtCode==-ACSERR_FAILSEND)
	{
		MessageBox(NULL,"����OpenDeviceʱ,��������ʧ��!!�������̷�������������������!!","Error",MB_OK);
		return false;
	}
	if(rtCode==-ACSERR_BADPARAMETER)
	{
		MessageBox(NULL,"����OpenDeviceʱ,�豸���Ϊ��!!","Error",MB_OK);
		return false;
	}
	/*�������豸*/
	for(j=0;j<=aryDspData[i].fVoiceCount;j++)
	{
		rtCode=XMS_ctsOpenDevice(pFlowHandle,&aryDspData[i].pVoiceDevice[j].fDeviceID,NULL);
		if(rtCode==-ACSERR_BADHDL)
		{
			MessageBox(NULL,"����OpenDeviceʱ,���̾���Ƿ�!!","Error",MB_OK);
			return false;
		}
		if(rtCode==-ACSERR_FAILSEND)
		{
			MessageBox(NULL,"����OpenDeviceʱ,��������ʧ��!!�������̷�������������������!!","Error",MB_OK);
			return false;
		}
		if(rtCode==-ACSERR_BADPARAMETER)
		{
			MessageBox(NULL,"����OpenDeviceʱ,�豸���Ϊ��!!","Error",MB_OK);
			return false;
		}
	}
	/*�򿪽ӿ��豸*/
	for(j=0;j<=aryDspData[i].fTrunkCount;j++)
	{
		rtCode=XMS_ctsOpenDevice(pFlowHandle,&aryDspData[i].pTrunkDevice[j].fDeviceID,NULL);
		if(rtCode==-ACSERR_BADHDL)
		{
			MessageBox(NULL,"����OpenDeviceʱ,���̾���Ƿ�!!","Error",MB_OK);
			return false;
		}
		if(rtCode==-ACSERR_FAILSEND)
		{
			MessageBox(NULL,"����OpenDeviceʱ,��������ʧ��!!�������̷�������������������!!","Error",MB_OK);
			return false;
		}
		if(rtCode==-ACSERR_BADPARAMETER)
		{
			MessageBox(NULL,"����OpenDeviceʱ,�豸���Ϊ��!!","Error",MB_OK);
			return false;
		}
	}
	/*��VOIP�豸*/
	for(j=0;j<=aryDspData[i].fVoipCount;j++)
	{
		rtCode=XMS_ctsOpenDevice(pFlowHandle,&aryDspData[i].pVoipDevice[j].fDeviceID,NULL);
		if(rtCode==-ACSERR_BADHDL)
		{
			MessageBox(NULL,"����OpenDeviceʱ,���̾���Ƿ�!!","Error",MB_OK);
			return false;
		}
		if(rtCode==-ACSERR_FAILSEND)
		{
			MessageBox(NULL,"����OpenDeviceʱ,��������ʧ��!!�������̷�������������������!!","Error",MB_OK);
			return false;
		}
		if(rtCode==-ACSERR_BADPARAMETER)
		{
			MessageBox(NULL,"����OpenDeviceʱ,�豸���Ϊ��!!","Error",MB_OK);
			return false;
		}
	}
	return true;
}


/***********************************************************************
���ܣ�����ȫ���豸��ţ�������Ӧ��index������±�
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
���ܣ���ʽ��������ʾ���еĵ��ڴ�ȫ�ֱ���
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
		strcpy(mInfo,"ȫ�ֺ�:");
		strcat(mInfo,mTemp);
		itoa(aryTrunkIndex[i].fDspNo,mTemp,10);
		strcat(mInfo,"  DSP:");
		strcat(mInfo,mTemp);
		itoa(aryTrunkIndex[i].fRowNo,mTemp,10);
		strcat(mInfo," Row:");
		strcat(mInfo,mTemp);
		itoa(aryTrunkIndex[i].fIndex,mTemp,10);
		strcat(mInfo," �±�:");
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
			strcat(mInfo,"  �����������豸");
		else
			strcat(mInfo,"  δ���������豸");
		itoa(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID.m_s16ChannelID,mTemp,10);
		strcat(mInfo," ͨ��:");
		strcat(mInfo,mTemp);
		strcat(mInfo,"  LocalNumber:");
		strcat(mInfo,aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLocalNumber);

		if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLineType==LINE_IN)
		{
			strcat(mInfo," ��λ��:");
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
		strcpy(mInfo,"ȫ�ֺ�:");
		strcat(mInfo,mTemp);
		itoa(aryVoiceIndex[i].fDspNo,mTemp,10);
		strcat(mInfo,"  DSP:");
		strcat(mInfo,mTemp);
		itoa(aryVoiceIndex[i].fRowNo,mTemp,10);
		strcat(mInfo," Row:");
		strcat(mInfo,mTemp);
		itoa(aryVoiceIndex[i].fIndex,mTemp,10);
		strcat(mInfo," �±�:");
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
		strcat(mInfo," �����Ľӿ��豸:");
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
		strcpy(mInfo,"ȫ�ֺ�:");
		strcat(mInfo,mTemp);
		itoa(aryVoipIndex[i].fDspNo,mTemp,10);
		strcat(mInfo,"  DSP:");
		strcat(mInfo,mTemp);
		itoa(aryVoipIndex[i].fRowNo,mTemp,10);
		strcat(mInfo," Row:");
		strcat(mInfo,mTemp);
		itoa(aryVoipIndex[i].fIndex,mTemp,10);
		strcat(mInfo," �±�:");
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
			strcat(mInfo,"  ����������");
		else
			strcat(mInfo,"  δ��������");
		itoa(aryDspData[aryVoipIndex[i].fDspNo].pVoipDevice[aryVoipIndex[i].fIndex].fLinkVoiceChannelNo,mTemp,10);
		strcat(mInfo,mTemp);
		pMainDlg->m_VoipList.AddString(mInfo);
	}

}

/***********************************************************************
���ܣ������豸�ײ��ţ���ȡ����Ӧindex�����е������±�
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
���ܣ���̬��ʾ״̬����Ϣ
***********************************************************************/
void	DisplayStatusBar()
{
	char	mTemp[30];
	char	mInfo[300];
	strcpy(mInfo,"DSP����:");
	itoa(mOpenDspCount+1,mTemp,10);
	strcat(mInfo,mTemp);
	strcat(mInfo,"   �ӿ��豸:��");
	itoa(mPubTrunkNo+1,mTemp,10);
	strcat(mInfo,mTemp);
	strcat(mInfo,"  �Ѵ�:");
	itoa(mOpenTrunkCount+1,mTemp,10);
	strcat(mInfo,mTemp);
	strcat(mInfo,"   �����豸:��");
	itoa(mPubVoiceNo+1,mTemp,10);
	strcat(mInfo,mTemp);
	strcat(mInfo,"  �Ѵ�:");
	itoa(mOpenVoiceCount+1,mTemp,10);
	strcat(mInfo,mTemp);
	strcat(mInfo,"   IP�豸:��");
	itoa(mPubVoipNo+1,mTemp,10);
	strcat(mInfo,mTemp);
	strcat(mInfo,"  �Ѵ�:");
	itoa(mOpenVoipCount+1,mTemp,10);
	strcat(mInfo,mTemp);
	strcpy(pMainDlg->mUpdateStatusBar,mInfo);
	PostMessage(pMainDlg->m_hWnd,WM_REFRESH_STATUS_BAR,0,0);
	//pMainDlg->m_wndStatusBar.SetPaneText(7,mTemp);	
}					

/***********************************************************************
���ܣ��ж��û�˫��ĳһ����Ŀʱ���Ƿ��ܽ����޸ģ��������û�ѡ����Ҫ�༭���豸ȫ�ֱ��
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
���ܣ������ݿ��ж�ȡ��·��Ϣ����ˢ�½���
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
		if(strcmp(mTemp,"����")==-1)
			aryDspData[mDspNo].pTrunkDevice[mIndex].fEnabled=false;
		else
			aryDspData[mDspNo].pTrunkDevice[mIndex].fEnabled=true;
		mValue=(_variant_t)objDb.pRs->GetCollect("flongenable");
		if(mValue.vt!=VT_NULL)
			strcpy(mTemp,(char *)_bstr_t(mValue));
		else
			strcpy(mTemp,"");
		if(strcmp(mTemp,"��ͨ")==-1)
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
			strcpy(mTemp,"��ֹ");
		if(strcmp(mTemp,"����")==0)
			aryDspData[mDspNo].pTrunkDevice[mIndex].fEnabled=true;
		else
			aryDspData[mDspNo].pTrunkDevice[mIndex].fEnabled=false;
		objDb.CloseDB();
		DrawOneItem(mDspNo,aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID.m_s16ChannelID,DEVICE_TRUNK);
	}
	return true;
}

/***********************************************************************
���ܣ���̬ˢ��״̬��
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
���ܣ�����һ�����õ�����ͨ���������ظ�����ͨ���������±�
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
���ܣ�����ģ���м����ߵĹ�������
1.�����������ͨ�������ӵ�����ͨ����GTD����(ͨ���źż��)
2.��������ͨ���Ĺ�������
***********************************************************************/
bool	SetTrunkLineInitParameter(DeviceID_t *pTrunkDevice,DeviceID_t *pVoiceDevice)
{
	RetCode_t	rtCode;
	CmdParamData_AnalogTrunk_t	mTrunkParam;
	DJ_S16		m16ParamType;
	DJ_S16		m16ParamSize;
	void		*pTrunkParam=NULL;

	/*����GTD(ͨ����Ƶ���)��������*/
	//if(!SetVoiceGTD(pVoiceDevice))	return false;

	/*�������߹�������*/
	m16ParamType=ANALOGTRUNK_PARAM_UNIPARAM;
	m16ParamSize=sizeof(CmdParamData_AnalogTrunk_t);
	memset(&mTrunkParam,0,sizeof(mTrunkParam));
//	mTrunkParam.m_VocDevID=*pVoiceDevice;										/*ָ���󶨹�ϵ*/
//	mTrunkParam.m_u8CallerIdOption=XMS_ANALOG_TRUNK_CALLERID_FSKORDTMF;			/*���봫��ģʽ*/
//	mTrunkParam.m_u8CallInRingCount=2;											/*�������*/
//	mTrunkParam.m_u8DialtoneDetect=XMS_ANALOG_TRUNK_DIALTONE_DETECT;			/*����ź���*/
//	mTrunkParam.m_u8CallOutMethod=XMS_ANALOG_TRUNK_CALLOUT_APP_FULLCONTROL;
	mTrunkParam.m_u16CallInRingCount = 2; //ring count before CallIn event
	mTrunkParam.m_u16CallInRingTimeOut = 45;//Unit:ms Ring TimeOut,when ring timed out ,state changed to DCS_Free

	pTrunkParam=(void *) &mTrunkParam;

	rtCode=XMS_ctsSetParam(pFlowHandle,pTrunkDevice,m16ParamType,m16ParamSize,(void *)pTrunkParam);
	if(rtCode<0)	return false;
	return true;
}

/***********************************************************************
���ܣ����������豸��GTD����
***********************************************************************/
bool	SetVoiceGTD(DeviceID_t *pVoiceDevice)
{
	RetCode_t	rtCode;
	DJ_S16		m16ParamType;
	DJ_S16		m16ParamSize;
	void		*pTrunkParam=NULL;
	CmdParamData_Voice_t	mVoiceParam;
	/*����GTD(ͨ����Ƶ���)��������*/
	memset(&mVoiceParam,0,sizeof(CmdParamData_Voice_t));
	mVoiceParam.m_u8GtdCtrlValid=1;												/*����GTD*/
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
���ܣ�����user�豸�Ĳ���
***********************************************************************/
bool	SetUserDeviceParam(DeviceID_t *pUserDevice,DeviceID_t *pVoiceDevice)
{
	RetCode_t	rtCode;
	DJ_S16		m16ParamType;
	DJ_S16		m16ParamSize;
	void		*pUserParam=NULL;
	CmdParamData_AnalogUser_t mUserParam;

	/*����User�豸��������*/
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
���ܣ�����Ӧ�������豸���Ų�������æ��������������ֹͣ����
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
���ܣ����ýӿ��豸ͨ��״̬
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
���ܣ����������豸��ͨ��״̬
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
���ܣ�����VOIP�豸��ͨ��״̬
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
���ܣ�����һ�����õ�����ͨ������ϵͳ������ѯ����δ�ҵ��򷵻�-1
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
���ܣ����������豸֮���˫������
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
���ܣ���������豸֮�������
***********************************************************************/
void	DeleteLink(DeviceID_t	*pSourceDev,DeviceID_t	*pTargetDev)
{
	int r=0; 
	r=XMS_ctsUnlinkDevice(pFlowHandle,pSourceDev,pTargetDev,NULL);
	r=XMS_ctsUnlinkDevice(pFlowHandle,pTargetDev,pSourceDev,NULL);
}

/***********************************************************************
���ܣ���λ�ӿ��豸����������Ϊ��ʼ��״̬(������ֻ��������)
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
	/*��������ӵĽӿ��豸���򽫸��������*/
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
			/*��������߽ӿڵ����߽ӿڷ���һ�����ߺ���*/
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

	/*���߲�IP*/
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
			/*IPͨ����æ��*/
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
	/*��ͨ�����ݳ�ʼ��*/
	aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkTrunk=false;
	aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkTrunkChannelNo=-1;
	strcpy(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff,"");		
	strcpy(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fCallInNumber,"");
	strcpy(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fCallOutNumber,"");
	/*�Ĳ�����ݳ�ʼ��*/
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
���ܣ���λ�ӿ��豸����������Ϊ��ʼ��״̬(������ֻ��������)
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
	/*����������Ĳ�ɵ�״̬�£�����������*/
	if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsChaHuan==false)
	{
		/*��������ӵĽӿ��豸���򽫸��������*/
		if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkTrunk)
		{
			DeviceID_t		mLinkDeviceID;
			long			mLinkIndex;
			long			mLinkDspNo;
			long			mLinkDeviceIndex;
			mLinkDeviceID=aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkTrunkDeviceID;
			/*������ӵ�������*/
			if(mLinkDeviceID.m_s16DeviceMain!=0 && mLinkDeviceID.m_s16DeviceSub==XMS_DEVSUB_ANALOG_USER)
			{
				mLinkIndex=FindIndexByChannelNo(mDspNo,aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkTrunkChannelNo,DEVICE_TRUNK);
				mLinkDeviceIndex=aryTrunkIndex[mLinkIndex].fIndex;
				mLinkDspNo=aryTrunkIndex[mLinkIndex].fDspNo;
				DeleteLink(&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkTrunkDeviceID,
					&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID);
				aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fIsLinkTrunk=false;
				/*������Ľӿ��豸������ͨ����æ��*/
				if(aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fIsLinkVoice==false)
				{
					SetLink(&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDeviceID,
						&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkVoiceDeviceID);
					aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fIsLinkVoice=true;
				}
				/*��ֹͣ����*/
				PlayTone(&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkVoiceDeviceID,BUSY_TONE);
				XMS_ctsClearCall(pFlowHandle,&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDeviceID,0,NULL);
			}
			/*������ӵ������ߣ�������������߲������ߣ����������ȹһ�*/
			if(mLinkDeviceID.m_s16DeviceMain!=0 && mLinkDeviceID.m_s16DeviceSub==XMS_DEVSUB_ANALOG_TRUNK)
			{
				mLinkIndex=FindIndexByChannelNo(mDspNo,aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkTrunkChannelNo,DEVICE_TRUNK);
				mLinkDeviceIndex=aryTrunkIndex[mLinkIndex].fIndex;
				mLinkDspNo=aryTrunkIndex[mLinkIndex].fDspNo;
				DeleteLink(&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkTrunkDeviceID,
					&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID);
				aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkTrunkChannelNo=-1;
				aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fIsLinkTrunk=false;
				/*��ַ�����������Ϣ*/
				XMS_ctsClearCall(pFlowHandle,&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDeviceID,0,NULL);
				XMS_ctsResetDevice(pFlowHandle,&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDeviceID,NULL);
				XMS_ctsGetDevState(pFlowHandle,&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDeviceID,NULL);
				aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fIsLinkVoice=false;
				/*������ͨ������Ϊ����״̬*/
				SetTrunkDeviceState(&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDeviceID,VOC_FREE);
				strcpy(aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fCallInNumber,"");
				strcpy(aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fCallOutNumber,"");
				strcpy(aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDtmfBuff,"");
			}
		}
		/*������ӵ���VOIP*/
		if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fisLinkVoipVoice)
		{
			long		mLinkVoipVoiceNo;
			long		mLinkVoipVoiceDspNo;
			long		mLinkVoipVoiceIndex;
			long		mVoipDspNo;
			long		mVoipDeviceNo;
			long		mVoipDeviceIndex;
			
			/*���Ƚ����ӿ�ͨ����VOIP�豸������ͨ��֮������ӶϿ�*/		
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
			/*��VOIPͨ�����������Ϣ*/
			mVoipDeviceNo=aryDspData[mLinkVoipVoiceDspNo].pVoiceDevice[mLinkVoipVoiceIndex].fLinkVoipChannelNo;
			mVoipDspNo=mDspNo;			/*�����û��*/
			mVoipDeviceIndex=FindIndexByChannelNo(mVoipDspNo,mVoipDeviceNo,DEVICE_VOIP);
			mVoipDspNo=aryVoipIndex[mVoipDeviceIndex].fDspNo;
			mVoipDeviceIndex=aryVoipIndex[mVoipDeviceIndex].fIndex;
			XMS_ctsClearCall(pFlowHandle,&aryDspData[mVoipDspNo].pVoipDevice[mVoipDeviceIndex].fDeviceID,0,NULL);
		}
	}
	else
	{
		/*�Ĳ�ɵĴ���*/
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
				AddMsg("���ֹͣ����");
			}
			/*��ȡ��ǰ���ӵĽӿ�ͨ����Ϣ*/
			mLinkDeviceNo=aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkTrunkChannelNo;
			mLinkDspNo=-1;
			mLinkDeviceIndex=FindIndexByChannelNo(mLinkDspNo,mLinkDeviceNo,DEVICE_TRUNK);
			mLinkDspNo=aryTrunkIndex[mLinkDeviceIndex].fDspNo;
			mLinkDeviceIndex=aryTrunkIndex[mLinkDeviceIndex].fIndex;
			/*��ȡҪֱ�����ӵ�ͨ��*/
			/*���ӵ��ǽӿ�*/
			if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fPrevDeviceType==1)
			{
				mPrevDeviceNo=aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fPrevTrunkChannelNo;
				mPrevDspNo=-1;
				mPrevDeviceIndex=FindIndexByChannelNo(mPrevDspNo,mPrevDeviceNo,DEVICE_TRUNK);
				mPrevDspNo=aryTrunkIndex[mPrevDeviceIndex].fDspNo;
				mPrevDeviceIndex=aryTrunkIndex[mPrevDeviceIndex].fIndex;
				/*���е������ɹ������͵���������ͨ���ˡ��Źһ�*/
				if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fChaHuanIsCallStatus==true)
				{
					/*�Ͽ���ͨ��������ͨ���Ľӿ�*/
					/*�¼ӵĴ���-----wwk*/
					ControlPlay_t	mControlPlay;
					mControlPlay.m_u16ControlType=XMS_STOP_PLAY;
					XMS_ctsControlPlay(pFlowHandle,&aryDspData[mPrevDspNo].pVoipDevice[mPrevDeviceIndex].fLinkVoiceDeviceID,&mControlPlay,NULL);
					AddMsg("���ֹͣ����1");
					DeleteLink(&aryDspData[mPrevDspNo].pTrunkDevice[mPrevDeviceIndex].fDeviceID,
						&aryDspData[mPrevDspNo].pTrunkDevice[mPrevDeviceIndex].fLinkVoiceDeviceID);
					/*�Ͽ���ͨ����ǰ��ͨ��������*/
					DeleteLink(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,
						&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkTrunkDeviceID);
					aryDspData[mPrevDspNo].pTrunkDevice[mPrevDeviceIndex].fIsLinkVoice=false;
					aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkVoice=false;
					/*����ͨ����Ŀ��ͨ������˫������*/	
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
				/*�ñ��Ϊ���ѹһ�*/
				aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fChaHuanIsReset=true;
				aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkTrunkChannelNo=aryDspData[mPrevDspNo].pTrunkDevice[mPrevDeviceIndex].fChannelNo;
				aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkTrunkDeviceID=aryDspData[mPrevDspNo].pTrunkDevice[mPrevDeviceIndex].fDeviceID;
				aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fChaHuanLinkChannenlNo=aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fChannelNo;
				aryDspData[mPrevDspNo].pTrunkDevice[mPrevDeviceIndex].fLinkTrunkChannelNo=aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fChannelNo;
				aryDspData[mPrevDspNo].pTrunkDevice[mPrevDeviceIndex].fLinkTrunkDeviceID=aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDeviceID;
				SetTrunkDeviceState(&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDeviceID,VOC_WAITHANGUP);
				SetTrunkDeviceState(&aryDspData[mPrevDspNo].pTrunkDevice[mPrevDeviceIndex].fDeviceID,VOC_WAITHANGUP);
			}
			/*���ӵ���VOIP*/
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

				/*�Ͽ���ͨ����ǰ��ͨ��������*/
				DeleteLink(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,
					&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkTrunkDeviceID);
				
				/*����˫������*/
				SetLink(&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDeviceID,
					    &aryDspData[mPrevDspNo].pVoipDevice[mPrevDeviceIndex].fLinkVoiceDeviceID);
				/*���û���*/
				MixerControlParam_t paramMixer = {0};
				paramMixer.m_u8SRC1_Ctrl = XMS_MIXER_FROM_PLAY ;
				paramMixer.m_u16SRC_ChID1 = aryDspData[mPrevDspNo].pVoipDevice[mPrevDeviceIndex].fLinkVoiceDeviceID.m_s16ChannelID;
				paramMixer.m_u8SRC2_Ctrl = XMS_MIXER_FROM_INPUT;
				paramMixer.m_u16SRC_ChID2 = aryDspData[mPrevDspNo].pVoipDevice[mPrevDeviceIndex].fLinkVoiceDeviceID.m_s16ChannelID;
				//RetCode_t ret = XMS_ctsSetParam(pFlowHandle, &aryDspData[mPrevDspNo].pVoipDevice[mPrevDeviceIndex].fDeviceID,
				//	VOIP_PARAM_MIXER, sizeof(paramMixer), &paramMixer);
				XMS_ctsLinkDevice(pFlowHandle,&aryDspData[mPrevDspNo].pVoipDevice[mPrevDeviceIndex].fDeviceID,
					              &aryDspData[mPrevDspNo].pVoipDevice[mPrevDeviceIndex].fLinkVoiceDeviceID,NULL);
				/*ֹͣ��ת����*/
				mControlPlay.m_u16ControlType=XMS_STOP_PLAY;
				XMS_ctsControlPlay(pFlowHandle,&aryDspData[mPrevDspNo].pVoipDevice[mPrevDeviceIndex].fLinkVoiceDeviceID,&mControlPlay,NULL);
				AddMsg("���ֹͣ����ת����");
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
	/*��ͨ�����ݳ�ʼ��*/
	/*��������������ͨ��*/
	//AddMsg(GetVoiceStatusString(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode));
	if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode==VOC_ERROR_CALL_LONG ||
		aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode==VOC_PLAY_NOTICE ||
		aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode==VOC_PLAY_NUMBER ||
		aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode==VOC_PLAY_DIAL_WRONG ||
		aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode==VOC_PLAY_WELCOME)
	{
		/*��ֹͣ����*/
		ControlPlay_t	mPlayControl;
		mPlayControl.m_u16ControlType=XMS_STOP_PLAY;
		XMS_ctsControlPlay(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID,&mPlayControl,NULL);
		AddMsg("����������ֹͣ����");
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
���ܣ����ݵ绰�����������Ӧ������ͨ������������ͨ���ŵ�ȫ���豸���
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
���� �����߹�������--��Ӧ�ʹ��������йص������¼�
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
		/*�����¼�*/
		{
			DisplayEvt(pEvt);
			ResetUserDevice(pDevice,pEvt);
			return;
		}
		break;
	case	XMS_EVT_CALLIN:
		/*�û�ժ���¼�*/
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
		/*�����¼�*/
		{
			Acs_CallControl_Data	*pControlData=NULL;
			DisplayEvt(pEvt);
			pControlData=(Acs_CallControl_Data *)FetchEventData(pEvt);
			/*��ȡ��ͨ���Ļ�������*/
			mChannelID=pDevice->m_s16ChannelID;
			mDspNo=pDevice->m_s8ModuleID;
			mIndex=FindIndexByChannelID(mDspNo,mChannelID,DEVICE_TRUNK);
			mDeviceIndex=aryTrunkIndex[mIndex].fIndex;
			/*���ӵ��ǽӿ��豸*/
			if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkTrunk==true)
			{
				mLinkChannelNo=aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkTrunkChannelNo;
				mLinkDeviceIndex=FindIndexByChannelNo(mDspNo,mLinkChannelNo,DEVICE_TRUNK);
				mLinkDspNo=aryTrunkIndex[mLinkDeviceIndex].fDspNo;
				mLinkDeviceIndex=aryTrunkIndex[mLinkDeviceIndex].fIndex;
				if(pControlData->m_s32AcsEvtState==1)
				{
					/*���гɹ�*/
					/*�����ж˽ӿ�ͨ��������ͨ��֮������ӶϿ�*/
					if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsChaHuan==false)
					{
						PlayTone(&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkVoiceDeviceID,STOP_TONE);
						DeleteLink(&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDeviceID,
							&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkVoiceDeviceID);
						XMS_ctsResetDevice(pFlowHandle,&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkVoiceDeviceID,NULL);
						XMS_ctsGetDevState(pFlowHandle,&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkVoiceDeviceID,NULL);

						aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fIsLinkVoice=false;
						/*����˫������*/
						SetLink(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,
							&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDeviceID);
						aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkTrunk=true;
						aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkVoice=false;			/*���ص���������Ҳ���Ͽ�*/
						aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fIsLinkTrunk=true;
						aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fIsLinkVoice=false;	/*ԭ�е�VOICE�����Ѿ����Ͽ�*/
						/*����ϵͳ�ײ�Ĵ���*/
						aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fChaHuanIsCallStatus=true;
						SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,VOC_CONNECTED);
						SetTrunkDeviceState(&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDeviceID,VOC_CONNECTED);
					}
					else
					{
						/*����ǲ�ɣ���ʱ��˫���Ľӿ�ͨ���Ѿ�����ʱ϶���ӣ�����˫��������ͨ���Ѿ��ر�*/
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
								/*�ں��гɹ�֮ǰ�͹һ��ˡ���ֱ�ӽ������ߺ����ߵ�����*/
								ControlPlay_t	mControlPlay;
								DeleteLink(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,
									&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID);
								aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkVoice=false;
								XMS_ctsResetDevice(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID,NULL);
								XMS_ctsGetDevState(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID,NULL);

								mControlPlay.m_u16ControlType=XMS_STOP_PLAY;
								XMS_ctsControlPlay(pFlowHandle,&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkVoiceDeviceID,&mControlPlay,NULL);
								AddMsg("����ǰ�һ�ֹͣ����");
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
					/*����ʧ�ܣ����ȼ���ǲ��������Ĳ�����̣����ǣ�����������*/
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
						/*�Ĳ��ʱ��������жԷ�ʧ�ܣ������½ӻ���*/
						/*�ӻ�ԭ��ͨ��*/
						if(aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fPrevDeviceType==1 && 
						   aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLineState==DCS_CONNECT)
						{	
							/*ԭͨ��Ϊ�ӿ��豸*/
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

							AddMsg("��ɷ���ֹͣ");
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
							/*ԭͨ��ΪVOIP�豸*/
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

							AddMsg("���VOIPֹͣ����");
							/*������ͨ����ӿ�ͨ�����а�*/
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
						/*���Ĳ���������*/
						aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fIsChaHuan=false;
						aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fChaHuanCount=0;
						SetTrunkDeviceState(&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fDeviceID,VOC_CONNECTED);
					}
				}
			} 
			/*������ӵ���VOIP�豸*/
			if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fisLinkVoipVoice==true)
			{
				if(pControlData->m_s32AcsEvtState==1)
				{
					PlayTone(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoipVoiceDeviceID,STOP_TONE);
					/*��VOIP������ͨ������ϯ����˫������*/
					SetLink(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,
							&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoipVoiceDeviceID);
					SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,VOC_CONNECTED);

					/*�¼������*/
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
					/*VOIP������ϯ����ϯ��Ӧ��*/
					long	mVoipDspNo;
					long	mVoipDeviceIndex;
					long	mVoipDeviceNo;
					mVoipDeviceNo=aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoipDeviceNo;
					mVoipDspNo=-1;
					mVoipDeviceIndex=FindIndexByChannelNo(mVoipDspNo,mVoipDeviceNo,DEVICE_VOIP);
					mVoipDspNo=aryVoipIndex[mVoipDeviceIndex].fDspNo;
					mVoipDeviceIndex=aryVoipIndex[mVoipDeviceIndex].fIndex;
					/*��Է���������Ϣ*/
					XMS_ctsClearCall(pFlowHandle,&aryDspData[mVoipDspNo].pVoipDevice[mVoipDeviceIndex].fDeviceID,0,NULL);
					ResetVoipDevice(&aryDspData[mVoipDspNo].pVoipDevice[mVoipDeviceIndex].fDeviceID,NULL);
					ResetUserDevice(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,NULL);
				}
			}
		}
		break;
	case	XMS_EVT_LINKDEVICE:
		/*���������¼�*/
		break;
	case	XMS_EVT_UNLINKDEVICE:
		/*��������¼�*/
		break;
	case	XMS_EVT_SENDIODATA:
		/*���������¼�*/
		{
			Acs_IO_Data	 *pSendData;
			pSendData=(Acs_IO_Data *)FetchEventData(pEvt);
			char	mTemp[20];
			itoa(mChannelID,mTemp,10);
			AddMsg(mTemp);
		}
		break;
	case	XMS_EVT_ANALOG_INTERFACE:
		/*�Ĳ���¼�*/
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
					/*��ɴ�����*/
					aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsChaHuan=true;
					ChaHuanProcess(mDspNo,mDeviceIndex);
				}
			}
		}
		break;
	case	XMS_EVT_PLAY:
		/*�������*/
		{
			Acs_MediaProc_Data		*pMediaData=NULL;
			long			mCode;
			DisplayEvt(pEvt);
			pMediaData=(Acs_MediaProc_Data *)FetchEventData(pEvt);
			mCode=pMediaData->m_s32AcsEvtErrCode;
			if(mCode==0)	AddMsg("��������");
			if(mCode==1)	AddMsg("�յ�Ӧ�ó���ָֹͣ��");
			if(mCode==2)	AddMsg("�յ�DTMF����TOne��");
			if(mCode==3)	AddMsg("���ļ�ʧ��");
			if(mCode==4)	AddMsg("������ͨ������");
			if(mCode==5)	AddMsg("��Ӳ��ģ������Ͽ�");

			if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode==VOC_FREE)return;
			/*�����ǰ��״̬�ǲ��ŷǷ�����;���������֮�󣬽��ŷ�æ�����߹�*/
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
				/*����VOIP������ʾ�������⴦��*/
				XMS_ctsClearCall(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoipDeviceID,0,NULL);
				ResetUserDevice(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,NULL);
				ResetVoipDevice(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoipDeviceID,NULL);
				return;
			}
		}
		break;
	case	XMS_EVT_SETPARAM:
		{
			AddMsg("�������");
		}
		break;
	case	XMS_EVT_RECVIODATA:				
		/*���������¼�--DTMF*/
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
					/*ֹͣ�Ų�����*/
					PlayTone(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID,STOP_TONE);
					InsertDtmfToDevice(mDspNo,mDeviceIndex,DEVICE_TRUNK,pDtmf);
					AddMsg(pDtmf);

				}
				if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff[0]=='9' &&
				   aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode!=VOC_FLASH_WAIT_DIAL)
				{
					/*��������*/
					strcpy(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff,"");
					CallTrunk(mDspNo,mDeviceIndex,false);
				}
				if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff[0]=='0' &&
				   aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode!=VOC_FLASH_WAIT_DIAL)
				{
					/*��������,�����Ǻ��г�;*/
					strcpy(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff,"");
					CallTrunk(mDspNo,mDeviceIndex,true);
				}
				if(strcmp(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff,"##00")==0 && 
				   aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode!=VOC_FLASH_WAIT_DIAL)
				{
					/*��ѯ��;��ͨ״̬*/
					strcpy(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff,"");
					QueryCallLongStatus(mDspNo,mDeviceIndex);
					return;
				}
				if(strcmp(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff,"##01")==0 &&
				   aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode!=VOC_FLASH_WAIT_DIAL)
				{
					/*��ѯ��������*/
					strcpy(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff,"");
					PlayPhoneNumber(mDspNo,mDeviceIndex);
					return;
				}
				if(strcmp(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff,"##02")==0 && 
				   aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode!=VOC_FLASH_WAIT_DIAL)
				{
					/*������;����*/
					strcpy(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff,"");
					SetEnableCallLong(mDspNo,mDeviceIndex);
					return;
				}
				if(strcmp(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff,"##03")==0 &&
				   aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode!=VOC_FLASH_WAIT_DIAL)
				{
					/*�رճ�;����*/
					strcpy(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff,"");
					SetDisEnableCallLong(mDspNo,mDeviceIndex);
					return;
				}
				if(strcmp(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff,"##07")==0 && 
				   aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode!=VOC_FLASH_WAIT_DIAL)
				{
					/*�رյ绰*/
					strcpy(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff,"");
					SetDisableCall(mDspNo,mDeviceIndex);
					return;
				}
				if(strcmp(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff,"##08")==0 &&
				   aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode!=VOC_FLASH_WAIT_DIAL)
				{
					/*�����绰*/
					strcpy(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff,"");
					SetEnableCall(mDspNo,mDeviceIndex);
					return;
				}
				/*���Ź��ܲ˵�*/					
				/*if(strcmp(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff,"##09")==0 && 
				   aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode!=VOC_FLASH_WAIT_DIAL)
				{
					strcpy(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff,"");
					PlayMenu(mDspNo,mDeviceIndex);
					return;
				}
				*/

				/*��λ����*/
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
               /*���߲���IP����*/
				if(strlen(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff)==5 &&
					aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff[0]=='8' &&
					aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode!=VOC_FLASH_WAIT_DIAL
					)
				{
					/*�����ܲ�����VOIP*/
					/*unsigned int i;
					for(i=0;i<strlen(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff);i++)
						aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff[i]=aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff[i+1];
					aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff[i]='\0';
					*/
					CallVoip(mDspNo,mDeviceIndex,false);
				}

                   /*���߲���IP*/
				if(strlen(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff)==4 &&
					aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff[0]=='7' &&
					   aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode!=VOC_FLASH_WAIT_DIAL
					)
				{
					/*�����ܲ�����VOIP*/
					CallVoip(mDspNo,mDeviceIndex,false);
				}

				if(strlen(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff)==4)
				{
					/*�������߻��ߺ����ܲ�*/

					if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff[0]=='5')
					{
						/*���ߺ�������*/
						CallUser(mDspNo,mDeviceIndex);	
					}
					else if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff[0]=='7' &&
					   aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode!=VOC_FLASH_WAIT_DIAL)
					{
						/*�����ܲ�����VOIP*/
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
							/*����ͨ����״̬����Ϊ�Ƿ����г�;״̬*/
							SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,VOC_PLAY_NOTICE);
						}
					}
				}
				//�쳣����
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
					/*����ͨ����״̬����Ϊ�Ƿ����г�;״̬*/
					SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,VOC_PLAY_NOTICE);
				}

			}
			/*�¶δ���ר�������ղ����ߵĺ���*/
			if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode==VOC_CALL_OUT &&
			   aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkTrunk==true)
			{
				/*�����պ�*/
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
���ܣ����߹�������--��Ӧ�ʹ��������йص������¼�
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
		/*�����¼�*/
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
					/*����ͨת��*/
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
						
						/*��������������ͨ��*/
						mLinkTrunkIndex=FindIndexByChannelNo(mDspNo,aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkTrunkChannelNo,DEVICE_TRUNK);
						mLinkTrunkDspNo=aryTrunkIndex[mLinkTrunkIndex].fDspNo;
						mLinkTrunkIndex=aryTrunkIndex[mLinkTrunkIndex].fIndex;
						/*��������������ͨ���󶨵�����ͨ��*/
						mLinkVoiceIndex=FindIndexByChannelID(aryDspData[mLinkTrunkDspNo].pTrunkDevice[mLinkTrunkIndex].fLinkVoiceDeviceID.m_s8ModuleID,aryDspData[mLinkTrunkDspNo].pTrunkDevice[mLinkTrunkIndex].fLinkVoiceDeviceID.m_s16ChannelID,DEVICE_VOICE);
						mLinkVoiceDspNo=aryVoiceIndex[mLinkVoiceIndex].fDspNo;
						mLinkVoiceIndex=aryVoiceIndex[mLinkVoiceIndex].fIndex;
						

						/*�Ͽ���ͨ������������ͨ������*/
						DeleteLink(&aryDspData[mLinkTrunkDspNo].pTrunkDevice[mLinkTrunkIndex].fDeviceID,
								   &aryDspData[mLinkTrunkDspNo].pTrunkDevice[mLinkTrunkIndex].fLinkVoiceDeviceID);


						aryDspData[mLinkTrunkDspNo].pTrunkDevice[mLinkTrunkIndex].fIsLinkVoice=false;
						aryDspData[mLinkVoiceDspNo].pVoiceDevice[mLinkVoiceIndex].fIsLinkTrunk=false;
						/*������ͨ��������ͨ��֮�佨��ʱ϶����*/
						SetLink(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
								&aryDspData[mLinkTrunkDspNo].pTrunkDevice[mLinkTrunkIndex].fDeviceID);
						/*�޸�����ͨ����״̬*/
						SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,VOC_CALL_OUT);
						SetTrunkDeviceState(&aryDspData[mLinkTrunkDspNo].pTrunkDevice[mLinkTrunkIndex].fDeviceID,VOC_CALL_OUT);

						/*��������ת�����ߺ��еķ����¼�*/
						/*Ϊ���ܹ����յ����к��룬���Խ��м�ͨ������������ͨ������һ�����������*/
						//XMS_ctsLinkDevice(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
						//				  &aryDspData[mLinkVoiceDspNo].pVoiceDevice[mLinkVoiceIndex].fDeviceID,NULL);
						return;
					}
			}
	}
		break;
	case	XMS_EVT_CALLIN:
		/*�����¼�*/
		{
			Acs_CallControl_Data	*pControlData=NULL;
			pControlData=(Acs_CallControl_Data *)FetchEventData(pEvt);			
			/*�������ջ���*/
			strcpy(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fCallInNumber,aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLocalNumber);
			/*�ж��Ƿ��ǻ���ͨ����ת�ӹ�����*/
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
					/*��ʼ�Ż�ӭ��*/
					/*�����Ľ���*/
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
		
			/*�����Ľ���*/
			XMS_ctsAlertCall(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,NULL);
			XMS_ctsAnswerCallIn(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,NULL);
			//SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,VOC_CALL_IN);
			/*�ȴ��û�����*/
			SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,VOC_ANSWER_CALL);
		}
		break;
	case	XMS_EVT_ANSWERCALL:
		/*����Ӧ��ɹ�*/
		{
			if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode==VOC_ANSWER_CALL)
			{
				ControlPlay_t			mControlPlay;
				mControlPlay.m_u16ControlType=XMS_STOP_PLAY;
				/*�����Ľ���*/
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
		/*�����¼�*/
		{
			Acs_CallControl_Data		*pControlData=NULL;
			pControlData=(Acs_CallControl_Data *)FetchEventData(pEvt);
			DisplayEvt(pEvt);
			if(pControlData->m_s32AcsEvtState==1)
			{
				/*����ɹ������ô���*/
				if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fisLinkVoipVoice)
				{
					SetLink(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,
						&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoipVoiceDeviceID);
				}
			}
			else
			{
				/*����ʧ��*/
				/*��һ�����������ͨ��������ͨ��֮�������*/
				mChannelID=pDevice->m_s16ChannelID;
				mDspNo=pDevice->m_s8ModuleID;
				mIndex=FindIndexByChannelID(mDspNo,mChannelID,DEVICE_TRUNK);
				mDeviceIndex=aryTrunkIndex[mDspNo].fIndex;
				mLinkDeviceNo=aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkTrunkChannelNo;
				mLinkDeviceIndex=FindIndexByChannelNo(mDspNo,mLinkDeviceNo,DEVICE_TRUNK);
				if(mLinkDeviceIndex<0)
				{
					/*��ܵײ����*/
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
				/*�ڶ�����������ͨ����æ��*/
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
		/*���������¼�*/
		break;
	case	XMS_EVT_PLAY:
		/*�����¼�*/
		{
			/*���Ż�ӭ��ʾ������Ϻ�Ĵ���*/
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
			/*�û����Ŵ��󣬷�����Ϻ��æ��*/
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
		/*��������¼�*/
		break;
	case	XMS_EVT_SENDIODATA:
		/*���������¼�*/
		{
			Acs_IO_Data	 *pSendData;
			pSendData=(Acs_IO_Data *)FetchEventData(pEvt);
			if(pSendData->m_u16IoDataLen>0)
				AddMsg("���ͳɹ�����");
			else
				AddMsg("����ʧ������");
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
					/*��������������ͨ��*/
					//sleep(500)
					mLinkTrunkIndex=FindIndexByChannelNo(mDspNo,aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkTrunkChannelNo,DEVICE_TRUNK);
					mLinkTrunkDspNo=aryTrunkIndex[mLinkTrunkIndex].fDspNo;
					mLinkTrunkIndex=aryTrunkIndex[mLinkTrunkIndex].fIndex;
					/*��������������ͨ���󶨵�����ͨ��*/
					mLinkVoiceIndex=FindIndexByChannelID(aryDspData[mLinkTrunkDspNo].pTrunkDevice[mLinkTrunkIndex].fLinkVoiceDeviceID.m_s8ModuleID,aryDspData[mLinkTrunkDspNo].pTrunkDevice[mLinkTrunkIndex].fLinkVoiceDeviceID.m_s16ChannelID,DEVICE_VOICE);
					mLinkVoiceDspNo=aryVoiceIndex[mLinkVoiceIndex].fDspNo;
					mLinkVoiceIndex=aryVoiceIndex[mLinkVoiceIndex].fIndex;
					/*�Ͽ���ͨ������������ͨ������*/
					DeleteLink(&aryDspData[mLinkTrunkDspNo].pTrunkDevice[mLinkTrunkIndex].fDeviceID,
							   &aryDspData[mLinkTrunkDspNo].pTrunkDevice[mLinkTrunkIndex].fLinkVoiceDeviceID);
					aryDspData[mLinkTrunkDspNo].pTrunkDevice[mLinkTrunkIndex].fIsLinkVoice=false;
					aryDspData[mLinkVoiceDspNo].pVoiceDevice[mLinkVoiceIndex].fIsLinkTrunk=false;
					/*������ͨ��������ͨ��֮�佨��ʱ϶����*/
					SetLink(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
							&aryDspData[mLinkTrunkDspNo].pTrunkDevice[mLinkTrunkIndex].fDeviceID);
					/*�޸�����ͨ����״̬*/
					SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,VOC_CALL_OUT);
					SetTrunkDeviceState(&aryDspData[mLinkTrunkDspNo].pTrunkDevice[mLinkTrunkIndex].fDeviceID,VOC_CALL_OUT);

					/*��������ת�����ߺ��еķ����¼�*/
					/*Ϊ���ܹ����յ����к��룬���Խ��м�ͨ������������ͨ������һ�����������*/
/*					XMS_ctsLinkDevice(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
									  &aryDspData[mLinkVoiceDspNo].pVoiceDevice[mLinkVoiceIndex].fDeviceID,NULL);
*/
					return;				
			}

		}
		break;
	case	XMS_EVT_RECVIODATA:
		/*���������¼������ڲ���ģ�������ˣ�����һЩ�¼��ķ��أ��ǲ���dtmf��ʵ�֡�*/
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
				/*�ź����ļ��*/
				if(*pDtmfBuff=='G' || *pDtmfBuff=='g')	//����ǲ�����
				{
					if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode==VOC_WAIT_FIRST_DIAL_TONE)
					{
						//����DTMFǰ������
						if(strcmp(mPrevCode,"")!=0)
						{
							if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkVoice==false)
							{
								SetLink(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID);
								aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fIsLinkVoice=true;
							}
							//SetTrunkLineInitParameter(&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fLinkVoiceDeviceID);
							//�ȴ����ò����¼�����
							XMS_ctsSetDevTimer(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDeviceID,500);
							return;
						}
					}
				}
			}
			if(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fStatusCode==VOC_WAIT_DIAL)
			{
				/*��⣬�������к��룬���ڴ˴������û��İ���*/
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
						/*���ֻ�����0*/
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
					//��������תIP����
					if(strlen(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff)==4 && 
					   aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff[0]=='7')
					{
						strcpy(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fCallOutNumber,aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff);
						CallVoip(mDspNo,mDeviceIndex,true);
					}

					if(strlen(aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff)==5 &&
						aryDspData[mDspNo].pTrunkDevice[mDeviceIndex].fDtmfBuff[0]=='8')
					{
						/*�����ܲ�����VOIP*/
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
���ܣ���dtmf����뵽ĳ��ͨ����dtmf������
***********************************************************************/
void	InsertDtmfToDevice(long mDspNo,long mIndex,DEVICE_TYPE mDeviceType,char * pDtmf)
{
	strcat(aryDspData[mDspNo].pTrunkDevice[mIndex].fDtmfBuff,pDtmf);
//	AddMsg(pDtmf);
	DrawOneItem(mDspNo,aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID.m_s16ChannelID,DEVICE_TRUNK);
	return;
}

/***********************************************************************
���ܣ�ʵ�����ߺ������ߡ����������¼�����
***********************************************************************/
bool	CallUser(long mDspNo,long mIndex)
{
	char	mLocalNumber[20];
	char	mTargetNumber[20];
	long	mTargetChannelNo;		/*ȫ���豸���*/
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
		/*���еĺ��벻����*/
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
	
	/*�������·����ֹ����Ҳֱ�ӷ�æ��*/
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

	/*����Լ����Լ���绰���򲥷�������ʾ*/
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
	/*�Է���·Ϊ�ǿ���״̬*/
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
	/*ADD BY TIM,���Ӷ�user�豸�������е�����*/ 
	if(aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fIsLinkVoice==false)
	{
		//SetLink(&aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fDeviceID,
		//	&aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fLinkVoiceDeviceID);
		//aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fIsLinkVoice=true;
	}
	SetUserDeviceParam(&aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fDeviceID,&aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fLinkVoiceDeviceID);

	/*���Բ���*/
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
	
	/*�Է����ݳ�ʼ��*/
	strcpy(aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fCallInNumber,mLocalNumber);
	aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fLinkTrunkDeviceID=aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID;
	aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fLinkTrunkChannelNo=aryDspData[mDspNo].pTrunkDevice[mIndex].fChannelNo;
	aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fIsLinkTrunk=true;

	/*�޸ı�ͨ�����ݣ����Ż�����*/
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
���ܣ����ݽӿ��豸��ȫ�ֱ�š����������ڵ�index������±�
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
���ܣ�ʵ�����ߺ��У���Ӧ���ߵ��¼�
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
	/*����Ǻ��г�;*/
	if(isCallLong==true)
	{
		/*��������г�;*/
		if(aryDspData[mDspNo].pTrunkDevice[mIndex].fEnabledCallLong==false)
		{
			/*������ʾ��*/
			if(aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice==false)
			{
				SetLink(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
					&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID);
				aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice=true;
			}
			PlayFile(&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID,DIAL_LONG_ERROR_FILE,false);
			/*����ͨ����״̬����Ϊ�Ƿ����г�;״̬*/
			SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,VOC_ERROR_CALL_LONG);
			return;
		}
	}
	/*����һ�����õ�����ͨ��*/
	mTrunkIndex=FindFreeTrunkToUse();
	if(mTrunkIndex==-1)
	{
		/*û�п��е�����ͨ��*/
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
	/*��ʼ���к���*/
	rtCode=XMS_ctsMakeCallOut(pFlowHandle,&aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fDeviceID,
		mLocalNumber,mTargetNumber,NULL);
	if(rtCode<0)
	{
		/*����ʧ��*/
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
	/*�Ͽ���ͨ������������ͨ������*/
	mLinkVoiceIndex=FindIndexByChannelNo(mDspNo,aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceChannelNo,DEVICE_VOICE);
	mLinkVoiceDspNo=aryVoiceIndex[mLinkVoiceIndex].fDspNo;
	mLinkVoiceIndex=aryVoiceIndex[mLinkVoiceIndex].fIndex;
	DeleteLink(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
		&aryDspData[mLinkVoiceDspNo].pVoiceDevice[mLinkVoiceIndex].fDeviceID);
	aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice=false;
	aryDspData[mLinkVoiceDspNo].pVoiceDevice[mLinkVoiceIndex].fIsLinkTrunk=false;
	/*������ͨ��������ͨ��֮�佨��ʱ϶����*/
	SetLink(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
		&aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fDeviceID);
	/*�޸ı��ӿ�����*/
	aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkTrunk=true;
	aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkTrunkChannelNo=aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fChannelNo;
	aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkTrunkDeviceID=aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fDeviceID;
	aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice=false;
	/*�޸����ӵ�����ͨ���ӿ�����*/
	aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fIsLinkTrunk=true;
	aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fLinkTrunkChannelNo=aryDspData[mDspNo].pTrunkDevice[mIndex].fChannelNo;
	aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fLinkTrunkDeviceID=aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID;
	aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fIsLinkVoice=false;
	/*�޸�����ͨ����״̬*/
	SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,VOC_CALL_OUT);
	SetTrunkDeviceState(&aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fDeviceID,VOC_CALL_OUT);
	/*��������ת�����ߺ��еķ����¼�*/
	/*Ϊ���ܹ����յ����к��룬���Խ��м�ͨ������������ͨ������һ�����������*/
	XMS_ctsLinkDevice(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
		&aryDspData[mLinkVoiceDspNo].pVoiceDevice[mLinkVoiceIndex].fDeviceID,NULL);
	return;	
}


/***********************************************************************
���ܣ��жϺ������Ƿ��г�;������߷Ƿ��ĺ���
***********************************************************************/
bool	JudgePhoneNumIsValid(char * pPhoneNumber)
{
	return true;
}


/***********************************************************************
���ܣ���ָ��������ͨ����������ϵͳȫ�������ļ�����
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
	mProperty.m_u16PlayType=XMS_PLAY_TYPE_FILE;			/*�����ļ�����*/
	if(isStop)
		mProperty.m_u32StopMode=XMS_Any_Code;			/*����Ϊ�յ�����һ��dtmf����ֹͣ����*/
	mPlayTaskID=mPlayTaskID+1;
	if(mPlayTaskID>255)	mPlayTaskID=0;					/*ȫ�ֵĲ�������ID*/
	mProperty.m_u8TaskID=(DJ_U8)mPlayTaskID;
	strcpy(mProperty.m_s8PlayContent,pFileName);
	rtCode=XMS_ctsPlay(pFlowHandle,pDevice,&mProperty,NULL);
	if(rtCode<0)	return false;
	return true;
}

/***********************************************************************
����:VOIP�¼�����Ӧ�ʹ���
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
		/*�û�ժ���¼�----���δ��뻹����Ҫ�޸�*/
		{
			Acs_CallControl_Data		*pControlData=NULL;
			char		mCallerNum[20];						/*���к���*/
			char		mCallingNum[20];					/*���к���*/
			RetCode_t	rtCode;
			long		mTrunkIndex;
			pControlData=(Acs_CallControl_Data *)FetchEventData(pEvt);
			strcpy(mCallerNum,pControlData->m_s8CalledNum);		/*��ȡ���к���*/
			strcpy(mCallingNum,pControlData->m_s8CallingNum);	/*��ȡ���к���*/
			/*������ԣ�ֱ��ָ��һ��ͨ��*/
			if(mCallerNum[0]!='4')
			{
				mTrunkIndex=FindChannelNoByPhoneNumber(mCallerNum);
				if(mTrunkIndex==-1)
				{
					/*������еĺ��벻���ڣ���ֱ�Ӳ��ߣ���ʱ���޸ģ���æ��*/
					XMS_ctsClearCall(pFlowHandle,pDevice,0,NULL);
					ResetVoipDevice(pDevice,pEvt);
					//Sleep(3000);
					//aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fLineState=DCS_FREE;
					return;
				}
			}
			strcpy(aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fCallInNumber,mCallingNum);
			strcpy(aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fCallOutNumber,mCallerNum);
			/*Ӧ��ú���*/
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
			/*�ҵ���Ӧ������ͨ������Ը��м�ͨ���������*/
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
			//����IP��������
			if(aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fCallOutNumber[0]=='4')
			{
				unsigned int	i;
				for(i=0;i<strlen(aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fCallOutNumber);i++)
					aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fCallOutNumber[i]=aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fCallOutNumber[i+1];
				aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fCallOutNumber[i]='\0';
				CallTrunkByVoip(mDspNo,mDeviceIndex);
				return;
			}
			/*���ݱ��к��룬��������ͨ��*/
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
			/*��VOIPͨ��������Ӧ��VOICE�豸��*/
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

			/*������ͨ���������*/
			/*��VOIPͨ����Ϣд������ͨ�����Ա����в��ɹ������в���*/
			aryDspData[mTrunkDspNo].pTrunkDevice[mTrunkIndex].fLinkVoipDeviceNo=aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fChannelNo;
			aryDspData[mTrunkDspNo].pTrunkDevice[mTrunkIndex].fLinkVoipDeviceID=aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fDeviceID;
			CallUserByVoip(mTrunkDspNo,mTrunkIndex);
			/*���ø�ͨ���Ż�����*/
			PlayTone(&aryDspData[mLinkVoipVoiceDspNo].pVoiceDevice[mLinkVoipVoiceIndex].fDeviceID,BACK_TONE);
		}
		break;
	case	XMS_EVT_CALLOUT:
		/*�����¼�*/
		{
			Acs_CallControl_Data	*pControlData=NULL;
			long					mLinkUserDspNo;
			long					mLinkUserDeviceIndex;
			long					mLinkUserDeviceNo;


			pControlData=(Acs_CallControl_Data *)FetchEventData(pEvt);
			/*��ȡ��ͨ���Ļ�������*/
			mChannelID=pDevice->m_s16ChannelID;
			mDspNo=pDevice->m_s8ModuleID;
			mIndex=FindIndexByChannelID(mDspNo,mChannelID,DEVICE_VOIP);
			mDeviceIndex=aryVoipIndex[mIndex].fIndex;
			mLinkChannelNo=aryDspData[mDspNo].pVoipDevice[mDeviceIndex].fLinkVoiceChannelNo;
			mLinkDeviceIndex=FindIndexByChannelNo(mDspNo,mLinkChannelNo,DEVICE_VOICE);
			mLinkDspNo=aryVoiceIndex[mLinkDeviceIndex].fDspNo;
			mLinkDeviceIndex=aryVoiceIndex[mLinkDeviceIndex].fIndex;
			/*ͨ����VOIP����������ͨ�����ҵ����Ӧ�Ľӿ�ͨ��*/
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
				/*���гɹ�*/
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
				/*����ʧ��*/
				/*�Ͽ��ӿ��豸��voip�������豸֮�������*/
				DeleteLink(&aryDspData[mLinkUserDspNo].pTrunkDevice[mLinkUserDeviceIndex].fDeviceID,
						   &aryDspData[mLinkDspNo].pVoiceDevice[mLinkDeviceIndex].fDeviceID);
				aryDspData[mLinkDspNo].pVoiceDevice[mLinkDeviceIndex].fIsLinkTrunk=false;
				aryDspData[mLinkDspNo].pVoiceDevice[mLinkDeviceIndex].fLinkTrunkChannelNo=-1;

				/*�ӿ�ͨ����æ��*/
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
		/*���������¼�*/
		break;
	case	XMS_EVT_UNLINKDEVICE:
		/*��������¼�*/
		break;
	case	XMS_EVT_SENDIODATA:
		/*���������¼�*/
		break;
	case	XMS_EVT_PLAY:
		/*�������*/

		break;
	case	XMS_EVT_RECVIODATA:				
		/*���������¼�--DTMF*/
		break;
	}
	return;
}


/***********************************************************************
���ܣ�����һ�����õ�VOIPͨ�������û�п��еģ�����-1������ȷ���ҵ�����ͬһ��DSP������ͨ��
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
���ܣ�ʵ������VOIP���к��У���������VOIP�¼���
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

	/*����һ�����õ�VOIPͨ��*/
	mVoipIndex=FindFreeVoipToUse(pVoipDspNo);
//	mVoipIndex=59;
	if(mVoipIndex==-1)
	{
		/*û�п��е�VOIPͨ�����к���*/
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

	/*���ñ�����Ϣ*/
	pCallParam.m_PrivateData.m_u32DataSize=sizeof(VoIPCallPrivate_t);
	pCallParam.m_s32Protocol=aryDspData[mTargetDspNo].pVoipDevice[mTargetDeviceIdIndex].fProtocolType;
	//����323Э�鲻��Ҫ��дIP��ַ�Լ��˿�
	if(pCallParam.m_s32Protocol==2)
	{
		pCallParam.m_s32CalleePort=aryDspData[mTargetDspNo].pVoipDevice[mTargetDeviceIdIndex].fTargetAddr.port;
		strcpy(pCallParam.m_s8CalleeAddress,aryDspData[mTargetDspNo].pVoipDevice[mTargetDeviceIdIndex].fTargetAddr.tszAddr);
		strcpy(pCallParam.m_s8CalleeUserID,aryDspData[mTargetDspNo].pVoipDevice[mTargetDeviceIdIndex].fTargetAddr.tszUserID);
	}
	/*����������Ϣ*/
	strcpy(pCallParam.m_s8CallerAddress,aryDspData[mTargetDspNo].pVoipDevice[mTargetDeviceIdIndex].fSourceAddr.tszAddr);
	pCallParam.m_s32CallerPort=aryDspData[mTargetDspNo].pVoipDevice[mTargetDeviceIdIndex].fSourceAddr.port;
	strcpy(pCallParam.m_s8CallerUserID,aryDspData[mTargetDspNo].pVoipDevice[mTargetDeviceIdIndex].fSourceAddr.tszUserID);
	/*��ʼ���к���*/

	rtCode=XMS_ctsMakeCallOut(pFlowHandle,&aryDspData[mTargetDspNo].pVoipDevice[mTargetDeviceIdIndex].fDeviceID,
		mLocalNumber,mTargetNumber,(PrivateData_t *)&pCallParam);
	if(rtCode<0)
	{
		/*����ʧ��*/
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
	/*�Ͽ��û�ͨ����������������ͨ��*/
	mLinkVoiceIndex=FindIndexByChannelNo(mDspNo,aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceChannelNo,DEVICE_VOICE);
	mLinkVoiceDspNo=aryVoiceIndex[mLinkVoiceIndex].fDspNo;
	mLinkVoiceIndex=aryVoiceIndex[mLinkVoiceIndex].fIndex;
	if(aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice==true)
	{
		DeleteLink(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
			&aryDspData[mLinkVoiceDspNo].pVoiceDevice[mLinkVoiceIndex].fDeviceID);
		aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice=false;
	}

	/*��voip�豸���������������豸���а�*/
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

	/*���ӿ�ͨ����VOIP�󶨵�����ͨ������˫���ʱ϶����*/
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

	/*�Ż�����*/
	PlayFileCircly(&aryDspData[mLinkVoiceDspNo].pVoiceDevice[mLinkVoiceIndex].fDeviceID,BACK_SIGN_MUSIC,false);
	//PlayTone(&aryDspData[mLinkVoiceDspNo].pVoiceDevice[mLinkVoiceIndex].fDeviceID,BACK_TONE);


	/*�޸�����ͨ����״̬*/
	SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,VOC_CALL_OUT);
	SetVoipDeviceState(&aryDspData[mTargetDspNo].pVoipDevice[mTargetDeviceIdIndex].fDeviceID,VOIP_CALL_VOIP);
	return;	
}


/***********************************************************************
���ܣ���λVOIP�豸
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
	/*��VOIP����������ͨ����æ��*/
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
	//��������˽ӿ�
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
	/*��ͨ�����ݳ�ʼ��*/
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
���ܣ�ʵ�ֶ�VOIP������ϯ��Ӧ��
ע�⣺mdspno,mindex:����ʾ�����Ǹ���ϯ
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

	/*�Է���·Ϊ�ǿ���״̬*/
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

	/*ADD BY TIM,���Ӷ�user�豸�������е�����*/ 
	SetUserDeviceParam(&mTargetDevice,&aryDspData[mDspNo].pTrunkDevice[mTargetDeviceIdIndex].fLinkVoiceDeviceID);

	/*���Բ���*/
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

	/*��дVOIPͨ������*/
	mVoipDspNo=-1;
	mVoipDeviceNo=aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoipDeviceNo;
	mVoipDeviceIndex=FindIndexByChannelNo(mVoipDspNo,mVoipDeviceNo,DEVICE_VOIP);
	mVoipDspNo=aryVoipIndex[mVoipDeviceIndex].fDspNo;
	mVoipDeviceIndex=aryVoipIndex[mVoipDeviceIndex].fIndex;
	aryDspData[mVoipDspNo].pVoipDevice[mVoipDeviceIndex].fLinkTrunkChannelID=aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID;
	aryDspData[mVoipDspNo].pVoipDevice[mVoipDeviceIndex].fLinkTrunkChannelNo=aryDspData[mDspNo].pTrunkDevice[mIndex].fChannelNo;
	aryDspData[mVoipDspNo].pVoipDevice[mVoipDeviceIndex].fLinkTrunkLineType=aryDspData[mDspNo].pTrunkDevice[mIndex].fLineType;
	aryDspData[mVoipDspNo].pVoipDevice[mVoipDeviceIndex].fIsLinkTrunk=false;

	/*�˴������Ҫ*/
	aryDspData[mDspNo].pTrunkDevice[mIndex].fisLinkVoipVoice=true;
	SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,VOC_CALL_IN);
	return;
}


/***********************************************************************
���ܣ�ʵ�����ߺ�������
***********************************************************************/
void	CallUserByTrunk(long mDspNo,long mIndex)
{
	char	mLocalNumber[20];
	char	mTargetNumber[20];
	long	mTargetChannelNo;		/*ȫ���豸���*/
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
		/*���еĺ��벻����*/
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
	
	/*�������·����ֹ����Ҳֱ�ӷ�æ��*/
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
	/*�Է���·Ϊ�ǿ���״̬*/
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

	/*ADD BY TIM,���Ӷ�user�豸�������е�����*/ 
	SetUserDeviceParam(&mTargetDevice,&aryDspData[mDspNo].pTrunkDevice[mTargetDeviceIdIndex].fLinkVoiceDeviceID);

	/*���Բ���*/
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
	
	/*�Է����ݳ�ʼ��*/
	strcpy(aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fCallInNumber,mLocalNumber);
	aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fLinkTrunkDeviceID=aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID;
	aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fLinkTrunkChannelNo=aryDspData[mDspNo].pTrunkDevice[mIndex].fChannelNo;
	aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fIsLinkTrunk=true;

	/*�޸ı�ͨ�����ݣ����Ż�����*/
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
���ܣ������û�ָ����ͨ���ţ��Զ�������Ӧ�ķ���ĺ����ļ�
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

	/*��ʼ���з�������*/	
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
	mPlayProperty.m_u16PlayType=XMS_PLAY_TYPE_FILE_QUEUE;	/*���������ļ�����*/
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
		mPlayProperty.m_u16PlayType=XMS_PLAY_TYPE_FILE_QUEUE;	/*���������ļ�����*/
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
���ܣ���ѯ��;�Ƿ�ͨ����
***********************************************************************/
bool	QueryCallLongStatus(DJ_S32	mDspNo,long	mIndex)
{
	
	if(aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice==false)
	{
		SetLink(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
			&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID);
		aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice=true;
	}
	/*��������г�;*/
	if(aryDspData[mDspNo].pTrunkDevice[mIndex].fEnabledCallLong==false)
	{
		PlayFile(&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID,NOT_HAVE_LONG_NOTICE_VOC,false);
		/*����ͨ����״̬����Ϊ�Ƿ����г�;״̬*/
		SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,VOC_PLAY_NOTICE);
	}
	/*������г�;*/
	if(aryDspData[mDspNo].pTrunkDevice[mIndex].fEnabledCallLong==true)
	{
		PlayFile(&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID,HAVE_LONG_NOTICE_VOC,false);
		/*����ͨ����״̬����Ϊ�Ƿ����г�;״̬*/
		SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,VOC_PLAY_NOTICE);
	}
	return true;
}

/***********************************************************************
���ܣ���ͨ��;����
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
	strcpy(mSQL,"update objUserLine set flongenable='��ͨ' where fdspno='" );
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
	/*��ʼ��������*/
	if(aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice==false)
	{
		SetLink(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
			&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID);
		aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice=true;
	}

	memset((PlayProperty_t *)&mPlayProperty,0,sizeof(PlayProperty_t));
	mPlayProperty.m_u8TaskID=(DJ_U8)mPlayTaskID;
	strcpy(mPlayProperty.m_s8PlayContent,SETTING_FINISH_VOC);
	mPlayProperty.m_u16PlayType=XMS_PLAY_TYPE_FILE_QUEUE;	/*���������ļ�����*/
	rtCode=XMS_ctsPlay(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID,
		&mPlayProperty,NULL);
	if(rtCode<0)	return false;

	mPlayProperty.m_u8TaskID=(DJ_U8)mPlayTaskID;
	strcpy(mPlayProperty.m_s8PlayContent,HAVE_LONG_NOTICE_VOC);
	mPlayProperty.m_u16PlayType=XMS_PLAY_TYPE_FILE_QUEUE;	/*���������ļ�����*/
	rtCode=XMS_ctsPlay(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID,
		&mPlayProperty,NULL);
	if(rtCode<0)	return false;
	/*����ͨ����״̬����Ϊ�Ƿ����г�;״̬*/
	SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,VOC_PLAY_NOTICE);
	return true;
}

/***********************************************************************
���ܣ���ֹ��;����
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
	strcpy(mSQL,"update objUserLine set flongenable='��ֹ' where fdspno='" );
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
	/*��ʼ��������*/
	if(aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice==false)
	{
		SetLink(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
			&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID);
		aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice=true;
	}

	memset((PlayProperty_t *)&mPlayProperty,0,sizeof(PlayProperty_t));
	mPlayProperty.m_u8TaskID=(DJ_U8)mPlayTaskID;
	strcpy(mPlayProperty.m_s8PlayContent,SETTING_FINISH_VOC);
	mPlayProperty.m_u16PlayType=XMS_PLAY_TYPE_FILE_QUEUE;	/*���������ļ�����*/
	rtCode=XMS_ctsPlay(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID,
		&mPlayProperty,NULL);
	if(rtCode<0)	return false;

	mPlayProperty.m_u8TaskID=(DJ_U8)mPlayTaskID;
	strcpy(mPlayProperty.m_s8PlayContent,NOT_HAVE_LONG_NOTICE_VOC);
	mPlayProperty.m_u16PlayType=XMS_PLAY_TYPE_FILE_QUEUE;	/*���������ļ�����*/
	rtCode=XMS_ctsPlay(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID,
		&mPlayProperty,NULL);
	if(rtCode<0)	return false;
	/*����ͨ����״̬����Ϊ�Ƿ����г�;״̬*/
	SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,VOC_PLAY_NOTICE);
	return true;
}


/***********************************************************************
���ܣ����Ų˵�����
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
	/*����ͨ����״̬����Ϊ�Ƿ����г�;״̬*/
	SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,VOC_PLAY_NOTICE);
	return true;
}


/***********************************************************************
���ܣ������绰
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
	strcpy(mSQL,"update objUserLine set fisenable='����' where fdspno='" );
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
	/*��ʼ��������*/
	if(aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice==false)
	{
		SetLink(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
			&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID);
		aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice=true;
	}
	memset((PlayProperty_t *)&mPlayProperty,0,sizeof(PlayProperty_t));
	mPlayProperty.m_u8TaskID=(DJ_U8)mPlayTaskID;
	strcpy(mPlayProperty.m_s8PlayContent,SETTING_FINISH_VOC);
	mPlayProperty.m_u16PlayType=XMS_PLAY_TYPE_FILE_QUEUE;	/*���������ļ�����*/
	rtCode=XMS_ctsPlay(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID,
		&mPlayProperty,NULL);
	if(rtCode<0)	return false;
	mPlayProperty.m_u8TaskID=(DJ_U8)mPlayTaskID;
	strcpy(mPlayProperty.m_s8PlayContent,ENABLE_THE_CALL_VOC);
	mPlayProperty.m_u16PlayType=XMS_PLAY_TYPE_FILE_QUEUE;	/*���������ļ�����*/
	rtCode=XMS_ctsPlay(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID,
		&mPlayProperty,NULL);
	if(rtCode<0)	return false;
	/*����ͨ����״̬����Ϊ�Ƿ����г�;״̬*/
	SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,VOC_PLAY_NOTICE);
	return true;
}


/***********************************************************************
���ܣ��رյ绰
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
	strcpy(mSQL,"update objUserLine set fisenable='��ֹ' where fdspno='" );
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
	/*��ʼ��������*/
	if(aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice==false)
	{
		SetLink(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
			&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID);
		aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice=true;
	}
	memset((PlayProperty_t *)&mPlayProperty,0,sizeof(PlayProperty_t));
	mPlayProperty.m_u8TaskID=(DJ_U8)mPlayTaskID;
	strcpy(mPlayProperty.m_s8PlayContent,SETTING_FINISH_VOC);
	mPlayProperty.m_u16PlayType=XMS_PLAY_TYPE_FILE_QUEUE;	/*���������ļ�����*/
	rtCode=XMS_ctsPlay(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID,
		&mPlayProperty,NULL);
	if(rtCode<0)	return false;
	mPlayProperty.m_u8TaskID=(DJ_U8)mPlayTaskID;
	strcpy(mPlayProperty.m_s8PlayContent,DISABLE_THE_CALL_VOC);
	mPlayProperty.m_u16PlayType=XMS_PLAY_TYPE_FILE_QUEUE;	/*���������ļ�����*/
	rtCode=XMS_ctsPlay(pFlowHandle,&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID,
		&mPlayProperty,NULL);
	if(rtCode<0)	return false;
	/*����ͨ����״̬����Ϊ�Ƿ����г�;״̬*/
	SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,VOC_PLAY_NOTICE);
	return true;
}


/***********************************************************************
���ܣ��Ĳ�ɴ���
***********************************************************************/
void	ChaHuanProcess(DJ_S32	mDspNo,long mIndex)
{
	DJ_S32		mLinkDspNo;
	long		mLinkIndex;
	long		mLinkDeviceIndex;
	long		mLinkChannelNo;

	/*��һ���Ĳ�ɵĴ���*/
	if(aryDspData[mDspNo].pTrunkDevice[mIndex].fChaHuanCount==1)
	{
		/*�����ǰ���ӵ�����ϯ*/
		if(aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkTrunk==true)
		{ 
			/*����ǰ���ӵ�ͨ����������*/
			aryDspData[mDspNo].pTrunkDevice[mIndex].fPrevTrunkChannelNo=
				aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkTrunkChannelNo;
			aryDspData[mDspNo].pTrunkDevice[mIndex].fPrevTrunkDeviceID=
				aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkTrunkDeviceID;
			aryDspData[mDspNo].pTrunkDevice[mIndex].fPrevDeviceType=1;			/*��ʾ���ӵ���Trunk*/
			
			/*�Ͽ�ԭͨ����ͨ��֮�������*/
			DeleteLink(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
				&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkTrunkDeviceID);
			/*��ͨ�����������������ӣ����Ų�����*/
			if(aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice==false)
			{
				SetLink(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
					&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID);
				aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice=true;
			}
			PlayTone(&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID,DIAL_TONE);
			/*��ͨ����״̬����Ϊ�Ĳ��״̬*/
			SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,VOC_FLASH_WAIT_DIAL);
			/*�����ӵĽӿ�ͨ����������������Ӧ������ͨ����������*/
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
			/*��������ת������*/
			PlayFile(&aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fLinkVoiceDeviceID,BACKGROUND_MUSIC,false);
			/*�ȴ���ͨ�����ż���*/
			aryDspData[mDspNo].pTrunkDevice[mIndex].fChaHuanIsReset=false;
			aryDspData[mDspNo].pTrunkDevice[mIndex].fChaHuanIsCallStatus=false;
		}
		/*�����ǰ���ӵ���VOIP*/
		if(aryDspData[mDspNo].pTrunkDevice[mIndex].fisLinkVoipVoice==true)
		{
			long		mLinkVoipVoiceNo;
			DJ_S32		mLinkVoipVoiceDspNo;
			long		mLinkVoipVoiceIndex;

			/*����ǰ���ӵ�ͨ����������*/
			aryDspData[mDspNo].pTrunkDevice[mIndex].fPrevVoipChannelNo=
				aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoipVoiceDeviceNo;
			aryDspData[mDspNo].pTrunkDevice[mIndex].fPrevVoipDeviceID=
				aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoipVoiceDeviceID;
			aryDspData[mDspNo].pTrunkDevice[mIndex].fPrevDeviceType=2;			/*��ʾΪVOIP*/
			/*��ȡ�����ӵ�VOIPͨ����Ϣ*/
			mLinkVoipVoiceNo=aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoipVoiceDeviceNo;
			mLinkVoipVoiceDspNo=-1;
			mLinkVoipVoiceIndex=FindIndexByChannelNo(mLinkVoipVoiceDspNo,mLinkVoipVoiceNo,DEVICE_VOICE);
			mLinkVoipVoiceDspNo=aryVoiceIndex[mLinkVoipVoiceIndex].fDspNo;
			mLinkVoipVoiceIndex=aryVoiceIndex[mLinkVoipVoiceIndex].fIndex;
			/*VOIP�豸��ȫ�ֱ��*/
			mLinkChannelNo=aryDspData[mLinkVoipVoiceDspNo].pVoiceDevice[mLinkVoipVoiceIndex].fLinkVoipChannelNo;
			mLinkDspNo=-1;
			mLinkDeviceIndex=FindIndexByChannelNo(mLinkDspNo,mLinkChannelNo,DEVICE_VOIP);
			mLinkDspNo=aryVoipIndex[mLinkDeviceIndex].fDspNo;
			mLinkDeviceIndex=aryVoipIndex[mLinkDeviceIndex].fIndex;
			/*�Ͽ����м�ͨ����VOIPͨ��֮�������*/
			DeleteLink(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
				&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoipVoiceDeviceID);
			/*��ͨ������Ų�����*/
			if(aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice==false)
			{
				SetLink(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,
					&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID);
				aryDspData[mDspNo].pTrunkDevice[mIndex].fIsLinkVoice=true;
			}
			PlayTone(&aryDspData[mDspNo].pTrunkDevice[mIndex].fLinkVoiceDeviceID,DIAL_TONE);
			SetTrunkDeviceState(&aryDspData[mDspNo].pTrunkDevice[mIndex].fDeviceID,VOC_FLASH_WAIT_DIAL);
			/*��VOIPͨ����ת����*/
			if(aryDspData[mLinkDspNo].pVoipDevice[mLinkDeviceIndex].fIsLinkVoice==false)
			{
				XMS_ctsLinkDevice(pFlowHandle,&aryDspData[mLinkDspNo].pVoipDevice[mLinkDeviceIndex].fDeviceID,
					&aryDspData[mLinkDspNo].pVoipDevice[mLinkDeviceIndex].fLinkVoiceDeviceID,NULL);
			}
			/*����VOIPͨ���Ļ���*/
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

	/*�ڶ����Ĳ�ɣ����Ļ���*/
	if(aryDspData[mDspNo].pTrunkDevice[mIndex].fChaHuanCount==2)
	{
		DJ_S32		mLinkDspNo;
		long		mLinkDeviceIndex;
		/*�ӻ�ԭ��ͨ��*/
		mLinkDspNo=mDspNo;
		mLinkDeviceIndex=mIndex;

		if(aryDspData[mLinkDspNo].pTrunkDevice[mLinkDeviceIndex].fPrevDeviceType==1)
		{	
			/*ԭͨ��Ϊ�ӿ��豸*/
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
			AddMsg("�Ķ���ֹͣ����");
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
			/*ԭͨ��ΪVOIP�豸*/
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
			AddMsg("�ĵڶ���ֹͣ����2");
			/*������ͨ����ӿ�ͨ�����а�*/
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
		/*���Ĳ���������*/
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

/*����Ϣ��ʾ������ʾ �¼����͡�DSPģ��š�ͨ����*/
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
���ܣ�����һ�����õ����ӻ���ͨ���ߺ��������
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
���ܣ������ٲ��Ų������ڵ��豸ȫ��ͨ����
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
/* ����IP��������                                                                     */
/************************************************************************/
void	CallTrunkByVoip(long mDspNo,long mIndex)
{
	//ע�⣺�ú���û��ʵ�ֻ���ͨ
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

	/*����һ�����õ�����ͨ��*/
	mTrunkIndex=FindFreeTrunkToUse();
	if(mTrunkIndex==-1)
	{
		/*û�п��е�����ͨ��*/
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
	
	/*��ʼ���к���*/
	rtCode=XMS_ctsMakeCallOut(pFlowHandle,&aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fDeviceID,
		mLocalNumber,mTargetNumber,NULL);
	Sleep(5000);

	if(rtCode<0)
	{
		/*����ʧ��*/
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


	/*��ip�����������߽���ʱ϶����*/
	XMS_ctsLinkDevice(pFlowHandle,&aryDspData[mDspNo].pVoipDevice[mIndex].fDeviceID,
			&aryDspData[mDspNo].pVoipDevice[mIndex].fLinkVoiceDeviceID,NULL);
	//Sleep(1000);
	SetLink(&aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fDeviceID,
		    &aryDspData[mDspNo].pVoipDevice[mIndex].fLinkVoiceDeviceID);

	/*�޸����߽ӿ�����*/
	aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fisLinkVoipVoice=true;
	aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fLinkVoipVoiceDeviceID=
		aryDspData[mDspNo].pVoipDevice[mIndex].fLinkVoiceDeviceID;
	aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fLinkVoipVoiceDeviceNo=
		aryDspData[mDspNo].pVoipDevice[mIndex].fLinkVoiceChannelNo;
	aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fLinkVoipDeviceID=aryDspData[mDspNo].pVoipDevice[mIndex].fDeviceID;
	aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fLinkVoipDeviceNo=aryDspData[mDspNo].pVoipDevice[mIndex].fChannelNo;

	/*�޸ı��ӿ�����*/
	aryDspData[mDspNo].pVoipDevice[mIndex].fIsLinkTrunk=true;
	aryDspData[mDspNo].pVoipDevice[mIndex].fLinkTrunkChannelNo=aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fChannelNo;
	aryDspData[mDspNo].pVoipDevice[mIndex].fLinkTrunkChannelID=aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fDeviceID;

	SetVoipDeviceState(&aryDspData[mDspNo].pVoipDevice[mIndex].fDeviceID,VOIP_CALL_VOIP);
	SetTrunkDeviceState(&aryDspData[mTargetDspNo].pTrunkDevice[mTargetDeviceIdIndex].fDeviceID,VOC_CALL_OUT);
	return;
}


bool	PlayFileCircly(DeviceID_t *pDevice,DJ_S8 *pFileName,bool isStop)					/*�ļ�����*/
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
	mProperty.m_u16PlayType=XMS_PLAY_TYPE_FILE;			/*�����ļ�����*/
	if(isStop)
		mProperty.m_u32StopMode=XMS_Any_Code;			/*����Ϊ�յ�����һ��dtmf����ֹͣ����*/
	mProperty.m_u32MaxTime=999999;
	mPlayTaskID=mPlayTaskID+1;
	if(mPlayTaskID>255)	mPlayTaskID=0;					/*ȫ�ֵĲ�������ID*/
	mProperty.m_u8TaskID=(DJ_U8)mPlayTaskID;
	strcpy(mProperty.m_s8PlayContent,pFileName);
	rtCode=XMS_ctsPlay(pFlowHandle,pDevice,&mProperty,NULL);
	if(rtCode<0)	return false;
	return true;

}
