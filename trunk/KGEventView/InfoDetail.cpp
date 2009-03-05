// InfoDetail.cpp : implementation file
//

#include "stdafx.h"
#include "KGEventView.h"
#include "InfoDetail.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInfoDetail

IMPLEMENT_DYNCREATE(CInfoDetail, CListView)

CInfoDetail::CInfoDetail()
{

}

CInfoDetail::~CInfoDetail()
{

}


BEGIN_MESSAGE_MAP(CInfoDetail, CListView)
	//{{AFX_MSG_MAP(CInfoDetail)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInfoDetail drawing

void CInfoDetail::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CInfoDetail diagnostics

#ifdef _DEBUG
void CInfoDetail::AssertValid() const
{
	CListView::AssertValid();
}

void CInfoDetail::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CInfoDetail message handlers

void CInfoDetail::OnInitialUpdate() 
{
    CString   msg;
	
	CListView::OnInitialUpdate();


	// TODO: Add your specialized code here and/or call the base class
	CMainFrame *pFrm=(CMainFrame *)AfxGetMainWnd();
	pFrm->m_InfoDetail=(CInfoDetail*)this;

	LV_COLUMN lvc;
	CListCtrl& ls = GetListCtrl() ;

	cInfolt = &ls ;

	lvc.mask =  LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM ; //LVCF_FMT |

//	cInfolt->SetBkColor(RGB(190,190,190)); 
	lvc.iSubItem = 0;
	lvc.pszText = "消息";
	lvc.cx = 250;
	ls.InsertColumn(0,&lvc);

	lvc.iSubItem = 1;
 	lvc.pszText = "分析";
 	lvc.cx = 280;
 	ls.InsertColumn(1,&lvc);

	lvc.iSubItem = 2;
	lvc.pszText = "值";
	lvc.cx = 220;
 	ls.InsertColumn(2,&lvc);

	iRow=-1;
}
BOOL CInfoDetail::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style |= LVS_SHOWSELALWAYS | LVS_REPORT ;
	
	return CListView::PreCreateWindow(cs);
}

void CInfoDetail::InsItem()
{
	iRow++;
    CListCtrl& list = GetListCtrl();
    list.InsertItem(iRow,"?");

}
void CInfoDetail::setitemdt(CString dt)
{
    cInfolt->SetItemText(iRow,2,dt);
}
void CInfoDetail::setRawV(CString dt)
{
    cInfolt->SetItemText(iRow,0,dt);
}
void CInfoDetail::DelAll()
{
   cInfolt->DeleteAllItems();
   iRow=-1;
}

void CInfoDetail::setAnlsV(CString dt)
{
    cInfolt->SetItemText(iRow,1,dt);
}
