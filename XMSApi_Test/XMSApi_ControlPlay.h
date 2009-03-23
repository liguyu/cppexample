#if !defined(AFX_XMSAPI_CONTROLPLAY_H__95B3AD9B_FD3D_4805_B95D_46DFF73CA468__INCLUDED_)
#define AFX_XMSAPI_CONTROLPLAY_H__95B3AD9B_FD3D_4805_B95D_46DFF73CA468__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XMSApi_ControlPlay.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CXMSApi_ControlPlay dialog

class CXMSApi_ControlPlay : public CDialog
{
// Construction
public:
	void Get_u16StepSize();
	CXMSApi_ControlPlay(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CXMSApi_ControlPlay)
	enum { IDD = IDD_DIALOG_CONTROLPLAY };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXMSApi_ControlPlay)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CXMSApi_ControlPlay)
	virtual BOOL OnInitDialog();
	afx_msg void OnBUTTONStop();
	afx_msg void OnBUTTONPause();
	afx_msg void OnBUTTONResume();
	afx_msg void OnBUTTONStepBack();
	afx_msg void OnBUTTONForward();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XMSAPI_CONTROLPLAY_H__95B3AD9B_FD3D_4805_B95D_46DFF73CA468__INCLUDED_)
