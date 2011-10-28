
#ifndef _ELMAT_H_
#define _ELMAT_H_

#include "../matlab.h"
#include "../../../annlab/AnnMatrix.h"

using namespace annlab;

namespace matlab {

CAnnMatrix zeros  ( int _rows, int _cols );
CAnnMatrix ones   ( int _rows, int _cols );
CAnnMatrix eyes   ( int _rows, int _cols );
CAnnMatrix rands  ( int _rows, int _cols );
CAnnMatrix rands2 ( int _rows, int _cols );

CAnnMatrix transpose  ( CAnnMatrix &m );
CAnnMatrix linspace   ( double d1, double d2, int n );
CAnnMatrix expand_row ( CAnnMatrix &m, int _rows );
CAnnMatrix expand_col ( CAnnMatrix &m, int _cols );

CAnnMatrix diag ( CAnnMatrix &m );

CAnnMatrix abs  ( CAnnMatrix &m );
CAnnMatrix sqrt ( CAnnMatrix &m );
CAnnMatrix sum  ( CAnnMatrix &m );
CAnnMatrix sign ( CAnnMatrix &m );

CAnnMatrix sumsqrt ( CAnnMatrix &m );

CAnnMatrix norm  ( CAnnMatrix &m );
CAnnMatrix normr ( CAnnMatrix &m );

CAnnMatrix randnr( int _rows, int _cols );

double dotprod ( const CAnnVector &x, const CAnnVector y );

CAnnMatrix dotprod ( const CAnnMatrix &x, const CAnnMatrix &y );
CAnnMatrix dotdiv  ( const CAnnMatrix &x, const CAnnMatrix &y );

}  // namespace matlab

#endif  /* _ELMAT_H_ */
