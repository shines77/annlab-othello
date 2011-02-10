
#pragma once

#ifndef _ELMAT_H_
#define _ELMAT_H_

#include "..\nnet.h"
#include "..\..\CxMatrix.h"

_MATLAB_BEGIN

CxMatrix zeros  ( int _rows, int _cols );
CxMatrix ones   ( int _rows, int _cols );
CxMatrix eyes   ( int _rows, int _cols );
CxMatrix rands  ( int _rows, int _cols );
CxMatrix rands2 ( int _rows, int _cols );

CxMatrix transpose ( CxMatrix &m );
CxMatrix linspace  ( double d1, double d2, int n );
CxMatrix expand_row( CxMatrix &m, int _rows );
CxMatrix expand_col( CxMatrix &m, int _cols );

CxMatrix diag( CxMatrix &m );

CxMatrix abs  ( CxMatrix &m );
CxMatrix sqrt ( CxMatrix &m );
CxMatrix sum  ( CxMatrix &m );
CxMatrix sign ( CxMatrix &m );

CxMatrix sumsqrt ( CxMatrix &m );

CxMatrix norm  ( CxMatrix &m );
CxMatrix normr ( CxMatrix &m );

CxMatrix randnr( int _rows, int _cols );

double dotprod ( const CxVector &x, const CxVector y );

CxMatrix dotprod ( const CxMatrix &x, const CxMatrix &y );
CxMatrix dotdiv  ( const CxMatrix &x, const CxMatrix &y );

_MATLAB_END

#endif  /* _ELMAT_H_ */
