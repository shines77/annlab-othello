
#ifndef _NNNETWORK_H_
#define _NNNETWORK_H_

#include "../matlab.h"
#include "../../../matlab/sys/AnnNetwork.h"
#include "../../../matlab/sys/AnnMatrix.h"

namespace matlab {

int parseNetLayers( CAnnNetwork *net,
				   const TCHAR *szSizesOfLayers,
				   CAnnNetLayers *pNetLayers,
				   int _inNumLayers = 0,
				   const TCHAR *szDelim = NULL );

int parseTransFcns( CAnnNetwork *net,
				   const TCHAR *szTransFcnOfLayers,
				   CAnnNetLayers *pNetLayers,
				   const TCHAR *szDelim = NULL );

int newff( CAnnNetwork *net,
		  const CAnnMatrix *_inputMinMax,
		  const TCHAR *szSizesOfLayers,
		  const TCHAR *szTransFcnOfLayers,
		  const TCHAR *szTrainFcn,
		  bool bInitWeightsNow = FALSE );

}  // namespace matlab

#endif  /* _NNNETWORK_H_ */
