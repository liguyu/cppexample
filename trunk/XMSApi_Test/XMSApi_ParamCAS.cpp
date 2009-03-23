// XMSApi_ParamCAS.cpp : implementation file
//

#include "stdafx.h"
#include "XMSApi_Test.h"
#include "XMSApi_ParamCAS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXMSApi_ParamCAS dialog

#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"

#include "XMSApi_Test_String.h"
#include "XMSApi_Test_Event.h"

extern	int		iOpenedIndex1;
extern	int		iOpenedIndex2;

extern	TYPE_MY_DEVICE_STRUCT	DevOpened[MAX_DEVICE_ALLOW_OPEN];


CXMSApi_ParamCAS::CXMSApi_ParamCAS(CWnd* pParent /*=NULL*/)
	: CDialog(CXMSApi_ParamCAS::IDD, pParent)
{
	//{{AFX_DATA_INIT(CXMSApi_ParamCAS)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CXMSApi_ParamCAS::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXMSApi_ParamCAS)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CXMSApi_ParamCAS, CDialog)
	//{{AFX_MSG_MAP(CXMSApi_ParamCAS)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXMSApi_ParamCAS message handlers

BOOL CXMSApi_ParamCAS::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
//	char	TmpStr[256];

	GetDlgItem ( IDC_EDIT_ParamCAS_DeviceID )->SetWindowText ( GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID) );

	GetDlgItem ( IDC_EDIT_ParamVoice_DeviceID )->SetWindowText ( GetString_DeviceAll (&DevOpened[iOpenedIndex2].DevID) );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
