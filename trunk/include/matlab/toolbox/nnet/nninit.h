
#ifndef _NNINIT_H_
#define _NNINIT_H_

#include "../matlab.h"
#include "../../../annlab/AnnNetwork.h"
#include "../../../annlab/CxMatrix.h"

namespace matlab {

int init  ( CAnnNetwork *net );

int initnw( CAnnNetwork *net,
			CxMatrix *matrix, int _index, int _numInputs, int _numNeurons,
			CxMatrix *inputRange, CxMatrix *activeRange );

int initwb( CAnnNetwork *net,
			CxMatrix *matrix, int _index, int _numInputs, int _numNeurons,
			CxMatrix *inputRange, CxMatrix *activeRange );

}  // namespace matlab

#endif  /* _NNINIT_H_ */
