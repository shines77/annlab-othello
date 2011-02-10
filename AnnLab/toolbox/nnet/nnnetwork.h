#pragma once

#ifndef _NNNETWORK_H_
#define _NNNETWORK_H_

#include "..\nnet.h"
#include "..\..\AnnNetwork.h"
#include "..\..\CxMatrix.h"

_MATLAB_BEGIN

int parseNetLayers( CAnnNetwork *pAnnNetwork,
				   const TCHAR *szSizesOfLayers,
				   CxNetLayers *pNetLayers,
				   int _inNumLayers = 0,
				   const TCHAR *szDelim = NULL );

int parseTransFcns( CAnnNetwork *pAnnNetwork,
				   const TCHAR *szTransFcnOfLayers,
				   CxNetLayers *pNetLayers,
				   const TCHAR *szDelim = NULL );

int newff( CAnnNetwork *pAnnNetwork,
		  const CxMatrix *_inputMinMax,
		  const TCHAR *szSizesOfLayers,
		  const TCHAR *szTransFcnOfLayers,
		  const TCHAR *szTrainFcn,
		  BOOL bInitWeightsNow = FALSE );

_MATLAB_END

#endif  /* _NNNETWORK_H_ */
