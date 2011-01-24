#pragma once

#ifndef _MATRIXUTILS_H_
#define _MATRIXUTILS_H_

#include "CxMatrix.h"

namespace MatUtils {

CxMatrix zeros  ( int _rows, int _cols );
CxMatrix ones   ( int _rows, int _cols );
CxMatrix rands  ( int _rows, int _cols );
CxMatrix rands2 ( int _rows, int _cols );

CxMatrix transpose( CxMatrix &m );

CxMatrix abs ( CxMatrix &m );
CxMatrix sqrt( CxMatrix &m );
CxMatrix sum ( CxMatrix &m );

CxMatrix normr ( CxMatrix &m );

CxMatrix randnr( int _rows, int _cols );

double dotprod ( const CxVector &x, const CxVector y );

CxMatrix dotprod ( const CxMatrix &x, const CxMatrix &y );
CxMatrix dotdiv  ( const CxMatrix &x, const CxMatrix &y );

};  /* namespace MatUtils */

#endif  /* _MATRIXUTILS_H_ */
