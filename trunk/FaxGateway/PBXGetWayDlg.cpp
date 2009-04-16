// PBXGetWayDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PBXGetWay.h"
#include "PBXGetWayDlg.h"
#include "globe.h"
#include "DTrunk.h"
#include "voip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPBXGetWayDlg dialog

CPBXGetWayDlg::CPBXGetWayDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPBXGetWayDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPBXGetWayDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPBXGetWayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPBXGetWayDlg)
	DDX_Control(pDX, IDC_VOIP, m_ListVOIP);
	DDX_Control(pDX, IDC_DTrunk, m_ListDTrunk);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPBXGetWayDlg, CDialog)
	//{{AFX_MSG_MAP(CPBXGetWayDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTONRecord, OnBUTTONRecord)
	ON_NOTIFY(NM_DBLCLK, IDC_VOIP, OnDblclkVoip)
	ON_NOTIFY(NM_DBLCLK, IDC_DTrunk, OnDblclkDTrunk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPBXGetWayDlg message handlers

BOOL CPBXGetWayDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	InitSystem();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPBXGetWayDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPBXGetWayDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPBXGetWayDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CPBXGetWayDlg::OnOK() 
{
	// TODO: Add extra validation here
	
//	CDialog::OnOK();
}


void CPBXGetWayDlg::OnDestroy() 
{	
	CDialog::OnDestroy();	
	ExitSystem();	
	
}

int CPBXGetWayDlg::ConvertTrunkChnInt(int *DspID, int *ChnID, char *ChStr)
{
	int ret = 0;
	CString strTemp;
	char p[4] = {0};

	ASSERT(ChStr != NULL);
	strTemp = ChStr;
	
	int comLog = strTemp.ReverseFind(',');
	
	strncpy(p, ChStr, comLog);
	*DspID = atoi(p);
	strcpy(p, &ChStr[comLog+1]);;		
	*ChnID = atoi(p);	

	if ((*DspID <= 0) || (*ChnID >= 128) || (*ChnID <= 0))
	{
		AfxMessageBox("Selecting DeviceID is unavailability");
	}
	else
	{
		ret = 1;
	}

	return ret;
}

int CPBXGetWayDlg::ConvertVoipChnInt(int *DspID, int *ChnID, char *ChStr)
{
	int ret;
	CString strTemp;
	char *temp = NULL;
	char p[4] = {0};

	ASSERT(ChStr != NULL);

	strTemp = ChStr;
	strTemp.Remove('(');
	strTemp.Remove(')');
	
	int comLog = strTemp.ReverseFind(',');
	temp = strTemp.GetBuffer(strTemp.GetLength());
	strncpy(p, temp, comLog);
	*DspID = atoi(p);
	strcpy(p, &temp[comLog+1]);;		
	*ChnID = atoi(p);

	if ((*DspID <= 0) || (*ChnID >= 128) || (*ChnID < 0))
	{
		AfxMessageBox("Selecting DeviceID is unavailability");
	}
	else
	{
		ret = 1;
	}

	TRACE("ConvertVoipChnInt %d, %d\n", *DspID, *ChnID);

	return ret;
}

int CPBXGetWayDlg::ConvertVoiceChnInt(int *DspID, int *ChnID, int iDspID)
{
	int ret = 0;
	CString strTemp;
	char p[4] = {0};
	
	int iCount = ((CListBox *)GetDlgItem(IDC_EDITRecordDev))->GetCount();
	int findIndex = -1;
	int bFind = 0;
	
	for (int i = 0; i < iCount; i++)
	{
		((CListBox *)GetDlgItem(IDC_EDITRecordDev))->GetText(i, strTemp);
		findIndex = atoi(strTemp.GetBuffer(strTemp.ReverseFind(',') - 1));
		
		if (findIndex == iDspID)
		{
			bFind = 1;
			break;	
		}
	}
	
	if (findIndex == -1)
	{
		AfxMessageBox("Don't find availablity voice");
		return ret = 0;
	}

	char *ChStr = strTemp.GetBuffer(strTemp.GetLength());
	
	int comLog = strTemp.ReverseFind(',');
	strncpy(p, ChStr, comLog);
	*DspID = atoi(p);
	strcpy(p, &ChStr[comLog+1]);;		
	*ChnID = atoi(p);

	if ((*DspID <= 0) || (*ChnID >= 128) || (*ChnID <= 0))
	{
		AfxMessageBox("Don't find availablity voice");
	}
	else
	{
		ret = 1;
	}

	return ret;
}

void CPBXGetWayDlg::OnBUTTONRecord() 
{
	char ChStr[50] = {0};
	char ChType[50] = {0};
	int iDspID = 0;
	int iChnID = 0;
	int iChType = 0; //0 TRUNK, 1 VOIP
	int iVocDspID = 0;
	int iVocChnID = 0;
	CString strStemp;
	
//	GetRecordedItem();
	GetDlgItem(IDC_EDITVoip)->GetWindowText(ChStr, sizeof(ChStr));
	GetDlgItem(IDC_STATICType)->GetWindowText(ChType, sizeof(ChType));
	
	if (strcmp(ChType, "TRUNK CH") == 0)
	{
		if (ConvertTrunkChnInt(&iDspID, &iChnID, ChStr) == 0)
		{
			return;		
		}

		iChType = 0;
	}
	else if (strcmp(ChType, "VOIP CH") == 0)
	{

		if (ConvertVoipChnInt(&iDspID, &iChnID, ChStr) == 0)
		{
			return;
		}
		iChType = 1;
	}
	else
	{
		AfxMessageBox("Please select channel");
		return;
	}

	if (ConvertVoiceChnInt(&iVocDspID, &iVocChnID, iDspID) == 1)
		RecordFile(iChType, iDspID, iChnID, iVocDspID, iVocChnID);

	GetDlgItem(IDC_BUTTONRecord)->EnableWindow(FALSE);
}

void CPBXGetWayDlg::OnDblclkVoip(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
	LPNMLISTVIEW   pNMListView   =   (NM_LISTVIEW*)pNMHDR; 

	char itemStr[50] = {0};	

	m_ListVOIP.GetItemText(pNMListView->iItem, 1, (LPTSTR)itemStr, sizeof(itemStr));

	GetDlgItem(IDC_STATICType)->SetWindowText(_T("VOIP CH"));	
	GetDlgItem(IDC_EDITVoip)->SetWindowText(itemStr);	
}

void CPBXGetWayDlg::OnDblclkDTrunk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
		*pResult = 0;
	LPNMLISTVIEW   pNMListView   =   (NM_LISTVIEW*)pNMHDR; 

	char itemStr[50] = {0};	

	m_ListDTrunk.GetItemText(pNMListView->iItem, 1, (LPTSTR)itemStr, sizeof(itemStr));
	
	GetDlgItem(IDC_STATICType)->SetWindowText(_T("TRUNK CH"));		
	GetDlgItem(IDC_EDITVoip)->SetWindowText(itemStr);
}

BOOL CPBXGetWayDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}
