// RecCSPDlg.cpp : implementation file
//

#include "stdafx.h"
#include "XMSApi_Test.h"
#include "RecCSPDlg.h"
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

extern ACSHandle_t		        g_acsHandle;
extern TYPE_MY_DEVICE_STRUCT	DevOpened[MAX_DEVICE_ALLOW_OPEN];
extern int			            iOpenedIndex1;
extern RecordCSPProperty_t      g_RecCSP;
extern FILE * g_cspFp;
extern char   g_szCSPFile[MAX_PATH];
/////////////////////////////////////////////////////////////////////////////
// CRecCSPDlg dialog




extern DeviceID_t DevRecordCsp;

CRecCSPDlg::CRecCSPDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRecCSPDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRecCSPDlg)
	//}}AFX_DATA_INIT
}


void CRecCSPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecCSPDlg)
	DDX_Control(pDX, IDC_EDIT_PACKLEN, m_PackLen);
	DDX_Control(pDX, IDC_COMBO_SRCMODE, m_SrcMode);
	DDX_Control(pDX, IDC_COMBO_ENCODETYPE, m_EncodeType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRecCSPDlg, CDialog)
	//{{AFX_MSG_MAP(CRecCSPDlg)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecCSPDlg message handlers

BOOL CRecCSPDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_EncodeType.AddString("XMS_Alaw_type");
	m_EncodeType.AddString("XMS_Ulaw_type");
	m_EncodeType.AddString("XMS_Vox_type");
	m_EncodeType.AddString("XMS_Linear_8bit");

	m_EncodeType.SetCurSel(0);
	
	m_SrcMode.AddString("XMS_SRC_8K");
	m_SrcMode.AddString("XMS_SRC_6K");
	m_SrcMode.SetCurSel(0);
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRecCSPDlg::OnButton1() 
{
	char szBuf[100] = {0};
	char MsgStr[100]={0};
	int nPackLen;
	int r = 0;
	
	g_cspFp = fopen(g_szCSPFile,"w+b");
	g_RecCSP.m_u8EncodeType = m_EncodeType.GetCurSel();
	g_RecCSP.m_u8SRCMode = m_SrcMode.GetCurSel();

	GetDlgItem ( IDC_EDIT_PACKLEN )->GetWindowText ( szBuf, 20 );
	sscanf ( szBuf, "%d", &nPackLen);
	g_RecCSP.m_u16PacketLen = nPackLen;

	if ( ((CButton *)GetDlgItem ( IDC_RADIO_BARGE_IN ))->GetCheck() )
		g_RecCSP.m_u8CSPRecType = XMS_CSPREC_BARGEIN; 

	if ( ((CButton *)GetDlgItem ( IDC_RADIO_NO_BARGE_IN ))->GetCheck() )
		g_RecCSP.m_u8CSPRecType = XMS_CSPREC_NON_BARGEIN; 

	r = XMS_ctsRecordCSP(g_acsHandle, &DevRecordCsp, 
		&g_RecCSP,NULL);
	if ( r < 0 )
	{
		sprintf ( MsgStr, "X(%d) XMS_ctsSRecordCSP() FAIL! (%s)", 
			r, GetString_DeviceAll (&DevRecordCsp)  );
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr);
	}
	else
	{
		sprintf ( MsgStr, "XMS_ctsSRecordCSP() OK! (%s)", 
			GetString_DeviceAll (&DevRecordCsp));
		AddMsg ( MSG_TYPE_FUNCTION, MsgStr );
	}

	CDialog::OnOK();
}

