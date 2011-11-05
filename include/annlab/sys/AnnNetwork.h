#pragma once

#include "../../../include/annlab/annlab_stddef.h"
#include "../../../include/annlab/sys/AnnObject.h"
#include "../../../include/annlab/sys/AnnMatrix.h"
#include <windows.h>

namespace annlab {

enum enumNetLayerType {
	LAYER_TYPE_NONE,
	LAYER_TYPE_INPUT,
	LAYER_TYPE_HIDDEN,
	LAYER_TYPE_OUTPUT,
	LAYER_TYPE_MAX,
};

class CAnnCell {};
class CAnnCells { int m, n; };

class CAnnTrainParam
{
public:
	CAnnTrainParam( void );
	virtual ~CAnnTrainParam( void );

public:
	// gets
	INLINE TCHAR * searchFcn( void ) const { return (TCHAR *)szSearchFcn; };

	// sets
	void setSearchFcn( const TCHAR *szFcnName );

	bool   show;				// 显示训练过程
	bool   showWindow;			// 显示训练窗口
	bool   showCommandLine;		// 显示命令行窗口

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

class CAnnTrainRecord
{
public:
	CAnnTrainRecord( void );
	virtual ~CAnnTrainRecord( void );
public:
	int epochs;
};

class CAnnNetInOutputs : public CAnnObject
{
public:
	CAnnNetInOutputs( void );
	CAnnNetInOutputs( const TCHAR *szName );
	virtual ~CAnnNetInOutputs( void );

public:
	void clear( void );
};

class CAnnNetInputs : public CAnnNetInOutputs
{
public:
	CAnnNetInputs( void );
	CAnnNetInputs( const TCHAR *szName );
	virtual ~CAnnNetInputs( void );

public:
	//
};

class CAnnNetOutputs : public CAnnNetInOutputs
{
public:
	CAnnNetOutputs( void );
	CAnnNetOutputs( const TCHAR *szName );
	virtual ~CAnnNetOutputs( void );

public:
	//
};

class CAnnNetWeights {
public:
	void clear( void ) {};
};

class CAnnNetBiases {
public:
	void clear( void ) {};
};

class CAnnNetLayer : public CAnnObject
{
public:
	CAnnNetLayer( void );
	CAnnNetLayer( int _numNeurons );
	CAnnNetLayer( int _prevNeurons, int _numNeurons );
	CAnnNetLayer( int _type, int _prevNeurons, int _numNeurons );
	CAnnNetLayer( const CAnnNetLayer & srcNetLayer );
	virtual ~CAnnNetLayer( void );

public:
	// property
	int index;
	int type;
	int prevNeurons;
	int numNeurons;

	CAnnNetLayer *prevLayer;
	CAnnNetLayer *nextLayer;

	// gets
	INLINE TCHAR * initFcn     ( void ) const { return (TCHAR *)m_szInitFcn;     };
	INLINE TCHAR * transferFcn ( void ) const { return (TCHAR *)m_szTransferFcn; };

	// sets
	void setInitFcn     ( const TCHAR *szFcnName );
	void setTransferFcn ( const TCHAR *szFcnName );

	void setType ( int _type ) { type = _type; };

	// methods
	bool initNetLayer( int _prevNeurons, int _numNeurons );
	bool initNetLayer( int _index, int _type, int _prevNeurons, int _numNeurons,
		CAnnNetLayer *_prevLayer = NULL, CAnnNetLayer *_nextLayer = NULL );

private:
	TCHAR     m_szInitFcn[FCN_NAME_LEN];
	TCHAR m_szTransferFcn[FCN_NAME_LEN];
};

class CAnnNetLayers : public CAnnObject
{ 
public:
	CAnnNetLayers( void );
	CAnnNetLayers( int _numLayers );
	CAnnNetLayers( const TCHAR *szSizesOfLayers );
	virtual ~CAnnNetLayers( void );

public:
	// property
	int numLayers;

	// gets
	CAnnNetLayer & operator [] ( int _index ) const;
	CAnnNetLayer & operator () ( int _index ) const;
	CAnnNetLayer & layer( int _index ) const;
	CAnnNetLayer * getLayer( int _index ) const;
 
	int  size ( void ) const;
	bool empty( void ) const;

	CAnnNetLayer * begin( void ) const;
	CAnnNetLayer * end  ( void ) const;
	CAnnNetLayer & first( void ) const;
	CAnnNetLayer & last ( void ) const;

	// sets
	int  setNumLayers( int _numLayers );

	// methods
	int  clear ( void );
	int  append( int _numNeurons );
	int  append( int _prevNeurons, int _numNeurons );
	int  append( CAnnNetLayer *pNetLayer );
	int  remove( int _index );
	int  insert( int _index, CAnnNetLayer *pNetLayer );
	bool modify( int _index, int _numNeuron );

	int  pop_front ( void );
	int  pop_back  ( void );
	int  push_front( CAnnNetLayer *pNetLayer );
	int  push_back ( CAnnNetLayer *pNetLayer );

protected:
	INLINE void commonConstructor( int _numLayers );
	int  initLayers( int _numLayers );
	void freeLayers( void );

	int parseNetLayers( const TCHAR *szSizesOfLayers, int _inNumLayers = 0,
		const TCHAR *szDelim = NULL );

private:
	CAnnNetLayer* m_firstLayer;
	CAnnNetLayer* m_lastLayer;
};

class CAnnNetwork : public CAnnObject
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
	int numCounter;

	CAnnMatrix input;
	CAnnMatrix output;
	CAnnMatrix inputRange;

	CAnnNetInputs  inputs;
	CAnnNetLayers  layers;
	CAnnNetOutputs outputs;

	CAnnMatrix     IW;
	CAnnMatrixList LW;
	CAnnMatrixList b;

	CAnnNetWeights inputWeights;
	CAnnNetWeights layerWeights;
	CAnnNetBiases  biases;

	CAnnTrainParam trainParam;

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
	bool Create( const TCHAR *szSizesOfLayers, const TCHAR *szTransFcnOfLayers,
		const CAnnMatrix *inputMinMax = NULL, const TCHAR *szTrainFcn = NULL );
	bool CreateEx( const TCHAR *szName, const TCHAR *szSizesOfLayers,
		const TCHAR *szTransFcnOfLayers, const CAnnMatrix *inputMinMax,
		const TCHAR *szTrainFcn = NULL );

	int parseNetLayers( const TCHAR *szSizesOfLayers, CAnnNetLayers *pNetLayers,
		int _inNumLayers = 0, const TCHAR *szDelim = NULL );
	int parseTransFcns( const TCHAR *szTransFcnOfLayers, CAnnNetLayers *pNetLayers,
		const TCHAR *szDelim = NULL );

	int  clearNetLayers( void );
	int  appendNetLayer( int _numNeurons );
	int  appendNetLayer( int _prevNeurons, int _numNeurons );
	bool removeNetLayer( int _index );
	bool modifyNetLayer( int _index, int _numNeurons );

	CAnnNetwork * newff( const CAnnMatrix *inputMinMax, const TCHAR *szSizesOfLayers,
		const TCHAR *szTransFcnOfLayers, const TCHAR *szTrainFcn,
		bool bInitWeightsNow = FALSE );

	CAnnNetwork * init( void );
	CAnnNetwork * train( const CAnnMatrixList *trainP, const CAnnMatrixList *trainT,
		CAnnTrainRecord *tr );

	bool initnw( CAnnMatrix *pMatrix, int _index, int _numInputs, int _numNeurons,
		CAnnMatrix *pInputRange, CAnnMatrix *pActiveRange );
	bool initwb( CAnnMatrix *pMatrix, int _index, int _numInputs, int _numNeurons,
		CAnnMatrix *pInputRange, CAnnMatrix *pActiveRange );

protected:
	bool initialize( const TCHAR *szSizesOfLayers, const TCHAR *szTransFcnOfLayers,
		const CAnnMatrix *inputMinMax = NULL, const TCHAR *szTrainFcn = NULL );
	bool initialize( const TCHAR *szName, const TCHAR *szSizesOfLayers,
		const TCHAR *szTransFcnOfLayers, const CAnnMatrix *inputMinMax,
		const TCHAR *szTrainFcn = NULL );
	void freeNetwork( void );

	static int trimString(const TCHAR *szString, TCHAR *szBuffer, DWORD dwBufSize);
private:
	TCHAR   m_szAdaptFcn[FCN_NAME_LEN];
	TCHAR   m_szDivideFcn[FCN_NAME_LEN];
	TCHAR   m_szInitFcn[FCN_NAME_LEN];
	TCHAR   m_szGradientFcn[FCN_NAME_LEN];
	TCHAR   m_szPerformFcn[FCN_NAME_LEN];
	TCHAR   m_szTrainFcn[FCN_NAME_LEN];
};

INLINE void setFcnName( TCHAR *szDstFuncName, int nNameSize, const TCHAR *szFcnName );

}  // namespace annlab
