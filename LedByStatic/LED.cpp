///////////////类cpp文件
// LED.cpp : implementation file
//
#include "stdafx.h"
//#include "colortext.h"
#include "LED.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLED

CLED::CLED()
{	
	m_crBK=RGB(192, 192, 192);	//指示灯矩形框背景色
	m_crOnFG=RGB(0,255,0);		//绿色	
	m_crOffFG=RGB(255,0,0);		//红色
	m_iType=0;	
	m_bOn=true;	
	m_brOnFG=new CBrush(m_crOnFG);	
	m_brOffFG=new CBrush(m_crOffFG);	
	m_sOnString="on";	
	m_sOffString="off";	
	m_bTextOn=true;	
}



CLED::~CLED()
{	
	delete m_brOffFG;	
	delete m_brOnFG;	
}

BEGIN_MESSAGE_MAP(CLED, CStatic)

//{{AFX_MSG_MAP(CLED)
ON_WM_CREATE()
ON_WM_PAINT()
ON_WM_DESTROY()
ON_WM_SIZE()
//}}AFX_MSG_MAP

END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CLED message handlers

int CLED::OnCreate(LPCREATESTRUCT lpCreateStruct)
{	
	if (CStatic::OnCreate(lpCreateStruct) == -1)		
		return -1;	
	// TODO: Add your specialized creation code here	
	return 0;	
}



void CLED::OnPaint()
{	
	CPaintDC dc(this); // device context for painting	
	// TODO: Add your message handler code here	
	DrawFigure();	
	// Do not call CStatic::OnPaint() for painting messages	
}



void CLED::OnDestroy()
{	
	CStatic::OnDestroy();	
	// TODO: Add your message handler code here	
}



void CLED::SetOnFgColor(COLORREF C)
{
	m_brOnFG->DeleteObject();	
	m_crOnFG=C;	
	m_brOnFG->CreateSolidBrush(m_crOnFG);	
	OnPaint();	
}

COLORREF CLED::GetOnFgColor()
{	
	return  m_crOnFG;	
}

void CLED::SetOffFgColor(COLORREF C)
{	
	m_brOffFG->DeleteObject();	
	m_crOffFG=C;	
	m_brOffFG->CreateSolidBrush(m_crOffFG);	
	OnPaint();	
}

COLORREF CLED::GetOffFgColor()
{	
	return m_crOffFG;	
}

void CLED::SetType(int type)
{	
	if (type<0||type>2)		
	{		
		return;		
	}	
	m_iType=type;	
	OnPaint();	
}

int CLED::GetType()
{	
	return m_iType;	
}



void CLED::DrawFigure()
{	
	CClientDC pdc(this);	
	CBrush b;	
	CRect r;	
	GetClientRect(r);	
	b.CreateSolidBrush(m_crBK);	
	pdc.SelectObject(&b);	
	pdc.FillRect(r,&b);	
	pdc.SetBkMode(TRANSPARENT);	
	if (m_bOn==true)		
	{		
		CPen pen(0,0,m_crOnFG);		
		pdc.SelectObject(&pen);		
		pdc.SelectObject(m_brOnFG);		
	}else		
	{		
		CPen pen(0,0,m_crOffFG);		
		pdc.SelectObject(&pen);		
		pdc.SelectObject(m_brOffFG);		
	}
		
	if (m_iType==0)//circle		
	{		
		CRect r1;		
		if (r.Width()>r.Height())			
		{			
			r1.left=r.left+(r.Width()-r.Height())/2.0;			
			r1.right=r.right-(r.Width()-r.Height())/2.0;			
			r1.top=r.top;			
			r1.bottom=r.bottom;			
		}else			
		{			
			r1.left=r.left;		
			r1.right=r.right;			
			r1.top=r.top+(r.Height()-r.Width())/2.0;			
			r1.bottom=r.bottom-(r.Height()-r.Width())/2.0;			
		}		
		pdc.Ellipse(r1);		
	}
	
	if (m_iType==1)//squre		
	{		
		pdc.Rectangle(r);		
	}
	
	if (m_iType==2)//ellipse		
	{		
		pdc.Ellipse(r);		
	}
	
	if (!m_bTextOn)		
		return;
	
	if(m_bOn)		
		pdc.TextOut(r.CenterPoint().x-m_sOnString.GetLength()*3.5,r.CenterPoint().y-10,m_sOnString,m_sOnString.GetLength());
	else		
		pdc.TextOut(r.CenterPoint().x-m_sOffString.GetLength()*3.5,r.CenterPoint().y-10,m_sOffString,m_sOffString.GetLength());
	
}



void CLED::SetOnOff(bool val)
{
	m_bOn=val;	
	OnPaint();	
}

BOOL CLED::GetOnOff()
{	
	return m_bOn;	
}



void CLED::OnSize(UINT nType, int cx, int cy)
{	
	CStatic::OnSize(nType, cx, cy);	
	OnPaint();
}



void CLED::SetBKColor(COLORREF C)
{	
	m_crBK=C;
	OnPaint();	
}

COLORREF CLED::GetBKColor()
{
	return m_crBK;	
}



void CLED::SetOnText(CString str)
{	
	m_sOnString=str;	
	OnPaint();	
}

CString CLED::GetOnText()
{	
	return m_sOnString;
}

void CLED::SetOffText(CString str)

{
	
	m_sOffString=str;
	
	OnPaint();
	
}

CString CLED::GOffText()

{
	
	return m_sOffString;
	
}

void CLED::SetTextOnOff(bool val)

{
	
	m_bTextOn=val;
	
	OnPaint();
	
	
	
}

bool CLED::GetTextOnOff()
{	
	return m_bTextOn;	
}