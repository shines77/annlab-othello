#include "StdAfx.h"
#include "MatUtils.h"

namespace MatUtils {

	// 重置大小并初始化为全0矩阵
	CxMatrix zeros( int _rows, int _cols )
	{
		CxMatrix _result(_rows, _cols, MAT_INIT_ZEROS);
		return _result;
	}

	// 重置大小并初始化为全1矩阵
	CxMatrix ones( int _rows, int _cols )
	{
		CxMatrix _result(_rows, _cols, MAT_INIT_ONES);
		return _result;
	}

	// 重置大小并初始化为[-1,1]随机数矩阵
	CxMatrix rands( int _rows, int _cols )
	{
		CxMatrix _result(_rows, _cols, MAT_INIT_RANDS);
		return _result;
	}

	// 重置大小并初始化为[0,1]随机数矩阵
	CxMatrix rands2( int _rows, int _cols )
	{
		CxMatrix _result(_rows, _cols, MAT_INIT_RANDS2);
		return _result;
	}

};  /* namespace MatUtils */
