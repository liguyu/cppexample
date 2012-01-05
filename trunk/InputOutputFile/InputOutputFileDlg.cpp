// InputOutputFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "InputOutputFile.h"
#include "InputOutputFileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

typedef struct
{
	int a;
	float b;
	char tmp[12];
}SAVE_STRUCT;

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
// CInputOutputFileDlg dialog

CInputOutputFileDlg::CInputOutputFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInputOutputFileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputOutputFileDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CInputOutputFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputOutputFileDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CInputOutputFileDlg, CDialog)
	//{{AFX_MSG_MAP(CInputOutputFileDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_INPUT, OnButtonInput)
	ON_BN_CLICKED(IDC_BUTTON_OUTPUT, OnButtonOutput)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputOutputFileDlg message handlers

BOOL CInputOutputFileDlg::OnInitDialog()
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
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CInputOutputFileDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CInputOutputFileDlg::OnPaint() 
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
HCURSOR CInputOutputFileDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CInputOutputFileDlg::OnButtonInput() 
{
	SAVE_STRUCT readSS;
	char	tmpStr[256]={0};
	FILE	*pFileHandle;			//文件句柄
	bool	eof_reached=false;		//读取到文件尾
	ULONG		nSize = 0;
	int		number = 0;//每次读到的字节数
	CFileDialog *lpszOpenFile;		//定义一个CfileDialog对象

	lpszOpenFile = new
		CFileDialog(TRUE,"","",OFN_FILEMUSTEXIST |OFN_HIDEREADONLY |OFN_OVERWRITEPROMPT, "文件类型(*.bin)|*.bin||");//生成一个对话框
	if(lpszOpenFile->DoModal()== IDOK )	//假如点击对话框确定按钮
	{
		CString szGetName;
		szGetName = lpszOpenFile->GetPathName();//得到打开文件的路径
		this->SetDlgItemText(IDC_EDIT_INPUT_FILE, szGetName);//在窗口标题上显示路径

		//读取bin文件
		pFileHandle = fopen( szGetName, "rb" );
		if (pFileHandle == NULL)
		{
			sprintf(tmpStr,"打开文件失败, %s",szGetName);
			AfxMessageBox(tmpStr);
		}
		
		nSize = 0x0;
		fseek(pFileHandle, nSize, 0);
		number=fread(&readSS,sizeof(SAVE_STRUCT), 1,pFileHandle);
		sprintf(tmpStr,"文件\"%s\", 位置:0x%X, 读取%d个Byte",szGetName,nSize,number*4);
		if (number>0)//写DSP
		{
			
		}

		
		fclose(pFileHandle); 

	}
	delete lpszOpenFile;	//释放分配的对话框
	
	sprintf(tmpStr,"%d",readSS.a);
	this->SetDlgItemText(IDC_EDIT_INT,tmpStr);

	sprintf(tmpStr,"%f",readSS.b);
	this->SetDlgItemText( IDC_EDIT_FLOAT,tmpStr );
	
	sprintf(tmpStr,"%s",readSS.tmp);
	this->SetDlgItemText( IDC_EDIT_STRING,tmpStr );

	
}

void CInputOutputFileDlg::OnButtonOutput() 
{
	SAVE_STRUCT		ss;
	FILE			*pFileHandle;			//文件句柄
	int				numwritten;
	CFileDialog		*lpszOpenFile;		//定义一个CfileDialog对象
	char			tmpStr[64]={0};

	memset(&ss,0,sizeof(SAVE_STRUCT));
	
	this->GetDlgItem ( IDC_EDIT_INT )->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%d", &ss.a );

	this->GetDlgItem ( IDC_EDIT_FLOAT )->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%f", &ss.b );

	this->GetDlgItem ( IDC_EDIT_STRING )->GetWindowText( tmpStr, 30 );
	sscanf ( tmpStr, "%s", ss.tmp );


	lpszOpenFile = new CFileDialog(TRUE,"","", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "文件类型(*.bin)|*.bin||");//生成一个对话框
	if(lpszOpenFile->DoModal()== IDOK )	//假如点击对话框确定按钮
	{
		CString szGetName;
		szGetName = lpszOpenFile->GetPathName();//得到打开文件的路径
		this->SetDlgItemText(IDC_EDIT_OUTPUT_FILE, szGetName);//在窗口标题上显示路径
		
		//读取bin文件
		pFileHandle = fopen( szGetName, "w+" );
		if (pFileHandle == NULL)
		{
			sprintf(tmpStr,"打开文件失败, %s",szGetName);
			AfxMessageBox(tmpStr);
		}
		numwritten = fwrite( &ss, sizeof( SAVE_STRUCT ), 1, pFileHandle );

		
		if (numwritten==1 )//写DSP
		{
			AfxMessageBox("OK");
			
		}
		
		
		fclose(pFileHandle); 
		
	}
	delete lpszOpenFile;	//释放分配的对话框	
}
