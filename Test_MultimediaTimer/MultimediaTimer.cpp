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
	m_MaxTimeSpace  = 2560;      //���ʱ�����Ϊ 2560 ms [2^X]          
	m_MinTimeSpace  = 10  ;      //��С��ʱ�����Ϊ 10 ms  [ԭ��]
	//
	m_CurTimerRes   = 50;        //��ʼ��ý�嶨ʱ���10ms
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

//������ʱ��
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

//ֹͣ��ʱ��
void CMultimediaTimer::Stop()
{
	timeKillEvent(m_TimerID);
	m_bTimerStop  = TRUE;
	m_bTimerPause = FALSE;
}

//��ͣ��ʱ��
void CMultimediaTimer::Pause()
{
	timeKillEvent(m_TimerID);
	if(!m_bTimerStop && !m_bTimerPause)
	{
		m_bTimerPause = TRUE;
	}
}

//�ӿ첥���ٶȣ����ٶ�ʱ���
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

//���������ٶȣ����Ӷ�ʱ���
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

//���ö�ʱʱ����ڷ�Χ���Դ��ε�Start��SetCurRes������Ч
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

//��ö�ʱʱ�䷶Χ
void CMultimediaTimer::GetRange(UINT &Lower, UINT &Upper) const
{
	Lower = m_MinTimeSpace;
	Upper = m_MaxTimeSpace;
}

//��õ�ǰʱ����
void CMultimediaTimer::SetCurRes(UINT timerCurRes)
{
	m_CurTimerRes = max(min(timerCurRes,m_tc.wPeriodMax),m_wAccuracy);
}

//���ûص������ʹ������
void CMultimediaTimer::SetProcParameter(LPTIMECALLBACK callbackproc, DWORD dwUser)
{
	m_pCallBackProc = callbackproc;
	m_dwUser = dwUser;
}

//��ʱ���Ƿ�����
BOOL CMultimediaTimer::IsStart() const
{
	return (!m_bTimerPause && !m_bTimerStop);
}

//��ʱ���Ƿ��Ѿ�����
BOOL CMultimediaTimer::IsStop() const
{
	return m_bTimerStop;
}

//��ö�ʱ����ͣ״̬
BOOL CMultimediaTimer::IsPause() const
{
	return m_bTimerPause;
}

//����Res��״̬
UINT CMultimediaTimer::GetResState(UINT Res) const
{
	if(Res < m_MinTimeSpace)
		return TIMER_LOWLIMIT;
	if(Res > m_MaxTimeSpace)
		return  TIMER_UPLIMIT;
	return TIMER_NORMAL;
}

//��ö�ʱ��״̬
UINT CMultimediaTimer::GetResState() const
{
	return GetResState(m_CurTimerRes);	
}

//��õ�ǰʱ����
UINT CMultimediaTimer::GetCurRes() const
{
	return m_CurTimerRes;
}

//��ö�ʱ���ֱ��ʷ�Χ
const TIMECAPS& CMultimediaTimer::timeGetDevCaps() const
{
	return m_tc;
}

//���ٶ���ߵ�ʱ�򱻵���
void CMultimediaTimer::OnSpeedUp(UINT &timerCurRes)
{//SpeedUp algorithm
	timerCurRes = timerCurRes / 2;
}

//���ٶȽ��͵�ʱ�򱻵���
void CMultimediaTimer::OnSlowDown(UINT &timerCurRes)
{//SlowDown algorithm
	timerCurRes = timerCurRes * 2;
}
