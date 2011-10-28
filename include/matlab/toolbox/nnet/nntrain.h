
#ifndef _NNTRAIN_H_
#define _NNTRAIN_H_

#include "../matlab.h"
#include "../../../annlab/AnnNetwork.h"
#include "../../../annlab/CxMatrix.h"

namespace matlab {

int train( CAnnNetwork *net,
		  CxTrainRecord *tr,
		  CxMatrixList *trainP,
		  CxMatrixList *trainT,
		  CxTrainParam *tainParam );

int traincgf( CAnnNetwork *net,
		  CxTrainRecord *tr,
		  CxMatrixList *trainV,
		  CxMatrixList *valV,
		  CxMatrixList *testV );

}  // namespace matlab

#endif  /* _NNTRAIN_H_ */
