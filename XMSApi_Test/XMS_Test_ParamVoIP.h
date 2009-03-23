#if !defined(AFX_XMS_TEST_PARAMVOIP_H__DB3DCF25_5227_400B_AB77_36DECDBE4496__INCLUDED_)
#define AFX_XMS_TEST_PARAMVOIP_H__DB3DCF25_5227_400B_AB77_36DECDBE4496__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XMS_Test_ParamVoIP.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CXMS_Test_ParamVoIP dialog

class CXMS_Test_ParamVoIP : public CDialog
{
// Construction
public:
	CXMS_Test_ParamVoIP(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CXMS_Test_ParamVoIP)
	enum { IDD = IDD_DIALOG_PARAM_VOIP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXMS_Test_ParamVoIP)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CXMS_Test_ParamVoIP)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XMS_TEST_PARAMVOIP_H__DB3DCF25_5227_400B_AB77_36DECDBE4496__INCLUDED_)
