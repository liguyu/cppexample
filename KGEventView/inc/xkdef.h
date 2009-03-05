/****************************************************************************/
/* Copyright (c) 2004,chenwenyue											*/
/* All rights reserved.													    */
/* File name:	xkdef.h														*/
/* Author: 	chenwenyue													    */
/* Version: 	1.0															*/
/* Date:		2006.10.30													*/
/* Description:	��������ͷ�ļ�												*/
/* History:																	*/
/*	   Date:																*/
/*	   Author:																*/
/*	   Modification:														*/
/****************************************************************************/
#ifndef __XK_DEF_H
#define __XK_DEF_H

#include "logmgrlib.h"

#pragma   warning(disable:4786)

#include <cstdio>
#include <cstddef>

#include <map>
#include <list>
#include <queue>
#include <vector>
#include <bitset>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include <memory>
#include <cassert>
using namespace std;

#define XK_ASSERT(s)	assert(s)

// ������windowsƽ̨��
#define XK_WORK_ON_WIN_PLATFORM
#if defined(XK_WORK_ON_WIN_PLATFORM)
#ifdef _DEBUG
#	define XK_DEBUG
#endif
#endif
// �ڲ��������Ͷ���
typedef  signed long			Tint;
typedef  unsigned long			Tuint;
typedef  unsigned char			Tbyte;
typedef  short  int				Tshort;
typedef  unsigned short int		Tushort;
typedef  float					Tfloat;
typedef  double					Tdouble;

typedef  unsigned long			Tdword;
typedef	 unsigned short			Tword;

typedef unsigned long			Terror;

typedef  int 					Tbool;

#define  XK_TRUE				1
#define  XK_FALSE				0
#define  XK_NULL				0
// ������Ϣ����
typedef  unsigned long			Tpoint;
typedef  long					Tlparam;
typedef  long					Twparam;

typedef  time_t					Ttime;

#define XK_X_PARAM(lParam)		( lParam & 0x0000ffff )
#define XK_Y_PARAM(lParam)		( (lParam>>16) & 0x0000ffff )
#define XK_MAKE_PARAM(x,y)		( ((y<<16) & 0xffff0000) + ( x & 0x0000ffff ) )	
/* ����ͨ�������: 0x00005000 */
#define		XK_ERR_GETHOSTNAME_FAIL				0x00005001					// �õ�������ʧ��
#define		XK_ERR_GETHOSTADDR_FAIL				0x00005002					// �õ�������ַʧ��
#define		XK_ERR_CREATESOCKET_FAIL			0x00005003					// ����socketʧ��
#define		XK_ERR_SETSOCKETOPT_FAIL			0x00005004					// ����socket����ʧ��
#define		XK_ERR_SETSOCKETBOLCKING_FAIL		0x00005005					// ����socket��������ʧ��
#define		XK_ERR_GETSOCKETOPT_FAIL			0x00005006					// ��ȡsocket����ʧ��
#define		XK_ERR_SOCKETCONNECT_FAIL			0x00005007					// ���ӳ���
#define		XK_ERR_SOCKETACCEPT_FAIL			0x00005008					// ���ܳ���
#define		XK_ERR_SOCKETLISTEN_FAIL			0x00005009					// ��������
#define		XK_ERR_SOCKETSEND_FAIL				0x0000500a					// ���ͱ��ĳ���
#define		XK_ERR_SOCKETRECV_FAIL				0x0000500b					// ���ձ��ĳ���
#define		XK_ERR_SOCKETBIND_FAIL				0x0000500c					// ���׽��ֳ���

#endif  // __XK_DEF_H