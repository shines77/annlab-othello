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

#ifndef __ANNLAB_EXPORTED_FUNC
  #if _MSC_VER >= 1400
    #define __ANNLAB_EXPORTED_FUNC   __cdecl
  #else
    #define __ANNLAB_EXPORTED_FUNC
  #endif
#endif

namespace annlab {

    //! Type for an assertion handler
    typedef void(*assertion_handler_type)( const char* filename, int line, const char* expression, const char* comment );

#if ANNLAB_USE_ASSERT

    //! Assert that x is true.
    /** If x is false, print assertion failure message.  
        If the comment argument is not NULL, it is printed as part of the failure message.  
        The comment argument has no other effect. */
    #define __ANNLAB_ASSERT(predicate, message)     ((predicate) ? ((void)0) : annlab::assertion_failure(__FILE__,__LINE__,#predicate,message))
    #define __ANNLAB_ASSERT_EX                      __ANNLAB_ASSERT

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
