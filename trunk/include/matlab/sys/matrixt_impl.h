
#if !defined(_MATRIXT_IMPL_internal_H_)
#error Do not #include this internal file directly; use matrixt.h header file instead.
#endif

#ifndef _MATRIXT_IMPL_H_
#define _MATRIXT_IMPL_H_

#ifndef _MATRIXT_IMPL_internal_H_
namespace matlab {
#endif

#include <math.h>

template<typename T>
MatrixT<T>::MatrixT( void ) :
    pvData(NULL), pvOrigPtr(NULL), alloc_size(0), rows(0), cols(0), totals(0)
{
}

template<typename T>
MatrixT<T>::MatrixT( int _size )
{
    initialize(_size, _size, INIT_MODE_CONSTRUCTOR);
}

template<typename T>
MatrixT<T>::MatrixT( int _rows, int _cols, int _initFcn /*= MAT_INIT_NONE */ )
{
    initialize(_rows, _cols, INIT_MODE_CONSTRUCTOR, static_cast<T>(0), _initFcn);
}

template<typename T>
matlab::MatrixT<T>::MatrixT( int _rows, int _cols, const value_type& _x )
{
    initialize(_rows, _cols, INIT_MODE_CONSTRUCTOR);
    set_by_scalar(_x);
}

template<typename T>
matlab::MatrixT<T>::MatrixT( int _rows, int _cols, const value_type* _array )
{
    initialize(_rows, _cols, INIT_MODE_CONSTRUCTOR);
    copy_from_array(_array);
}

template<typename T>
MatrixT<T>::MatrixT( const TCHAR *szName, int _rows, int _cols, int _initFcn /*= MAT_INIT_NONE */ )
{
    initialize_ex(szName, _rows, _cols, INIT_MODE_CONSTRUCTOR, static_cast<T>(0), _initFcn);
}

template<typename T>
MatrixT<T>::MatrixT( const MatrixT<T>& src )
{
    initialize_ex(NULL, src.rows, src.cols, INIT_MODE_CONSTRUCTOR);
    copy_from_array(src.data());

    //MatrixT<T>* pMatrix = copy( &src );
    //ASSERT(pMatrix != NULL);
}

template<typename T>
MatrixT<T>::MatrixT( const MatrixT<T>& src, bool b_copy_data )
{
    int _rows, _cols;
    _rows = src.rows;
    _cols = src.cols;

    initialize_ex(NULL, _rows, _cols, INIT_MODE_CONSTRUCTOR);

    if (b_copy_data) {
        MatrixT<T>* pMatrix = copy( &src );
        ASSERT(pMatrix != NULL);
    }
}

template<typename T>
MatrixT<T>::~MatrixT( void )
{
    destroy();
}

template<typename T>
inline void MatrixT<T>::destroy( void )
{
    if (pvOrigPtr != NULL) {
        delete[] pvOrigPtr;
        pvOrigPtr = NULL;
        pvData = NULL;
    }
    rows = cols = 0;
    totals = 0;
    alloc_size = 0;
}

/* Notice: use the function carefully! */
template<typename T>
inline void MatrixT<T>::free( void )
{
    if (pvOrigPtr != NULL) {
        delete[] pvOrigPtr;
        pvOrigPtr = NULL;
        pvData = NULL;
    }
}

template<typename T>
inline void MatrixT<T>::initialize( int _rows, int _cols,
                                   int _initMode /*= INIT_MODE_NONE*/,
                                   value_type _fillVal /*= 0.0*/,
                                   int _initFcn /*= MAT_INIT_DEFAULT*/ )
{
    initialize_ex(NULL, _rows, _cols, _initMode, _fillVal, _initFcn);
}

template<typename T>
void MatrixT<T>::initialize_ex( const TCHAR *szName, int _rows, int _cols,
                               int _initMode /*= INIT_MODE_NONE*/,
                               value_type _fillVal /*= 0.0*/,
                               int _initFcn /*= MAT_INIT_DEFAULT*/ )
{
    int _alloc_size;
    int _totals;
    const int nAdditionSize =
        (int)ceil(double(MAT_ADDR_ALIGN_SIZE) / (double)sizeof(typename T));

    if (_initMode == INIT_MODE_CONSTRUCTOR) {
        rows = cols = 0;
        totals      = 0;
        alloc_size  = 0;
        pvOrigPtr   = NULL;
        pvData      = NULL;
    }

    if (_initMode != INIT_MODE_RESIZE) {
        if (szName != NULL)
            _tcscpy_s(m_szName, _countof(m_szName), szName);
        else
            _tcscpy_s(m_szName, _countof(m_szName), _T(""));
    }

    _totals = _rows * _cols;
    _alloc_size = _totals + nAdditionSize;

    pointer pvNewOrigPtr = new value_type[_alloc_size];
    if (pvNewOrigPtr != NULL) {
        pointer pvNewData = (pointer)MAT_ADDR_ALIGN(pvNewOrigPtr);
        if (pvOrigPtr == NULL) {
            // save the new data buffer
            pvOrigPtr = pvNewOrigPtr;
            pvData = pvNewData;

            // save the new sizes
            rows = _rows;
            cols = _cols;
            totals = _totals;
            alloc_size = _alloc_size;

            // fill data for matrix
            if (_initFcn != FILL_MODE_NONE)
                fill_data(_rows, _cols, _fillVal, _initFcn);
        }
        else {
            // reserve(copy) old data and fill data buffer
            reserve_and_fill_data(pvNewData, _rows, _cols, _fillVal, _initFcn);

            // clear old data
            if (pvOrigPtr != NULL)
                delete[] pvOrigPtr;

            // save the new data buffer
            pvOrigPtr = pvNewOrigPtr;
            pvData = pvNewData;

            // save the new sizes
            rows = _rows;
            cols = _cols;
            totals = _totals;
            alloc_size = _alloc_size;
        }				
    }
}

template<typename T>
void MatrixT<T>::fill_data( int _rows, int _cols,
                           value_type _fillVal /*= 0.0*/,
                           int _initFcn /*= MAT_INIT_DEFAULT */ )
{
    ASSERT(pvData != NULL);
    ASSERT(_rows <= rows && _rows >= 0 && _cols <= cols && _cols >= 0);

    // init matrix data
    int _totals = _rows * _cols;
    ASSERT(_totals >= 0);
    if (totals <= 0)
        return;

    switch (_initFcn) {
    case FILL_MODE_ZEROS:		    // all zeros
        for (int i=0; i<_totals; ++i)
            pvData[i] = static_cast<T>(0);
        break;
    case FILL_MODE_ONES:			    // all ones
        for (int i=0; i<_totals; ++i)
            pvData[i] = static_cast<T>(1);
        break;
    case FILL_MODE_RANDS:		    // all [-1,1] randomize
        for (int i=0; i<_totals; ++i)
            pvData[i] = (value_type)(2.0 * (value_type)rand() / (value_type)(RAND_MAX) - 1.0);
        break;
    case FILL_MODE_RANDS_POSITIVE:	// all [0,1] positive randomize
        for (int i=0; i<_totals; ++i)
            pvData[i] = (value_type)rand() / (value_type)(RAND_MAX);
        break;
    case FILL_MODE_EYES:
        // ��δ�ṩ
        break;
    case FILL_MODE_NONE:
        // do nothing
        break;
    }
}

#define MATRIX_FILL_DATA_FUNC_SIGNED(T) \
template<> \
void MatrixT<T>::fill_data( int _rows, int _cols, \
                           value_type _fillVal /*= 0.0*/, \
                           int _initFcn /*= MAT_INIT_DEFAULT */ ) \
{ \
    ASSERT(pvData != NULL); \
    ASSERT(_rows <= rows && _rows >= 0 && _cols <= cols && _cols >= 0); \
                            \
    /* init matrix data */  \
    int _totals = _rows * _cols; \
    ASSERT(_totals >= 0); \
    if (totals <= 0) \
        return; \
                        \
    switch (_initFcn) { \
    case FILL_MODE_ZEROS:		    /* all zeros */ \
        for (int i=0; i<_totals; ++i) \
            pvData[i] = static_cast<T>(0); \
        break; \
    case FILL_MODE_ONES:			    /* all ones */ \
        for (int i=0; i<_totals; ++i) \
            pvData[i] = static_cast<T>(1); \
        break; \
    case FILL_MODE_RANDS:		    /* all [-1,1] randomize */ \
        /* whether is the signed integer type? */ \
        if ((value_type)(0) > (value_type)(-1)) { \
            /* signed integer */ \
            for (int i=0; i<_totals; ++i) \
                pvData[i] = (value_type)((rand() % 3) - 1); \
        } \
        else { \
            /* unsigned integer, not allowed -1 */ \
            for (int i=0; i<_totals; ++i) \
                pvData[i] = (value_type)(rand() & 1); \
        } \
        break; \
    case FILL_MODE_RANDS_POSITIVE:	/* all [0,1] positive randomize */ \
        for (int i=0; i<_totals; ++i) \
            pvData[i] = (value_type)(rand() & 1); \
        break; \
    case FILL_MODE_EYES: \
        /* ��δ�ṩ */ \
        break; \
    case FILL_MODE_NONE: \
        /* do nothing */ \
        break; \
    } \
}

#define MATRIX_FILL_DATA_FUNC_UNSIGNED(T) \
template<> \
void MatrixT<T>::fill_data( int _rows, int _cols, \
                           value_type _fillVal /*= 0.0*/, \
                           int _initFcn /*= MAT_INIT_DEFAULT */ ) \
{ \
    ASSERT(pvData != NULL); \
    ASSERT(_rows <= rows && _rows >= 0 && _cols <= cols && _cols >= 0); \
                            \
    /* init matrix data */  \
    int _totals = _rows * _cols; \
    ASSERT(_totals >= 0); \
    if (totals <= 0) \
        return; \
                        \
    switch (_initFcn) { \
    case FILL_MODE_ZEROS:		    /* all zeros */ \
        for (int i=0; i<_totals; ++i) \
            pvData[i] = static_cast<T>(0); \
        break; \
    case FILL_MODE_ONES:			    /* all ones */ \
        for (int i=0; i<_totals; ++i) \
            pvData[i] = static_cast<T>(1); \
        break; \
    case FILL_MODE_RANDS:		    /* all [0,1] randomize */ \
        /* unsigned integer, not allowed -1 */ \
        for (int i=0; i<_totals; ++i) \
            pvData[i] = (value_type)(rand() & 1); \
        break; \
    case FILL_MODE_RANDS_POSITIVE:	/* all [0,1] positive randomize */ \
        for (int i=0; i<_totals; ++i) \
            pvData[i] = (value_type)(rand() & 1); \
        break; \
    case FILL_MODE_EYES: \
        /* ��δ�ṩ */ \
        break; \
    case FILL_MODE_NONE: \
        /* do nothing */ \
        break; \
    } \
}

MATRIX_FILL_DATA_FUNC_SIGNED(int64_t)
MATRIX_FILL_DATA_FUNC_SIGNED(int32_t)
MATRIX_FILL_DATA_FUNC_SIGNED(int16_t)
MATRIX_FILL_DATA_FUNC_SIGNED(int8_t)
MATRIX_FILL_DATA_FUNC_SIGNED(signed char)

MATRIX_FILL_DATA_FUNC_UNSIGNED(uint64_t)
MATRIX_FILL_DATA_FUNC_UNSIGNED(uint32_t)
MATRIX_FILL_DATA_FUNC_UNSIGNED(uint16_t)
MATRIX_FILL_DATA_FUNC_UNSIGNED(uint8_t)
MATRIX_FILL_DATA_FUNC_UNSIGNED(bool)

template<typename T>
void MatrixT<T>::reserve_and_fill_data( pointer pvNewData, int _rows, int _cols,
                                       value_type _fillVal /*= 0.0*/,
                                       int _initFcn /*= MAT_INIT_DEFAULT */ )
{
    ASSERT(pvData != NULL && pvNewData != NULL);
    if (pvData == NULL || pvNewData == NULL)
        return;

    ASSERT(pvNewData != pvData);
    if (pvNewData == pvData)
        return;

    //ASSERT(_rows <= rows && _rows >= 0 && _cols <= cols && _cols >= 0);
    ASSERT(_rows >= 0 && _cols >= 0);
    if (_rows < 0 || _cols < 0)
        return;

    // init matrix data
    int _newTotal, _oldTotal, _copyLength, _fillLength;
    _newTotal = _rows * _cols;

    ASSERT(_newTotal >= 0);
    if (_newTotal == 0) {
        destroy();
        return;
    }

    _oldTotal  = rows * cols;
    _copyLength = MIN(_newTotal, _oldTotal);
    _fillLength = _newTotal - _oldTotal;

    // copy the old data
    if (_copyLength != 0)
        memcpy_s(pvNewData, sizeof(typename T) * _copyLength, pvData, sizeof(typename T) * _copyLength);

    if (_initFcn == FILL_MODE_NONE || _fillLength <= 0)
        return;

    pointer pvNewDataFill = pvNewData + _copyLength;
    if (pvNewDataFill == NULL)
        return;

    double _fRndNum;
    switch (_initFcn) {
    case FILL_MODE_ZEROS:		// all zeros
        for (int i=0; i<_fillLength; i++)
            pvNewDataFill[i] = static_cast<T>(0);
        break;
    case FILL_MODE_ONES:			// all ones
        for (int i=0; i<_fillLength; i++)
            pvNewDataFill[i] = static_cast<T>(1);
        break;
    case FILL_MODE_RANDS:		// all [-1,1] randomize
        for (int i=0; i<_fillLength; i++) {
            _fRndNum = 2.0 * (double)rand() / (double)(RAND_MAX + 1) - 1.0;
            pvNewDataFill[i] = static_cast<T>(_fRndNum);
        }
        break;
    case FILL_MODE_RANDS_POSITIVE:		// all [0,1] randomize
        for (int i=0; i<_fillLength; i++) {
            _fRndNum = (double)rand() / (double)(RAND_MAX + 1);
            pvNewDataFill[i] = static_cast<T>(_fRndNum);
        }
        break;
    case FILL_MODE_EYES:
        // ��δ�ṩ
        break;
    case FILL_MODE_NONE:
        // do nothing
        break;
    }
}

template<typename T>
inline bool MatrixT<T>::empty( void ) const
{
	return (sizes() == 0);
}

template<typename T>
inline void MatrixT<T>::clear( void )
{
    destroy();
}

//////////////////////////////////////////////////////////////////////
//
// size() ��ȡ�����������������ܳ���
//
// n = 0������, �����ܳ��� (Ĭ��)
// n = 1,       ��������
// n = 2,       ��������
//
//////////////////////////////////////////////////////////////////////

template <typename T>
inline typename MatrixT<T>::size_type MatrixT<T>::size( int n /*= 0 */ ) const
{
	size_type _size;
	if (n == 1)
		_size = rows;
	else if (n == 2)
		_size = cols;
	else
		_size = totals;
	return _size;
}

//////////////////////////////////////////////////////////////////////
//
// sizes() ��ȡ�����ܳ���(����)
//
//////////////////////////////////////////////////////////////////////

template<typename T>
inline typename MatrixT<T>::size_type MatrixT<T>::sizes( void ) const
{
    return totals;
}

//////////////////////////////////////////////////////////////////////
//
// is_same_size() �жϾ��������������Ƿ���ͬ
//
// n = 0������, �����������Ƿ���ͬ (Ĭ��)
// n = 1,       �����Ƿ���ͬ
// n = 2,       �����Ƿ���ͬ
//
//////////////////////////////////////////////////////////////////////

template<typename T>
inline bool MatrixT<T>::is_same_size( const MatrixT<T>* target, int n /*= 0 */ )
{
	if (target != NULL) {
		if (n == 1)
			return (rows == target->rows);
		else if (n == 2)
			return (cols == target->cols);
		else
			return (rows == target->rows && cols == target->cols);
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
//
// is_same_sizes() �жϾ��������������Ƿ���ͬ(���ٰ�)
//
//////////////////////////////////////////////////////////////////////

template<typename T>
inline bool MatrixT<T>::is_same_sizes( const MatrixT<T>& target )
{
	return (rows == target.rows && cols == target.cols);
}

template<typename T>
inline int MatrixT<T>::resize( int _rows, int _cols )
{
    if (rows == _rows && cols == _cols)
        return totals;

    if (_rows * _cols == totals) {
        rows = _rows;
        cols = _cols;
        return totals;
    }

    destroy();
    initialize(_rows, _cols, INIT_MODE_RESIZE);
    return totals;
}

template<typename T>
inline int MatrixT<T>::resize_ex( int _rows, int _cols,
                                 value_type _fillVal /*= 0.0*/,
                                 int _initFcn /*= MAT_INIT_DEFAULT */ )
{
    if (rows == _rows && cols == _cols) {
        if (_initFcn != FILL_MODE_NONE)
            fill_data(_rows, _cols, _fillVal, _initFcn);
        return totals;
    }

    destroy();
    initialize(_rows, _cols, INIT_MODE_RESIZE, _fillVal, _initFcn);
    return totals;
}

template<typename T>
inline void MatrixT<T>::copy_from_array( const_pointer _array )
{
    __MY_ASSERT((_array != NULL), _T(""));

	for (int i=0; i<totals; ++i)
		pvData[i] = _array[i];
}

template<typename T>
inline void matlab::MatrixT<T>::set_by_scalar( const_value_type _x )
{
    __MY_ASSERT((_array != NULL), _T(""));

	for (int i=0; i<totals; ++i)
		pvData[i] = _x;
}

template<typename T>
inline typename MatrixT<T>::size_type
MatrixT<T>::copy_from_array_s( const_pointer _array, size_type _size )
{
    __MY_ASSERT((_array != NULL), _T(""));

    size_type copy_size = (_size <= totals) ? _size : totals;
	for (int i=0; i<(int)copy_size; ++i)
		pvData[i] = _array[i];
    return copy_size;
}

template<typename T>
inline MatrixT<T>* MatrixT<T>::clone( const MatrixT<T>* src )
{
    return NULL;
}

template<typename T>
inline MatrixT<T>* MatrixT<T>::copy( const MatrixT<T>* src )
{
    resize(src->rows, src->cols);
    copy_from_array(src->data());
    return NULL;
}

template<typename T>
void MatrixT<T>::display( void )
{
    display(name());
}

template<typename T>
void MatrixT<T>::display( const TCHAR *szText )
{
#if defined(MATLAB_USE_DISPLAY) && (MATLAB_USE_DISPLAY)
	TRACE(_T("MatrixT<T>: Name = [ %s ], [rows = %d, cols = %d]\n"), szName, rows, cols);
	TRACE(_T("============================================================================================================\n\n"));
	for (int r=0; r<rows; r++) {
		TRACE(_T("\t"));
		for (int c=0; c<cols; c++) {
			TRACE(_T("%0.4f  \t"), get_element(r, c));
		}
		TRACE(_T("\n\n"));
	}
	TRACE(_T("============================================================================================================\n\n"));
#endif
}

template<typename T>
inline MatrixT<T>::operator T*()
{
    return pvData;
}

template<typename T>
inline MatrixT<T>::operator const T*() const
{
    return pvData;
}

template<typename T>
MatrixT<T>& matlab::MatrixT<T>::operator = ( value_type _value )
{
    // resize to one item matrix
    resize(1, 1);

    // set the single data
    set_element(0, 0, _value);

    // finally return a reference to ourselves
    return *this;
}

template<typename T>
MatrixT<T>& MatrixT<T>::operator = ( MatrixT<T>& _Right )
{
    if (&_Right == this || _Right.data() == pvData)
        return *this;

    if (rows == _Right.rows && cols == _Right.cols) {
        copy_from_array(_Right.data());
    }
    else {
        destroy();
        initialize(_Right.rows, _Right.cols, INIT_MODE_RESIZE);
        copy_from_array(_Right.data());

        //MatrixT<T>* dest = copy(&_Right);
        //ASSERT(dest != NULL);
    }

    // finally return a reference to ourselves
    return *this;
}

template<typename T>
inline MatrixT<T>& MatrixT<T>::transpose( void )
{
    // ���л��еľ���, ��Vector
    if (cols == 1 || rows == 1) {
        int temp = rows;
        rows = cols;
        cols = temp;
        return *this;
    }

    // �Գƾ���(�������)
    if (cols == rows) {
	    // ת�ø�Ԫ��
	    for (int i=0; i<rows; ++i) {
		    for (int j=i+1; j<cols; ++j)
			    set_element(j, i, get_element(i, j));
	    }
    }
    else {
	    MatrixT<T> _trans((MatrixT<T> &)*this);
	    // ת�ø�Ԫ��
	    for (int i=0; i<rows; ++i) {
		    for (int j=0; j<cols; ++j)
			    set_element(j, i, _trans.get_element(i, j));
	    }
        int temp = rows;
        rows = cols;
        cols = temp;
    }

	return *this;
}

template<typename T>
inline int MatrixT<T>::zeros( int _rows, int _cols )
{

}

template<typename T>
inline int MatrixT<T>::ones( int _rows, int _cols )
{

}

template<typename T>
inline int MatrixT<T>::rands( int _rows, int _cols )
{

}

template<typename T>
inline int MatrixT<T>::rands2( int _rows, int _cols )
{

}

template<typename T>
inline MatrixT<T> MatrixT<T>::_zeros( int _rows, int _cols ) const
{

}

template<typename T>
inline MatrixT<T> MatrixT<T>::_ones( int _rows, int _cols ) const
{

}

template<typename T>
inline MatrixT<T> MatrixT<T>::_rands( int _rows, int _cols ) const
{

}

template<typename T>
inline MatrixT<T> MatrixT<T>::_rands2( int _rows, int _cols ) const
{

}

#ifndef _MATRIXT_IMPL_internal_H_
}  // namespace matlab
#endif

#endif  // _MATRIXT_IMPL_H_
