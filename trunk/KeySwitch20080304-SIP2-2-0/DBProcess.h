// DBProcess.h: interface for the CDBProcess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBPROCESS_H__C672465D_72F5_4209_ADAA_83F30D5B68ED__INCLUDED_)
#define AFX_DBPROCESS_H__C672465D_72F5_4209_ADAA_83F30D5B68ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDBProcess  
{
public:
	_ConnectionPtr		pConn;												//ADO连接对象
	_RecordsetPtr		pRs;												//本类公用的唯一查询结果集合
	BOOL				isConnect;											//标记当前状态，是否连接上数据库
	char				pConnStr[150];										//数据库连接串
	long				mRecordcount;										//记录数

	CDBProcess();
	virtual ~CDBProcess();

	void	SetConnectStr(char *pSourceStr);								//设置连接串
	BOOL	ConnectDB();													//连接数据库
	BOOL	ExcueteSQL(char *SQL);											//执行一条更新的SQL语句
	BOOL	ExcueteQuery(char *SQL);										//执行一个查询
	long	getRecCount();													//获取记录条数
	void	CloseDB();														//关闭数据库	
};

#endif // !defined(AFX_DBPROCESS_H__C672465D_72F5_4209_ADAA_83F30D5B68ED__INCLUDED_)
