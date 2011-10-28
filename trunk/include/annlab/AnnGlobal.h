
#ifndef _ANN_GLOBAL_H_
#define _ANN_GLOBAL_H_

#include <stdlib.h>
#include <tchar.h>
#include <stddef.h>
#include "stdint.h"

#define INLINE inline

#define FCN_NAME_LEN      64

#define MIN(a, b)         (((a) < (b)) ? (a) : (b))
#define MAX(a, b)         (((a) > (b)) ? (a) : (b))

#define __MY_ASSERT(text, ...)

INLINE void setFcnName( TCHAR *szDstFuncName, int nNameSize, const TCHAR *szFcnName );

#endif  // _ANN_GLOBAL_H_
