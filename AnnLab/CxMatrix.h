#pragma once

#include "AnnGlobal.h"
#include "CxBaseObject.h"
#include "CxList.h"
#include <list>
#include <vector>

using namespace std;

#define MAT_NAN_ITEM         (-99.99)

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

class CxVector : public CxBaseObject
{
public:
	CxVector( void );
	CxVector( int _size );
	CxVector( CxVector & srcVerctor );
	virtual ~CxVector( void );

public:
	// property
	int length;
	int mem_alloc;

	// gets
	double operator [] ( int _index ) const;
	INLINE int size( void ) const { return length; };

	//sets

	// methos
	CxVector *copy( const CxVector *srcVector );

protected:
	void freeVector( void );
	BOOL initVector( const TCHAR *szName, int _size, int _initMode = INIT_MODE_NONE,
		int _initFcn = MAT_INIT_NONE );
	BOOL initData( int _size, int _initFcn = MAT_INIT_NONE );
	
private:
	double *m_pOrigData;
	double *m_pData;
};

class CxVectors : public CxBaseObject
{
public:
	CxVectors( void );
	virtual ~CxVectors( void );

public:
	// property
	int length;

	// gets
	INLINE int size( void ) const { return length; };

	// sets

protected:
	void freeVector( void );
};

class CxMatrix : public CxBaseObject
{
public:
	CxMatrix( void );
	CxMatrix( int _size );
	CxMatrix( int _rows, int _cols, int _initFcn = MAT_INIT_NONE );
	CxMatrix( const TCHAR *szName, int _rows, int _cols, int _initFcn = MAT_INIT_NONE );
	CxMatrix( const CxMatrix & scrMatrix );								// 拷贝构造函数
	virtual ~CxMatrix( void );

public:
	// gets
	int    size        ( int n = 0 ) const;
	BOOL   sameSize    ( const CxMatrix *target, int n = 0 );
	double getElement  ( int _index ) const;						// 获取指定元素的值
	double getElement  ( int _row, int _col ) const;				// 获取指定元素的值

	// 获取矩阵的指定行矩阵
	int    getRowVector( int _row, double* pVector ) const;
	// 获取矩阵的指定列矩阵
	int    getColVector( int _col, double* pVector ) const;

	// 获取矩阵的指定行矩阵
	int    getRowVector( int _row, CxMatrix* pMatrix ) const;
	// 获取矩阵的指定列矩阵
	int    getColVector( int _col, CxMatrix* pMatrix ) const;

	// 把第x行至第y行的数据转换成为新的矩阵
	int    getRowVector( int _rowStart, int _rowEnd, CxMatrix* pMatrix ) const;
	// 把第x列至第y列的数据转换成为新的矩阵
	int    getColVector( int _colStart, int _colEnd, CxMatrix* pMatrix ) const;

	/* 把第x行至第y行的第m列至第n列的数据转换成为新的矩阵 */
	int    getPartOfMatrix( int _rowStart, int _rowEnd,
							int _colStart, int _colEnd, CxMatrix* pMatrix ) const;

	// 获取矩阵的指定行矩阵
	CxMatrix getRowVector( int _row ) const;
	// 获取矩阵的指定列矩阵
	CxMatrix getColVector( int _col ) const;

	double *getData    ( void ) const { return m_pData;     };		// 获得数据指针
	double *getOrigData( void ) const { return m_pOrigData; };		// 获得原始数据指针

	// sets
	BOOL   setElement  ( int _row, int _col, double _value );		// 设置指定元素的值
	BOOL   setElement  ( int _index, double _value );				// 设置指定元素的值
	double *setData    ( double *pBuffer, int _length );			// 从缓冲区写入数据
	double *setData    ( double *pBuffer, int _rows, int _cols );	// 从缓冲区写入数据

	// property
	int rows, cols;
	int length;
	int mem_alloc;

	// operator(重载运算符)
	double      operator () ( int _index );
	double      operator () ( int _row, int _col );
	double      operator [] ( int _index );

	CxMatrix &  operator =  ( CxMatrix & _Right );
	CxMatrix &  operator =  ( int _Right        );
	CxMatrix &  operator =  ( double _Right     );
	BOOL        operator == ( CxMatrix & _Right );
	BOOL        operator != ( CxMatrix & _Right );
	CxMatrix    operator +  ( double _value     );
	CxMatrix    operator +  ( CxMatrix & _Right );
	CxMatrix &  operator += ( CxMatrix & _Right );
	CxMatrix    operator -  ( double _value     );
	CxMatrix    operator -  ( CxMatrix & _Right );
	CxMatrix &  operator -= ( CxMatrix & _Right );
	CxMatrix    operator *  ( double _value     );
	CxMatrix    operator *  ( CxMatrix & _Right );
	CxMatrix &  operator *= ( double _value     );
	CxMatrix &  operator *= ( CxMatrix & _Right );
	CxMatrix    operator /  ( double _value     );
	CxMatrix    operator /  ( CxMatrix &_Right  );
	CxMatrix &  operator /= ( double _value     );
	CxMatrix    operator ^  ( double _value     );
	CxMatrix &  operator ^= ( double _value     );

	friend CxMatrix operator + ( double _value, CxMatrix & _Right );
	friend CxMatrix operator - ( double _value, CxMatrix & _Right );
	friend CxMatrix operator * ( double _value, CxMatrix & _Right );
	friend CxMatrix operator / ( double _value, CxMatrix & _Right );

	// methods
	void freeMatrix( void );
	BOOL initMatrix( const TCHAR *szName, int _rows, int _cols, int _initMode = INIT_MODE_NONE,
		double _fillVal = 0.0, int _initFcn = MAT_INIT_DEFAULT );
	BOOL initData  ( int _rows, int _cols, double _fillVal = 0.0, int _initFcn = MAT_INIT_DEFAULT );
	BOOL copyData  ( double *pNewData, int _rows, int _cols, double _fillVal = 0.0,
		int _initFcn = MAT_INIT_DEFAULT );

	BOOL Create  ( int _rows, int _cols, double _fillVal = 0.0, int _initFcn = MAT_INIT_DEFAULT );
	BOOL createEx( const TCHAR *szName, int _rows, int _cols, double _fillVal = 0.0,
		int _initFcn = MAT_INIT_DEFAULT );

	CxMatrix * copy ( const CxMatrix *srcMartix );
	CxMatrix * clone( const CxMatrix *srcMartix );

	void clear( void );
	int  resize( int _rows, int _cols, double _fillVal = 0.0, int _initFcn = MAT_INIT_DEFAULT );
	BOOL empty( void ) const;
	BOOL makeUnitMatrix( int _size );

	void display( void );
	void display( const TCHAR *szName );

	// 矩阵的转置
	CxMatrix & transpose( void );

	int zeros  ( int _rows, int _cols );
	int ones   ( int _rows, int _cols );
	int rands  ( int _rows, int _cols );
	int rands2 ( int _rows, int _cols );

	CxMatrix _zeros  ( int _rows, int _cols ) const;
	CxMatrix _ones   ( int _rows, int _cols ) const;
	CxMatrix _rands  ( int _rows, int _cols ) const;
	CxMatrix _rands2 ( int _rows, int _cols ) const;

private:
	double *m_pOrigData;
	double *m_pData;
};

class CxMatrixs : public CxBaseObject
{
public:
	CxMatrixs( void );
	CxMatrixs( int _numMatrixs );
	CxMatrixs( const TCHAR *szName, int _numMatrixs );
	virtual ~CxMatrixs( void );

public:
	// gets
	const CxMatrix & operator [] ( int _index ) const;
	CxMatrix & matrixs( int _index ) const;

	// sets
	BOOL setMatrixs( int _numMatrixs );

	// property
	int numMatrixs;

	// methods
	void freeMatrixs( void );
	BOOL initMatrix ( const TCHAR *szName, int _numMatrixs );

	BOOL create( const TCHAR *szName, int _numMatrixs );

	CxMatrixs * copy ( const CxMatrixs *srcMartixs );
	CxMatrixs * clone( const CxMatrixs *srcMartixs );

private:
	CxMatrix *pMatrixs;
};

class CxMatrixVector : public vector<CxMatrix>, public CxBaseObject
{
public:
	CxMatrixVector( void );
	virtual ~CxMatrixVector( void );
};

class CxMatrixList : public list<CxMatrix>, public CxBaseObject
{
public:
	CxMatrixList( void );
	virtual ~CxMatrixList( void );
};

/*
CxMatrix operator + ( double _value, CxMatrix & _Right );
CxMatrix operator - ( double _value, CxMatrix & _Right );
CxMatrix operator * ( double _value, CxMatrix & _Right );
CxMatrix operator / ( double _value, CxMatrix & _Right );
//*/

//typedef CxList<CxMatrix> CxMatrixList;
