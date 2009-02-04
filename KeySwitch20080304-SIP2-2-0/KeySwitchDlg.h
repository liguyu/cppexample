// KeySwitchDlg.h : header file
//

#if !defined(AFX_KEYSWITCHDLG_H__4E3CFF4E_7F90_45B4_A943_366F7716F6C0__INCLUDED_)
#define AFX_KEYSWITCHDLG_H__4E3CFF4E_7F90_45B4_A943_366F7716F6C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define	WM_REFRESH_STATUS_BAR	WM_USER+200

/////////////////////////////////////////////////////////////////////////////
// CKeySwitchDlg dialog

#include "SysSet/cinlineset.h"
#include "sysset/outlinesetdlg.h"

class CKeySwitchDlg : public CDialog
{
// Construction
public:
	char		mUpdateStatusBar[200];		//×´Ì¬À¸ÐÅÏ¢
	CKeySwitchDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CKeySwitchDlg)
	enum { IDD = IDD_KEYSWITCH_DIALOG };
	CListBox	m_VoipList;
	CListBox	m_VoiceList;
	CListBox	m_TrunkList;
	CListBox	m_DspList;
	CListBox	m_ListInfo;
	CListCtrl	m_ListControl;
	CStatusBar		m_wndStatusBar;			//×´Ì¬À¸
	bool			mIsRun;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKeySwitchDlg)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CInLineSetDlg	m_InLineDlg;
	COutLineSetDlg	m_OutLineDlg;

	// Generated message map functions
	//{{AFX_MSG(CKeySwitchDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	afx_msg void OnInlineSet();
	afx_msg void OnOutlineSet();
	afx_msg void OnExitSys();
	afx_msg void OnStartService();
	afx_msg void OnDebug();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnItemdblclickListStatus(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListStatus(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg	LRESULT OnRefreshStatusBar(WPARAM	wParam,LPARAM	lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButton5();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYSWITCHDLG_H__4E3CFF4E_7F90_45B4_A943_366F7716F6C0__INCLUDED_)
