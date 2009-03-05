#if !defined(AFX_INFODETAIL_H__4F5A526E_1D78_41BF_A226_9A3BA7BDA853__INCLUDED_)
#define AFX_INFODETAIL_H__4F5A526E_1D78_41BF_A226_9A3BA7BDA853__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InfoDetail.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInfoDetail view

class CInfoDetail : public CListView
{
protected:
	CInfoDetail();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CInfoDetail)

// Attributes
public:
   CListCtrl*  cInfolt;
// Operations
public:
	void setAnlsV(CString dt);
	void setRawV(CString dt);
	void DelAll();
	void setitemdt(CString dt);
	void InsItem();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInfoDetail)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CInfoDetail();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	int     iRow;
	//{{AFX_MSG(CInfoDetail)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INFODETAIL_H__4F5A526E_1D78_41BF_A226_9A3BA7BDA853__INCLUDED_)
