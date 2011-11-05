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

#if !defined(_ANNLAB_annlab_windef_H_)
#error Do not #include this internal file directly; use public ANNLAB headers instead.
#endif /* _ANNLAB_annlab_windef_H_ */

#ifndef _ANNLAB_INTERNAL_ANNLAB_WINDEF_H_
#define _ANNLAB_INTERNAL_ANNLAB_WINDEF_H_

// Check that the target Windows version has all API calls requried for ANNLAB.
// Do not increase the version in condition beyond 0x0500 without prior discussion!
#if defined(_WIN32_WINNT) && _WIN32_WINNT<0x0400
#error ANNLAB is unable to run on old Windows versions; _WIN32_WINNT must be 0x0400 or greater.
#endif

#if !defined(_MT)
#error ANNLAB requires linkage with multithreaded C/C++ runtime library. \
       Choose multithreaded DLL runtime in project settings, or use /MD[d] compiler switch.
#endif

// Workaround for the problem with MVSC headers failing to define namespace std
namespace std {
	using ::size_t; using ::ptrdiff_t;
}

#define __ANNLAB_STRING_AUX(x)		#x
#define __ANNLAB_STRING(x)			__ANNLAB_STRING_AUX(x)

// Default setting of ANNLAB_USE_DEBUG
#ifdef ANNLAB_USE_DEBUG
#    if ANNLAB_USE_DEBUG 
#        if !defined(_DEBUG)
#            pragma message(__FILE__ "(" __ANNLAB_STRING(__LINE__) ") : Warning: Recommend using /MDd if compiling with ANNLAB_USE_DEBUG!=0")
#        endif
#    else
#        if defined(_DEBUG)
#            pragma message(__FILE__ "(" __ANNLAB_STRING(__LINE__) ") : Warning: Recommend using /MD if compiling with ANNLAB_USE_DEBUG==0")
#        endif
#    endif
#endif

#if __ANNLAB_BUILD && !defined(__ANNLAB_NO_IMPLICIT_LINKAGE)
#define __ANNLAB_NO_IMPLICIT_LINKAGE   1
#endif

#if _MSC_VER
    #if !__ANNLAB_NO_IMPLICIT_LINKAGE
        #ifdef __ANNLAB_LIB_NAME
	        #pragma comment(lib, __ANNLAB_STRING(__ANNLAB_LIB_NAME))
        #else
			#ifdef _DEBUG
				#pragma comment(lib, "annlab_debug.lib")
			#else
				#pragma comment(lib, "annlab.lib")
			#endif
        #endif
    #endif
#endif

#endif  // _ANNLAB_INTERNAL_ANNLAB_WINDEF_H_
