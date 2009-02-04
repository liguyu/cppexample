#if !defined(AFX_CINLINESET_H__4D4BB1CF_4707_4FB5_9044_311172BD933E__INCLUDED_)
#define AFX_CINLINESET_H__4D4BB1CF_4707_4FB5_9044_311172BD933E__INCLUDED_



#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// cinlineset.h : header file
//
#include "DJAcsAPIDef.h"
#include "DJAcsDataDef.h"

/////////////////////////////////////////////////////////////////////////////
// CInLineSetDlg dialog

class CInLineSetDlg : public CDialog
{
// Construction
public:
	CInLineSetDlg(CWnd* pParent = NULL);   // standard constructor
	DJ_S32	mDspNo;						
	long	mDeviceNo;
	bool	pResult;

// Dialog Data
	//{{AFX_DATA(CInLineSetDlg)
	enum { IDD = IDD_IN_LINE_SET };
	CEdit	m_TargetUser;
	CEdit	m_QuickDial;
	CEdit	m_LineNo;
	CComboBox	m_EnableLong;
	CComboBox	m_Enable;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInLineSetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	/*自定义函数*/
	bool	ReadLineInfo();
	bool	SaveLineInfo();
	bool	VerifyData();
	// Generated message map functions
	//{{AFX_MSG(CInLineSetDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CINLINESET_H__4D4BB1CF_4707_4FB5_9044_311172BD933E__INCLUDED_)
