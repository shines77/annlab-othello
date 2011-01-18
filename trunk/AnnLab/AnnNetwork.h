#pragma once

#include "AnnGlobal.h"
#include "CxBaseObject.h"
#include "CxMatrix.h"

enum enumNetLayerType {
	LAYER_TYPE_NONE,
	LAYER_TYPE_INPUT,
	LAYER_TYPE_HIDDEN,
	LAYER_TYPE_OUTPUT,
	LAYER_TYPE_MAX,
};

class CxCell {};
class CxCells { int m, n; };

class CxTrainParam
{
public:
	CxTrainParam( void );
	virtual ~CxTrainParam( void );

public:
	// gets
	INLINE TCHAR * searchFcn( void ) const { return (TCHAR *)szSearchFcn; };

	// sets
	void setSearchFcn( const TCHAR *szFcnName );

	BOOL   show;				// 显示训练过程
	BOOL   showWindow;			// 显示训练窗口
	BOOL   showCommandLine;		// 显示命令行窗口

	int    epochs;				// 最大迭代次数 1-∞
	int    max_fail;			// 最大验证失败次数

	double lr;					// 学习率 (0.01-0.5)
	double mc;					// 动量因子 (0.8-0.99)
	double goal;				// 训练目标精度
	double time;				// 最大迭代时间 默认为0, 不限制

	double min_grad, max_grad;	// 最小/最大梯度

	double alpha, beta;			// alpha, beta
	double delta, gama;			// delta, gama
	double minstep, maxstep;	// 最小/最大步长
	double low_lim, up_lim;		// Lower/Upper limit on change in step size.
	double bmax;				// Maximum step size.

private:
	TCHAR  szSearchFcn[64];		// 搜寻函数名称
};

class CxTrainRecord { int m, n, k, l; };

class CxNetLayer : public CxBaseObject
{
public:
	CxNetLayer( void );
	CxNetLayer( int _numNeurons );
	CxNetLayer( int _prevNeurons, int _numNeurons );
	virtual ~CxNetLayer( void );

public:
	// gets
	INLINE TCHAR * initFcn     ( void ) const { return (TCHAR *)m_szInitFcn;     };
	INLINE TCHAR * transferFcn ( void ) const { return (TCHAR *)m_szTransferFcn; };

	// sets
	void setInitFcn     ( const TCHAR *szFcnName );
	void setTransferFcn ( const TCHAR *szFcnName );

	// property
	int index;
	int type;
	int numNeurons;
	int prevNeurons;

	CxNetLayer *prevLayer;
	CxNetLayer *nextLayer;

private:
	TCHAR     m_szInitFcn[FCN_NAME_LEN];
	TCHAR m_szTransferFcn[FCN_NAME_LEN];
};

class CxNetLayers : public CxBaseObject
{
public:
	CxNetLayers( void );
	CxNetLayers( int _numLayers );
	virtual ~CxNetLayers( void );

public:
	// property
	int numLayers;

	// gets
	CxNetLayer & operator [] ( int _index ) const;
	CxNetLayer & layers( int _index ) const;

	// sets
	BOOL setNumLayers( int _numLayers );

	// methods
	int  clear ( void );
	int  append( int _numNeurons );
	int  append( int _prevNeurons, int _numNeurons );
	int  append( CxNetLayer *pNetLayer );
	BOOL remove( int _index );
	BOOL modify( int _index, int _numNeuron );

protected:
	BOOL initLayers( int _numLayers );
	void freeLayers( void );

	CxNetLayer *getLayer( int _index ) const;

private:
	CxNetLayer* m_firstLayer;
	CxNetLayer* m_lastLayer;
};

class CBaseAnnNetwork : public CxBaseObject
{
public:
	CBaseAnnNetwork( void );
	CBaseAnnNetwork( const TCHAR *szName );
	virtual ~CBaseAnnNetwork( void );

	// methods
	virtual BOOL Create( const TCHAR *szSizesOfLayers, const TCHAR *szTransFcnOfLayers,
		const CxMatrix *inputMinMax = NULL, const TCHAR *szTrainFcn = NULL ) = 0;
	virtual BOOL CreateEx( const TCHAR *szName, const TCHAR *szSizesOfLayers,
		const TCHAR *szTransFcnOfLayers, const CxMatrix *inputMinMax,
		const TCHAR *szTrainFcn = NULL ) = 0;
};

class CAnnNetwork : public CBaseAnnNetwork
{
public:
	CAnnNetwork( void );
	CAnnNetwork( const TCHAR *szName );
	virtual ~CAnnNetwork( void );

public:
	// property
	int numInputs;
	int numLayers;
	int numOutputs;

	CxNetLayers layers;

	CxMatrix inputMinMax;

	CxMatrixs IW;
	CxMatrixs LW;
	CxVectors b;

	CxCells inputs;
	CxCells outputs;

	CxCells biases;
	CxCells inputWeights;
	CxCells layerWeights;

	CxTrainParam trainParam;

	// gets
	INLINE TCHAR * adaptFcn( void ) const    { return (TCHAR *)m_szAdaptFcn;    };
	INLINE TCHAR * divideFcn( void ) const   { return (TCHAR *)m_szDivideFcn;   };
	INLINE TCHAR * initFcn( void ) const     { return (TCHAR *)m_szInitFcn;     };
	INLINE TCHAR * gradientFcn( void ) const { return (TCHAR *)m_szGradientFcn; };
	INLINE TCHAR * performFcn( void ) const  { return (TCHAR *)m_szPerformFcn;  };
	INLINE TCHAR * trainFcn( void ) const    { return (TCHAR *)m_szTrainFcn;    };

	// sets
	INLINE void setNumInputs( int _numInputs )  { numInputs = _numInputs;   };
	INLINE void setNumLayers( int _numLayers )  { numLayers = _numLayers;   };
	INLINE void setOutputs  ( int _numOutputs ) { numOutputs = _numOutputs; };

	void setAdaptFcn    ( const TCHAR *szFcnName );
	void setDivideFcn   ( const TCHAR *szFcnName );
	void setInitFcn     ( const TCHAR *szFcnName );
	void setGradientFcn ( const TCHAR *szFcnName );
	void setPerformFcn  ( const TCHAR *szFcnName );
	void setTrainFcn    ( const TCHAR *szFcnName );

	// methods
	BOOL Create( const TCHAR *szSizesOfLayers, const TCHAR *szTransFcnOfLayers,
		const CxMatrix *inputMinMax = NULL, const TCHAR *szTrainFcn = NULL );
	BOOL CreateEx( const TCHAR *szName, const TCHAR *szSizesOfLayers,
		const TCHAR *szTransFcnOfLayers, const CxMatrix *inputMinMax,
		const TCHAR *szTrainFcn = NULL );

	int parseNetLayers( const TCHAR *szSizesOfLayers, CxNetLayers *pNetLayers,
		int _inNumLayers = 0, const TCHAR *szDelim = NULL );
	int parseTransFcns( const TCHAR *szTransFcnOfLayers, CxNetLayers *pNetLayers,
		const TCHAR *szDelim = NULL );

	int  clearNetLayers( void );
	int  appendNetLayer( int _numNeurons );
	int  appendNetLayer( int _prevNeurons, int _numNeurons );
	BOOL removeNetLayer( int _index );
	BOOL modifyNetLayer( int _index, int _numNeurons );

	CAnnNetwork * newff( const CxMatrix *inputMinMax, const TCHAR *szSizesOfLayers,
		const TCHAR *szTransFcnOfLayers, const TCHAR *szTrainFcn,
		BOOL bInitWeightsNow = FALSE );

	CAnnNetwork * init( void );
	CAnnNetwork * train( const CxMatrix *trainP, const CxMatrix *trainT,
		CxTrainRecord *trainRecord );

protected:
	BOOL CommonConstructor( const TCHAR *szSizesOfLayers, const TCHAR *szTransFcnOfLayers,
		const CxMatrix *inputMinMax = NULL, const TCHAR *szTrainFcn = NULL );
	BOOL CommonConstructor( const TCHAR *szName, const TCHAR *szSizesOfLayers,
		const TCHAR *szTransFcnOfLayers, const CxMatrix *inputMinMax,
		const TCHAR *szTrainFcn = NULL );
	void FreeNetwork( void );

	static int trimString(const TCHAR *szString, TCHAR *szBuffer, DWORD dwBufSize);

private:
	TCHAR   m_szAdaptFcn[FCN_NAME_LEN];
	TCHAR   m_szDivideFcn[FCN_NAME_LEN];
	TCHAR   m_szInitFcn[FCN_NAME_LEN];
	TCHAR   m_szGradientFcn[FCN_NAME_LEN];
	TCHAR   m_szPerformFcn[FCN_NAME_LEN];
	TCHAR   m_szTrainFcn[FCN_NAME_LEN];
};
