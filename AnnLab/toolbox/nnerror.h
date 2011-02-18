#pragma once

#ifndef _NNERROR_H_
#define _NNERROR_H_

#include "nnet.h"

#define ERR_NN_NONE            0x00000000UL

#define ERR_NN_INVALID_PARAM   0x80000001UL

_MATLAB_BEGIN

unsigned long nnGetErrorInfo( unsigned long errCode );

_MATLAB_END

#endif  /* _NNERROR_H_ */
