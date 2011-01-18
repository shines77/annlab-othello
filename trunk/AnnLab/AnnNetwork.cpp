#include "StdAfx.h"
#include "AnnGlobal.h"
#include "AnnNetwork.h"
#include <excpt.h>
#include <Winbase.h>

#define ADAPTFCN_DEFAULT      _T("trains")
#define DIVIDEFCN_DEFAULT     _T("")
#define INITFCN_DEFAULT       _T("initlay")
#define GRADIENTFCN_DEFAULT   _T("gdefaults")
#define PERFORMFCN_DEFAULT    _T("mse")
#define TRAINFCN_DEFAULT      _T("traingd")

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
	show       = TRUE;			// 显示训练过程
	showWindow = TRUE;			// 显示训练窗口
	showCommandLine = TRUE;		// 显示命令行窗口

	epochs   = 0;				// 最大迭代次数 1-∞
	max_fail = 6;				// 最大验证失败次数

	lr   = 0.05;				// 学习率 (0.01-0.5)
	mc   = 0.9;					// 动量因子 (0.8-0.99)
	goal = 0.0001;				// 训练目标精度
	time = 0.0;					// 最大迭代时间 默认为0, 表示不限制

	min_grad = 1E-6;			// Minimum performance gradient
	max_grad = 1.0;				// Maximum performance gradient

	alpha   = 0.001;			// Scale factor which determines sufficient reduction in perf.
	beta    = 0.1;				// Scale factor which determines sufficiently large step size.
	delta   = 0.01;				// Initial step size in interval location step.
	gama    = 0.1;				// Parameter to avoid small reductions in performance. Usually set to 0.1. (See use in srch_cha.)
	minstep = 1E-6;				// Minimum step length.
	maxstep = 100;				// Maximum step length.
	low_lim = 0.1;				// Lower limit on change in step size.
	up_lim  = 0.5;				// Upper limit on change in step size.
	bmax    = 26.0;				// Maximum step size.

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
// CxNetLayer
//////////////////////////////////////////////////////////////////

CxNetLayer::CxNetLayer( void )
{
	index       = -1;
	type        = LAYER_TYPE_NONE;
	numNeurons  = 0;
	prevNeurons = 0;

	prevLayer   = NULL;
	nextLayer   = NULL;

	_tcscpy_s(m_szInitFcn,     _countof(m_szInitFcn),     _T(""));
	_tcscpy_s(m_szTransferFcn, _countof(m_szTransferFcn), _T(""));
}

CxNetLayer::CxNetLayer( int _numNeurons )
{
	index       = -1;
	type        = LAYER_TYPE_NONE;
	numNeurons  = _numNeurons;
	prevNeurons = 0;

	prevLayer   = NULL;
	nextLayer   = NULL;

	_tcscpy_s(m_szInitFcn,     _countof(m_szInitFcn),     _T(""));
	_tcscpy_s(m_szTransferFcn, _countof(m_szTransferFcn), _T(""));
}

CxNetLayer::CxNetLayer( int _prevNeurons, int _numNeurons )
{
	index       = -1;
	type        = LAYER_TYPE_NONE;
	numNeurons  = _numNeurons;
	prevNeurons = _prevNeurons;

	prevLayer   = NULL;
	nextLayer   = NULL;

	_tcscpy_s(m_szInitFcn,     _countof(m_szInitFcn),     _T(""));
	_tcscpy_s(m_szTransferFcn, _countof(m_szTransferFcn), _T(""));
}

CxNetLayer::~CxNetLayer( void )
{
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
	numLayers    = 0;
	m_firstLayer = NULL;
	m_lastLayer  = NULL;
}

CxNetLayers::CxNetLayers( int _numLayers )
{
	numLayers    = 0;
	m_firstLayer = NULL;
	m_lastLayer  = NULL;

	initLayers(_numLayers);
}

CxNetLayers::~CxNetLayers( void )
{
	freeLayers();
}

int CxNetLayers::initLayers( int _numLayers )
{
	int retNumLayers = 0;
	if (_numLayers > 0)
	{
		freeLayers();

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

	//if (_numLayers == numLayers) {
		numLayers = 0;
		m_firstLayer = NULL;
		m_lastLayer  = NULL;
	//}
}

BOOL CxNetLayers::setNumLayers( int _numLayers )
{
	return initLayers(_numLayers);
}

CxNetLayer & CxNetLayers::operator [] ( int _index ) const
{
	CxNetLayer &layer = (CxNetLayer &)layers(_index);

	/*
	int indLayer = -1;
	__try {
		indLayer = layer.index;
	}
	__except (filter(GetExceptionCode(), GetExceptionInformation())) {
		indLayer = 100;
	}
	//*/
	return layer;
};

CxNetLayer & CxNetLayers::layers( int _index ) const
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
	//throw "layers[index] failure!";
	//RaiseException( EXCEPTION_ACCESS_VIOLATION, 0, 0, 0 );
	// Create temp 1 layer
	CxNetLayer *pNetLayers = new CxNetLayer();
	if (pNetLayers != NULL) {
		pNetLayers->index = -1;
		return (CxNetLayer &)(*pNetLayers);
	}
	//throw "layers temp create failure!";
	RaiseException( EXCEPTION_INVALID_HANDLE, 0, 0, 0 );
	return *(new CxNetLayer());
}

CxNetLayer * CxNetLayers::getLayer( int _index ) const
{
	CxNetLayer *nextLayer;
	CxNetLayer *curLayer = m_firstLayer;
	int _nowIndex = 0, _numNeurons = -1;

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
	freeLayers();
	numLayers = 0;
	return oldLayers;
}

int CxNetLayers::append( CxNetLayer *pNetLayer )
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

BOOL CxNetLayers::remove( int _index )
{
	return FALSE;
}

BOOL CxNetLayers::modify( int _index, int _numNeuron )
{
	return FALSE;
}

//////////////////////////////////////////////////////////////////
// CBaseAnnNetwork
//////////////////////////////////////////////////////////////////

CBaseAnnNetwork::CBaseAnnNetwork( void )
{

}

CBaseAnnNetwork::CBaseAnnNetwork( const TCHAR *szName )
{

}

CBaseAnnNetwork::~CBaseAnnNetwork( void )
{

}

//////////////////////////////////////////////////////////////////
// CAnnNetwork
//////////////////////////////////////////////////////////////////

CAnnNetwork::CAnnNetwork( void )
{
	CommonConstructor(_T(""), _T(""));
}

CAnnNetwork::CAnnNetwork( const TCHAR *szName )
{
	CommonConstructor(szName, _T(""), _T(""), NULL);
}

CAnnNetwork::~CAnnNetwork( void )
{
	FreeNetwork();
}

void CAnnNetwork::FreeNetwork( void )
{
	//
}

BOOL CAnnNetwork::CommonConstructor( const TCHAR *szName,
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

	// adaptFcn => 'trains'
	_tcscpy_s(m_szAdaptFcn, _countof(m_szAdaptFcn), ADAPTFCN_DEFAULT);
	// divideFcn => ''
	_tcscpy_s(m_szDivideFcn, _countof(m_szDivideFcn), DIVIDEFCN_DEFAULT);
	// initFcn => 'initlay'
	_tcscpy_s(m_szInitFcn, _countof(m_szInitFcn), INITFCN_DEFAULT);
	// gradientFcn => 'gdefaults'
	_tcscpy_s(m_szGradientFcn, _countof(m_szGradientFcn), GRADIENTFCN_DEFAULT);
	// performFcn => 'mse'
	_tcscpy_s(m_szPerformFcn, _countof(m_szPerformFcn), PERFORMFCN_DEFAULT);
	// trainFcn => 'traingd'
	_tcscpy_s(m_szTrainFcn, _countof(m_szTrainFcn), (szTrainFcn == NULL) ? TRAINFCN_DEFAULT : szTrainFcn);

	return TRUE;
}

BOOL CAnnNetwork::CommonConstructor( const TCHAR *szSizesOfLayers,
									const TCHAR *szTransFcnOfLayers,
									const CxMatrix *inputMinMax, /*= NULL */
									const TCHAR *szTrainFcn /*= NULL */)
{
	return CommonConstructor(_T(""), szSizesOfLayers, szTransFcnOfLayers, inputMinMax, szTrainFcn);
}

BOOL CAnnNetwork::CreateEx( const TCHAR *szName,
						   const TCHAR *szSizesOfLayers,
						   const TCHAR *szTransFcnOfLayers,
						   const CxMatrix *inputMinMax,
						   const TCHAR *szTrainFcn /*= NULL */ )
{
	BOOL bResult;
	bResult = CommonConstructor(szName, szSizesOfLayers, szTransFcnOfLayers, inputMinMax, szTrainFcn);
	return bResult;
}

BOOL CAnnNetwork::Create( const TCHAR *szSizesOfLayers,
						 const TCHAR *szTransFcnOfLayers,
						 const CxMatrix *inputMinMax, /*= NULL */
						 const TCHAR *szTrainFcn /*= NULL */ )
{
	BOOL bResult;
	bResult = CreateEx(_T(""), szSizesOfLayers, szTransFcnOfLayers, inputMinMax, szTrainFcn);
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
	return layers.clear();
}

int CAnnNetwork::appendNetLayer( int _numNeurons )
{
	if (_numNeurons > 0)
		return layers.append(_numNeurons);
	else
		return -1;
}

int CAnnNetwork::appendNetLayer( int _prevNeurons, int _numNeurons )
{
	if (_numNeurons > 0)
		return layers.append(_prevNeurons, _numNeurons);
	else
		return -1;
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
					if (_indexLayer == 0)
						clearNetLayers();
					int _index = appendNetLayer(prevNeuron, _numNeuron);
					if (_index >= 0) {
						if (_indexLayer == 0)
							layers[_index].type = LAYER_TYPE_INPUT;
						else if (_indexLayer == _inNumLayers - 1)
							layers[_index].type = LAYER_TYPE_OUTPUT;
						else
							layers[_index].type = LAYER_TYPE_HIDDEN;
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
	TRACE(_T("parseNetLayers(): _indexLayer = %d.\n"), _indexLayer);
	return _indexLayer;
}

int CAnnNetwork::parseTransFcns( const TCHAR *szTransFcnOfLayers,
								CxNetLayers *pNetLayers,
								const TCHAR *szDelim /*= NULL */ )
{
	TCHAR *pszOffset, *pszEndOf, *pszDest, *pszDelim;
	TCHAR szBuffer[512], szFcnName[512];
	DWORD dwBufSize = 512;
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
	TRACE(_T("parseTransFcns(): _indexFcns = %d.\n"), _indexFcns);
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
		inputMinMax.copy(_inputMinMax);
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
	pNetwork = this;
	return pNetwork;
}

CAnnNetwork * CAnnNetwork::train( const CxMatrix *trainP, const CxMatrix *trainT,
								 CxTrainRecord *trainRecord )
{
	CAnnNetwork *pNetwork = NULL;
	pNetwork = this;
	return pNetwork;
}
