// MultimediaTimer.cpp: implementation of the CMultimediaTimer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MultimediaTimer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMultimediaTimer::CMultimediaTimer()
{
	m_MaxTimeSpace  = 2560;      //最大定时器间隔为 2560 ms [2^X]          
	m_MinTimeSpace  = 10  ;      //最小定时器间隔为 10 ms  [原速]
	//
	m_CurTimerRes   = 50;        //初始多媒体定时间隔10ms
	m_bTimerStop    = TRUE;
	m_bTimerPause   = TRUE;
	m_pCallBackProc = NULL;

	if(::timeGetDevCaps(&m_tc,sizeof(TIMECAPS)) == TIMERR_NOERROR) 	
	{ 
		m_wAccuracy = min(max(m_tc.wPeriodMin,TIMER_ACCURACY),m_tc.wPeriodMax);
		timeBeginPeriod(m_wAccuracy); 
	}	
}

CMultimediaTimer::~CMultimediaTimer()
{
	timeKillEvent(m_TimerID);
	timeEndPeriod(m_wAccuracy);
}

//启动定时器
void CMultimediaTimer::Start(BOOL bOneShot /*= FALSE/**/)
{
	ASSERT(m_pCallBackProc != NULL);
	UINT fuEvent = (bOneShot ? TIME_ONESHOT : TIME_PERIODIC);
	if(m_bTimerStop || m_bTimerPause)
	{
		m_TimerID = timeSetEvent(m_CurTimerRes, 
			                     m_wAccuracy,
								 (LPTIMECALLBACK) m_pCallBackProc,
								 m_dwUser,
								 fuEvent);
		m_bTimerStop  = FALSE;
		m_bTimerPause = FALSE;
	}
}

void CMultimediaTimer::Start(UINT timerCurRes, BOOL bOneShot)
{
	ASSERT(m_pCallBackProc != NULL);
	m_CurTimerRes = max(min(timerCurRes,m_tc.wPeriodMax),m_wAccuracy);
	UINT fuEvent  = (bOneShot ? TIME_ONESHOT : TIME_PERIODIC);
	if(m_bTimerStop || m_bTimerPause)
	{
		m_TimerID = timeSetEvent(m_CurTimerRes, 
			m_wAccuracy,
			m_pCallBackProc,
			m_dwUser,
			fuEvent);
		m_bTimerStop  = FALSE;
		m_bTimerPause = FALSE;
	}
}

//停止定时器
void CMultimediaTimer::Stop()
{
	timeKillEvent(m_TimerID);
	m_bTimerStop  = TRUE;
	m_bTimerPause = FALSE;
}

//暂停定时器
void CMultimediaTimer::Pause()
{
	timeKillEvent(m_TimerID);
	if(!m_bTimerStop && !m_bTimerPause)
	{
		m_bTimerPause = TRUE;
	}
}

//加快播放速度，减少定时间隔
BOOL CMultimediaTimer::SpeedUp()
{
	switch(GetResState())
	{
	case TIMER_UPLIMIT:
		m_CurTimerRes = m_MaxTimeSpace;
		break;
	case TIMER_LOWLIMIT:
		m_CurTimerRes = m_MinTimeSpace;
		break;
	default:;
	}
	UINT nOldRes = m_CurTimerRes;
	OnSpeedUp(nOldRes);
	if(GetResState(nOldRes) == TIMER_NORMAL)
		m_CurTimerRes = nOldRes;
	else return FALSE;
	
	if(!m_bTimerStop && !m_bTimerPause)
	{
		timeKillEvent(m_TimerID);
		m_TimerID = timeSetEvent(m_CurTimerRes, 
			m_wAccuracy,
			m_pCallBackProc,
			m_dwUser,
			TIME_PERIODIC);
	}
	return TRUE;
}

//减慢播放速度，增加定时间隔
BOOL CMultimediaTimer::SlowDown()
{	
	switch(GetResState())
	{
	case TIMER_UPLIMIT:
		m_CurTimerRes = m_MaxTimeSpace;
		break;
	case TIMER_LOWLIMIT:
		m_CurTimerRes = m_MinTimeSpace;
		break;
	default:;
	}
	UINT nOldRes = m_CurTimerRes;
	OnSlowDown(nOldRes);
	if(GetResState(nOldRes) == TIMER_NORMAL)
		m_CurTimerRes = nOldRes;
	else return FALSE;
	
	if(!m_bTimerStop && !m_bTimerPause)
	{
		timeKillEvent(m_TimerID);
		m_TimerID = timeSetEvent(m_CurTimerRes, 
			m_wAccuracy,
			m_pCallBackProc,
			m_dwUser,
			TIME_PERIODIC);
	}
	return TRUE;
}

//设置定时时间调节范围，对带参的Start和SetCurRes函数无效
void CMultimediaTimer::SetRange(UINT Lower, UINT Upper)
{
	//Make change
	if(Lower > Upper)
	{
		UINT temp = Lower;
		Lower = Upper;
		Upper = temp;
	}
	m_MinTimeSpace = max(min(Lower,m_tc.wPeriodMax),m_wAccuracy);
	m_MaxTimeSpace = max(min(m_tc.wPeriodMax,Upper),m_MinTimeSpace);
}

//获得定时时间范围
void CMultimediaTimer::GetRange(UINT &Lower, UINT &Upper) const
{
	Lower = m_MinTimeSpace;
	Upper = m_MaxTimeSpace;
}

//获得当前时间间隔
void CMultimediaTimer::SetCurRes(UINT timerCurRes)
{
	m_CurTimerRes = max(min(timerCurRes,m_tc.wPeriodMax),m_wAccuracy);
}

//设置回调函数和传入参数
void CMultimediaTimer::SetProcParameter(LPTIMECALLBACK callbackproc, DWORD dwUser)
{
	m_pCallBackProc = callbackproc;
	m_dwUser = dwUser;
}

//定时器是否运行
BOOL CMultimediaTimer::IsStart() const
{
	return (!m_bTimerPause && !m_bTimerStop);
}

//定时器是否已经启动
BOOL CMultimediaTimer::IsStop() const
{
	return m_bTimerStop;
}

//获得定时器暂停状态
BOOL CMultimediaTimer::IsPause() const
{
	return m_bTimerPause;
}

//返回Res的状态
UINT CMultimediaTimer::GetResState(UINT Res) const
{
	if(Res < m_MinTimeSpace)
		return TIMER_LOWLIMIT;
	if(Res > m_MaxTimeSpace)
		return  TIMER_UPLIMIT;
	return TIMER_NORMAL;
}

//获得定时器状态
UINT CMultimediaTimer::GetResState() const
{
	return GetResState(m_CurTimerRes);	
}

//获得当前时间间隔
UINT CMultimediaTimer::GetCurRes() const
{
	return m_CurTimerRes;
}

//获得定时器分辨率范围
const TIMECAPS& CMultimediaTimer::timeGetDevCaps() const
{
	return m_tc;
}

//当速度提高的时候被调用
void CMultimediaTimer::OnSpeedUp(UINT &timerCurRes)
{//SpeedUp algorithm
	timerCurRes = timerCurRes / 2;
}

//当速度降低的时候被调用
void CMultimediaTimer::OnSlowDown(UINT &timerCurRes)
{//SlowDown algorithm
	timerCurRes = timerCurRes * 2;
}
