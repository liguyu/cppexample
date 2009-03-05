// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "KGEventView.h"
#include "DialogRun.h"

#include "MainFrm.h"
#include "ReadPacket.h"
#include "commontool.h"
#include "ReadEvtDetail.h"
#include "ReadAPIDetail.h"
#include "MyDlgBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern   CHECK_INFO     m_CheckInfo;
extern   LONGLONG       irow;
CTime    NowTime;
CTime    NewTime;
CString			        m_IP;

#define ILC_COLOR8 0x0008  

char    msgtrans[__MAX_PACKET__DATA];
/////////////////////////////////////////////////////////////////////////////
CDJNetUnit::CDJNetUnit()
:m_pMainFrame(NULL)
{
}

CDJNetUnit::~CDJNetUnit()
{
}

void CDJNetUnit::RemoveAll()
{
	m_tNetUnitTbl.RemoveAll();
}

void CDJNetUnit::OnRemoveSocket(void *p, void *pNull)
{
	m_pMainFrame->m_bConnected	= FALSE;
    m_pMainFrame->m_bMonitor=FALSE;
	CXKNetUnit::OnRemoveSocket(p, pNull);
}

void CDJNetUnit::DecodePacket(Tbyte *msgbuf, Tint nNumRecv )
{
	Tbyte * buf	= msgbuf;
	Tint        nLen	= nNumRecv;
	CString     msg;
	char        tmp[100];
	ReadPacket  m_rdpkt(msgbuf,nNumRecv);
	DJ_S8       szTmp[__MAX_PACKET__DATA]="0";
//	ReadPacket  m_rdpkt((LONGLONG )msgbuf);

	CTime ct = CTime::GetCurrentTime();
	sprintf(tmp,"%s",ct.Format("%Y.%m.%d %H:%M:%S"));

	//msg+=*buf;
	if( NULL == buf || nLen <= 0 || nLen >= __MAX_PACKET__DATA)
	{
		XK_ASSERT(FALSE);
		return;
	}
	HextoStr((DJ_U8*)msgbuf,szTmp,nNumRecv);
	m_pMainFrame->OnShowRead((char *)msgbuf, nNumRecv,CMainFrame::w_port,"");
	m_pMainFrame->WriteToFile(szTmp);
}

void CDJNetUnit::DecodeDebugInfo(Tbyte *msgbuf, Tint nNumRecv)
{
//	CMsg::PrintDebugInfoMsg(msgbuf, nNumRecv);
}

void CDJNetUnit::DecodeStatusInfo(Tbyte *msgbuf, Tint nNumRecv)
{
//	CMsg::PrintInfoMsg(msgbuf, nNumRecv);
}
/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_COMMAND(ID_RUN_DLG,OnAppRUN)
	ON_COMMAND(ID_Stop, OnStop)
	ON_COMMAND_EX(IDD_VIEW_DIALOGBAR, OnBarCheck)
    ON_UPDATE_COMMAND_UI(IDD_VIEW_DIALOGBAR, OnUpdateControlBarMenu)
	ON_UPDATE_COMMAND_UI(ID_RUN_DLG, OnUpdateRunDlg)
	ON_UPDATE_COMMAND_UI(ID_Stop, OnUpdateStop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	    ID_SEPARATOR,           // status line indicator
		ID_INDICATOR_CAPS,
		ID_INDICATOR_NUM,
		ID_INDICATOR_SCRL,
		ID_INDICATOR_IP,
	    ID_INDICATOR_PORT,
};
/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_InfoRaw=NULL;
	m_bConnected=FALSE;
	m_strIP="192.168.11.31";
	m_IP="192.168.11.31";
	m_strPort="2010";
	m_bMonitor=FALSE;
	//FilePath="d:\\LOG";
	FilePath.Empty();
	memset(msgtrans,0,__MAX_PACKET__DATA);
	pFrm=(CMainFrame *)AfxGetMainWnd();
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	////////////////////////////////////////////////////////
// 	HBITMAP hbm = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),
// 		MAKEINTRESOURCE(IDR_MAINFRAME), //加载IDB_TOOLBAR
// 		IMAGE_BITMAP, //按位图格式
// 		0,0, // cx,cy
// 		LR_CREATEDIBSECTION | LR_LOADMAP3DCOLORS );
// 	
// 	//LoadImage返回一个位图句柄HBITMAP，但在MFC下使用CBitmaps会更加方便，可创建一个
// 	//	CBitmap对象并用Attach()成员函数将它与位图句柄联系起来：
// 		
// 	  CBitmap bm;
//       bm.Attach(hbm); 
// 	  m_ilToolBar.Create(32,32,ILC_COLOR8, 4, 4);
// 	  m_ilToolBar.Add(&bm,(CBitmap*)NULL); 
// 	  m_wndToolBar.GetToolBarCtrl().SetImageList(&m_ilToolBar); 

  ////////////////////////////////////////////////////////////////   

	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndDialogBar.Create(this, IDD_DIALOGBAR,
		CBRS_TOP|CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY ,//| CBRS_HIDE_INPLACE,CBRS_TOP
		IDD_VIEW_DIALOGBAR))
//			CG_ID_VIEW_DIALOGBAR))
	{
		TRACE0("Failed to create dialog bar m_wndDialogBar\n");
		return -1;		// fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	m_wndDialogBar.EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndDialogBar,AFX_IDW_DOCKBAR_TOP|AFX_IDW_DOCKBAR_LEFT);

//	SetTimer(1,1000,NULL);
	pFrm=(CMainFrame *)AfxGetMainWnd();
	pFrm->ShowWindow(SW_SHOWNORMAL);   
    pFrm->UpdateWindow();

	m_wndStatusBar.SetPaneInfo(4,ID_INDICATOR_IP,SBPS_NORMAL,100);    //设置状态栏宽度
	m_wndStatusBar.SetPaneInfo(5,ID_INDICATOR_PORT,SBPS_NORMAL,60);    //设置状态栏宽度

	g_tNetUnit.m_pMainFrame	= this;
//	OnListen();
    
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
 	cs.cx   =   850;   
 	cs.cy   =   600;   
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	CRect   rect;   
    GetClientRect(   &rect   );
	int     ihigh=0;
	int     i=0;
	i=rect.Height();

	if (!m_wndSplitter.CreateStatic(this, 3, 1))
	{
		TRACE0("Failed to CreateStaticSplitter\n");
		return FALSE;
	}

	// add the first splitter pane - the default view in column 0
	ihigh=i/2;
	if (!m_wndSplitter.CreateView(0, 0,
		RUNTIME_CLASS(CInfoRaw), CSize(300,ihigh), pContext))
	{
		TRACE0("Failed to create second pane\n");
		return FALSE;
	}
	ihigh=i/12;
   if (!m_wndSplitter.CreateView(1, 0,
	RUNTIME_CLASS(EditTxt), CSize(300,ihigh), pContext))
   {
	TRACE0("Failed to create second pane\n");
	return FALSE;
	}
	ihigh=i*5/12;
	if (!m_wndSplitter.CreateView(2, 0,
		RUNTIME_CLASS(CInfoDetail), CSize(300, ihigh), pContext))
	{
		TRACE0("Failed to create first pane\n");
		return FALSE;
	}
	pFrm=(CMainFrame *)AfxGetMainWnd();
	m_bCreateSplitter=TRUE;
	m_wndSplitter.SetActivePane(0,0,NULL);

	NowTime=CTime::GetCurrentTime();
	iiter=m_msg.begin();
	return TRUE;
//	return CFrameWnd::OnCreateClient(lpcs, pContext);
}
void CMainFrame::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	char     tmp[100];
	CString  msg;
	CString  sg;
	CTime ct ; 
	ct = CTime::GetCurrentTime();
	sprintf(tmp,"%s",ct.Format("%Y.%m.%d %H:%M:%S"));

//	GetLocalTime(&NowTime);
//	msg.Format("%d.%d.%d %d:%d:%d",NowTime.wYear,NowTime.wMonth,NowTime.wDay,\
//		       NowTime.wHour,NowTime.wMinute,NowTime.wSecond);
	
//	AfxMessageBox("Timer");
	CFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::WriteToFile(CString state)
{
	CString        Temp;
	CSAStatusLog   cs;
    NewTime=CTime::GetCurrentTime();

	if(NewTime.GetDay()!=NowTime.GetDay())
 	{
    NowTime=CTime::GetCurrentTime();
	Temp=cs.GetBaseDir(FilePath);
	if(Temp.IsEmpty()) 
    {
		CString  Tempstr(COleDateTime::GetCurrentTime().Format\
		( "%Y.%m.%d-%H.%M.%S.txt"));
		Temp+=Tempstr;
		FilePath=Temp;

        g_statusLog.Init(FilePath);
 	    g_statusLog.Enable(TRUE);
    }
		else
		{
			CString  Tempstr(COleDateTime::GetCurrentTime().Format\
				( "%Y.%m.%d-%H.%M.%S.txt"));
			//Temp+='\\';
			Temp+=Tempstr;
			FilePath=Temp;
			
			g_statusLog.InitPath(FilePath);
			g_statusLog.Enable(TRUE);
			
		}
	}
	else
	{
		if(FilePath.IsEmpty())
		{
	    CString  Tempstr(COleDateTime::GetCurrentTime().Format\
		( "%Y.%m.%d-%H.%M.%S.txt"));
	//	Temp+='\\';
		Temp+=Tempstr;
		FilePath=Temp;

        g_statusLog.InitPath(FilePath);
 	    g_statusLog.Enable(TRUE);
		}
	}
	if (!g_statusLog.m_bEnable) return;
    g_statusLog.StatusOut(state);
}
void CMainFrame::OnListen()
{
	TRACE0("Onlisten.......................");
	int   iPort=atoi(m_strPort);
	// 起动线程:一个侦听线程
	g_tThreadQueue.Create(1);
	// 建立网元
	g_tNetUnit.Create(&g_tThreadQueue, __LOGMGR_CLIENT_ID__, __LOGMGR_CLIENT_TYPE__, string("LogMgr=>msg client"));
	// 侦听
	m_bConnected = g_tNetUnit.AddClient(m_strIP, iPort);
	if (!m_bConnected)  {
		TRACE0("CXKNetUnit::AddClient()=====>Error\n");
		m_bMonitor=FALSE;
		AfxMessageBox("AddClient Error!");
	}
}

void CMainFrame::OnAppRUN()
{
    DialogRun dlgRun;
	dlgRun.DoModal();
}

void  CMainFrame::ShowMsgDetail(CString msg)
{
	DJ_U8  Temp[__MAX_PACKET__DATA];
	DJ_U8 *pTemp;
	int    k;
	int    devlen;

	devlen=sizeof(DeviceID_t);

	m_edit->SetText(msg);
	pFrm=(CMainFrame *)AfxGetMainWnd();
	memset(Temp,0,__MAX_PACKET__DATA);

	msg.Remove(' ');
    pTemp=Temp;
	memcpy((DJ_Void*)Temp,msg,msg.GetLength());

	k=msg.GetLength();

    StrtoHex(pTemp,&k);

	ReadPacket      rPkt(pTemp,k);
    ReadEvtDetail   rEvtDetail(pTemp,k);
	ReadAPIDetail   rApiDetail(pTemp,k);

	if(APP_EVT==rPkt.GetPktType())
	{
	  int y=0;
      rEvtDetail.GetString_EvtData();
      pFrm->m_InfoDetail->DelAll();
      for (int l=0;l<rEvtDetail.m_Submsg.nNUm;l++)
      {
      y++;
      pFrm->m_InfoDetail->InsItem();
      pFrm->m_InfoDetail->setRawV(rEvtDetail.m_Submsg.n_MSgSrc[y]);
      pFrm->m_InfoDetail->setAnlsV(rEvtDetail.m_Submsg.n_MsgAnly[y]);
      pFrm->m_InfoDetail->setitemdt(rEvtDetail.m_Submsg.n_Msg[y]);
      }
	}
	if(APP_CMD==rPkt.GetPktType())
	{
		int y=0;
		rApiDetail.GetString_APIData();
		pFrm->m_InfoDetail->DelAll();
		for (int l=0;l<rApiDetail.m_Submsg.nNUm;l++)
		{
			y++;
			pFrm->m_InfoDetail->InsItem();
			pFrm->m_InfoDetail->setRawV(rApiDetail.m_Submsg.n_MSgSrc[y]);
			pFrm->m_InfoDetail->setAnlsV(rApiDetail.m_Submsg.n_MsgAnly[y]);
			pFrm->m_InfoDetail->setitemdt(rApiDetail.m_Submsg.n_Msg[y]);
		}
	}
}

void CMainFrame::OpenFile(CString sFileName)
{
	int nCnt	= 0;
	char buf[__MAX_PACKET__DATA];
	CString strMsg;
	CString Time;
	int   k;
	DJ_U8  Tempstr[__MAX_PACKET__DATA];
	DJ_U8 *pTemp=NULL;

     g_statusLog.InitPath(sFileName);
	 g_statusLog.MapOpen(CSAStatusLog::OT_READ);

	 if (g_statusLog.m_fMapOpen)
	 {
	 nCnt	= pFrm->g_statusLog.ReadFirstLine(buf, sizeof(buf));
	 pFrm->m_InfoRaw->DelAll();
	 pFrm->m_InfoDetail->DelAll();
	 while(nCnt>0)
	 {
		 strMsg	= buf;
		 m_read=strMsg.Mid(22);
		 Time=strMsg.Left(19);
		 m_read.Remove(' ');
		 k=m_read.GetLength();
		 memset(buf,0,sizeof(__MAX_PACKET__DATA));
		 memset(Tempstr,0,sizeof(__MAX_PACKET__DATA));
		 strcpy((DJ_S8 *)Tempstr,m_read);
         pTemp=Tempstr;
		 StrtoHex(pTemp,&k);
		 OnShowRead((DJ_S8 *)pTemp,k,w_Read,Time);
         nCnt	= g_statusLog.ReadNextLine(buf,sizeof(buf));
	 }
     g_statusLog.MapClose();
	}
	 else  AfxMessageBox("Open  fail!");
}

void CMainFrame::OnShowRead(char *pRead, int cnt, showtype Type,CString cs)
{

	char        tmp[100]={0};
	ReadPacket  m_rdpkt(pRead,cnt);
	CTime ct = CTime::GetCurrentTime();
	DJ_S8       szTmp[__MAX_PACKET__DATA]="0";

	sprintf(tmp,"%s",ct.Format("%Y.%m.%d %H:%M:%S"));

	HextoStr((DJ_U8*)pRead,szTmp,cnt);

	m_rdpkt.GetPktType();
	if (PKG_CMD==m_rdpkt.f_head)
	{
				if(m_CheckInfo.API[m_rdpkt.pkd_IDinfo.iAPI].IsCheck&&\
					m_CheckInfo.DEV[m_rdpkt.pkd_IDinfo.iDev].IsCheck&&\
					m_CheckInfo.DSP[m_rdpkt.pkd_IDinfo.iDsp].IsCheck&&\
					m_CheckInfo.CHNL[m_rdpkt.pkd_IDinfo.iChnl].IsCheck)
				{
					pFrm->m_InfoRaw->InsItem();
					if(1==Type) pFrm->m_InfoRaw->settxt(0,1,cs);
					else pFrm->m_InfoRaw->settxt(0,1,tmp);
					pFrm->m_InfoRaw->settxt(0,2,m_rdpkt.m_convert.msgType);
					pFrm->m_InfoRaw->settxt(0,3,m_rdpkt.m_convert.msg);
					pFrm->m_InfoRaw->settxt(0,4,m_rdpkt.m_convert.dev);
					pFrm->m_InfoRaw->settxt(0,5,szTmp);
					
				}
	}
			
	if (PKG_EVT==m_rdpkt.f_head)
	{
				if(m_CheckInfo.EVT[m_rdpkt.pkd_IDinfo.iEvt].IsCheck&&\
					m_CheckInfo.DEV[m_rdpkt.pkd_IDinfo.iDev].IsCheck&&\
					m_CheckInfo.DSP[m_rdpkt.pkd_IDinfo.iDsp].IsCheck&&\
					m_CheckInfo.CHNL[m_rdpkt.pkd_IDinfo.iChnl].IsCheck)
				{
					pFrm->m_InfoRaw->InsItem();
					if(1==Type) pFrm->m_InfoRaw->settxt(0,1,cs);
					else pFrm->m_InfoRaw->settxt(0,1,tmp);
					pFrm->m_InfoRaw->settxt(0,2,m_rdpkt.m_convert.msgType);
					pFrm->m_InfoRaw->settxt(0,3,m_rdpkt.m_convert.msg);
					pFrm->m_InfoRaw->settxt(0,4,m_rdpkt.m_convert.dev);
					pFrm->m_InfoRaw->settxt(0,5,szTmp);					
				}
	}
}

void CMainFrame::OnStop() 
{
	// TODO: Add your command handler code here
	g_tThreadQueue.RemoveAll();
	g_tNetUnit.RemoveAll();
	m_bMonitor=FALSE;
    m_bConnected=FALSE;
}

void CMainFrame::OnUpdateRunDlg(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_bConnected);
}

void CMainFrame::OnUpdateStop(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bConnected);
}
