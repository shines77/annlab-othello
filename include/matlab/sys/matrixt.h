
#ifndef _MATRIXT_H_
#define _MATRIXT_H_

#include "../matlab_stddef.h"
#include <list>
#include <vector>
#include <typeinfo.h>

using namespace std;

#define MATRIXT_BOUNDS_CHECK    1

#define MAT_NAN_ITEM            (-128.0)

#define MAT_ADDR_ALIGN_SIZE     (128UL)
#define MAT_ADDR_ALIGN_MASK     (MAT_ADDR_ALIGN_SIZE - 1)
#define MAT_ADDR_ALIGN(x)       (((uint32_t)(x) + MAT_ADDR_ALIGN_MASK) & (~MAT_ADDR_ALIGN_MASK))

namespace matlab {

template<typename T>
class MatrixT : public CAnnObject
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

    typedef enum _enumMatFillMode {
        FILL_MODE_NONE = 0,
        FILL_MODE_ZEROS,
        FILL_MODE_ONES,
        FILL_MODE_EYES,
        FILL_MODE_RANDS,
        FILL_MODE_RANDS_POSITIVE,
        FILL_MODE_MAX
    } enumMatFillMode;

    typedef enum _enumMatInitMode {
        INIT_MODE_NONE = 0,
        INIT_MODE_CONSTRUCTOR,
        INIT_MODE_RESIZE,
        INIT_MODE_NOT_RESERVED,     // ����������
        INIT_MODE_RESERVE_STRUCT,   // ��������, �ұ���ԭ����Ԫ�ص��߼�λ��, �հ�λ����ָ���������
        INIT_MODE_RESERVE_ORDER,    // ��������, ������ԭ�������ڴ��е�����λ��, �հ�λ����ָ���������
        INIT_MODE_MAX
    } enumMatInitMode;

    static const unsigned int MAT_INIT_DEFAULT = FILL_MODE_NONE;

private:
    pointer     pvData;
    ptr_pointer ppvRow;
    pointer     pvAlloc;
    ptr_pointer ppvRowAlloc;

public:
    int rows;
    int cols;
    int totals;
    int totals_actual;

private:
    size_type   alloc_size;

public:
    MatrixT( void );
    explicit MatrixT( int _size );
    MatrixT( int _rows, int _cols, int _initFcn = FILL_MODE_NONE );
    MatrixT( const TCHAR *szName, int _rows, int _cols, int _initFcn = FILL_MODE_NONE );
    MatrixT( int _rows, int _cols, const value_type& _x );
    MatrixT( int _rows, int _cols, const value_type* _array );
    MatrixT( const MatrixT<T>& src );                                   // �������캯��
    MatrixT( const MatrixT<T>& src, bool b_copy_data );
    virtual ~MatrixT( void );

    // gets
    inline pointer   data      ( void ) const { return pvData;  };      // �������ָ��
    inline pointer   alloc_ptr ( void ) const { return pvAlloc; };      // ���ԭʼ����ָ��
    inline size_type element_size ( void ) const { return sizeof(typename T); };
    inline size_type alloc_bytes  ( void ) const { return sizeof(typename T) * alloc_size; };

    size_type   size         ( int n = 0 ) const;
    size_type   sizes        ( void ) const;
    bool        is_same_size ( const MatrixT<T>* target, int n = 0 );
    bool        is_same_sizes( const MatrixT<T>& target );

    value_type get_element( int _index );                              // ��ȡָ��Ԫ�ص�ֵ
    const value_type get_element( int _index ) const;                  // ��ȡָ��Ԫ�ص�ֵ
    value_type get_element( int _row, int _col );                      // ��ȡָ��Ԫ�ص�ֵ
    const value_type get_element( int _row, int _col ) const;          // ��ȡָ��Ԫ�ص�ֵ

    // sets
    void        set_element  ( int _index, value_type _value );         // ����ָ��Ԫ�ص�ֵ
    void        set_element  ( int _row, int _col, value_type _value ); // ����ָ��Ԫ�ص�ֵ

    /////////////////////////////////////////////////////////////////////////

    // references
    operator T*();
    operator const T*() const;

    // accessors
    pointer operator[]( int _row );
    const_pointer operator[]( int _row ) const;

    reference operator()( int _index );
    const_reference operator()( int _index ) const;
    reference operator()( int _row, int _col );
    const_reference operator()( int _row, int _col ) const;

    // operator override (���������)

    MatrixT<T>& operator =  ( MatrixT<T>& _Right );
//  MatrixT<T>& operator =  ( int _value         );
    MatrixT<T>& operator =  ( value_type _value  );
    bool        operator == ( MatrixT<T>& _Right );
    bool        operator != ( MatrixT<T>& _Right );

    MatrixT<T>  operator +  ( value_type _value  );
    MatrixT<T>  operator +  ( MatrixT<T>& _Right );
    MatrixT<T>& operator += ( value_type _value  );
    MatrixT<T>& operator += ( MatrixT<T>& _Right );

    MatrixT<T>  operator -  ( value_type _value  );
    MatrixT<T>  operator -  ( MatrixT<T>& _Right );
    MatrixT<T>& operator -= ( value_type _value  );
    MatrixT<T>& operator -= ( MatrixT<T>& _Right );

    MatrixT<T>  operator *  ( value_type _value  );
    MatrixT<T>  operator *  ( MatrixT<T>& _Right );
    MatrixT<T>& operator *= ( value_type _value  );
    MatrixT<T>& operator *= ( MatrixT<T>& _Right );

    MatrixT<T>  operator /  ( value_type _value  );
    MatrixT<T>  operator /  ( MatrixT<T>& _Right );
    MatrixT<T>& operator /= ( value_type _value  );

    MatrixT<T>  operator ^  ( value_type _value  );
    MatrixT<T>& operator ^= ( value_type _value  );

    friend MatrixT<T> operator + ( value_type _value, MatrixT<T>& _Right );
    friend MatrixT<T> operator - ( value_type _value, MatrixT<T>& _Right );
    friend MatrixT<T> operator * ( value_type _value, MatrixT<T>& _Right );
    friend MatrixT<T> operator / ( value_type _value, MatrixT<T>& _Right );

    friend ostream& operator << (ostream& out, const MatrixT<T>& _matrix);
    friend istream& operator >> (istream& in, MatrixT<T>& _matrix);

    /////////////////////////////////////////////////////////////////////////

    // methods
    void free     ( void );
    bool empty    ( void ) const;
    void clear    ( value_type _fillVal = static_cast<T>(0),
                    int _initFcn = MAT_INIT_DEFAULT );
    int  resize   ( int _rows, int _cols );
    int  resize_ex( int _rows, int _cols, value_type _fillVal = static_cast<T>(0),
                    int _initFcn = MAT_INIT_DEFAULT );

    // copy()�������ƽṹ�Լ�����
    MatrixT<T>* copy ( const MatrixT<T>* src );
    // clone()�����ƽṹ�ͷ����ڴ�, ����������
    MatrixT<T>* clone( const MatrixT<T>* src );

    void copy_from_array ( const_pointer _array );
    void set_by_scalar   ( const_value_type _x );

    size_type copy_from_array_s( const_pointer _array, size_type _size );

    void display( void );
    void display( const TCHAR *szText );

    void display_ex( void );
    void display_ex( const TCHAR *szText );

    // �����ת��
    MatrixT<T>& transpose( void );

    int zeros  ( int _rows, int _cols );
    int ones   ( int _rows, int _cols );
    int rands  ( int _rows, int _cols );
    int rands2 ( int _rows, int _cols );

    MatrixT<T> _zeros  ( int _rows, int _cols ) const;
    MatrixT<T> _ones   ( int _rows, int _cols ) const;
    MatrixT<T> _rands  ( int _rows, int _cols ) const;
    MatrixT<T> _rands2 ( int _rows, int _cols ) const;

    /////////////////////////////////////////////////////////////////////////
    // get part of matrix

    // ��ȡ�����ָ���о���
    int get_row_vector( int _row, double* pVector ) const;
    // ��ȡ�����ָ���о���
    int get_col_vector( int _col, double* pVector ) const;

    // ��ȡ�����ָ���о���
    int get_row_vector( int _row, MatrixT<T>* pMatrix ) const;
    // ��ȡ�����ָ���о���
    int get_col_vector( int _col, MatrixT<T>* pMatrix ) const;

    // �ѵ�x������y�е�����ת����Ϊ�µľ���
    int get_row_vector( int _rowStart, int _rowEnd, MatrixT<T>* pMatrix ) const;
    // �ѵ�x������y�е�����ת����Ϊ�µľ���
    int get_col_vector( int _colStart, int _colEnd, MatrixT<T>* pMatrix ) const;

    // �ѵ�x������y�еĵ�m������n�е�����ת����Ϊ�µľ���
    int get_partof_matrix( int _rowStart, int _rowEnd,
            int _colStart, int _colEnd, MatrixT<T>* pMatrix ) const;

protected:
    // methods
    void destroy( void );

    void initialize(int _rows, int _cols, int _initMode = INIT_MODE_NONE,
        value_type _fillVal = static_cast<T>(0),
        int _initFcn = MAT_INIT_DEFAULT);
    void initialize_ex(const TCHAR *szName, int _rows, int _cols,
        int _initMode = INIT_MODE_NONE,
        value_type _fillVal = static_cast<T>(0),
        int _initFcn = MAT_INIT_DEFAULT);

    void init_martix(int _rows, int _cols, int _initMode = INIT_MODE_NONE,
        value_type _fillVal = static_cast<T>(0),
        int _initFcn = MAT_INIT_DEFAULT );
    void fill_data( int _rows, int _cols,
        value_type _fillVal = static_cast<T>(0),
        int _initFcn = MAT_INIT_DEFAULT );
    void reserve_and_fill_data( pointer pvNewData, int _rows, int _cols,
        value_type _fillVal = static_cast<T>(0),
        int _initFcn = MAT_INIT_DEFAULT );

private:
    //
};

#define _MATRIXT_IMPL_internal_H_
#include "matrixt_impl.h"
#undef  _MATRIXT_IMPL_internal_H_

}  // namespace matlab

#endif  // _MATRIXT_H_
