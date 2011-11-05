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

// IMPORTANT: To use assertion handling in MATLAB, exactly one of the MATLAB source files
// should #include matlab_assert_impl.h thus instantiating assertion handling routines.
// The intent of putting it to a separate file is to allow some tests to use it
// as well in order to avoid dependency on the library.

// include headers for required function declarations
#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "../../include/matlab/matlab_assert.h"
#include "../../include/matlab/matlab_config.h"
#if _MSC_VER
#include <crtdbg.h>
#define __MATLAB_USE_DBGBREAK_DLG   MATLAB_USE_DEBUG
#endif

using namespace std;

namespace matlab {
    //! Type for an assertion handler
    typedef void(*assertion_handler_type)( const char* filename, int line, const char* expression, const char* comment );

    static assertion_handler_type assertion_handler = NULL;

    assertion_handler_type __MATLAB_EXPORTED_FUNC set_assertion_handler( assertion_handler_type new_handler ) {
        assertion_handler_type old_handler = assertion_handler;
        assertion_handler = new_handler;
        return old_handler;
    }

    void __MATLAB_EXPORTED_FUNC assertion_failure( const char* filename, int line, const char* expression, const char* comment ) {
        if ( assertion_handler_type assertion_func = assertion_handler ) {
            (*assertion_func)(filename, line, expression, comment);
        } else {
            static bool already_failed = false;
            if( !already_failed ) {
                already_failed = true;
                fprintf( stderr, "Assertion [%s] failed on line %d of file %s\n",
                         expression, line, filename );
                if ( comment )
                    fprintf( stderr, "Detailed description: %s\n", comment );
#if __MATLAB_USE_DBGBREAK_DLG
                if (1 == _CrtDbgReport(_CRT_ASSERT, filename, line, "matlab_debug.dll", "%s\r\n%s", expression, comment ? comment : ""))
                        _CrtDbgBreak();
#else
                fflush(stderr);
                abort();
#endif
            }
        }
    }

#if defined(_MSC_VER) && _MSC_VER<1400
    #define vsnprintf _vsnprintf
    #define vsnprintf_s _vsnprintf_s
#endif

    namespace internal {
        //! Report a runtime warning.
        void __MATLAB_EXPORTED_FUNC runtime_warning( const char* format, ... )
        {
            char str[1024]; memset(str, 0, 1024);
            va_list args; va_start(args, format);
#if (defined(_WIN32) || defined(_WIN64)) && ((defined(_MSC_VER) && _MSC_VER>=1400) \
    || (defined(__INTEL_COMPILER) && __INTEL_COMPILER>=1000))
            vsnprintf_s( str, 1024, 1024-1, format, args );
#else
            vsnprintf( str, 1024-1, format, args );
#endif
            fprintf( stderr, "MATLAB Warning: %s\n", str );
        }
    } // namespace internal

} /* namespace matlab */
