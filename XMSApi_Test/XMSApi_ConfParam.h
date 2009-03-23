#if !defined(AFX_XMSAPI_CONFPARAM_H__17B019EA_FBDD_4BC9_BE8C_08A86DD4AEDB__INCLUDED_)
#define AFX_XMSAPI_CONFPARAM_H__17B019EA_FBDD_4BC9_BE8C_08A86DD4AEDB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XMSApi_ConfParam.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CXMSApi_ConfParam dialog

class CXMSApi_ConfParam : public CDialog
{
// Construction
public:
	CXMSApi_ConfParam(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CXMSApi_ConfParam)
	enum { IDD = IDD_DIALOG_CONF };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXMSApi_ConfParam)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CXMSApi_ConfParam)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XMSAPI_CONFPARAM_H__17B019EA_FBDD_4BC9_BE8C_08A86DD4AEDB__INCLUDED_)
