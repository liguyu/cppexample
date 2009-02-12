// DJIPPBXGatewayDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DJIPPBXGateway.h"
#include "DJIPPBXGatewayDlg.h"

#include "GatewayWork.h"

#include "PlayDlg.h"

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
	virtual BOOL OnInitDialog();
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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDJIPPBXGatewayDlg dialog

CDJIPPBXGatewayDlg::CDJIPPBXGatewayDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDJIPPBXGatewayDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDJIPPBXGatewayDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_piVOIPHangupInfo = NULL;
	m_piInterfHangupInfo = NULL;
}

void CDJIPPBXGatewayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDJIPPBXGatewayDlg)
	DDX_Control(pDX, IDC_EDIT_CONTROLINFO, m_editCtrlInfo);
	DDX_Control(pDX, IDC_LIST_LOGINFO, m_listLogInfo);
	DDX_Control(pDX, IDC_LIST_CALLINFO, m_listCallInfo);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDJIPPBXGatewayDlg, CDialog)
	//{{AFX_MSG_MAP(CDJIPPBXGatewayDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_EXIT, OnButtonExit)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_CALLNUMCFG, OnButtonCallnumcfg)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, OnButtonPlay)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SHOWLISTCALLINFOONE, OnSHOWLISTCALLINFOONE)
	ON_MESSAGE(WM_SHOWLISTCALLINFOALL, OnSHOWLISTCALLINFOALL)
	ON_MESSAGE(WM_TNDCALLBACKMESSAGE, OnTNDCALLBACKMESSAGE)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDJIPPBXGatewayDlg message handlers

BOOL CDJIPPBXGatewayDlg::OnInitDialog()
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
	tnd.cbSize = sizeof(NOTIFYICONDATA);
	tnd.hWnd = m_hWnd;
	tnd.uID = IDR_MAINFRAME;
	tnd.uFlags = NIF_MESSAGE|NIF_ICON|NIF_TIP;
	tnd.uCallbackMessage = WM_TNDCALLBACKMESSAGE;// 用户自定义的消息（鼠标在任务栏的本程序图标上动作时发送给本程序的消息）
	tnd.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	strcpy(tnd.szTip, theApp.m_clsLIni.GLoadID("IDD_DJIPPBXGATEWAY_DIALOG"));// 图标提示为“东进IP网关服务程序”
	Shell_NotifyIcon(NIM_ADD, &tnd);// 向任务栏添加图标

	SetWindowText(theApp.m_clsLIni.GLoadID("IDD_DJIPPBXGATEWAY_DIALOG"));
	GetDlgItem(IDC_BUTTON_EXIT)->SetWindowText(theApp.m_clsLIni.GLoadID("IDC_BUTTON_EXIT"));
	GetDlgItem(IDC_STATIC_INFOLIST)->SetWindowText(theApp.m_clsLIni.GLoadID("IDC_STATIC_INFOLIST"));
	GetDlgItem(IDC_STATIC_EVENTINFO)->SetWindowText(theApp.m_clsLIni.GLoadID("IDC_STATIC_EVENTINFO"));
	GetDlgItem(IDC_STATIC_CTRLBTN)->SetWindowText(theApp.m_clsLIni.GLoadID("IDC_STATIC_CTRLBTN"));
	GetDlgItem(IDC_BUTTON_CALLNUMCFG)->SetWindowText(theApp.m_clsLIni.GLoadID("IDC_BUTTON_CALLNUMCFG"));

#ifndef _DEBUG
	GetDlgItem(IDC_BUTTON_PLAY)->ShowWindow(FALSE);
#endif

	m_listLogInfo.ResetContent();
	//m_listLogInfo.AddString("系统开始运行...");

	m_listCallInfo.InsertColumn(LCIC_INDEX,	theApp.m_clsLIni.GLoadID("LCIC_INDEX"), 
		LVCFMT_CENTER, 60);
	m_listCallInfo.InsertColumn(LCIC_VOIPVOCDSPID, theApp.m_clsLIni.GLoadID("LCIC_VOIPVOCDSPID"), 
		LVCFMT_CENTER, 60);
	m_listCallInfo.InsertColumn(LCIC_VOIPCHANNEL, theApp.m_clsLIni.GLoadID("LCIC_VOIPCHANNEL"), 
		LVCFMT_CENTER, 60);
	m_listCallInfo.InsertColumn(LCIC_VOCCHANNEL, theApp.m_clsLIni.GLoadID("LCIC_VOCCHANNEL"), 
		LVCFMT_CENTER, 60);
	m_listCallInfo.InsertColumn(LCIC_VOIPSTATE,	theApp.m_clsLIni.GLoadID("LCIC_VOIPSTATE"), 
		LVCFMT_CENTER, 60);
	m_listCallInfo.InsertColumn(LCIC_VOIPCALLDRE, theApp.m_clsLIni.GLoadID("LCIC_VOIPCALLDRE"), 
		LVCFMT_CENTER, 60);
	m_listCallInfo.InsertColumn(LCIC_VOIPRAWCALLERID, theApp.m_clsLIni.GLoadID("LCIC_VOIPRAWCALLERID"), 
		LVCFMT_CENTER, 100);
	m_listCallInfo.InsertColumn(LCIC_VOIPRAWCALLEEID, theApp.m_clsLIni.GLoadID("LCIC_VOIPRAWCALLEEID"), 
		LVCFMT_CENTER, 100);
	m_listCallInfo.InsertColumn(LCIC_VOIPNOWCALLERID, theApp.m_clsLIni.GLoadID("LCIC_VOIPNOWCALLERID"), 
		LVCFMT_CENTER, 100);
	m_listCallInfo.InsertColumn(LCIC_VOIPNOWCALLEEID, theApp.m_clsLIni.GLoadID("LCIC_VOIPNOWCALLEEID"), 
		LVCFMT_CENTER, 100);
	m_listCallInfo.InsertColumn(LCIC_PSTNDSPID, theApp.m_clsLIni.GLoadID("LCIC_PSTNDSPID"), 
		LVCFMT_CENTER, 60);
	m_listCallInfo.InsertColumn(LCIC_PSTNCHANNEL, theApp.m_clsLIni.GLoadID("LCIC_PSTNCHANNEL"), 
		LVCFMT_CENTER, 60);
//	m_listCallInfo.InsertColumn(LCIC_PSTNSTATE, theApp.m_clsLIni.GLoadString("LCIC_PSTNSTATE"), 
//		LVCFMT_CENTER, 60);

	int iResult = InitDJKeygoe();
	if( iResult != 1 )
	{
		MessageBox(theApp.m_clsLIni.GLoadString("InitDJKeygoeFail"));
		PostQuitMessage(-1);
	}

	if( g_pShowInfo == NULL )
	{
		g_pShowInfo = new CShowInfo((DWORD)this);
	}

	// VOIP INFO
	int iVoipDSPSum = 0;
	for(int iDspIndex = 1; iDspIndex < MAX_DSP_SUM; ++iDspIndex)
	{
		if( g_cfgiVoipDspID[iDspIndex] > 0 )
		{
			++iVoipDSPSum;
		}
	}

	int iVoipSum = iVoipDSPSum * 30;

	m_piVOIPHangupInfo = new int[iVoipSum];
	memset(m_piVOIPHangupInfo, 0, iVoipSum*sizeof(int));

	CString strLcicIndex;
	for(int iVoipIndex = 0; iVoipIndex < iVoipSum; ++iVoipIndex)
	{
		strLcicIndex.Format("%d", iVoipIndex);
		m_listCallInfo.InsertItem(iVoipIndex, strLcicIndex);
	}

	// Interface INFO
	int iE1Sum = 0;
	for(iDspIndex = 1; iDspIndex < MAX_DSP_SUM; ++iDspIndex)
	{
		for(int iE1Index = 0; iE1Index < MAX_DSP_E1_SUM; ++iE1Index)
		{
			if( g_cfgiDigiDspID[iDspIndex][iE1Index] > 0 )
			{
				++iE1Sum;
			}
		}
	}
	m_piInterfHangupInfo = new int[iE1Sum*32];
	memset(m_piInterfHangupInfo, 0, iE1Sum*32*sizeof(int));

	return TRUE;  // return TRUELCIC_PSTNSTATE  unless you set the focus to a control
}

void CDJIPPBXGatewayDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDJIPPBXGatewayDlg::OnPaint() 
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
HCURSOR CDJIPPBXGatewayDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CDJIPPBXGatewayDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	ExitDJKeygoe();
	//m_listLogInfo.AddString("系统退出。");
	KillTimer(WM_TIMER_CHECKCHSTATE_TIMER);
	Shell_NotifyIcon(NIM_DELETE,&tnd);// 向任务栏删除图标

	return CDialog::DestroyWindow();
}

void CDJIPPBXGatewayDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	if( AfxMessageBox(theApp.m_clsLIni.GLoadString("SureExit"), MB_YESNO) != IDYES )
	{
		return;
	}

	CDialog::OnCancel();
}

void CDJIPPBXGatewayDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( AfxMessageBox(theApp.m_clsLIni.GLoadString("SureExit"), MB_YESNO) != IDYES )
	{
		return;
	}

	CDialog::OnOK();
}

HRESULT CDJIPPBXGatewayDlg::OnSHOWLISTCALLINFOONE(WPARAM wParam, LPARAM lParam)
{
	static bool bInit = true;
	if(bInit)
	{
		SetTimer(WM_TIMER_CHECKCHSTATE_TIMER, WM_TIMER_CHECKCHSTATE_TIME, NULL);
		bInit = false;
	}

	int iIndex = wParam;
	int iDspID = lParam/DSPBASE;
	int iVoipID = lParam%DSPBASE;

	if( m_iVoipResList[iIndex] != lParam )
	{
		ReInitCallInfo();
	}

	VOIP_STRUCT *pOneVOIP = &g_AllDeviceRes[iDspID].pVOIP[iVoipID];

	m_listCallInfo.SetItemText(iIndex, LCIC_VOIPSTATE, VOIPSTATE_STRING[pOneVOIP->iState]);
	m_listCallInfo.SetItemText(iIndex, LCIC_VOIPCALLDRE, CALLDRE_STRING[pOneVOIP->iCallDre]);
	m_listCallInfo.SetItemText(iIndex, LCIC_VOIPRAWCALLERID, pOneVOIP->CallerNum);
	m_listCallInfo.SetItemText(iIndex, LCIC_VOIPRAWCALLEEID, pOneVOIP->CalleeNum);
	m_listCallInfo.SetItemText(iIndex, LCIC_VOIPNOWCALLERID, pOneVOIP->CallerNumNow);
	m_listCallInfo.SetItemText(iIndex, LCIC_VOIPNOWCALLEEID, pOneVOIP->CalleeNumNow);
	CString strTemp;
	strTemp.Format("%d", pOneVOIP->LinkDevID.m_s8ModuleID);
	m_listCallInfo.SetItemText(iIndex, LCIC_PSTNDSPID, strTemp);
	strTemp.Format("%d", pOneVOIP->LinkDevID.m_s16ChannelID);
	m_listCallInfo.SetItemText(iIndex, LCIC_PSTNCHANNEL, strTemp);

	return 0;
}

HRESULT CDJIPPBXGatewayDlg::OnSHOWLISTCALLINFOALL(WPARAM wParam, LPARAM lParam)
{
	ReInitCallInfo();

	return 0;
}

void CDJIPPBXGatewayDlg::ReInitCallInfo()
{
	//m_listCallInfo.DeleteAllItems();
	int iListMaxIndex = m_listCallInfo.GetItemCount() - 1;

	int iLcicIndex = -1;
	CString strLcicIndex;
	for(int iDspID = 0; iDspID < MAX_DSP_SUM; ++iDspID)
	{
		for(int iVoipID = 0; iVoipID < g_AllDeviceRes[iDspID].lVOIPNum; ++iVoipID)
		{
			if( g_AllDeviceRes[iDspID].pVOIP[iVoipID].iState > 0 )
			{
				if( ++iLcicIndex > iListMaxIndex )
				{
					strLcicIndex.Format("%d", iLcicIndex);
					m_listCallInfo.InsertItem(iLcicIndex, strLcicIndex);
				}
				strLcicIndex.Format("%d", iDspID);
				m_listCallInfo.SetItemText(iLcicIndex, LCIC_VOIPVOCDSPID, strLcicIndex);
				strLcicIndex.Format("%d", iVoipID);
				m_listCallInfo.SetItemText(iLcicIndex, LCIC_VOIPCHANNEL, strLcicIndex);
				m_listCallInfo.SetItemText(iLcicIndex, LCIC_VOCCHANNEL, strLcicIndex);
				m_listCallInfo.SetItemText(iLcicIndex, LCIC_VOIPSTATE, 
					VOIPSTATE_STRING[g_AllDeviceRes[iDspID].pVOIP[iVoipID].iState]);

				g_AllDeviceRes[iDspID].pVOIP[iVoipID].iSeqID = iLcicIndex;

				m_iVoipResList[iLcicIndex] = iDspID*DSPBASE+iVoipID;
			}
		}
	}
}

HRESULT CDJIPPBXGatewayDlg::OnTNDCALLBACKMESSAGE(WPARAM wParam, LPARAM lParam)
{
	UINT uID = (UINT) wParam;			// 发出该消息的图标的ID
	UINT uMouseMsg = (UINT) lParam;		// 鼠标动作

	if( uMouseMsg==WM_LBUTTONDOWN )		// 如果是单击左键
	{
		switch(uID)
		{
		case IDR_MAINFRAME:					// 如果是本程序的图标
//			GetCursorPos(&pt);							// 取得鼠标位置
//			if( !AfxGetApp()->m_pMainWnd->IsZoomed() )	// 窗口是否最大化
			if( !AfxGetApp()->m_pMainWnd->IsWindowVisible() )
			{
				AfxGetApp()->m_pMainWnd->ShowWindow(SW_SHOWNORMAL);//显示程序窗口
			}
			else
			{
				AfxGetApp()->m_pMainWnd->ShowWindow(FALSE);
			}
			break;
			
		default:
			break;
		}
	}
	else if( uMouseMsg==WM_RBUTTONDOWN )	// 如果是单击右键
	{
		switch(uID)
		{
		case IDR_MAINFRAME:						// 如果是本程序的图标
			{
				if( AfxGetApp()->m_pMainWnd->IsWindowVisible() )
				{
					if( !AfxGetApp()->m_pMainWnd->IsIconic() )		// 窗口是否最小化
					{
						AfxGetApp()->m_pMainWnd->ShowWindow(SW_SHOWMINIMIZED);	// 最小化程序窗口
					}
					else
					{
						AfxGetApp()->m_pMainWnd->ShowWindow(SW_SHOWNORMAL);
					}
				}
				else
				{
					CAboutDlg dlgAbout;
					dlgAbout.DoModal();
				}
			}
			break;

		default:
			break;
		}
	}

	return 0;
}

void CDJIPPBXGatewayDlg::OnButtonExit() 
{
	// TODO: Add your control notification handler code here
	if( AfxMessageBox(theApp.m_clsLIni.GLoadString("SureExit"), MB_YESNO) != IDYES )
	{
		return;
	}

	CDialog::OnOK();
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetWindowText(theApp.m_clsLIni.GLoadID("IDD_ABOUTBOX"));
	GetDlgItem(IDC_STATIC_NAMEINFO)->SetWindowText(theApp.m_clsLIni.GLoadID("IDC_STATIC_NAMEINFO"));
	GetDlgItem(IDC_STATIC_COPYRIGHTINFO)->SetWindowText(theApp.m_clsLIni.GLoadID("IDC_STATIC_COPYRIGHTINFO"));
	GetDlgItem(IDOK)->SetWindowText(theApp.m_clsLIni.GLoadID("ABOUTDLGIDOK"));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDJIPPBXGatewayDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == WM_TIMER_CHECKCHSTATE_TIMER)
	{
		// VOIP CH
		int iVoipIndex = -1;
		for(int i = 1; i < MAX_DSP_SUM; ++i)
		{
			if(g_AllDeviceRes[i].lFlag && g_AllDeviceRes[i].lUsed)
			{
				int j = g_AllDeviceRes[i].lVOIPNum;
				if( g_AllDeviceRes[i].pVOIP == NULL || g_cfgiVoipDspID[i] == 0 )
				{
					continue;
				}

				for(int k = 0; k < j; ++k)
				{
					++iVoipIndex;
					if(g_AllDeviceRes[i].pVOIP[k].iState == VOIP_HANGUP && 
						g_AllDeviceRes[i].pVOIP[k].iLineState == DCS_FREE )
					{
						m_piVOIPHangupInfo[iVoipIndex] += 1;
						if( m_piVOIPHangupInfo[iVoipIndex] > 2 )
						{
							g_AllDeviceRes[i].pVOIP[k].iState = VOIP_FREE;
							g_pShowInfo->ShowInfo(iVoipIndex, i, k);
							m_piVOIPHangupInfo[iVoipIndex] = 0;
						}
					}
					else
					{
						m_piVOIPHangupInfo[iVoipIndex] = 0;
					}
				}
			}
		}

		// Interface CH
		int iInterfIndex = -1;
		for(i = 1; i < MAX_DSP_SUM; ++i)
		{
			if(g_AllDeviceRes[i].lFlag && g_AllDeviceRes[i].lUsed)
			{
				int k = g_AllDeviceRes[i].lInterfNum;
				if( g_AllDeviceRes[i].pInterf == NULL )
				{
					continue;
				}

				for(int iPcm = 0; iPcm < MAX_DSP_E1_SUM; ++iPcm)
				{
					if( g_cfgiDigiDspID[i][iPcm] > 0 )
					{
						// pInterf
						for(int j = 32*iPcm; j < k && j < 32*(iPcm+1); ++j)
						{
							++iInterfIndex;
							if(/*g_AllDeviceRes[i].pInterf[j].iState == TRK_HANGUP && */
								g_AllDeviceRes[i].pInterf[j].iLineState == DCS_FREE )
							{
								m_piInterfHangupInfo[iInterfIndex] += 1;
								if( m_piInterfHangupInfo[iInterfIndex] >= 35 )	//	底层30秒复位 故此处控制以30秒以上为复位时间
								{
									// 复位如果需要安全 尚需释放操作
									g_AllDeviceRes[i].pInterf[j].iState = TRK_FREE;
									m_piInterfHangupInfo[iInterfIndex] = 0;
								}
							}
							else
							{
								m_piInterfHangupInfo[iInterfIndex] = 0;
							}
						}
					}
				}
			}
		}
	}

	CDialog::OnTimer(nIDEvent);
}

void CDJIPPBXGatewayDlg::OnButtonCallnumcfg() 
{
	// TODO: Add your control notification handler code here
	int		iPhoneNum = 0;
	CString	strSecString;
	CString	strPhoneNumString;
	for(int iDspIndex = 1; iDspIndex < MAX_DSP_SUM; ++iDspIndex)
	{
		for(int iDspE1Index = 0; iDspE1Index < MAX_DSP_E1_SUM; ++iDspE1Index)
		{
			if( g_cfgiDigiDspID[iDspIndex][iDspE1Index] > 0 )
			{
				for(int i = iDspE1Index*32; i < (iDspE1Index+1)*32 && i < g_AllDeviceRes[iDspIndex].lInterfNum; ++i)
				{
					if(g_AllDeviceRes[iDspIndex].pInterf[i].deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_TRUNK ||
						g_AllDeviceRes[iDspIndex].pInterf[i].deviceID.m_s16DeviceSub == XMS_DEVSUB_ANALOG_USER )
					{
						strSecString.Format("%d-%d-%d", iDspIndex, iDspE1Index, i);
						iPhoneNum = iDspIndex*1000 + iDspE1Index*100 + i;
						strPhoneNumString.Format("%d", iPhoneNum);
						GetPrivateProfileString("PHONE", strSecString, strPhoneNumString, 
							g_AllDeviceRes[iDspIndex].pInterf[i].SelfCallNum, CALL_NUMBER_MAX_LENGTH, g_strCfgIni);
						WritePrivateProfileString("PHONE", strSecString, 
							g_AllDeviceRes[iDspIndex].pInterf[i].SelfCallNum, g_strCfgIni);
					}
				}
			}
		}
	}
	TRACE("%s %s\n", theApp.m_clsLIni.GLoadID("IDC_BUTTON_CALLNUMCFG"), theApp.m_clsLIni.GLoadString("Success"));
	CString strInfo;
	strInfo.Format("%s %s", theApp.m_clsLIni.GLoadID("IDC_BUTTON_CALLNUMCFG"), 
		theApp.m_clsLIni.GLoadString("Success"));
	m_editCtrlInfo.SetWindowText(strInfo);
}

void CDJIPPBXGatewayDlg::OnButtonPlay() 
{
	// TODO: Add your control notification handler code here
	CPlayDlg clsPlayDlg;
	if(clsPlayDlg.DoModal() == IDOK)
	{
		PlayProperty_t playParam;
		memset(&playParam, 0, sizeof(PlayProperty_t));
		playParam.m_u16PlayType = XMS_PLAY_TYPE_FILE;
		strcpy(playParam.m_s8PlayContent, clsPlayDlg.m_strFile);
		int iResult = XMS_ctsPlay(g_acsHandle, &g_AllDeviceRes[clsPlayDlg.m_iDspID].pVoice[clsPlayDlg.m_iChID].deviceID, 
			&playParam, NULL);
		CString strInfo;
		strInfo.Format("VOC(%d.%d) %s Ret(%d)", clsPlayDlg.m_iDspID, clsPlayDlg.m_iChID, 
			clsPlayDlg.m_strFile, iResult);
		m_editCtrlInfo.SetWindowText(strInfo);
		TRACE("VOC(%d.%d) %s Ret(%d)\n", clsPlayDlg.m_iDspID, clsPlayDlg.m_iChID, clsPlayDlg.m_strFile, iResult);
	}
}
