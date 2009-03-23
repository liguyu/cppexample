// XMSApi_SendIOData.cpp : implementation file
//

#include "stdafx.h"
#include "XMSApi_Test.h"
#include "XMSApi_SendIOData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"

#include "XMSApi_Test_String.h"
#include "XMSApi_Test_Event.h"
#include "XMSApi_Test_Sub.h"
#include "XMSApi_TestDlg.h"

extern	int		iOpenedIndex1;

extern	TYPE_MY_DEVICE_STRUCT	DevOpened[MAX_DEVICE_ALLOW_OPEN];

extern ACSHandle_t		g_acsHandle;
extern CXMSApi_TestDlg	*pdlg;
/////////////////////////////////////////////////////////////////////////////
// CXMSApi_SendIOData dialog


CXMSApi_SendIOData::CXMSApi_SendIOData(CWnd* pParent /*=NULL*/)
	: CDialog(CXMSApi_SendIOData::IDD, pParent)
{
	//{{AFX_DATA_INIT(CXMSApi_SendIOData)
	m_tick = 30;
	//}}AFX_DATA_INIT
}


void CXMSApi_SendIOData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXMSApi_SendIOData)
	DDX_Control(pDX, IDC_COMBO1, m_ComboType);
	DDX_Text(pDX, IDC_EDIT1, m_tick);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CXMSApi_SendIOData, CDialog)
	//{{AFX_MSG_MAP(CXMSApi_SendIOData)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_WM_CLOSE()

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXMSApi_SendIOData message handlers
typedef struct 
{
	DJ_U16		u16IOType;
	char		TypeStr[32];
} TYPE_MY_SEND_IO_DATA;

#define	MAX_IO_TYPE_NUM		12

TYPE_MY_SEND_IO_DATA IoDataStru[MAX_IO_TYPE_NUM] =
{
	{XMS_IO_TYPE_DTMF,			"IO_TYPE_DTMF"},
	{XMS_IO_TYPE_MR2F,			"IO_TYPE_MR2F"},
	{XMS_IO_TYPE_MR2B,			"IO_TYPE_MR2B"},
	{XMS_IO_TYPE_GTG,			"IO_TYPE_GTG"},
	{XMS_IO_TYPE_FSK,			"IO_TYPE_FSK"},
	{XMS_IO_TYPE_VOIP_H245UII,	"IO_TYPE_VOIP_H245UII"},
	{XMS_IO_TYPE_VOIP_SIPINFO,	"IO_TYPE_VOIP_SIPINFO"},
	{XMS_IO_TYPE_VOIP_RTPDTMF,	"IO_TYPE_VOIP_RTPDTMF"},
	{XMS_IO_TYPE_DCH_I_FRAME,	"IO_TYPE_DCH_I_FRAME"},
	{XMS_IO_TYPE_DCH_UI_FRAME,	"IO_TYPE_DCH_UI_FRAME"},
	{XMS_IO_TYPE_DCH_DEBUG,		"IO_TYPE_DCH_DEBUG"},
//	{XMS_IO_TYPE_USERDEF,		"IO_TYPE_USERDEF"}
};


int g_iLen = 0;
char g_TmpStr[1024];

BOOL CXMSApi_SendIOData::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//char	TmpStr[256];

	m_Run = 1;
	m_tick=30;

	int		i;

	GetDlgItem ( IDC_EDIT_DevcieID )->SetWindowText ( GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID) );

	for ( i = 0; i < MAX_IO_TYPE_NUM; i ++ )
	{
		m_ComboType.AddString ( IoDataStru[i].TypeStr );
	}
	m_ComboType.SetCurSel ( 0 );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

DWORD PressSendIO(LPVOID lp)
{
	RetCode_t			r;
	int                 i = 0;
	int					iLen;
	int					iIndex;
	char				TmpStr[256];
	char				MsgStr[160];
	int                 nCount = 0;
	CXMSApi_SendIOData * ioDlg = NULL;

	ioDlg = (CXMSApi_SendIOData *)lp;
	// m_u16IoType
	iIndex = ioDlg->m_ComboType.GetCurSel ();

	// m_u16IoLen	
	GetWindowText (GetDlgItem (ioDlg->m_hWnd,IDC_EDIT_u16IoLen), TmpStr, 32 );
	sscanf ( TmpStr, "%d", &iLen );

	GetWindowText(GetDlgItem ( ioDlg->m_hWnd,IDC_EDIT_DataContent ), TmpStr, 256 );
	nCount = pdlg->m_ListDevOpened.GetCount();			
	
	while(ioDlg->m_Run)
	{
		for(i=0;i<nCount;i++)
		{
			for(int j=0;j<4;j++)
			{
				r = XMS_ctsSendIOData ( g_acsHandle, &DevOpened[i].DevID, 
					IoDataStru[iIndex].u16IOType, (DJ_U16)iLen, (DJ_Void *) TmpStr );
				if ( r < 0 )
				{
					sprintf ( MsgStr, "X(%d) XMS_ctsSendIOData( %s, %d, [%s] ) FAIL! (%s)", 
						r, IoDataStru[iIndex].TypeStr, iLen, TmpStr, 
						GetString_DeviceAll (&DevOpened[i].DevID)  );
					AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
				}
				else
				{
					sprintf ( MsgStr, "XMS_ctsSendIOData( %s, %d, [%s] ) OK! (%s)", 
						IoDataStru[iIndex].TypeStr, iLen, TmpStr,  
						GetString_DeviceAll (&DevOpened[i].DevID)  );
					AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
				}
			}		
		}

		Sleep(ioDlg->m_tick);
	}

	return 1;
}

void CXMSApi_SendIOData::OnButton1() 
{
	// TODO: Add your control notification handler code here
	RetCode_t			r;
	int                 i = 0;
	int					iLen;
	int					iIndex;
	char				TmpStr[1024];
	char				MsgStr[1024];

	// m_u16IoType
	iIndex = m_ComboType.GetCurSel ();

	// m_u16IoLen
	GetDlgItem ( IDC_EDIT_u16IoLen )->GetWindowText ( TmpStr, 32 );
	sscanf ( TmpStr, "%d", &iLen );

	GetDlgItem ( IDC_EDIT_DataContent )->GetWindowText ( TmpStr, 256 );

	if(DevOpened[iOpenedIndex1].DevID.m_s16DeviceMain == XMS_DEVMAIN_INTERFACE_CH
		&& DevOpened[iOpenedIndex1].DevID.m_s16DeviceSub == XMS_DEVSUB_E1_DCH)
	{
		UpdateData(TRUE);

		m_Run = 1;
		m_hThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)PressSendIO,
			(LPVOID)this,0,NULL);
		CloseHandle(m_hThread);
	}
	else
	{
		/*{XMS_IO_TYPE_DTMF,			"IO_TYPE_DTMF"},
	{XMS_IO_TYPE_MR2F,			"IO_TYPE_MR2F"},
	{XMS_IO_TYPE_MR2B,			"IO_TYPE_MR2B"},
	{XMS_IO_TYPE_GTG,			"IO_TYPE_GTG"},
	{XMS_IO_TYPE_FSK,			"IO_TYPE_FSK"},
	{XMS_IO_TYPE_VOIP_H245UII,	"IO_TYPE_VOIP_H245UII"},
	{XMS_IO_TYPE_VOIP_SIPINFO,	"IO_TYPE_VOIP_SIPINFO"},
	{XMS_IO_TYPE_VOIP_RTPDTMF,	"IO_TYPE_VOIP_RTPDTMF"},
	{XMS_IO_TYPE_DCH_I_FRAME,	"IO_TYPE_DCH_I_FRAME"},
	{XMS_IO_TYPE_DCH_UI_FRAME,	"IO_TYPE_DCH_UI_FRAME"},
	{XMS_IO_TYPE_DCH_DEBUG,		"IO_TYPE_DCH_DEBUG"},*/

		switch(iIndex)
		{
		case 0://IO_TYPE_DTMF
		case 1://IO_TYPE_MR2F
		case 2://IO_TYPE_MR2B
		case 3://IO_TYPE_GTG
		case 4://IO_TYPE_FSK

		case 8://IO_TYPE_DCH_I_FRAME
		case 9://IO_TYPE_DCH_UI_FRAME
		case 10://IO_TYPE_DCH_DEBUG
		{

			r = XMS_ctsSendIOData ( g_acsHandle, &DevOpened[iOpenedIndex1].DevID, 
			IoDataStru[iIndex].u16IOType, (DJ_U16)iLen, (DJ_Void *) TmpStr );
			if ( r < 0 )
			{
				sprintf ( MsgStr, "X(%d) XMS_ctsSendIOData( %s, %d, [%s] ) FAIL! (%s)", 
					r, IoDataStru[iIndex].TypeStr, iLen, TmpStr, 
					GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID)  );
				AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
			}
			else
			{
				sprintf ( MsgStr, "XMS_ctsSendIOData( %s, %d, [%s] ) OK! (%s)", 
					IoDataStru[iIndex].TypeStr, iLen, TmpStr,  
					GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID)  );
				AddMsg ( MSG_TYPE_FUNCTION, MsgStr );


				//-sendfaskcaller
				//int iSendFskCaller =((CButton *)GetDlgItem ( IDC_CHECK_SENDFSKCALLER ))->GetCheck();
				//if( iSendFskCaller)
				//{
					//Set Timer 2000ms
					//r = XMS_ctsSetDevTimer(g_acsHandle,&DevOpened[iOpenedIndex1].DevID,2000);
				//}
			}
		}
		break;

		case 5://IO_TYPE_VOIP_H245UII
		{
			if ( DevOpened[iOpenedIndex1].DevID.m_s16DeviceMain != XMS_DEVMAIN_VOIP)
			{
				sprintf ( MsgStr, "XMS_ctsSendIOData( %s, %d, [%s] ) Failed! (%s)--For VoIP Only", 
					IoDataStru[iIndex].TypeStr, iLen, TmpStr,  
					GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID)  );
				AddMsg ( MSG_TYPE_FUNCTION, MsgStr );

				return;
			}

			Acs_VoIP_H245UII h245Uii = {0};
			h245Uii.m_s8Signal = TmpStr[0]; // dtmf char in "0123456789#*ABCD!"
			h245Uii.m_u32Duration = 200; // milliseconds

			r = XMS_ctsSendIOData ( g_acsHandle, &DevOpened[iOpenedIndex1].DevID, 
			IoDataStru[iIndex].u16IOType, sizeof(h245Uii), &h245Uii );
			if ( r < 0 )
			{
				sprintf ( MsgStr, "X(%d) XMS_ctsSendIOData( %s, %d, [%c] ) FAIL! (%s)", 
					r, IoDataStru[iIndex].TypeStr, iLen, TmpStr[0], 
					GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID)  );
				AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
			}
			else
			{
				sprintf ( MsgStr, "XMS_ctsSendIOData( %s, %d, [%c] ) OK! (%s)", 
					IoDataStru[iIndex].TypeStr, iLen, TmpStr[0],  
					GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID)  );
				AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
			}
		}
		break;
		case 6://IO_TYPE_VOIP_SIPINFO
		{
			if ( DevOpened[iOpenedIndex1].DevID.m_s16DeviceMain != XMS_DEVMAIN_VOIP)
			{
				sprintf ( MsgStr, "XMS_ctsSendIOData( %s, %d, [%s] ) Failed! (%s)--For VoIP Only", 
					IoDataStru[iIndex].TypeStr, iLen, TmpStr,  
					GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID)  );
				AddMsg ( MSG_TYPE_FUNCTION, MsgStr );

				return;
			}
			Acs_VoIP_SIPINFO sipInfo = {0};
			strcpy(sipInfo.m_s8ContentType, "application/dtmf");
			sprintf(sipInfo.m_s8Body, "dtmf=%c\nduration=%d", TmpStr[0], 200);

			r = XMS_ctsSendIOData ( g_acsHandle, &DevOpened[iOpenedIndex1].DevID, 
			IoDataStru[iIndex].u16IOType, sizeof(sipInfo), &sipInfo );
			if ( r < 0 )
			{
				sprintf ( MsgStr, "X(%d) XMS_ctsSendIOData( %s, %d, [%c] ) FAIL! (%s)", 
					r, IoDataStru[iIndex].TypeStr, iLen, TmpStr[0], 
					GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID)  );
				AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
			}
			else
			{
				sprintf ( MsgStr, "XMS_ctsSendIOData( %s, %d, [%c] ) OK! (%s)", 
					IoDataStru[iIndex].TypeStr, iLen, TmpStr[0],  
					GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID)  );
				AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
			}
		}
		break;
		case 7://IO_TYPE_VOIP_RTPDTMF
		{
			if ( DevOpened[iOpenedIndex1].DevID.m_s16DeviceMain != XMS_DEVMAIN_VOIP)
			{
				sprintf ( MsgStr, "XMS_ctsSendIOData( %s, %d, [%s] ) Failed! (%s)--For VoIP Only", 
					IoDataStru[iIndex].TypeStr, iLen, TmpStr,  
					GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID)  );
				AddMsg ( MSG_TYPE_FUNCTION, MsgStr );

				return;
			}

			Acs_VoIP_RTPDTMF rtpDtmf = {0};
			rtpDtmf.m_s8EventChar	= TmpStr[0];	// rtp dtmf event char
			rtpDtmf.m_s8Volume		= -32;				// volume (in dbm0 from -63 to 0)
			rtpDtmf.m_u16Duration	= 200;				// duration (in timestamp units

			r = XMS_ctsSendIOData ( g_acsHandle, &DevOpened[iOpenedIndex1].DevID, 
			IoDataStru[iIndex].u16IOType, sizeof(rtpDtmf), &rtpDtmf );
			if ( r < 0 )
			{
				sprintf ( MsgStr, "X(%d) XMS_ctsSendIOData( %s, %d, [%c] ) FAIL! (%s)", 
					r, IoDataStru[iIndex].TypeStr, iLen, TmpStr[0], 
					GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID)  );
				AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
			}
			else
			{
				sprintf ( MsgStr, "XMS_ctsSendIOData( %s, %d, [%c] ) OK! (%s)", 
					IoDataStru[iIndex].TypeStr, iLen, TmpStr[0],  
					GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID)  );
				AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
			}
		}
		break;

		}
		

	}
}

void CXMSApi_SendIOData::OnButton2() 
{
	m_Run = 0;	
}

void CXMSApi_SendIOData::OnClose() 
{
	m_Run = 0; 	
	CDialog::OnClose();
}



