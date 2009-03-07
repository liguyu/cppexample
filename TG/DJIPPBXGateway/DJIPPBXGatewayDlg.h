// DJIPPBXGatewayDlg.h : header file
//

#if !defined(AFX_DJIPPBXGATEWAYDLG_H__669BDA14_8B44_4A26_B3B8_293EC973A7ED__INCLUDED_)
#define AFX_DJIPPBXGATEWAYDLG_H__669BDA14_8B44_4A26_B3B8_293EC973A7ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDJIPPBXGatewayDlg dialog
#include "GatewayWork.h"
#define WM_SHOWLISTCALLINFOONE (WM_USER + 100)
#define WM_SHOWLISTCALLINFOALL (WM_USER + 101)
#define WM_TNDCALLBACKMESSAGE (WM_USER + 102)
#define WM_TIMER_CHECKCHSTATE_TIMER 1
#define WM_TIMER_CHECKCHSTATE_TIME 1000

enum LISTCALLINFOCOLUMN
{
	LCIC_INDEX,
		LCIC_VOIPVOCDSPID,
		LCIC_VOIPCHANNEL,
		LCIC_VOCCHANNEL,
		LCIC_VOIPSTATE,
		LCIC_VOIPCALLDRE,
		LCIC_VOIPRAWCALLERID,
		LCIC_VOIPRAWCALLEEID,
		LCIC_VOIPNOWCALLERID,
		LCIC_VOIPNOWCALLEEID,
		LCIC_PSTNDSPID,
		LCIC_PSTNCHANNEL,
//		LCIC_PSTNSTATE
};

class CDJIPPBXGatewayDlg : public CDialog
{
// Construction
public:
	int *m_piVOIPHangupInfo;
	int *m_piInterfHangupInfo;
	CDJIPPBXGatewayDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDJIPPBXGatewayDlg)
	enum { IDD = IDD_DJIPPBXGATEWAY_DIALOG };
	CListBox	m_listLogInfo;
	CListCtrl	m_listCallInfo;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDJIPPBXGatewayDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void ReInitCallInfo();
	int m_iVoipResList[MAX_VOIP_SUM];
	NOTIFYICONDATA tnd;								// Õ–≈ÃÕº±ÍΩ·ππ
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDJIPPBXGatewayDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnButtonExit();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg HRESULT OnSHOWLISTCALLINFOONE(WPARAM wParam, LPARAM lParam);
	afx_msg HRESULT OnSHOWLISTCALLINFOALL(WPARAM wParam, LPARAM lParam);
	afx_msg HRESULT OnTNDCALLBACKMESSAGE(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DJIPPBXGATEWAYDLG_H__669BDA14_8B44_4A26_B3B8_293EC973A7ED__INCLUDED_)
