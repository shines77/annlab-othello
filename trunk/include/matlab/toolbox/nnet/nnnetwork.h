
#ifndef _NNNETWORK_H_
#define _NNNETWORK_H_

#include "../matlab.h"
#include "../../../annlab/AnnNetwork.h"
#include "../../../annlab/CxMatrix.h"

namespace matlab {

int parseNetLayers( CAnnNetwork *net,
				   const TCHAR *szSizesOfLayers,
				   CxNetLayers *pNetLayers,
				   int _inNumLayers = 0,
				   const TCHAR *szDelim = NULL );

int parseTransFcns( CAnnNetwork *net,
				   const TCHAR *szTransFcnOfLayers,
				   CxNetLayers *pNetLayers,
				   const TCHAR *szDelim = NULL );

int newff( CAnnNetwork *net,
		  const CxMatrix *_inputMinMax,
		  const TCHAR *szSizesOfLayers,
		  const TCHAR *szTransFcnOfLayers,
		  const TCHAR *szTrainFcn,
		  bool bInitWeightsNow = FALSE );

}  // namespace matlab

#endif  /* _NNNETWORK_H_ */
