
#include "StdAfx.h"
#include "nnnetwork.h"
#include "nnutils.h"

_USING_NAMESPACE_MATLAB;

_MATLAB_BEGIN

int parseNetLayers( CAnnNetwork *net,
				   const TCHAR *szSizesOfLayers,
				   CxNetLayers *pNetLayers,
				   int _inNumLayers /*= 0 */,
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
						net->clearNetLayers();
					}
					int _index = net->appendNetLayer(prevNeuron, _numNeuron);
					if (_index >= 0) {
						if (_indexLayer == 0) {
							net->layers[_index].type = LAYER_TYPE_INPUT;
						}
						else if (_indexLayer == _inNumLayers - 1) {
							net->layers[_index].type = LAYER_TYPE_OUTPUT;
						}
						else {
							net->layers[_index].type = LAYER_TYPE_HIDDEN;
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
	TRACE(_T("matlab::parseNetLayers() Exit: _indexLayer = %d.\n"), _indexLayer);
	return _indexLayer;
}

int parseTransFcns( CAnnNetwork *net,
				   const TCHAR *szTransFcnOfLayers,
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
					if (_indexFcns >= 0 && _indexFcns < net->layers.numLayers)
						net->layers[_indexFcns+1].setTransferFcn(szFcnName);
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
	TRACE(_T("matlab::parseTransFcns() Exit: _indexFcns = %d.\n"), _indexFcns);
	return _indexFcns;
}

/******************************************************************************
%
%  function net = newff(varargin)
%  NEWFF Create a feed-forward backpropagation network.
%
%  Syntax
%
%    net = newff(P,T,S,TF,BTF,BLF,PF,IPF,OPF,DDF)
%
%  Description
%
%    NEWFF(P,T,S,TF,BTF,BLF,PF,IPF,OPF,DDF) takes,
%      P  - RxQ1 matrix of Q1 representative R-element input vectors.
%      T  - SNxQ2 matrix of Q2 representative SN-element target vectors.
%      Si  - Sizes of N-1 hidden layers, S1 to S(N-1), default = [].
%            (Output layer size SN is determined from T.)
%      TFi - Transfer function of ith layer. Default is 'tansig' for
%            hidden layers, and 'purelin' for output layer.
%      BTF - Backprop network training function, default = 'trainlm'.
%      BLF - Backprop weight/bias learning function, default = 'learngdm'.
%      PF  - Performance function, default = 'mse'.
%      IPF - Row cell array of input processing functions.
%            Default is {'fixunknowns','remconstantrows','mapminmax'}.
%      OPF - Row cell array of output processing functions.
%            Default is {'remconstantrows','mapminmax'}.
%      DDF - Data division function, default = 'dividerand';
%    and returns an N layer feed-forward backprop network.
%
%    The transfer functions TF{i} can be any differentiable transfer
%    function such as TANSIG, LOGSIG, or PURELIN.
%
%    The training function BTF can be any of the backprop training
%    functions such as TRAINLM, TRAINBFG, TRAINRP, TRAINGD, etc.
%
%    *WARNING*: TRAINLM is the default training function because it
%    is very fast, but it requires a lot of memory to run.  If you get
%    an "out-of-memory" error when training try doing one of these:
%
%    (1) Slow TRAINLM training, but reduce memory requirements, by
%        setting NET.trainParam.mem_reduc to 2 or more. (See HELP TRAINLM.)
%    (2) Use TRAINBFG, which is slower but more memory efficient than TRAINLM.
%    (3) Use TRAINRP which is slower but more memory efficient than TRAINBFG.
%
%    The learning function BLF can be either of the backpropagation
%    learning functions such as LEARNGD, or LEARNGDM.
%
%    The performance function can be any of the differentiable performance
%    functions such as MSE or MSEREG.
%
%  Examples
%
%    load simplefit_dataset
%    net = newff(simplefitInputs,simplefitTargets,20);
%    net = train(net,simplefitInputs,simplefitTargets);
%    simplefitOutputs = sim(net,simplefitInputs);
%
%  Algorithm
%
%    Feed-forward networks consist of Nl layers using the DOTPROD
%    weight function, NETSUM net input function, and the specified
%    transfer functions.
%
%    The first layer has weights coming from the input.  Each subsequent
%    layer has a weight coming from the previous layer.  All layers
%    have biases.  The last layer is the network output.
%
%    Each layer's weights and biases are initialized with INITNW.
%
%    Adaption is done with TRAINS which updates weights with the
%    specified learning function. Training is done with the specified
%    training function. Performance is measured according to the specified
%    performance function.
%
%  See also NEWCF, NEWELM, SIM, INIT, ADAPT, TRAIN, TRAINS
%
******************************************************************************/

int newff( CAnnNetwork *net, const CxMatrix *_inputMinMax,
		  const TCHAR *szSizesOfLayers,
		  const TCHAR *szTransFcnOfLayers, const TCHAR *szTrainFcn,
		  BOOL bInitWeightsNow /*= FALSE */ )
{
	int _numLayers, _numTransFcns;
	// parseNetLayers
	_numLayers = parseNetLayers(net, szSizesOfLayers, NULL);
	if (_numLayers <= 0)
		return NULL;
	_numLayers = parseNetLayers(net, szSizesOfLayers, &net->layers, _numLayers);

	// parseTransFcns
	_numTransFcns = parseTransFcns(net, szTransFcnOfLayers, NULL);
	if (_numTransFcns <= 0 || _numTransFcns != _numLayers - 1)
		return NULL;
	_numTransFcns = parseTransFcns(net, szTransFcnOfLayers, &net->layers);

	if (_inputMinMax != NULL) {
		net->inputRange.copy(_inputMinMax);
	}

	TCHAR szBuffer[512];
	TCHAR szText[512];
	int nRetSize;
	_tcscpy_s(szText, _countof(szText), _T(" "));
	nRetSize = trimString(szText, szBuffer, _countof(szBuffer));
	TRACE(_T("trimString(\"%s\") = %s. (nRetSize = %d)\n"), szText, szBuffer, nRetSize);

	return 0;
}

_MATLAB_END
