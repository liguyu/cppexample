// WTLClockView.h : interface of the CWTLClockView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WTLCLOCKVIEW_H__C2B91381_24DC_4CD3_9136_B49760C3D63D__INCLUDED_)
#define AFX_WTLCLOCKVIEW_H__C2B91381_24DC_4CD3_9136_B49760C3D63D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CWTLClockView : public CWindowImpl<CWTLClockView>
{
public:
    CWTLClockView() : m_bClockRunning(true)
        { }

    DECLARE_WND_CLASS(NULL)

    BOOL PreTranslateMessage(MSG* pMsg);

    BEGIN_MSG_MAP_EX(CWTLClockView)
        MESSAGE_HANDLER(WM_PAINT, OnPaint)
        MSG_WM_CREATE(OnCreate)
        MSG_WM_DESTROY(OnDestroy)
        MSG_WM_TIMER(OnTimer)
        MSG_WM_ERASEBKGND(OnEraseBkgnd)
    END_MSG_MAP()

    LRESULT OnCreate ( LPCREATESTRUCT lpcs )
    {
        GetLocalTime ( &m_stLastTime );
        SetTimer ( 1, 1000 );
        SetMsgHandled(false);
        return 0;
    }

    void OnDestroy()
    {
        KillTimer(1);
        SetMsgHandled(false);
    }

    void OnTimer ( UINT uTimerID/*, TIMERPROC pTimerProc*/ )
    {
        if ( 1 != uTimerID )
            SetMsgHandled(false);
        else
            {
            // If the clock is running, get the current time & redraw our
            // window so the new time appears.
            if ( m_bClockRunning )
                {
                GetLocalTime ( &m_stLastTime );
                RedrawWindow();
                }
            }
    }

	LRESULT OnEraseBkgnd ( HDC hdc )
    {
        return 1;   // OnPaint will draw the whole window
    }

    LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

    void StartClock() 
    { 
        m_bClockRunning = true; 
        GetLocalTime ( &m_stLastTime );
        RedrawWindow();
    }

    void StopClock() 
    { 
        m_bClockRunning = false; 
        RedrawWindow();
    }

protected:
    SYSTEMTIME m_stLastTime;
    bool       m_bClockRunning;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WTLCLOCKVIEW_H__C2B91381_24DC_4CD3_9136_B49760C3D63D__INCLUDED_)
