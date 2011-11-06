
#if !defined(_MATRIXT_IMPL_internal_H_)
#error Do not #include this internal file directly; use matrixt.h header file instead.
#endif

#ifndef _MATRIXT_IMPL_H_
#define _MATRIXT_IMPL_H_

#ifndef _MATRIXT_IMPL_internal_H_
namespace annlab {
#endif

#include <math.h>

template<typename T>
MatrixT<T>::MatrixT( void ) :
    pvData(NULL), ppvCol(NULL), pvAlloc(NULL), ppvColAlloc(NULL),
    rows(0), cols(0), totals(0), totals_actual(0), alloc_size(0)
{
}

template<typename T>
MatrixT<T>::MatrixT( int _size )
{
    initialize(_size, _size);
}

template<typename T>
MatrixT<T>::MatrixT( int _rows, int _cols)
{
    initialize(_rows, _cols);
}

template<typename T>
MatrixT<T>::MatrixT( int _rows, int _cols, const value_type& _x )
{
    initialize(_rows, _cols);
    set_by_scalar(_x);
}

template<typename T>
MatrixT<T>::MatrixT( int _rows, int _cols, const value_type* _array )
{
    initialize(_rows, _cols);
    copy_from_array(_array);
}

template<typename T>
MatrixT<T>::MatrixT( int _rows, int _cols, int _initFcn,
                    value_type _fillVal /*= value_type(0.0)*/)
{
    initialize(_rows, _cols, _initFcn, value_type(0.0) );
}

template<typename T>
MatrixT<T>::MatrixT( const TCHAR *szName, int _rows, int _cols,
                    int _initFcn /*= INIT_FCN_DEFAULT */,
                    value_type _fillVal /*= value_type(0.0)*/ )
{
    initialize_ex(szName, _rows, _cols, _initFcn, _fillVal);
}

template<typename T>
MatrixT<T>::MatrixT( const MatrixT<T>& src )
{
    initialize_ex(NULL, src.rows, src.cols);
    copy_from_array(src.get_data());

    //MatrixT<T>* pMatrix = copy( &src );
    //ASSERT(pMatrix != NULL);
}

template<typename T>
MatrixT<T>::MatrixT( const MatrixT<T>& src, bool b_copy_data )
{
    initialize_ex(NULL, src.rows, src.cols);

    if (b_copy_data) {
        copy_from_array(src.get_data());
        //MatrixT<T>* pMatrix = copy( &src );
        //ASSERT(pMatrix != NULL);
    }
}

template<typename T>
MatrixT<T>::~MatrixT( void )
{
    destroy_data();
}

template<typename T>
inline void MatrixT<T>::destroy_data( void )
{
    if (pvAlloc != NULL) {
        delete[] pvAlloc;
        if (ppvColAlloc != NULL)
            delete[] ppvColAlloc;
    }
}

template<typename T>
inline void MatrixT<T>::destroy( void )
{
    if (pvAlloc != NULL) {
        delete[] pvAlloc;
        pvAlloc = NULL;
        pvData = NULL;
        if (ppvColAlloc != NULL) {
            delete[] ppvColAlloc;
            ppvColAlloc = NULL;
            ppvCol = NULL;
        }
    }
    alloc_size = 0;
    rows = cols = 0;
    totals = 0;
    totals_actual = 0;
}

/* Notice: use the function carefully! */
template<typename T>
inline void MatrixT<T>::free( void )
{
    destroy();
}

template<typename T>
inline void MatrixT<T>::initialize( int _rows, int _cols,
                                   int _initFcn /*= INIT_FCN_DEFAULT*/,
                                   value_type _fillVal /*= 0.0*/ )
{
    initialize_ex(NULL, _rows, _cols, _initFcn, _fillVal);
}

template<typename T>
void MatrixT<T>::initialize_ex( const TCHAR *szName, int _rows, int _cols,
                               int _initFcn /*= INIT_FCN_DEFAULT*/,
                               value_type _fillVal /*= 0.0*/ )
{
    size_type _alloc_size;
    int _totals;
    const int nAdditionSize =
        (int)ceil(double(MAT_CACHE_ALIGN_SIZE) / (double)sizeof(typename T));
    const int nAdditionRowSize =
        (int)ceil(double(MAT_CACHE_ALIGN_SIZE) / (double)sizeof(pointer));

    if (szName == NULL)
        _tcscpy_s(m_szName, _countof(m_szName), _T(""));
    else
        _tcscpy_s(m_szName, _countof(m_szName), szName);

    _totals = _rows * _cols;
    _alloc_size = _totals + nAdditionSize;

    pointer pvNewOrigPtr = new value_type[_alloc_size];
    ptr_pointer ppvOrigCol = new pointer[_cols + nAdditionRowSize];
    if (pvNewOrigPtr != NULL && ppvOrigCol != NULL) {
        pointer pvNewData = (pointer)MAT_CACHE_ALIGN_128(pvNewOrigPtr);
        ptr_pointer ppvNewCol = (ptr_pointer)MAT_CACHE_ALIGN_128(ppvOrigCol);

        ASSERT(pvNewData != NULL);
        ASSERT(ppvNewCol != NULL);

        // save the new data buffer
        pvData = pvNewData;
        ppvCol = ppvNewCol;
        pvAlloc = pvNewOrigPtr;
        ppvColAlloc = ppvOrigCol;
        alloc_size = _alloc_size;

        // save the new sizes
        rows = _rows;
        cols = _cols;
        totals = _totals;
        totals_actual = _totals;

        // save the row pointer
        pointer p = pvData;
        for (int i=0; i<cols; ++i) {
            ppvCol[i] = p;
            p += rows;
        }

        // fill data for matrix
        if (_initFcn != INIT_FCN_NONE)
            fill_data(_rows, _cols, _initFcn, _fillVal);
    }
    else {
        pvData      = NULL;
        ppvCol      = NULL;
        pvAlloc     = NULL;
        ppvColAlloc = NULL;
        alloc_size  = 0;

        rows = cols = 0;
        totals      = 0;
        totals_actual = 0;
    }
}

template<typename T>
void MatrixT<T>::init_martix( int _rows, int _cols,
                             int _initMode /*= INIT_TYPE_NONE*/,
                             int _initFcn /*= MAT_INIT_DEFAULT */,
                             value_type _fillVal /*= value_type(0.0)*/ )
{
    size_type _alloc_size;
    int _totals;
    const int nAdditionSize =
        (int)ceil(double(MAT_CACHE_ALIGN_SIZE) / (double)sizeof(typename T));
    const int nAdditionRowSize =
        (int)ceil(double(MAT_CACHE_ALIGN_SIZE) / (double)sizeof(typename pointer));

    _totals = _rows * _cols;
    _alloc_size = _totals + nAdditionSize;

    pointer pvNewOrigPtr = new value_type[_alloc_size];
    ptr_pointer ppvOrigCol = new pointer[_cols + nAdditionRowSize];
    if (pvNewOrigPtr != NULL && ppvOrigCol != NULL) {
        pointer pvNewData = (pointer)MAT_CACHE_ALIGN_128(pvNewOrigPtr);
        ptr_pointer ppvNewCol = (ptr_pointer)MAT_CACHE_ALIGN_128(ppvOrigCol);
        if (pvAlloc == NULL) {
            // save the new data buffer
            pvAlloc = pvNewOrigPtr;
            ppvColAlloc = ppvOrigCol;
            pvData = pvNewData;
            ppvCol = ppvNewCol;
            alloc_size = _alloc_size;

            // save the new sizes
            rows = _rows;
            cols = _cols;
            totals = _totals;
            totals_actual = _totals;

            // save the row pointer
            pointer p = pvData;
            for (int i=0; i<cols; ++i) {
                ppvCol[i] = p;
                p += rows;
            }

            // fill data for matrix
            if (_initFcn != INIT_FCN_NONE)
                fill_data(_rows, _cols, _initFcn, _fillVal);
        }
        else {
            // reserve(copy) old data and fill data buffer
            //reserve_and_fill_data(pvNewData, _rows, _cols, _initFcn, _fillVal);

            // clear old data
            if (pvAlloc != NULL) {
                delete[] pvAlloc;
                if (ppvColAlloc = NULL)
                    delete[] ppvColAlloc;
            }

            // save the new data buffer
            pvAlloc = pvNewOrigPtr;
            ppvColAlloc = ppvOrigCol;
            pvData = pvNewData;
            ppvCol = ppvNewCol;
            alloc_size = _alloc_size;

            // save the new sizes
            rows = _rows;
            cols = _cols;
            totals = _totals;
            totals_actual = _totals;

            // save the row pointer
            pointer p = pvData;
            for (int i=0; i<cols; ++i) {
                ppvCol[i] = p;
                p += rows;
            }
        }
    }
}

template<typename T>
void MatrixT<T>::reserve_and_fill_data( pointer pvNewData, int _rows, int _cols,
                                       int _initFcn /*= MAT_INIT_DEFAULT */,
                                       value_type _fillVal /*= 0.0*/ )
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

    if (_initFcn == INIT_FCN_NONE || _fillLength <= 0)
        return;

    pointer pvNewDataFill = pvNewData + _copyLength;
    if (pvNewDataFill == NULL)
        return;

    double _fRndNum;
    switch (_initFcn) {
    case INIT_FCN_ZEROS:       // all zeros
        for (int i=0; i<_fillLength; i++)
            pvNewDataFill[i] = value_type(0.0);
        break;
    case INIT_FCN_ONES:            // all ones
        for (int i=0; i<_fillLength; i++)
            pvNewDataFill[i] = static_cast<T>(1);
        break;
    case INIT_FCN_RANDS:       // all [-1,1] randomize
        for (int i=0; i<_fillLength; i++) {
            _fRndNum = 2.0 * (double)rand() / (double)(RAND_MAX + 1) - 1.0;
            pvNewDataFill[i] = static_cast<T>(_fRndNum);
        }
        break;
    case INIT_FCN_RANDS_POSITIVE:      // all [0,1] randomize
        for (int i=0; i<_fillLength; i++) {
            _fRndNum = (double)rand() / (double)(RAND_MAX + 1);
            pvNewDataFill[i] = static_cast<T>(_fRndNum);
        }
        break;
    case INIT_FCN_EYES:
        // 暂未提供
        break;
    case INIT_FCN_NONE:
        // do nothing
        break;
    }
}

template<typename T>
void MatrixT<T>::fill( int _initFcn /*= INIT_FCN_DEFAULT */,
                       value_type _fillVal /*= value_type(0.0)*/ )
{
    fill_data(cols, rows, _initFcn, _fillVal);
}

template<typename T>
inline bool MatrixT<T>::is_empty( void ) const
{
    return (sizes() == 0);
}

//////////////////////////////////////////////////////////////////////
//
// size() 获取矩阵行数或列数或总长度
//
// n = 0或其他, 返回总长度 (默认)
// n = 1,       返回行数
// n = 2,       返回列数
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
// sizes() 获取矩阵总长度(快速)
//
//////////////////////////////////////////////////////////////////////

template<typename T>
inline typename MatrixT<T>::size_type MatrixT<T>::sizes( void ) const
{
    return totals;
}

//////////////////////////////////////////////////////////////////////
//
// is_same_size() 判断矩阵行数或列数是否相同
//
// n = 0或其他, 行数和列数是否相同 (默认)
// n = 1,       行数是否相同
// n = 2,       列数是否相同
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
// is_same_sizes() 判断矩阵行数或列数是否相同(快速版)
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

    int _totals = _rows * _cols;
    if (_totals <= totals_actual) {
        if (_rows > rows) {
            if (ppvColAlloc != NULL)
                delete[] ppvColAlloc;

            const int nAdditionRowSize =
                (int)ceil(double(MAT_CACHE_ALIGN_SIZE) / (double)sizeof(typename pointer));
            ptr_pointer ppvOrigCol = new pointer[_cols + nAdditionRowSize];
            if (ppvOrigCol != NULL) {
                ptr_pointer ppvNewCol = (ptr_pointer)MAT_CACHE_ALIGN_128(ppvOrigCol);
                __ANNLAB_ASSERT(ppvNewCol != NULL);

                ppvCol = ppvNewCol;
                ppvColAlloc = ppvOrigCol;

                // reset the row pointers
                pointer p = pvData;
                for (int i=0; i<_cols; ++i) {
                    ppvCol[i] = p;
                    p += _rows;
                }
            }
            else {
                ppvColAlloc = NULL;
                ppvCol = NULL;
            }
        }
        rows = _rows;
        cols = _cols;
        totals = _totals;
        return _totals;
    }

    destroy();
    init_martix(_rows, _cols);
    return _totals;
}

template<typename T>
inline int MatrixT<T>::resize_ex( int _rows, int _cols,
                                 int _initFcn /*= MAT_INIT_DEFAULT */,
                                 value_type _fillVal /*= 0.0*/ )
{
    if (rows == _rows && cols == _cols) {
        if (_initFcn != INIT_FCN_NONE)
            fill_data(_rows, _cols, _initFcn, _fillVal);
        return totals;
    }

    destroy();
    initialize(_rows, _cols, _fillVal, _initFcn);
    return totals;
}

template<typename T>
inline void MatrixT<T>::copy_data( pointer dest, const_pointer src, int _totals )
{
    switch (totals)
    {
    default:
        copy_big_data(dest, src, _totals);
        break;
    case 8:
        dest[7] = src[7];
    case 7:
        dest[6] = src[6];
    case 6:
        dest[5] = src[5];
    case 5:
        dest[4] = src[4];
    case 4:
        dest[3] = src[3];
    case 3:
        dest[2] = src[2];
    case 2:
        dest[1] = src[1];
    case 1:
        dest[0] = src[0];
    }
}

template<typename T>
inline void MatrixT<T>::copy_big_data( pointer dest, const_pointer src, int _totals )
{
    switch (totals)
    {
    default:
        std::memcpy(dest, src, totals * sizeof(T));
        break;
    case 32:
        dest[31] = src[31];
    case 31:
        dest[30] = src[30];
    case 30:
        dest[29] = src[29];
    case 29:
        dest[28] = src[28];
    case 28:
        dest[27] = src[27];
    case 27:
        dest[26] = src[26];
    case 26:
        dest[25] = src[25];
    case 25:
        dest[24] = src[24];
    case 24:
        dest[23] = src[23];
    case 23:
        dest[22] = src[22];
    case 22:
        dest[21] = src[21];
    case 21:
        dest[20] = src[20];
    case 20:
        dest[19] = src[19];
    case 19:
        dest[18] = src[18];
    case 18:
        dest[17] = src[17];
    case 17:
        dest[16] = src[16];
    case 16:
        dest[15] = src[15];
    case 15:
        dest[14] = src[14];
    case 14:
        dest[13] = src[13];
    case 13:
        dest[12] = src[12];
    case 12:
        dest[11] = src[11];
    case 11:
        dest[10] = src[10];
    case 10:
        dest[9] = src[9];
    case 9:
        dest[8] = src[8];
    case 8:
        dest[7] = src[7];
    case 7:
        dest[6] = src[6];
    case 6:
        dest[5] = src[5];
    case 5:
        dest[4] = src[4];
    case 4:
        dest[3] = src[3];
    case 3:
        dest[2] = src[2];
    case 2:
        dest[1] = src[1];
    case 1:
        dest[0] = src[0];
    }
}

template<typename T>
inline void MatrixT<T>::copy_row( const MatrixT<T>& src, int _row )
{
    const int _cols = src.cols;
    pointer dest = get_data();

    switch (_cols) {
    default:
        int i,j;
        for (i=0, j=1; j<_cols; i+=2, j+=2) {
            dest[i] = src.get_at(_row, i); 
            dest[j] = src.get_at(_row, j);
        }

        if (i < _cols) {
            dest[i] = src.get_at(_row, i); 
        }
        break;
    case 8:
      dest[7] = src.get_at(_row, 7);
    case 7:
      dest[6] = src.get_at(_row, 6);
    case 6:
      dest[5] = src.get_at(_row, 5);
    case 5:
      dest[4] = src.get_at(_row, 4);
    case 4:
      dest[3] = src.get_at(_row, 3);
    case 3:
      dest[2] = src.get_at(_row, 2);
    case 2:
      dest[1] = src.get_at(_row, 1);
    case 1:
      dest[0] = src.get_at(_row, 0);
    }
}

template<typename T>
inline void MatrixT<T>::copy_from_array( const_pointer _array )
{
    __MY_ASSERT((_array != NULL), _T(""));

#if MATRIXT_FAST_MODE
    copy_data(pvData, _array, totals);
#else
    for (int i=0; i<totals; ++i)
        pvData[i] = _array[i];
#endif
}

template<typename T>
inline void MatrixT<T>::set_by_scalar( const_value_type _x )
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
    copy_from_array(src->get_data());
    return NULL;
}

template<typename T>
inline typename MatrixT<T>::value_type& MatrixT<T>::get_at( int _index )
{
    return pvData[_index];
}

template<typename T>
inline typename MatrixT<T>::value_type MatrixT<T>::get_at( int _index ) const
{
    return pvData[_index];
}

template<typename T>
inline typename MatrixT<T>::value_type& MatrixT<T>::get_at( int _row, int _col )
{
    return pvData[_col * rows + _row];
}

template<typename T>
inline typename MatrixT<T>::value_type MatrixT<T>::get_at( int _row, int _col ) const
{
    return pvData[_col * rows + _row];
}

template<typename T>
inline void MatrixT<T>::set_at( int _index, const value_type _value )
{
    pvData[_index] = _value;
}

template<typename T>
inline void MatrixT<T>::set_at( int _row, int _col, const value_type _value )
{
    pvData[_col * rows + _row] = _value;
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
inline typename MatrixT<T>::pointer MatrixT<T>::operator[]( int _row )
{
    return ppvCol[_row];
}

template<typename T>
inline typename MatrixT<T>::const_pointer MatrixT<T>::operator[]( int _row ) const
{
    return ppvCol[_row];
}

template<typename T>
inline typename MatrixT<T>::reference MatrixT<T>::operator()( int _index )
{
    return pvData[_index];
}

template<typename T>
inline typename MatrixT<T>::value_type MatrixT<T>::operator()( int _index ) const
{
    return pvData[_index];
}

template<typename T>
inline typename MatrixT<T>::reference MatrixT<T>::operator()( int _row, int _col )
{
    return pvData[_col * rows + _row];
}

template<typename T>
inline typename MatrixT<T>::value_type MatrixT<T>::operator()( int _row, int _col ) const
{
    return pvData[_col * rows + _row];
}

template<typename T>
inline typename MatrixT<T>::pointer MatrixT<T>::get_rowptr( int _row )
{
    return &(pvData[_row * cols]);
}

template<typename T>
inline typename MatrixT<T>::const_pointer MatrixT<T>::get_rowptr( int _row ) const
{
    return const_pointer(&(pvData[_row * cols]));
}

template<typename T>
inline typename MatrixT<T>::pointer MatrixT<T>::get_colptr( int _col )
{
    return &(pvData[_col * rows]);
}

template<typename T>
inline typename MatrixT<T>::const_pointer MatrixT<T>::get_colptr( int _col ) const
{
    return const_pointer(&(pvData[_col * rows]));
}

template<typename T>
inline MatrixT<T>& MatrixT<T>::operator = ( value_type _value )
{
    // resize to one item matrix
    resize(1, 1);

    // set the single data
    set_at(0, 0, _value);

    // finally return a reference to ourselves
    return *this;
}

template<typename T>
inline MatrixT<T>& MatrixT<T>::operator = ( MatrixT<T>& _Right )
{
    if ((&_Right) == this || (pvData == _Right.get_data() && pvData != NULL))
        return *this;

    if (rows == _Right.rows && cols == _Right.cols) {
        copy_from_array(_Right.get_data());
    }
    else {
        destroy();
        initialize(_Right.rows, _Right.cols);
        copy_from_array(_Right.get_data());

        //MatrixT<T>* dest = copy(&_Right);
        //ASSERT(dest != NULL);
    }

    // finally return a reference to ourselves
    return *this;
}

template<typename T>
inline MatrixT<T> MatrixT<T>::operator+( value_type _value )
{
    // Matrix addition
#if MATRIXT_FAST_MODE
    // Copy the current matrix
    MatrixT<T> _Result((MatrixT<T> &)*this);

    int _length       = _Result.sizes();
    value_type *pData = _Result.get_data();

    __ANNLAB_ASSERT(pData != NULL);
    for (int i=0; i<_length; ++i) {
        (*pData) +=_value;
        pData++;
    }
#else
    // Create the result matrix
    MatrixT<T> _Result(rows, cols);

    for (int i=0; i<rows; ++i) {
        for (int j=0; j<cols; ++j)
            _Result.set_at(i, j, get_at(i, j) + _value);
    }
#endif

    return _Result;
}

template<typename T>
inline MatrixT<T> MatrixT<T>::operator+( MatrixT<T>& _Right )
{
    // Copy the current matrix
    MatrixT<T> _Result((MatrixT<T> &)*this);
#if _DEBUG
    if (_Right.is_empty())
        return _Result;
#endif

    // Check whether it is its own matrix
    __ANNLAB_ASSERT(rows == _Right.rows && cols == _Right.cols);

#if _DEBUG
    if (rows != _Right.rows || cols != _Right.cols) {
        throw _T("Incompatible dimensions in operator + ( MatrixT<T> & _Right ).");
        //exit(1);
        //_Result.copy(this);
        return _Result;
    }
#endif

    // Matrix addition
#if MATRIXT_FAST_MODE
    int _length   = _Result.sizes();
    int _lenRight = _Right.sizes();
    value_type *pData      = _Result.get_data();
    value_type *pDataRight = _Right.get_data();

    __ANNLAB_ASSERT(pData != NULL && pDataRight != NULL && _length == _lenRight);
    //if (pData != NULL && pDataRight != NULL && _length == _lenRight) {
#if 0
        for (int i=0; i<_length; ++i) {
            (*pData++) += (value_type)(*pDataRight++);
        }
#else
        value_type *pData2      = pData + 1;
        value_type *pDataRight2 = pDataRight + 1;
        for (int i=0; i<_length/2; ++i) {
            (*pData)  += (value_type)(*pDataRight);
            (*pData2) += (value_type)(*pDataRight2);
            pData += 2;
            pData2 += 2;
            pDataRight += 2;
            pDataRight2 += 2;
        }
        if (_length & 1)
            (*pData) += (value_type)(*pDataRight);
#endif
    //}
#else
    for (int i=0; i<rows; ++i) {
        for (int j=0; j<cols; ++j)
            _Result.set_at(i, j, get_at(i, j) + _Right.get_at(i, j));
    }
#endif

    return _Result;
}

template<typename T>
inline MatrixT<T>& MatrixT<T>::operator+=( value_type _value )
{
    // Matrix addition
#if MATRIXT_FAST_MODE
    int _length       = sizes();
    value_type *pData = get_data();

    __ANNLAB_ASSERT(pData != NULL);
    if (pData != NULL) {
        for (int i=0; i<_length; i++) {
            (*pData) += _value;
            pData++;
        }
    }
#else
    for (int i=0; i<rows; ++i) {
        for (int j=0; j<cols; ++j)
            set_at(i, j, get_at(i, j) + _value);
    }
#endif

    return *this;
}

template<typename T>
inline MatrixT<T>& MatrixT<T>::operator+=( MatrixT<T>& _Right )
{
#if _DEBUG
    if (_Right.is_empty())
        return *this;
#endif

    // Check whether it is its own matrix
    __ANNLAB_ASSERT(rows == _Right.rows && cols == _Right.cols);

#if _DEBUG
    if (rows != _Right.rows || cols != _Right.cols) {
        throw _T("Incompatible dimensions in operator += ( MatrixT<T> & _Right ). ");
        //exit(1);
        return *this;
    }
#endif

    // Matrix addition
#if MATRIXT_FAST_MODE
    int _length   = sizes();
    int _lenRight = _Right.sizes();
    value_type *pData      = get_data();
    value_type *pDataRight = _Right.get_data();

    __ANNLAB_ASSERT(pData != NULL && pDataRight != NULL && _length == _lenRight);
    //if (pData != NULL && pDataRight != NULL && _length == _lenRight) {
        for (int i=0; i<_length; i++) {
            (*pData++) += (value_type)(*pDataRight++);
        }
    //}
#else
    for (int i=0; i<rows; ++i) {
        for (int j=0; j<cols; ++j)
            set_at(i, j, get_at(i, j) + _Right.get_at(i, j));
    }
#endif

    return *this;
}

template<typename T>
inline MatrixT<T> MatrixT<T>::operator*( value_type _value )
{
    // Copy the current matrix
    MatrixT<T> _Result((MatrixT<T> &)_Right);

    int _length       = _Result.sizes();
    value_type *pData = _Result.get_data();

    __ANNLAB_ASSERT(pData != NULL);
    for (int i=0; i<_length; ++i) {
        (*pData) *= _value;
        pData++;
    }
}

template<typename T>
inline MatrixT<T> MatrixT<T>::operator*( MatrixT<T>& _Right )
{
    // 首先检查乘矩阵的行数和被乘矩阵的列数是否相同
    __ANNLAB_ASSERT(cols == _Right.rows);

    int _newRows, _newCols, _oldCols;
    _newRows = rows;
    _newCols = _Right.cols;
    _oldCols = cols;

    // 创建目标乘积矩阵
    MatrixT<T> _Result(_newRows, _newCols);

    // Matrix multiplication，即
    //
    // [A][B][C]   [G][H]     [A*G + B*I + C*K][A*H + B*J + C*L]
    // [D][E][F] * [I][J]  =  [D*G + E*I + F*K][D*H + E*J + F*L]
    //             [K][L]
    //
#if MATRIXT_FAST_MODE
  #if 1
    value_type _value1, _value2;
    MatrixT<T> tmp_row(1, _oldCols);
    pointer tmp_rowdata;
    pointer right_colptr;

    for (int _row=0; _row<_newRows; ++_row) {
        tmp_row.copy_row(*this, _row);
        for (int _col=0; _col<_newCols; ++_col) {
            _value1 = value_type(0.0);
            _value2 = value_type(0.0);
            tmp_rowdata  = tmp_row.get_data();
            right_colptr = _Right.get_colptr(_col);
            int i, j;
            for (i=0, j=1; j<_oldCols; i+=2, j+=2) {
                //_value1 += get_at(i, k) * _Right.get_at(k, j);
                _value1 = tmp_rowdata[i] * right_colptr[i];
                _value2 = tmp_rowdata[j] * right_colptr[j];
            }
            if (i < _oldCols)
                _value1 = tmp_rowdata[i] * right_colptr[i];
            _Result.set_at(_row, _col, _value1 + _value2);
        }
    }
  #else
    value_type _value1, _value2;
    pointer right_colptr;

    for (int _row=0; _row<_newRows; ++_row) {
        for (int _col=0; _col<_newCols; ++_col) {
            _value1 = value_type(0.0);
            _value2 = value_type(0.0);
            right_colptr = _Right.get_colptr(_col);
            int i, j;
            for (i=0, j=1; j<_oldCols; i+=2, j+=2) {
                //_value1 += get_at(i, k) * _Right.get_at(k, j);
                _value1 = get_at(_row, i) * right_colptr[i];
                _value2 = get_at(_row, j) * right_colptr[j];
            }
            if (i < _oldCols)
                _value1 = get_at(_row, i) * right_colptr[i];
            _Result.set_at(_row, _col, _value1 + _value2);
        }
    }
  #endif
#else
    value_type _value;
    for (int i=0; i<_newRows; ++i) {
        for (int j=0; j<_newCols; ++j) {
            _value = value_type(0.0);
            for (int k=0; k<_oldCols; ++k)
                _value += get_at(i, k) * _Right.get_at(k, j);
            _Result.set_at(i, j, _value);
        }
    }
#endif  // !MATRIXT_FAST_MODE

    return _Result;
}

template<typename T>
inline MatrixT<T>& MatrixT<T>::operator*=( value_type _value )
{
    return *this;
    // Matrix multiplication
    for (int i=0; i<rows; ++i) {
        for (int j=0; j<cols; ++j)
            set_at(i, j, get_at(i, j) * _value) ;
    }

    return *this;
}

template<typename T>
inline MatrixT<T>& MatrixT<T>::operator*=( MatrixT<T>& _Right )
{
    // 首先检查乘矩阵的行数和被乘矩阵的列数是否相同
    __ANNLAB_ASSERT(cols == _Right.rows);

    // Copy the current matrix
    MatrixT<T> _Left((MatrixT<T> &)*this);

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
    value_type _value;
    for (int i=0; i<_newRows; ++i) {
        for (int j=0; j<_newCols; ++j) {
            _value = value_type(0.0);
            for (int k=0; k<_oldCols; ++k)
                _value += _Left.get_at(i, k) * _Right.get_at(k, j);
            set_at(i, j, _value);
        }
    }

    return *this;
}

template<typename T>
inline MatrixT<T>& MatrixT<T>::transpose( void )
{
    // 单行或单列的矩阵, 即Vector
    if (cols == 1 || rows == 1) {
        int temp = rows;
        rows = cols;
        cols = temp;
        return *this;
    }

    // 对称矩阵(方块矩阵)
    if (cols == rows) {
        // 转置各元素
        for (int i=0; i<rows; ++i) {
            for (int j=i+1; j<cols; ++j)
                set_at(j, i, get_at(i, j));
        }
    }
    else {
        MatrixT<T> _trans((MatrixT<T> &)*this);
        // 转置各元素
        for (int i=0; i<rows; ++i) {
            for (int j=0; j<cols; ++j)
                set_at(j, i, _trans.get_at(i, j));
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
    // 重置大小并初始化为全0矩阵
    return resize(_rows, _cols, INIT_FCN_ZEROS);
}

template<typename T>
inline int MatrixT<T>::ones( int _rows, int _cols )
{
    // 重置大小并初始化为全1矩阵
    return resize(_rows, _cols, INIT_FCN_ONES);
}

template<typename T>
inline int MatrixT<T>::rands( int _rows, int _cols )
{
    // 重置大小并初始化为[-1,1]随机数矩阵
    return resize(_rows, _cols, INIT_FCN_RANDS);
}

template<typename T>
inline int MatrixT<T>::rands2( int _rows, int _cols )
{
    // 重置大小并初始化为[0,1]随机数矩阵
    return resize(_rows, _cols, INIT_FCN_RANDS_POSITIVE);
}

template<typename T>
inline MatrixT<T> MatrixT<T>::_zeros( int _rows, int _cols )
{
    // Copy the current matrix
    MatrixT<T> _zeros(_rows, _cols);

    // 所有元素置0
    for (int i=0; i<_rows; ++i) {
        for (int j=0; j<_cols; ++j)
            _zeros.set_at(i, j, value_type(0.0));
    }

    return _zeros;
}

template<typename T>
inline MatrixT<T> MatrixT<T>::_ones( int _rows, int _cols )
{
    // Copy the current matrix
    MatrixT<T> _ones(_rows, _cols);

    // 所有元素置1
    for (int i=0; i<_rows; ++i) {
        for (int j=0; j<_cols; ++j)
            _ones.set_at(i, j, value_type(1.0));
    }

    return _ones;
}

template<typename T>
inline MatrixT<T> MatrixT<T>::_rands( int _rows, int _cols )
{
    // Copy the current matrix
    MatrixT<T> _rands(_rows, _cols);

    // 所有元素置[-1,1]的随机数
    value_type _fRndNum;
    for (int i=0; i<_rows; ++i) {
        for (int j=0; j<_cols; ++j) {
            _fRndNum = value_type(2.0) * value_type(rand()) / value_type(RAND_MAX)
                - value_type(1.0);
            _rands.set_at(i, j, _fRndNum);
        }
    }

    return _rands;
}

template<typename T>
inline MatrixT<T> MatrixT<T>::_rands2( int _rows, int _cols )
{
    // Copy the current matrix
    CAnnMatrix _rands(_rows, _cols);

    // 所有元素置[0,1]的随机数
    value_type _fRndNum;
    for (int i=0; i<_rows; ++i) {
        for (int j=0; j<_cols; ++j) {
            _fRndNum = = value_type(rand()) / value_type(RAND_MAX);
            _rands.set_at(i, j, _fRndNum);
        }
    }

    return _rands;
}

template<typename T>
inline void MatrixT<T>::fill_data( int _rows, int _cols,
                           int _initFcn /*= MAT_INIT_DEFAULT */,
                           value_type _fillVal /*= 0.0*/ )
{
    ASSERT(pvData != NULL);
    ASSERT(_rows <= rows && _rows >= 0 && _cols <= cols && _cols >= 0);

    // init matrix data
    int _totals = _rows * _cols;
    ASSERT(_totals >= 0);
    if (_totals <= 0)
        return;

    switch (_initFcn) {
    case INIT_FCN_ZEROS:           // all zeros
        for (int i=0; i<_totals; ++i)
            pvData[i] = value_type(0.0);
        break;
    case INIT_FCN_ONES:            // all ones
        for (int i=0; i<_totals; ++i)
            pvData[i] = value_type(1.0);
        break;
    case INIT_FCN_EYES:
        // 暂未提供
        break;
    case INIT_FCN_RANDS:           // all [-1,1] randomize
        for (int i=0; i<_totals; ++i)
            pvData[i] = value_type(value_type(2.0) * value_type(rand()) / value_type(RAND_MAX) - value_type(1.0));
        break;
    case INIT_FCN_RANDS_POSITIVE:  // all [0,1] positive randomize
        for (int i=0; i<_totals; ++i)
            pvData[i] = value_type(rand()) / value_type(RAND_MAX);
        break;
    case INIT_FCN_SPECIFIED:
        for (int i=0; i<_totals; ++i)
            pvData[i] = _fillVal;
        break;
    case INIT_FCN_NONE:
        // do nothing
        break;
    }
}

#define MATRIXT_INIT_FCN_FUNC_SIGNED(Ty) \
template<> \
inline void MatrixT<Ty>::fill_data( int _rows, int _cols, \
                            int _initFcn /*= MAT_INIT_DEFAULT */, \
                            value_type _fillVal /*= 0.0*/ ) \
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
    case INIT_FCN_ZEROS:           /* all zeros */ \
        for (int i=0; i<_totals; ++i) \
            pvData[i] = value_type(0.0); \
        break; \
    case INIT_FCN_ONES:            /* all ones */ \
        for (int i=0; i<_totals; ++i) \
            pvData[i] = value_type(1.0); \
        break; \
    case INIT_FCN_EYES: \
        /* 暂未提供 */ \
        break; \
    case INIT_FCN_RANDS:           /* all [-1,1] randomize */ \
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
    case INIT_FCN_RANDS_POSITIVE:  /* all [0,1] positive randomize */ \
        for (int i=0; i<_totals; ++i) \
            pvData[i] = (value_type)(rand() & 1); \
        break; \
    case INIT_FCN_SPECIFIED: \
        for (int i=0; i<_totals; ++i) \
            pvData[i] = _fillVal; \
        break; \
    case INIT_FCN_NONE: \
        /* do nothing */ \
        break; \
    } \
}

#define MATRIXT_INIT_FCN_FUNC_UNSIGNED(Ty) \
template<> \
inline void MatrixT<Ty>::fill_data( int _rows, int _cols, \
                            int _initFcn /*= MAT_INIT_DEFAULT */, \
                            value_type _fillVal /*= 0.0*/ ) \
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
    case INIT_FCN_ZEROS:           /* all zeros */ \
        for (int i=0; i<_totals; ++i) \
            pvData[i] = value_type(0.0); \
        break; \
    case INIT_FCN_ONES:            /* all ones */ \
        for (int i=0; i<_totals; ++i) \
            pvData[i] = value_type(1.0); \
        break; \
    case INIT_FCN_EYES: \
        /* 暂未提供 */ \
        break; \
    case INIT_FCN_RANDS:           /* all [0,1] randomize */ \
        /* unsigned integer, not allowed -1 */ \
        for (int i=0; i<_totals; ++i) \
            pvData[i] = (value_type)(rand() & 1); \
        break; \
    case INIT_FCN_RANDS_POSITIVE:  /* all [0,1] positive randomize */ \
        for (int i=0; i<_totals; ++i) \
            pvData[i] = (value_type)(rand() & 1); \
        break; \
    case INIT_FCN_SPECIFIED: \
        for (int i=0; i<_totals; ++i) \
            pvData[i] = _fillVal; \
        break; \
    case INIT_FCN_NONE: \
        /* do nothing */ \
        break; \
    } \
}

MATRIXT_INIT_FCN_FUNC_SIGNED(int64_t)
MATRIXT_INIT_FCN_FUNC_SIGNED(int32_t)
MATRIXT_INIT_FCN_FUNC_SIGNED(int16_t)
MATRIXT_INIT_FCN_FUNC_SIGNED(int8_t)
MATRIXT_INIT_FCN_FUNC_SIGNED(signed char)

MATRIXT_INIT_FCN_FUNC_UNSIGNED(uint64_t)
MATRIXT_INIT_FCN_FUNC_UNSIGNED(uint32_t)
MATRIXT_INIT_FCN_FUNC_UNSIGNED(uint16_t)
MATRIXT_INIT_FCN_FUNC_UNSIGNED(uint8_t)
MATRIXT_INIT_FCN_FUNC_UNSIGNED(bool)

/*
 * AnsiToUnicode converts the ANSI string pszA to a Unicode string
 * and returns the Unicode string through ppszW. Space for the
 * the converted string is allocated by AnsiToUnicode.
 */

 HRESULT __fastcall AnsiToUnicode(LPCSTR pszA, LPOLESTR* ppszW)
{

    ULONG cCharacters;
    DWORD dwError;

    // If input is null then just return the same.
    if (NULL == pszA)
    {
        *ppszW = NULL;
        return NOERROR;
    }

    // Determine number of wide characters to be allocated for the
    // Unicode string.
    cCharacters =  strlen(pszA)+1;

    // Use of the OLE allocator is required if the resultant Unicode
    // string will be passed to another COM component and if that
    // component will free it. Otherwise you can use your own allocator.
    *ppszW = (LPOLESTR) CoTaskMemAlloc(cCharacters*2);
    if (NULL == *ppszW)
        return E_OUTOFMEMORY;

    // Covert to Unicode.
    if (0 == MultiByteToWideChar(CP_ACP, 0, pszA, cCharacters,
        *ppszW, cCharacters))
    {
        dwError = GetLastError();
        CoTaskMemFree(*ppszW);
        *ppszW = NULL;
        return HRESULT_FROM_WIN32(dwError);
    }

    return NOERROR;
}
/*
 * UnicodeToAnsi converts the Unicode string pszW to an ANSI string
 * and returns the ANSI string through ppszA. Space for the
 * the converted string is allocated by UnicodeToAnsi.
 */

HRESULT __fastcall UnicodeToAnsi(LPCOLESTR pszW, LPSTR* ppszA)
{

    ULONG cbAnsi, cCharacters;
    DWORD dwError;

    // If input is null then just return the same.
    if (pszW == NULL)
    {
        *ppszA = NULL;
        return NOERROR;
    }

    cCharacters = wcslen(pszW)+1;
    // Determine number of bytes to be allocated for ANSI string. An
    // ANSI string can have at most 2 bytes per character (for Double
    // Byte Character Strings.)
    cbAnsi = cCharacters*2;

    // Use of the OLE allocator is not required because the resultant
    // ANSI  string will never be passed to another COM component. You
    // can use your own allocator.
    *ppszA = (LPSTR) CoTaskMemAlloc(cbAnsi);
    if (NULL == *ppszA)
        return E_OUTOFMEMORY;

    // Convert to ANSI.
    if (0 == WideCharToMultiByte(CP_ACP, 0, pszW, cCharacters, *ppszA,
        cbAnsi, NULL, NULL))
    {
        dwError = GetLastError();
        CoTaskMemFree(*ppszA);
        *ppszA = NULL;
        return HRESULT_FROM_WIN32(dwError);
    }
    return NOERROR;
}

template<typename T>
void MatrixT<T>::display( void )
{
    display(name());
}

template<typename T>
void MatrixT<T>::display( const TCHAR *szText )
{
#if defined(ANNLAB_USE_DISPLAY) && (ANNLAB_USE_DISPLAY)
    TCHAR* szTypeName = NULL;
    AnsiToUnicode(typeid(T).name(), &szTypeName);
    TRACE(("MatrixT<%s>: Name = [ %s ], [rows = %d, cols = %d]\n"), szTypeName, szText, rows, cols);
    TRACE(_T("============================================================================================================\n\n"));
    for (int r=0; r<rows; r++) {
        TRACE(_T("\t"));
        for (int c=0; c<cols; c++) {
            value_type value = get_at(r, c);
            if (abs(value) < static_cast<T>(10000000.0))
                TRACE(_T("%12.4f "), value);
            else
                TRACE(_T("%12.4E "), value);
        }
        TRACE(_T("\n\n"));
    }
    TRACE(_T("============================================================================================================\n\n"));
    if (szTypeName != NULL)
        CoTaskMemFree(szTypeName);
#endif
}

#define MATRIXT_DISPLAY_FUNC_SIGNED(Ty) \
template<> \
void MatrixT<Ty>::display( const TCHAR *szText ) \
{ \
    TRACE(_T("MatrixT<")_T(#Ty)_T(">: Name = [ %s ], [rows = %d, cols = %d]\n"), szText, rows, cols); \
    TRACE(_T("============================================================================================================\n\n")); \
    for (int r=0; r<rows; r++) { \
        TRACE(_T("\t")); \
        for (int c=0; c<cols; c++) { \
            TRACE(_T("%10d  "), get_at(r, c)); \
        } \
        TRACE(_T("\n\n")); \
    } \
    TRACE(_T("============================================================================================================\n\n")); \
}

#define MATRIXT_DISPLAY_FUNC_UNSIGNED(Ty) \
template<> \
void MatrixT<Ty>::display( const TCHAR *szText ) \
{ \
    TRACE(_T("MatrixT<")_T(#Ty)_T(">: Name = [ %s ], [rows = %d, cols = %d]\n"), szText, rows, cols); \
    TRACE(_T("============================================================================================================\n\n")); \
    for (int r=0; r<rows; r++) { \
        TRACE(_T("\t")); \
        for (int c=0; c<cols; c++) { \
            TRACE(_T("%10u "), get_at(r, c)); \
        } \
        TRACE(_T("\n\n")); \
    } \
    TRACE(_T("============================================================================================================\n\n")); \
}

#if defined(ANNLAB_USE_DISPLAY) && (ANNLAB_USE_DISPLAY)
MATRIXT_DISPLAY_FUNC_SIGNED(int64_t)
MATRIXT_DISPLAY_FUNC_SIGNED(int32_t)
MATRIXT_DISPLAY_FUNC_SIGNED(int16_t)
MATRIXT_DISPLAY_FUNC_SIGNED(int8_t)
MATRIXT_DISPLAY_FUNC_SIGNED(signed char)

MATRIXT_DISPLAY_FUNC_UNSIGNED(uint64_t)
MATRIXT_DISPLAY_FUNC_UNSIGNED(uint32_t)
MATRIXT_DISPLAY_FUNC_UNSIGNED(uint16_t)
MATRIXT_DISPLAY_FUNC_UNSIGNED(uint8_t)
MATRIXT_DISPLAY_FUNC_UNSIGNED(bool)
#endif

template<typename T>
void MatrixT<T>::display_ex( void )
{
    display_ex(name());
}

template<typename T>
void MatrixT<T>::display_ex( const TCHAR *szText )
{
#if defined(ANNLAB_USE_DISPLAY_EX) && (ANNLAB_USE_DISPLAY_EX)
    char*  szTypeNameA = NULL;
    TCHAR* szTypeName = NULL;
#ifdef _UNICODE
    szTypeNameA = (char* )typeid(T).name();
    AnsiToUnicode(szTypeNameA, &szTypeName);
#else
    szTypeNameA = (char* )typeid(T).name();
    szTypeName = szTypeNameA;
#endif
    TRACE(_T("MatrixT<%s>: Name = [ %s ], [rows = %d, cols = %d]\n"), szTypeName, szText, rows, cols);
    TRACE(_T("============================================================================================================\n\n"));
    for (int r=0; r<rows; r++) {
        TRACE(_T("\t"));
        for (int c=0; c<cols; c++) {
            value_type value = get_at(r, c);
            if (abs(value) < static_cast<T>(10000000.0))
                TRACE(_T("%12.4f "), value);
            else
                TRACE(_T("%12.4E "), value);
        }
        TRACE(_T("\n\n"));
    }
    TRACE(_T("============================================================================================================\n\n"));
#ifdef _UNICODE
    if (szTypeName != NULL) {
        CoTaskMemFree(szTypeName);
        szTypeName = NULL;
    }
#endif
    if (szTypeNameA) {
        const int nHeaderSize = 0x48;
        bool bFound = false;
        uint32_t* szHeader = (uint32_t *)(szTypeNameA - nHeaderSize);
        if (szHeader != NULL && (*szHeader) == (uint32_t)szTypeNameA)
            bFound = true;
        ::free(szTypeNameA);
        if (bFound)
            ::free(szHeader);
    }
#endif
}

#define MATRIXT_DISPLAY_EX_FUNC_SIGNED(Ty) \
template<> \
void MatrixT<Ty>::display_ex( const TCHAR *szText ) \
{ \
    TRACE(_T("MatrixT<")_T(#Ty)_T(">: Name = [ %s ], [rows = %d, cols = %d]\n"), szText, rows, cols); \
    TRACE(_T("============================================================================================================\n\n")); \
    for (int r=0; r<rows; r++) { \
        TRACE(_T("\t")); \
        for (int c=0; c<cols; c++) { \
            TRACE(_T("%10d "), get_at(r, c)); \
        } \
        TRACE(_T("\n\n")); \
    } \
    TRACE(_T("============================================================================================================\n\n")); \
}

#define MATRIXT_DISPLAY_EX_FUNC_UNSIGNED(Ty) \
template<> \
void MatrixT<Ty>::display_ex( const TCHAR *szText ) \
{ \
    TRACE(_T("MatrixT<")_T(#Ty)_T(">: Name = [ %s ], [rows = %d, cols = %d]\n"), szText, rows, cols); \
    TRACE(_T("============================================================================================================\n\n")); \
    for (int r=0; r<rows; r++) { \
        TRACE(_T("\t")); \
        for (int c=0; c<cols; c++) { \
            TRACE(_T("%10u "), get_at(r, c)); \
        } \
        TRACE(_T("\n\n")); \
    } \
    TRACE(_T("============================================================================================================\n\n")); \
}

#if defined(ANNLAB_USE_DISPLAY_EX) && (ANNLAB_USE_DISPLAY_EX)
MATRIXT_DISPLAY_EX_FUNC_SIGNED(int64_t)
MATRIXT_DISPLAY_EX_FUNC_SIGNED(int32_t)
MATRIXT_DISPLAY_EX_FUNC_SIGNED(int16_t)
MATRIXT_DISPLAY_EX_FUNC_SIGNED(int8_t)
MATRIXT_DISPLAY_EX_FUNC_SIGNED(signed char)

MATRIXT_DISPLAY_EX_FUNC_UNSIGNED(uint64_t)
MATRIXT_DISPLAY_EX_FUNC_UNSIGNED(uint32_t)
MATRIXT_DISPLAY_EX_FUNC_UNSIGNED(uint16_t)
MATRIXT_DISPLAY_EX_FUNC_UNSIGNED(uint8_t)
MATRIXT_DISPLAY_EX_FUNC_UNSIGNED(bool)
#endif


#ifndef _MATRIXT_IMPL_internal_H_
}  // namespace annlab
#endif

#endif  // _MATRIXT_IMPL_H_
