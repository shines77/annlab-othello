
#ifndef _NNTRAIN_H_
#define _NNTRAIN_H_

#include "../../annlab.h"
#include "../../../annlab/sys/AnnNetwork.h"
#include "../../../annlab/sys/AnnMatrix.h"

namespace annlab {

int train( CAnnNetwork *net,
		  CAnnTrainRecord *tr,
		  CAnnMatrixList *trainP,
		  CAnnMatrixList *trainT,
		  CAnnTrainParam *tainParam );

int traincgf( CAnnNetwork *net,
		  CAnnTrainRecord *tr,
		  CAnnMatrixList *trainV,
		  CAnnMatrixList *valV,
		  CAnnMatrixList *testV );

}  // namespace annlab

#endif  /* _NNTRAIN_H_ */
