
#if !defined(_FASTMATRIX_IMPL_internal_H_)
#error Do not #include this internal file directly; use fastmatrix.h header file instead.
#endif

#ifndef _FASTMATRIX_IMPL_H_
#define _FASTMATRIX_IMPL_H_

#ifndef _FASTMATRIX_IMPL_internal_H_
namespace annlab {
#endif

template<typename T>
FastMatrix<T>::FastMatrix(void) :
    pvData(NULL), ppvCol(NULL), pvAlloc(NULL), ppvColAlloc(NULL),
    rows(0), cols(0), totals(0), totals_actual(0), alloc_size(0)
{
}

template<typename T>
FastMatrix<T>::FastMatrix(int _size)
{
    init(_size, _size);
}

template<typename T>
FastMatrix<T>::FastMatrix(int _rows, int _cols)
{
    init(_rows, _cols);
}

template<typename T>
FastMatrix<T>::FastMatrix(int _rows, int _cols, const value_type& _x)
{
    init(_rows, _cols);
    set_by_scalar(_x);
}

template<typename T>
FastMatrix<T>::FastMatrix(int _rows, int _cols, const value_type* _array)
{
    init(_rows, _cols);
    copy_from_array(_array);
}

template<typename T>
FastMatrix<T>::FastMatrix(int _rows, int _cols, int _initFcn,
                    value_type _fillVal /*= value_type(0.0)*/)
{
    init(_rows, _cols, _initFcn, value_type(0.0));
}

template<typename T>
FastMatrix<T>::FastMatrix(const TCHAR *szName, int _rows, int _cols,
                    int _initFcn /*= INIT_FCN_DEFAULT */,
                    value_type _fillVal /*= value_type(0.0)*/)
{
    init_ex(szName, _rows, _cols, _initFcn, _fillVal);
}

template<typename T>
FastMatrix<T>::FastMatrix(const FastMatrix<T>& src)
{
    init_ex(NULL, src.rows, src.cols);
    copy_from_array(src.get_data());

    //MatrixT<T>* pMatrix = copy(&src);
    //ASSERT(pMatrix != NULL);
}

template<typename T>
FastMatrix<T>::FastMatrix(const FastMatrix<T>& src, bool b_copy_data)
{
    init_ex(NULL, src.rows, src.cols);

    if (b_copy_data) {
        copy_from_array(src.get_data());
        //MatrixT<T>* pMatrix = copy(&src);
        //ASSERT(pMatrix != NULL);
    }
}

template<typename T>
FastMatrix<T>::~FastMatrix(void)
{
    destroy_data();
}

template<typename T>
inline void FastMatrix<T>::destroy_data(void)
{
    if (pvAlloc != NULL) {
        delete[] pvAlloc;
        if (ppvColAlloc != NULL)
            delete[] ppvColAlloc;
    }
}

template<typename T>
inline void FastMatrix<T>::destroy(void)
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
inline void FastMatrix<T>::free(void)
{
    destroy();
}

template<typename T>
inline void FastMatrix<T>::init(int _rows, int _cols,
                                int _initFcn /*= INIT_FCN_DEFAULT*/,
                                value_type _fillVal /*= 0.0*/)
{
    init_ex(NULL, _rows, _cols, _initFcn, _fillVal);
}

template<typename T>
void FastMatrix<T>::init_ex(const TCHAR *szName, int _rows, int _cols,
                            int _initFcn /*= INIT_FCN_DEFAULT*/,
                            value_type _fillVal /*= 0.0*/)
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
void FastMatrix<T>::init_martix(int _rows, int _cols,
                             int _initMode /*= INIT_TYPE_NONE*/,
                             int _initFcn /*= MAT_INIT_DEFAULT */,
                             value_type _fillVal /*= value_type(0.0)*/)
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
void FastMatrix<T>::reserve_and_fill_data(pointer pvNewData, int _rows, int _cols,
                                       int _initFcn /*= MAT_INIT_DEFAULT */,
                                       value_type _fillVal /*= 0.0*/)
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
void FastMatrix<T>::fill(int _initFcn /*= INIT_FCN_DEFAULT */,
                         value_type _fillVal /*= value_type(0.0)*/)
{
    fill_data(cols, rows, _initFcn, _fillVal);
}

template<typename T>
inline int FastMatrix<T>::refill(int _initFcn, value_type _fillVal /*= 0.0*/)
{
    int _totals = rows * cols;
    fill_data(rows, cols, _initFcn, _fillVal);
    return _totals;
}

template<typename T>
inline bool FastMatrix<T>::is_empty(void) const
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
inline typename FastMatrix<T>::size_type FastMatrix<T>::size(int n /*= 0 */) const
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
inline typename FastMatrix<T>::size_type FastMatrix<T>::sizes(void) const
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
inline bool FastMatrix<T>::is_same_size(const FastMatrix<T>* target, int n /*= 0 */)
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
inline bool FastMatrix<T>::is_same_sizes(const FastMatrix<T>& target)
{
    return (rows == target.rows && cols == target.cols);
}

template<typename T>
inline int FastMatrix<T>::resize(int _rows, int _cols)
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
inline int FastMatrix<T>::resize_ex(int _rows, int _cols,
                                    int _initFcn /*= MAT_INIT_DEFAULT */,
                                    value_type _fillVal /*= 0.0*/)
{
    if (rows == _rows && cols == _cols) {
        if (_initFcn != INIT_FCN_NONE)
            fill_data(_rows, _cols, _initFcn, _fillVal);
        return totals;
    }

    destroy();
    init(_rows, _cols, _fillVal, _initFcn);
    return totals;
}

template<typename T>
inline void FastMatrix<T>::copy_data(pointer dest, const_pointer src, int _totals)
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
inline void FastMatrix<T>::copy_big_data(pointer dest, const_pointer src, int _totals)
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
inline void FastMatrix<T>::copy_row(const FastMatrix<T>& src, int _row)
{
    const int _cols = src.cols;
    pointer dest = get_data();

    switch (_cols) {
    default:
#if 1
        {
            pointer src_ptr = src.get_data();
            const int _rows = src.rows;
            for (int i=0; i<_cols; ++i) {
                //dest[i] = src.get_at(_row, i);
                (*dest++) = *src_ptr;
                src_ptr += _rows;
            }
        }
        break;
#else
        int i,j;
        for (i=0, j=1; j<_cols; i+=2, j+=2) {
            dest[i] = src.get_at(_row, i); 
            dest[j] = src.get_at(_row, j);
        }

        if (i < _cols) {
            dest[i] = src.get_at(_row, i); 
        }
        break;
#endif
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
inline void FastMatrix<T>::copy_row(const FastMatrix<T>& src, int _row, int r_step, int _start_col, int c_step)
{
    const int _cols = src.cols;
    pointer dest = get_data();

    switch (_cols) {
    default:
#if 1
        {
            pointer src_ptr = src.get_data();
            const int _rows = src.rows;
            for (int i=0; i<_cols; ++i) {
                //dest[i] = src.get_at(_row, i);
                (*dest++) = *src_ptr;
                src_ptr += _rows;
            }
        }
        break;
#else
        int i,j;
        for (i=0, j=1; j<_cols; i+=2, j+=2) {
            dest[i] = src.get_at(_row, i); 
            dest[j] = src.get_at(_row, j);
        }

        if (i < _cols) {
            dest[i] = src.get_at(_row, i); 
        }
        break;
#endif
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
inline void FastMatrix<T>::copy_from_array(const_pointer _array)
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
inline void FastMatrix<T>::set_by_scalar(const_value_type _x)
{
    __MY_ASSERT((_array != NULL), _T(""));

    for (int i=0; i<totals; ++i)
        pvData[i] = _x;
}

template<typename T>
inline typename FastMatrix<T>::size_type
FastMatrix<T>::copy_from_array_s(const_pointer _array, size_type _size)
{
    __MY_ASSERT((_array != NULL), _T(""));

    size_type copy_size = (_size <= totals) ? _size : totals;
    for (int i=0; i<(int)copy_size; ++i)
        pvData[i] = _array[i];
    return copy_size;
}

template<typename T>
inline FastMatrix<T>* FastMatrix<T>::clone(const FastMatrix<T>* src)
{
    return NULL;
}

template<typename T>
inline FastMatrix<T>* FastMatrix<T>::copy(const FastMatrix<T>* src)
{
    resize(src->rows, src->cols);
    copy_from_array(src->get_data());
    return NULL;
}

template<typename T>
inline typename FastMatrix<T>::value_type& FastMatrix<T>::get_at(int _index)
{
    return pvData[_index];
}

template<typename T>
inline typename FastMatrix<T>::value_type FastMatrix<T>::get_at(int _index) const
{
    return pvData[_index];
}

template<typename T>
inline typename FastMatrix<T>::value_type& FastMatrix<T>::get_at(int _row, int _col)
{
    return pvData[_col * rows + _row];
}

template<typename T>
inline typename FastMatrix<T>::value_type FastMatrix<T>::get_at(int _row, int _col) const
{
    return pvData[_col * rows + _row];
}

template<typename T>
inline void FastMatrix<T>::set_at(int _index, const value_type _value)
{
    pvData[_index] = _value;
}

template<typename T>
inline void FastMatrix<T>::set_at(int _row, int _col, const value_type _value)
{
    pvData[_col * rows + _row] = _value;
}

template<typename T>
inline FastMatrix<T>::operator T*()
{
    return pvData;
}

template<typename T>
inline FastMatrix<T>::operator const T*() const
{
    return pvData;
}

template<typename T>
inline typename FastMatrix<T>::pointer FastMatrix<T>::operator[](int _row)
{
    return ppvCol[_row];
}

template<typename T>
inline typename FastMatrix<T>::const_pointer FastMatrix<T>::operator[](int _row) const
{
    return ppvCol[_row];
}

template<typename T>
inline typename FastMatrix<T>::reference FastMatrix<T>::operator()(int _index)
{
    return pvData[_index];
}

template<typename T>
inline typename FastMatrix<T>::value_type FastMatrix<T>::operator()(int _index) const
{
    return pvData[_index];
}

template<typename T>
inline typename FastMatrix<T>::reference FastMatrix<T>::operator()(int _row, int _col)
{
    return pvData[_col * rows + _row];
}

template<typename T>
inline typename FastMatrix<T>::value_type FastMatrix<T>::operator()(int _row, int _col) const
{
    return pvData[_col * rows + _row];
}

template<typename T>
inline typename FastMatrix<T>::pointer FastMatrix<T>::get_rowptr(int _row)
{
    return &(pvData[_row * cols]);
}

template<typename T>
inline typename FastMatrix<T>::const_pointer FastMatrix<T>::get_rowptr(int _row) const
{
    return const_pointer(&(pvData[_row * cols]));
}

template<typename T>
inline typename FastMatrix<T>::pointer FastMatrix<T>::get_colptr(int _col)
{
    return &(pvData[_col * rows]);
}

template<typename T>
inline typename FastMatrix<T>::const_pointer FastMatrix<T>::get_colptr(int _col) const
{
    return const_pointer(&(pvData[_col * rows]));
}

template<typename T>
inline FastMatrix<T>& FastMatrix<T>::operator = (value_type _value)
{
    // resize to one item matrix
    resize(1, 1);

    // set the single data
    set_at(0, 0, _value);

    // finally return a reference to ourselves
    return *this;
}

template<typename T>
inline FastMatrix<T>& FastMatrix<T>::operator = (FastMatrix<T>& _Right)
{
    if ((&_Right) == this || (pvData == _Right.get_data() && pvData != NULL))
        return *this;

    if (rows == _Right.rows && cols == _Right.cols) {
        copy_from_array(_Right.get_data());
    }
    else {
        destroy();
        init(_Right.rows, _Right.cols);
        copy_from_array(_Right.get_data());

        //MatrixT<T>* dest = copy(&_Right);
        //ASSERT(dest != NULL);
    }

    // finally return a reference to ourselves
    return *this;
}

template<typename T>
inline FastMatrix<T> FastMatrix<T>::operator+(value_type _value)
{
    // Matrix addition
#if MATRIXT_FAST_MODE
    // Copy the current matrix
    FastMatrix<T> _Result((FastMatrix<T> &)*this);

    int _length       = _Result.sizes();
    value_type *pData = _Result.get_data();

    __ANNLAB_ASSERT(pData != NULL);
    for (int i=0; i<_length; ++i) {
        (*pData) +=_value;
        pData++;
    }
#else
    // Create the result matrix
    FastMatrix<T> _Result(rows, cols);

    for (int i=0; i<rows; ++i) {
        for (int j=0; j<cols; ++j)
            _Result.set_at(i, j, get_at(i, j) + _value);
    }
#endif

    return _Result;
}

template<typename T>
inline FastMatrix<T> FastMatrix<T>::operator+(FastMatrix<T>& _Right)
{
    // Copy the current matrix
    FastMatrix<T> _Result((FastMatrix<T> &)*this);
#if _DEBUG
    if (_Right.is_empty())
        return _Result;
#endif

    // Check whether it is its own matrix
    __ANNLAB_ASSERT(rows == _Right.rows && cols == _Right.cols);

#if _DEBUG
    if (rows != _Right.rows || cols != _Right.cols) {
        throw _T("Incompatible dimensions in operator + (MatrixT<T> & _Right).");
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
inline FastMatrix<T>& FastMatrix<T>::operator+=(value_type _value)
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
inline FastMatrix<T>& FastMatrix<T>::operator+=(FastMatrix<T>& _Right)
{
#if _DEBUG
    if (_Right.is_empty())
        return *this;
#endif

    // Check whether it is its own matrix
    __ANNLAB_ASSERT(rows == _Right.rows && cols == _Right.cols);

#if _DEBUG
    if (rows != _Right.rows || cols != _Right.cols) {
        throw _T("Incompatible dimensions in operator += (MatrixT<T> & _Right). ");
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
inline FastMatrix<T> FastMatrix<T>::operator*(value_type _value)
{
    // Copy the current matrix
    FastMatrix<T> _Result((FastMatrix<T> &)_Right);

    int _length       = _Result.sizes();
    value_type *pData = _Result.get_data();

    __ANNLAB_ASSERT(pData != NULL);
    for (int i=0; i<_length; ++i) {
        (*pData) *= _value;
        pData++;
    }
}

//#define MULTIPLY_SSE2_NAKED  1

template<typename T>
inline
#ifdef MULTIPLY_SSE2_NAKED
__declspec(naked)
#endif
void FastMatrix<T>::CalcArrayProducts_double_SSE2(pointer dest,
                                               pointer src,
                                               pointer alpha,
                                               unsigned int len)
{
    __asm {
#ifdef MULTIPLY_SSE2_NAKED
        push        ebp
        mov         ebp, esp
        push        edx
        push        ecx
        push        esi
        push        edi

        mov         edx, dword ptr [ebp + 10h]
        mov         ecx, dword ptr [ebp + 14h]
        mov         edi, dword ptr [ebp + 08h]
        mov         esi, dword ptr [ebp + 0Ch]
#else
        mov         edx, alpha
        mov         ecx, len
        mov         edi, dest
        mov         esi, src
#endif

        movlpd      xmm6, qword ptr [edx]           ; xmm6 = [0,     alpha]
        shr         ecx, 3
        shufpd      xmm6, xmm6, 00000000b           ; xmm6 = [alpha, alpha]

        ALIGN 16                                    ; Align address of loop to a 16-byte boundary.

sse2_multiple_loop:
        prefetchnta [esi + 768]
        prefetchnta [edi + 768]

        movapd      xmm2, xmmword ptr [esi]         ; [src]
        mulpd       xmm2, xmm6

        movapd      xmm3, xmmword ptr [esi + 16]    ; [src  + 16]
        addpd       xmm2, xmmword ptr [edi     ]
        mulpd       xmm3, xmm6    

        movapd      xmmword ptr [edi     ], xmm2

        movapd      xmm4, xmmword ptr [esi + 32]    ; [src  + 32]
        addpd       xmm3, xmmword ptr [edi + 16]
        mulpd       xmm4, xmm6

        movapd      xmmword ptr [edi + 16], xmm3

        movapd      xmm5, xmmword ptr [esi + 48]    ; [src  + 48]
        addpd       xmm4, xmmword ptr [edi + 32]
        mulpd       xmm5, xmm6

        movapd      xmmword ptr [edi + 32], xmm4

        addpd       xmm5, xmmword ptr [edi + 48]
        movapd      xmmword ptr [edi + 48], xmm5

        add         edi, 64
        add         esi, 64

        dec         ecx
        jne         sse2_multiple_loop

#ifdef MULTIPLY_SSE2_NAKED
        pop         edi
        pop         esi
        pop         ecx
        pop         edx
        mov         esp, ebp
        pop         ebp

        ret
#endif
    }
}

#define _MM128_MEM(ptr)           (*((__m128*)(ptr))
#define _MM128I_MEM(ptr)          (*((__m128i*)(ptr))
#define _MM128D_MEM(ptr)          (*((__m128d*)(ptr))

#define _MATRIXT_MULT_TEST_      1

#if _MATRIXT_MULT_TEST_

#define DOUBLE_NUMS_PER_LOOP     8
#define MM_PREFETCH_OFFSET       1024
#define MM_PREFETCH_OFFSET_V     (MM_PREFETCH_OFFSET/sizeof(value_type))
#define USE_SEE2_WRITE_PREFRTCH  1

#define _DONT_USE_MAT_MULT_SSE2  1
/* 0=纯C代码, 1=ASM嵌入SSE2代码, 2=编译器级别SSE2代码, 3=其他(默认) */
#define _MULT_SSE2_MODE          2

template<typename T>
inline FastMatrix<T> FastMatrix<T>::operator*(FastMatrix<T>& _Right)
{
    // 首先检查乘矩阵的列数和被乘矩阵的行数是否相同
    __ANNLAB_ASSERT(cols == _Right.rows);

    int _newRows, _newCols, _oldCols;
    _newRows = rows;
    _newCols = _Right.cols;
    _oldCols = cols;

    // 创建目标乘积矩阵
    FastMatrix<T> _Result(_newRows, _newCols);

#if _DONT_USE_MAT_MULT_SSE2
  #if 1
    //__declspec(align(16)) value_type _value;
    //__declspec(align(16)) double _value128[2] = { 0.0, 0.0 };
    FastMatrix<T> tmp_row(1, _oldCols);
    register pointer temp_ptr;
    register pointer right_ptr;
    pointer out_ptr;
    __m128d mm0, mm1, mm2, mm3;
    __m128d mm4, mm5, mm6, mm7;
    __m128d mmv0, mmv1;

    for (int _row=0; _row<_newRows; ++_row) {
        tmp_row.copy_row(*this, _row);
        out_ptr = _Result.get_data() + _row;
        for (int _col=0; _col<_newCols; ++_col) {
            //_value = value_type(0.0);
            temp_ptr  = tmp_row.get_data();
            right_ptr = _Right.get_colptr(_col);
    #if 1
            /*
            __asm {
                nop
                nop
                emms
            }
            //*/
            mmv0 = _mm_setzero_pd();
            mmv1 = _mm_setzero_pd();
            for (int i=0; i<_oldCols/8; ++i) {
                // prefetchnta [addr + offset]
                _mm_prefetch((char *)(temp_ptr  + 1024/8), _MM_HINT_NTA);
                _mm_prefetch((char *)(right_ptr + 1024/8), _MM_HINT_NTA);

                mm0 = _mm_load_pd(temp_ptr);
                mm1 = _mm_load_pd(right_ptr);

                mm2 = _mm_load_pd(temp_ptr  + 2);
                mm3 = _mm_load_pd(right_ptr + 2);

                mm0 = _mm_mul_pd(mm0, mm1);
                mm2 = _mm_mul_pd(mm2, mm3);

                mm4 = _mm_load_pd(temp_ptr  + 4);
                mm5 = _mm_load_pd(right_ptr + 4);

                mm6 = _mm_load_pd(temp_ptr  + 6);
                mm7 = _mm_load_pd(right_ptr + 6);

                mm4 = _mm_mul_pd(mm4, mm5);
                mm6 = _mm_mul_pd(mm6, mm7);

                mm0 = _mm_add_pd(mm0, mm2);
                mm4 = _mm_add_pd(mm4, mm6);

                mmv0 = _mm_add_pd(mmv0, mm0);
                mmv1 = _mm_add_pd(mmv1, mm4);

                // _value += get_at(i, k) * _Right.get_at(k, j);
                // _value += tmp_rowdata[i] * right_colptr[i];
                // _value += (*tmp_rowdata++) * (*right_colptr++);

                temp_ptr  += 8;
                right_ptr += 8;
            }
            mmv0 = _mm_hadd_pd(mmv0, mmv1);
            mmv0 = _mm_hadd_pd(mmv0, mmv0);
            // _Result.set_at(_row, _col, _value);
            // (*out_ptr) = _value;
            _mm_store_sd(out_ptr, mmv0);
            //_mm_stream_pd(out_ptr, mmv1);
            out_ptr += _newRows;
            /*
            __asm {
                nop
                emms
            }
            //*/
    #else
            for (int i=0; i<_oldCols/8; ++i) {
                // prefetchnta [addr + offset]
                _mm_prefetch((char *)(temp_ptr  + MM_PREFETCH_OFFSET_V), _MM_HINT_NTA);
                _mm_prefetch((char *)(right_ptr + MM_PREFETCH_OFFSET_V), _MM_HINT_NTA);

                mm0 = _mm_load_pd(temp_ptr);
                mm1 = _mm_load_pd(right_ptr);

                mm2 = _mm_load_pd(temp_ptr  + 2);
                mm3 = _mm_load_pd(right_ptr + 2);

                mm4 = _mm_load_pd(temp_ptr  + 4);
                mm5 = _mm_load_pd(right_ptr + 4);

                mm6 = _mm_load_pd(temp_ptr  + 6);
                mm7 = _mm_load_pd(right_ptr + 6);

                mm0 = _mm_mul_pd(mm0, mm1);
                mm2 = _mm_mul_pd(mm2, mm3);

                mm4 = _mm_mul_pd(mm4, mm5);
                mm6 = _mm_mul_pd(mm6, mm7);

                mm0 = _mm_hadd_pd(mm0, mm2);
                mm0 = _mm_hadd_pd(mm0, mm0);

                //mmv0 = _mm_load_pd(&_value);

                mm4 = _mm_hadd_pd(mm4, mm6);
                mm4 = _mm_hadd_pd(mm4, mm4);

                mm0 = _mm_add_pd(mm0, mm4);

                _mm_store_sd((double *)&_value, mm0);

                temp_ptr += 8;
                right_ptr += 8;
                //_value += (*temp_ptr++) * (*right_ptr++);
            }
            //_Result.set_at(_row, _col, _value);
            (*out_ptr) = _value;
            out_ptr += _newRows;
    #endif
        }
    }

  #else
    value_type _value;
    FastMatrix<T> tmp_row(1, _oldCols);
    register pointer tmp_rowdata;
    register pointer right_colptr;
    pointer out_ptr;

    for (int _row=0; _row<_newRows; ++_row) {
        tmp_row.copy_row(*this, _row);
        out_ptr = _Result.get_data() + _row;
        for (int _col=0; _col<_newCols; ++_col) {
            _value = value_type(0.0);
            tmp_rowdata  = tmp_row.get_data();
            right_colptr = _Right.get_colptr(_col);
            for (int i=0; i<_oldCols; ++i) {
                //_value += get_at(i, k) * _Right.get_at(k, j);
                //_value += tmp_rowdata[i] * right_colptr[i];
                _value += (*tmp_rowdata++) * (*right_colptr++);
            }
            //_Result.set_at(_row, _col, _value);
            (*out_ptr) = _value;
            out_ptr += _newRows;
        }
    }
  #endif
#else  /* !_DONT_USE_MAT_MULT_SSE2 */
    value_type temp;
    register pointer left_ptr, out_ptr;
    pointer right_ptr;
    right_ptr = _Right.get_data();

    for (int n_col=0; n_col<_newCols; ++n_col) {
        out_ptr = _Result.get_data() + n_col * _newRows;
        for (int n_row=0; n_row<_newRows; ++n_row) {
            *out_ptr++ = value_type(0.0);
        }
        for (int r_row=0; r_row<_oldCols; ++r_row) {
            temp = right_ptr[r_row + n_col * _oldCols];
            if (temp > value_type(FLOAT_EPSINON) || temp < value_type(-FLOAT_EPSINON)) {
                out_ptr  = _Result.get_data() + n_col * _newRows;
                left_ptr = get_data() + r_row * _newRows;

            #if (_MULT_SSE2_MODE == 0)      // pure c code
                /*
                __asm {
                    nop
                    emms
                }  //*/
                for (int s_row=0; s_row<_newRows; ++s_row) {
                    (*out_ptr++) += temp * (*left_ptr++);
                }
                /*
                __asm {
                    nop
                    nop
                    emms
                }  //*/
            #elif (_MULT_SSE2_MODE == 1)    // asm SSE2 code
                // call SSE2 asm routine
                CalcArrayProducts_double_SSE2(out_ptr, left_ptr, &temp, _newRows);

            #elif (_MULT_SSE2_MODE == 2)    // intel/vc.net inline SSE2 code
                __m128d tmp;
                __m128d mm1, mm2, mm3, mm4;
#if USE_SEE2_WRITE_PREFRTCH
                __m128d mma, mmb;
#endif
#if (DOUBLE_NUMS_PER_LOOP == 16)
                __m128d mm5, mm6, mm7, mm8;
#endif
                tmp = _mm_setzero_pd();
                tmp = _mm_loadl_pd(tmp, &temp);
                tmp = _mm_shuffle_pd(tmp, tmp, 0);

                for (int s_row=_newRows/DOUBLE_NUMS_PER_LOOP;
                    s_row >= 0;
                    out_ptr += DOUBLE_NUMS_PER_LOOP, left_ptr += DOUBLE_NUMS_PER_LOOP, --s_row) {
#if USE_SEE2_WRITE_PREFRTCH
                    // prefetchnta [addr + offset]
                    _mm_prefetch((char *)(left_ptr + MM_PREFETCH_OFFSET_V), _MM_HINT_NTA);
                    _mm_prefetch((char *)(out_ptr  + MM_PREFETCH_OFFSET_V), _MM_HINT_NTA);

                    mm1 = _mm_load_pd(left_ptr   );
                    mm2 = _mm_load_pd(left_ptr + 2);

                    mma = _mm_load_pd(out_ptr);
                    mmb = _mm_load_pd(out_ptr + 2);

                    mm1 = _mm_mul_pd(mm1, tmp);
                    mm2 = _mm_mul_pd(mm2, tmp);

                    mm3 = _mm_load_pd(left_ptr + 4);
                    mm4 = _mm_load_pd(left_ptr + 6);

                    mm3 = _mm_mul_pd(mm3, tmp);
                    mm4 = _mm_mul_pd(mm4, tmp);

                    mm1 = _mm_add_pd(mm1, mma);
                    mm2 = _mm_add_pd(mm2, mmb);

                    mm3 = _mm_add_pd(mm3, *((__m128d*)(out_ptr + 4)));
                    mm4 = _mm_add_pd(mm4, *((__m128d*)(out_ptr + 6)));

                    _mm_store_pd(out_ptr,     mm1);
                    _mm_store_pd(out_ptr + 2, mm2);

#if (DOUBLE_NUMS_PER_LOOP == 16)
                    _mm_prefetch((char *)(left_ptr + MM_PREFETCH_OFFSET_V + 8), _MM_HINT_NTA);
                    _mm_prefetch((char *)(out_ptr  + MM_PREFETCH_OFFSET_V + 8), _MM_HINT_NTA);

                    mm5 = _mm_load_pd(left_ptr + 8);
                    mm6 = _mm_load_pd(left_ptr + 10);
#endif
                    _mm_store_pd(out_ptr + 4, mm3);
                    _mm_store_pd(out_ptr + 6, mm4);

#if (DOUBLE_NUMS_PER_LOOP == 16)
                    mma = _mm_load_pd(out_ptr + 8);
                    mmb = _mm_load_pd(out_ptr + 10);

                    mm5 = _mm_mul_pd(mm5, tmp);
                    mm6 = _mm_mul_pd(mm6, tmp);

                    mm7 = _mm_load_pd(left_ptr + 12);
                    mm8 = _mm_load_pd(left_ptr + 14);

                    mm5 = _mm_add_pd(mm5, mma);
                    mm6 = _mm_add_pd(mm6, mmb);

                    mm7 = _mm_mul_pd(mm7, tmp);
                    mm8 = _mm_mul_pd(mm8, tmp);

                    _mm_store_pd(out_ptr + 8,  mm5);
                    _mm_store_pd(out_ptr + 10, mm6);

                    mm7 = _mm_add_pd(mm7, *((__m128d*)(out_ptr + 12)));
                    mm8 = _mm_add_pd(mm8, *((__m128d*)(out_ptr + 14)));

                    _mm_store_pd(out_ptr + 12, mm7);
                    _mm_store_pd(out_ptr + 14, mm8);
#endif
#else /* !USE_SEE2_WRITE_PREFRTCH */
                    // prefetchnta [addr + offset]
                    _mm_prefetch((char *)(left_ptr + MM_PREFETCH_OFFSET_V), _MM_HINT_NTA);
                    _mm_prefetch((char *)(out_ptr  + MM_PREFETCH_OFFSET_V), _MM_HINT_NTA);

                    mm1 = _mm_load_pd(left_ptr   );
                    mm2 = _mm_load_pd(left_ptr + 2);

                    mm3 = _mm_load_pd(left_ptr + 4);
                    mm4 = _mm_load_pd(left_ptr + 6);

                    mm1 = _mm_mul_pd(mm1, tmp);
                    mm2 = _mm_mul_pd(mm2, tmp);

                    mm3 = _mm_mul_pd(mm3, tmp);
                    mm4 = _mm_mul_pd(mm4, tmp);

                    mm1 = _mm_add_pd(mm1, *((__m128d*)(out_ptr))   );
                    mm2 = _mm_add_pd(mm2, *((__m128d*)(out_ptr + 2)));

                    mm3 = _mm_add_pd(mm3, *((__m128d*)(out_ptr + 4)));
                    mm4 = _mm_add_pd(mm4, *((__m128d*)(out_ptr + 6)));

                    _mm_store_pd(out_ptr,     mm1);
                    _mm_store_pd(out_ptr + 2, mm2);

#if (DOUBLE_NUMS_PER_LOOP == 16)
                    _mm_prefetch((char *)(left_ptr + MM_PREFETCH_OFFSET_V + 8), _MM_HINT_NTA);
                    _mm_prefetch((char *)(out_ptr  + MM_PREFETCH_OFFSET_V + 8), _MM_HINT_NTA);

                    mm5 = _mm_load_pd(left_ptr + 8);
                    mm6 = _mm_load_pd(left_ptr + 10);
#endif
                    _mm_store_pd(out_ptr + 4, mm3);
                    _mm_store_pd(out_ptr + 6, mm4);

#if (DOUBLE_NUMS_PER_LOOP == 16)
                    mm5 = _mm_mul_pd(mm5, tmp);
                    mm6 = _mm_mul_pd(mm6, tmp);

                    mm7 = _mm_load_pd(left_ptr + 12);
                    mm8 = _mm_load_pd(left_ptr + 14);

                    mm5 = _mm_add_pd(mm5, *((__m128d*)(out_ptr +  8)));
                    mm6 = _mm_add_pd(mm6, *((__m128d*)(out_ptr + 10)));

                    mm7 = _mm_mul_pd(mm7, tmp);
                    mm8 = _mm_mul_pd(mm8, tmp);

                    _mm_store_pd(out_ptr + 8,  mm5);
                    _mm_store_pd(out_ptr + 10, mm6);

                    mm7 = _mm_add_pd(mm7, *((__m128d*)(out_ptr + 12)));
                    mm8 = _mm_add_pd(mm8, *((__m128d*)(out_ptr + 14)));

                    _mm_store_pd(out_ptr + 12, mm7);
                    _mm_store_pd(out_ptr + 14, mm8);
#endif
        #endif  /* !USE_SEE2_WRITE_PREFRTCH */
                }
    #endif  /* _MULT_SSE2_MODE other value */
            }
        }
    }
#endif /* !_DONT_USE_MAT_MULT_SSE2 */

    return _Result;
}

#else  /* !_MATRIXT_MULT_TEST_ */

template<typename T>
inline FastMatrix<T> FastMatrix<T>::operator*(FastMatrix<T>& _Right)
{
    // 首先检查乘矩阵的列数和被乘矩阵的行数是否相同
    __ANNLAB_ASSERT(cols == _Right.rows);

    int _newRows, _newCols, _oldCols;
    _newRows = rows;
    _newCols = _Right.cols;
    _oldCols = cols;

    // 创建目标乘积矩阵
    FastMatrix<T> _Result(_newRows, _newCols);

    // Matrix multiplication，即
    //
    // [A][B][C]   [G][H]     [A*G + B*I + C*K][A*H + B*J + C*L]
    // [D][E][F] * [I][J]  =  [D*G + E*I + F*K][D*H + E*J + F*L]
    //             [K][L]
    //
if (typeid(value_type) == typeid(double)) {

#if 1 & MATRIXT_FAST_MODE
  #if 1
    #if 1
      #if 1
    register value_type temp;
    register pointer left_ptr, out_ptr;
    pointer right_ptr;
    //left_ptr  = get_data();
    right_ptr = _Right.get_data();
    //out_ptr   = _Result.get_data();

#define DOUBLE_NUMS_PER_LOOP    16
#define MM_PREFETCH_OFFSET      768
#define MM_PREFETCH_OFFSET_V    (MM_PREFETCH_OFFSET/sizeof(value_type))
#define USE_SEE2_WRITE_PREFRTCH 1

    for (int n_col=0; n_col<_newCols; ++n_col) {
        out_ptr = _Result.get_data() + n_col * _newRows;
        for (int n_row=0; n_row<_newRows; ++n_row) {
            *out_ptr++ = value_type(0.0);
        }
        for (int r_row=0; r_row<_oldCols; ++r_row) {
            temp = right_ptr[r_row + n_col * _oldCols];
            //if (temp != value_type(0.0)) {
            if (temp > value_type(FLOAT_EPSINON) || temp < value_type(-FLOAT_EPSINON)) {
                out_ptr  = _Result.get_data() + n_col * _newRows;
                left_ptr = get_data() + r_row * _newRows;
  #if 1
                /*
                __asm {
                    nop
                    nop
                    nop
                    emms
                }
                //*/
    #if 0
                CalcArrayProducts_double_SSE2(out_ptr, left_ptr, &temp, _newRows);
                ///*
    #else
                __m128d tmp;
                __m128d mm1, mm2, mm3, mm4;
        #if USE_SEE2_WRITE_PREFRTCH
                __m128d mma, mmb;
        #endif
#if (DOUBLE_NUMS_PER_LOOP == 16)
                __m128d mm5, mm6, mm7, mm8;
#endif
                tmp = _mm_setzero_pd();
                tmp = _mm_loadl_pd(tmp, &temp);
                tmp = _mm_shuffle_pd(tmp, tmp, 0);
                pointer end_ptr = out_ptr + _newRows;

                for (int s_row=_newRows/DOUBLE_NUMS_PER_LOOP;
                    s_row >= 0;
                    out_ptr += DOUBLE_NUMS_PER_LOOP, left_ptr += DOUBLE_NUMS_PER_LOOP, --s_row) {
                /*
                for (; (out_ptr != NULL) && (out_ptr < end_ptr);
                    out_ptr+=DOUBLE_COUNT_PER_TIME, left_ptr+=DOUBLE_COUNT_PER_TIME) {
                //*/
        #if USE_SEE2_WRITE_PREFRTCH
                    // prefetchnta [addr]
                    _mm_prefetch((char *)(left_ptr + MM_PREFETCH_OFFSET_V), _MM_HINT_NTA);
                    _mm_prefetch((char *)(out_ptr  + MM_PREFETCH_OFFSET_V), _MM_HINT_NTA);

                    mm1 = _mm_load_pd(left_ptr   );
                    mm2 = _mm_load_pd(left_ptr + 2);

                    mma = _mm_load_pd(out_ptr);
                    mmb = _mm_load_pd(out_ptr + 2);

                    mm1 = _mm_mul_pd(mm1, tmp);
                    mm2 = _mm_mul_pd(mm2, tmp);

                    mm3 = _mm_load_pd(left_ptr + 4);
                    mm4 = _mm_load_pd(left_ptr + 6);

                    mm3 = _mm_mul_pd(mm3, tmp);
                    mm4 = _mm_mul_pd(mm4, tmp);

                    mm1 = _mm_add_pd(mm1, mma);
                    mm2 = _mm_add_pd(mm2, mmb);

                    mm3 = _mm_add_pd(mm3, *((__m128d*)(out_ptr + 4)));
                    mm4 = _mm_add_pd(mm4, *((__m128d*)(out_ptr + 6)));

                    _mm_store_pd(out_ptr,     mm1);
                    _mm_store_pd(out_ptr + 2, mm2);

#if (DOUBLE_NUMS_PER_LOOP == 16)
                    _mm_prefetch((char *)(left_ptr + MM_PREFETCH_OFFSET_V + 8), _MM_HINT_NTA);
                    _mm_prefetch((char *)(out_ptr  + MM_PREFETCH_OFFSET_V + 8), _MM_HINT_NTA);

                    mm5 = _mm_load_pd(left_ptr + 8);
                    mm6 = _mm_load_pd(left_ptr + 10);
#endif
                    _mm_store_pd(out_ptr + 4, mm3);
                    _mm_store_pd(out_ptr + 6, mm4);

#if (DOUBLE_NUMS_PER_LOOP == 16)
                    mma = _mm_load_pd(out_ptr + 8);
                    mmb = _mm_load_pd(out_ptr + 10);

                    mm5 = _mm_mul_pd(mm5, tmp);
                    mm6 = _mm_mul_pd(mm6, tmp);

                    mm7 = _mm_load_pd(left_ptr + 12);
                    mm8 = _mm_load_pd(left_ptr + 14);

                    mm5 = _mm_add_pd(mm5, mma);
                    mm6 = _mm_add_pd(mm6, mmb);

                    mm7 = _mm_mul_pd(mm7, tmp);
                    mm8 = _mm_mul_pd(mm8, tmp);

                    _mm_store_pd(out_ptr + 8,  mm5);
                    _mm_store_pd(out_ptr + 10, mm6);

                    mm7 = _mm_add_pd(mm7, *((__m128d*)(out_ptr + 12)));
                    mm8 = _mm_add_pd(mm8, *((__m128d*)(out_ptr + 14)));

                    _mm_store_pd(out_ptr + 12, mm7);
                    _mm_store_pd(out_ptr + 14, mm8);
#endif
        #else /* !USE_SEE2_WRITE_PREFRTCH */
                    // prefetchnta [addr]
                    _mm_prefetch((char *)(left_ptr + MM_PREFETCH_OFFSET_V), _MM_HINT_NTA);
                    _mm_prefetch((char *)(out_ptr  + MM_PREFETCH_OFFSET_V), _MM_HINT_NTA);

                    mm1 = _mm_load_pd(left_ptr   );
                    mm2 = _mm_load_pd(left_ptr + 2);

                    mm3 = _mm_load_pd(left_ptr + 4);
                    mm4 = _mm_load_pd(left_ptr + 6);

                    mm1 = _mm_mul_pd(mm1, tmp);
                    mm2 = _mm_mul_pd(mm2, tmp);

                    mm3 = _mm_mul_pd(mm3, tmp);
                    mm4 = _mm_mul_pd(mm4, tmp);

                    mm1 = _mm_add_pd(mm1, *((__m128d*)(out_ptr))   );
                    mm2 = _mm_add_pd(mm2, *((__m128d*)(out_ptr + 2)));

                    mm3 = _mm_add_pd(mm3, *((__m128d*)(out_ptr + 4)));
                    mm4 = _mm_add_pd(mm4, *((__m128d*)(out_ptr + 6)));

                    _mm_store_pd(out_ptr,     mm1);
                    _mm_store_pd(out_ptr + 2, mm2);

#if (DOUBLE_NUMS_PER_LOOP == 16)
                    _mm_prefetch((char *)(left_ptr + MM_PREFETCH_OFFSET_V + 8), _MM_HINT_NTA);
                    _mm_prefetch((char *)(out_ptr  + MM_PREFETCH_OFFSET_V + 8), _MM_HINT_NTA);

                    mm5 = _mm_load_pd(left_ptr + 8);
                    mm6 = _mm_load_pd(left_ptr + 10);
#endif
                    _mm_store_pd(out_ptr + 4, mm3);
                    _mm_store_pd(out_ptr + 6, mm4);

#if (DOUBLE_NUMS_PER_LOOP == 16)
                    mm5 = _mm_mul_pd(mm5, tmp);
                    mm6 = _mm_mul_pd(mm6, tmp);

                    mm7 = _mm_load_pd(left_ptr + 12);
                    mm8 = _mm_load_pd(left_ptr + 14);

                    mm5 = _mm_add_pd(mm5, *((__m128d*)(out_ptr +  8)));
                    mm6 = _mm_add_pd(mm6, *((__m128d*)(out_ptr + 10)));

                    mm7 = _mm_mul_pd(mm7, tmp);
                    mm8 = _mm_mul_pd(mm8, tmp);

                    _mm_store_pd(out_ptr + 8,  mm5);
                    _mm_store_pd(out_ptr + 10, mm6);

                    mm7 = _mm_add_pd(mm7, *((__m128d*)(out_ptr + 12)));
                    mm8 = _mm_add_pd(mm8, *((__m128d*)(out_ptr + 14)));

                    _mm_store_pd(out_ptr + 12, mm7);
                    _mm_store_pd(out_ptr + 14, mm8);
#endif
        #endif  /* !USE_SEE2_WRITE_PREFRTCH */
                    /*
                    __asm {
                        nop
                        emms
                    }
                    //*/
                }
                //*/
    #endif
  #else
                for (int s_row=0; s_row<_newRows; ++s_row) {
                    (*out_ptr++) += temp * (*left_ptr++);
                }
                /*
                __asm {
                    nop
                    nop
                    emms
                }
                //*/
  #endif
                /*
                for (int s_row=0; s_row<_newRows/2; ++s_row) {
                    *out_ptr     += temp * (*left_ptr);
                    *(out_ptr+1) += temp * (*(left_ptr+1));
                    out_ptr  += 2;
                    left_ptr += 2;
                }
                if (_newRows & 1)
                    *out_ptr += temp * (*left_ptr);
                //*/
            }
        }
    }
      #else
    value_type temp;
    pointer left_ptr, right_ptr, out_ptr;
    left_ptr  = pvData;
    right_ptr = _Right.get_data();
    out_ptr   = _Result.get_data();

    for (int n_col=0; n_col<_newCols; ++n_col) {
        for (int n_row=0; n_row<_newRows; ++n_row) {
            //_Result.set_at(n_row, n_col, value_type(0.0));
            out_ptr[n_row + n_col * _newRows] = value_type(0.0);
        }
        for (int r_row=0; r_row<_oldCols; ++r_row) {
            //temp = _Right.get_at(r_row, n_col);
            temp = right_ptr[r_row + n_col * _oldCols];
            if (temp > value_type(FLOAT_EPSINON) || temp < value_type(-FLOAT_EPSINON)) {
                for (int s_row=0; s_row<_newRows; ++s_row) {
                    //_Result.set_at(s_row, n_col,
                    //    _Result.get_at(s_row, n_col) + temp * get_at(s_row, r_row));
                    out_ptr[s_row + n_col * _newRows] += temp * left_ptr[s_row + r_row * _newRows];
                    //_value1 = value_type(0.0);
                    //_value2 = value_type(0.0);
                }
            }
        }
    }
      #endif
    #else
    value_type _value1, _value2;
    FastMatrix<T> tmp_row(1, _oldCols);
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
    #endif
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
#if 1
    // 这个版本要稍快于下面的版本, 与我预料的有点差距, 不解..., 不过对Cache利用都不好
    value_type _value;
    for (int i=0; i<_newRows; ++i) {
        for (int j=0; j<_newCols; ++j) {
            _value = value_type(0.0);
            for (int k=0; k<_oldCols; ++k)
                _value += get_at(i, k) * _Right.get_at(k, j);
            _Result.set_at(i, j, _value);
        }
    }
#else
    // 表面上看这个利用缓存要好一些, 但比上面的版本慢, 可能是get_at(i, k)地址过于的游离导致
    // _Right.get_at(k, j)很容易在Cache中被挤到2级缓存(L2 Cache)或导致未命中率提高
    value_type _value;
    for (int j=0; j<_newCols; ++j) {
        for (int i=0; i<_newRows; ++i) {
            _value = value_type(0.0);
            for (int k=0; k<_oldCols; ++k)
                _value += get_at(i, k) * _Right.get_at(k, j);
            _Result.set_at(i, j, _value);
        }
    }
#endif
#endif  // !MATRIXT_FAST_MODE
    }
    else {
        value_type temp;
        pointer left_ptr, right_ptr, out_ptr;
        left_ptr  = pvData;
        right_ptr = _Right.get_data();
        out_ptr   = _Result.get_data();

        for (int n_col=0; n_col<_newCols; ++n_col) {
            for (int n_row=0; n_row<_newRows; ++n_row) {
                //_Result.set_at(n_row, n_col, value_type(0.0));
                out_ptr[n_row + n_col * _newRows] = value_type(0.0);
            }
            for (int r_row=0; r_row<_oldCols; ++r_row) {
                //temp = _Right.get_at(r_row, n_col);
                temp = right_ptr[r_row + n_col * _oldCols];
                if (temp > value_type(FLOAT_EPSINON) || temp < value_type(-FLOAT_EPSINON)) {
                    for (int s_row=0; s_row<_newRows; ++s_row) {
                        //_Result.set_at(s_row, n_col,
                        //    _Result.get_at(s_row, n_col) + temp * get_at(s_row, r_row));
                        out_ptr[s_row + n_col * _newRows] += temp * left_ptr[s_row + r_row * _newRows];
                        //_value1 = value_type(0.0);
                        //_value2 = value_type(0.0);
                    }
                }
            }
        }
    }

    return _Result;
}

#endif  // !_MATRIXT_MULT_TEST_

template<typename T>
inline FastMatrix<T>& FastMatrix<T>::operator*=(value_type _value)
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
inline FastMatrix<T>& FastMatrix<T>::operator*=(FastMatrix<T>& _Right)
{
    // 首先检查乘矩阵的行数和被乘矩阵的列数是否相同
    __ANNLAB_ASSERT(cols == _Right.rows);

    // Copy the current matrix
    FastMatrix<T> _Left((FastMatrix<T> &)*this);

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
inline FastMatrix<T>& FastMatrix<T>::transpose(void)
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
        value_type temp;
        for (int i=0; i<rows; ++i) {
            for (int j=i+1; j<cols; ++j) {
                // 仅交换需要转置的元素
                temp = get_at(j, i);
                set_at(j, i, get_at(i, j));
                set_at(i, j, temp);
            }
        }
    }
    else {
        // 先复制一个原矩阵的副本
        FastMatrix<T> _trans((FastMatrix<T> &)*this);

        // 转置行和列数
        int temp = rows;
        rows = cols;
        cols = temp;

        // 转置各元素
        for (int i=0; i<cols; ++i) {
            for (int j=0; j<rows; ++j) {
                set_at(j, i, _trans.get_at(i, j));
            }
        }
    }

    return *this;
}

template<typename T>
inline int FastMatrix<T>::zeros(void)
{
    // 重置大小并初始化为全0矩阵
    return refill(INIT_FCN_ZEROS);
}

template<typename T>
inline int FastMatrix<T>::ones(void)
{
    // 重置大小并初始化为全1矩阵
    return refill(INIT_FCN_ONES);
}

template<typename T>
inline int FastMatrix<T>::rands(void)
{
    // 重置大小并初始化为[-1,1]随机数矩阵
    return refill(INIT_FCN_RANDS);
}

template<typename T>
inline int FastMatrix<T>::rands2(void)
{
    // 重置大小并初始化为[0,1]随机数矩阵
    return refill(INIT_FCN_RANDS_POSITIVE);
}

template<typename T>
inline int FastMatrix<T>::order(void)
{
    // 重置大小并初始化为指定的特殊顺序排列矩阵
    return refill(INIT_FCN_ORDER);
}

template<typename T>
inline int FastMatrix<T>::zeros(int _rows, int _cols)
{
    // 重置大小并初始化为全0矩阵
    return resize(_rows, _cols, INIT_FCN_ZEROS);
}

template<typename T>
inline int FastMatrix<T>::ones(int _rows, int _cols)
{
    // 重置大小并初始化为全1矩阵
    return resize(_rows, _cols, INIT_FCN_ONES);
}

template<typename T>
inline int FastMatrix<T>::rands(int _rows, int _cols)
{
    // 重置大小并初始化为[-1,1]随机数矩阵
    return resize(_rows, _cols, INIT_FCN_RANDS);
}

template<typename T>
inline int FastMatrix<T>::rands2(int _rows, int _cols)
{
    // 重置大小并初始化为[0,1]随机数矩阵
    return resize(_rows, _cols, INIT_FCN_RANDS_POSITIVE);
}

template<typename T>
inline int FastMatrix<T>::order(int _rows, int _cols)
{
    // 重置大小并初始化为指定的特殊顺序排列矩阵
    return resize(_rows, _cols, INIT_FCN_ORDER);
}

template<typename T>
inline FastMatrix<T> FastMatrix<T>::_zeros(int _rows, int _cols)
{
    // Copy the current matrix
    FastMatrix<T> _zeros(_rows, _cols);

    // 所有元素置0
    for (int i=0; i<_rows; ++i) {
        for (int j=0; j<_cols; ++j)
            _zeros.set_at(i, j, value_type(0.0));
    }

    return _zeros;
}

template<typename T>
inline FastMatrix<T> FastMatrix<T>::_ones(int _rows, int _cols)
{
    // Copy the current matrix
    FastMatrix<T> _ones(_rows, _cols);

    // 所有元素置1
    for (int i=0; i<_rows; ++i) {
        for (int j=0; j<_cols; ++j)
            _ones.set_at(i, j, value_type(1.0));
    }

    return _ones;
}

template<typename T>
inline FastMatrix<T> FastMatrix<T>::_rands(int _rows, int _cols)
{
    // Copy the current matrix
    FastMatrix<T> _rands(_rows, _cols);

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
inline FastMatrix<T> FastMatrix<T>::_rands2(int _rows, int _cols)
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
inline FastMatrix<T> FastMatrix<T>::_order(int _rows, int _cols)
{
    // Copy the current matrix
    CAnnMatrix _order(_rows, _cols);

    // 按指定的顺序编号
    int _nIndex;
    for (int i=0; i<_rows; ++i) {
        for (int j=0; j<_cols; ++j) {
            if (order == 0)
			    _nIndex = j * _rows + i;
            else
                _nIndex = i * _cols + j;
            _order.set_at(i, j, static_cast<value_type>(_nIndex));
        }
    }

    return _order;
}

template<typename T>
inline void FastMatrix<T>::fill_data(int _rows, int _cols,
                           int _initFcn /*= MAT_INIT_DEFAULT */,
                           value_type _fillVal /*= 0.0*/)
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
    case INIT_FCN_RANDS:           // all [-1, 1] randomize
        for (int i=0; i<_totals; ++i)
            pvData[i] = value_type(value_type(2.0) * value_type(rand()) / value_type(RAND_MAX) - value_type(1.0));
        break;
    case INIT_FCN_RANDS_POSITIVE:  // all [0, 1] positive randomize
        for (int i=0; i<_totals; ++i)
            pvData[i] = value_type(rand()) / value_type(RAND_MAX);
        break;
    case INIT_FCN_ORDER:
		for (int i=0; i<_totals; i++) {
			pvData[i] = value_type(i);
		}
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
inline void FastMatrix<Ty>::fill_data(int _rows, int _cols, \
                            int _initFcn /*= MAT_INIT_DEFAULT */, \
                            value_type _fillVal /*= 0.0*/) \
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
    case INIT_FCN_ORDER: \
		for (int i=0; i<_totals; i++) { \
			pvData[i] = value_type(i); \
		} \
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
inline void FastMatrix<Ty>::fill_data(int _rows, int _cols, \
                            int _initFcn /*= MAT_INIT_DEFAULT */, \
                            value_type _fillVal /*= 0.0*/) \
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

 HRESULT __fastcall AnsiToUnicode(LPCSTR pszA, LPOLESTR *ppszW)
{

    ULONG cCharacters;
    DWORD dwError;

    // If input is null then just return the same.
    if (NULL == pszA) {
        *ppszW = NULL;
        return NOERROR;
    }

    // Determine number of wide characters to be allocated for the
    // Unicode string.
    cCharacters = strlen(pszA) + 1;

    // Use of the OLE allocator is required if the resultant Unicode
    // string will be passed to another COM component and if that
    // component will free it. Otherwise you can use your own allocator.
    *ppszW = (LPOLESTR)CoTaskMemAlloc(cCharacters * 2);
    if (NULL == *ppszW)
        return E_OUTOFMEMORY;

    // Covert to Unicode.
    if (0 == MultiByteToWideChar(CP_ACP, 0, pszA, cCharacters,
            *ppszW, cCharacters)) {
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
void FastMatrix<T>::display(void)
{
    display(name());
}

template<typename T>
void FastMatrix<T>::display(const TCHAR *szText)
{
#if defined(ANNLAB_USE_DISPLAY) && (ANNLAB_USE_DISPLAY)
    TCHAR *szTypeName = NULL;
    //AnsiToUnicode(typeid(T).name(), &szTypeName);
    TRACE(("FastMatrix<%s>: Name = [ %s ], [rows = %d, cols = %d]\n"), typeid(T).name(), szText, rows, cols);
    //TRACE(_T("FastMatrix<double>: Name = [ %s ], [rows = %d, cols = %d]\n"), szText, rows, cols);
    TRACE(_T("============================================================================================================\n\n"));
    for (int r=0; r<rows; r++) {
        TRACE(_T("\t"));
        for (int c=0; c<cols; c++) {
            value_type _value = get_at(r, c);
            if (fabs(_value) < static_cast<T>(10000000.0))
                TRACE(_T("%12.4f "), _value);
            else
                TRACE(_T("%12.4E "), _value);
        }
        TRACE(_T("\n\n"));
    }
    TRACE(_T("============================================================================================================\n\n"));
    //if (szTypeName != NULL)
    //    CoTaskMemFree(szTypeName);
#endif
}

#define MATRIXT_DISPLAY_FUNC_SIGNED(Ty) \
template<> \
void FastMatrix<Ty>::display(const TCHAR *szText) \
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
void FastMatrix<Ty>::display(const TCHAR *szText) \
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
void FastMatrix<T>::display_ex(void)
{
    display_ex(name());
}

template<typename T>
void FastMatrix<T>::display_ex(const TCHAR *szText)
{
#if defined(ANNLAB_USE_DISPLAY_EX) && (ANNLAB_USE_DISPLAY_EX)
    char*  szTypeNameA = NULL;
    TCHAR* szTypeName = NULL;
#ifdef _UNICODE
    szTypeNameA = (char*)typeid(T).name();
    AnsiToUnicode(szTypeNameA, &szTypeName);
#else
    szTypeNameA = (char*)typeid(T).name();
    szTypeName = szTypeNameA;
#endif
    TRACE(_T("MatrixT<%s>: Name = [ %s ], [rows = %d, cols = %d]\n"), szTypeName, szText, rows, cols);
    TRACE(_T("============================================================================================================\n\n"));
    for (int r=0; r<rows; r++) {
        TRACE(_T("\t"));
        for (int c=0; c<cols; c++) {
            value_type value = get_at(r, c);
            if (fabs(value) < static_cast<T>(10000000.0))
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
#ifndef _DEBUG
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
#endif
}

#define MATRIXT_DISPLAY_EX_FUNC_SIGNED(Ty) \
template<> \
void FastMatrix<Ty>::display_ex(const TCHAR *szText) \
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
void FastMatrix<Ty>::display_ex(const TCHAR *szText) \
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


#ifndef _FASTMATRIX_IMPL_internal_H_
}  // namespace annlab
#endif

#endif  // _FASTMATRIX_IMPL_H_
