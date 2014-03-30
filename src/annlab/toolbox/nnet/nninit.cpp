
#include "../../../../include/annlab/toolbox/nnet/nninit.h"
#include "../../../../include/annlab/toolbox/matlab/elmat.h"
#include <math.h>

namespace annlab {

int init( CAnnNetwork *net )
{
	int nRetCode = ERR_NN_NONE;

	if (net == NULL)
		return ERR_NN_INVALID_PARAM;

	int _numInputs, _numNeurons;
	CAnnMatrix _inputRange, _activeRange;
	int _rows, _cols;

	for (int _index=1; _index<=net->numLayers; _index++) {
		CAnnNetLayer *pNetLayer = net->layers.getLayer(_index);
		if (pNetLayer != NULL) {
			_numInputs  = pNetLayer->prevNeurons;
			_numNeurons = pNetLayer->numNeurons;
			__MY_TRACE(_T("_tcsicmp(pNetLayer->initFcn(): [%s]\n"), pNetLayer->initFcn());
			if (_tcsicmp(pNetLayer->initFcn(), _T("initnw")) == 0
				|| _tcsicmp(pNetLayer->initFcn(), _T("initlay")) == 0) {
					// 'initnw': Nguyen-Widrow method
					if (_index == 1) {
						initnw(net, &net->IW, _index, _numInputs, _numNeurons, &_inputRange, &_activeRange);
					}
					else if (_index >= 2) {
						//CxMatrix *pMatrix = LW[_index - 2];
						initnw(net, net->LW[_index - 2], _index, _numInputs, _numNeurons, &_inputRange, &_activeRange);
					}
			}
			if (_tcsicmp(pNetLayer->initFcn(), _T("initwb")) == 0) {
				// 'initwb': Init weights and biases
				if (_index == 1) {
					initwb(net, &net->IW, _numInputs, _index, _numNeurons, &_inputRange, &_activeRange);
				}
				else if (_index >= 2) {
					//CxMatrix *pMatrix = LW[_index - 2];
					initwb(net, net->LW[_index - 2], _index, _numInputs, _numNeurons, &_inputRange, &_activeRange);
				}
			}
			else {
				// dafault is 'initwb' and 'rands'
				if (_index == 1) {
					_rows = net->IW.rows;
					_cols = net->IW.cols;
					net->IW.rands(_rows, _cols);
				}
				else {
					CAnnMatrix *pMatrix = net->LW[_index - 2];
					if (pMatrix != NULL) {
						_rows = pMatrix->rows;
						_cols = pMatrix->cols;
						pMatrix->rands(_rows, _cols);
					}
				}
			}
		}
	}
	return nRetCode;
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

int initnw( CAnnNetwork *net,
			CAnnMatrix *matrix, int _index, int _numInputs, int _numNeurons,
			CAnnMatrix *inputRange, CAnnMatrix *activeRange )
{
	int nRetCode = ERR_NN_NONE;

	if (net == NULL)
		return ERR_NN_INVALID_PARAM;

	TCHAR szText[512];
	// Assume inputs and net inputs range in [-1 1].

	// Weights
	double wMag;
	CAnnMatrix wDir, _weights, _biases;
	// wMag = 0.7*s^(1/r);
	wMag = 0.7 * ::pow((double)_numNeurons, 1.0/(double)_numInputs);
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
		_biases = wMag * dotprod(transpose(linspace(-1, 1, _numNeurons)), sign(_weights.get_col_vector(0)));
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

	CAnnMatrix mx, my, mxp;
	mx = x * ones(_numInputs, 1);
	my = y * ones(_numInputs, 1);

	// mxp = mx';
	mxp = transpose(mx);

	// w = w.*mxp(ones(1,s),:);
	_weights = dotprod(_weights, expand_row(mxp ,_numNeurons));
	_stprintf_s(szText, _countof(szText), _T("_weights[%d]"), _index);
	_weights.display(szText);

	// b = w*my+b;
	_biases = _weights * my + _biases;
	_stprintf_s(szText, _countof(szText), _T("_biases[%d]"), _index);
	_biases.display(szText);

	nRetCode = ERR_NN_SUCCEED;
	return nRetCode;
}

int initwb( CAnnNetwork *annNetwork,
			CAnnMatrix *matrix, int _index, int _numInputs, int _numNeurons,
			CAnnMatrix *inputRange, CAnnMatrix *activeRange )
{
	int nRetCode = ERR_NN_NONE;

	if (annNetwork == NULL)
		return ERR_NN_INVALID_PARAM;

	nRetCode = ERR_NN_SUCCEED;
	return nRetCode;
}

}  // namespace annlab
