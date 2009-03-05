/****************************************************************************/
/* Copyright (c) 2004,陈文越												*/
/* 所有权利保留,未经作者允许,任何组织和个人不得将些代码用于商业盈利			*/
/* 用途,否则由此产生的一切法律责任由此组织或个人承担.						*/
/* 文件名:	xkkernel.cpp													*/
/* 作	者: 	陈文越                                                      */
/* 版	本: 	1.0                                                         */
/* 创建日期:		2006.10.30                                              */
/* 描述:	运行内核实现文件												*/
/* 历史信息:                                                                */
/*		修改日期:															*/
/*		修改人:                                                             */
/*		修改内容:															*/
/****************************************************************************/
#include "xkmsgqueue.h"

CXKTransKeyInfo::CXKTransKeyInfo()
: m_nType(XK_KEY_TYPE_UNKNOWN)
, m_nPos(0)
, m_nLen(0)
{
}

CXKTransKeyInfo::~CXKTransKeyInfo()
{
}

CXKTransKey::CXKTransKey()
: m_Keys(XK_NULL)
, m_nCnt(0)
, m_nIndex(0)
{
}

CXKTransKey::~CXKTransKey()
{
	Destroy();
}

Tbool CXKTransKey::Create(Tuint nCnt)
{
	XK_ASSERT( m_nCnt == 0 );
	XK_ASSERT( XK_NULL == m_Keys );
	XK_ASSERT( 0 == m_nIndex );
	if(nCnt==0)	return XK_FALSE;

	m_Keys	= new CXKTransKeyInfo[nCnt];
	if( m_Keys == XK_NULL )	return XK_FALSE;
	m_nCnt	= nCnt;

	return XK_TRUE;
}

void CXKTransKey::Destroy()
{
	if( m_Keys != XK_NULL)
	{
		delete []m_Keys;
		m_Keys	= XK_NULL;
	}

	m_nCnt		= 0;
	m_nIndex	= 0;
}
//////////////////////////////////////////////////////////////////////
// CXKTransBuf
//////////////////////////////////////////////////////////////////////
CXKTransBuf::CXKTransBuf()
: m_nSize(0)
, m_pData(XK_NULL)
{
}
CXKTransBuf::~CXKTransBuf()
{
	Destroy();
}

void CXKTransBuf::Destroy()
{
	if( XK_NULL != m_pData )
	{
		XK_ASSERT( m_nSize > 0 );
		delete m_pData;
		m_nSize		= 0;
		m_pData		= XK_NULL;
	}
}

Tbool CXKTransBuf::Create(Tuint nSize)
{
	Tbool bRet	= XK_TRUE;
	// 防止内存泄露
	Destroy();

	XK_ASSERT( XK_NULL == m_pData );
	XK_ASSERT( nSize > 0 );
	
	m_pData		= new Tbyte( nSize );
	if( XK_NULL != m_pData )
	{
		m_nSize		= nSize;
	}
	else
	{
		bRet		= XK_FALSE;
	}

	return bRet;
}

void CXKTransBuf::CopyData(const Tbyte *ptr, Tuint nLen)
{
	XK_ASSERT( XK_NULL != ptr );
	XK_ASSERT( nLen > 0 );
	if( XK_NULL == ptr || nLen == 0 ) return;
	// 防止内存泄露
	Destroy();
	XK_ASSERT( XK_NULL == m_pData );
	
	m_pData		= new Tbyte( nLen );
	if( XK_NULL != m_pData )
	{
		m_nSize		= nLen;
		memcpy(m_pData, ptr, nLen);
	}
}

Tbool CXKTransBuf::GetKey(Tuint nPos, Tbyte &data)
{
	XK_ASSERT( XK_NULL != m_pData );
	XK_ASSERT( m_nSize > 0 );
	Tbool	bRet	= XK_TRUE;
	Tuint nLen	= nPos + sizeof(Tbyte);
	if( nLen <= m_nSize && XK_NULL != m_pData )
	{
		data	= m_pData[nPos];
	}
	else
	{
		bRet	= XK_FALSE;
	}

	return bRet;
}

Tbool CXKTransBuf::GetKey(Tuint nPos, Tint &data)
{
	XK_ASSERT( XK_NULL != m_pData );
	XK_ASSERT( m_nSize > 0 );
	Tbool	bRet	= XK_TRUE;
	Tuint nLen	= nPos + sizeof(Tint);
	if( nLen <= m_nSize && XK_NULL != m_pData )
	{
		if(IsLittleEndian())	// LITTLE_ENDIAN
		{
			XK_ASSERT( sizeof(Tint) == 4 );
			data	= m_pData[nPos]<<24 | m_pData[nPos + 1]<<16 | m_pData[nPos + 2]<<8 | m_pData[nPos + 3];
		}
		else
		{
			data	= *(Tint*)(&m_pData[nPos]);
		}
	}
	else
	{
		bRet	= XK_FALSE;
	}

	return bRet;
}
Tbool CXKTransBuf::GetKey(Tuint nPos, Tuint &data)
{
	XK_ASSERT( XK_NULL != m_pData );
	XK_ASSERT( m_nSize > 0 );
	Tbool	bRet	= XK_TRUE;
	Tuint nLen	= nPos + sizeof(Tuint);
	if( nLen <= m_nSize && XK_NULL != m_pData )
	{
		if(IsLittleEndian())	// LITTLE_ENDIAN
		{
			XK_ASSERT( sizeof(Tuint) == 4 );
			data	= m_pData[nPos]<<24 | m_pData[nPos + 1 ]<<16 | m_pData[nPos + 2]<<8 | m_pData[nPos + 3];
		}
		else
		{
			data	= *(Tuint*)(&m_pData[nPos]);
		}
	}
	else
	{
		bRet	= XK_FALSE;
	}

	return bRet;
}
Tbool CXKTransBuf::GetKey(Tuint nPos, Tfloat &data)
{
	XK_ASSERT( XK_NULL != m_pData );
	XK_ASSERT( m_nSize > 0 );
	Tbool	bRet	= XK_TRUE;
	Tuint nLen	= nPos + sizeof(Tfloat);
	if( nLen <= m_nSize && XK_NULL != m_pData )
	{
		if(IsLittleEndian())	// LITTLE_ENDIAN
		{
			XK_ASSERT( sizeof(Tfloat) == 4 );
			data	= m_pData[nPos]<<24 | m_pData[nPos + 1 ]<<16 | m_pData[nPos + 2]<<8 | m_pData[nPos + 3];
		}
		else
		{
			data	= *(Tfloat*)(&m_pData[nPos]);
		}
	}
	else
	{
		bRet	= XK_FALSE;
	}

	return bRet;
}
Tbool CXKTransBuf::GetKey(Tuint nPos, Tdouble &data)
{
	XK_ASSERT( XK_NULL != m_pData );
	XK_ASSERT( m_nSize > 0 );
	Tbool	bRet	= XK_TRUE;
	Tuint nLen	= nPos + sizeof(Tdouble);
	if( nLen <= m_nSize && XK_NULL != m_pData )
	{
		if(IsLittleEndian())	// LITTLE_ENDIAN
		{
			XK_ASSERT( sizeof(Tdouble) == 8 );
			data	= m_pData[nPos]<<56 | m_pData[nPos + 1 ]<<48 | m_pData[nPos + 2]<<40 | m_pData[nPos + 3]<<32
					| m_pData[nPos + 4]<<24 | m_pData[nPos + 5 ]<<16 | m_pData[nPos + 6]<<8 | m_pData[nPos + 7];
		}
		else
		{
			data	= *(Tdouble*)(&m_pData[nPos]);
		}
	}
	else
	{
		bRet	= XK_FALSE;
	}

	return bRet;
}
Tbool CXKTransBuf::GetKey(Tuint nPos, Tshort &data)
{
	XK_ASSERT( XK_NULL != m_pData );
	XK_ASSERT( m_nSize > 0 );
	Tbool	bRet	= XK_TRUE;
	Tuint nLen	= nPos + sizeof(Tshort);
	if( nLen <= m_nSize && XK_NULL != m_pData )
	{
		if(IsLittleEndian())	// LITTLE_ENDIAN
		{
			XK_ASSERT(sizeof(Tshort) == 2 );
			data	= m_pData[nPos]<<8 | m_pData[nPos + 1];
		}
		else
		{
			data	= *(Tshort*)(&m_pData[nPos]);
		}
	}
	else
	{
		bRet	= XK_FALSE;
	}

	return bRet;
}
Tbool CXKTransBuf::GetKey(Tuint nPos, Tbool &data)
{
	XK_ASSERT( XK_NULL != m_pData );
	XK_ASSERT( m_nSize > 0 );
	Tbool	bRet	= XK_TRUE;
	Tuint nLen	= nPos + sizeof(Tbool);
	if( nLen <= m_nSize && XK_NULL != m_pData )
	{
		if(IsLittleEndian())	// LITTLE_ENDIAN
		{
			XK_ASSERT(sizeof(Tbool) == 4 );
			data	= m_pData[nPos]<<24 | m_pData[nPos + 1]<<16 | m_pData[nPos + 2]<<8 | m_pData[nPos + 3];
		}
		else
		{
			data	= *(Tbool*)(&m_pData[nPos]);
		}
	}
	else
	{
		bRet	= XK_FALSE;
	}

	return bRet;
}
Tbool CXKTransBuf::GetKey(Tuint nPos, string &data)
{
	XK_ASSERT( XK_NULL != m_pData );
	XK_ASSERT( m_nSize > 0 );
	Tbool	bRet	= XK_TRUE;
	Tuint nLen	= m_nSize - nPos;
	if( nLen > 0 && XK_NULL != m_pData )
	{
		data	= (char*)(&m_pData[nPos]);
	}
	else
	{
		bRet	= XK_FALSE;
	}

	return bRet;
}
Tbool CXKTransBuf::GetKey(Tuint nPos, Tbyte *data, Tuint nL)
{
	XK_ASSERT( XK_NULL != m_pData );
	XK_ASSERT( m_nSize > 0 );
	Tbool	bRet	= XK_TRUE;
	Tuint nLen	= m_nSize - nPos;
	if( nLen > 0 && XK_NULL != m_pData )
	{
		memcpy(data, (char*)m_pData[nPos], nLen);
	}
	else
	{
		bRet	= XK_FALSE;
	}

	return bRet;
}

Tbool CXKTransBuf::SetKey(Tuint nPos, Tbyte data)
{
	XK_ASSERT( XK_NULL != m_pData );
	XK_ASSERT( m_nSize > 0 );
	Tbool	bRet	= XK_TRUE;
	Tuint nLen	= nPos + sizeof(Tbyte);
	if( nLen <= m_nSize && XK_NULL != m_pData )
	{
		m_pData[nPos]	= data;
	}
	else
	{
		bRet	= XK_FALSE;
	}

	return bRet;
}

Tbool CXKTransBuf::SetKey(Tuint nPos, Tint data)
{
	XK_ASSERT( XK_NULL != m_pData );
	XK_ASSERT( m_nSize > 0 );
	Tbool	bRet	= XK_TRUE;
	Tuint nLen	= nPos + sizeof(Tint);
	if( nLen <= m_nSize && XK_NULL != m_pData )
	{
		if(IsLittleEndian())	// LITTLE_ENDIAN
		{
			XK_ASSERT(sizeof(Tint) == 4 );
			Tbyte* p	= (Tbyte*)&data;
			nLen	= sizeof(data);
			while( nLen > 0 )
			{
				m_pData[nPos + nLen -1 ]	= *p;
				nLen--;
				p++;
			}
		}
		else
		{
			*(Tint*)(&m_pData[nPos])	= data;
		}
	}
	else
	{
		bRet	= XK_FALSE;
	}

	return bRet;
}
Tbool CXKTransBuf::SetKey(Tuint nPos, Tuint data)
{
	XK_ASSERT( XK_NULL != m_pData );
	XK_ASSERT( m_nSize > 0 );
	Tbool	bRet	= XK_TRUE;
	Tuint nLen	= nPos + sizeof(Tuint);
	if( nLen <= m_nSize && XK_NULL != m_pData )
	{
		if(IsLittleEndian())	// LITTLE_ENDIAN
		{
			XK_ASSERT(sizeof(Tuint) == 4 );
			Tbyte* p	= (Tbyte*)&data;
			nLen	= sizeof(data);
			while( nLen > 0 )
			{
				m_pData[nPos + nLen -1 ]	= *p;
				nLen--;
				p++;
			}
		}
		else
		{
			*(Tuint*)(&m_pData[nPos])	= data;
		}
	}
	else
	{
		bRet	= XK_FALSE;
	}

	return bRet;
}
Tbool CXKTransBuf::SetKey(Tuint nPos, Tfloat data)
{
	XK_ASSERT( XK_NULL != m_pData );
	XK_ASSERT( m_nSize > 0 );
	Tbool	bRet	= XK_TRUE;
	Tuint nLen	= nPos + sizeof(Tfloat);
	if( nLen <= m_nSize && XK_NULL != m_pData )
	{
		if(IsLittleEndian())	// LITTLE_ENDIAN
		{
			XK_ASSERT(sizeof(Tfloat) == 4 );
			Tbyte* p	= (Tbyte*)&data;
			nLen	= sizeof(data);
			while( nLen > 0 )
			{
				m_pData[nPos + nLen -1 ]	= *p;
				nLen--;
				p++;
			}
		}
		else
		{
			*(Tfloat*)(&m_pData[nPos])	= data;
		}
	}
	else
	{
		bRet	= XK_FALSE;
	}

	return bRet;
}
Tbool CXKTransBuf::SetKey(Tuint nPos, Tdouble data)
{
	XK_ASSERT( XK_NULL != m_pData );
	XK_ASSERT( m_nSize > 0 );
	Tbool	bRet	= XK_TRUE;
	Tuint nLen	= nPos + sizeof(Tdouble);
	if( nLen <= m_nSize && XK_NULL != m_pData )
	{
		if(IsLittleEndian())	// LITTLE_ENDIAN
		{
			XK_ASSERT(sizeof(Tdouble) == 8 );
			Tbyte* p	= (Tbyte*)&data;
			nLen	= sizeof(data);
			while( nLen > 0 )
			{
				m_pData[nPos + nLen -1 ]	= *p;
				nLen--;
				p++;
			}
		}
		else
		{
			*(Tdouble*)(&m_pData[nPos])	= data;
		}
	}
	else
	{
		bRet	= XK_FALSE;
	}

	return bRet;
}
Tbool CXKTransBuf::SetKey(Tuint nPos, Tshort data)
{
	XK_ASSERT( XK_NULL != m_pData );
	XK_ASSERT( m_nSize > 0 );
	Tbool	bRet	= XK_TRUE;
	Tuint nLen	= nPos + sizeof(Tshort);
	if( nLen <= m_nSize && XK_NULL != m_pData )
	{
		if(IsLittleEndian())	// LITTLE_ENDIAN
		{
			XK_ASSERT(sizeof(Tshort) == 2 );
			Tbyte* p	= (Tbyte*)&data;
			nLen	= sizeof(data);
			while( nLen > 0 )
			{
				m_pData[nPos + nLen -1 ]	= *p;
				nLen--;
				p++;
			}
		}
		else
		{
			*(Tshort*)(&m_pData[nPos])	= data;
		}
	}
	else
	{
		bRet	= XK_FALSE;
	}

	return bRet;
}
Tbool CXKTransBuf::SetKey(Tuint nPos, Tbool data)
{
	XK_ASSERT( XK_NULL != m_pData );
	XK_ASSERT( m_nSize > 0 );
	Tbool	bRet	= XK_TRUE;
	Tuint nLen	= nPos + sizeof(Tbool);
	if( nLen <= m_nSize && XK_NULL != m_pData )
	{
		if(IsLittleEndian())	// LITTLE_ENDIAN
		{
			XK_ASSERT(sizeof(Tbool) == 2 );
			Tbyte* p	= (Tbyte*)&data;
			nLen	= sizeof(data);
			while( nLen > 0 )
			{
				m_pData[nPos + nLen -1 ]	= *p;
				nLen--;
				p++;
			}
		}
		else
		{
			*(Tbool*)(&m_pData[nPos])	= data;
		}
	}
	else
	{
		bRet	= XK_FALSE;
	}

	return bRet;
}
Tbool CXKTransBuf::SetKey(Tuint nPos, const string &data)
{
	XK_ASSERT( XK_NULL != m_pData );
	XK_ASSERT( m_nSize > 0 );
	Tbool	bRet	= XK_TRUE;
	Tuint nLen	= nPos + data.length();
	if( nLen <= m_nSize && XK_NULL != m_pData )
	{
		memcpy(&m_pData[nPos], data.c_str(), nLen - nPos + 1);
	}
	else
	{
		bRet	= XK_FALSE;
	}

	return bRet;
}
Tbool CXKTransBuf::SetKey(Tuint nPos, const Tbyte *data, Tuint nL)
{
	XK_ASSERT( XK_NULL != m_pData );
	XK_ASSERT( m_nSize > 0 );
	Tbool	bRet	= XK_TRUE;
	Tuint nLen	= nPos + nL;
	if( nLen <= m_nSize && XK_NULL != m_pData )
	{
		memcpy(&m_pData[nPos], data, nL + 1);
	}
	else
	{
		bRet	= XK_FALSE;
	}

	return bRet;
}
//////////////////////////////////////////////////////////////////////
// CXKTransData
//////////////////////////////////////////////////////////////////////
#define	theTransMap	(CXKTransData::GetTransKeyMap())

CXKTransData::CXKTransData()
: m_nMsgID(0)
{
}

CXKTransData::~CXKTransData()
{
}

Tbool CXKTransData::SetMsgID(Tuint nMsgID)
{
	Tbool bRet	= XK_FALSE;
	XK_TRANSMSG_MAP::iterator it	= theTransMap.find(nMsgID);
	XK_ASSERT( it != theTransMap.end() );
	if( it != theTransMap.end() )
	{
		CXKTransKey &key	= (*it).second;
		Tuint nCnt	= key.GetCount();
		CXKTransKeyInfo	*ptr	= key.GetKeys();
		XK_ASSERT( XK_NULL != ptr );
		Tuint nSize	= 0;
		for(Tuint index = 0;index < nCnt; index++)
		{
			nSize	+= ptr[index].m_nLen;
		}

		XK_ASSERT( nSize > 0 );
		bRet		= m_data.Create(nSize);
		if( XK_TRUE == bRet )
		{
			bRet	= m_data.SetKey(XK_KEY_MSGHEADER_ID, nMsgID);
		}
		m_nMsgID	= nMsgID;
	}

	return bRet;
}

Tbool CXKTransData::CopyData(const Tbyte *ptr, Tuint nLen)
{
	XK_ASSERT( XK_NULL != ptr );
	XK_ASSERT( nLen>sizeof(Tuint) );
	m_data.CopyData(ptr, nLen);

	return m_data.GetKey(0, m_nMsgID);
}

Tbool CXKTransData::GetKey(Tuint nKeyID, Tbyte &data)
{
	Tbool bRet	= XK_FALSE;
	XK_TRANSMSG_MAP::iterator it	= theTransMap.find(m_nMsgID);
	XK_ASSERT( it != theTransMap.end() );
	if( it != theTransMap.end() )
	{
		CXKTransKey &key		= (*it).second;
		CXKTransKeyInfo	*ptr	= key.GetKeys();
		XK_ASSERT( XK_NULL != ptr );
		for(Tuint nIndex= 0;nIndex< key.GetCount() ; nIndex++ )
		{
			if( ptr[nIndex].m_nKeyID == nKeyID )
			{
				XK_ASSERT( ptr[nIndex].m_nType	== TDT_BYTE );
				if( ptr[nIndex].m_nType	== TDT_BYTE )
				{
					bRet	= m_data.GetKey(ptr[nIndex].m_nPos, data);
				}
				break;
			}
		}
	}

	return bRet;
}

Tbool CXKTransData::GetKey(Tuint nKeyID, Tint &data)
{
	Tbool bRet	= XK_FALSE;
	XK_TRANSMSG_MAP::iterator it	= theTransMap.find(m_nMsgID);
	XK_ASSERT( it != theTransMap.end() );
	if( it != theTransMap.end() )
	{
		CXKTransKey &key		= (*it).second;
		CXKTransKeyInfo	*ptr	= key.GetKeys();
		XK_ASSERT( XK_NULL != ptr );
		for(Tuint nIndex= 0;nIndex< key.GetCount() ; nIndex++ )
		{
			if( ptr[nIndex].m_nKeyID == nKeyID )
			{
				XK_ASSERT( ptr[nIndex].m_nType	== TDT_INT );
				if( ptr[nIndex].m_nType	== TDT_INT )
				{
					bRet	= m_data.GetKey(ptr[nIndex].m_nPos, data);
				}
				break;
			}
		}
	}

	return bRet;
}

Tbool CXKTransData::GetKey(Tuint nKeyID, Tuint &data)
{
	Tbool bRet	= XK_FALSE;
	XK_TRANSMSG_MAP::iterator it	= theTransMap.find(m_nMsgID);
	XK_ASSERT( it != theTransMap.end() );
	if( it != theTransMap.end() )
	{
		CXKTransKey &key		= (*it).second;
		CXKTransKeyInfo	*ptr	= key.GetKeys();
		XK_ASSERT( XK_NULL != ptr );
		for(Tuint nIndex= 0;nIndex< key.GetCount() ; nIndex++ )
		{
			if( ptr[nIndex].m_nKeyID == nKeyID )
			{
				XK_ASSERT( ptr[nIndex].m_nType	== TDT_UINT );
				if( ptr[nIndex].m_nType	== TDT_UINT )
				{
					bRet	= m_data.GetKey(ptr[nIndex].m_nPos, data);
				}
				break;
			}
		}
	}

	return bRet;
}

Tbool CXKTransData::GetKey(Tuint nKeyID, Tfloat &data)
{
	Tbool bRet	= XK_FALSE;
	XK_TRANSMSG_MAP::iterator it	= theTransMap.find(m_nMsgID);
	XK_ASSERT( it != theTransMap.end() );
	if( it != theTransMap.end() )
	{
		CXKTransKey &key		= (*it).second;
		CXKTransKeyInfo	*ptr	= key.GetKeys();
		XK_ASSERT( XK_NULL != ptr );
		for(Tuint nIndex= 0;nIndex< key.GetCount() ; nIndex++ )
		{
			if( ptr[nIndex].m_nKeyID == nKeyID )
			{
				XK_ASSERT( ptr[nIndex].m_nType	== TDT_FLOAT );
				if( ptr[nIndex].m_nType	== TDT_FLOAT )
				{
					bRet	= m_data.GetKey(ptr[nIndex].m_nPos, data);
				}
				break;
			}
		}
	}

	return bRet;
}

Tbool CXKTransData::GetKey(Tuint nKeyID, Tdouble &data)
{
	Tbool bRet	= XK_FALSE;
	XK_TRANSMSG_MAP::iterator it	= theTransMap.find(m_nMsgID);
	XK_ASSERT( it != theTransMap.end() );
	if( it != theTransMap.end() )
	{
		CXKTransKey &key		= (*it).second;
		CXKTransKeyInfo	*ptr	= key.GetKeys();
		XK_ASSERT( XK_NULL != ptr );
		for(Tuint nIndex= 0;nIndex< key.GetCount() ; nIndex++ )
		{
			if( ptr[nIndex].m_nKeyID == nKeyID )
			{
				XK_ASSERT( ptr[nIndex].m_nType	== TDT_DOUBLE );
				if( ptr[nIndex].m_nType	== TDT_DOUBLE )
				{
					bRet	= m_data.GetKey(ptr[nIndex].m_nPos, data);
				}
				break;
			}
		}
	}

	return bRet;
}

Tbool CXKTransData::GetKey(Tuint nKeyID, Tshort &data)
{
	Tbool bRet	= XK_FALSE;
	XK_TRANSMSG_MAP::iterator it	= theTransMap.find(m_nMsgID);
	XK_ASSERT( it != theTransMap.end() );
	if( it != theTransMap.end() )
	{
		CXKTransKey &key		= (*it).second;
		CXKTransKeyInfo	*ptr	= key.GetKeys();
		XK_ASSERT( XK_NULL != ptr );
		for(Tuint nIndex= 0;nIndex< key.GetCount() ; nIndex++ )
		{
			if( ptr[nIndex].m_nKeyID == nKeyID )
			{
				XK_ASSERT( ptr[nIndex].m_nType	== TDT_SHORT );
				if( ptr[nIndex].m_nType	== TDT_SHORT )
				{
					bRet	= m_data.GetKey(ptr[nIndex].m_nPos, data);
				}
				break;
			}
		}
	}

	return bRet;
}

Tbool CXKTransData::GetKey(Tuint nKeyID, Tbool &data)
{
	Tbool bRet	= XK_FALSE;
	XK_TRANSMSG_MAP::iterator it	= theTransMap.find(m_nMsgID);
	XK_ASSERT( it != theTransMap.end() );
	if( it != theTransMap.end() )
	{
		CXKTransKey &key		= (*it).second;
		CXKTransKeyInfo	*ptr	= key.GetKeys();
		XK_ASSERT( XK_NULL != ptr );
		for(Tuint nIndex= 0;nIndex< key.GetCount() ; nIndex++ )
		{
			if( ptr[nIndex].m_nKeyID == nKeyID )
			{
				XK_ASSERT( ptr[nIndex].m_nType	== TDT_BOOL );
				if( ptr[nIndex].m_nType	== TDT_BOOL )
				{
					bRet	= m_data.GetKey(ptr[nIndex].m_nPos, data);
				}
				break;
			}
		}
	}

	return bRet;
}

Tbool CXKTransData::GetKey(Tuint nKeyID, string &data)
{
	Tbool bRet	= XK_FALSE;
	XK_TRANSMSG_MAP::iterator it	= theTransMap.find(m_nMsgID);
	XK_ASSERT( it != theTransMap.end() );
	if( it != theTransMap.end() )
	{
		CXKTransKey &key		= (*it).second;
		CXKTransKeyInfo	*ptr	= key.GetKeys();
		XK_ASSERT( XK_NULL != ptr );
		for(Tuint nIndex= 0;nIndex< key.GetCount() ; nIndex++ )
		{
			if( ptr[nIndex].m_nKeyID == nKeyID )
			{
				XK_ASSERT( ptr[nIndex].m_nType	== TDT_STRING );
				if( ptr[nIndex].m_nType	== TDT_STRING )
				{
					bRet	= m_data.GetKey(ptr[nIndex].m_nPos, data);
				}
				break;
			}
		}
	}

	return bRet;
}

Tbool CXKTransData::GetKey(Tuint nKeyID, Tbyte *data, Tuint nLen)
{
	Tbool bRet	= XK_FALSE;
	XK_TRANSMSG_MAP::iterator it	= theTransMap.find(m_nMsgID);
	XK_ASSERT( it != theTransMap.end() );
	if( it != theTransMap.end() )
	{
		CXKTransKey &key		= (*it).second;
		CXKTransKeyInfo	*ptr	= key.GetKeys();
		XK_ASSERT( XK_NULL != ptr );
		for(Tuint nIndex= 0;nIndex< key.GetCount() ; nIndex++ )
		{
			if( ptr[nIndex].m_nKeyID == nKeyID )
			{
				XK_ASSERT( ptr[nIndex].m_nType	== TDT_BYTEARRAY );
				if( ptr[nIndex].m_nType	== TDT_BYTEARRAY )
				{
					bRet	= m_data.GetKey(ptr[nIndex].m_nPos, data, nLen);
				}
				break;
			}
		}
	}

	return bRet;
}

Tbool CXKTransData::SetKey(Tuint nKeyID, Tbyte data)
{
	Tbool bRet	= XK_FALSE;
	XK_TRANSMSG_MAP::iterator it	= theTransMap.find(m_nMsgID);
	XK_ASSERT( it != theTransMap.end() );
	if( it != theTransMap.end() )
	{
		CXKTransKey &key		= (*it).second;
		CXKTransKeyInfo	*ptr	= key.GetKeys();
		XK_ASSERT( XK_NULL != ptr );
		for(Tuint nIndex= 0;nIndex< key.GetCount() ; nIndex++ )
		{
			if( ptr[nIndex].m_nKeyID == nKeyID )
			{
				XK_ASSERT( ptr[nIndex].m_nType	== TDT_BYTE );
				if( ptr[nIndex].m_nType	== TDT_BYTE )
				{
					bRet	= m_data.SetKey(ptr[nIndex].m_nPos, data);
				}
				break;
			}
		}
	}

	return bRet;
}

Tbool CXKTransData::SetKey(Tuint nKeyID, Tint data)
{
	Tbool bRet	= XK_FALSE;
	XK_TRANSMSG_MAP::iterator it	= theTransMap.find(m_nMsgID);
	XK_ASSERT( it != theTransMap.end() );
	if( it != theTransMap.end() )
	{
		CXKTransKey &key		= (*it).second;
		CXKTransKeyInfo	*ptr	= key.GetKeys();
		XK_ASSERT( XK_NULL != ptr );
		for(Tuint nIndex= 0;nIndex< key.GetCount() ; nIndex++ )
		{
			if( ptr[nIndex].m_nKeyID == nKeyID )
			{
				XK_ASSERT( ptr[nIndex].m_nType	== TDT_INT );
				if( ptr[nIndex].m_nType	== TDT_INT )
				{
					bRet	= m_data.SetKey(ptr[nIndex].m_nPos, data);
				}
				break;
			}
		}
	}

	return bRet;
}

Tbool CXKTransData::SetKey(Tuint nKeyID, Tuint data)
{
	Tbool bRet	= XK_FALSE;
	XK_TRANSMSG_MAP::iterator it	= theTransMap.find(m_nMsgID);
	XK_ASSERT( it != theTransMap.end() );
	if( it != theTransMap.end() )
	{
		CXKTransKey &key		= (*it).second;
		CXKTransKeyInfo	*ptr	= key.GetKeys();
		XK_ASSERT( XK_NULL != ptr );
		for(Tuint nIndex= 0;nIndex< key.GetCount() ; nIndex++ )
		{
			if( ptr[nIndex].m_nKeyID == nKeyID )
			{
				XK_ASSERT( ptr[nIndex].m_nType	== TDT_UINT );
				if( ptr[nIndex].m_nType	== TDT_UINT )
				{
					bRet	= m_data.SetKey(ptr[nIndex].m_nPos, data);
				}
				break;
			}
		}
	}

	return bRet;
}

Tbool CXKTransData::SetKey(Tuint nKeyID, Tfloat data)
{
	Tbool bRet	= XK_FALSE;
	XK_TRANSMSG_MAP::iterator it	= theTransMap.find(m_nMsgID);
	XK_ASSERT( it != theTransMap.end() );
	if( it != theTransMap.end() )
	{
		CXKTransKey &key		= (*it).second;
		CXKTransKeyInfo	*ptr	= key.GetKeys();
		XK_ASSERT( XK_NULL != ptr );
		for(Tuint nIndex= 0;nIndex< key.GetCount() ; nIndex++ )
		{
			if( ptr[nIndex].m_nKeyID == nKeyID )
			{
				XK_ASSERT( ptr[nIndex].m_nType	== TDT_FLOAT );
				if( ptr[nIndex].m_nType	== TDT_FLOAT )
				{
					bRet	= m_data.SetKey(ptr[nIndex].m_nPos, data);
				}
				break;
			}
		}
	}

	return bRet;
}

Tbool CXKTransData::SetKey(Tuint nKeyID, Tdouble data)
{
	Tbool bRet	= XK_FALSE;
	XK_TRANSMSG_MAP::iterator it	= theTransMap.find(m_nMsgID);
	XK_ASSERT( it != theTransMap.end() );
	if( it != theTransMap.end() )
	{
		CXKTransKey &key		= (*it).second;
		CXKTransKeyInfo	*ptr	= key.GetKeys();
		XK_ASSERT( XK_NULL != ptr );
		for(Tuint nIndex= 0;nIndex< key.GetCount() ; nIndex++ )
		{
			if( ptr[nIndex].m_nKeyID == nKeyID )
			{
				XK_ASSERT( ptr[nIndex].m_nType	== TDT_DOUBLE );
				if( ptr[nIndex].m_nType	== TDT_DOUBLE )
				{
					bRet	= m_data.SetKey(ptr[nIndex].m_nPos, data);
				}
				break;
			}
		}
	}

	return bRet;
}

Tbool CXKTransData::SetKey(Tuint nKeyID, Tshort data)
{
	Tbool bRet	= XK_FALSE;
	XK_TRANSMSG_MAP::iterator it	= theTransMap.find(m_nMsgID);
	XK_ASSERT( it != theTransMap.end() );
	if( it != theTransMap.end() )
	{
		CXKTransKey &key		= (*it).second;
		CXKTransKeyInfo	*ptr	= key.GetKeys();
		XK_ASSERT( XK_NULL != ptr );
		for(Tuint nIndex= 0;nIndex< key.GetCount() ; nIndex++ )
		{
			if( ptr[nIndex].m_nKeyID == nKeyID )
			{
				XK_ASSERT( ptr[nIndex].m_nType	== TDT_SHORT );
				if( ptr[nIndex].m_nType	== TDT_SHORT )
				{
					bRet	= m_data.SetKey(ptr[nIndex].m_nPos, data);
				}
				break;
			}
		}
	}

	return bRet;
}

Tbool CXKTransData::SetKey(Tuint nKeyID, Tbool data)
{
	Tbool bRet	= XK_FALSE;
	XK_TRANSMSG_MAP::iterator it	= theTransMap.find(m_nMsgID);
	XK_ASSERT( it != theTransMap.end() );
	if( it != theTransMap.end() )
	{
		CXKTransKey &key		= (*it).second;
		CXKTransKeyInfo	*ptr	= key.GetKeys();
		XK_ASSERT( XK_NULL != ptr );
		for(Tuint nIndex= 0;nIndex< key.GetCount() ; nIndex++ )
		{
			if( ptr[nIndex].m_nKeyID == nKeyID )
			{
				XK_ASSERT( ptr[nIndex].m_nType	== TDT_BOOL );
				if( ptr[nIndex].m_nType	== TDT_BOOL )
				{
					bRet	= m_data.SetKey(ptr[nIndex].m_nPos, data);
				}
				break;
			}
		}
	}

	return bRet;
}

Tbool CXKTransData::SetKey(Tuint nKeyID, const string &data)
{
	Tbool bRet	= XK_FALSE;
	XK_TRANSMSG_MAP::iterator it	= theTransMap.find(m_nMsgID);
	XK_ASSERT( it != theTransMap.end() );
	if( it != theTransMap.end() )
	{
		CXKTransKey &key		= (*it).second;
		CXKTransKeyInfo	*ptr	= key.GetKeys();
		XK_ASSERT( XK_NULL != ptr );
		for(Tuint nIndex= 0;nIndex< key.GetCount() ; nIndex++ )
		{
			if( ptr[nIndex].m_nKeyID == nKeyID )
			{
				XK_ASSERT( ptr[nIndex].m_nType	== TDT_STRING );
				if( ptr[nIndex].m_nType	== TDT_STRING )
				{
					bRet	= m_data.SetKey(ptr[nIndex].m_nPos, data);
				}
				break;
			}
		}
	}

	return bRet;
}

Tbool CXKTransData::SetKey(Tuint nKeyID, const Tbyte *data, Tuint nLen)
{
	Tbool bRet	= XK_FALSE;
	XK_TRANSMSG_MAP::iterator it	= theTransMap.find(m_nMsgID);
	XK_ASSERT( it != theTransMap.end() );
	if( it != theTransMap.end() )
	{
		CXKTransKey &key		= (*it).second;
		CXKTransKeyInfo	*ptr	= key.GetKeys();
		XK_ASSERT( XK_NULL != ptr );
		for(Tuint nIndex= 0;nIndex< key.GetCount() ; nIndex++ )
		{
			if( ptr[nIndex].m_nKeyID == nKeyID )
			{
				XK_ASSERT( ptr[nIndex].m_nType	== TDT_BYTEARRAY );
				if( ptr[nIndex].m_nType	== TDT_BYTEARRAY )
				{
					bRet	= m_data.SetKey(ptr[nIndex].m_nPos, data, nLen);
				}
				break;
			}
		}
	}

	return bRet;
}

XK_TRANSMSG_MAP& CXKTransData::GetTransKeyMap()
{
	static XK_TRANSMSG_MAP theMap;
	return theMap;
}

Tbool CXKTransData::InstallMsg(Tuint nMsgID, Tbyte nCnt)
{
	if(theTransMap.find(nMsgID) != theTransMap.end())
		return XK_FALSE;

	theTransMap.insert(XK_TRANSMSG_MAP::value_type(nMsgID, CXKTransKey()));
	XK_TRANSMSG_MAP::iterator it	= theTransMap.find(nMsgID);
	XK_ASSERT( it != theTransMap.end() );

	CXKTransKey &key	= (*it).second;
	if( !key.Create(nCnt) )
	{
		theTransMap.erase(it);
		return XK_FALSE;
	}

	return XK_TRUE;
}

Tint CXKTransData::AddKey(Tuint nMsgID, Tuint nKeyID, Tbyte nType, Tuint nLen /*= 0*/)
{
	XK_TRANSMSG_MAP::iterator it	= theTransMap.find(nMsgID);
	if( it == theTransMap.end() )	return -1;

	CXKTransKey &key	= (*it).second;
	
	CXKTransKeyInfo	*ptr	= key.GetKeys();
	Tuint nIndex	= key.GetIndex();
	XK_ASSERT( nIndex< key.GetCount() );
	ptr[nIndex].m_nKeyID	= nKeyID;
	ptr[nIndex].m_nType		= nType;
	Tuint nL	= 0;
	switch(nType)
	{
	case TDT_BYTE:
		nL	= sizeof(Tbyte);
		break;
	case TDT_INT: 
		nL	= sizeof(Tint);
		break;
	case TDT_UINT: 
		nL	= sizeof(Tuint);
		break;
	case TDT_FLOAT: 
		nL	= sizeof(Tfloat);
		break;
	case TDT_DOUBLE: 
		nL	= sizeof(Tdouble);
		break;
	case TDT_SHORT: 
		nL	= sizeof(Tshort);
		break;
	case TDT_BOOL: 
		nL	= sizeof(Tbool);
		break;
	case TDT_STRING: 
	case TDT_BYTEARRAY:
		nL	= nLen;
		break;
	default:
		break;
	}

	ptr[nIndex].m_nLen		= nL;
	if( (nIndex+1) == key.GetCount() )
	{
		Tuint nPos	= 0;
		// 为各子项计算缓冲区位置
		for(Tuint i=0;i< key.GetCount(); i++)
		{
			XK_ASSERT( ptr[i].m_nLen != 0 );
			ptr[i].m_nPos	= nPos;
			nPos	+= ptr[i].m_nLen;
		}
	}
	// 移动字段索引
	key.IncIndex();

	return 0;
}
//////////////////////////////////////////////////////////////////////
// CXKMsgQueue
//////////////////////////////////////////////////////////////////////
CXKMsgQueue::CXKMsgQueue()
{
}
CXKMsgQueue::~CXKMsgQueue()
{
	Destroy();
}

void CXKMsgQueue::Destroy()
{
	m_mutex.Lock();
	while( !m_queue.empty() )
	{
		delete m_queue.front();
		m_queue.pop();
	}
	m_mutex.Unlock();
}

void CXKMsgQueue::Push(CXKTransData	*pTransData)
{
	if( XK_NULL == pTransData )	return;

	m_mutex.Lock();
	m_queue.push( pTransData );
	m_mutex.Unlock();
}

CXKTransData *CXKMsgQueue::Pop( )
{
	CXKTransData	*pTransData	= XK_NULL;
	m_mutex.Lock();
	pTransData	= m_queue.front();
	m_queue.pop();
	m_mutex.Unlock();

	return pTransData;
}

Tbool CXKMsgQueue::IsEmpty()
{
	Tbool bRet	= XK_FALSE;
	m_mutex.Lock();
	bRet	= m_queue.empty()?XK_TRUE:XK_FALSE;
	m_mutex.Unlock();

	return bRet;
}
