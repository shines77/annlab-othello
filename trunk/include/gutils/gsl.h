// Guo Standard Library standard header
#pragma once
#ifndef _GSL_H_
#define _GSL_H_

#ifndef RC_INVOKED
#include "gyvals.h"

#ifdef _MSC_VER
#pragma pack(push,_CRT_PACKING)
#endif  /* _MSC_VER */

_GSL_BEGIN

class CxGSL
{
public:
	CxGSL(void) { };
	virtual ~CxGSL(void) { };
};

_GSL_END

#ifdef  _MSC_VER
#pragma pack(pop)
#endif  /* _MSC_VER */

#endif /* RC_INVOKED */

#endif  /* _GSL_H_ */
