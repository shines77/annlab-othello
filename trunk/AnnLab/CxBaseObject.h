#pragma once

#include "AnnGlobal.h"

#define OBJ_NAME_LEN      64

class CxBaseObject
{
public:
	CxBaseObject( void )                { setName(NULL);   }
	CxBaseObject( const TCHAR *szName ) { setName(szName); }
	virtual ~CxBaseObject( void )       {                  }

public:
	// gets
	INLINE TCHAR * name( void ) const {
		return (TCHAR *)m_szName;
	};

	// sets
	void setName( const TCHAR *szName ) {
		if (szName != NULL)
			_tcscpy_s(m_szName, _countof(m_szName), szName);
		else
			_tcscpy_s(m_szName, _countof(m_szName), _T(""));
	}

protected:
	TCHAR m_szName[OBJ_NAME_LEN];

};
