
#ifndef _NNERROR_H_
#define _NNERROR_H_

#include "../annlab_stddef.h"

#define ERR_NN_NONE            0x00000000UL
#define ERR_NN_SUCCEED         0x00000001UL

#define ERR_NN_INVALID_PARAM   0x80000001UL

namespace annlab {

static unsigned long GetErrorInfo( unsigned long errCode );

}  // namespace annlab

#endif  /* _NNERROR_H_ */
