
#ifndef _ANNLAB_STDDEF_H_
#define _ANNLAB_STDDEF_H_

#include <stdlib.h>
#include <tchar.h>
#include <stddef.h>
#include "./vs/stdint.h"

// Marketing-driven product version
#define ANNLAB_VERSION_MAJOR           1
#define ANNLAB_VERSION_MINOR           0

// Engineering-focused interface version
#define ANNLAB_INTERFACE_VERSION       6000
#define ANNLAB_INTERFACE_VERSION_MAJOR (ANNLAB_INTERFACE_VERSION/1000)

// The oldest major interface version still supported
// To be used in SONAME, manifests, etc.
#define ANNLAB_COMPATIBLE_INTERFACE_VERSION    2

#define __ANNLAB_STRING_AUX(x)     #x
#define __ANNLAB_STRING(x)         __ANNLAB_STRING_AUX(x)

#define STRING_MERGE(a,b,c)     a##b##c
#define TEST_STRING_DEF(Tx)     STRING_MERGE("first ", " o ", " last")

#define INLINE inline

#define FCN_NAME_LEN      64

#define MIN(a, b)         (((a) < (b)) ? (a) : (b))
#define MAX(a, b)         (((a) > (b)) ? (a) : (b))

#define __MY_ASSERT(expression, ...)
#define __MY_TRACE(expression, ...)

#define FLOAT_EPSINON     (0.00001)
#define DOUBLE_EPSINON    (0.00000001)

// We do not need defines below for resource processing on windows
#if !defined RC_INVOKED

// Define groups for Doxygen documentation
/**
 * @defgroup algorithms         Algorithms
 * @defgroup containers         Containers
 * @defgroup memory_allocation  Memory Allocation
 * @defgroup synchronization    Synchronization
 * @defgroup timing             Timing
 * @defgroup task_scheduling    Task Scheduling
 */

// Simple text that is displayed on the main page of Doxygen documentation.
/**
 * \mainpage Main Page
 *
 * Click the tabs above for information about the
 * - <a href="./modules.html">Modules</a> (groups of functionality) implemented by the library 
 * - <a href="./annotated.html">Classes</a> provided by the library
 * - <a href="./files.html">Files</a> constituting the library.
 * .
 * Please note that significant part of ANNLAB functionality is implemented in the form of
 * template functions, descriptions of which are not accessible on the <a href="./annotated.html">Classes</a>
 * tab. Use <a href="./modules.html">Modules</a> or <a href="./namespacemembers.html">Namespace/Namespace Members</a>
 * tabs to find them.
 *
 * Additional pieces of information can be found here
 * - \subpage concepts
 * .
 */

/** \page concepts ANN concepts
    
    A concept is a set of requirements to a type, which are necessary and sufficient
    for the type to model a particular behavior or a set of behaviors. Some concepts 
    are specific to a particular algorithm (e.g. algorithm body), while other ones 
    are common to several algorithms (e.g. range concept). 

    All ANNLAB algorithms make use of different classes implementing various concepts.
    Implementation classes are supplied by the user as type arguments of template 
    parameters and/or as objects passed as function call arguments. The library 
    provides predefined  implementations of some concepts (e.g. several kinds of 
    \ref range_req "ranges"), while other ones must always be implemented by the user. 
    
    ANNLAB defines a set of minimal requirements each concept must conform to. Here is 
    the list of different concepts hyperlinked to the corresponding requirements specifications:
    - \subpage range_req
    - \subpage parallel_do_body_req
    - \subpage parallel_for_body_req
    - \subpage parallel_reduce_body_req
    - \subpage parallel_scan_body_req
    - \subpage parallel_sort_iter_req
**/

// Define preprocessor symbols used to determine architecture
#if _WIN32||_WIN64
#   if defined(_M_X64)||defined(__x86_64__)  // the latter for MinGW support
#       define __ANNLAB_x86_64 1
#   elif defined(_M_IA64)
#       define __ANNLAB_ipf 1
#   elif defined(_M_IX86)||defined(__i386__) // the latter for MinGW support
#       define __ANNLAB_x86_32 1
#   endif
#else /* Assume generic Unix */
#   if !__linux__ && !__APPLE__
#       define __ANNLAB_generic_os 1
#   endif
#   if __x86_64__
#       define __ANNLAB_x86_64 1
#   elif __ia64__
#       define __ANNLAB_ipf 1
#   elif __i386__||__i386  // __i386 is for Sun OS
#       define __ANNLAB_x86_32 1
#   else
#       define __ANNLAB_generic_arch 1
#   endif
#endif

// ann_config.h should be included the first since it contains macro definitions used in other headers
#include "annlab_config.h"

#if defined(_MSC_VER) && (_MSC_VER >=1400)
    #define __ANNLAB_EXPORTED_FUNC   __cdecl
    #define __ANNLAB_EXPORTED_METHOD __thiscall
#else
    #define __ANNLAB_EXPORTED_FUNC
    #define __ANNLAB_EXPORTED_METHOD
#endif

#include <cstddef>      /* Need size_t and ptrdiff_t */

#if defined(_MSC_VER) && _MSC_VER
    #define _ANNLAB_annlab_windef_H_
    #include "internal/_annlab_windef.h"
    #undef  _ANNLAB_annlab_windef_H_
#else
    #include <stdint.h>
#endif

#include "annlab_assert.h"

//! The namespace annlab contains all components of the library.
namespace annlab {

#if defined(_MSC_VER) && _MSC_VER
    namespace internal {
        typedef __int8 int8_t;
        typedef __int16 int16_t;
        typedef __int32 int32_t;
        typedef __int64 int64_t;
        typedef unsigned __int8 uint8_t;
        typedef unsigned __int16 uint16_t;
        typedef unsigned __int32 uint32_t;
        typedef unsigned __int64 uint64_t;
    } // namespace internal
#else /* Posix */
    namespace internal {
        using ::int8_t;
        using ::int16_t;
        using ::int32_t;
        using ::int64_t;
        using ::uint8_t;
        using ::uint16_t;
        using ::uint32_t;
        using ::uint64_t;
    } // namespace internal
#endif /* Posix */

    using std::size_t;
    using std::ptrdiff_t;

//! The function returns the interface version of the ANNLAB shared library being used.
/**
 * The version it returns is determined at runtime, not at compile/link time.
 * So it can be different than the value of ANNLAB_INTERFACE_VERSION obtained at compile time.
 */
//extern "C" int __ANNLAB_EXPORTED_FUNC ANNLAB_runtime_interface_version();

/**
 * @cond INTERNAL
 * @brief Identifiers declared inside namespace internal should never be used directly by client code.
 */
namespace internal {

//! Compile-time constant that is upper bound on cache line/sector size.
/** It should be used only in situations where having a compile-time upper 
    bound is more useful than a run-time exact answer.
    @ingroup memory_allocation */
const size_t NFS_MaxLineSize = 128;

/** Label for data that may be accessed from different threads, and that may eventually become wrapped
    in a formal atomic type.
    
    Note that no problems have yet been observed relating to the definition currently being empty,
    even if at least "volatile" would seem to be in order to avoid data sometimes temporarily hiding
    in a register (although "volatile" as a "poor man's atomic" lacks several other features of a proper
    atomic, some of which are now provided instead through specialized functions).

    Note that usage is intentionally compatible with a definition as qualifier "volatile",
    both as a way to have the compiler help enforce use of the label and to quickly rule out
    one potential issue.

    Note however that, with some architecture/compiler combinations, e.g. on Itanium, "volatile" 
    also has non-portable memory semantics that are needlessly expensive for "relaxed" operations.

    Note that this must only be applied to data that will not change bit patterns when cast to/from
    an integral type of the same length; annlab::atomic must be used instead for, e.g., floating-point types.

    TODO: apply wherever relevant **/
#define __ANNLAB_atomic // intentionally empty, see above

template<class T, int S>
struct padded_base : T {
    char pad[NFS_MaxLineSize - sizeof(T) % NFS_MaxLineSize];
};
template<class T> struct padded_base<T, 0> : T {};

//! Pads type T to fill out to a multiple of cache line size.
template<class T>
struct padded : padded_base<T, sizeof(T)> {};

//! Extended variant of the standard offsetof macro
/** The standard offsetof macro is not sufficient for ANNLAB as it can be used for
    POD-types only. The constant 0x1000 (not NULL) is necessary to appease GCC. **/
#define __ANNLAB_offsetof(class_name, member_name) \
    ((ptrdiff_t)&(reinterpret_cast<class_name*>(0x1000)->member_name) - 0x1000)

//! Returns address of the object containing a member with the given name and address
#define __ANNLAB_get_object_ref(class_name, member_name, member_addr) \
    (*reinterpret_cast<class_name*>((char*)member_addr - __ANNLAB_offsetof(class_name, member_name)))

//! Throws std::runtime_error with what() returning error_code description prefixed with aux_info
void __ANNLAB_EXPORTED_FUNC handle_perror( int error_code, const char* aux_info );

#if defined(ANNLAB_USE_EXCEPTIONS) && ANNLAB_USE_EXCEPTIONS
    #define __ANNLAB_TRY try
    #define __ANNLAB_CATCH(e)   catch(e)
    #define __ANNLAB_THROW(e)   throw e
    #define __ANNLAB_RETHROW()  throw
#else /* !ANNLAB_USE_EXCEPTIONS */
    inline bool __ANNLAB_false() { return false; }
    #define __ANNLAB_TRY
    #define __ANNLAB_CATCH(e)   if ( annlab::internal::__ANNLAB_false() )
    #define __ANNLAB_THROW(e)   ((void)0)
    #define __ANNLAB_RETHROW()  ((void)0)
#endif /* !ANNLAB_USE_EXCEPTIONS */

//! Report a runtime warning.
void __ANNLAB_EXPORTED_FUNC runtime_warning( const char* format, ... );

#if ANNLAB_USE_ASSERT
static void* const poisoned_ptr = reinterpret_cast<void*>(-1);

//! Set p to invalid pointer value.
template<typename T>
inline void poison_pointer( T*& p ) { p = reinterpret_cast<T*>(poisoned_ptr); }

/** Expected to be used in assertions only, thus no empty form is defined. **/
template<typename T>
inline bool is_poisoned( T* p ) { return p == reinterpret_cast<T*>(poisoned_ptr); }
#else
template<typename T>
inline void poison_pointer( T* ) {/*do nothing*/}
#endif /* !ANNLAB_USE_ASSERT */

//! Cast pointer from U* to T.
/** This method should be used sparingly as a last resort for dealing with 
    situations that inherently break strict ISO C++ aliasing rules. */
template<typename T, typename U> 
inline T punned_cast( U* ptr ) {
    uintptr_t x = reinterpret_cast<uintptr_t>(ptr);
    return reinterpret_cast<T>(x);
}

//! Base class for types that should not be assigned.
class no_assign {
    // Deny assignment
    void operator=( const no_assign& );
public:
#if __GNUC__
    //! Explicitly define default construction, because otherwise gcc issues gratuitous warning.
    no_assign() {}
#endif /* __GNUC__ */
};

//! Base class for types that should not be copied or assigned.
class no_copy: no_assign {
    //! Deny copy construction
    no_copy( const no_copy& );
public:
    //! Allow default construction
    no_copy() {}
};

//! Class for determining type of std::allocator<T>::value_type.
template<typename T>
struct allocator_type {
    typedef T value_type;
};

#if defined(_MSC_VER) && _MSC_VER
//! Microsoft std::allocator has non-standard extension that strips const from a type. 
template<typename T>
struct allocator_type<const T> {
    typedef T value_type;
};
#endif

// Struct to be used as a version tag for inline functions.
/** Version tag can be necessary to prevent loader on Linux from using the wrong 
    symbol in debug builds (when inline functions are compiled as out-of-line). **/
struct version_tag_v1 {};

typedef version_tag_v1 version_tag;

} // internal
//! @endcond

}  // namespace annlab

#endif /* RC_INVOKED */

#endif  // _ANNLAB_STDDEF_H_
