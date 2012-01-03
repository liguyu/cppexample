// Page2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "tabctrl.h"
#include "Page2Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Page2Dlg dialog


Page2Dlg::Page2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(Page2Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(Page2Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void Page2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Page2Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Page2Dlg, CDialog)
	//{{AFX_MSG_MAP(Page2Dlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Page2Dlg message handlers
