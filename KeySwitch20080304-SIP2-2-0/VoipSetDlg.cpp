// VoipSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "KeySwitch.h"
#include "VoipSetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVoipSetDlg dialog


CVoipSetDlg::CVoipSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVoipSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVoipSetDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CVoipSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVoipSetDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVoipSetDlg, CDialog)
	//{{AFX_MSG_MAP(CVoipSetDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVoipSetDlg message handlers
