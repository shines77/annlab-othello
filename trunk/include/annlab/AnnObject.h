#pragma once

#include "../../include/annlab/ann_stddef.h"

#define OBJ_NAME_LEN      64

class CAnnObject
{
public:
	CAnnObject( void )                { set_name(NULL);   }
	CAnnObject( const TCHAR *szName ) { set_name(szName); }
	virtual ~CAnnObject( void )       {                   }

protected:
	TCHAR m_szName[OBJ_NAME_LEN];

public:
	// gets
	INLINE TCHAR * name( void ) const {
		return (TCHAR *)m_szName;
	};

	// sets
	void set_name( const TCHAR *szName ) {
		if (szName != NULL)
			_tcscpy_s(m_szName, _countof(m_szName), szName);
		else
			_tcscpy_s(m_szName, _countof(m_szName), _T(""));
	}
};
