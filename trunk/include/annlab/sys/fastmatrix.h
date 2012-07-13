
#ifndef _FASTMATRIX_H_
#define _FASTMATRIX_H_

#include "../annlab_stddef.h"
#include <list>
#include <vector>
#include <typeinfo.h>

#include <math.h>
#include <intrin.h>
#include <xmmintrin.h>
#include <emmintrin.h>

using namespace std;

#define MATRIXT_FAST_MODE       1
#define MATRIXT_BOUNDS_CHECK    1

#define MAT_NAN_ITEM            (-128.0)

#define MAT_CACHE_ALIGN_SIZE    (128UL)
#define MAT_CACHE_ALIGN_MASK    (MAT_CACHE_ALIGN_SIZE - 1)
#define MAT_CACHE_ALIGN_128(x)  (((uint32_t)(x) + MAT_CACHE_ALIGN_MASK) & (~MAT_CACHE_ALIGN_MASK))

namespace annlab {

typedef enum _enumMatFillData {
    INIT_FCN_NONE = 0,
    INIT_FCN_ZEROS,
    INIT_FCN_ONES,
    INIT_FCN_EYES,
    INIT_FCN_RANDS,
    INIT_FCN_RANDS_POSITIVE,
    INIT_FCN_SPECIFIED,
    INIT_FCN_MAX
} enumMatFillData;

typedef enum _enumMatInitType {
    INIT_TYPE_NONE = 0,
    INIT_TYPE_CONSTRUCTOR,
    INIT_TYPE_RESIZE,
    INIT_TYPE_NOT_RESERVED,     // 不保留数据
    INIT_TYPE_RESERVE_STRUCT,   // 保留数据, 且保持原矩阵元素的逻辑位置, 空白位置用指定数据填白
    INIT_TYPE_RESERVE_ORDER,    // 保留数据, 仅保持原数据在内存中的物理位置, 空白位置用指定数据填白
    INIT_TYPE_MAX
} enumMatInitType;

template<typename T>
class FastMatrix : public CAnnObject
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

    static const unsigned int INIT_FCN_DEFAULT = INIT_FCN_NONE;

private:
    pointer     pvData;
    ptr_pointer ppvCol;
    pointer     pvAlloc;
    ptr_pointer ppvColAlloc;

public:
    int rows;
    int cols;
    int totals;
    int totals_actual;

private:
    size_type   alloc_size;

public:
    FastMatrix( void );
    explicit FastMatrix( int _size );
    FastMatrix( int _rows, int _cols );
    FastMatrix( int _rows, int _cols, const value_type& _x );
    FastMatrix( int _rows, int _cols, const value_type* _array );
    FastMatrix( int _rows, int _cols, int _initFcn, value_type _fillVal = value_type(0.0) );
    FastMatrix( const TCHAR *szName, int _rows, int _cols, int _initFcn = INIT_FCN_DEFAULT,
        value_type _fillVal = value_type(0.0) );
    FastMatrix( const FastMatrix<T>& src );                                   // 拷贝构造函数
    FastMatrix( const FastMatrix<T>& src, bool b_copy_data );
    virtual ~FastMatrix( void );

    // gets
    inline pointer   get_data     ( void ) const { return pvData;  };   // 获得数据指针
    inline pointer   alloc_ptr    ( void ) const { return pvAlloc; };   // 获得原始数据指针

    inline pointer          get_rowptr ( int _row ); 
    inline const_pointer    get_rowptr ( int _row ) const;
    inline pointer          get_colptr ( int _col );
    inline const_pointer    get_colptr ( int _col ) const;

    inline size_type element_size ( void ) const { return sizeof(typename T); };
    inline size_type alloc_bytes  ( void ) const { return sizeof(typename T) * alloc_size; };

    inline size_type size         ( int n = 0 ) const;
    inline size_type sizes        ( void ) const;
    inline bool      is_empty     ( void ) const;
    inline bool      is_same_size ( const FastMatrix<T>* target, int n = 0 );
    inline bool      is_same_sizes( const FastMatrix<T>& target );

    inline value_type& get_at( int _index );                            // 获取指定元素的值
    inline value_type  get_at( int _index ) const;                      // 获取指定元素的值
    inline value_type& get_at( int _row, int _col );                    // 获取指定元素的值
    inline value_type  get_at( int _row, int _col ) const;              // 获取指定元素的值

    // sets
    inline void set_at( int _index, const value_type _value );          // 设置指定元素的值
    inline void set_at( int _row, int _col, const value_type _value );  // 设置指定元素的值

    /////////////////////////////////////////////////////////////////////////

    // references
    inline operator T*();
    inline operator const T*() const;

    // accessors
    inline pointer operator[]( int _row );
    inline const_pointer operator[]( int _row ) const;

    inline reference operator()( int _index );
    inline value_type operator()( int _index ) const;

    inline reference operator()( int _row, int _col );
    inline value_type operator()( int _row, int _col ) const;

    // operator override (重载运算符)

//  inline MatrixT<T>& operator =  ( int _value         );
    inline FastMatrix<T>& operator =  ( value_type _value  );
    inline FastMatrix<T>& operator =  ( FastMatrix<T>& _Right );
    inline bool        operator == ( FastMatrix<T>& _Right );
    inline bool        operator != ( FastMatrix<T>& _Right );

    inline FastMatrix<T>  operator +  ( value_type _value  );
    inline FastMatrix<T>  operator +  ( FastMatrix<T>& _Right );
    inline FastMatrix<T>& operator += ( value_type _value  );
    inline FastMatrix<T>& operator += ( FastMatrix<T>& _Right );

    inline FastMatrix<T>  operator -  ( value_type _value  );
    inline FastMatrix<T>  operator -  ( FastMatrix<T>& _Right );
    inline FastMatrix<T>& operator -= ( value_type _value  );
    inline FastMatrix<T>& operator -= ( FastMatrix<T>& _Right );

    inline FastMatrix<T>  operator *  ( value_type _value  );
    inline FastMatrix<T>  operator *  ( FastMatrix<T>& _Right );
    inline FastMatrix<T>& operator *= ( value_type _value  );
    inline FastMatrix<T>& operator *= ( FastMatrix<T>& _Right );

    inline FastMatrix<T>  operator /  ( value_type _value  );
    inline FastMatrix<T>  operator /  ( FastMatrix<T>& _Right );
    inline FastMatrix<T>& operator /= ( value_type _value  );

    inline FastMatrix<T>  operator ^  ( value_type _value  );
    inline FastMatrix<T>& operator ^= ( value_type _value  );

    /*
    friend inline MatrixT<T> operator + ( value_type _value, MatrixT<T>& _Right );
    friend inline MatrixT<T> operator - ( value_type _value, MatrixT<T>& _Right );
    friend inline MatrixT<T> operator * ( value_type _value, MatrixT<T>& _Right );
    friend inline MatrixT<T> operator / ( value_type _value, MatrixT<T>& _Right );

    friend inline ostream& operator << (ostream& out, const MatrixT<T>& _matrix);
    friend inline istream& operator >> (istream& in, MatrixT<T>& _matrix);
    //*/

    /////////////////////////////////////////////////////////////////////////

    // methods
    inline void free     ( void );
    inline void fill     ( int _initFcn = INIT_FCN_DEFAULT,
                    value_type _fillVal = value_type(0.0) );
    inline int  resize   ( int _rows, int _cols );
    inline int  resize_ex( int _rows, int _cols, int _initFcn = INIT_FCN_DEFAULT,
                        value_type _fillVal = value_type(0.0) );

    // copy()完整复制结构以及数据
    inline FastMatrix<T>* copy ( const FastMatrix<T>* src );
    // clone()仅复制结构和分配内存, 不复制数据
    inline FastMatrix<T>* clone( const FastMatrix<T>* src );

    inline void copy_from_array ( const_pointer _array );
    inline void set_by_scalar   ( const_value_type _x );
    
    inline void copy_data(pointer dest, const_pointer src, int _totals);
    inline void copy_big_data(pointer dest, const_pointer src, int _totals);

    inline size_type copy_from_array_s( const_pointer _array, size_type _size );

    void display( void );
    void display( const TCHAR *szText );

    void display_ex( void );
    void display_ex( const TCHAR *szText );

    // 矩阵的转置
    inline FastMatrix<T>& transpose( void );

    inline int zeros  ( int _rows, int _cols );
    inline int ones   ( int _rows, int _cols );
    inline int rands  ( int _rows, int _cols );
    inline int rands2 ( int _rows, int _cols );

    static inline FastMatrix<T> _zeros  ( int _rows, int _cols );
    static inline FastMatrix<T> _ones   ( int _rows, int _cols );
    static inline FastMatrix<T> _rands  ( int _rows, int _cols );
    static inline FastMatrix<T> _rands2 ( int _rows, int _cols );

    /////////////////////////////////////////////////////////////////////////
    // get part of matrix

    inline void copy_row( const FastMatrix<T>& src, int _row );
    inline void copy_row( const FastMatrix<T>& src, int _row, int r_step, int _start_col, int c_step );

    // 获取矩阵的指定行矩阵
    inline int get_row_vector( int _row, double* pVector ) const;
    // 获取矩阵的指定列矩阵
    inline int get_col_vector( int _col, double* pVector ) const;

    // 获取矩阵的指定行矩阵
    inline int get_row_vector( int _row, FastMatrix<T>* pMatrix ) const;
    // 获取矩阵的指定列矩阵
    inline int get_col_vector( int _col, FastMatrix<T>* pMatrix ) const;

    // 把第x行至第y行的数据转换成为新的矩阵
    inline int get_row_vector( int _rowStart, int _rowEnd, FastMatrix<T>* pMatrix ) const;
    // 把第x列至第y列的数据转换成为新的矩阵
    inline int get_col_vector( int _colStart, int _colEnd, FastMatrix<T>* pMatrix ) const;

    // 把第x行至第y行的第m列至第n列的数据转换成为新的矩阵
    inline int get_partof_matrix( int _rowStart, int _rowEnd,
                int _colStart, int _colEnd, FastMatrix<T>* pMatrix ) const;

    inline void CalcArrayProducts_double_SSE2( pointer dest, pointer src,
        pointer alpha, unsigned int len );

protected:
    // methods
    inline void destroy( void );
    inline void destroy_data( void );

    inline void initialize( int _rows, int _cols, int _initFcn = INIT_FCN_DEFAULT,
            value_type _fillVal = value_type(0.0) );
    inline void initialize_ex( const TCHAR *szName, int _rows, int _cols,
            int _initFcn = INIT_FCN_DEFAULT,
            value_type _fillVal = value_type(0.0) );

    inline void init_martix( int _rows, int _cols, int _initMode = INIT_TYPE_NONE,
            int _initFcn = INIT_FCN_DEFAULT,
            value_type _fillVal = value_type(0.0) );
    inline void fill_data( int _rows, int _cols,
            int _initFcn = INIT_FCN_DEFAULT,
            value_type _fillVal = value_type(0.0) );
    inline void reserve_and_fill_data( pointer pvNewData, int _rows, int _cols,
            int _initFcn = INIT_FCN_DEFAULT,
            value_type _fillVal = value_type(0.0) );

private:
    //
};

#define _FASTMATRIX_IMPL_internal_H_
#include "fastmatrix_impl.h"
#undef  _FASTMATRIX_IMPL_internal_H_

}  // namespace annlab

#endif  // _FASTMATRIX_H_
