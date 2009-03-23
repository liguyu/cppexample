#if !defined(AFX_XMSAPI_PARAMCAS_H__18FAFF3F_CB60_4AD4_821C_F870704B1F89__INCLUDED_)
#define AFX_XMSAPI_PARAMCAS_H__18FAFF3F_CB60_4AD4_821C_F870704B1F89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XMSApi_ParamCAS.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CXMSApi_ParamCAS dialog

class CXMSApi_ParamCAS : public CDialog
{
// Construction
public:
	CXMSApi_ParamCAS(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CXMSApi_ParamCAS)
	enum { IDD = IDD_DIALOG_PARAM_CAS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXMSApi_ParamCAS)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CXMSApi_ParamCAS)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XMSAPI_PARAMCAS_H__18FAFF3F_CB60_4AD4_821C_F870704B1F89__INCLUDED_)
