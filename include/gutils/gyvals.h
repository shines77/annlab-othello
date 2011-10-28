/* gyvals.h values header for Microsoft C/C++ */
#pragma once
#ifndef _GZ_YVALS_H
#define _GZ_YVALS_H

#include <yvals.h>

/* NAMESPACE */

#if defined(__cplusplus)
  #define _GSL_BEGIN		namespace gsl {
  #define _GSL_END			}
  #define _GSL				::gsl::

  /*
    We use the stdext (standard extension) namespace to contain extensions that are not part of the current standard
  */
  #define _GSLEXT_BEGIN	    namespace gslext {
  #define _GSLEXT_END		}
  #define _GSLEXT	        ::gslext::

  #ifdef _GSL_USING
    #define _C_GSL_BEGIN	namespace gsl {	/* only if *.c compiled as C++ */
    #define _C_GSL_END		}
    #define _CSTD			::gsl::

  #else /* _GSL_USING */
    /* #define _GLOBAL_USING	*.h in global namespace, c* imports to std */
  
    #define _C_GSL_BEGIN
    #define _C_GSL_END
    #define _CSTD			::
  #endif /* _GSL_USING */

  #define _C_LIB_DECL		extern "C" {	/* C has extern "C" linkage */
  #define _END_C_LIB_DECL	}
  #define _EXTERN_C			extern "C" {
  #define _END_EXTERN_C		}

#else /* __cplusplus */
  #define _GSL_BEGIN
  #define _GSL_END
  #define _GSL

  #define _C_GSL_BEGIN
  #define _C_GSL_END
  #define _CSTD

  #define _C_LIB_DECL
  #define _END_C_LIB_DECL
  #define _EXTERN_C
  #define _END_EXTERN_C
#endif /* __cplusplus */

#define _Restrict	restrict

#ifdef __cplusplus
  _GSL_BEGIN
  typedef bool _Bool;
  _GSL_END
#endif /* __cplusplus */

#endif  // _GZ_YVALS_H
