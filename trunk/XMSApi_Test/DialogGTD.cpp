// DialogGTD.cpp : implementation file
//

#include "stdafx.h"
#include "XMSApi_Test.h"
#include "DialogGTD.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogGTD dialog


CDialogGTD::CDialogGTD(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogGTD::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogGTD)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDialogGTD::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogGTD)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogGTD, CDialog)
	//{{AFX_MSG_MAP(CDialogGTD)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogGTD message handlers


