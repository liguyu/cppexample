#if !defined(AFX_MYDLGBAR_H__D5A150C6_A031_4B18_866C_A331FB002452__INCLUDED_)
#define AFX_MYDLGBAR_H__D5A150C6_A031_4B18_866C_A331FB002452__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyDlgBar.h : header file
//
#include "CheckComboBox.h"
#define   MAX_API       100
#define   MAX_EVT       100
/////////////////////////////////////////////////////////////////////////////
// CMyDlgBar dialog

class CMyDlgBar : public CDialogBar
{
// Construction
public:
	CMyDlgBar();   // standard constructor
	//{{AFX_DATA(CMyDlgBar)
	enum { IDD = IDD_DIALOGBAR };
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	void InitSelectInfo();
	void GetChnlSelectInfo();
	void GetDspSelectInfo();
	void GetDevSelectInfo();
	void GetEVTSelectInfo();
	void GetAPISelectInfo();


	void initdevtype();
	void initappid();
	void initapi();
	void initevt();
	void initdspid();
	void initchnlid();
	typedef struct 
	{
		CString   Name;
		BOOL      State;
	}State_Bar;
// Dialog Data



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyDlgBar)
	public:
    virtual void    OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHandler);
	virtual BOOL Create(CWnd* pParent, UINT nIDTemplate, UINT nStyle, UINT nID);
	
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

    CBitmapButton	   m_subButton;
	CCheckComboBox     m_appid;
	CCheckComboBox     m_api;
 	CCheckComboBox     m_evt;
	CCheckComboBox     m_devtype;
    CCheckComboBox     m_dspid;
 	CCheckComboBox     m_chnlid;

	// Generated message map functions
	//{{AFX_MSG(CMyDlgBar)
		// NOTE: the ClassWizard will add member functions here
	afx_msg LONG OnInitDialog ( UINT, LONG );
	afx_msg void OnBtnClk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CMyDlgBar)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYDLGBAR_H__D5A150C6_A031_4B18_866C_A331FB002452__INCLUDED_)
