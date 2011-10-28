
#ifndef _NNUTILS_H_
#define _NNUTILS_H_

#include "../matlab.h"
#include "../../../annlab/AnnNetwork.h"
#include "../../../annlab/AnnMatrix.h"

namespace matlab {

int trimString( const TCHAR *szString, TCHAR *szBuffer, DWORD dwBufSize );

int calcperf2( CAnnNetwork *net, CAnnXArray *X, double *Pd, double *Tl, double *Ai, int Q, int TS,
			  double *perf, double *El, double *Ac, double *N, double *LWZ, double *IWZ, double *BZ );

}  // namespace matlab

#endif  /* _NNUTILS_H_ */
