#pragma once

#include "../annlab_stddef.h"
#include "AnnObject.h"
#include "AnnList.h"
#include <list>
#include <vector>

using namespace std;

#define MAT_NAN_ITEM            (-128.0)

#define MAT_CACHE_ALIGN_SIZE    (128UL)
#define MAT_CACHE_ALIGN_MASK    (MAT_CACHE_ALIGN_SIZE - 1)
#define MAT_CACHE_ALIGN_128(x)  (((uint32_t)(x) + MAT_CACHE_ALIGN_MASK) & (~MAT_CACHE_ALIGN_MASK))

namespace annlab {

enum enumMatrixInit {
	MAT_INIT_NONE = 0,
	MAT_INIT_ZEROS,
	MAT_INIT_ONES,
	MAT_INIT_EYES,
	MAT_INIT_RANDS,
	MAT_INIT_RANDS_POSITIVE,
	MAT_INIT_MAX
};

enum enumMatInitMode {
	INIT_MODE_NONE = 0,
	INIT_MODE_CONSTRUCTOR,
	INIT_MODE_RESIZE,
	INIT_MODE_MAX
};

class CAnnVector : public CAnnObject
{
public:
	CAnnVector( void );
	CAnnVector( int _size );
	CAnnVector( CAnnVector & srcVerctor );
	virtual ~CAnnVector( void );

public:
	// property
	int length;
	int mem_alloc;

	// gets
	double operator [] ( int _index ) const;
	INLINE int size( void ) const { return length; };

	//sets

	// methos
	CAnnVector *copy( const CAnnVector *srcVector );

protected:
	void freeVector( void );
	bool initVector( const TCHAR *szName, int _size, int _initMode = INIT_MODE_NONE,
		int _initFcn = MAT_INIT_NONE );
	bool initData( int _size, int _initFcn = MAT_INIT_NONE );

private:
	double *m_pOrigData;
	double *m_pData;
};

class CAnnVectors : public CAnnObject
{
public:
	CAnnVectors( void );
	virtual ~CAnnVectors( void );

public:
	// property
	int length;

	// gets
	INLINE int size( void ) const { return length; };

	// sets

protected:
	void freeVector( void );
};

class CAnnMatrix : public CAnnObject
{
public:
	CAnnMatrix( void );
	explicit CAnnMatrix( int _size );
	CAnnMatrix( int _rows, int _cols, int _initFcn = MAT_INIT_NONE );
	CAnnMatrix( const TCHAR *szName, int _rows, int _cols, int _initFcn = MAT_INIT_NONE );
	CAnnMatrix( const CAnnMatrix & scrMatrix );			            // �������캯��
	CAnnMatrix( const CAnnMatrix & scrMatrix, bool bCopyData );
	virtual ~CAnnMatrix( void );

public:
    static const unsigned int MAT_INIT_DEFAULT = MAT_INIT_NONE;

	// gets
	int    size         ( int n = 0 ) const;
	bool   is_same_size ( const CAnnMatrix *target, int n = 0 );
	double get_element  ( int _index ) const;						// ��ȡָ��Ԫ�ص�ֵ
	double get_element  ( int _row, int _col ) const;				// ��ȡָ��Ԫ�ص�ֵ

	// ��ȡ�����ָ���о���
	int    get_row_vector( int _row, double* pVector ) const;
	// ��ȡ�����ָ���о���
	int    get_col_vector( int _col, double* pVector ) const;

	// ��ȡ�����ָ���о���
	int    get_row_vector( int _row, CAnnMatrix* pMatrix ) const;
	// ��ȡ�����ָ���о���
	int    get_col_vector( int _col, CAnnMatrix* pMatrix ) const;

	// �ѵ�x������y�е�����ת����Ϊ�µľ���
	int    get_row_vector( int _rowStart, int _rowEnd, CAnnMatrix* pMatrix ) const;
	// �ѵ�x������y�е�����ת����Ϊ�µľ���
	int    get_col_vector( int _colStart, int _colEnd, CAnnMatrix* pMatrix ) const;

	/* �ѵ�x������y�еĵ�m������n�е�����ת����Ϊ�µľ��� */
	int    get_partof_matrix( int _rowStart, int _rowEnd,
                            int _colStart, int _colEnd, CAnnMatrix* pMatrix ) const;

	// ��ȡ�����ָ���о���
	CAnnMatrix get_row_vector( int _row ) const;
	// ��ȡ�����ָ���о���
	CAnnMatrix get_col_vector( int _col ) const;

	double *get_data     ( void ) const { return m_pData;     };	// �������ָ��
	double *get_orig_data( void ) const { return m_pOrigData; };	// ���ԭʼ����ָ��

	// sets
	bool   set_element  ( int _row, int _col, double _value );		// ����ָ��Ԫ�ص�ֵ
	bool   set_element  ( int _index, double _value );				// ����ָ��Ԫ�ص�ֵ
	double *set_data    ( double *pBuffer, int _length );			// �ӻ�����д������
	double *set_data    ( double *pBuffer, int _rows, int _cols );	// �ӻ�����д������

	// property
	int rows, cols;
	int length;
	int malloc_size;

	// operator(���������)
	double        operator () ( int _index );
	double        operator () ( int _row, int _col );
	double        operator [] ( int _index );

	CAnnMatrix &  operator =  ( CAnnMatrix & _Right );
	CAnnMatrix &  operator =  ( int _Right        );
	CAnnMatrix &  operator =  ( double _Right     );
	bool          operator == ( CAnnMatrix & _Right );
	bool          operator != ( CAnnMatrix & _Right );
	CAnnMatrix    operator +  ( double _value     );
	CAnnMatrix    operator +  ( CAnnMatrix & _Right );
	CAnnMatrix &  operator += ( double _value     );
	CAnnMatrix &  operator += ( CAnnMatrix & _Right );
	CAnnMatrix    operator -  ( double _value     );
	CAnnMatrix    operator -  ( CAnnMatrix & _Right );
	CAnnMatrix &  operator -= ( double _value     );
	CAnnMatrix &  operator -= ( CAnnMatrix & _Right );
	CAnnMatrix    operator *  ( double _value     );
	CAnnMatrix    operator *  ( CAnnMatrix & _Right );
	CAnnMatrix &  operator *= ( double _value     );
	CAnnMatrix &  operator *= ( CAnnMatrix & _Right );
	CAnnMatrix    operator /  ( double _value     );
	CAnnMatrix    operator /  ( CAnnMatrix &_Right );
	CAnnMatrix &  operator /= ( double _value     );
	CAnnMatrix    operator ^  ( double _value     );
	CAnnMatrix &  operator ^= ( double _value     );

	friend CAnnMatrix operator + ( double _value, CAnnMatrix & _Right );
	friend CAnnMatrix operator - ( double _value, CAnnMatrix & _Right );
	friend CAnnMatrix operator * ( double _value, CAnnMatrix & _Right );
	friend CAnnMatrix operator / ( double _value, CAnnMatrix & _Right );

    friend ostream & operator << (ostream & ios, const CAnnMatrix & _matrix);
    friend istream & operator >> (istream & ios, CAnnMatrix & _matrix);

	// methods
	void free_matrix( void );
	bool init_matrix( const TCHAR *szName, int _rows, int _cols, int _initMode = INIT_MODE_NONE,
		double _fillVal = 0.0, int _initFcn = MAT_INIT_DEFAULT );
	bool init_data  ( int _rows, int _cols, double _fillVal = 0.0, int _initFcn = MAT_INIT_DEFAULT );
	bool copy_data  ( double *pNewData, int _rows, int _cols, double _fillVal = 0.0,
		int _initFcn = MAT_INIT_DEFAULT );

	bool create   ( int _rows, int _cols, double _fillVal = 0.0, int _initFcn = MAT_INIT_DEFAULT );
	bool create_ex( const TCHAR *szName, int _rows, int _cols, double _fillVal = 0.0,
		int _initFcn = MAT_INIT_DEFAULT );

	CAnnMatrix * copy ( const CAnnMatrix *srcMartix );
	CAnnMatrix * clone( const CAnnMatrix *srcMartix );

	void clear ( void );
	int  resize( int _rows, int _cols, double _fillVal = 0.0, int _initFcn = MAT_INIT_DEFAULT );
	bool empty ( void ) const;
	bool make_unit_matrix( int _size );

	void display( void );
	void display( const TCHAR *szName );

	// �����ת��
	CAnnMatrix & transpose( void );

	int zeros  ( int _rows, int _cols );
	int ones   ( int _rows, int _cols );
	int rands  ( int _rows, int _cols );
	int rands2 ( int _rows, int _cols );

	CAnnMatrix _zeros  ( int _rows, int _cols ) const;
	CAnnMatrix _ones   ( int _rows, int _cols ) const;
	CAnnMatrix _rands  ( int _rows, int _cols ) const;
	CAnnMatrix _rands2 ( int _rows, int _cols ) const;

private:
	double *m_pOrigData;
	double *m_pData;
};

class CAnnMatrixs : public CAnnObject
{
public:
	CAnnMatrixs( void );
	explicit CAnnMatrixs( int _numMatrixs );
	CAnnMatrixs( const TCHAR *szName, int _numMatrixs );
	virtual ~CAnnMatrixs( void );

public:
	// gets
	const CAnnMatrix & operator [] ( int _index ) const;
	CAnnMatrix & matrixs( int _index ) const;

	// sets
	bool set_matrixs( int _numMatrixs );

	// property
	int numMatrixs;

	// methods
	void free_matrixs( void );
	bool init_matrixs( const TCHAR *szName, int _numMatrixs );

	bool create( const TCHAR *szName, int _numMatrixs );

	CAnnMatrixs * copy ( const CAnnMatrixs *srcMartixs );
	CAnnMatrixs * clone( const CAnnMatrixs *srcMartixs );

private:
	CAnnMatrix *pMatrixs;
};

class CAnnMatrixVector : public vector<CAnnMatrix>, public CAnnObject
{
public:
	CAnnMatrixVector( void );
	virtual ~CAnnMatrixVector( void );
};

class CAnnMatrixList : public list<CAnnMatrix>, public CAnnObject
{
public:
	CAnnMatrixList( void );
	virtual ~CAnnMatrixList( void );

public:
	int N, TS, Q;
	double *X, *T, *indices;
	double *Xi, *Pd, *Tl, *Ai;
	double *Y;

public:
	CAnnMatrix * operator [] ( int _index );
};

class CAnnXArray : public CAnnObject
{
public:
	CAnnXArray( void );
	virtual ~CAnnXArray( void );

public:
	int length, malloc_size;
	double *items;

public:
	double operator [] ( int _index );

	void free   ( void );
	void clear  ( void );
	int  resize ( int _size );

protected:
	//
};

/*
CxMatrix operator + ( double _value, CxMatrix & _Right );
CxMatrix operator - ( double _value, CxMatrix & _Right );
CxMatrix operator * ( double _value, CxMatrix & _Right );
CxMatrix operator / ( double _value, CxMatrix & _Right );
//*/

//typedef CAnnList<CAnnMatrix> CAnnMatrixList;

}  // namespace annlab
