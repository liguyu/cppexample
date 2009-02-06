// PlayDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DJIPPBXGateway.h"
#include "PlayDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlayDlg dialog


CPlayDlg::CPlayDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPlayDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPlayDlg)
	m_iDspID = 0;
	m_iChID = 0;
	m_strFile = _T("");
	//}}AFX_DATA_INIT
}


void CPlayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlayDlg)
	DDX_Text(pDX, IDC_EDIT_VOCDSPID, m_iDspID);
	DDX_Text(pDX, IDC_EDIT_VOCCHID, m_iChID);
	DDX_Text(pDX, IDC_EDIT_FILE, m_strFile);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPlayDlg, CDialog)
	//{{AFX_MSG_MAP(CPlayDlg)
	ON_BN_CLICKED(IDC_BUTTON_FILE, OnButtonFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlayDlg message handlers

void CPlayDlg::OnButtonFile() 
{
	// TODO: Add your control notification handler code here
	CFileDialog clsFileDlg(TRUE);
	if(clsFileDlg.DoModal() == IDOK)
	{
		UpdateData(TRUE);
		m_strFile = clsFileDlg.GetPathName();
		UpdateData(FALSE);
	}
}

void CPlayDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	CDialog::OnOK();
}

BOOL CPlayDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_iDspID = 1;
	m_iChID = 0;
	m_strFile = "C:\\DJKeygoe\\Samples\\Voc\\Prompt.pcm";
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
