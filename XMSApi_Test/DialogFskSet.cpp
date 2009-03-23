// DialogFskSet.cpp : implementation file
//

#include "stdafx.h"
#include "XMSApi_Test.h"
#include "DialogFskSet.h"

#include "DJAcsDataDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern char cfg_rxFilterWindow[20];
extern char cfg_rxPeriodTheshold[20];
extern char cfg_rx55Count[20];
extern char cfg_rxAllOneSamples[20];
extern char cfg_rxDataTimeOut[20];
extern char cfg_txBit1Freq[20];
extern char cfg_txBit0Freq[20];
extern char cfg_tx55Count[20];
extern char cfg_txAllOneSamples[20];

/////////////////////////////////////////////////////////////////////////////
// CDialogFskSet dialog

extern CmdParamData_FskParam_t    g_FskSet;

CDialogFskSet::CDialogFskSet(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogFskSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogFskSet)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDialogFskSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogFskSet)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogFskSet, CDialog)
	//{{AFX_MSG_MAP(CDialogFskSet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogFskSet message handlers

void CDialogFskSet::OnOK() 
{
	// TODO: Add extra validation here

	//char TmpStr[20];
	int iTmpVal;

	GetDlgItem ( IDC_EDIT_rxFilterWindow )->GetWindowText ( cfg_rxFilterWindow, 20 );
	sscanf ( cfg_rxFilterWindow, "%d", &iTmpVal );
	g_FskSet.rxFilterWindow = iTmpVal;

	GetDlgItem ( IDC_EDIT_rxPeriodTheshold )->GetWindowText ( cfg_rxPeriodTheshold, 20 );
	sscanf ( cfg_rxPeriodTheshold, "%d", &iTmpVal );
	g_FskSet.rxPeriodTheshold = iTmpVal;

	GetDlgItem ( IDC_EDIT_rx55Count )->GetWindowText ( cfg_rx55Count, 20 );
	sscanf ( cfg_rx55Count, "%d", &iTmpVal );
	g_FskSet.rx55Count = iTmpVal;

	GetDlgItem ( IDC_EDIT_rxAllOneSamples )->GetWindowText ( cfg_rxAllOneSamples, 20 );
	sscanf ( cfg_rxAllOneSamples, "%d", &iTmpVal );
	g_FskSet.rxAllOneSamples = iTmpVal;

	GetDlgItem ( IDC_EDIT_rxDataTimeOut )->GetWindowText ( cfg_rxDataTimeOut, 20 );
	sscanf ( cfg_rxDataTimeOut, "%d", &iTmpVal );
	g_FskSet.rxDataTimeOut = iTmpVal;

	GetDlgItem ( IDC_EDIT_txBit1Freq )->GetWindowText ( cfg_txBit1Freq, 20 );
	sscanf ( cfg_txBit1Freq, "%d", &iTmpVal );
	g_FskSet.txBit1Freq = iTmpVal;

	GetDlgItem ( IDC_EDIT_txBit0Freq )->GetWindowText ( cfg_txBit0Freq, 20 );
	sscanf ( cfg_txBit0Freq, "%d", &iTmpVal );
	g_FskSet.txBit0Freq = iTmpVal;

	GetDlgItem ( IDC_EDIT_tx55Count )->GetWindowText ( cfg_tx55Count, 20 );
	sscanf ( cfg_tx55Count, "%d", &iTmpVal );
	g_FskSet.tx55Count = iTmpVal;

	GetDlgItem ( IDC_EDIT_txAllOneSamples )->GetWindowText ( cfg_txAllOneSamples, 20 );
	sscanf ( cfg_txAllOneSamples, "%d", &iTmpVal );
	g_FskSet.txAllOneSamples = iTmpVal;


	//g_FskSet.rxFilterWindow = 
	CDialog::OnOK();
}

BOOL CDialogFskSet::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//char* TmpStr="0";
	// TODO: Add extra initialization here
	GetDlgItem ( IDC_EDIT_rxFilterWindow )->SetWindowText ( cfg_rxFilterWindow );

	GetDlgItem ( IDC_EDIT_rxPeriodTheshold )->SetWindowText ( cfg_rxPeriodTheshold );

	GetDlgItem ( IDC_EDIT_rx55Count )->SetWindowText ( cfg_rx55Count );

	GetDlgItem ( IDC_EDIT_rxAllOneSamples )->SetWindowText ( cfg_rxAllOneSamples );

	GetDlgItem ( IDC_EDIT_rxDataTimeOut )->SetWindowText ( cfg_rxDataTimeOut );

	GetDlgItem ( IDC_EDIT_txBit1Freq )->SetWindowText ( cfg_txBit1Freq );

	GetDlgItem ( IDC_EDIT_txBit0Freq )->SetWindowText ( cfg_txBit0Freq );

	GetDlgItem ( IDC_EDIT_tx55Count )->SetWindowText ( cfg_tx55Count );

	GetDlgItem ( IDC_EDIT_txAllOneSamples )->SetWindowText ( cfg_txAllOneSamples );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
