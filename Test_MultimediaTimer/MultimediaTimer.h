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

#include "Mmsystem.h"                     //ý�嶨ʱ��
#pragma comment(lib,"winmm")

const UINT TIMER_ACCURACY =  1;           //����ʱ�ӷֱ��ʣ���msΪ��λ
#define TIMER_UPLIMIT  0x0001
#define TIMER_LOWLIMIT 0x0002
#define TIMER_NORMAL   0x0004

class CMultimediaTimer  
{	
public:
	CMultimediaTimer();
	virtual ~CMultimediaTimer();

public://���ſ��ƺ���
	void Start(UINT timerCurRes, BOOL bOneShot);    //������ʱ��
	void Start(BOOL bOneShot = FALSE);			    
	void Stop();									//ֹͣ��ʱ��
	void Pause();                                   //��ͣ��ʱ��
	BOOL SpeedUp();								    //�ӿ첥���ٶ�
	BOOL SlowDown();								//���������ٶ�
	void SetRange(UINT Lower, UINT Upper);          //���ö�ʱʱ����ڷ�Χ���Դ��ε�Start��SetCurRes������Ч
	void GetRange(UINT &Lower, UINT &Upper) const;  //��ö�ʱʱ�䷶Χ
	void SetCurRes(UINT timerCurRes);               //��õ�ǰʱ����
	void SetProcParameter(LPTIMECALLBACK callbackproc, DWORD dwUser);            
	                                                //���ûص������ʹ������
	BOOL IsStart() const;                           //��ʱ���Ƿ�����
	BOOL IsStop() const;						    //��ʱ���Ƿ���ֹ
	BOOL IsPause() const;							//��ʱ���Ƿ���ͣ
	UINT GetResState() const;	    	            //��ö�ʱ��ʱ����״̬
	UINT GetCurRes() const;					        //��õ�ǰʱ����
	const TIMECAPS& timeGetDevCaps() const;         //��ö�ʱ���ֱ��ʷ�Χ
protected:
	virtual void OnSpeedUp(UINT &timerCurRes);      //���ٶ���ߵ�ʱ�򱻵���
	virtual void OnSlowDown(UINT &timerCurRes);     //���ٶȽ��͵�ʱ�򱻵���
	UINT GetResState(UINT Res) const;               //����Res��״̬
private:
	TIMECAPS m_tc;                                   
	UINT	 m_CurTimerRes;                         //��ǰʱ����
	BOOL	 m_bTimerStop;                          //��ʱ���Ƿ��Ѿ�����
	BOOL     m_bTimerPause;                         //��ʱ���Ƿ���ͣ
	DWORD	 m_dwUser;                              //�ص������Ĵ������
	UINT	 m_wAccuracy;                           //ʹ�õķֱ��� 
	UINT	 m_TimerID;                             //���嶨ʱ�����
	LPTIMECALLBACK m_pCallBackProc;                 //�ص�����ָ��

	UINT m_MaxTimeSpace;                            //���ʱ�����
	UINT m_MinTimeSpace;                            //��С��ʱ�����
};

#endif // !defined(TY_MultimediaTimer)
