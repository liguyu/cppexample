// Page1Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "tabctrl.h"
#include "Page1Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Page1Dlg dialog


Page1Dlg::Page1Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(Page1Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(Page1Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void Page1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Page1Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Page1Dlg, CDialog)
	//{{AFX_MSG_MAP(Page1Dlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Page1Dlg message handlers
