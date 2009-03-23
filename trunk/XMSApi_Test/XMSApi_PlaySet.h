#if !defined(AFX_XMSAPI_PLAYSET_H__626F5DD2_1B52_4B7A_BD23_6C6743C86576__INCLUDED_)
#define AFX_XMSAPI_PLAYSET_H__626F5DD2_1B52_4B7A_BD23_6C6743C86576__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XMSApi_PlaySet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CXMSApi_PlaySet dialog

class CXMSApi_PlaySet : public CDialog
{
// Construction
public:
	void Enable_Tone(bool IsEnable);
	void Enable_File(bool IsEnable);
	void HandleEnableDisable();
	CXMSApi_PlaySet(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CXMSApi_PlaySet)
	enum { IDD = IDD_DIALOG_PLAY };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXMSApi_PlaySet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CXMSApi_PlaySet)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnRADIONormalStop();
	afx_msg void OnRADIOSingleCode();
	afx_msg void OnRADIOAnyCode();
	afx_msg void OnRadioPlayTypeFile();
	afx_msg void OnRadioPlayTypeIndex();
	afx_msg void OnRadioPlayFileNormal();
	afx_msg void OnRadioPlayFileQueue();
	afx_msg void OnRadioPlayTone();
	afx_msg void OnRadioPlayTypeFileQueue();
	afx_msg void OnRadioPlayTypeIndexQueue();
	afx_msg void OnChangeEDITPlayu32MaxTime();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XMSAPI_PLAYSET_H__626F5DD2_1B52_4B7A_BD23_6C6743C86576__INCLUDED_)
