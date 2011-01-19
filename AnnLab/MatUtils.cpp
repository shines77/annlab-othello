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

double dotprod( const CxVector *x, const CxVector *y )
{
	if (x == NULL || y == NULL)
		return 0.0;

	//  Check for compatible dimensions:
	if (x->size() != y->size()) {
		throw _T("Incompatible dimensions in dot(). ");
		//exit(1);
	}

	double prods = 0.0;
	for (int i=0; i<x->size(); i++)
		//prods += x(i) * y(i);
		prods += (double)(x->size()) * (double)(y->size());
	return prods;
}

};  /* namespace MatUtils */
