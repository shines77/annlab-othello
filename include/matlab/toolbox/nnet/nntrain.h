
#ifndef _NNTRAIN_H_
#define _NNTRAIN_H_

#include "../matlab.h"
#include "../../../annlab/AnnNetwork.h"
#include "../../../annlab/AnnMatrix.h"

namespace matlab {

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

}  // namespace matlab

#endif  /* _NNTRAIN_H_ */
