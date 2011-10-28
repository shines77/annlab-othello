
#ifndef _NNINIT_H_
#define _NNINIT_H_

#include "../matlab.h"
#include "../../../annlab/AnnNetwork.h"
#include "../../../annlab/AnnMatrix.h"

using namespace annlab;

namespace matlab {

int init  ( CAnnNetwork *net );

int initnw( CAnnNetwork *net,
			CAnnMatrix *matrix, int _index, int _numInputs, int _numNeurons,
			CAnnMatrix *inputRange, CAnnMatrix *activeRange );

int initwb( CAnnNetwork *net,
			CAnnMatrix *matrix, int _index, int _numInputs, int _numNeurons,
			CAnnMatrix *inputRange, CAnnMatrix *activeRange );

}  // namespace matlab

#endif  /* _NNINIT_H_ */
