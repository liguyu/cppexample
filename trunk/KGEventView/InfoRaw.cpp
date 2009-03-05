// InfoRaw.cpp : implementation file
//

#include "stdafx.h"
#include "KGEventView.h"
#include "InfoRaw.h"
#include "MainFrm.h"
#include "ReadPacket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern        char    msgtrans[__MAX_PACKET__DATA];
LONGLONG      irow;
/////////////////////////////////////////////////////////////////////////////
// CInfoRaw

IMPLEMENT_DYNCREATE(CInfoRaw, CListView)

CInfoRaw::CInfoRaw()
{
}

CInfoRaw::~CInfoRaw()
{
}


BEGIN_MESSAGE_MAP(CInfoRaw, CListView)
	//{{AFX_MSG_MAP(CInfoRaw)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	ON_WM_SYSCOLORCHANGE()
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInfoRaw drawing

void CInfoRaw::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CInfoRaw diagnostics

#ifdef _DEBUG
void CInfoRaw::AssertValid() const
{
	CListView::AssertValid();
}

void CInfoRaw::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CInfoRaw message handlers


void CInfoRaw::OnInitialUpdate() 
{
	CListView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	// TODO: Add your specialized code here and/or call the base class
	CMainFrame *pFrm=(CMainFrame *)AfxGetMainWnd();
	pFrm->m_InfoRaw=(CInfoRaw*)this;
	
	LV_COLUMN lvc;
	CListCtrl& ls = GetListCtrl();

	cRlt = &ls ;
	lvc.mask =  LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM ; //LVCF_FMT |

	cRlt->SetExtendedStyle(cRlt->GetExtendedStyle()   |   LVS_EX_FULLROWSELECT);
//	cRlt->SetBkColor(RGB(55,55,55)); 

	lvc.iSubItem = 0;
	lvc.pszText = "ID";
	lvc.cx = 80;
	cRlt->InsertColumn(0,&lvc);

	lvc.iSubItem = 1;
	lvc.pszText = "Time";
	lvc.cx = 150;
	cRlt->InsertColumn(1,&lvc);

	lvc.iSubItem = 2;
	lvc.pszText = "Type";
	lvc.cx = 40;
	cRlt->InsertColumn(2,&lvc);

	lvc.iSubItem = 3;
	lvc.pszText = "API/EVT";
	lvc.cx = 150;
	cRlt->InsertColumn(3,&lvc);

	lvc.iSubItem = 4;
	lvc.pszText = "DeviceID_t";
	lvc.cx = 200;
	cRlt->InsertColumn(4,&lvc);

	lvc.iSubItem = 5;
	lvc.pszText = "RawData";
	lvc.cx = 500;
	cRlt->InsertColumn(5,&lvc);

	irow=0;
}

BOOL CInfoRaw::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style |= LVS_SHOWSELALWAYS | LVS_REPORT|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES;
       	
	return CListView::PreCreateWindow(cs);
}

void CInfoRaw::InsItem()
{
	int        i;
	CString    tmp;
    //CInfoRaw::cRlt->InsertItem(0,msg.GetBuffer(msg.GetLength()));
    CListCtrl& list = GetListCtrl();
	tmp.Format("%010X",irow);
    cRlt->InsertItem(cRlt->GetItemCount(),tmp.GetBuffer(tmp.GetLength()));
	i=cRlt->GetItemCount();
	irow++;
	if(irow>200) cRlt->DeleteItem(201);
	//else return;
}

void CInfoRaw::OnSysColorChange() 
{
	CListView::OnSysColorChange();
	
	// TODO: Add your message handler code here
	crWindow              = ::GetSysColor(COLOR_WINDOW);
	crWindowText          = ::GetSysColor(COLOR_WINDOWTEXT);
	crHighLight     = ::GetSysColor(COLOR_HIGHLIGHT);
	crHighLightText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
}

void CInfoRaw::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	//draw each item.set txt color,bkcolor....
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
	
	// Take the default processing unless we set this to something else below.
	*pResult = CDRF_DODEFAULT;
	
	// First thing - check the draw stage. If it's the control's prepaint
	// stage, then tell Windows we want messages for every item.
	
	if (pLVCD->nmcd.dwDrawStage == CDDS_PREPAINT)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if (pLVCD->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
	{
		// This is the notification message for an item.  We'll request
		// notifications before each subitem's prepaint stage.
		
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
	
// 	else if((CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage)    
//     {    
// 
// 		pLVCD->clrText= RGB(0,205,0);
//         pLVCD->clrTextBk = RGB(55,55,55);  
// 
//         *pResult = CDRF_DODEFAULT;    
//     }
}


void CInfoRaw::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CString       my_msg;
	*pResult = 0;
	CMainFrame *pFrm=(CMainFrame *)AfxGetMainWnd();

	 DWORD dwPos = GetMessagePos();
	 CPoint point( LOWORD(dwPos), HIWORD(dwPos) );

	 cRlt->ScreenToClient(&point);

	 LVHITTESTINFO lvinfo;
	 lvinfo.pt = point;
	 lvinfo.flags = LVHT_ABOVE;


	 int nItem = cRlt->SubItemHitTest(&lvinfo);
	 if(nItem != -1)
	 {
		CString info;
		CString strtemp;
		CString msg;
		strtemp.Format("单击的是第%d行第%d列: Txt=", lvinfo.iItem, lvinfo.iSubItem);
//		cRlt->GetItemText(lvinfo.iItem,lvinfo.iSubItem,(char *)(LPCTSTR)msg,__MAX_PACKET__DATA);
       //strtemp+=msg;
		cRlt->GetItemText(lvinfo.iItem,5,msgtrans,__MAX_PACKET__DATA);
		pFrm->ShowMsgDetail(msgtrans);
 //       pFrm->m_edit->SetText(my_msg);
	 }
}

void CInfoRaw::settxt(int row, int volum, CString msg)
{
     cRlt->SetItemText(row,volum,msg.GetBuffer(msg.GetLength()));
}

void CInfoRaw::DelAll()
{
    cRlt->DeleteAllItems();
}
