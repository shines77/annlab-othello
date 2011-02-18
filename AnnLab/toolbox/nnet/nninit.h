#pragma once

#ifndef _NNINIT_H_
#define _NNINIT_H_

#include "..\nnet.h"
#include "..\..\AnnNetwork.h"
#include "..\..\CxMatrix.h"

_MATLAB_BEGIN

int init  ( CAnnNetwork *net );

int initnw( CAnnNetwork *net,
			CxMatrix *matrix, int _index, int _numInputs, int _numNeurons,
			CxMatrix *inputRange, CxMatrix *activeRange );

int initwb( CAnnNetwork *net,
			CxMatrix *matrix, int _index, int _numInputs, int _numNeurons,
			CxMatrix *inputRange, CxMatrix *activeRange );

_MATLAB_END

#endif  /* _NNINIT_H_ */
