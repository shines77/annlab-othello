// CAnnList standard header

#ifndef _ANNLIST_H_
#define _ANNLIST_H_

#ifndef RC_INVOKED
//#include <xutility>
#include "AnnObject.h"
#include <list>

using namespace std;

#ifdef _MSC_VER
#pragma pack(push,_CRT_PACKING)
#endif  /* _MSC_VER */

_STD_BEGIN

/*
// TEMPLATE CLASS CxList
template<class _Ty,
class _Ax = allocator<_Ty> >
class CAnnList
	: public list<_Ty, _Ax>, public CAnnObject
{
	// bidirectional linked list
public:
	typedef CAnnList<_Ty, _Ax> _Myt;
	typedef list<_Ty, _Ax> _Mybase;
	typedef typename _Mybase::_Alty _Alloc;
};

class CAnnListEx
{
public:
	CAnnListEx(void);
	virtual ~CAnnListEx(void);
};
//*/

_STD_END

#ifdef  _MSC_VER
#pragma pack(pop)
#endif  /* _MSC_VER */

#endif /* RC_INVOKED */

#endif  /* _ANNLIST_H_ */
