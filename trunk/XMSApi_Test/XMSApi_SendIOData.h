#if !defined(AFX_XMSAPI_SENDIODATA_H__DE00AD1A_8D4C_46B8_A40F_25EA562C4088__INCLUDED_)
#define AFX_XMSAPI_SENDIODATA_H__DE00AD1A_8D4C_46B8_A40F_25EA562C4088__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XMSApi_SendIOData.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CXMSApi_SendIOData dialog

class CXMSApi_SendIOData : public CDialog
{
// Construction
public:
	CXMSApi_SendIOData(CWnd* pParent = NULL);   // standard constructor
	HANDLE m_hThread;
	int    m_Run;

// Dialog Data
	//{{AFX_DATA(CXMSApi_SendIOData)
	enum { IDD = IDD_DIALOG1 };
	CComboBox	m_ComboType;
	UINT	m_tick;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXMSApi_SendIOData)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CXMSApi_SendIOData)
	virtual BOOL OnInitDialog();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnClose();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XMSAPI_SENDIODATA_H__DE00AD1A_8D4C_46B8_A40F_25EA562C4088__INCLUDED_)
