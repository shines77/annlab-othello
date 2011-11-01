
#ifndef _MATRIXT_H_
#define _MATRIXT_H_

#include "../matlab_stddef.h"
#include <list>
#include <vector>

using namespace std;

#define MAT_NAN_ITEM         (-128.0)

#define MAT_ADDR_ALIGN_SIZE  (64UL)
#define MAT_ADDR_ALIGN_MASK  (MAT_ADDR_ALIGN_SIZE - 1)
#define MAT_ADDR_ALIGN(x)    (((uint32_t)(x) + MAT_ADDR_ALIGN_MASK) & (~MAT_ADDR_ALIGN_MASK))

namespace matlab {
//
template<typename T>
class MatrixT
{
public:
    typedef T value_type;
    typedef const value_type const_value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type** ptr_pointer;
    typedef const value_type** const_ptr_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    enum enumMatrixInit {
	    MAT_INIT_NONE = 0,
	    MAT_INIT_ZEROS,
	    MAT_INIT_ONES,
	    MAT_INIT_EYES,
	    MAT_INIT_RANDS,
	    MAT_INIT_RANDS2,
	    MAT_INIT_MAX
    };

    enum enumMatInitMode {
	    INIT_MODE_NONE = 0,
	    INIT_MODE_FIRST,
	    INIT_MODE_RESIZE,
	    INIT_MODE_MAX
    };

private:
    pointer m_pvData;
    pointer m_pvOrigPtr;
    int     alloc_size;

public:
	// property
	int rows;
    int cols;
	int totals;

public:
	MatrixT( void );
	explicit MatrixT( int _size );
	MatrixT( int _rows, int _cols, int _initFcn = MAT_INIT_NONE );
	MatrixT( const TCHAR *szName, int _rows, int _cols, int _initFcn = MAT_INIT_NONE );
	MatrixT( const MatrixT<T>& src );			            // 拷贝构造函数
	MatrixT( const MatrixT<T>& src, bool b_copy_data );
	virtual ~MatrixT( void );

    // gets
    size_type   size         ( int n = 0 ) const;
    size_type   sizes        ( void ) const;
	bool        is_same_size ( const MatrixT<T>* target, int n = 0 );
    bool        is_same_sizes   ( const MatrixT<T>& target );
    value_type  get_element  ( int _index ) const;                      // 获取指定元素的值
    value_type  get_element  ( int _row, int _col ) const;              // 获取指定元素的值

    // sets
    bool        set_element  ( int _row, int _col, double _value );     // 设置指定元素的值
    bool        set_element  ( int _index, double _value );             // 设置指定元素的值

    /////////////////////////////////////////////////////////////////////////
    // gets partof matrix

	// 获取矩阵的指定行矩阵
	int    get_row_vector( int _row, double* pVector ) const;
	// 获取矩阵的指定列矩阵
	int    get_col_vector( int _col, double* pVector ) const;

	// 获取矩阵的指定行矩阵
	int    get_row_vector( int _row, MatrixT<T>* pMatrix ) const;
	// 获取矩阵的指定列矩阵
	int    get_col_vector( int _col, MatrixT<T>* pMatrix ) const;

	// 把第x行至第y行的数据转换成为新的矩阵
	int    get_row_vector( int _rowStart, int _rowEnd, MatrixT<T>* pMatrix ) const;
	// 把第x列至第y列的数据转换成为新的矩阵
	int    get_col_vector( int _colStart, int _colEnd, MatrixT<T>* pMatrix ) const;

	/* 把第x行至第y行的第m列至第n列的数据转换成为新的矩阵 */
	int    get_partof_matrix( int _rowStart, int _rowEnd,
                            int _colStart, int _colEnd, MatrixT<T>* pMatrix ) const;

    /////////////////////////////////////////////////////////////////////////

    // operator (重载运算符)
    value_type    operator () ( int _index );
    value_type    operator () ( int _row, int _col );
    value_type    operator [] ( int _index );

    MatrixT<T>&   operator =  ( MatrixT<T>& _Right );
    MatrixT<T>&   operator =  ( int _Right         );
    MatrixT<T>&   operator =  ( value_type _Right  );
    bool          operator == ( MatrixT<T>& _Right );
    bool          operator != ( MatrixT<T>& _Right );
    MatrixT<T>    operator +  ( value_type _value  );
    MatrixT<T>    operator +  ( MatrixT<T>& _Right );
    MatrixT<T>&   operator += ( value_type _value  );
    MatrixT<T>&   operator += ( MatrixT<T>& _Right );
    MatrixT<T>    operator -  ( value_type _value  );
    MatrixT<T>    operator -  ( MatrixT<T>& _Right );
    MatrixT<T>&   operator -= ( value_type _value  );
    MatrixT<T>&   operator -= ( MatrixT<T>& _Right );
    MatrixT<T>    operator *  ( value_type _value  );
    MatrixT<T>    operator *  ( MatrixT<T>& _Right );
    MatrixT<T>&   operator *= ( value_type _value  );
    MatrixT<T>&   operator *= ( MatrixT<T>& _Right );
    MatrixT<T>    operator /  ( value_type _value  );
    MatrixT<T>    operator /  ( MatrixT<T>& _Right );
    MatrixT<T>&   operator /= ( value_type _value  );
    MatrixT<T>    operator ^  ( value_type _value  );
    MatrixT<T>&   operator ^= ( value_type _value  );

    friend MatrixT<T> operator + ( double _value, MatrixT<T>& _Right );
    friend MatrixT<T> operator - ( double _value, MatrixT<T>& _Right );
    friend MatrixT<T> operator * ( double _value, MatrixT<T>& _Right );
    friend MatrixT<T> operator / ( double _value, MatrixT<T>& _Right );

    friend ostream& operator << (ostream& ios, const MatrixT<T>& _matrix);
    friend istream& operator >> (istream& ios, MatrixT<T>& _matrix);

    // methods
    void destroy( void );
	void clear  ( void );
	int  resize ( int _rows, int _cols, double _fillVal = 0.0, int _initFcn = MAT_INIT_DEFAULT );
	bool empty  ( void ) const;

	void display( void );
	void display( const TCHAR *szText );

	// 矩阵的转置
	MatrixT<T>& transpose( void );

	int zeros  ( int _rows, int _cols );
	int ones   ( int _rows, int _cols );
	int rands  ( int _rows, int _cols );
	int rands2 ( int _rows, int _cols );

	MatrixT<T> _zeros  ( int _rows, int _cols ) const;
	MatrixT<T> _ones   ( int _rows, int _cols ) const;
	MatrixT<T> _rands  ( int _rows, int _cols ) const;
	MatrixT<T> _rands2 ( int _rows, int _cols ) const;
};

#define _MATRIXT_IMPL_internal_H_
#include "matrixt_impl.h"
#undef  _MATRIXT_IMPL_internal_H_
    
}  // namespace matlab

#endif  // _MATRIXT_H_
