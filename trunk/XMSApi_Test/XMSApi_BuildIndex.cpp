// XMSApi_BuildIndex.cpp : implementation file
//

#include "stdafx.h"
#include "XMSApi_Test.h"
#include "XMSApi_BuildIndex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"

#include "XMSApi_Test_Sub.h"
#include "XMSApi_Test_String.h"
#include "XMSApi_Test_Event.h"
#include "XMSApi_Test_Func.h"

extern	int		iOpenedIndex1;

extern	TYPE_MY_DEVICE_STRUCT	DevOpened[MAX_DEVICE_ALLOW_OPEN];

extern PlayProperty_t		g_BuildIndexProperty;
/////////////////////////////////////////////////////////////////////////////
// CXMSApi_BuildIndex dialog


CXMSApi_BuildIndex::CXMSApi_BuildIndex(CWnd* pParent /*=NULL*/)
	: CDialog(CXMSApi_BuildIndex::IDD, pParent)
{
	//{{AFX_DATA_INIT(CXMSApi_BuildIndex)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CXMSApi_BuildIndex::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXMSApi_BuildIndex)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CXMSApi_BuildIndex, CDialog)
	//{{AFX_MSG_MAP(CXMSApi_BuildIndex)
	ON_BN_CLICKED(IDC_BUTTON_SelectBuildIndex, OnBUTTONSelectBuildIndex)
	ON_BN_CLICKED(IDC_BUTTON_Build, OnBUTTONBuild)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXMSApi_BuildIndex message handlers

void CXMSApi_BuildIndex::OnBUTTONSelectBuildIndex() 
{
	// TODO: Add your control notification handler code here
	char	TmpStr[256];

	GetDlgItem ( IDC_EDIT_BuildPlayIndex )->GetWindowText ( TmpStr, MAX_FILE_NAME_LEN-1 );

	My_SelectFile ( TmpStr );

	GetDlgItem ( IDC_EDIT_BuildPlayIndex )->SetWindowText ( TmpStr );
	
}

void CXMSApi_BuildIndex::OnBUTTONBuild() 
{
	// TODO: Add your control notification handler code here

	GetNowSetVal ();

	DoFunc_BuildPlayIndex_Real ();
}

BOOL CXMSApi_BuildIndex::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	GetDlgItem ( IDC_EDIT_BuildIndex_DeviceID )->SetWindowText ( GetString_DeviceAll (&DevOpened[iOpenedIndex1].DevID) );
	
	if ( g_BuildIndexProperty.m_u16PlayType == XMS_BUILD_INDEX_FILE )
		((CButton *)GetDlgItem ( IDC_RADIO_INDEX_FILE ))->SetCheck(1);
	else
		((CButton *)GetDlgItem ( IDC_RADIO_INDEX_RAM ))->SetCheck(1);

	GetDlgItem ( IDC_EDIT_BuildPlayIndex )->SetWindowText ( g_BuildIndexProperty.m_s8PlayContent );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CXMSApi_BuildIndex::OnOK() 
{
	// TODO: Add extra validation here
	
	GetNowSetVal ();

	CDialog::OnOK();
}

void CXMSApi_BuildIndex::GetNowSetVal()
{
	
	if( ((CButton *)GetDlgItem ( IDC_RADIO_INDEX_FILE ))->GetCheck() )
		g_BuildIndexProperty.m_u16PlayType = XMS_BUILD_INDEX_FILE; 
	else
		g_BuildIndexProperty.m_u16PlayType = XMS_BUILD_INDEX_RAM; 

	GetDlgItem ( IDC_EDIT_BuildPlayIndex )->GetWindowText ( g_BuildIndexProperty.m_s8PlayContent, ACS_MAX_FILE_NAME_LEN-1 );

}
