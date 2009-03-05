#if !defined(AFX_INFORAW_H__2AEA0684_3D9D_4CAC_95D8_97F923166244__INCLUDED_)
#define AFX_INFORAW_H__2AEA0684_3D9D_4CAC_95D8_97F923166244__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InfoRaw.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInfoRaw view

class CInfoRaw : public CListView
{
protected:
	CInfoRaw();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CInfoRaw)
// Attributes
public:

   CListCtrl*  cRlt;
   CListCtrl   clst;

   struct ItemData
	{
	public:
		ItemData() : arrpsz( NULL ), dwData( NULL ),crText ( NULL ),crBak (NULL) {}

		LPTSTR* arrpsz;
		DWORD dwData;
		//color
		COLORREF * crText;
		COLORREF * crBak;
		

	private:
		// ban copying.
		ItemData( const ItemData& );
		ItemData& operator=( const ItemData& );
	};
// Operations
public:
	void DelAll();
	void settxt(int row,int volum,CString msg);
	void InsItem();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInfoRaw)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);	
	virtual void OnSysColorChange();
//	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CInfoRaw();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CInfoRaw)
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	COLORREF crWindowText,crWindow,crHighLight,crHighLightText;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INFORAW_H__2AEA0684_3D9D_4CAC_95D8_97F923166244__INCLUDED_)
