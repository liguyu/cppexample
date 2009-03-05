// DialogRun.cpp : implementation file
//

#include "stdafx.h"
#include "KGEventView.h"
#include "DialogRun.h"
#include "KGEventViewDoc.h"
#include "SAStatusLog.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DialogRun dialog


DialogRun::DialogRun(CWnd* pParent /*=NULL*/)
	: CDialog(DialogRun::IDD, pParent)
{
	EnableAutomation();

	//{{AFX_DATA_INIT(DialogRun)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DialogRun::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CDialog::OnFinalRelease();
}

void DialogRun::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DialogRun)
	DDX_Text(pDX,IDC_EDIT_FILE_PATH,FilePath);
	DDX_Text(pDX,IDC_IPADDRESS,m_IP);
    DDX_Text(pDX,IDC_EDIT_PORT,m_strPort);
	DDV_MaxChars(pDX,m_strPort,4);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DialogRun, CDialog)
	//{{AFX_MSG_MAP(DialogRun)
	ON_BN_CLICKED(IDC_BUTTON_FPATH, OnButtonFpath)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(DialogRun, CDialog)
	//{{AFX_DISPATCH_MAP(DialogRun)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IDialogRun to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {CA3694F4-EF0F-40C5-95F7-58DF56DC6DD5}
static const IID IID_IDialogRun =
{ 0xca3694f4, 0xef0f, 0x40c5, { 0x95, 0xf7, 0x58, 0xdf, 0x56, 0xdc, 0x6d, 0xd5 } };

BEGIN_INTERFACE_MAP(DialogRun, CDialog)
	INTERFACE_PART(DialogRun, IID_IDialogRun, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DialogRun message handlers

void DialogRun::OnButtonFpath() 
{
	// TODO: Add your control notification handler code here
	CString   strFile(COleDateTime::GetCurrentTime().Format( "%Y.%m.%d-%H.%M.%S.txt ")); 
	CFileDialog     filedlg(false,   
		"txt ",   
		strFile,   
		OFN_HIDEREADONLY   |   OFN_OVERWRITEPROMPT,   "bitmap   file   (*.txt)   |*.txt|   | "); 
	strFile= "保存文件: "+strFile; 
    filedlg.m_ofn.lpstrTitle   =   strFile;                         
    filedlg.m_ofn.lpstrInitialDir   =   "e:\\ ";//初始路径 
    filedlg.m_ofn.nMaxFile   =   MAX_PATH; 
	
    if(filedlg.DoModal()==IDCANCEL) 
		return;
	CString name=filedlg.GetPathName();
	
	FilePath=name;
//    pFrm->g_statusLog.InitPath(FilePath);
//    pFrm->g_statusLog.Enable(TRUE);
// 	g_statusLog.InitPath(FilePath);
// 	g_statusLog.Enable(TRUE);
	
	UpdateData(FALSE);
}

void DialogRun::OnOK() 
{
    GetDlgItemText(IDC_EDIT_FILE_PATH,FilePath);
    GetDlgItemText(IDC_IPADDRESS,m_IP);
    GetDlgItemText(IDC_EDIT_PORT,m_strPort);
	// TODO: Add extra validation here
    pFrm->m_strIP = m_IP;
    pFrm->m_strPort = m_strPort;
    pFrm->FilePath = FilePath;
	pFrm->OnListen();
	pFrm->m_InfoRaw->DelAll();
	if (m_strPort.IsEmpty())
		pFrm->g_statusLog.Enable(FALSE);
		
	else
	{
	pFrm->g_statusLog.InitPath(FilePath);
 	pFrm->g_statusLog.Enable(TRUE);
	}
	CDialog::OnOK();
}

BOOL DialogRun::OnInitDialog() 
{
	CDialog::OnInitDialog();
	pFrm=(CMainFrame *)AfxGetMainWnd();
	// TODO: Add extra initialization here
	m_IP="127.0.0.1";
    m_strPort="2010";
	SetDlgItemText(IDC_IPADDRESS,m_IP);
	SetDlgItemText(IDC_EDIT_PORT,m_strPort);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
