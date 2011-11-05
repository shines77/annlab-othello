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

#ifndef _ANNLAB_ASSERT_H_
#define _ANNLAB_ASSERT_H_

#include "annlab_config.h"
#if _MSC_VER
#include <crtdbg.h>
#endif

#ifndef __ANNLAB_EXPORTED_FUNC
  #if _MSC_VER >= 1400
    #define __ANNLAB_EXPORTED_FUNC   __cdecl
  #else
    #define __ANNLAB_EXPORTED_FUNC
  #endif
#endif

#if _MSC_VER
/* Asserts */
/* We use !! below to ensure that any overloaded operators used to evaluate expr do not end up at operator || */
#define __ANNLAB_CRT_ASSERT_EXPR(expr, msg) \
        (void) ((!!(expr)) || \
                (1 != _CrtDbgReportW(_CRT_ASSERT, _CRT_WIDE(__FILE__), __LINE__, NULL, msg)) || \
                (_CrtDbgBreak(), 0))

#ifndef __ANNLAB_CRT_ASSERT
#define __ANNLAB_CRT_ASSERT(expr, ...)      __ANNLAB_CRT_ASSERT_EXPR((expr), _CRT_WIDE(#expr))
#endif

#ifndef __ANNLAB_CRT_ASSERT_EX
#define __ANNLAB_CRT_ASSERT_EX(expr, msg) \
    __ANNLAB_CRT_ASSERT_EXPR((expr), (((msg) == NULL) ? _CRT_WIDE(#expr) \
    : _CRT_WIDE(#expr)_CRT_WIDE("\n\nDescription: ")_CRT_WIDE(#msg)))
#endif

#else
#ifndef __ANNLAB_CRT_ASSERT
#define __ANNLAB_CRT_ASSERT(expr)
#endif

#ifndef __ANNLAB_CRT_ASSERTE
#define __ANNLAB_CRT__ASSERTE(expr)
#endif
#endif /* !_MSC_VER */

namespace annlab {

    //! Type for an assertion handler
    typedef void(*assertion_handler_type)( const char* filename, int line, const char* expression, const char* comment );

#if ANNLAB_USE_ASSERT

    //! Assert that x is true.
    /** If x is false, print assertion failure message.  
        If the comment argument is not NULL, it is printed as part of the failure message.  
        The comment argument has no other effect. */
    #define __ANNLAB_ASSERT(predicate, ...)         ((predicate) ? ((void)0) : annlab::assertion_failure(__FILE__,__LINE__,#predicate,NULL))
    #define __ANNLAB_ASSERT_EX(predicate, message)  ((predicate) ? ((void)0) : annlab::assertion_failure(__FILE__,__LINE__,#predicate,message))

    //! Set assertion handler and return previous value of it.
    assertion_handler_type __ANNLAB_EXPORTED_FUNC set_assertion_handler( assertion_handler_type new_handler );

    //! Process an assertion failure.
    /** Normally called from __ANNLAB_ASSERT macro.
        If assertion handler is null, print message for assertion failure and abort.
        Otherwise call the assertion handler. */
    void __ANNLAB_EXPORTED_FUNC assertion_failure( const char* filename, int line, const char* expression, const char* comment );

#else /* !ANNLAB_USE_ASSERT */

    //! No-op version of __ANNLAB_ASSERT.
    #define __ANNLAB_ASSERT(predicate, comment)     ((void)0)
    //! "Extended" version is useful to suppress warnings if a variable is only used with an assert
    #define __ANNLAB_ASSERT_EX(predicate, comment)  ((void)(1 && (predicate)))

#endif /* !ANNLAB_USE_ASSERT */

} /* namespace annlab */

#endif  // _ANNLAB_ASSERT_H_
