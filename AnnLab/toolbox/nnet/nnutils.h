#pragma once

#ifndef _NNUTILS_H_
#define _NNUTILS_H_

#include "..\nnet.h"
#include "..\..\AnnNetwork.h"
#include "..\..\CxMatrix.h"

_MATLAB_BEGIN

int trimString( const TCHAR *szString, TCHAR *szBuffer, DWORD dwBufSize );

int calcperf2( CAnnNetwork *net, CAnnXArray *X, double *Pd, double *Tl, double *Ai, int Q, int TS,
			  double *perf, double *El, double *Ac, double *N, double *LWZ, double *IWZ, double *BZ );

_MATLAB_END

#endif  /* _NNUTILS_H_ */
