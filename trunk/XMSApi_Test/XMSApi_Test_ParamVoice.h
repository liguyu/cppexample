#if !defined(AFX_XMSAPI_TEST_PARAMVOICE_H__BFF863CC_F48A_4C30_8465_0EC2BE8C4E51__INCLUDED_)
#define AFX_XMSAPI_TEST_PARAMVOICE_H__BFF863CC_F48A_4C30_8465_0EC2BE8C4E51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XMSApi_Test_ParamVoice.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CXMSApi_Test_ParamVoice dialog

class CXMSApi_Test_ParamVoice : public CDialog
{
// Construction
public:
	void HandleEnableDisable();
	CXMSApi_Test_ParamVoice(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CXMSApi_Test_ParamVoice)
	enum { IDD = IDD_DIALOG_PARAM_VOICE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXMSApi_Test_ParamVoice)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CXMSApi_Test_ParamVoice)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCHECKu8InputCtrlValid();
	afx_msg void OnCHECKu8OutputCtrlValid();
	afx_msg void OnCHECKu8GtdCtrlValid();
	afx_msg void OnCHECKInu8AgcEnable();
	afx_msg void OnCHECKInu8EcEnable();
	afx_msg void OnCHECKOutu8AgcEnable();
	afx_msg void OnRADIOOutSilence();
	afx_msg void OnRADIOOutInput();
	afx_msg void OnRADIOOutPlay();
	afx_msg void OnRADIOOutConfSum();
	afx_msg void OnRADIOOutRtpInput();
	afx_msg void OnRADIOOutMixer();
	afx_msg void OnCHECKGtdu8ChannelEnable();
	afx_msg void OnCHECKu8PlayGainMode();
	afx_msg void OnCHECKGtdu8MR2BEnable();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XMSAPI_TEST_PARAMVOICE_H__BFF863CC_F48A_4C30_8465_0EC2BE8C4E51__INCLUDED_)
