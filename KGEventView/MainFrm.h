// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__04204E37_D7B9_4C59_8D9D_755A3CB7DACD__INCLUDED_)
#define AFX_MAINFRM_H__04204E37_D7B9_4C59_8D9D_755A3CB7DACD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "MyDlgBar.h"
#include "InfoRaw.h"
#include "InfoDetail.h"
#include <vector> 
#include "SAStatusLog.h"
#include "xknetunit.h"
#include "EditTxt.h"


using   namespace   std; 
//////////////////////////////////////////////////////////////////////
class CMainFrame;

class CDJNetUnit : public CXKNetUnit
{
public:

	CDJNetUnit();
	virtual ~CDJNetUnit();
public:
	void DecodeDebugInfo(Tbyte *msgbuf, Tint nNumRecv);
	void DecodeStatusInfo(Tbyte *msgbuf, Tint nNumRecv);
	
	virtual void DecodePacket( Tbyte *msgbuf, Tint nNumRecv );
	virtual void OnRemoveSocket(void *p, void *pNull);
	
	void RemoveAll();

public:
	
	CMainFrame			*m_pMainFrame;	
	CString				m_strIP;
	CString				m_strPort;
	
};

/////////////////////////////////////////////////////////////



class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)
	CMainFrame *pFrm;
	BOOL       m_bCreateSplitter;
	enum showtype
	{
		w_Read=1,
        w_port
	};
// Attributes
public:
	CString             FilePath;
	CStatusBar			m_wndStatusBar; 
    CString				m_strPort;
	CXKThreadQueue	    g_tThreadQueue;
	CDJNetUnit		    g_tNetUnit;
	BOOL	            m_bConnected;
	BOOL	            m_bMonitor;			// 时实监控标志
	std::string			m_strIP;
	CSAStatusLog        g_statusLog;

	CImageList          m_ilToolBar;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	void OnShowRead(char *pRead, int cnt, showtype Type,CString cs);
	void OpenFile(CString sFileName);
	void          ShowMsgDetail(CString msg);
	void          WriteToFile(CString state);
	void          OnListen();
    EditTxt *     m_edit; 
	CInfoRaw*     m_InfoRaw;
	CInfoDetail*  m_InfoDetail;
	CMyDlgBar     m_wndDialogBar;
	CSplitterWnd  m_wndSplitter;
	CString       m_read;
	virtual       ~CMainFrame();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CToolBar    m_wndToolBar;

private:
	vector<CString>           m_msg;
	vector<CString>::iterator iiter;
// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnAppRUN();
	afx_msg void OnStop();
	afx_msg void OnUpdateRunDlg(CCmdUI* pCmdUI);
	afx_msg void OnUpdateStop(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__04204E37_D7B9_4C59_8D9D_755A3CB7DACD__INCLUDED_)
