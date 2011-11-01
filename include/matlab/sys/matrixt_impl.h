
#if !defined(_MATRIXT_IMPL_internal_H_)
#error Do not #include this internal file directly; use matrixt.h header file instead.
#endif

#ifndef _MATRIXT_IMPL_H_
#define _MATRIXT_IMPL_H_

#ifndef _MATRIXT_IMPL_internal_H_
namespace matlab {
#endif

template<typename T>
MatrixT<T>::MatrixT( void )
{

}

template<typename T>
MatrixT<T>::MatrixT( int _size )
{

}

template<typename T>
MatrixT<T>::MatrixT( const TCHAR *szName, int _rows, int _cols, int _initFcn /*= MAT_INIT_NONE */ )
{

}

template<typename T>
MatrixT<T>::MatrixT( const MatrixT<T>& src )
{

}

template<typename T>
MatrixT<T>::MatrixT( const MatrixT<T>& src, bool b_copy_data )
{

}

template<typename T>
MatrixT<T>::MatrixT( int _rows, int _cols, int _initFcn /*= MAT_INIT_NONE */ )
{

}

template<typename T>
MatrixT<T>::~MatrixT( void )
{
    destroy();
}

template<typename T>
void MatrixT<T>::destroy( void )
{

}

template<typename T>
inline bool MatrixT<T>::empty( void ) const
{
	return (sizes() == 0);
}

template<typename T>
inline void MatrixT<T>::clear( void )
{

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
int MatrixT<T>::resize( int _rows, int _cols,
                       double _fillVal /*= 0.0*/,
                       int _initFcn /*= MAT_INIT_DEFAULT */ )
{

}

template<typename T>
void MatrixT<T>::display( void )
{

}

template<typename T>
void MatrixT<T>::display( const TCHAR *szText )
{

}

template<typename T>
MatrixT<T>& MatrixT<T>::transpose( void )
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
			    set_element(j, i, get_element(i, j));
	    }
    }
    else {
	    MatrixT<T> _trans((MatrixT<T> &)*this);
	    // 转置各元素
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
int MatrixT<T>::zeros( int _rows, int _cols )
{

}

template<typename T>
int MatrixT<T>::ones( int _rows, int _cols )
{

}

template<typename T>
int MatrixT<T>::rands( int _rows, int _cols )
{

}

template<typename T>
int MatrixT<T>::rands2( int _rows, int _cols )
{

}

template<typename T>
MatrixT<T> MatrixT<T>::_zeros( int _rows, int _cols ) const
{

}

template<typename T>
MatrixT<T> MatrixT<T>::_ones( int _rows, int _cols ) const
{

}

template<typename T>
MatrixT<T> MatrixT<T>::_rands( int _rows, int _cols ) const
{

}

template<typename T>
MatrixT<T> MatrixT<T>::_rands2( int _rows, int _cols ) const
{

}

#ifndef _MATRIXT_IMPL_internal_H_
}  // namespace matlab
#endif

#endif  // _MATRIXT_IMPL_H_
