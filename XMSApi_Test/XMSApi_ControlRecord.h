#if !defined(AFX_XMSAPI_CONTROLRECORD_H__CFC1FA47_5FA7_4935_AE69_3095A46E3DBE__INCLUDED_)
#define AFX_XMSAPI_CONTROLRECORD_H__CFC1FA47_5FA7_4935_AE69_3095A46E3DBE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XMSApi_ControlRecord.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CXMSApi_ControlRecord dialog

class CXMSApi_ControlRecord : public CDialog
{
// Construction
public:
	void Get_u32StepSize();
	CXMSApi_ControlRecord(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CXMSApi_ControlRecord)
	enum { IDD = IDD_DIALOG_CONTROLRECORD };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXMSApi_ControlRecord)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CXMSApi_ControlRecord)
	virtual BOOL OnInitDialog();
	afx_msg void OnButton1();
	virtual void OnOK();
	afx_msg void OnBUTTONStop();
	afx_msg void OnBUTTONPause();
	afx_msg void OnBUTTONResume();
	afx_msg void OnBUTTONStepBack();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XMSAPI_CONTROLRECORD_H__CFC1FA47_5FA7_4935_AE69_3095A46E3DBE__INCLUDED_)
