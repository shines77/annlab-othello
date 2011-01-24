#include "StdAfx.h"
#include <math.h>
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

CxMatrix transpose( CxMatrix &m )
{
	CxMatrix _Result(m.cols, m.rows);
	int _index = 0;
	for (int i=0; i<_Result.cols; i++) {
		for (int j=0; j<_Result.rows; j++) {
			_Result.setElement(j, i, m.getElement(i, j));
		}
	}
	return _Result;
}

CxMatrix diag( CxMatrix &m )
{
	CxMatrix _Result;
	int _size = MIN(m.rows, m.cols);
	_size = MAX(_size, 0);
	ASSERT(_size > 0);

	if (_size > 0) {
		_Result.resize(_size, 1, MAT_INIT_ZEROS);
		for (int i=0; i<_size; i++) {
			_Result.setElement(i, 0, m.getElement(i, i));
		}
	}
	return _Result;
}

CxMatrix abs( CxMatrix &m )
{
	CxMatrix _Result(m);
	int _index = 0;
	for (int i=0; i<_Result.rows; i++) {
		for (int j=0; j<_Result.cols; j++) {
			_Result.setElement(_index, ::abs(m.getElement(_index)));
			_index++;
		}
	}
	return _Result;
}

CxMatrix sqrt( CxMatrix &m )
{
	CxMatrix _Result(m);
	int _index = 0;
	for (int i=0; i<_Result.rows; i++) {
		for (int j=0; j<_Result.cols; j++) {
			_Result.setElement(_index, ::sqrt(m.getElement(_index)));
			_index++;
		}
	}
	return _Result;
}

/****************************************************************************
 **
 ** ��ÿһ�е�Ԫ����Ӻ�õ��������һ�����еľ���(������);
 **
 ** ����:
 **     >> m = [1,2,3;4,5,6;7,8,9]; sum(m)
 **        ans =
 **            12    15    18 
 ****************************************************************************/

CxMatrix sum( CxMatrix &m )
{
	int _cols = m.cols;
	CxMatrix _Result(1, _cols);
	for (int i=0; i<m.cols; i++) {
		double _sum = 0.0;
		for (int j=0; j<m.rows; j++)
			_sum += m.getElement(j, i);
		_Result.setElement(0, i, _sum);
	}
	return _Result;
}

CxMatrix sumsqrt( CxMatrix &m )
{
	int _cols = m.cols;
	CxMatrix _Result(1, _cols);
	for (int i=0; i<m.cols; i++) {
		double _sum = 0.0;
		for (int j=0; j<m.rows; j++)
			_sum += m.getElement(j, i);
		_Result.setElement(0, i, ::sqrt(_sum));
	}
	return _Result;
}

CxMatrix norm( CxMatrix &m )
{
	/**********************************************************************
		%NORM Normalize of a matrix.
		%
		%  Syntax
		%
		%    norm(M)
		%
		%  Description
		%
		%    NORM(M) normalizes the matrix of M to a length of 1.
		%
		%  Examples
		%
		%    m = [1 2; 3 4]
		%    n = norm(m)
		%
		%  See also NORMC, NORMR.

		% Mark Beale, 1-31-92
		% Copyright 1992-2007 The MathWorks, Inc.
		% $Revision: 1.1.6.5 $  $Date: 2007/11/09 20:49:53 $

		if nargin < 1,error('NNET:Arguments','Not enough input arguments.'); end

		[mr,mc]=size(m);
		if (mc == 1)
			n = m ./ abs(m);
		else
			n = sqrt(sum(m.*m));
		end
	**********************************************************************/
	int mr, mc;
	CxMatrix n;
	mr = m.rows;
	mc = m.cols;
	/* n = sqrt(sum(diag(m'*m))); <== 'fro'���� */
	n = sqrt(sum(diag(transpose(m) * m)));
	return n;
}

CxMatrix normr( CxMatrix &m )
{
	/**********************************************************************
		%NORMR Normalize rows of a matrix.
		%
		%  Syntax
		%
		%    normr(M)
		%
		%  Description
		%
		%    NORMR(M) normalizes the columns of M to a length of 1.
		%
		%  Examples
		%
		%    m = [1 2; 3 4]
		%    n = normr(m)
		%
		%  See also NORMC.

		% Mark Beale, 1-31-92
		% Copyright 1992-2007 The MathWorks, Inc.
		% $Revision: 1.1.6.5 $  $Date: 2007/11/09 20:49:53 $

		if nargin < 1,error('NNET:Arguments','Not enough input arguments.'); end

		[mr,mc]=size(m);
		if (mc == 1)
			n = m ./ abs(m);
		else
			n = sqrt(ones./(sum((m.*m)')))'*ones(1,mc).*m;
		end
	**********************************************************************/
	int mr, mc;
	CxMatrix n;
	mr = m.rows;
	mc = m.cols;
	if (mc == 1) {
		/* n = m ./ abs(m); */
		n = dotdiv(m, abs(m));
	}
	else {
		/* n = sqrt(ones./(sum((m.*m)')))'*ones(1,mc).*m; */
		n = dotprod(transpose(sqrt(ones(1, mr) / sum(transpose(dotprod(m, m))))) * ones(1, mc), m);
		//n = transpose(sqrt(ones(1, mr) / sum(transpose(dotprod(m, m)))));
		//n = (transpose(dotprod(m, m)));
	}
	return n;
}

CxMatrix randnr( int _rows, int _cols )
{
	CxMatrix _Result;
	_Result = normr(rands(_rows, _cols));
	return _Result;
}

double dotprod( const CxVector &x, const CxVector &y )
{
	//  Check for compatible dimensions:
	if (x.size() != y.size()) {
		throw _T("Incompatible dimensions in dot(). ");
		//exit(1);
	}

	double prods = 0.0;
	for (int i=0; i<x.size(); i++)
		//prods += x(i) * y(i);
		prods += (double)(x.size()) * (double)(y.size());
	return prods;
}

CxMatrix dotprod( const CxMatrix &x, const CxMatrix &y )
{
	CxMatrix _Result;

	// Check for compatible dimensions:
	if (x.size() != y.size()) {
		throw _T("Incompatible dimensions in dot(). ");
		//exit(1);
		return _Result;
	}

	int xr, xc, yr, yc;
	xr = x.rows; xc = x.cols;
	yr = y.rows; yc = y.cols;

	int minx, maxx, miny, maxy;
	minx = MIN(xr, xc);
	maxx = MAX(xr, xc);
	miny = MIN(yr, yc);
	maxy = MIN(yr, yc);

	double prods = 0.0;
	if (minx == 1 && miny == 1 && maxx == maxy) {
		_Result.resize(1, maxx);
		for (int i=0; i<maxx; i++)
			_Result.setElement(i, x.getElement(i) * y.getElement(i));
	}
	else if (xr == yr && xc == yc) {
		_Result.resize(xr, xc);
		int _index = 0;
		for (int i=0; i<xr; i++) {
			for (int j=0; j<xc; j++) {
				_Result.setElement(_index, x.getElement(_index) * y.getElement(_index));
				_index++;
			}
		}
	}
	return _Result;
}

CxMatrix dotdiv( const CxMatrix &x, const CxMatrix &y )
{
	CxMatrix _Result;

	// Check for compatible dimensions:
	if (x.size() != y.size()) {
		throw _T("Incompatible dimensions in dot(). ");
		//exit(1);
		return _Result;
	}

	int xr, xc, yr, yc;
	xr = x.rows; xc = x.cols;
	yr = y.rows; yc = y.cols;

	int minx, maxx, miny, maxy;
	minx = MIN(xr, xc);
	maxx = MAX(xr, xc);
	miny = MIN(yr, yc);
	maxy = MIN(yr, yc);

	double prods = 0.0;
	if (minx == 1 && miny == 1 && maxx == maxy) {
		_Result.resize(1, maxx);
		for (int i=0; i<maxx; i++)
			_Result.setElement(i, x.getElement(i) / y.getElement(i));
	}
	else if (xr == yr && xc == yc) {
		_Result.resize(xr, xc);
		int _index = 0;
		for (int i=0; i<xr; i++) {
			for (int j=0; j<xc; j++) {
				_Result.setElement(_index, x.getElement(_index) / y.getElement(_index));
				_index++;
			}
		}
	}
	return _Result;
}

};  /* namespace MatUtils */