
#ifndef _ELMAT_H_
#define _ELMAT_H_

#include "../../annlab.h"
#include "../../../annlab/sys/AnnMatrix.h"

namespace annlab {

CAnnMatrix zeros  ( int _rows, int _cols );
CAnnMatrix ones   ( int _rows, int _cols );
CAnnMatrix eyes   ( int _rows, int _cols );
CAnnMatrix rands  ( int _rows, int _cols );
CAnnMatrix rands2 ( int _rows, int _cols );

CAnnMatrix  transpose ( const CAnnMatrix &mat );
//CAnnMatrix& transpose ( const CAnnMatrix &mat );
CAnnMatrix linspace   ( double d1, double d2, int n );
CAnnMatrix expand_row ( CAnnMatrix &mat, int _rows );
CAnnMatrix expand_col ( CAnnMatrix &mat, int _cols );

CAnnMatrix diag ( CAnnMatrix &mat );

CAnnMatrix abs  ( CAnnMatrix &mat );
CAnnMatrix sqrt ( CAnnMatrix &mat );
CAnnMatrix sum  ( CAnnMatrix &mat );
CAnnMatrix sign ( CAnnMatrix &mat );

CAnnMatrix sumsqrt ( CAnnMatrix &mat );

CAnnMatrix norm  ( CAnnMatrix &mat );
CAnnMatrix normr ( CAnnMatrix &mat );

CAnnMatrix randnr( int _rows, int _cols );

double dotprod ( const CAnnVector &x, const CAnnVector y );

CAnnMatrix dotprod ( const CAnnMatrix &x, const CAnnMatrix &y );
CAnnMatrix dotdiv  ( const CAnnMatrix &x, const CAnnMatrix &y );

}  // namespace annlab

#endif  /* _ELMAT_H_ */
