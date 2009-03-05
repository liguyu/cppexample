/****************************************************************************/
/* Copyright (c) 2004,chenwenyue											*/
/* All rights reserved.													    */
/* File name:	xkdef.h														*/
/* Author: 	chenwenyue													    */
/* Version: 	1.0															*/
/* Date:		2006.10.30													*/
/* Description:	公共定义头文件												*/
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

// 工作在windows平台下
#define XK_WORK_ON_WIN_PLATFORM
#if defined(XK_WORK_ON_WIN_PLATFORM)
#ifdef _DEBUG
#	define XK_DEBUG
#endif
#endif
// 内部数据类型定义
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
// 用于消息发送
typedef  unsigned long			Tpoint;
typedef  long					Tlparam;
typedef  long					Twparam;

typedef  time_t					Ttime;

#define XK_X_PARAM(lParam)		( lParam & 0x0000ffff )
#define XK_Y_PARAM(lParam)		( (lParam>>16) & 0x0000ffff )
#define XK_MAKE_PARAM(x,y)		( ((y<<16) & 0xffff0000) + ( x & 0x0000ffff ) )	
/* 网络通信类错误: 0x00005000 */
#define		XK_ERR_GETHOSTNAME_FAIL				0x00005001					// 得到主机名失败
#define		XK_ERR_GETHOSTADDR_FAIL				0x00005002					// 得到主机地址失败
#define		XK_ERR_CREATESOCKET_FAIL			0x00005003					// 创建socket失败
#define		XK_ERR_SETSOCKETOPT_FAIL			0x00005004					// 设置socket属性失败
#define		XK_ERR_SETSOCKETBOLCKING_FAIL		0x00005005					// 设置socket阻塞属性失败
#define		XK_ERR_GETSOCKETOPT_FAIL			0x00005006					// 获取socket属性失败
#define		XK_ERR_SOCKETCONNECT_FAIL			0x00005007					// 连接出错
#define		XK_ERR_SOCKETACCEPT_FAIL			0x00005008					// 接受出错
#define		XK_ERR_SOCKETLISTEN_FAIL			0x00005009					// 监听出错
#define		XK_ERR_SOCKETSEND_FAIL				0x0000500a					// 发送报文出错
#define		XK_ERR_SOCKETRECV_FAIL				0x0000500b					// 接收报文出错
#define		XK_ERR_SOCKETBIND_FAIL				0x0000500c					// 绑定套接字出错

#endif  // __XK_DEF_H