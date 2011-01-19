#pragma once

#include "AnnGlobal.h"
#include "CxBaseObject.h"
#include "CxList.h"

using namespace std;

#define MAT_NAN_ITEM         (-99.99)

#define MET_ADDR_ALIGN_SIZE  (64UL)
#define MET_ADDR_ALIGN_MASK  (MET_ADDR_ALIGN_SIZE - 1)
#define MET_ADDR_ALIGN(x)    (((DWORD)(x) + MET_ADDR_ALIGN_MASK) & (~MET_ADDR_ALIGN_MASK))

enum enumMatrixInit {
	MAT_INIT_NONE,
	MAT_INIT_ZEROS,
	MAT_INIT_ONES,
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

class CxVector { };
class CxVectors { };

class CxMatrix : public CxBaseObject
{
public:
	CxMatrix( void );
	CxMatrix( int _size );
	CxMatrix( int _rows, int _cols, int _initFcn = MAT_INIT_NONE );
	CxMatrix( const TCHAR *szName, int _rows, int _cols, int _initFcn = MAT_INIT_NONE );
	CxMatrix( CxMatrix & scrMatrix );								// �������캯��
	virtual ~CxMatrix( void );

public:
	// gets
	int    size        ( int n = 0 ) const;
	BOOL   sameSize    ( const CxMatrix *target, int n = 0 );
	double getElement  ( int _index ) const;						// ��ȡָ��Ԫ�ص�ֵ
	double getElement  ( int _row, int _col ) const;				// ��ȡָ��Ԫ�ص�ֵ
	int    getRowVector( int _row, double* pVector ) const;			// ��ȡ�����ָ���о���
	int    getColVector( int _col, double* pVector ) const;			// ��ȡ�����ָ���о���

	double *getDataPtr ( void ) const { return m_pData; };			// �������ָ��
	double *getOrigDataPtr ( void ) const { return m_pOrigData; };	// ���ԭʼ����ָ��

	// sets
	BOOL   setElement  ( int _row, int _col, double _value );		// ����ָ��Ԫ�ص�ֵ
	BOOL   setElement  ( int _index, double _value );				// ����ָ��Ԫ�ص�ֵ
	double *setDataBuf ( double *pBuffer, int _length );			// �ӻ�����д������
	double *setDataBuf ( double *pBuffer, int _rows, int _cols );	// �ӻ�����д������

	// property
	int rows, cols;
	int length;
	int mem_alloc;

	// ���������
	CxMatrix &  operator =  ( CxMatrix & _Right );
	BOOL        operator == ( CxMatrix & _Right );
	BOOL        operator != ( CxMatrix & _Right );
	CxMatrix &	operator +  ( CxMatrix & _Right );
	CxMatrix &	operator -  ( CxMatrix & _Right );
	CxMatrix &	operator += ( CxMatrix & _Right );
	CxMatrix &	operator -= ( CxMatrix & _Right );
	CxMatrix &	operator *  ( double _value     );
	CxMatrix &	operator *  ( CxMatrix & _Right );
	CxMatrix &	operator /  ( double _value     );
	CxMatrix &	operator ^  ( double _value     );

	// methods
	void freeMatrix( void );
	BOOL initMatrix( const TCHAR *szName, int _rows, int _cols, int _initMode = INIT_MODE_NONE,
		int _initFcn = MAT_INIT_DEFAULT );
	BOOL initData( int _rows, int _cols, int _initFcn = MAT_INIT_DEFAULT );

	BOOL Create  ( int _rows, int _cols, int _initFcn = MAT_INIT_DEFAULT );
	BOOL createEx( const TCHAR *szName, int _rows, int _cols, int _initFcn = MAT_INIT_DEFAULT );

	CxMatrix * copy ( const CxMatrix *srcMartix );
	CxMatrix * clone( const CxMatrix *srcMartix );

	int resize( int _rows, int _cols, int _initFcn = MAT_INIT_DEFAULT );
	BOOL makeUnitMatrix( int _size );

	// �����ת��
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

typedef CxList<CxMatrix> CxMatrixList;
