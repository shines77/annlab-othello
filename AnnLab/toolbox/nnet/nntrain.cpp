
#include "StdAfx.h"
#include "nntrain.h"
#include "nnutils.h"

_USING_NAMESPACE_MATLAB;

_MATLAB_BEGIN

/******************************************************************************
%
% function [net,tr,Y,E,Xf,Af]=train(net,X,T,Xi,Ai,arg6,arg7)
%  TRAIN Train a neural network.
%
%  Syntax
%
%    [net,tr] = train(NET,X,T,Pi,Ai)
%
%  Description
%
%    TRAIN trains a network NET according to NET.trainFcn and
%    NET.trainParam.
%
%    TRAIN(NET,X,T,Xi,Ai) takes,
%      NET - Network.
%      X   - Network inputs.
%      T   - Network targets, default = zeros.
%      Xi  - Initial input delay conditions, default = zeros.
%      Ai  - Initial layer delay conditions, default = zeros.
%    and returns,
%      NET - New network.
%      TR  - Training record (epoch and perf).
%
%    Note that T is optional and need only be used for networks
%    that require targets.  Xi and Xf are also optional and need
%    only be used for networks that have input or layer delays.
%    Optional arguments VV and TV are described below.
%
%    TRAIN's signal arguments can have two formats: cell array or matrix.
%    
%    The cell array format is easiest to describe.  It is most
%    convenient for networks with multiple inputs and outputs,
%    and allows sequences of inputs to be presented:
%      X  - NixTS cell array, each element P{i,ts} is an RixQ matrix.
%      T  - NtxTS cell array, each element P{i,ts} is an VixQ matrix.
%      Xi - NixID cell array, each element Xi{i,k} is an RixQ matrix.
%      Ai - NlxLD cell array, each element Ai{i,k} is an SixQ matrix.
%      Y  - NOxTS cell array, each element Y{i,ts} is an UixQ matrix.
%      E  - NtxTS cell array, each element X{i,ts} is an VixQ matrix.
%      Xf - NixID cell array, each element Pf{i,k} is an RixQ matrix.
%      Af - NlxLD cell array, each element Af{i,k} is an SixQ matrix.
%    Where:
%      Ni = net.numInputs
%      Nl = net.numLayers
%      Nt = net.numOutputs
%      ID = net.numInputDelays
%      LD = net.numLayerDelays
%      TS = number of time steps
%      Q  = batch size
%      Ri = net.inputs{i}.size
%      Si = net.layers{i}.size
%      Vi = net.outputs{i}.size
%
%    The columns of Xi, Xf, Ai, and Af are ordered from the oldest delay
%    condition to most recent:
%      Xi{i,k} = input i at time ts=k-ID.
%      Xf{i,k} = input i at time ts=TS+k-ID.
%      Ai{i,k} = layer output i at time ts=k-LD.
%      Af{i,k} = layer output i at time ts=TS+k-LD.
%
%    The matrix format can be used if only one time step is to be
%    simulated (TS = 1).  It is convenient for network's with
%     only one input and output, but can be used with networks that
%     have more.
%
%    Each matrix argument is found by storing the elements of
%    the corresponding cell array argument into a single matrix:
%      P  - (sum of Ri)xQ matrix
%      T  - (sum of Vi)xQ matrix
%      Xi - (sum of Ri)x(ID*Q) matrix.
%      Ai - (sum of Si)x(LD*Q) matrix.
%      Y  - (sum of Ui)xQ matrix.
%      E  - (sum of Vi)xQ matrix
%      Xf - (sum of Ri)x(ID*Q) matrix.
%      Af - (sum of Si)x(LD*Q) matrix.
%
%  Examples
%
%    Here input P and targets T define a simple function which
%    we can plot:
%
%      p = [0 1 2 3 4 5 6 7 8];
%      t = [0 0.84 0.91 0.14 -0.77 -0.96 -0.28 0.66 0.99];
%      plot(p,t,'o')
%
%    Here NEWFF is used to create a two layer feed forward network.
%    The network will have a single hidden layer of 10 neurons.
%
%      net = newff(p,t,10);
%      y1 = sim(net,p)
%      plot(p,t,'o',p,y1,'x')
%
%    Here the network is trained for up to 50 epochs to a error goal of
%    0.01, and then resimulated.
%
%      net.trainParam.epochs = 50;
%      net.trainParam.goal = 0.01;
%      net = train(net,p,t);
%      y2 = sim(net,p)
%      plot(p,t,'o',p,y1,'x',p,y2,'*')
%      
%  Algorithm
%
%    TRAIN calls the function indicated by NET.trainFcn, using the
%    training parameter values indicated by NET.trainParam.
%
%    Typically one epoch of training is defined as a single presentation
%    of all input vectors to the network.  The network is then updated
%    according to the results of all those presentations.
%
%    Training occurs until a maximum number of epochs occurs, the
%    performance goal is met, or any other stopping condition of the
%    function NET.trainFcn occurs.
%
%    Some training functions depart from this norm by presenting only
%    one input vector (or sequence) each epoch. An input vector (or sequence)
%    is chosen randomly each epoch from concurrent input vectors (or sequences).
%    NEWC and NEWSOM return networks that use TRAINR, a training function
%    that does this.
%
%  See also INIT, REVERT, SIM, ADAPT

%  Mark Beale, 11-31-97
%  Copyright 1992-2008 The MathWorks, Inc.
%  $Revision: 1.11.4.9 $ $Date: 2008/06/20 08:04:29 $
%
******************************************************************************/

int train( CAnnNetwork *annNetwork,
		  CxTrainRecord *trainRecord,
		  CxMatrixList *trainP,
		  CxMatrixList *trainT,
		  CxTrainParam *tainParam )
{
	if (annNetwork == NULL)
		return ERR_NN_INVALID_PARAM;

	int nRetCode = ERR_NN_NONE;
	if (_tcsicmp(annNetwork->trainFcn(), _T("traincgf")) == 0) {
		nRetCode = traincgf(annNetwork, trainRecord, trainP, trainT, NULL);
	}
	return nRetCode;
}

/******************************************************************************
%
% function [net,tr] = traincgf(net,tr,trainV,valV,testV,varargin)
%  TRAINCGF Conjugate gradient backpropagation with Fletcher-Reeves updates.
%
%  Syntax
%  
%    [net,tr] = traincgf(net,tr,trainV,valV,testV)
%    info = traincgf('info')
%
%  Description
%
%    TRAINCGF is a network training function that updates weight and
%    bias values according to the conjugate gradient backpropagation
%     with Fletcher-Reeves updates.
%
%  TRAINCGF(NET,TR,TRAINV,VALV,TESTV) takes these inputs,
%      NET - Neural network.
%      TR  - Initial training record created by TRAIN.
%      TRAINV - Training data created by TRAIN.
%      VALV - Validation data created by TRAIN.
%      TESTV - Test data created by TRAIN.
%    and returns,
%      NET - Trained network.
%      TR  - Training record of various values over each epoch.
%
%    Each argument TRAINV, VALV and TESTV is a structure of these fields:
%      X  - NxTS cell array of inputs for N inputs and TS timesteps.
%           X{i,ts} is an RixQ matrix for ith input and ts timestep.
%      Xi - NxNid cell array of input delay states for N inputs and Nid delays.
%           Xi{i,j} is an RixQ matrix for ith input and jth state.
%      Pd - NxSxNid cell array of delayed input states.
%      T  - NoxTS cell array of targets for No outputs and TS timesteps.
%           T{i,ts} is an SixQ matrix for the ith output and ts timestep.
%      Tl - NlxTS cell array of targets for Nl layers and TS timesteps.
%           Tl{i,ts} is an SixQ matrix for the ith layer and ts timestep.
%      Ai - NlxTS cell array of layer delays states for Nl layers, TS timesteps.
%           Ai{i,j} is an SixQ matrix of delayed outputs for layer i, delay j.
%
%    Training occurs according to training parameters, with default values:
%      net.trainParam.show             25  Epochs between displays
%      net.trainParam.showCommandLine   0 generate command line output
%      net.trainParam.showWindow        1 show training GUI
%      net.trainParam.epochs          100  Maximum number of epochs to train
%      net.trainParam.goal              0  Performance goal
%      net.trainParam.time            inf  Maximum time to train in seconds
%      net.trainParam.min_grad       1e-6  Minimum performance gradient
%      net.trainParam.max_fail          5  Maximum validation failures
%      net.trainParam.searchFcn 'srchcha'  Name of line search routine to use.
%
%   Parameters related to line search methods (not all used for all methods):
%      net.trainParam.scal_tol         20  Divide into delta to determine tolerance for linear search.
%      net.trainParam.alpha         0.001  Scale factor which determines sufficient reduction in perf.
%      net.trainParam.beta            0.1  Scale factor which determines sufficiently large step size.
%      net.trainParam.delta          0.01  Initial step size in interval location step.
%      net.trainParam.gama            0.1  Parameter to avoid small reductions in performance. Usually set
%                                           to 0.1. (See use in SRCH_CHA.)
%      net.trainParam.low_lim         0.1  Lower limit on change in step size.
%      net.trainParam.up_lim          0.5  Upper limit on change in step size.
%      net.trainParam.maxstep         100  Maximum step length.
%      net.trainParam.minstep      1.0e-6  Minimum step length.
%      net.trainParam.bmax             26  Maximum step size.
%
%    TRAINCGF('info') returns useful information about this function.
%
%  Network Use
%
%    You can create a standard network that uses TRAINCGF with
%    NEWFF, NEWCF, or NEWELM.
%
%    To prepare a custom network to be trained with TRAINCGF:
%    1) Set NET.trainFcn to 'traincgf'.
%       This will set NET.trainParam to TRAINCGF's default parameters.
%    2) Set NET.trainParam properties to desired values.
%
%    In either case, calling TRAIN with the resulting network will
%    train the network with TRAINCGF.
%
%
%  Examples
%
%    Here is a problem consisting of inputs P and targets T that we would
%    like to solve with a network.
%
%      p = [0 1 2 3 4 5];
%      t = [0 0 0 1 1 1];
%
%    A feedforward network is created with a hidden layer of 2 neurons.
%
%      net = newff(p,t,2,{},'traincgf');
%      a = sim(net,p)
%
%    Here the network is trained and tested.
%
%      net = train(net,p,t);
%      a = sim(net,p)
%
%    See NEWFF, NEWCF, and NEWELM for other examples.
%
%  Algorithm
%
%    TRAINCGF can train any network as long as its weight, net input,
%    and transfer functions have derivative functions.
%
%     Backpropagation is used to calculate derivatives of performance
%    PERF with respect to the weight and bias variables X.  Each
%    variable is adjusted according to the following:
%
%       X = X + a*dX;
%
%     where dX is the search direction.  The parameter a is selected
%     to minimize the performance along the search direction.  The line
%     search function searchFcn is used to locate the minimum point.
%     The first search direction is the negative of the gradient of performance.
%     In succeeding iterations the search direction is computed from the new
%     gradient and the previous search direction, according to the
%     formula:
%
%       dX = -gX + dX_old*Z;
%
%     where gX is the gradient. The parameter Z can be computed in several 
%     different ways.  For the Fletcher-Reeves variation of conjugate gradient
%     it is computed according to
%
%      Z=normnew_sqr/norm_sqr;
%
%     where norm_sqr is the norm square of the previous gradient and
%     normnew_sqr is the norm square of the current gradient.
%    See page 78 of Scales (Introduction to Non-Linear Optimization 1985) for
%     a more detailed discussion of the algorithm.
%
%    Training stops when any of these conditions occur:
%    1) The maximum number of EPOCHS (repetitions) is reached.
%    2) The maximum amount of TIME has been exceeded.
%    3) Performance has been minimized to the GOAL.
%    4) The performance gradient falls below MINGRAD.
%    5) Validation performance has increased more than MAX_FAIL times
%       since the last time it decreased (when using validation).
%
%  See also NEWFF, NEWCF, TRAINGDM, TRAINGDA, TRAINGDX, TRAINLM,
%           TRAINCGP, TRAINCGB, TRAINSCG, TRAINOSS,
%           TRAINBFG.
%
%   References
%
%     Scales, Introduction to Non-Linear Optimization, 1985.

% Updated by Orlando De Jesús, Martin Hagan, 7-20-05
% Copyright 1992-2008 The MathWorks, Inc.
% $Revision: 1.1.6.11 $ $Date: 2008/10/31 06:23:30 $

% FUNCTION INFO
% =============
%
******************************************************************************/

int traincgf( CAnnNetwork *annNetwork,
			 CxTrainRecord *trainRecord,
			 CxMatrixList *trainV,
			 CxMatrixList *valV,
			 CxMatrixList *testV )
{
	TCHAR szBuffer[512];
	TCHAR szText[512];
	int nRetcode;
	_tcscpy_s(szText, _countof(szText), _T(" "));
	nRetcode = trimString(szText, szBuffer, _countof(szBuffer));
	TRACE(_T("traincgf(\"%s\") = %s. (nRetcode = %d)\n"), szText, szBuffer, nRetcode);

	return ERR_NN_NONE;
}

_MATLAB_END
