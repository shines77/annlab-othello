
#include "../../../../include/matlab/toolbox/matlab/elmat.h"
#include <math.h>

namespace matlab {

// 重置大小并初始化为全0矩阵
CAnnMatrix zeros( int _rows, int _cols )
{
	CAnnMatrix _result(_rows, _cols, MAT_INIT_ZEROS);
	return _result;
}

// 重置大小并初始化为全1矩阵
CAnnMatrix ones( int _rows, int _cols )
{
	CAnnMatrix _result(_rows, _cols, MAT_INIT_ONES);
	return _result;
}

CAnnMatrix eyes( int _rows, int _cols )
{
	CAnnMatrix _result(_rows, _cols, MAT_INIT_EYES);
	return _result;
}

// 重置大小并初始化为[-1,1]随机数矩阵
CAnnMatrix rands( int _rows, int _cols )
{
	CAnnMatrix _result(_rows, _cols, MAT_INIT_RANDS);
	return _result;
}

// 重置大小并初始化为[0,1]随机数矩阵
CAnnMatrix rands2( int _rows, int _cols )
{
	CAnnMatrix _result(_rows, _cols, MAT_INIT_RANDS2);
	return _result;
}

CAnnMatrix transpose( CAnnMatrix &m )
{
	CAnnMatrix _Result(m.cols, m.rows);
	for (int i=0; i<_Result.cols; i++) {
		for (int j=0; j<_Result.rows; j++) {
			_Result.setElement(j, i, m.getElement(i, j));
		}
	}
	return _Result;
}

CAnnMatrix expand_row( CAnnMatrix &m, int _rows )
{
	CAnnMatrix _Result(_rows, m.cols);
	for (int i=0; i<_Result.rows; i++) {
		for (int j=0; j<_Result.cols; j++) {
			_Result.setElement(i, j, m.getElement(0, j));
		}
	}
	return _Result;
}

CAnnMatrix expand_col( CAnnMatrix &m, int _cols )
{
	CAnnMatrix _Result(m.rows, _cols);
	for (int i=0; i<_Result.cols; i++) {
		for (int j=0; j<_Result.rows; j++) {
			_Result.setElement(j, i, m.getElement(j, 0));
		}
	}
	return _Result;
}

CAnnMatrix diag( CAnnMatrix &m )
{
	CAnnMatrix _Result;
	int _size = MIN(m.rows, m.cols);
	_size = MAX(_size, 0);
	__MY_ASSERT(_size > 0);

	if (_size > 0) {
		_Result.resize(_size, 1, MAT_INIT_ZEROS);
		for (int i=0; i<_size; i++) {
			_Result.setElement(i, 0, m.getElement(i, i));
		}
	}
	return _Result;
}

CAnnMatrix abs( CAnnMatrix &m )
{
	CAnnMatrix _Result(m);
	int _index = 0;
	for (int i=0; i<_Result.rows; i++) {
		for (int j=0; j<_Result.cols; j++) {
			_Result.setElement(_index, ::abs(m.getElement(_index)));
			_index++;
		}
	}
	return _Result;
}

CAnnMatrix sqrt( CAnnMatrix &m )
{
	CAnnMatrix _Result(m);
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
 ** 把每一列的元素相加后得到的至组成一个单行的矩阵(行向量);
 **
 ** 例如:
 **     >> m = [1,2,3;4,5,6;7,8,9]; sum(m)
 **        ans =
 **            12    15    18 
 ****************************************************************************/

CAnnMatrix sum( CAnnMatrix &m )
{
	int _cols = m.cols;
	CAnnMatrix _Result(1, _cols);
	for (int i=0; i<m.cols; i++) {
		double _sum = 0.0;
		for (int j=0; j<m.rows; j++)
			_sum += m.getElement(j, i);
		_Result.setElement(0, i, _sum);
	}
	return _Result;
}

CAnnMatrix sumsqrt( CAnnMatrix &m )
{
	int _cols = m.cols;
	CAnnMatrix _Result(1, _cols);
	for (int i=0; i<m.cols; i++) {
		double _sum = 0.0;
		for (int j=0; j<m.rows; j++)
			_sum += m.getElement(j, i);
		_Result.setElement(0, i, ::sqrt(_sum));
	}
	return _Result;
}

CAnnMatrix sign( CAnnMatrix &m )
{
	double _sign;
	CAnnMatrix _Result(m);
	for (int i=0; i<m.cols; i++) {
		for (int j=0; j<m.rows; j++) {
			double _val = m.getElement(j, i);
			if (_val == 0.0)
				_sign = 0.0;
			else
				_sign = _val / ::abs(_val);
			_Result.setElement(j, i, _sign);
		}
	}
	return _Result;
}

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

CAnnMatrix norm( CAnnMatrix &m )
{
	int mr, mc;
	CAnnMatrix n;
	mr = m.rows;
	mc = m.cols;
	/* n = sqrt(sum(diag(m'*m))); <== 'fro'参数 */
	n = sqrt(sum(diag(transpose(m) * m)));
	return n;
}

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

CAnnMatrix normr( CAnnMatrix &m )
{
	int mr, mc;
	CAnnMatrix n;
	mr = m.rows;
	mc = m.cols;
	if (mc == 1) {
		/* n = m ./ abs(m); */
		n = dotdiv(m, abs(m));
	}
	else {
		/* n = sqrt(ones./(sum((m.*m)')))'*ones(1,mc).*m; */
		n = dotprod(transpose(sqrt(ones(1, mr) / sum(transpose(dotprod(m, m))))) * ones(1, mc), m);
	}
	return n;
}

/*****************************************************************

	function y = linspace(d1, d2, n)
	%LINSPACE Linearly spaced vector.
	%   LINSPACE(X1, X2) generates a row vector of 100 linearly
	%   equally spaced points between X1 and X2.
	%
	%   LINSPACE(X1, X2, N) generates N points between X1 and X2.
	%   For N < 2, LINSPACE returns X2.
	%
	%   Class support for inputs X1,X2:
	%      float: double, single
	%
	%   See also LOGSPACE, :.

	%   Copyright 1984-2004 The MathWorks, Inc. 
	%   $Revision: 5.12.4.1 $  $Date: 2004/07/05 17:01:20 $

	if nargin == 2
		n = 100;
	end

	n = double(n);
	y = [d1+(0:n-2)*(d2-d1)/(floor(n)-1) d2];

*******************************************************************/

CAnnMatrix linspace(double d1, double d2, int n)
{
	CAnnMatrix _Result;
	if (n > 1) {
		_Result.resize(1, n);
		for (int i=0; i<n; i++) {
			_Result.setElement(0, i, d1 + i * (d2 - d1)/( n - 1));
		}
	}
	else if (n == 1) {
		_Result.resize(1, 1);
		_Result.setElement(0, 0, d2);
	}
	return _Result;
}

CAnnMatrix randnr( int _rows, int _cols )
{
	CAnnMatrix _Result;
	_Result = normr(rands(_rows, _cols));
	return _Result;
}

double dotprod( const CAnnVector &x, const CAnnVector &y )
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

CAnnMatrix dotprod( const CAnnMatrix &x, const CAnnMatrix &y )
{
	CAnnMatrix _Result;

	// Check for compatible dimensions:
	__MY_ASSERT(x.size() == y.size());
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

CAnnMatrix dotdiv( const CAnnMatrix &x, const CAnnMatrix &y )
{
	CAnnMatrix _Result;

	// Check for compatible dimensions:
	__MY_ASSERT(x.size() == y.size());
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

}  // namespace matlab
