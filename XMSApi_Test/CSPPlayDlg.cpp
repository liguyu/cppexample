// CSPPlayDlg.cpp : implementation file
//

#include "stdafx.h"
#include "XMSApi_Test.h"
#include "CSPPlayDlg.h"
#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"
#include "XMSApi_Test_Event.h"
#include "XMSApi_Test_String.h"
#include "XMSApi_Test_Sub.h"

#include "DJAcsDataDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern FILE * g_cspFp;
extern char   g_szCSPFile[MAX_PATH];
extern int    g_cspPlay;
extern ACSHandle_t		        g_acsHandle;
extern TYPE_MY_DEVICE_STRUCT	DevOpened[MAX_DEVICE_ALLOW_OPEN];
extern int			            iOpenedIndex1;

/////////////////////////////////////////////////////////////////////////////
// CCSPPlayDlg dialog

extern DeviceID_t DevPlayCsp;

CCSPPlayDlg::CCSPPlayDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCSPPlayDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCSPPlayDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCSPPlayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCSPPlayDlg)
	DDX_Control(pDX, IDC_EDIT_DATALEN, m_DataLen);
	DDX_Control(pDX, IDC_COMBO_DATATYPE, m_DataType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCSPPlayDlg, CDialog)
	//{{AFX_MSG_MAP(CCSPPlayDlg)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCSPPlayDlg message handlers

void CCSPPlayDlg::OnButton1() 
{
	char MsgStr[100]={0};
	int r = 0;
	CSPPlayProperty_t cspProperty={0};

	cspProperty.m_u8TaskID = (DJ_U8)GetTickCount();
	cspProperty.m_u8SrcMode = XMS_SRC_8K;
	cspProperty.m_u8DecodeType = XMS_Alaw_type;
	cspProperty.m_u32MaxSize = 0;
	cspProperty.m_u32MaxTime = 0;
	cspProperty.m_u32StopMode = XMS_Normal_Stop;
	cspProperty.m_u32StopCode = 0;

	
	//XMS_ctsPlayCSP(g_acsHandle,&DevOpened[iOpenedIndex1].DevID, &cspProperty,NULL);
	XMS_ctsPlayCSP(g_acsHandle,&DevPlayCsp, &cspProperty,NULL);
	if ( r < 0 )
	{
		sprintf ( MsgStr, "X(%d) XMS_ctsPlayCSP() FAIL! (%s)", 
			r, GetString_DeviceAll (&DevPlayCsp)  );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr);
	}
	else
	{
		sprintf ( MsgStr, "XMS_ctsPlayCSP() OK! (%s)", 
			GetString_DeviceAll (&DevPlayCsp));
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}

	CDialog::OnOK();	
}

BOOL CCSPPlayDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	char szBuf[100] = {0};

	g_cspPlay = 1;
	m_DataLen.SetWindowText("4000");
	m_DataType.AddString("Voc");	
	m_DataType.AddString("Video");
	m_DataType.SetCurSel(0);

	sprintf(szBuf,"%d","0");	
	GetDlgItem ( IDC_EDIT_TASKID )->SetWindowText(szBuf);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
