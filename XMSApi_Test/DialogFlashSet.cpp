// DialogFlashSet.cpp : implementation file
//

#include "stdafx.h"
#include "XMSApi_Test.h"
#include "DialogFlashSet.h"

#include "DJAcsDataDef.h"

#include "XMSApi_Test_Sub.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogFlashSet dialog

extern CmdParamData_FlashParam_t  g_FlashSet;
extern char cfg_flash_mintime[20];
extern char cfg_flash_maxtime[20];
extern char			cfg_IniName[MAX_FILE_NAME_LEN];

CDialogFlashSet::CDialogFlashSet(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogFlashSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogFlashSet)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDialogFlashSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogFlashSet)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogFlashSet, CDialog)
	//{{AFX_MSG_MAP(CDialogFlashSet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogFlashSet message handlers

BOOL CDialogFlashSet::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	GetDlgItem ( IDC_EDIT_m_u8MinHookFlashTime )->SetWindowText ( cfg_flash_mintime );

	GetDlgItem ( IDC_EDIT_m_u8MaxHookFlashTime )->SetWindowText ( cfg_flash_maxtime );

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogFlashSet::OnOK() 
{
	// TODO: Add extra validation here
	//char TmpStr[20];
	int iTmpVal;

	GetDlgItem ( IDC_EDIT_m_u8MinHookFlashTime )->GetWindowText ( cfg_flash_mintime, 20 );
	sscanf ( cfg_flash_mintime, "%d", &iTmpVal );
	g_FlashSet.m_u8MinHookFlashTime = iTmpVal;

	GetDlgItem ( IDC_EDIT_m_u8MaxHookFlashTime )->GetWindowText ( cfg_flash_maxtime, 20 );
	sscanf ( cfg_flash_maxtime, "%d", &iTmpVal );
	g_FlashSet.m_u8MaxHookFlashTime = iTmpVal;

	WritePrivateProfileString("flash","mintime", cfg_flash_mintime,cfg_IniName);
	WritePrivateProfileString("flash","maxtime", cfg_flash_maxtime,cfg_IniName);

	CDialog::OnOK();
}
