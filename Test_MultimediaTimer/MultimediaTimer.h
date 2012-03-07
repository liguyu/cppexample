//////////////////////////////////////////////////////////////////////
// MultimediaTimer.h: interface for the CMultimediaTimer class.
// 2005.3.9
// If this code works, it was written by TY.
// If not, I don't know who wrote it.
//
// Use this CMultimediaTimer class as the fellows:
// step 1 : create a CMultimediaTimer object
// step 2 : define the Timer Callback Proc function in your programe, 
//          the function must be the same type as below:
//          void PASCAL TimerProc(UINT wTimerID, UINT msg,DWORD dwUser,DWORD dwl,DWORD dw2);
// step 3 : use class member function SetProcParameter, set TimerProc and Parameter,
//          if you need you can also use SetRange & SetCurRes functions to Modify
//          CMultimediaTimer object initial state in this step
// step 4 : use class play control function as you need
// Welcome any advice   wangjianddy@gmail.com
//////////////////////////////////////////////////////////////////////

#if !defined(TY_MultimediaTimer)
#define TY_MultimediaTimer

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Mmsystem.h"                     //媒体定时器
#pragma comment(lib,"winmm")

const UINT TIMER_ACCURACY =  1;           //定义时钟分辨率，以ms为单位
#define TIMER_UPLIMIT  0x0001
#define TIMER_LOWLIMIT 0x0002
#define TIMER_NORMAL   0x0004

class CMultimediaTimer  
{	
public:
	CMultimediaTimer();
	virtual ~CMultimediaTimer();

public://播放控制函数
	void Start(UINT timerCurRes, BOOL bOneShot);    //启动定时器
	void Start(BOOL bOneShot = FALSE);			    
	void Stop();									//停止定时器
	void Pause();                                   //暂停定时器
	BOOL SpeedUp();								    //加快播放速度
	BOOL SlowDown();								//减慢播放速度
	void SetRange(UINT Lower, UINT Upper);          //设置定时时间调节范围，对带参的Start和SetCurRes函数无效
	void GetRange(UINT &Lower, UINT &Upper) const;  //获得定时时间范围
	void SetCurRes(UINT timerCurRes);               //获得当前时间间隔
	void SetProcParameter(LPTIMECALLBACK callbackproc, DWORD dwUser);            
	                                                //设置回调函数和传入参数
	BOOL IsStart() const;                           //定时器是否运行
	BOOL IsStop() const;						    //定时器是否终止
	BOOL IsPause() const;							//定时器是否暂停
	UINT GetResState() const;	    	            //获得定时器时间间隔状态
	UINT GetCurRes() const;					        //获得当前时间间隔
	const TIMECAPS& timeGetDevCaps() const;         //获得定时器分辨率范围
protected:
	virtual void OnSpeedUp(UINT &timerCurRes);      //当速度提高的时候被调用
	virtual void OnSlowDown(UINT &timerCurRes);     //当速度降低的时候被调用
	UINT GetResState(UINT Res) const;               //返回Res的状态
private:
	TIMECAPS m_tc;                                   
	UINT	 m_CurTimerRes;                         //当前时间间隔
	BOOL	 m_bTimerStop;                          //定时器是否已经启动
	BOOL     m_bTimerPause;                         //定时器是否暂停
	DWORD	 m_dwUser;                              //回调函数的传入参数
	UINT	 m_wAccuracy;                           //使用的分辨率 
	UINT	 m_TimerID;                             //定义定时器句柄
	LPTIMECALLBACK m_pCallBackProc;                 //回调函数指针

	UINT m_MaxTimeSpace;                            //最大定时器间隔
	UINT m_MinTimeSpace;                            //最小定时器间隔
};

#endif // !defined(TY_MultimediaTimer)
