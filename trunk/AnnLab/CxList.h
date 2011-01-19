// CxList standard header
#pragma once
#ifndef _CXLIST_H_
#define _CXLIST_H_

#ifndef RC_INVOKED
//#include <xutility>
#include "CxBaseObject.h"
#include <list>

using namespace std;

#ifdef _MSC_VER
#pragma pack(push,_CRT_PACKING)
#endif  /* _MSC_VER */

_STD_BEGIN

// TEMPLATE CLASS CxList
template<class _Ty,
class _Ax = allocator<_Ty> >
class CxList
	: public list<_Ty, _Ax>, public CxBaseObject
{
	// bidirectional linked list
public:
	typedef CxList<_Ty, _Ax> _Myt;
	typedef list<_Ty, _Ax> _Mybase;
	typedef typename _Mybase::_Alty _Alloc;
};

class CxListEx
{
public:
	CxListEx(void);
	virtual ~CxListEx(void);
};

_STD_END

#ifdef  _MSC_VER
#pragma pack(pop)
#endif  /* _MSC_VER */

#endif /* RC_INVOKED */

#endif  /* _CXLIST_H_ */
