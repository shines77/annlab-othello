#pragma once

#include "ann_stddef.h"
#include "AnnObject.h"
#include "AnnList.h"
#include <list>
#include <vector>

using namespace std;

#define MAT_NAN_ITEM         (-128.0)

#define MET_ADDR_ALIGN_SIZE  (64UL)
#define MET_ADDR_ALIGN_MASK  (MET_ADDR_ALIGN_SIZE - 1)
#define MET_ADDR_ALIGN(x)    (((DWORD)(x) + MET_ADDR_ALIGN_MASK) & (~MET_ADDR_ALIGN_MASK))

enum enumMatrixInit {
	MAT_INIT_NONE,
	MAT_INIT_ZEROS,
	MAT_INIT_ONES,
	MAT_INIT_EYES,
	MAT_INIT_RANDS,
	MAT_INIT_RANDS2,
	MAT_INIT_MAX
};

enum enumMatInitMode {
	INIT_MODE_NONE,
	INIT_MODE_FIRST,
	INIT_MODE_RESIZE,
	INIT_MODE_MAX
};

#define MAT_INIT_DEFAULT    MAT_INIT_NONE

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
	CAnnMatrix( const CAnnMatrix & scrMatrix );								// 拷贝构造函数
	CAnnMatrix( const CAnnMatrix & scrMatrix, bool bCopyData );
	virtual ~CAnnMatrix( void );

public:
	// gets
	int    size        ( int n = 0 ) const;
	bool   sameSize    ( const CAnnMatrix *target, int n = 0 );
	double getElement  ( int _index ) const;						// 获取指定元素的值
	double getElement  ( int _row, int _col ) const;				// 获取指定元素的值

	// 获取矩阵的指定行矩阵
	int    getRowVector( int _row, double* pVector ) const;
	// 获取矩阵的指定列矩阵
	int    getColVector( int _col, double* pVector ) const;

	// 获取矩阵的指定行矩阵
	int    getRowVector( int _row, CAnnMatrix* pMatrix ) const;
	// 获取矩阵的指定列矩阵
	int    getColVector( int _col, CAnnMatrix* pMatrix ) const;

	// 把第x行至第y行的数据转换成为新的矩阵
	int    getRowVector( int _rowStart, int _rowEnd, CAnnMatrix* pMatrix ) const;
	// 把第x列至第y列的数据转换成为新的矩阵
	int    getColVector( int _colStart, int _colEnd, CAnnMatrix* pMatrix ) const;

	/* 把第x行至第y行的第m列至第n列的数据转换成为新的矩阵 */
	int    getPartOfMatrix( int _rowStart, int _rowEnd,
							int _colStart, int _colEnd, CAnnMatrix* pMatrix ) const;

	// 获取矩阵的指定行矩阵
	CAnnMatrix getRowVector( int _row ) const;
	// 获取矩阵的指定列矩阵
	CAnnMatrix getColVector( int _col ) const;

	double *get_data     ( void ) const { return m_pData;     };	// 获得数据指针
	double *get_orig_data( void ) const { return m_pOrigData; };	// 获得原始数据指针

	// sets
	bool   setElement  ( int _row, int _col, double _value );		// 设置指定元素的值
	bool   setElement  ( int _index, double _value );				// 设置指定元素的值
	double *setData    ( double *pBuffer, int _length );			// 从缓冲区写入数据
	double *setData    ( double *pBuffer, int _rows, int _cols );	// 从缓冲区写入数据

	// property
	int rows, cols;
	int length;
	int malloc_size;

	// operator(重载运算符)
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

	// methods
	void freeMatrix( void );
	bool initMatrix( const TCHAR *szName, int _rows, int _cols, int _initMode = INIT_MODE_NONE,
		double _fillVal = 0.0, int _initFcn = MAT_INIT_DEFAULT );
	bool initData  ( int _rows, int _cols, double _fillVal = 0.0, int _initFcn = MAT_INIT_DEFAULT );
	bool copyData  ( double *pNewData, int _rows, int _cols, double _fillVal = 0.0,
		int _initFcn = MAT_INIT_DEFAULT );

	bool create  ( int _rows, int _cols, double _fillVal = 0.0, int _initFcn = MAT_INIT_DEFAULT );
	bool createEx( const TCHAR *szName, int _rows, int _cols, double _fillVal = 0.0,
		int _initFcn = MAT_INIT_DEFAULT );

	CAnnMatrix * copy ( const CAnnMatrix *srcMartix );
	CAnnMatrix * clone( const CAnnMatrix *srcMartix );

	void clear( void );
	int  resize( int _rows, int _cols, double _fillVal = 0.0, int _initFcn = MAT_INIT_DEFAULT );
	bool empty( void ) const;
	bool makeUnitMatrix( int _size );

	void display( void );
	void display( const TCHAR *szName );

	// 矩阵的转置
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
	CAnnMatrixs( int _numMatrixs );
	CAnnMatrixs( const TCHAR *szName, int _numMatrixs );
	virtual ~CAnnMatrixs( void );

public:
	// gets
	const CAnnMatrix & operator [] ( int _index ) const;
	CAnnMatrix & matrixs( int _index ) const;

	// sets
	bool setMatrixs( int _numMatrixs );

	// property
	int numMatrixs;

	// methods
	void freeMatrixs( void );
	bool initMatrix ( const TCHAR *szName, int _numMatrixs );

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
