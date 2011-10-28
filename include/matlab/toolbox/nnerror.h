
#ifndef _NNERROR_H_
#define _NNERROR_H_

#include "matlab.h"

#define ERR_NN_NONE            0x00000000UL
#define ERR_NN_SUCCEED         0x00000001UL

#define ERR_NN_INVALID_PARAM   0x80000001UL

namespace matlab {

static unsigned long GetErrorInfo( unsigned long errCode );

}  // namespace matlab

#endif  /* _NNERROR_H_ */
