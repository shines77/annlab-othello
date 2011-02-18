#include "StdAfx.h"
#include "AnnGlobal.h"
#include "AnnNetwork.h"
#include "toolbox\matlab\elmat.h"
#include "toolbox\nnet\nnnetwork.h"
#include <excpt.h>
#include <Winbase.h>
#include <math.h>

using namespace matlab;

#define error printf

#define LAYER_INITFCN_DEFAULT      _T("initnw")
#define LAYER_TRANSFERFCN_DEFAULT  _T("tansig")

#define NNET_ADAPTFCN_DEFAULT      _T("trains")
#define NNET_DIVIDEFCN_DEFAULT     _T("")
#define NNET_INITFCN_DEFAULT       _T("initlay")
#define NNET_GRADIENTFCN_DEFAULT   _T("gdefaults")
#define NNET_PERFORMFCN_DEFAULT    _T("mse")
#define NNET_TRAINFCN_DEFAULT      _T("traingd")

INLINE void setFcnName( TCHAR *szDstFuncName, int nNameSize, const TCHAR *szFcnName )
{
	if (szDstFuncName != NULL) {
		if (szFcnName != NULL) {
			_tcscpy_s(szDstFuncName, nNameSize, szFcnName);
			_tcslwr_s(szDstFuncName, nNameSize);
		}
		else
			_tcscpy_s(szDstFuncName, nNameSize, _T(""));
	}
}

//////////////////////////////////////////////////////////////////
// CxTrainParam
//////////////////////////////////////////////////////////////////

CxTrainParam::CxTrainParam( void )
{
	show            = TRUE;		// 显示训练过程
	showWindow      = TRUE;		// 显示训练窗口
	showCommandLine = TRUE;		// 显示命令行窗口

	epochs   = 0;				// 最大迭代次数 1-∞
	max_fail = 6;				// 最大验证失败次数

	lr       = 0.05;			// 学习率 (0.01-0.5)
	mc       = 0.9;				// 动量因子 (0.8-0.99)
	goal     = 0.0001;			// 训练目标精度
	time     = 0.0;				// 最大迭代时间 默认为0, 表示不限制

	min_grad = 1E-6;			// Minimum performance gradient
	max_grad = 1.0;				// Maximum performance gradient

	alpha    = 0.001;			// Scale factor which determines sufficient reduction in perf.
	beta     = 0.1;				// Scale factor which determines sufficiently large step size.
	delta    = 0.01;			// Initial step size in interval location step.
	gama     = 0.1;				/* Parameter to avoid small reductions in performance.
								   Usually set to 0.1. (See use in srch_cha.) */
	minstep  = 1E-6;			// Minimum step length.
	maxstep  = 100;				// Maximum step length.
	low_lim  = 0.1;				// Lower limit on change in step size.
	up_lim   = 0.5;				// Upper limit on change in step size.
	bmax     = 26.0;			// Maximum step size.

	// 搜寻函数名称
	_tcscpy_s(szSearchFcn, _countof(szSearchFcn), _T(""));
}

CxTrainParam::~CxTrainParam( void )
{
}

void CxTrainParam::setSearchFcn( const TCHAR *szFcnName )
{
	setFcnName(szSearchFcn, _countof(szSearchFcn), szFcnName);
}

//////////////////////////////////////////////////////////////////
// CxTrainRecord
//////////////////////////////////////////////////////////////////

CxTrainRecord::CxTrainRecord( void )
{
	epochs = 0;
}

CxTrainRecord::~CxTrainRecord( void )
{
}

//////////////////////////////////////////////////////////////////
// CxNetInOutputs
//////////////////////////////////////////////////////////////////

CxNetInOutputs::CxNetInOutputs( void )
{

}

CxNetInOutputs::CxNetInOutputs( const TCHAR *szName )
{

}

CxNetInOutputs::~CxNetInOutputs( void )
{

}

void CxNetInOutputs::clear( void )
{

}

//////////////////////////////////////////////////////////////////
// CxNetInputs
//////////////////////////////////////////////////////////////////

CxNetInputs::CxNetInputs( void )
{

}

CxNetInputs::CxNetInputs( const TCHAR *szName )
{

}

CxNetInputs::~CxNetInputs( void )
{

}

//////////////////////////////////////////////////////////////////
// CxNetOutputs
//////////////////////////////////////////////////////////////////

CxNetOutputs::CxNetOutputs( void )
{

}

CxNetOutputs::CxNetOutputs( const TCHAR *szName )
{

}

CxNetOutputs::~CxNetOutputs( void )
{

}


//////////////////////////////////////////////////////////////////
// CxNetLayer
//////////////////////////////////////////////////////////////////

CxNetLayer::CxNetLayer( void )
{
	initNetLayer(0, 0);
}

CxNetLayer::CxNetLayer( int _numNeurons )
{
	initNetLayer(0, _numNeurons);
}

CxNetLayer::CxNetLayer( int _prevNeurons, int _numNeurons )
{
	initNetLayer(_prevNeurons, _numNeurons);
}

CxNetLayer::CxNetLayer( int _type, int _prevNeurons, int _numNeurons )
{
	initNetLayer(-1, _type, _prevNeurons, _numNeurons);
}

CxNetLayer::CxNetLayer( const CxNetLayer & srcNetLayer )
{
	int _index, _type;
	int _prevNeurons, _numNeurons;
	CxNetLayer *_prevLayer, *_nextLayer;

	_index       = srcNetLayer.index;
	_type        = srcNetLayer.type;
	_prevNeurons = srcNetLayer.prevNeurons;
	_numNeurons  = srcNetLayer.numNeurons;

	_prevLayer   = srcNetLayer.prevLayer;
	_nextLayer   = srcNetLayer.nextLayer;

	initNetLayer(_index, _type, _prevNeurons, _numNeurons,
		_prevLayer, _nextLayer);

	setInitFcn    (srcNetLayer.initFcn());
	setTransferFcn(srcNetLayer.transferFcn());
}

CxNetLayer::~CxNetLayer( void )
{
}

BOOL CxNetLayer::initNetLayer( int _index, int _type,
							  int _prevNeurons,
							  int _numNeurons,
							  CxNetLayer *_prevLayer, /*= NULL */
							  CxNetLayer *_nextLayer  /*= NULL */ )
{
	index       = _index;
	type        = _type;
	prevNeurons = _prevNeurons;
	numNeurons  = _numNeurons;

	prevLayer   = NULL;
	nextLayer   = NULL;

	
	//_tcscpy_s(m_szInitFcn,     _countof(m_szInitFcn),     _T(""));
	//_tcscpy_s(m_szTransferFcn, _countof(m_szTransferFcn), _T(""));

	_tcscpy_s(m_szInitFcn,     _countof(m_szInitFcn),     LAYER_INITFCN_DEFAULT);
	_tcscpy_s(m_szTransferFcn, _countof(m_szInitFcn),     LAYER_TRANSFERFCN_DEFAULT);

	return TRUE;
}

BOOL CxNetLayer::initNetLayer( int _prevNeurons, int _numNeurons )
{
	return initNetLayer(-1, LAYER_TYPE_NONE, _prevNeurons, _numNeurons);
}

void CxNetLayer::setInitFcn( const TCHAR *szFcnName )
{
	setFcnName(m_szInitFcn, _countof(m_szInitFcn), szFcnName);
}

void CxNetLayer::setTransferFcn( const TCHAR *szFcnName )
{
	setFcnName(m_szTransferFcn, _countof(m_szTransferFcn), szFcnName);
}

//////////////////////////////////////////////////////////////////
// CxNetLayers
//////////////////////////////////////////////////////////////////

CxNetLayers::CxNetLayers( void )
{
	commonConstructor(0);
}

CxNetLayers::CxNetLayers( int _numLayers )
{
	commonConstructor(_numLayers);
}

CxNetLayers::CxNetLayers( const TCHAR *szSizesOfLayers )
{
	commonConstructor(0);

	if (szSizesOfLayers != NULL) {
		// first time to get the num layers
		int _numLayers = parseNetLayers(szSizesOfLayers, 0);
		// more than 2 layers
		if (_numLayers > 1) {
			// parse net layer info from string 'szSizesOfLayers'
			_numLayers = parseNetLayers(szSizesOfLayers, _numLayers);
		}
	}
}

CxNetLayers::~CxNetLayers( void )
{
	freeLayers();
}

INLINE void CxNetLayers::commonConstructor( int _numLayers )
{
	numLayers    = 0;
	m_firstLayer = NULL;
	m_lastLayer  = NULL;

	initLayers(_numLayers);
}

int CxNetLayers::initLayers( int _numLayers )
{
	int retNumLayers = 0;
	if (_numLayers > 0)
	{
		clear();

		numLayers = _numLayers;
		for (int i=0; i<numLayers; i++) {
			CxNetLayer *pNetLayer = new CxNetLayer();
			if (pNetLayer != NULL) {
				if (append(pNetLayer))
					retNumLayers++;
			}
		}
	}
	return retNumLayers;
}

void CxNetLayers::freeLayers( void )
{
	clear();
}

int CxNetLayers::parseNetLayers( const TCHAR *szSizesOfLayers,
								int _inNumLayers, /*= 0 */
								const TCHAR *szDelim /*= NULL */ )
{
	TCHAR *pszOffset, *pszEndOf, *pszDest, *pszDelim;
	TCHAR szBuffer[512];
	DWORD dwBufSize = 512;

	TRACE(_T("CxNetLayers::parseNetLayers() Enter.\n"));
	if (szSizesOfLayers == NULL)
		return -1;

	int lenNumber, lenDelim;
	int _indexLayer = 0, _numNeuron;
	int prevNeuron = 0;

	pszDelim = (szDelim == NULL) ? _T(",") : szDelim;
	lenDelim = _tcslen(pszDelim);

	pszOffset = (TCHAR *)szSizesOfLayers;
	pszEndOf  = (TCHAR *)szSizesOfLayers + _tcslen(szSizesOfLayers);
	while (pszOffset <= pszEndOf) {
		pszDest = _tcsstr(pszOffset, pszDelim);
		if (pszDest != NULL)
			lenNumber = pszDest - pszOffset;
		else
			lenNumber = pszEndOf - pszOffset;
		if (lenNumber > 0) {
			if (_inNumLayers > 0) {
				if (lenNumber > (int)dwBufSize)
					lenNumber = dwBufSize - 1;
				memcpy_s(szBuffer, _countof(szBuffer), pszOffset, sizeof(TCHAR) * lenNumber);
				szBuffer[lenNumber] = 0;
				_numNeuron = _tstoi(szBuffer);
				if (_numNeuron > 0) {
					if (_indexLayer == 0)
						clear();
					int _index = append(prevNeuron, _numNeuron);
					if (_index >= 0) {
						if (_indexLayer == 0)
							layer(_index).type = LAYER_TYPE_INPUT;
						else if (_indexLayer == _inNumLayers - 1)
							layer(_index).type = LAYER_TYPE_OUTPUT;
						else
							layer(_index).type = LAYER_TYPE_HIDDEN;
					}
				}
				TRACE(_T("numNeuron[%d] = %d.\n"), _indexLayer, _numNeuron);
				prevNeuron = _numNeuron;
			}
			_indexLayer++;
		}
		if (pszDest != NULL)
			pszOffset += lenNumber + lenDelim;
		else
			break;
	}
	TRACE(_T("CxNetLayers::parseNetLayers() Exit: _indexLayer = %d.\n"), _indexLayer);
	return _indexLayer;
}

BOOL CxNetLayers::setNumLayers( int _numLayers )
{
	return initLayers(_numLayers);
}

CxNetLayer & CxNetLayers::operator [] ( int _index ) const
{
	CxNetLayer &_layer = (CxNetLayer &)layer(_index);
	return _layer;
}

CxNetLayer & CxNetLayers::operator () ( int _index ) const
{
	CxNetLayer &_layer = (CxNetLayer &)layer(_index);
	return _layer;
}

CxNetLayer & CxNetLayers::layer( int _index ) const
{
	ASSERT(_index >= 0 && _index < numLayers);
	if (_index >= 0 && _index < numLayers) {
		CxNetLayer *nextLayer;
		CxNetLayer *curLayer = m_firstLayer;
		int _nowIndex = 0;
		while (curLayer != NULL) {
			if (_nowIndex == _index)
				return (CxNetLayer &)*curLayer;
			_nowIndex++;
			if (_nowIndex > _index)
				break;
			nextLayer = curLayer->nextLayer;
			if (nextLayer != NULL)
				curLayer = nextLayer;
			else
				break;
		}
	}
	throw _T("CxNetLayers::layer(index): index overflow!");
	//RaiseException( EXCEPTION_ACCESS_VIOLATION, 0, 0, 0 );
	// Create temp 1 layer
	CxNetLayer *pNetLayers = new CxNetLayer();
	if (pNetLayers != NULL) {
		pNetLayers->index = -1;
		return (CxNetLayer &)(*pNetLayers);
	}
	throw _T("CxNetLayers::layer(index): temp layer create failure!");
	//RaiseException( EXCEPTION_INVALID_HANDLE, 0, 0, 0 );
	return *(new CxNetLayer());
}

CxNetLayer * CxNetLayers::getLayer( int _index ) const
{
	CxNetLayer *nextLayer;
	CxNetLayer *curLayer = m_firstLayer;
	int _nowIndex = 0;

	ASSERT(_index >= 0 && _index < numLayers);
	if (_index < 0 || _index >= numLayers)
		return NULL;

	while (curLayer != NULL) {
		if (_nowIndex == _index)
			return (CxNetLayer *)curLayer;

		_nowIndex++;
		if (_nowIndex > _index)
			break;
		nextLayer = curLayer->nextLayer;
		if (nextLayer != NULL)
			curLayer = nextLayer;
		else
			break;
	}
	return NULL;
}

int CxNetLayers::clear( void )
{
	int oldLayers = numLayers;

	CxNetLayer *nextLayer;
	CxNetLayer *curLayer = m_firstLayer;
	int _numLayers = 0;

	while (curLayer != NULL && _numLayers < numLayers) {
		nextLayer = curLayer->nextLayer;
		delete curLayer;
		_numLayers++;
		if (nextLayer != NULL) {
			curLayer = nextLayer;
			m_firstLayer = nextLayer;
		}
		else
			break;
	}

	numLayers = 0;
	m_firstLayer = NULL;
	m_lastLayer  = NULL;

	return oldLayers;
}

int CxNetLayers::size( void ) const
{
	CxNetLayer *nextLayer;
	CxNetLayer *curLayer = m_firstLayer;
	int _numLayers = 0;

	while (curLayer != NULL) {
		_numLayers++;
		if (_numLayers > numLayers)
			break;
		nextLayer = curLayer->nextLayer;
		if (nextLayer != NULL)
			curLayer = nextLayer;
		else
			break;
	}
	return _numLayers;
}

BOOL CxNetLayers::empty( void ) const
{
	int _numLayers = size();
	return (_numLayers <= 0);
}

int CxNetLayers::pop_front( void )
{
	CxNetLayer *nextLayer;
	CxNetLayer *curLayer = m_firstLayer;
	if (curLayer != NULL) {
		nextLayer = curLayer->nextLayer;
		delete curLayer;
		numLayers--;
		m_firstLayer = nextLayer;
		if (nextLayer == NULL || numLayers <= 0)
			m_lastLayer  = NULL;
		return numLayers;
	}
	return -1;
}

int CxNetLayers::pop_back( void )
{
	CxNetLayer *prevLayer;
	CxNetLayer *curLayer = m_lastLayer;
	if (curLayer != NULL) {
		prevLayer = curLayer->prevLayer;
		delete curLayer;
		numLayers--;
		m_lastLayer = prevLayer;
		if (prevLayer == NULL || numLayers <= 0)
			m_firstLayer = NULL;
		return numLayers;
	}
	return -1;
}

int CxNetLayers::push_front( CxNetLayer *pNetLayer )
{
	if (pNetLayer != NULL) {
		pNetLayer->index = numLayers;
		if (m_firstLayer == NULL || numLayers == 0) {
			m_firstLayer = pNetLayer;
			m_lastLayer  = pNetLayer;
		}
		else {
			m_lastLayer->prevLayer = pNetLayer;
			pNetLayer->nextLayer   = m_lastLayer;
			pNetLayer->prevLayer   = NULL;

			m_firstLayer = pNetLayer;
		}
		numLayers++;
		return (numLayers - 1);
	}
	return -1;
}

int CxNetLayers::push_back( CxNetLayer *pNetLayer )
{
	if (pNetLayer != NULL) {
		pNetLayer->index = numLayers;
		if (m_lastLayer == NULL || numLayers == 0) {
			m_firstLayer = pNetLayer;
			m_lastLayer  = pNetLayer;
		}
		else {
			m_lastLayer->nextLayer = pNetLayer;
			pNetLayer->prevLayer   = m_lastLayer;
			pNetLayer->nextLayer   = NULL;

			m_lastLayer = pNetLayer;
		}
		numLayers++;
		return (numLayers - 1);
	}
	return -1;
}

int CxNetLayers::append( CxNetLayer *pNetLayer )
{
	return push_back(pNetLayer);
}

int CxNetLayers::append( int _prevNeurons, int _numNeurons )
{
	if (_numNeurons > 0) {
		CxNetLayer *pNetLayer = new CxNetLayer(_prevNeurons, _numNeurons);
		if (pNetLayer != NULL) {
			pNetLayer->index       = numLayers;
			pNetLayer->numNeurons  = _numNeurons;
			pNetLayer->prevNeurons = _prevNeurons;
			return append(pNetLayer);
		}
	}
	return -1;
}

int CxNetLayers::append( int _numNeurons )
{
	int _prevNeurons = 0;
	if (numLayers > 0) {
		CxNetLayer *pNetLayer = getLayer(numLayers - 1);
		if (pNetLayer != NULL) {
			_prevNeurons = pNetLayer->numNeurons;
			if (_prevNeurons < 0)
				return -1;
		}
	}
	return append(_prevNeurons, _numNeurons);
}

int CxNetLayers::insert( int _index, CxNetLayer *pNetLayer )
{
	return 0;
}

int CxNetLayers::remove( int _index )
{
	return 0;
}

BOOL CxNetLayers::modify( int _index, int _numNeuron )
{
	return FALSE;
}

//////////////////////////////////////////////////////////////////
// CAnnNetwork
//////////////////////////////////////////////////////////////////

CAnnNetwork::CAnnNetwork( void )
{
	commonConstructor(_T(""), _T(""));
}

CAnnNetwork::CAnnNetwork( const TCHAR *szName )
{
	commonConstructor(szName, _T(""), _T(""), NULL);
}

CAnnNetwork::~CAnnNetwork( void )
{
	freeNetwork();
}

void CAnnNetwork::freeNetwork( void )
{
	clearNetLayers();
}

BOOL CAnnNetwork::commonConstructor( const TCHAR *szName,
									const TCHAR *szSizesOfLayers,
									const TCHAR *szTransFcnOfLayers,
									const CxMatrix *inputMinMax,
									const TCHAR *szTrainFcn /*= NULL */ )
{
	if (szName != NULL)
		_tcscpy_s(m_szName, _countof(m_szName), szName);
	else
		_tcscpy_s(m_szName, _countof(m_szName), _T(""));

	int _numInputs, _numLayers, _numOutputs;
	_numInputs = _numLayers = _numOutputs = 0;

	numInputs  = _numInputs;
	numLayers  = _numLayers;
	numOutputs = _numOutputs;
	numCounter = 0;

	// adaptFcn => 'trains'
	_tcscpy_s(m_szAdaptFcn, _countof(m_szAdaptFcn), NNET_ADAPTFCN_DEFAULT);
	// divideFcn => ''
	_tcscpy_s(m_szDivideFcn, _countof(m_szDivideFcn), NNET_DIVIDEFCN_DEFAULT);
	// initFcn => 'initlay'
	_tcscpy_s(m_szInitFcn, _countof(m_szInitFcn), NNET_INITFCN_DEFAULT);
	// gradientFcn => 'gdefaults'
	_tcscpy_s(m_szGradientFcn, _countof(m_szGradientFcn), NNET_GRADIENTFCN_DEFAULT);
	// performFcn => 'mse'
	_tcscpy_s(m_szPerformFcn, _countof(m_szPerformFcn), NNET_PERFORMFCN_DEFAULT);
	// trainFcn => 'traingd'
	_tcscpy_s(m_szTrainFcn, _countof(m_szTrainFcn), (szTrainFcn == NULL) ? NNET_TRAINFCN_DEFAULT : szTrainFcn);

	return TRUE;
}

BOOL CAnnNetwork::commonConstructor( const TCHAR *szSizesOfLayers,
									const TCHAR *szTransFcnOfLayers,
									const CxMatrix *inputMinMax, /*= NULL */
									const TCHAR *szTrainFcn /*= NULL */)
{
	return commonConstructor(_T(""), szSizesOfLayers, szTransFcnOfLayers, inputMinMax, szTrainFcn);
}

BOOL CAnnNetwork::createEx( const TCHAR *szName,
						   const TCHAR *szSizesOfLayers,
						   const TCHAR *szTransFcnOfLayers,
						   const CxMatrix *inputMinMax,
						   const TCHAR *szTrainFcn /*= NULL */ )
{
	BOOL bResult;
	bResult = commonConstructor(szName, szSizesOfLayers, szTransFcnOfLayers, inputMinMax, szTrainFcn);
	return bResult;
}

BOOL CAnnNetwork::create( const TCHAR *szSizesOfLayers,
						 const TCHAR *szTransFcnOfLayers,
						 const CxMatrix *inputMinMax, /*= NULL */
						 const TCHAR *szTrainFcn /*= NULL */ )
{
	BOOL bResult;
	bResult = createEx(_T(""), szSizesOfLayers, szTransFcnOfLayers, inputMinMax, szTrainFcn);
	return bResult;
}

void CAnnNetwork::setAdaptFcn( const TCHAR *szFcnName )
{
	setFcnName(m_szAdaptFcn, _countof(m_szAdaptFcn), szFcnName);
}

void CAnnNetwork::setDivideFcn( const TCHAR *szFcnName )
{
	setFcnName(m_szDivideFcn, _countof(m_szDivideFcn), szFcnName);
}

void CAnnNetwork::setInitFcn( const TCHAR *szFcnName )
{
	setFcnName(m_szInitFcn, _countof(m_szInitFcn), szFcnName);
}

void CAnnNetwork::setGradientFcn( const TCHAR *szFcnName )
{
	setFcnName(m_szGradientFcn, _countof(m_szGradientFcn), szFcnName);
}

void CAnnNetwork::setPerformFcn( const TCHAR *szFcnName )
{
	setFcnName(m_szPerformFcn, _countof(m_szPerformFcn), szFcnName);
}

void CAnnNetwork::setTrainFcn( const TCHAR *szFcnName )
{
	setFcnName(m_szTrainFcn, _countof(m_szTrainFcn), szFcnName);
}

int CAnnNetwork::clearNetLayers( void )
{
	numInputs  = 0;
	numLayers  = 0;
	numOutputs = 0;
	numCounter = 0;

	input.clear();
	output.clear();
	inputRange.clear();

	IW.clear();
	LW.clear();
	b.clear();

	inputWeights.clear();
	layerWeights.clear();
	biases.clear();

	inputs.clear();
	outputs.clear();

	int _numLayers = layers.clear();
	return _numLayers;
}

int CAnnNetwork::appendNetLayer( int _prevNeurons, int _numNeurons )
{
	int _numLayers = -1;
	if (_numNeurons > 0) {
		_numLayers = layers.append(_prevNeurons, _numNeurons);
		if (_numLayers >= 0) {
			if (numCounter == 0) {
				numInputs++;
			}
			else {
				if (_prevNeurons > 0) {
					if (numCounter == 1) {
						/*
						CxMatrix *inputWeight = new CxMatrix(_prevNeurons, _numNeurons);
						if (inputWeight != NULL) {
							IW.push_back(*inputWeight);
							delete inputWeight;
						}
						//*/
						IW.resize(_prevNeurons, _numNeurons);
					}
					else {
						CxMatrix *layerWeight = new CxMatrix(_prevNeurons, _numNeurons);
						if (layerWeight != NULL) {
							LW.push_back(*layerWeight);
							delete layerWeight;
						}
						CxMatrix *biase = new CxMatrix(_numNeurons, 1);
						if (biase != NULL) {
							b.push_back(*biase);
							delete biase;
						}
					}
					numLayers++;
				}
			}
			numCounter++;
		}
	}
	return _numLayers;
}

int CAnnNetwork::appendNetLayer( int _numNeurons )
{
	return appendNetLayer(0, _numNeurons);
}

BOOL CAnnNetwork::removeNetLayer( int _index )
{
	return TRUE;
}

BOOL CAnnNetwork::modifyNetLayer( int _index, int _numNeurons )
{
	return TRUE;
}

int CAnnNetwork::trimString(const TCHAR *szString, TCHAR *szBuffer, DWORD dwBufSize)
{
	ASSERT(szBuffer != NULL && szString != NULL && dwBufSize != 0);
	if (szBuffer == NULL || szString == NULL || dwBufSize == 0)
		return -1;

	ASSERT(szBuffer != szString);
	if (szBuffer == szString)
		return -1;

	TCHAR *pszHead, *pszTail;
	pszHead = (TCHAR *)szString;
	pszTail = (TCHAR *)szString + _tcslen(szString) - 1;
	while (pszHead <= pszTail && _tcsncicmp(pszHead, _T(" "), 1) <= 0)
		pszHead++;

	if (pszHead > pszTail) {
		_tcscpy_s(szBuffer, dwBufSize, _T(""));
		return 0;
	}
	else {
		while (_tcsncicmp(pszTail, _T(" "), 1) <= 0)
			pszTail--;

		if (pszTail < pszHead) {
			_tcscpy_s(szBuffer, dwBufSize, _T(""));
			return 0;
		}
		else {
			int nNewLength = pszTail - pszHead + 1;
			if (nNewLength <= (int)dwBufSize) {
				if (nNewLength > 0)
					_tcsncpy_s(szBuffer, dwBufSize, pszHead, nNewLength);
				szBuffer[nNewLength] = 0;
				return nNewLength;
			}
		}
	}
	return -1;
}

int CAnnNetwork::parseNetLayers( const TCHAR *szSizesOfLayers,
								CxNetLayers *pNetLayers,
								int _inNumLayers, /*= 0 */
								const TCHAR *szDelim /*= NULL */ )
{
	TCHAR *pszOffset, *pszEndOf, *pszDest, *pszDelim;
	TCHAR szBuffer[512];
	DWORD dwBufSize = 512;

	TRACE(_T("CAnnNetwork::parseNetLayers() Enter.\n"));
	if (szSizesOfLayers == NULL)
		return -1;

	int lenNumber, lenDelim;
	int _indexLayer = 0, _numNeuron;
	int prevNeuron = 0;

	pszDelim = (szDelim == NULL) ? _T(",") : szDelim;
	lenDelim = _tcslen(pszDelim);

	pszOffset = (TCHAR *)szSizesOfLayers;
	pszEndOf  = (TCHAR *)szSizesOfLayers + _tcslen(szSizesOfLayers);
	while (pszOffset <= pszEndOf) {
		pszDest = _tcsstr(pszOffset, pszDelim);
		if (pszDest != NULL)
			lenNumber = pszDest - pszOffset;
		else
			lenNumber = pszEndOf - pszOffset;
		if (lenNumber > 0) {
			if (pNetLayers != NULL && _inNumLayers != 0) {
				if (lenNumber > (int)dwBufSize)
					lenNumber = dwBufSize - 1;
				memcpy_s(szBuffer, _countof(szBuffer), pszOffset, sizeof(TCHAR) * lenNumber);
				szBuffer[lenNumber] = 0;
				_numNeuron = _tstoi(szBuffer);
				if (_numNeuron > 0) {
					if (_indexLayer == 0) {
						clearNetLayers();
					}
					int _index = appendNetLayer(prevNeuron, _numNeuron);
					if (_index >= 0) {
						if (_indexLayer == 0) {
							layers[_index].type = LAYER_TYPE_INPUT;
						}
						else if (_indexLayer == _inNumLayers - 1) {
							layers[_index].type = LAYER_TYPE_OUTPUT;
						}
						else {
							layers[_index].type = LAYER_TYPE_HIDDEN;
						}
					}
				}
				TRACE(_T("numNeuron[%d] = %d.\n"), _indexLayer, _numNeuron);
				prevNeuron = _numNeuron;
			}
			_indexLayer++;
		}
		if (pszDest != NULL)
			pszOffset += lenNumber + lenDelim;
		else
			break;
	}
	TRACE(_T("CAnnNetwork::parseNetLayers() Exit: _indexLayer = %d.\n"), _indexLayer);
	return _indexLayer;
}

int CAnnNetwork::parseTransFcns( const TCHAR *szTransFcnOfLayers,
								CxNetLayers *pNetLayers,
								const TCHAR *szDelim /*= NULL */ )
{
	TCHAR *pszOffset, *pszEndOf, *pszDest, *pszDelim;
	TCHAR szBuffer[512], szFcnName[512];
	DWORD dwBufSize = 512;

	TRACE(_T("CAnnNetwork::parseTransFcns() Enter.\n"));
	if (szTransFcnOfLayers == NULL)
		return -1;

	int lenFcnName, lenDelim;
	int _indexFcns = 0;

	pszDelim = (szDelim == NULL) ? _T(",") : szDelim;
	lenDelim = _tcslen(pszDelim);

	pszOffset = (TCHAR *)szTransFcnOfLayers;
	pszEndOf  = (TCHAR *)szTransFcnOfLayers + _tcslen(szTransFcnOfLayers);
	while (pszOffset <= pszEndOf) {
		pszDest = _tcsstr(pszOffset, pszDelim);
		if (pszDest != NULL)
			lenFcnName = pszDest - pszOffset;
		else
			lenFcnName = pszEndOf - pszOffset;
		if (lenFcnName > 0) {
			if (pNetLayers != NULL) {
				if (lenFcnName > (int)dwBufSize)
					lenFcnName = dwBufSize - 1;
				memcpy_s(szBuffer, _countof(szBuffer), pszOffset, sizeof(TCHAR) * lenFcnName);
				szBuffer[lenFcnName] = 0;
				int nRetSize = trimString(szBuffer, szFcnName, _countof(szFcnName));
				if (nRetSize > 0) {
					if (_indexFcns >= 0 && _indexFcns < layers.numLayers)
						layers[_indexFcns+1].setTransferFcn(szFcnName);
				}
				TRACE(_T("transFcn[%d] = %s. (nRetSize = %d)\n"), _indexFcns, szFcnName, nRetSize);
			}
			_indexFcns++;
		}
		if (pszDest != NULL)
			pszOffset += lenFcnName + lenDelim;
		else
			break;
	}
	TRACE(_T("CAnnNetwork::parseTransFcns() Exit: _indexFcns = %d.\n"), _indexFcns);
	return _indexFcns;
}

CAnnNetwork * CAnnNetwork::newff( const CxMatrix *_inputMinMax, const TCHAR *szSizesOfLayers,
								 const TCHAR *szTransFcnOfLayers, const TCHAR *szTrainFcn,
								 BOOL bInitWeightsNow /*= FALSE */ )
{
	CAnnNetwork *pNetwork = NULL;
	int _numLayers, _numTransFcns;
	// parseNetLayers
	_numLayers = parseNetLayers(szSizesOfLayers, NULL);
	if (_numLayers <= 0)
		return NULL;
	_numLayers = parseNetLayers(szSizesOfLayers, &layers, _numLayers);

	// parseTransFcns
	_numTransFcns = parseTransFcns(szTransFcnOfLayers, NULL);
	if (_numTransFcns <= 0 || _numTransFcns != _numLayers - 1 )
		return NULL;
	_numTransFcns = parseTransFcns(szTransFcnOfLayers, &layers);

	if (_inputMinMax != NULL) {
		inputRange.copy(_inputMinMax);
	}

	TCHAR szBuffer[512];
	TCHAR szText[512];
	int nRetSize;
	_tcscpy_s(szText, _countof(szText), _T(" "));
	nRetSize = trimString(szText, szBuffer, _countof(szBuffer));
	TRACE(_T("trimString(\"%s\") = %s. (nRetSize = %d)\n"), szText, szBuffer, nRetSize);

	pNetwork = this;
	return pNetwork;
}

CAnnNetwork * CAnnNetwork::init( void )
{
	CAnnNetwork *pNetwork = NULL;
	int _numInputs, _numNeurons;
	CxMatrix _inputRange, _activeRange;
	int _rows, _cols;

	for (int _index=1; _index<=numLayers; _index++) {
		CxNetLayer *pNetLayer = layers.getLayer(_index);
		if (pNetLayer != NULL) {
			_numInputs  = pNetLayer->prevNeurons;
			_numNeurons = pNetLayer->numNeurons;
			TRACE(_T("_tcsicmp(pNetLayer->initFcn(): [%s]\n"), pNetLayer->initFcn());
			if (_tcsicmp(pNetLayer->initFcn(), _T("initnw")) == 0
				|| _tcsicmp(pNetLayer->initFcn(), _T("initlay")) == 0) {
				// 'initnw': Nguyen-Widrow method
				if (_index == 1) {
					initnw(&IW, _index, _numInputs, _numNeurons, &_inputRange, &_activeRange);
				}
				else if (_index >= 2) {
					//CxMatrix *pMatrix = LW[_index - 2];
					initnw(LW[_index - 2], _index, _numInputs, _numNeurons, &_inputRange, &_activeRange);
				}
			}
			if (_tcsicmp(pNetLayer->initFcn(), _T("initwb")) == 0) {
				// 'initwb': Init weights and biases
				if (_index == 1) {
					initwb(&IW, _numInputs, _index, _numNeurons, &_inputRange, &_activeRange);
				}
				else if (_index >= 2) {
					//CxMatrix *pMatrix = LW[_index - 2];
					initwb(LW[_index - 2], _index, _numInputs, _numNeurons, &_inputRange, &_activeRange);
				}
			}
			else {
				// dafault is 'initwb' and 'rands'
				if (_index == 1) {
					_rows = IW.rows;
					_cols = IW.cols;
					IW.rands(_rows, _cols);
				}
				else {
					CxMatrix *pMatrix = LW[_index - 2];
					if (pMatrix != NULL) {
						_rows = pMatrix->rows;
						_cols = pMatrix->cols;
						pMatrix->rands(_rows, _cols);
					}
				}
			}
		}
	}
	pNetwork = this;
	return pNetwork;
}

CAnnNetwork * CAnnNetwork::train( const CxMatrixList *trainP, const CxMatrixList *trainT,
								 CxTrainRecord *tr )
{
	CAnnNetwork *pNetwork = NULL;
	pNetwork = this;
	return pNetwork;
}

/*****************************************************************
	function [w,b] = calcnw(pr,s,n)
	%CALCNW Calculates Nguyen-Widrow initial conditions.
	%
	%  PR
	%  S - Number of neurons.
	%  N - Active region of transfer function N = [Nmin Nmax].

	r = size(pr,1);

	% Nguyen-Widrow Method
	% --------------------

	% Assume inputs and net inputs range in [-1 1].

	% Weights
	wMag = 0.7*s^(1/r);
	wDir = randnr(s,r);
	w = wMag*wDir;

	% Biases
	if (s==1)
		b = 0;
	else
		b = wMag*linspace(-1,1,s)'.*sign(w(:,1));
	end

	% Conversions
	% -----------

	% Conversion of net inputs of [-1 1] to [Nmin Nmax]
	x = 0.5*(n(2)-n(1));
	y = 0.5*(n(2)+n(1));
	w = x*w;
	b = x*b+y;

	% Conversion of inputs of PR to [-1 1]
	x = 2./(pr(:,2)-pr(:,1));
	y = 1-pr(:,2).*x;

	xp = x';
	b = w*y+b;
	w = w.*xp(ones(1,s),:);

*****************************************************************/

BOOL CAnnNetwork::initnw( CxMatrix *pMatrix, int _index, int _numInputs, int _numNeurons,
						   CxMatrix *pInputRange, CxMatrix *pActiveRange )
{
	TCHAR szText[512];
	// Assume inputs and net inputs range in [-1 1].

	// Weights
	double wMag;
	CxMatrix wDir, _weights, _biases;
	// wMag = 0.7*s^(1/r);
	wMag = 0.7 * pow((double)_numNeurons, 1.0/(double)_numInputs);
	// wDir = randnr(s,r);
	wDir = randnr(_numNeurons, _numInputs);
	//wDir.display(_T("wDir = randnr(_numNeurons, _numInputs);"));
	_weights = wMag * wDir;
	//_weights.display(_T("_weights = wMag * wDir;"));

	// Biases
	if (_numNeurons == 1) {
		_biases = 0;
	}
	else {
		// b = wMag*linspace(-1,1,s)'.*sign(w(:,1));
		_biases = wMag * dotprod(transpose(linspace(-1, 1, _numNeurons)), sign(_weights.getColVector(0)));
	}

	// Conversions

	// Conversion of net inputs of [-1 1] to [Nmin Nmax]
	double x, y, minActive, maxActive;
	minActive = -2.0;
	maxActive = 2.0;
	x = 0.5 * (maxActive - minActive);
	y = 0.5 * (maxActive + minActive);

	_weights = x * _weights;
	_biases  = x * _biases + y;

	// % Conversion of inputs of PR to [-1 1]
	double minInput, maxInput;
	minInput = -1.0;
	maxInput = 1.0;

	x = 2.0 / (maxInput - minInput);
	y = 1.0 - maxInput * x;

	CxMatrix mx, my, mxp;
	mx = x * ones(_numInputs, 1);
	my = y * ones(_numInputs, 1);

	// mxp = mx';
	mxp = transpose(mx);

	// w = w.*mxp(ones(1,s),:);
	_weights = dotprod(_weights, expand_row(mxp ,_numNeurons));
	_stprintf_s(szText, _countof(szText), _T("_weights[%d]"), _index);
	_weights.display(szText);

	// b = w*my+b;
	_biases  = _weights * my + _biases;
	_stprintf_s(szText, _countof(szText), _T("_biases[%d]"), _index);
	_biases.display(szText);
	return TRUE;
}

BOOL CAnnNetwork::initwb( CxMatrix *pMatrix, int _index, int _numInputs, int _numNeurons,
						 CxMatrix *pInputRange, CxMatrix *pActiveRange )
{
	return TRUE;
}
