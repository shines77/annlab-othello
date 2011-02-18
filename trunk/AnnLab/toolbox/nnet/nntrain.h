#pragma once

#ifndef _NNTRAIN_H_
#define _NNTRAIN_H_

#include "..\nnet.h"
#include "..\..\AnnNetwork.h"
#include "..\..\CxMatrix.h"

_MATLAB_BEGIN

int train( CAnnNetwork *annNetwork,
		  CxTrainRecord *trainRecord,
		  CxMatrixList *trainP,
		  CxMatrixList *trainT,
		  CxTrainParam *tainParam );

int traincgf( CAnnNetwork *annNetwork,
		  CxTrainRecord *trainRecord,
		  CxMatrixList *trainV,
		  CxMatrixList *valV,
		  CxMatrixList *testV );

_MATLAB_END

#endif  /* _NNTRAIN_H_ */
