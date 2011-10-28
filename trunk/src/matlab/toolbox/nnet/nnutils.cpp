
#include "../../../../include/matlab/toolbox/nnet/nnutils.h"

namespace matlab {

int trimString(const TCHAR *szString, TCHAR *szBuffer, DWORD dwBufSize)
{
	__MY_ASSERT(szBuffer != NULL && szString != NULL && dwBufSize != 0);
	if (szBuffer == NULL || szString == NULL || dwBufSize == 0)
		return -1;

	__MY_ASSERT(szBuffer != szString);
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

/******************************************************************************
%
% function [perf,El,Y,Ac,N,BZ,IWZ,LWZ]=calcperf2(net,X,PD,T,Ai,Q,TS)
%   CALCPERF Calculate network outputs, signals, and performance.
%
%	Synopsis
%
%	  [perf,El,Ac,N,BZ,IWZ,LWZ]=calcperf(net,X,Pd,Tl,Ai,Q,TS)
%
%	Description
%
%	  This function calculates the outputs of each layer in
%	  response to a networks delayed inputs and initial layer
%	  delay conditions.
%
%	  [perf,El,Ac,N,LWZ,IWZ,BZ] = CALCPERF(NET,X,Pd,Tl,Ai,Q,TS) takes,
%	    NET - Neural network.
%	    X   - Network weight and bias values in a single vector.
%	    Pd  - Delayed inputs.
%	    Tl  - Layer targets.
%	    Ai  - Initial layer delay conditions.
%	    Q   - Concurrent size.
%	    TS  - Time steps.
%	  and returns,
%	    perf - Network performance.
%	    El   - Layer errors.
%	    Ac   - Combined layer outputs = [Ai, calculated layer outputs].
%	    N    - Net inputs.
%	    LWZ  - Weighted layer outputs.
%	    IWZ  - Weighted inputs.
%	    BZ   - Concurrent biases.
%
%	Examples
%
%	  Here we create a linear network with a single input element
%	  ranging from 0 to 1, two neurons, and a tap delay on the
%	  input with taps at 0, 2, and 4 timesteps.  The network is
%	  also given a recurrent connection from layer 1 to itself with
%	  tap delays of [1 2].
%
%	    net = newlin([0 1],2);
%	    net.layerConnect(1,1) = 1;
%	    net.layerWeights{1,1}.delays = [1 2];
%
%	  Here is a single (Q = 1) input sequence P with 5 timesteps (TS = 5),
%	  and the 4 initial input delay conditions Pi, combined inputs Pc,
%	  and delayed inputs Pd.
%
%	    P = {0 0.1 0.3 0.6 0.4};
%	    Pi = {0.2 0.3 0.4 0.1};
%	    Pc = [Pi P];
%	    Pd = calcpd(net,5,1,Pc);
%
%	  Here the two initial layer delay conditions for each of the
%	  two neurons are defined.
%
%	    Ai = {[0.5; 0.1] [0.6; 0.5]};
%
%	  Here we define the layer targets for the two neurons for
%	  each of the five time steps.
%	  
%	    Tl = {[0.1;0.2] [0.3;0.1], [0.5;0.6] [0.8;0.9], [0.5;0.1]};
%
%	  Here the network's weight and bias values are extracted.
%
%	    X = getx(net);
%
%	  Here we calculate the network's combined outputs Ac, and other
%	  signals described above..
%
%	    [perf,El,Ac,N,BZ,IWZ,LWZ] = calcperf(net,X,Pd,Tl,Ai,1,5)
%
******************************************************************************/

int calcperf2( CAnnNetwork *net, CAnnXArray *X, double *Pd, double *Tl, double *Ai, int Q, int TS,
			  double *perf, double *El, double *Ac, double *N, double *LWZ, double *IWZ, double *BZ )
{
	int nRetCode = ERR_NN_NONE;
	int retval = 0;

	__MY_ASSERT(net != NULL && X != NULL);
	if (net == NULL || X == NULL)
		return ERR_NN_INVALID_PARAM;

	TCHAR szBuffer[512];
	TCHAR szText[512];
	_tcscpy_s(szText, _countof(szText), _T(" "));
	nRetCode = trimString(szText, szBuffer, _countof(szBuffer));
	__MY_TRACE(_T("calcperf2(\"%s\") = %s. (nRetcode = %d)\n"), szText, szBuffer, nRetCode);

	return nRetCode;
}

}  // namespace matlab
