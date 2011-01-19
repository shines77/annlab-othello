#include "StdAfx.h"
#include "CxMatrix.h"
#include <assert.h>
#include <math.h>

//////////////////////////////////////////////////////////////////
// CxVector
//////////////////////////////////////////////////////////////////

CxVector::CxVector( void )
{
	initVector(NULL, 0, TRUE);
}

CxVector::CxVector( int _size )
{
	initVector(NULL, _size, TRUE);
}

CxVector::CxVector( CxVector & srcVerctor )
{
	int _size = srcVerctor.size();

	initVector(srcVerctor.Name(), _size, TRUE);

	CxVector *pVector = copy( &srcVerctor );
	ASSERT(pVector != NULL);
}

CxVector::~CxVector( void )
{
	freeVector();
}

void CxVector::freeVector( void )
{
	if (m_pOrigData != NULL) {
		delete[] m_pOrigData;
		m_pOrigData = NULL;
		m_pData = NULL;
	}
	length    = 0;
	mem_alloc = 0;
}

BOOL CxVector::initVector( const TCHAR *szName, int _size,
						  int _initMode, /*= INIT_MODE_NONE */
						  int _initFcn   /*= MAT_INIT_NONE */ )
{
	BOOL bResult = FALSE;
	if (_initMode == INIT_MODE_FIRST) {
		length      = 0;
		mem_alloc   = 0;
		m_pOrigData = NULL;
		m_pData     = NULL;
	}

	if (_initMode != INIT_MODE_RESIZE) {
		if (szName != NULL)
			_tcscpy_s(m_szName, _countof(m_szName), szName);
		else
			_tcscpy_s(m_szName, _countof(m_szName), _T(""));
	}

	if (_size >= 0) {
		freeVector();

		length = _size;
		if (length > 0) {
			mem_alloc = length + MET_ADDR_ALIGN_SIZE;
			m_pOrigData = new double[mem_alloc];
			if (m_pOrigData != NULL) {
				m_pData = (double *)MET_ADDR_ALIGN(m_pOrigData);
				// init vector data
				bResult = initData(length, _initFcn);
			}
		}
		else {
			mem_alloc = 0;
			bResult = TRUE;
		}
	}
	return bResult;
}

BOOL CxVector::initData(int _size, int _initFcn /*= MAT_INIT_NONE */ )
{
	ASSERT(m_pData != NULL);
	if (m_pData == NULL)
		return FALSE;

	_size = (_size < length) ? _size : length;
	ASSERT(_size >= 0);

	// init vector data
	if (_size <= 0) {
		if (_size == 0)
			freeVector();
		return (_size >= 0);
	}

	double _dblRand;
	switch (_initFcn) {
	case MAT_INIT_ZEROS:		// all zeros
		for (int i=0; i<_size; i++)
			m_pData[i] = 0.0;
		break;
	case MAT_INIT_ONES:			// all ones
		for (int i=0; i<_size; i++)
			m_pData[i] = 1.0;
		break;
	case MAT_INIT_RANDS:		// all [-1,1] randomize
		for (int i=0; i<_size; i++) {
			_dblRand = 2.0 * (double)rand() / (double)(RAND_MAX + 1) - 1.0;
			m_pData[i] = _dblRand;
		}
		break;
	case MAT_INIT_RANDS2:		// all [0,1] randomize
		for (int i=0; i<_size; i++) {
			_dblRand = (double)rand() / (double)(RAND_MAX + 1);
			m_pData[i] = _dblRand;
		}
		break;
	case MAT_INIT_NONE:
		// do nothing
		break;
	default:
		// don't init matrix
		break;
	}
	return TRUE;
}

double CxVector::operator []( int _index ) const
{
	double _value = 0.0;

	return _value;
}

CxVector * CxVector::copy( const CxVector *srcVector )
{
	if (srcVector != NULL) {
		//
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////
// CxVectors
//////////////////////////////////////////////////////////////////

CxVectors::CxVectors( void )
{
	length = 0;
}

CxVectors::~CxVectors( void )
{
	freeVector();
}

void CxVectors::freeVector( void )
{
	length = 0;
}

//////////////////////////////////////////////////////////////////
// CxMatrix
//////////////////////////////////////////////////////////////////

CxMatrix::CxMatrix( void )
{
	initMatrix(NULL, 0, 0, TRUE);
}

CxMatrix::CxMatrix( int _size )
{
	initMatrix(NULL, _size, _size, TRUE);
}

CxMatrix::CxMatrix( int _rows, int _cols, int _initFcn /*= MAT_INIT_NONE */ )
{
	initMatrix(NULL, _rows, _cols, TRUE, _initFcn);
}

CxMatrix::CxMatrix( const TCHAR *szName, int _rows, int _cols,
				   int _initFcn /*= MAT_INIT_NONE */ )
{
	initMatrix(szName, _rows, _cols, TRUE, _initFcn);
}

CxMatrix::CxMatrix( CxMatrix & scrMatrix )
{
	int _rows, _cols;
	_rows = scrMatrix.rows;
	_cols = scrMatrix.cols;

	initMatrix(NULL, _rows, _cols, TRUE);

	CxMatrix *pMatrix = copy( &scrMatrix );
	ASSERT(pMatrix != NULL);
}

CxMatrix::~CxMatrix( void )
{
	freeMatrix();
}

void CxMatrix::freeMatrix( void )
{
	if (m_pOrigData != NULL) {
		delete[] m_pOrigData;
		m_pOrigData = NULL;
		m_pData = NULL;
	}
	rows = cols = 0;
	length = 0;
	mem_alloc = 0;
}

BOOL CxMatrix::initMatrix( const TCHAR *szName, int _rows, int _cols,
						  int _initMode, /*= INIT_MODE_NONE */
						  int _initFcn   /*= MAT_INIT_NONE */ )
{
	BOOL bResult = FALSE;
	if (_initMode == INIT_MODE_FIRST) {
		rows = cols = 0;
		length = 0;
		mem_alloc = 0;
		m_pOrigData = NULL;
		m_pData = NULL;
	}

	if (_initMode != INIT_MODE_RESIZE) {
		if (szName != NULL)
			_tcscpy_s(m_szName, _countof(m_szName), szName);
		else
			_tcscpy_s(m_szName, _countof(m_szName), _T(""));
	}

	int _size = _rows * _cols;
	if (_size >= 0 && _rows >= 0 && _cols >= 0) {
		freeMatrix();
		rows = _rows;
		cols = _cols;
		length = cols * rows;

		if (_size > 0) {
			mem_alloc = _size + MET_ADDR_ALIGN_SIZE;
			m_pOrigData = new double[mem_alloc];
			if (m_pOrigData != NULL) {
				m_pData = (double *)MET_ADDR_ALIGN(m_pOrigData);
				// init matrix data
				bResult = initData(rows, cols, _initFcn);
			}
		}
		else {
			mem_alloc = 0;
			bResult = TRUE;
		}
	}
	return bResult;
}

BOOL CxMatrix::Create( int _rows, int _cols, int _initFcn /*= MAT_INIT_NONE */ )
{
	return initMatrix(_T(""), _rows, _cols, FALSE, _initFcn);
}

BOOL CxMatrix::createEx( const TCHAR *szName, int _rows, int _cols,
						int _initFcn /*= MAT_INIT_NONE */ )
{
	return initMatrix(szName, _rows, _cols, FALSE, _initFcn);
}

BOOL CxMatrix::initData( int _rows, int _cols, int _initFcn /*= MAT_INIT_NONE */ )
{
	ASSERT(m_pData != NULL);
	if (m_pData == NULL)
		return FALSE;

	ASSERT(_rows <= rows && _rows >= 0 && _cols <= cols && _cols >= 0);
	if (_rows > rows || _rows < 0 || _cols > cols || _cols < 0)
		return FALSE;

	// init matrix data
	int _size = _rows * _cols;
	_size = (_size > length) ? _size : length;
	ASSERT(_size >= 0);

	if (_size <= 0) {
		if (_size == 0)
			freeMatrix();
		return (_size >= 0);
	}

	double _dblRand;
	switch (_initFcn) {
	case MAT_INIT_ZEROS:		// all zeros
		for (int i=0; i<_size; i++)
			m_pData[i] = 0.0;
		break;
	case MAT_INIT_ONES:			// all ones
		for (int i=0; i<_size; i++)
			m_pData[i] = 1.0;
		break;
	case MAT_INIT_RANDS:		// all [-1,1] randomize
		for (int i=0; i<_size; i++) {
			_dblRand = 2.0 * (double)rand() / (double)(RAND_MAX + 1) - 1.0;
			m_pData[i] = _dblRand;
		}
		break;
	case MAT_INIT_RANDS2:		// all [0,1] randomize
		for (int i=0; i<_size; i++) {
			_dblRand = (double)rand() / (double)(RAND_MAX + 1);
			m_pData[i] = _dblRand;
		}
		break;
	case MAT_INIT_NONE:
		// do nothing
		break;
	default:
		// don't init matrix
		break;
	}
	return TRUE;
}

int CxMatrix::size( int n ) const
{
	int _size = 0;
	if (n == 1)
		_size = rows;
	else if (n == 2)
		_size = cols;
	else
		_size = rows * cols;
	return _size;
}

//////////////////////////////////////////////////////////////////////
//
// sameSize() 判断矩阵行数或列数是否相同
//
// n = 0或其他, 行数和列数是否相同 (默认)
// n = 1,       行数是否相同
// n = 2,       列数是否相同
//
//////////////////////////////////////////////////////////////////////

BOOL CxMatrix::sameSize( const CxMatrix *target, int n /*= 0 */ )
{
	if (target != NULL) {
		if (n == 1)
			return (rows == target->rows);
		else if (n == 2)
			return (cols == target->cols);
		else
			return (rows == target->rows && cols == target->cols);
	}
	return FALSE;
}

int CxMatrix::resize( int _rows, int _cols, int _initFcn /*= 0 */ )
{
	int _size = -1;
	if (_rows == rows && _cols == cols) {
		if (initData(_rows, _cols, _initFcn)) {
			_size = rows * cols;
			length = _size;
		}
	}
	else {
		if (initMatrix(NULL, _rows, _cols, INIT_MODE_RESIZE, _initFcn)) {
			_size = rows * cols;
			length = _size;
		}
	}
	return _size;
}

CxMatrix * CxMatrix::copy( const CxMatrix *srcMartix )
{
	CxMatrix *dstMartix = NULL;
	double *srcDataPtr = NULL;
	int _rows, _cols;

	if (srcMartix == this)
		return this;

	if (srcMartix != NULL) {
		_rows = srcMartix->rows;
		_cols = srcMartix->cols;
		if (_rows >= 0 && _cols >= 0) {
			int _size = resize(_rows, _cols);
			if (_size == (_rows * _cols) && _size >= 0) {
				srcDataPtr = srcMartix->getDataPtr();
				if (m_pData != NULL && srcDataPtr != NULL) {
					int _sizeNew = rows * cols;
#if 1
					// copy the pointer
					if (m_pData != srcDataPtr && _sizeNew > 0)
						memcpy(m_pData, srcDataPtr, sizeof(double) * _sizeNew);
#elif 0
					double *dstItemsPtr = m_pData;
					for (int i=0; i<_sizeNew; i++)
						*dstItemsPtr++ = *srcItemsPtr++;
#else
					for (int i=0; i<_cols; i++) {
						for (int j=0; j<_rows; j++) {
							int _index = i * _rows + j;
							if (_index >= 0 && _index < _sizeNew)
								m_pData[_index] = srcMartix->getElement(j, i);
						}
					}
#endif
				}
				dstMartix = this;
			}
		}
	}
	return dstMartix;
}

CxMatrix * CxMatrix::clone( const CxMatrix *srcMartix )
{
	return copy(srcMartix);
}

double CxMatrix::getElement( int _index ) const
{
	if (m_pData != NULL) {
		int _size = rows * cols;
		if (_index >= 0 && _index < _size) {
			return m_pData[_index];
		}
	}
	return MAT_NAN_ITEM;
}

double CxMatrix::getElement( int _row, int _col ) const
{
	int _index;
	if (_row >= 0 && _row < rows && _col >= 0 && _col < cols) {
		_index = _col * rows + _row;
		return getElement(_index);
	}
	return MAT_NAN_ITEM;
}

BOOL CxMatrix::setElement( int _index, double _value )
{
	if (m_pData != NULL) {
		int _size = rows * cols;
		if (_index >= 0 && _index < _size) {
			m_pData[_index] = _value;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CxMatrix::setElement( int _row, int _col, double _value )
{
	int _index;
	if (_row >= 0 && _row < rows && _col >= 0 && _col < cols) {
		_index = _col * rows + _row;
		return setElement(_index, _value);
	}
	return FALSE;
}

double * CxMatrix::setDataBuf( double *pBuffer, int _length )
{
	if (pBuffer != NULL) {
		int _size = rows * cols;
		if (_length > 0 && _length <= _size) {
			if (m_pData != NULL && pBuffer != m_pData) {
				memcpy(m_pData, pBuffer, sizeof(double) * _length);
				return pBuffer;
			}
		}
	}
	return NULL;
}

double * CxMatrix::setDataBuf( double *pBuffer, int _rows, int _cols )
{
	if (_rows >= 0 && _rows <= rows && _cols >= 0 && _cols <= cols) {
		return setDataBuf(pBuffer, _rows * _cols);
	}
	return NULL;
}

CxMatrix & CxMatrix::operator = ( CxMatrix & _Right )
{
	if (&_Right != this) {
		CxMatrix *dstMatrix = copy(&_Right);
		ASSERT(dstMatrix != NULL);
	}

	// finally return a reference to ourselves
	return *this;
}

BOOL CxMatrix::operator == ( CxMatrix & _Right )
{
	// 首先检查行列数是否相等
	if (this == &_Right)
		return TRUE;

	if (rows != _Right.rows || cols != _Right.cols)
		return FALSE;

	for (int i=0; i<rows; ++i) {
		for (int j=0; j<cols; ++j) {
			if (getElement(i, j) != _Right.getElement(i, j))
				return FALSE;
		}
	}

	return TRUE;
}

BOOL CxMatrix::operator != ( CxMatrix & _Right )
{
	return !(*this == _Right);
}

CxMatrix CxMatrix::operator + ( double _value )
{
	// 复制目标矩阵
	CxMatrix _Result((CxMatrix &)*this);		// 拷贝构造

	// 矩阵加法
	for (int i=0; i<rows; ++i) {
		for (int j=0; j<cols; ++j)
			_Result.setElement(i, j, getElement(i, j) + _value);
	}

	return _Result;
}

CxMatrix operator + ( double _value, CxMatrix & _Right )
{
	// 复制目标矩阵
	CxMatrix _Result((CxMatrix &)_Right);		// 拷贝构造

	// 进行减法操作
	for (int i=0; i<_Right.rows; ++i) {
		for (int j=0; j<_Right.cols; ++j)
			_Result.setElement(i, j, _value + _Right.getElement(i, j));
	}

	return _Result;
}

CxMatrix CxMatrix::operator + ( CxMatrix & _Right )
{
	// 首先检查行列数是否相等
	ASSERT(rows == _Right.rows && cols == _Right.cols);

	// 复制目标矩阵
	CxMatrix _Result((CxMatrix &)*this);		// 拷贝构造

	// 矩阵加法
	for (int i=0; i<rows; ++i) {
		for (int j=0; j<cols; ++j)
			_Result.setElement(i, j, getElement(i, j) + _Right.getElement(i, j));
	}

	return _Result;
}

CxMatrix & CxMatrix::operator += ( CxMatrix & _Right )
{
	// 首先检查行列数是否相等
	ASSERT(rows == _Right.rows && cols == _Right.cols);

	// 矩阵加法
	for (int i=0; i<rows; ++i) {
		for (int j=0; j<cols; ++j)
			setElement(i, j, getElement(i, j) + _Right.getElement(i, j));
	}

	return *this;
}

CxMatrix CxMatrix::operator - ( double _value )
{
	// 复制目标矩阵
	CxMatrix _Result((CxMatrix &)*this);		// 拷贝构造

	// 进行减法操作
	for (int i=0; i<rows; ++i) {
		for (int j=0; j<cols; ++j)
			_Result.setElement(i, j, getElement(i, j) - _value);
	}

	return _Result;
}

CxMatrix operator - ( double _value, CxMatrix & _Right )
{
	// 复制目标矩阵
	CxMatrix _Result((CxMatrix &)_Right);		// 拷贝构造

	// 进行减法操作
	for (int i=0; i<_Right.rows; ++i) {
		for (int j=0; j<_Right.cols; ++j)
			_Result.setElement(i, j, _value - _Right.getElement(i, j));
	}

	return _Result;
}

CxMatrix CxMatrix::operator - ( CxMatrix & _Right )
{
	// 首先检查行列数是否相等
	ASSERT(rows == _Right.rows && cols == _Right.cols);

	// 复制目标矩阵
	CxMatrix _Result((CxMatrix &)*this);		// 拷贝构造

	// 进行减法操作
	for (int i=0; i<rows; ++i) {
		for (int j=0; j<cols; ++j)
			_Result.setElement(i, j, getElement(i, j) - _Right.getElement(i, j));
	}

	return _Result;
}

CxMatrix & CxMatrix::operator -= ( CxMatrix & _Right )
{
	// 首先检查行列数是否相等
	ASSERT(rows == _Right.rows && cols == _Right.cols);

	// 进行减法操作
	for (int i=0; i<rows; ++i) {
		for (int j=0; j<cols; ++j)
			setElement(i, j, getElement(i, j) - _Right.getElement(i, j));
	}

	return *this;
}

CxMatrix CxMatrix::operator * ( double _value )
{
	// 复制目标矩阵
	CxMatrix _Result((CxMatrix &)*this);		// copy ourselves

	// 进行数乘
	for (int i=0; i<rows; ++i) {
		for (int j=0; j<cols; ++j)
			_Result.setElement(i, j, getElement(i, j) * _value) ;
	}

	return _Result;
}

CxMatrix operator * ( double _value, CxMatrix & _Right )
{
	// 复制目标矩阵
	CxMatrix _Result((CxMatrix &)_Right);		// 拷贝构造

	// 进行减法操作
	for (int i=0; i<_Right.rows; ++i) {
		for (int j=0; j<_Right.cols; ++j)
			_Result.setElement(i, j, _value * _Right.getElement(i, j));
	}

	return _Result;
}

CxMatrix CxMatrix::operator * ( CxMatrix & _Right )
{
	// 首先检查乘矩阵的行数和被乘矩阵的列数是否相同
	ASSERT(cols == _Right.rows);

	int _newRows, _newCols, _oldCols;
	_newRows = rows;
	_newCols = _Right.cols;
	_oldCols = cols;

	// 创建目标乘积矩阵
	CxMatrix _Result(_newRows, _newCols);

	// 矩阵乘法，即
	//
	// [A][B][C]   [G][H]     [A*G + B*I + C*K][A*H + B*J + C*L]
	// [D][E][F] * [I][J]  =  [D*G + E*I + F*K][D*H + E*J + F*L]
	//             [K][L]
	//
	double _value;
	for (int i=0; i<_newRows; ++i) {
		for (int j=0; j<_newCols; ++j) {
			_value = 0.0;
			for (int k=0; k<_oldCols; ++k)
				_value += getElement(i, k) * _Right.getElement(k, j);
			_Result.setElement(i, j, _value);
		}
	}

	return _Result;
}

CxMatrix & CxMatrix::operator *= ( double _value )
{
	// 进行数乘
	for (int i=0; i<rows; ++i) {
		for (int j=0; j<cols; ++j)
			setElement(i, j, getElement(i, j) * _value) ;
	}

	return *this;
}

CxMatrix & CxMatrix::operator *= ( CxMatrix & _Right )
{
	// 首先检查乘矩阵的行数和被乘矩阵的列数是否相同
	ASSERT(cols == _Right.rows);

	// 复制目标矩阵
	CxMatrix _Left((CxMatrix &)*this);

	int _newRows, _newCols, _oldCols;
	_newRows = rows;
	_newCols = _Right.cols;
	_oldCols = cols;

	// 创建目标乘积矩阵
	int _size = resize(_newRows, _newCols);

	// 矩阵乘法，即
	//
	// [A][B][C]   [G][H]     [A*G + B*I + C*K][A*H + B*J + C*L]
	// [D][E][F] * [I][J]  =  [D*G + E*I + F*K][D*H + E*J + F*L]
	//             [K][L]
	//
	double _value;
	for (int i=0; i<_newRows; ++i) {
		for (int j=0; j<_newCols; ++j) {
			_value = 0.0;
			for (int k=0; k<_oldCols; ++k)
				_value += _Left.getElement(i, k) * _Right.getElement(k, j);
			setElement(i, j, _value);
		}
	}

	return *this;
}

CxMatrix CxMatrix::operator ^ ( double _value )
{
	// 复制目标矩阵
	CxMatrix _Result((CxMatrix &)*this);		// copy ourselves

	double _base, _power;
	// 进行乘方
	for (int i=0; i<rows; ++i) {
		for (int j=0; j<cols; ++j) {
			_base  = getElement(i, j);
			_power = pow(_base, _value);
			_Result.setElement(i, j, _power);
		}
	}

	return _Result;
}

CxMatrix & CxMatrix::operator ^= ( double _value )
{
	double _base, _power;
	// 进行乘方
	for (int i=0; i<rows; ++i) {
		for (int j=0; j<cols; ++j) {
			_base  = getElement(i, j);
			_power = pow(_base, _value);
			setElement(i, j, _power);
		}
	}

	return *this;
}

CxMatrix CxMatrix::operator / ( double _value )
{
	// 复制目标矩阵
	CxMatrix _Result((CxMatrix &)*this);		// copy ourselves

	// 进行点除
	for (int i=0; i<rows; ++i) {
		for (int j=0; j<cols; ++j)
			_Result.setElement(i, j, getElement(i, j) / _value) ;
	}

	return _Result;
}

CxMatrix operator / ( double _value, CxMatrix & _Right )
{
	// 复制目标矩阵
	CxMatrix _Result((CxMatrix &)_Right);		// 拷贝构造

	// 进行减法操作
	for (int i=0; i<_Right.rows; ++i) {
		for (int j=0; j<_Right.cols; ++j)
			_Result.setElement(i, j, _value / _Right.getElement(i, j));
	}

	return _Result;
}

CxMatrix & CxMatrix::operator /= ( double _value )
{
	// 进行点除
	for (int i=0; i<rows; ++i) {
		for (int j=0; j<cols; ++j)
			setElement(i, j, getElement(i, j) / _value) ;
	}

	return *this;
}

BOOL CxMatrix::makeUnitMatrix( int _size )
{
	int _sizeNew = resize(_size, _size);
	if (_sizeNew >= 0) {
		for (int i=0; i<_size; ++i) {
			for (int j=0; j<_size; ++j) {
				if (i == j)
					setElement(i, j, 1.0);
			}
		}
		return TRUE;
	}
	return FALSE;
}

CxMatrix & CxMatrix::transpose( void )
{
	// 复制目标矩阵
	CxMatrix _Trans((CxMatrix &)*this);		// copy ourselves

	// 转置各元素
	for (int i=0; i<rows; ++i) {
		for (int j=0; j<cols; ++j)
			setElement(j, i, _Trans.getElement(i, j));
	}

	return *this;
}

int CxMatrix::zeros( int _rows, int _cols )
{
	// 重置大小并初始化为全0矩阵
	return resize(_rows, _cols, MAT_INIT_ZEROS);
}

int CxMatrix::ones( int _rows, int _cols )
{
	// 重置大小并初始化为全1矩阵
	return resize(_rows, _cols, MAT_INIT_ONES);
}

int CxMatrix::rands( int _rows, int _cols )
{
	// 重置大小并初始化为[-1,1]随机数矩阵
	return resize(_rows, _cols, MAT_INIT_RANDS);
}

int CxMatrix::rands2( int _rows, int _cols )
{
	// 重置大小并初始化为[0,1]随机数矩阵
	return resize(_rows, _cols, MAT_INIT_RANDS2);
}

CxMatrix CxMatrix::_zeros( int _rows, int _cols ) const
{
	// 复制目标矩阵
	CxMatrix _zeros0(_rows, _cols);

	// 所有元素置0
	for (int i=0; i<_rows; ++i) {
		for (int j=0; j<_cols; ++j)
			_zeros0.setElement(i, j, 0.0);
	}

	return _zeros0;
}

CxMatrix CxMatrix::_ones( int _rows, int _cols ) const
{
	// 复制目标矩阵
	CxMatrix _ones0(_rows, _cols);

	// 所有元素置1
	for (int i=0; i<_rows; ++i) {
		for (int j=0; j<_cols; ++j)
			_ones0.setElement(i, j, 1.0);
	}

	return _ones0;
}

CxMatrix CxMatrix::_rands( int _rows, int _cols ) const
{
	// 复制目标矩阵
	CxMatrix _rands(_rows, _cols);

	// 所有元素置[-1,1]的随机数
	double _dblRand;
	for (int i=0; i<_rows; ++i) {
		for (int j=0; j<_cols; ++j) {
			_dblRand = 2.0 * (double)rand() / (double)(RAND_MAX + 1) - 1.0;
			_rands.setElement(i, j, _dblRand);
		}
	}

	return _rands;
}

CxMatrix CxMatrix::_rands2( int _rows, int _cols ) const
{
	// 复制目标矩阵
	CxMatrix _rands(_rows, _cols);

	// 所有元素置[-1,1]的随机数
	double _dblRand;
	for (int i=0; i<_rows; ++i) {
		for (int j=0; j<_cols; ++j) {
			_dblRand = (double)rand() / (double)(RAND_MAX + 1);
			_rands.setElement(i, j, _dblRand);
		}
	}

	return _rands;
}

//////////////////////////////////////////////////////////////////
// CxMatrixs
//////////////////////////////////////////////////////////////////

CxMatrixs::CxMatrixs( void )
{
	_tcscpy_s(m_szName, _countof(m_szName), _T(""));

	numMatrixs = 0;
	pMatrixs = NULL;
}

CxMatrixs::CxMatrixs( int _numMatrixs )
{
	_tcscpy_s(m_szName, _countof(m_szName), _T(""));

	numMatrixs = _numMatrixs;
	pMatrixs = NULL;
}

CxMatrixs::CxMatrixs( const TCHAR *szName, int _numMatrixs )
{
	if (szName != NULL)
		_tcscpy_s(m_szName, _countof(m_szName), szName);
	else
		_tcscpy_s(m_szName, _countof(m_szName), _T(""));

	numMatrixs = _numMatrixs;
	pMatrixs = NULL;
}

CxMatrixs::~CxMatrixs( void )
{
	freeMatrixs();
}

void CxMatrixs::freeMatrixs( void )
{
	if (pMatrixs != NULL) {
		delete[] pMatrixs;
		pMatrixs = NULL;
	}
}

BOOL CxMatrixs::create( const TCHAR *szName, int _numMatrixs )
{
	return TRUE;
}

BOOL CxMatrixs::setMatrixs( int _numMatrixs )
{
	numMatrixs = _numMatrixs;
	return TRUE;
}

CxMatrixs * CxMatrixs::copy( const CxMatrixs *srcMartixs )
{
	return NULL;
}

CxMatrixs * CxMatrixs::clone( const CxMatrixs *srcMartixs )
{
	return copy(srcMartixs);
}

//////////////////////////////////////////////////////////////////
// CxMatrixList
//////////////////////////////////////////////////////////////////

CxMatrixList::CxMatrixList( void )
{

}

CxMatrixList::~CxMatrixList( void )
{

}
