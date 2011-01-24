
#ifndef _ANN_GLOBAL_H_
#define _ANN_GLOBAL_H_

#pragma once

#define INLINE inline

#define FCN_NAME_LEN      64

#define MIN(a, b)         (((a) < (b)) ? (a) : (b))
#define MAX(a, b)         (((a) > (b)) ? (a) : (b))

INLINE void setFcnName( TCHAR *szDstFuncName, int nNameSize, const TCHAR *szFcnName );

#endif  // _ANN_GLOBAL_H_
