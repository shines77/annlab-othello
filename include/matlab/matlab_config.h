/*
    Copyright 2005-2011 Intel Corporation.  All Rights Reserved.

    This file is part of Threading Building Blocks.

    Threading Building Blocks is free software; you can redistribute it
    and/or modify it under the terms of the GNU General Public License
    version 2 as published by the Free Software Foundation.

    Threading Building Blocks is distributed in the hope that it will be
    useful, but WITHOUT ANY WARRANTY; without even the implied warranty
    of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Threading Building Blocks; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    As a special exception, you may use this file as part of a free software
    library without restriction.  Specifically, if other files instantiate
    templates or use macros or inline functions from this file, or you compile
    this file and link it with other files to produce an executable, this
    file does not by itself cause the resulting executable to be covered by
    the GNU General Public License.  This exception does not however
    invalidate any other reasons why the executable file might be covered by
    the GNU General Public License.
*/

#ifndef _MATLAB_CONFIG_H_
#define _MATLAB_CONFIG_H_

/** This header is supposed to contain macro definitions and C style comments only.
    The macros defined here are intended to control such aspects of TBB build as 
    - presence of compiler features
    - compilation modes
    - feature sets
    - known compiler/platform issues
**/

#define __MATLAB_BUILD     1

#ifdef _DEBUG
#define MATLAB_USE_ASSERT       1
#define MATLAB_USE_TRACE        1
#define MATLAB_USE_DISPLAY      1
#define MATLAB_USE_DISPLAY_EX   1
#endif

#define __MATLAB_GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)

/** Presence of compiler features **/

#if (__MATLAB_GCC_VERSION >= 40400) && !defined(__INTEL_COMPILER)
    /** warning suppression pragmas available in GCC since 4.4 **/
    #define __MATLAB_GCC_WARNING_SUPPRESSION_PRESENT 1
#endif

/* TODO: The following condition should be extended when new compilers/runtimes 
         with std::exception_ptr support appear. */
#define __MATLAB_EXCEPTION_PTR_PRESENT  ((_MSC_VER >= 1600 || (__GXX_EXPERIMENTAL_CXX0X__ && __GNUC__==4 && __GNUC_MINOR__>=4)) && !__INTEL_COMPILER)

#if __GNUC__ || __SUNPRO_CC || __IBMCPP__
    /* ICC defines __GNUC__ and so is covered */
    #define __MATLAB_ATTRIBUTE_ALIGNED_PRESENT 1
#elif _MSC_VER && (_MSC_VER >= 1300 || __INTEL_COMPILER)
    #define __MATLAB_DECLSPEC_ALIGN_PRESENT 1
#endif

#if (__MATLAB_GCC_VERSION >= 40102) && !defined(__INTEL_COMPILER)
    /** built-in atomics available in GCC since 4.1.2 **/
    #define __MATLAB_GCC_BUILTIN_ATOMICS_PRESENT 1
#endif

/** User controlled TBB features & modes **/

#ifndef MATLAB_USE_DEBUG
#ifdef MATLAB_DO_ASSERT
#define MATLAB_USE_DEBUG MATLAB_DO_ASSERT
#else
#ifdef _DEBUG
#define MATLAB_USE_DEBUG _DEBUG
#else
#define MATLAB_USE_DEBUG 0
#endif
#endif /* MATLAB_DO_ASSERT */
#endif /* MATLAB_USE_DEBUG */

#ifndef MATLAB_USE_ASSERT
#ifdef MATLAB_DO_ASSERT
#define MATLAB_USE_ASSERT MATLAB_DO_ASSERT
#else 
#define MATLAB_USE_ASSERT MATLAB_USE_DEBUG
#endif /* MATLAB_DO_ASSERT */
#endif /* MATLAB_USE_ASSERT */

#ifndef MATLAB_USE_THREADING_TOOLS
#ifdef MATLAB_DO_THREADING_TOOLS
#define MATLAB_USE_THREADING_TOOLS MATLAB_DO_THREADING_TOOLS
#else 
#define MATLAB_USE_THREADING_TOOLS MATLAB_USE_DEBUG
#endif /* MATLAB_DO_THREADING_TOOLS */
#endif /* MATLAB_USE_THREADING_TOOLS */

#ifndef MATLAB_USE_PERFORMANCE_WARNINGS
#ifdef MATLAB_PERFORMANCE_WARNINGS
#define MATLAB_USE_PERFORMANCE_WARNINGS MATLAB_PERFORMANCE_WARNINGS
#else 
#define MATLAB_USE_PERFORMANCE_WARNINGS MATLAB_USE_DEBUG
#endif /* MATLAB_PEFORMANCE_WARNINGS */
#endif /* MATLAB_USE_PERFORMANCE_WARNINGS */

#if !defined(__EXCEPTIONS) && !defined(_CPPUNWIND) && !defined(__SUNPRO_CC) || defined(_XBOX)
    #if MATLAB_USE_EXCEPTIONS
        #error Compilation settings do not support exception handling. Please do not set MATLAB_USE_EXCEPTIONS macro or set it to 0.
    #elif !defined(MATLAB_USE_EXCEPTIONS)
        #define MATLAB_USE_EXCEPTIONS 0
    #endif
#elif !defined(MATLAB_USE_EXCEPTIONS)
    #define MATLAB_USE_EXCEPTIONS 1
#endif

#ifndef MATLAB_IMPLEMENT_CPP0X
    /** By default, use C++0x classes if available **/
    #if __GNUC__==4 && __GNUC_MINOR__>=4 && __GXX_EXPERIMENTAL_CXX0X__
        #define MATLAB_IMPLEMENT_CPP0X 0
    #else
        #define MATLAB_IMPLEMENT_CPP0X 1
    #endif
#endif /* MATLAB_IMPLEMENT_CPP0X */

#ifndef MATLAB_USE_CAPTURED_EXCEPTION
    #if __MATLAB_EXCEPTION_PTR_PRESENT
        #define MATLAB_USE_CAPTURED_EXCEPTION 0
    #else
        #define MATLAB_USE_CAPTURED_EXCEPTION 1
    #endif
#else /* defined MATLAB_USE_CAPTURED_EXCEPTION */
    #if !MATLAB_USE_CAPTURED_EXCEPTION && !__MATLAB_EXCEPTION_PTR_PRESENT
        #error Current runtime does not support std::exception_ptr. Set MATLAB_USE_CAPTURED_EXCEPTION and make sure that your code is ready to catch matlab::captured_exception.
    #endif
#endif /* defined MATLAB_USE_CAPTURED_EXCEPTION */

/** Check whether the request to use GCC atomics can be satisfied **/
#if (MATLAB_USE_GCC_BUILTINS && !__MATLAB_GCC_BUILTIN_ATOMICS_PRESENT)
    #error "GCC atomic built-ins are not supported."
#endif

/** Internal TBB features & modes **/

#ifndef __MATLAB_DYNAMIC_LOAD_ENABLED
    #define __MATLAB_DYNAMIC_LOAD_ENABLED !__MATLAB_TASK_CPP_DIRECTLY_INCLUDED
#elif !__MATLAB_DYNAMIC_LOAD_ENABLED
    #if _WIN32||_WIN64
        #define __MATLAB_NO_IMPLICIT_LINKAGE 1
        #define __TBBMALLOC_NO_IMPLICIT_LINKAGE 1
    #else
        #define __MATLAB_WEAK_SYMBOLS 1
    #endif
#endif

#ifndef __MATLAB_COUNT_TASK_NODES
    #define __MATLAB_COUNT_TASK_NODES MATLAB_USE_ASSERT
#endif

#ifndef __MATLAB_TASK_GROUP_CONTEXT
    #define __MATLAB_TASK_GROUP_CONTEXT 1
#endif /* __MATLAB_TASK_GROUP_CONTEXT */

#if MATLAB_USE_EXCEPTIONS && !__MATLAB_TASK_GROUP_CONTEXT
    #error MATLAB_USE_EXCEPTIONS requires __MATLAB_TASK_GROUP_CONTEXT to be enabled
#endif

#ifndef __MATLAB_SCHEDULER_OBSERVER
    #define __MATLAB_SCHEDULER_OBSERVER 1
#endif /* __MATLAB_SCHEDULER_OBSERVER */

#ifndef __MATLAB_TASK_PRIORITY
    #define __MATLAB_TASK_PRIORITY __MATLAB_TASK_GROUP_CONTEXT
#endif /* __MATLAB_TASK_PRIORITY */

#if __MATLAB_TASK_PRIORITY && !__MATLAB_TASK_GROUP_CONTEXT
    #error __MATLAB_TASK_PRIORITY requires __MATLAB_TASK_GROUP_CONTEXT to be enabled
#endif

#if !defined(__MATLAB_SURVIVE_THREAD_SWITCH) && (_WIN32 || _WIN64 || __linux__)
    #define __MATLAB_SURVIVE_THREAD_SWITCH 1
#endif /* __MATLAB_SURVIVE_THREAD_SWITCH */

#ifndef __MATLAB_DEFAULT_PARTITIONER
#if MATLAB_DEPRECATED
/** Default partitioner for parallel loop templates in TBB 1.0-2.1 */
#define __MATLAB_DEFAULT_PARTITIONER matlab::simple_partitioner
#else
/** Default partitioner for parallel loop templates since TBB 2.2 */
#define __MATLAB_DEFAULT_PARTITIONER matlab::auto_partitioner
#endif /* MATLAB_DEPRECATED */
#endif /* !defined(__MATLAB_DEFAULT_PARTITIONER */

/** Macros of the form __MATLAB_XXX_BROKEN denote known issues that are caused by
    the bugs in compilers, standard or OS specific libraries. They should be 
    removed as soon as the corresponding bugs are fixed or the buggy OS/compiler
    versions go out of the support list. 
**/

#if __GNUC__ && __MATLAB_x86_64 && __INTEL_COMPILER == 1200
    #define __MATLAB_ICC_12_0_INL_ASM_FSTCW_BROKEN 1
#endif

#if _MSC_VER && __INTEL_COMPILER && (__INTEL_COMPILER<1110 || __INTEL_COMPILER==1110 && __INTEL_COMPILER_BUILD_DATE < 20091012)
    /** Necessary to avoid ICL error (or warning in non-strict mode): 
        "exception specification for implicitly declared virtual destructor is 
        incompatible with that of overridden one". **/
    #define __MATLAB_DEFAULT_DTOR_THROW_SPEC_BROKEN 1
#endif

#if defined(_MSC_VER) && _MSC_VER < 1500 && !defined(__INTEL_COMPILER)
    /** VS2005 and earlier do not allow declaring template class as a friend 
        of classes defined in other namespaces. **/
    #define __MATLAB_TEMPLATE_FRIENDS_BROKEN 1
#endif

#if __GLIBC__==2 && __GLIBC_MINOR__==3 || __MINGW32__ || (__APPLE__ && __INTEL_COMPILER==1200 && !MATLAB_USE_DEBUG)
    //! Macro controlling EH usages in TBB tests
    /** Some older versions of glibc crash when exception handling happens concurrently. **/
    #define __MATLAB_THROW_ACROSS_MODULE_BOUNDARY_BROKEN 1
#endif

#if (_WIN32||_WIN64) && __INTEL_COMPILER == 1110
    /** That's a bug in Intel compiler 11.1.044/IA-32/Windows, that leads to a worker thread crash on the thread's startup. **/
    #define __MATLAB_ICL_11_1_CODE_GEN_BROKEN 1
#endif

#if __GNUC__==3 && __GNUC_MINOR__==3 && !defined(__INTEL_COMPILER)
    /** A bug in GCC 3.3 with access to nested classes declared in protected area */
    #define __MATLAB_GCC_3_3_PROTECTED_BROKEN 1
#endif

#if __MINGW32__ && (__GNUC__<4 || __GNUC__==4 && __GNUC_MINOR__<2)
    /** MinGW has a bug with stack alignment for routines invoked from MS RTLs.
        Since GCC 4.2, the bug can be worked around via a special attribute. **/
    #define __MATLAB_SSE_STACK_ALIGNMENT_BROKEN 1
#endif

#if __GNUC__==4 && __GNUC_MINOR__==3 && __GNUC_PATCHLEVEL__==0
    // GCC of this version may rashly ignore control dependencies
    #define __MATLAB_GCC_OPTIMIZER_ORDERING_BROKEN 1
#endif

#if __FreeBSD__
    /** A bug in FreeBSD 8.0 results in kernel panic when there is contention 
        on a mutex created with this attribute. **/
    #define __MATLAB_PRIO_INHERIT_BROKEN 1

    /** A bug in FreeBSD 8.0 results in test hanging when an exception occurs 
        during (concurrent?) object construction by means of placement new operator. **/
    #define __MATLAB_PLACEMENT_NEW_EXCEPTION_SAFETY_BROKEN 1
#endif /* __FreeBSD__ */

#if (__linux__ || __APPLE__) && __i386__ && defined(__INTEL_COMPILER)
    /** The Intel compiler for IA-32 (Linux|Mac OS X) crashes or generates 
        incorrect code when __asm__ arguments have a cast to volatile. **/
    #define __MATLAB_ICC_ASM_VOLATILE_BROKEN 1
#endif

#if !__INTEL_COMPILER && (_MSC_VER || __GNUC__==3 && __GNUC_MINOR__<=2)
    /** Bug in GCC 3.2 and MSVC compilers that sometimes return 0 for __alignof(T) 
        when T has not yet been instantiated. **/
    #define __MATLAB_ALIGNOF_NOT_INSTANTIATED_TYPES_BROKEN 1
#endif

#endif /* _MATLAB_CONFIG_H_ */
