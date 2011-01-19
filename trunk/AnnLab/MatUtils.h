#pragma once

#ifndef _MATRIXUTILS_H_
#define _MATRIXUTILS_H_

#include "CxMatrix.h"

namespace MatUtils {

	CxMatrix ones    ( int _rows, int _cols );
	CxMatrix zeros   ( int _rows, int _cols );
	CxMatrix rands   ( int _rows, int _cols );
	CxMatrix rands2  ( int _rows, int _cols );

};  /* namespace MatUtils */

#endif  /* _MATRIXUTILS_H_ */
