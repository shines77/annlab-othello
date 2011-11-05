
#ifndef _NNINIT_H_
#define _NNINIT_H_

#include "../../annlab.h"
#include "../../../annlab/sys/AnnNetwork.h"
#include "../../../annlab/sys/AnnMatrix.h"

namespace annlab {

int init  ( CAnnNetwork *net );

int initnw( CAnnNetwork *net,
			CAnnMatrix *matrix, int _index, int _numInputs, int _numNeurons,
			CAnnMatrix *inputRange, CAnnMatrix *activeRange );

int initwb( CAnnNetwork *net,
			CAnnMatrix *matrix, int _index, int _numInputs, int _numNeurons,
			CAnnMatrix *inputRange, CAnnMatrix *activeRange );

}  // namespace annlab

#endif  /* _NNINIT_H_ */
