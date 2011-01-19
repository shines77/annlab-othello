#include "StdAfx.h"
#include "MatUtils.h"

namespace MatUtils {

	// ���ô�С����ʼ��Ϊȫ0����
	CxMatrix zeros( int _rows, int _cols )
	{
		CxMatrix _result(_rows, _cols, MAT_INIT_ZEROS);
		return _result;
	}

	// ���ô�С����ʼ��Ϊȫ1����
	CxMatrix ones( int _rows, int _cols )
	{
		CxMatrix _result(_rows, _cols, MAT_INIT_ONES);
		return _result;
	}

	// ���ô�С����ʼ��Ϊ[-1,1]���������
	CxMatrix rands( int _rows, int _cols )
	{
		CxMatrix _result(_rows, _cols, MAT_INIT_RANDS);
		return _result;
	}

	// ���ô�С����ʼ��Ϊ[0,1]���������
	CxMatrix rands2( int _rows, int _cols )
	{
		CxMatrix _result(_rows, _cols, MAT_INIT_RANDS2);
		return _result;
	}

};  /* namespace MatUtils */
