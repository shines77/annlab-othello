
#include "../../include/gui/stdafx.h"
#include "../../include/annlab/AnnMatrix.h"
#include <assert.h>
#include <math.h>

/* whether use optimization of matrix operations */
#define MATRIX_FAST_MODE       1

namespace annlab {

//////////////////////////////////////////////////////////////////
// CxVector
//////////////////////////////////////////////////////////////////

CAnnVector::CAnnVector( void )
{
	initVector(NULL, 0, INIT_MODE_FIRST);
}

CAnnVector::CAnnVector( int _size )
{
	initVector(NULL, _size, INIT_MODE_FIRST);
}

CAnnVector::CAnnVector( CAnnVector & srcVerctor )
{
	int _size = srcVerctor.size();

	initVector(srcVerctor.name(), _size, INIT_MODE_FIRST);

	CAnnVector *pVector = copy( &srcVerctor );
	ASSERT(pVector != NULL);
}

CAnnVector::~CAnnVector( void )
{
	freeVector();
}

void CAnnVector::freeVector( void )
{
	if (m_pOrigData != NULL) {
		delete[] m_pOrigData;
		m_pOrigData = NULL;
		m_pData = NULL;
	}
	length    = 0;
	mem_alloc = 0;
}

bool CAnnVector::initVector( const TCHAR *szName, int _size,
						  int _initMode, /*= INIT_MODE_NONE */
						  int _initFcn   /*= MAT_INIT_NONE */ )
{
	bool bResult = false;
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
			bResult = true;
		}
	}
	return bResult;
}

bool CAnnVector::initData(int _size, int _initFcn /*= MAT_INIT_NONE */ )
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

double CAnnVector::operator []( int _index ) const
{
	double _value = 0.0;

	return _value;
}

CAnnVector * CAnnVector::copy( const CAnnVector *srcVector )
{
	if (srcVector != NULL) {
		//
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////
// CxVectors
//////////////////////////////////////////////////////////////////

CAnnVectors::CAnnVectors( void )
{
	length = 0;
}

CAnnVectors::~CAnnVectors( void )
{
	freeVector();
}

void CAnnVectors::freeVector( void )
{
	length = 0;
}

//////////////////////////////////////////////////////////////////
// CxMatrix
//////////////////////////////////////////////////////////////////

CAnnMatrix::CAnnMatrix( void )
{
	initMatrix(NULL, 0, 0, INIT_MODE_FIRST);
}

CAnnMatrix::CAnnMatrix( int _size )
{
	initMatrix(NULL, _size, _size, INIT_MODE_FIRST);
}

CAnnMatrix::CAnnMatrix( int _rows, int _cols, int _initFcn /*= MAT_INIT_NONE */ )
{
	initMatrix(NULL, _rows, _cols, INIT_MODE_FIRST, 0.0, _initFcn);
}

CAnnMatrix::CAnnMatrix( const TCHAR *szName, int _rows, int _cols,
				   int _initFcn /*= MAT_INIT_NONE */ )
{
	initMatrix(szName, _rows, _cols, INIT_MODE_FIRST, 0.0, _initFcn);
}

CAnnMatrix::CAnnMatrix( const CAnnMatrix & scrMatrix )
{
	int _rows, _cols;
	_rows = scrMatrix.rows;
	_cols = scrMatrix.cols;

	initMatrix(NULL, _rows, _cols, INIT_MODE_FIRST);

	CAnnMatrix *pMatrix = copy( &scrMatrix );
	ASSERT(pMatrix != NULL);
}

CAnnMatrix::CAnnMatrix( const CAnnMatrix & scrMatrix, bool bCopyData )
{
	int _rows, _cols;
	_rows = scrMatrix.rows;
	_cols = scrMatrix.cols;

	initMatrix(NULL, _rows, _cols, INIT_MODE_FIRST);

	if (bCopyData) {
		CAnnMatrix *pMatrix = copy( &scrMatrix );
		ASSERT(pMatrix != NULL);
	}
}

CAnnMatrix::~CAnnMatrix( void )
{
	freeMatrix();
}

void CAnnMatrix::freeMatrix( void )
{
	if (m_pOrigData != NULL) {
		delete[] m_pOrigData;
		m_pOrigData = NULL;
		m_pData = NULL;
	}
	rows = cols = 0;
	length = 0;
	malloc_size = 0;
}

bool CAnnMatrix::initMatrix( const TCHAR *szName, int _rows, int _cols,
						  int _initMode, /*= INIT_MODE_NONE */
						  double _fillVal,  /*= 0.0 */
						  int _initFcn   /*= MAT_INIT_NONE */ )
{
	bool bResult = false;
	if (_initMode == INIT_MODE_FIRST) {
		rows = cols = 0;
		length      = 0;
		malloc_size = 0;
		m_pOrigData = NULL;
		m_pData     = NULL;
	}

	if (_initMode != INIT_MODE_RESIZE) {
		if (szName != NULL)
			_tcscpy_s(m_szName, _countof(m_szName), szName);
		else
			_tcscpy_s(m_szName, _countof(m_szName), _T(""));
	}

	const int nAdditionSize = (int)ceil(double(MET_ADDR_ALIGN_SIZE) / (double)sizeof(double));

	int _malloc_size;
	int _length = _rows * _cols;
	if (_length >= 0 && _rows >= 0 && _cols >= 0) {
		if (_length > 0) {
			_malloc_size = _length + nAdditionSize;
			double *pNewOrigData = new double[_malloc_size];
			if (pNewOrigData != NULL) {
				double *pNewData = (double *)MET_ADDR_ALIGN(pNewOrigData);
				if (m_pOrigData == NULL) {
					// save the new data buffer
					m_pOrigData = pNewOrigData;
					m_pData = pNewData;
					// init data for matrix
					bResult = initData(_rows, _cols, _fillVal, _initFcn);
				}
				else {
					// copy data from old data buffer
					bResult = copyData(pNewData, _rows, _cols, _fillVal, _initFcn);
					// clear old data
					if (m_pOrigData != NULL) {
						delete[] m_pOrigData;
						m_pOrigData = NULL;
						m_pData = NULL;
					}
					// save the new data buffer
					m_pOrigData = pNewOrigData;
					m_pData = pNewData;
				}				
				// save the new sizes
				rows = _rows;
				cols = _cols;
				length = cols * rows;
				malloc_size = _malloc_size;
			}
		}
		else {
			if (m_pOrigData != NULL) {
				delete[] m_pOrigData;
				m_pOrigData = NULL;
				m_pData = NULL;
			}
			rows = _rows;
			cols = _cols;
			length = cols * rows;
			malloc_size = 0;
			bResult = true;
		}
	}
	return bResult;
}

bool CAnnMatrix::create( int _rows, int _cols, double _fillVal, /*= 0 */
					  int _initFcn /*= MAT_INIT_NONE */ )
{
	return initMatrix(_T(""), _rows, _cols, FALSE, _fillVal, _initFcn);
}

bool CAnnMatrix::createEx( const TCHAR *szName, int _rows, int _cols,
						double _fillVal, /*= 0 */
						int _initFcn /*= MAT_INIT_NONE */ )
{
	return initMatrix(szName, _rows, _cols, FALSE, _fillVal, _initFcn);
}

bool CAnnMatrix::initData( int _rows, int _cols, double _fillVal, /*= 0.0 */
						int _initFcn /*= MAT_INIT_NONE */ )
{
	ASSERT(m_pData != NULL);
	if (m_pData == NULL)
		return FALSE;

	/*
	ASSERT(_rows <= rows && _rows >= 0 && _cols <= cols && _cols >= 0);
	if (_rows > rows || _rows < 0 || _cols > cols || _cols < 0)
		return FALSE;
	//*/

	ASSERT(_rows >= 0 && _cols >= 0);
	if (_rows < 0 || _cols < 0)
		return FALSE;

	// init matrix data
	int _length = _rows * _cols;

	ASSERT(_length >= 0);
	if (_length == 0) {
		freeMatrix();
		return TRUE;
	}

	double _dblRand;
	switch (_initFcn) {
	case MAT_INIT_ZEROS:		// all zeros
		for (int i=0; i<_length; i++)
			m_pData[i] = 0.0;
		break;
	case MAT_INIT_ONES:			// all ones
		for (int i=0; i<_length; i++)
			m_pData[i] = 1.0;
		break;
	case MAT_INIT_RANDS:		// all [-1,1] randomize
		for (int i=0; i<_length; i++) {
			_dblRand = 2.0 * (double)rand() / (double)(RAND_MAX + 1) - 1.0;
			m_pData[i] = _dblRand;
		}
		break;
	case MAT_INIT_RANDS2:		// all [0,1] randomize
		for (int i=0; i<_length; i++) {
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

bool CAnnMatrix::copyData( double *pNewData, int _rows, int _cols,
						double _fillVal, /*= 0.0*/
						int _initFcn /*= MAT_INIT_DEFAULT */ )
{
	ASSERT(m_pData != NULL && pNewData != NULL);
	if (m_pData == NULL || pNewData == NULL)
		return FALSE;

	ASSERT(pNewData != m_pData);
	if (pNewData == m_pData)
		return FALSE;

	/*
	ASSERT(_rows <= rows && _rows >= 0 && _cols <= cols && _cols >= 0);
	if (_rows > rows || _rows < 0 || _cols > cols || _cols < 0)
		return FALSE;
	//*/

	ASSERT(_rows >= 0 && _cols >= 0);
	if (_rows < 0 || _cols < 0)
		return FALSE;

	// init matrix data
	int _newLength, _oldLength, _copyLength, _fillLength;
	_newLength = _rows * _cols;

	ASSERT(_newLength >= 0);
	if (_newLength == 0) {
		freeMatrix();
		return TRUE;
	}

	_oldLength  = rows * cols;
	_copyLength = MIN(_newLength, _oldLength);
	_fillLength = _newLength - _oldLength;

	// copy the old data
	if (_copyLength != 0)
		memcpy_s(pNewData, sizeof(double) * _copyLength, m_pData, sizeof(double) * _copyLength);

	if (_fillLength <= 0)
		return TRUE;

	double *pNewDataFill = pNewData + _copyLength;
	if (pNewDataFill == NULL)
		return FALSE;

	double _dblRand;
	switch (_initFcn) {
	case MAT_INIT_ZEROS:		// all zeros
		for (int i=0; i<_fillLength; i++)
			pNewDataFill[i] = 0.0;
		break;
	case MAT_INIT_ONES:			// all ones
		for (int i=0; i<_fillLength; i++)
			pNewDataFill[i] = 1.0;
		break;
	case MAT_INIT_RANDS:		// all [-1,1] randomize
		for (int i=0; i<_fillLength; i++) {
			_dblRand = 2.0 * (double)rand() / (double)(RAND_MAX + 1) - 1.0;
			pNewDataFill[i] = _dblRand;
		}
		break;
	case MAT_INIT_RANDS2:		// all [0,1] randomize
		for (int i=0; i<_fillLength; i++) {
			_dblRand = (double)rand() / (double)(RAND_MAX + 1);
			pNewDataFill[i] = _dblRand;
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

int CAnnMatrix::size( int n ) const
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

bool CAnnMatrix::empty( void ) const
{
	return (size() == 0);
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

bool CAnnMatrix::sameSize( const CAnnMatrix *target, int n /*= 0 */ )
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

void CAnnMatrix::clear( void )
{
	freeMatrix();
}

int CAnnMatrix::resize( int _rows, int _cols, double _fillVal /*= 0.0 */, int _initFcn /*= 0 */ )
{
	int _length = -1;
	if (_rows <= 0 || _cols <= 0) {
		clear();
		_length = 0;
	}
	else {
		if (_rows == rows && _cols == cols) {
			if (initData(_rows, _cols, _fillVal, _initFcn)) {
				_length = rows * cols;
				length = _length;
			}
		}
		else {
			if (initMatrix(NULL, _rows, _cols, INIT_MODE_RESIZE, _fillVal, _initFcn)) {
				_length = rows * cols;
				length = _length;
			}
		}
	}
	return _length;
}

CAnnMatrix * CAnnMatrix::copy( const CAnnMatrix *srcMartix )
{
	CAnnMatrix *pMartix = NULL;
	int _rows, _cols;

	if (srcMartix == this)
		return this;

	if (srcMartix != NULL) {
		_rows = srcMartix->rows;
		_cols = srcMartix->cols;
		if (_rows >= 0 && _cols >= 0) {
			int _length = resize(_rows, _cols);
			if (_length == (_rows * _cols) && _length >= 0) {
				double *dstDataPtr = get_data();
				double *srcDataPtr = srcMartix->get_data();
				if (dstDataPtr != NULL && srcDataPtr != NULL) {
					int _newLength = rows * cols;
#if 1
					// copy the pointer
					if (dstDataPtr != srcDataPtr && _newLength > 0)
						memcpy(dstDataPtr, srcDataPtr, sizeof(double) * _newLength);
#elif 0
					if (dstDataPtr != srcDataPtr) {
						for (int i=0; i<_newLength; i++)
							*dstDataPtr++ = *srcDataPtr++;
					}
#else
					for (int i=0; i<_cols; i++) {
						for (int j=0; j<_rows; j++) {
							int _index = i * _rows + j;
							if (_index >= 0 && _index < _newLength)
								m_pData[_index] = srcMartix->getElement(j, i);
						}
					}
#endif
				}
				pMartix = this;
			}
		}
	}
	return pMartix;
}

CAnnMatrix * CAnnMatrix::clone( const CAnnMatrix *srcMartix )
{
	return copy(srcMartix);
}

double CAnnMatrix::getElement( int _index ) const
{
	if (m_pData != NULL) {
		int _length = rows * cols;
		if (_index >= 0 && _index < _length) {
			return m_pData[_index];
		}
	}
	return MAT_NAN_ITEM;
}

double CAnnMatrix::getElement( int _row, int _col ) const
{
	int _index;
	if (_row >= 0 && _row < rows && _col >= 0 && _col < cols) {
		_index = _col * rows + _row;
		return getElement(_index);
	}
	return MAT_NAN_ITEM;
}

bool CAnnMatrix::setElement( int _index, double _value )
{
	if (m_pData != NULL) {
		int _length = rows * cols;
		if (_index >= 0 && _index < _length) {
			m_pData[_index] = _value;
			return TRUE;
		}
	}
	return FALSE;
}

bool CAnnMatrix::setElement( int _row, int _col, double _value )
{
	int _index;
	if (_row >= 0 && _row < rows && _col >= 0 && _col < cols) {
		_index = _col * rows + _row;
		return setElement(_index, _value);
	}
	return FALSE;
}

double * CAnnMatrix::setData( double *pBuffer, int _length )
{
	if (pBuffer != NULL) {
		int _length = rows * cols;
		if (_length > 0 && _length <= _length) {
			if (m_pData != NULL && pBuffer != m_pData) {
				memcpy(m_pData, pBuffer, sizeof(double) * _length);
				return pBuffer;
			}
		}
	}
	return NULL;
}

double * CAnnMatrix::setData( double *pBuffer, int _rows, int _cols )
{
	if (_rows >= 0 && _rows <= rows && _cols >= 0 && _cols <= cols) {
		return setData(pBuffer, _rows * _cols);
	}
	return NULL;
}

double CAnnMatrix::operator[]( int _index )
{
	return getElement(_index);
}

double CAnnMatrix::operator()( int _index )
{
	return getElement(_index);
}

double CAnnMatrix::operator()( int _row, int _col )
{
	return getElement(_row, _col);
}

CAnnMatrix & CAnnMatrix::operator = ( int _Right )
{
	// resize to one item matrix
	resize(1, 1);
	// set the single data
	setElement(0, 0, (double)_Right);

	// finally return a reference to ourselves
	return *this;
}

CAnnMatrix & CAnnMatrix::operator = ( double _Right )
{
	// resize to one item matrix
	resize(1, 1);
	// set the single data
	setElement(0, 0, _Right);

	// finally return a reference to ourselves
	return *this;
}

CAnnMatrix & CAnnMatrix::operator = ( CAnnMatrix & _Right )
{
	if (&_Right != this) {
		CAnnMatrix *dstMatrix = copy(&_Right);
		ASSERT(dstMatrix != NULL);
	}

	// finally return a reference to ourselves
	return *this;
}

bool CAnnMatrix::operator == ( CAnnMatrix & _Right )
{
	// Check whether it is its own matrix
	if (this == &_Right)
		return TRUE;

	// Check matrix of rows and columns are equal
	if (rows != _Right.rows || cols != _Right.cols)
		return FALSE;

	bool bFindDiff = false;

#if MATRIX_FAST_MODE
	int _length   = size();
	int _lenRight = _Right.size();
	double *pData      = get_data();
	double *pDataRight = _Right.get_data();

	ASSERT(pData != NULL && pDataRight != NULL && _length == _lenRight);
	if (pData != NULL && pDataRight != NULL) {
		for (int i=0; i<_length; ++i) {
			if ((*pData) != (*pDataRight)) {
				bFindDiff = true;
				break;
			}
			pData++;
			pDataRight++;
		}
	}
#else
	for (int i=0; i<rows; ++i) {
		for (int j=0; j<cols; ++j) {
			if (getElement(i, j) != _Right.getElement(i, j)) {
				bFindDiff = true;
				break;
			}
		}
	}
#endif

	return (bFindDiff == FALSE);
}

bool CAnnMatrix::operator != ( CAnnMatrix & _Right )
{
	return !(*this == _Right);
}

CAnnMatrix CAnnMatrix::operator + ( double _value )
{
	// Matrix addition
#if MATRIX_FAST_MODE
	// Copy the current matrix
	CAnnMatrix _Result((CAnnMatrix &)*this);		// Copy constructor

	int _length   = _Result.size();
	double *pData = _Result.get_data();

	ASSERT(pData != NULL);
	if (pData != NULL) {
		for (int i=0; i<_length; i++) {
			(*pData) +=_value;
			pData++;
		}
	}
#else
	// Create the result matrix
	CAnnMatrix _Result(rows, cols);

	for (int i=0; i<rows; ++i) {
		for (int j=0; j<cols; ++j)
			_Result.setElement(i, j, getElement(i, j) + _value);
	}
#endif

	return _Result;
}

CAnnMatrix operator + ( double _value, CAnnMatrix & _Right )
{
	// Matrix addition
#if MATRIX_FAST_MODE
	// Copy the target matrix
	CAnnMatrix _Result((CAnnMatrix &)_Right);		// Copy constructor

	int _length   = _Result.size();
	double *pData = _Result.get_data();

	ASSERT(pData != NULL);
	if (pData != NULL) {
		for (int i=0; i<_length; i++) {
			(*pData) += _value;
			pData++;
		}
	}
#else
	// Create the result matrix
	CAnnMatrix _Result(_Right.rows, _Right.cols);

	for (int i=0; i<_Right.rows; ++i) {
		for (int j=0; j<_Right.cols; ++j)
			_Result.setElement(i, j, _value + _Right.getElement(i, j));
	}
#endif

	return _Result;
}

CAnnMatrix CAnnMatrix::operator + ( CAnnMatrix & _Right )
{
	// Copy the current matrix
	CAnnMatrix _Result((CAnnMatrix &)*this);		// Copy constructor
	if (_Right.empty())
		return _Result;

	// Check whether it is its own matrix
	ASSERT(rows == _Right.rows && cols == _Right.cols);

	if (rows != _Right.rows || cols != _Right.cols) {
		throw _T("Incompatible dimensions in operator + ( CxMatrix & _Right ).");
		//exit(1);
		//_Result.copy(this);
		return _Result;
	}

	// Matrix addition
#if MATRIX_FAST_MODE
	int _length   = _Result.size();
	int _lenRight = _Right.size();
	double *pData      = _Result.get_data();
	double *pDataRight = _Right.get_data();

	ASSERT(pData != NULL && pDataRight != NULL && _length == _lenRight);
	if (pData != NULL && pDataRight != NULL && _length == _lenRight) {
		for (int i=0; i<_length; i++) {
			(*pData) += (double)(*pDataRight);
			pData++;
			pDataRight++;
		}
	}
#else
	for (int i=0; i<rows; ++i) {
		for (int j=0; j<cols; ++j)
			_Result.setElement(i, j, getElement(i, j) + _Right.getElement(i, j));
	}
#endif

	return _Result;
}

CAnnMatrix & CAnnMatrix::operator += ( double _value )
{
	// Matrix addition
#if MATRIX_FAST_MODE
	int _length   = size();
	double *pData = get_data();

	ASSERT(pData != NULL);
	if (pData != NULL) {
		for (int i=0; i<_length; i++) {
			(*pData) += _value;
			pData++;
		}
	}
#else
	for (int i=0; i<rows; ++i) {
		for (int j=0; j<cols; ++j)
			setElement(i, j, getElement(i, j) + _value);
	}
#endif

	return *this;
}

CAnnMatrix & CAnnMatrix::operator += ( CAnnMatrix & _Right )
{
	if (_Right.empty())
		return *this;

	// Check whether it is its own matrix
	ASSERT(rows == _Right.rows && cols == _Right.cols);

	if (rows != _Right.rows || cols != _Right.cols) {
		throw _T("Incompatible dimensions in operator += ( CxMatrix & _Right ). ");
		//exit(1);
		return *this;
	}

	// Matrix addition
#if MATRIX_FAST_MODE
	int _length   = size();
	int _lenRight = _Right.size();
	double *pData      = get_data();
	double *pDataRight = _Right.get_data();

	ASSERT(pData != NULL && pDataRight != NULL && _length == _lenRight);
	if (pData != NULL && pDataRight != NULL && _length == _lenRight) {
		for (int i=0; i<_length; i++) {
			(*pData) += (double)(*pDataRight);
			pData++;
			pDataRight++;
		}
	}
#else
	for (int i=0; i<rows; ++i) {
		for (int j=0; j<cols; ++j)
			setElement(i, j, getElement(i, j) + _Right.getElement(i, j));
	}
#endif

	return *this;
}

CAnnMatrix CAnnMatrix::operator - ( double _value )
{
	// Matrix subtraction
#if MATRIX_FAST_MODE
	// Copy the current matrix
	CAnnMatrix _Result((CAnnMatrix &)*this);		// Copy constructor

	int _length   = _Result.size();
	double *pData = _Result.get_data();

	ASSERT(pData != NULL);
	if (pData != NULL) {
		for (int i=0; i<_length; i++) {
			(*pData) -= _value;
			pData++;
		}
	}
#else
	// Create the result matrix
	CAnnMatrix _Result(rows, cols);

	for (int i=0; i<rows; ++i) {
		for (int j=0; j<cols; ++j)
			_Result.setElement(i, j, getElement(i, j) - _value);
	}
#endif

	return _Result;
}

CAnnMatrix operator - ( double _value, CAnnMatrix & _Right )
{
	// Matrix subtraction
#if MATRIX_FAST_MODE
	// Copy the current matrix
	CAnnMatrix _Result((CAnnMatrix &)_Right);		// Copy constructor

	int _length   = _Result.size();
	double *pData = _Result.get_data();

	ASSERT(pData != NULL);
	if (pData != NULL) {
		for (int i=0; i<_length; i++) {
			(*pData) = _value - (double)(*pData);
			pData++;
		}
	}
#else
	// Create the result matrix
	CAnnMatrix _Result(_Right.rows, _Right.cols);

	for (int i=0; i<_Right.rows; ++i) {
		for (int j=0; j<_Right.cols; ++j)
			_Result.setElement(i, j, _value - _Right.getElement(i, j));
	}
#endif

	return _Result;
}

CAnnMatrix CAnnMatrix::operator - ( CAnnMatrix & _Right )
{
	// Copy the current matrix
	CAnnMatrix _Result((CAnnMatrix &)*this);		// Copy constructor
	if (_Right.empty())
		return _Result;

	// Check whether it is its own matrix
	ASSERT(rows == _Right.rows && cols == _Right.cols);

	if (rows != _Right.rows || cols != _Right.cols) {
		throw _T("Incompatible dimensions in operator - ( CxMatrix & _Right ).");
		//exit(1);
		//_Result.copy(this);
		return _Result;
	}

	// Matrix subtraction
#if MATRIX_FAST_MODE
	int _length   = _Result.size();
	int _lenRight = _Right.size();
	double *pData      = _Result.get_data();
	double *pDataRight = _Right.get_data();

	ASSERT(pData != NULL && pDataRight != NULL && _length == _lenRight);
	if (pData != NULL && pDataRight != NULL && _length == _lenRight) {
		for (int i=0; i<_length; i++) {
			(*pData) -= (double)(*pDataRight);
			pData++;
			pDataRight++;
		}
	}
#else
	for (int i=0; i<rows; ++i) {
		for (int j=0; j<cols; ++j)
			_Result.setElement(i, j, getElement(i, j) - _Right.getElement(i, j));
	}
#endif

	return _Result;
}

CAnnMatrix & CAnnMatrix::operator -= ( double _value )
{
	// Matrix subtraction
#if MATRIX_FAST_MODE
	int _length   = size();
	double *pData = get_data();

	ASSERT(pData != NULL);
	if (pData != NULL) {
		for (int i=0; i<_length; i++) {
			(*pData) -= _value;
			pData++;
		}
	}
#else
	for (int i=0; i<rows; ++i) {
		for (int j=0; j<cols; ++j)
			setElement(i, j, getElement(i, j) - _value);
	}
#endif

	return *this;
}

CAnnMatrix & CAnnMatrix::operator -= ( CAnnMatrix & _Right )
{
	if (_Right.empty())
		return *this;

	// Check whether it is its own matrix
	ASSERT(rows == _Right.rows && cols == _Right.cols);

	if (rows != _Right.rows || cols != _Right.cols) {
		throw _T("Incompatible dimensions in operator -= ( CxMatrix & _Right ). ");
		//exit(1);
		return *this;
	}

	// Matrix subtraction
#if MATRIX_FAST_MODE
	int _length   = size();
	int _lenRight = _Right.size();
	double *pData      = get_data();
	double *pDataRight = _Right.get_data();

	ASSERT(pData != NULL && pDataRight != NULL && _length == _lenRight);
	if (pData != NULL && pDataRight != NULL && _length == _lenRight) {
		for (int i=0; i<_length; i++) {
			(*pData) -= (double)(*pDataRight);
			pData++;
			pDataRight++;
		}
	}
#else
	for (int i=0; i<rows; ++i) {
		for (int j=0; j<cols; ++j)
			setElement(i, j, getElement(i, j) - _Right.getElement(i, j));
	}
#endif

	return *this;
}

CAnnMatrix CAnnMatrix::operator * ( double _value )
{
	// Matrix multiplication
#if MATRIX_FAST_MODE
	// Copy the current matrix
	CAnnMatrix _Result((CAnnMatrix &)*this);		// copy ourselves

	int _length   = _Result.size();
	double *pData = _Result.get_data();

	ASSERT(pData != NULL);
	if (pData != NULL) {
		for (int i=0; i<_length; i++) {
			(*pData) *= _value;
			pData++;
		}
	}
#else
	// Create the result matrix
	CAnnMatrix _Result(rows, cols);

	for (int i=0; i<rows; ++i) {
		for (int j=0; j<cols; ++j)
			_Result.setElement(i, j, getElement(i, j) * _value) ;
	}
#endif

	return _Result;
}

CAnnMatrix operator * ( double _value, CAnnMatrix & _Right )
{
	// Matrix multiplication
#if MATRIX_FAST_MODE
	// Copy the current matrix
	CAnnMatrix _Result((CAnnMatrix &)_Right);		// Copy constructor

	int _length   = _Result.size();
	double *pData = _Result.get_data();

	ASSERT(pData != NULL);
	if (pData != NULL) {
		for (int i=0; i<_length; i++) {
			(*pData) *= _value;
			pData++;
		}
	}
#else
	// Create the result matrix
	CAnnMatrix _Result(_Right.rows, _Right.cols);

	for (int i=0; i<_Right.rows; ++i) {
		for (int j=0; j<_Right.cols; ++j)
			_Result.setElement(i, j, _value * _Right.getElement(i, j));
	}
#endif

	return _Result;
}

CAnnMatrix CAnnMatrix::operator * ( CAnnMatrix & _Right )
{
	// 首先检查乘矩阵的行数和被乘矩阵的列数是否相同
	ASSERT(cols == _Right.rows);

	//_Right.display();
	//display();

	int _newRows, _newCols, _oldCols;
	_newRows = rows;
	_newCols = _Right.cols;
	_oldCols = cols;

	// 创建目标乘积矩阵
	CAnnMatrix _Result(_newRows, _newCols);

	// Matrix multiplication，即
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

CAnnMatrix & CAnnMatrix::operator *= ( double _value )
{
	// Matrix multiplication
	for (int i=0; i<rows; ++i) {
		for (int j=0; j<cols; ++j)
			setElement(i, j, getElement(i, j) * _value) ;
	}

	return *this;
}

CAnnMatrix & CAnnMatrix::operator *= ( CAnnMatrix & _Right )
{
	// 首先检查乘矩阵的行数和被乘矩阵的列数是否相同
	ASSERT(cols == _Right.rows);

	// Copy the current matrix
	CAnnMatrix _Left((CAnnMatrix &)*this);

	int _newRows, _newCols, _oldCols;
	_newRows = rows;
	_newCols = _Right.cols;
	_oldCols = cols;

	// 创建目标乘积矩阵
	int _size = resize(_newRows, _newCols);

	// Matrix multiplication，即
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

CAnnMatrix CAnnMatrix::operator ^ ( double _value )
{
	// Copy the current matrix
	CAnnMatrix _Result((CAnnMatrix &)*this);		// copy ourselves

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

CAnnMatrix & CAnnMatrix::operator ^= ( double _value )
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

CAnnMatrix CAnnMatrix::operator / ( double _value )
{
	// Matrix division: dotdiv
#if MATRIX_FAST_MODE
	// Copy the current matrix
	CAnnMatrix _Result((CAnnMatrix &)*this);		// copy ourselves

	int _length   = _Result.size();
	double *pData = _Result.get_data();

	ASSERT(pData != NULL);
	if (pData != NULL) {
		for (int i=0; i<_length; i++) {
			(*pData) /= _value;
			pData++;
		}
	}
#else
	// Create the result matrix
	CAnnMatrix _Result(rows, cols);

	for (int i=0; i<rows; ++i) {
		for (int j=0; j<cols; ++j)
			_Result.setElement(i, j, getElement(i, j) / _value) ;
	}
#endif

	return _Result;
}

CAnnMatrix CAnnMatrix::operator / ( CAnnMatrix &_Right )
{
	// Copy the current matrix
	CAnnMatrix _Result((CAnnMatrix &)*this);		// copy ourselves

	// Check whether it is its own matrix
	ASSERT(rows == _Right.rows && cols == _Right.cols);

	if (rows != _Right.rows || cols != _Right.cols) {
		throw _T("Incompatible dimensions in operator / ( CxMatrix & _Right ).");
		//exit(1);
		//_Result.copy(this);
		return _Result;
	}

	// Matrix division: dotdiv
#if MATRIX_FAST_MODE
	int _length   = _Result.size();
	int _lenRight = _Right.size();
	double *pData = _Result.get_data();
	double *pDataRight = _Right.get_data();

	ASSERT(pData != NULL && pDataRight != NULL && _length == _lenRight);
	if (pData != NULL && pDataRight != NULL && _length == _lenRight) {
		for (int i=0; i<_length; i++) {
			(*pData) /= (double)(*pDataRight);
			pData++;
			pDataRight++;
		}
	}
#else
	for (int i=0; i<rows; ++i) {
		for (int j=0; j<cols; ++j)
			_Result.setElement(i, j, getElement(i, j) / _Right.getElement(i, j)) ;
	}
#endif

	return _Result;
}

CAnnMatrix operator / ( double _value, CAnnMatrix & _Right )
{
	// Copy the current matrix
	CAnnMatrix _Result((CAnnMatrix &)_Right);		// Copy constructor

	// Matrix division
#if MATRIX_FAST_MODE
	int _length   = _Result.size();
	double *pData = _Result.get_data();

	ASSERT(pData != NULL);
	if (pData != NULL) {
		for (int i=0; i<_length; i++) {
			(*pData) = _value / (double)(*pData);
			pData++;
		}
	}
#else
	for (int i=0; i<_Right.rows; ++i) {
		for (int j=0; j<_Right.cols; ++j)
			_Result.setElement(i, j, _value / _Right.getElement(i, j));
	}
#endif

	return _Result;
}

CAnnMatrix & CAnnMatrix::operator /= ( double _value )
{
	// Matrix division: dotdiv
	for (int i=0; i<rows; ++i) {
		for (int j=0; j<cols; ++j)
			setElement(i, j, getElement(i, j) / _value) ;
	}

	return *this;
}

bool CAnnMatrix::makeUnitMatrix( int _size )
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

CAnnMatrix & CAnnMatrix::transpose( void )
{
	// Copy the current matrix
	CAnnMatrix _Trans((CAnnMatrix &)*this);		// copy ourselves

	// 转置各元素
	for (int i=0; i<rows; ++i) {
		for (int j=0; j<cols; ++j)
			setElement(j, i, _Trans.getElement(i, j));
	}

	return *this;
}

int CAnnMatrix::zeros( int _rows, int _cols )
{
	// 重置大小并初始化为全0矩阵
	return resize(_rows, _cols, MAT_INIT_ZEROS);
}

int CAnnMatrix::ones( int _rows, int _cols )
{
	// 重置大小并初始化为全1矩阵
	return resize(_rows, _cols, MAT_INIT_ONES);
}

int CAnnMatrix::rands( int _rows, int _cols )
{
	// 重置大小并初始化为[-1,1]随机数矩阵
	return resize(_rows, _cols, MAT_INIT_RANDS);
}

int CAnnMatrix::rands2( int _rows, int _cols )
{
	// 重置大小并初始化为[0,1]随机数矩阵
	return resize(_rows, _cols, MAT_INIT_RANDS2);
}

CAnnMatrix CAnnMatrix::_zeros( int _rows, int _cols ) const
{
	// Copy the current matrix
	CAnnMatrix _zeros0(_rows, _cols);

	// 所有元素置0
	for (int i=0; i<_rows; ++i) {
		for (int j=0; j<_cols; ++j)
			_zeros0.setElement(i, j, 0.0);
	}

	return _zeros0;
}

CAnnMatrix CAnnMatrix::_ones( int _rows, int _cols ) const
{
	// Copy the current matrix
	CAnnMatrix _ones0(_rows, _cols);

	// 所有元素置1
	for (int i=0; i<_rows; ++i) {
		for (int j=0; j<_cols; ++j)
			_ones0.setElement(i, j, 1.0);
	}

	return _ones0;
}

CAnnMatrix CAnnMatrix::_rands( int _rows, int _cols ) const
{
	// Copy the current matrix
	CAnnMatrix _rands(_rows, _cols);

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

CAnnMatrix CAnnMatrix::_rands2( int _rows, int _cols ) const
{
	// Copy the current matrix
	CAnnMatrix _rands(_rows, _cols);

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

// 获取矩阵的指定行矩阵, _row下标从0开始
CAnnMatrix CAnnMatrix::getRowVector( int _row ) const
{
	CAnnMatrix _Result;
	if (_row >= 0 && _row < rows) {
		_Result.resize(1, cols);
		for (int i= 0; i<cols; i++)
			_Result.setElement(0, i, getElement(_row, i));
	}
	return _Result;
}

// 获取矩阵的指定列矩阵, _col下标从0开始
CAnnMatrix CAnnMatrix::getColVector( int _col ) const
{
	CAnnMatrix _Result;
	if (_col >= 0 && _col < cols) {
		_Result.resize(rows, 1);
		for (int i= 0; i<rows; i++)
			_Result.setElement(i, 0, getElement(i, _col));
	}
	return _Result;
}

void CAnnMatrix::display( void )
{
	TRACE(_T("CxMatrix: Name = [ %s ], [rows = %d, cols = %d]\n"), name(), rows, cols);
	TRACE(_T("============================================================================================================\n\n"));
	for (int r=0; r<rows; r++) {
		TRACE(_T("\t"));
		for (int c=0; c<cols; c++) {
			TRACE(_T("%0.4f  \t"), getElement(r, c));
		}
		TRACE(_T("\n\n"));
	}
	TRACE(_T("============================================================================================================\n\n"));
}

void CAnnMatrix::display( const TCHAR *szName )
{
#if 1
	TRACE(_T("CxMatrix: Name = [ %s ], [rows = %d, cols = %d]\n"), szName, rows, cols);
	TRACE(_T("============================================================================================================\n\n"));
	for (int r=0; r<rows; r++) {
		TRACE(_T("\t"));
		for (int c=0; c<cols; c++) {
			TRACE(_T("%0.4f  \t"), getElement(r, c));
		}
		TRACE(_T("\n\n"));
	}
	TRACE(_T("============================================================================================================\n\n"));
#endif
}

//////////////////////////////////////////////////////////////////
// CxMatrixs
//////////////////////////////////////////////////////////////////

CAnnMatrixs::CAnnMatrixs( void )
{
	_tcscpy_s(m_szName, _countof(m_szName), _T(""));

	numMatrixs = 0;
	pMatrixs = NULL;
}

CAnnMatrixs::CAnnMatrixs( int _numMatrixs )
{
	_tcscpy_s(m_szName, _countof(m_szName), _T(""));

	numMatrixs = _numMatrixs;
	pMatrixs = NULL;
}

CAnnMatrixs::CAnnMatrixs( const TCHAR *szName, int _numMatrixs )
{
	if (szName != NULL)
		_tcscpy_s(m_szName, _countof(m_szName), szName);
	else
		_tcscpy_s(m_szName, _countof(m_szName), _T(""));

	numMatrixs = _numMatrixs;
	pMatrixs = NULL;
}

CAnnMatrixs::~CAnnMatrixs( void )
{
	freeMatrixs();
}

void CAnnMatrixs::freeMatrixs( void )
{
	if (pMatrixs != NULL) {
		delete[] pMatrixs;
		pMatrixs = NULL;
	}
}

bool CAnnMatrixs::create( const TCHAR *szName, int _numMatrixs )
{
	return TRUE;
}

bool CAnnMatrixs::setMatrixs( int _numMatrixs )
{
	numMatrixs = _numMatrixs;
	return TRUE;
}

CAnnMatrixs * CAnnMatrixs::copy( const CAnnMatrixs *srcMartixs )
{
	return NULL;
}

CAnnMatrixs * CAnnMatrixs::clone( const CAnnMatrixs *srcMartixs )
{
	return copy(srcMartixs);
}

//////////////////////////////////////////////////////////////////
// CxMatrixList
//////////////////////////////////////////////////////////////////

CAnnMatrixList::CAnnMatrixList( void )
{
	N       = 0;
	TS      = 0;
	Q       = 0;

	X       = NULL;
	T       = NULL;
	indices = NULL;
	Xi      = NULL;
	Tl      = NULL;
	Pd      = NULL;
	Ai      = NULL;
	Y       = NULL;
}

CAnnMatrixList::~CAnnMatrixList( void )
{

}

CAnnMatrix * CAnnMatrixList::operator[]( int _index )
{
	if (_index < 0)
		return NULL;

	list<CAnnMatrix>::iterator itList;
	int _counter = 0;
	for (itList = begin(); itList != end(); itList++) {
		if (_counter == _index)
			return (CAnnMatrix *)&(*itList);
		_counter++;
	}
	return NULL;
}

CAnnMatrixVector::CAnnMatrixVector( void )
{

}

CAnnMatrixVector::~CAnnMatrixVector( void )
{

}

CAnnXArray::CAnnXArray( void )
{
	length = 0;
	malloc_size = 0;
	items = NULL;
}

CAnnXArray::~CAnnXArray( void )
{
	free();
}

double CAnnXArray::operator[]( int _index )
{
	return 0.0;
}

void CAnnXArray::free( void )
{
	if (items)
		delete[] items;

	length = 0;
	malloc_size = 0;
}

void CAnnXArray::clear( void )
{
	//
}

int CAnnXArray::resize( int _size )
{
	return 0;
}

}  // namespace annlab
