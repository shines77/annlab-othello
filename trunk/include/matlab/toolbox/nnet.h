
#ifndef _NNET_H_
#define _NNET_H_

/* NAMESPACE DEFINE */

#if defined(__cplusplus)
  #define _MATLAB_BEGIN   namespace matlab {
  #define _MATLAB_END     }
  #define _MATLAB         ::matlab::

  #ifdef _MATLAB_USING
    #define _C_MATLAB_BEGIN namespace matlab {	/* only if *.c compiled as C++ */
    #define _C_MATLAB_END   }
    #define _CMATLAB        ::matlab::

  #else /* _MATLAB_USING */
    /* #define _GLOBAL_USING	*.h in global namespace, c* imports to std */

    #define _C_MATLAB_BEGIN
    #define _C_MATLAB_END
    #define _CMATLAB        ::

  #endif /* _MATLAB_USING */

  #define _C_LIB_DECL     extern "C" {	/* C has extern "C" linkage */
  #define _END_C_LIB_DECL }
  #define _EXTERN_C       extern "C" {
  #define _END_EXTERN_C   }

  #define _USING_NAMESPACE_MATLAB   using namespace matlab

#else /* __cplusplus */

  #define _MATLAB_BEGIN
  #define _MATLAB_END
  #define _MATLAB

  #define _C_MATLAB_BEGIN
  #define _C_MATLAB_END
  #define _CMATLAB

  #define _C_LIB_DECL
  #define _END_C_LIB_DECL
  #define _EXTERN_C
  #define _END_EXTERN_C

  #define _USING_NAMESPACE_MATLAB

#endif /* __cplusplus */

_MATLAB_BEGIN	// namespace matlab {

_MATLAB_END		// namespace matlab

#endif  /* _NNET_H_ */
