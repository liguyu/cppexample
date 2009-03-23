// DialogFaxHeader.cpp : implementation file
//

#include "stdafx.h"
#include "XMSApi_Test.h"
#include "DialogFaxHeader.h"

#include "DJAcsDataDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern CmdParamData_FaxSetHeader_t g_FaxHeader;
	
	static int  WithHeader = 0;
	static char User[20];
	static char From[20];
	static char To[20];

/////////////////////////////////////////////////////////////////////////////
// CDialogFaxHeader dialog


CDialogFaxHeader::CDialogFaxHeader(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogFaxHeader::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogFaxHeader)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDialogFaxHeader::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogFaxHeader)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogFaxHeader, CDialog)
	//{{AFX_MSG_MAP(CDialogFaxHeader)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogFaxHeader message handlers

void CDialogFaxHeader::OnOK() 
{
	// TODO: Add extra validation here
	int iTmpVal=0;
	char Temp[21];


	GetDlgItem ( IDC_EDIT_WITHHEADER )->GetWindowText ( Temp, 20 );
	sscanf ( Temp, "%d", &iTmpVal );
	g_FaxHeader.s32WithHeader = iTmpVal;

	WithHeader = g_FaxHeader.s32WithHeader;

	GetDlgItem ( IDC_EDIT_HEADERUSER )->GetWindowText (User, 20 );
	sprintf(g_FaxHeader.s8HeaderUser,User);

	GetDlgItem ( IDC_EDIT_HEADERFROM )->GetWindowText (From, 20 );
	sprintf(g_FaxHeader.s8HeaderFrom,From);

	GetDlgItem ( IDC_EDIT_HEADERTO )->GetWindowText (To, 20 );
	sprintf(g_FaxHeader.s8HeaderTo,To);
	

	CDialog::OnOK();
}

BOOL CDialogFaxHeader::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	char Temp[20];
	sprintf(Temp,"%d",WithHeader);
	
	SetDlgItemText(IDC_EDIT_WITHHEADER,Temp);
	SetDlgItemText(IDC_EDIT_HEADERUSER,User);
	SetDlgItemText(IDC_EDIT_HEADERFROM,From);
	SetDlgItemText(IDC_EDIT_HEADERTO,To);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
