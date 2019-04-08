/* Debugging with contracts; enable with gcc -DDEBUG ...
 *
 * 15-122 Principles of Imperative Computation
 * Frank Pfenning
 */

#include <assert.h>

#undef ASSERT
#undef REQUIRES
#undef ENSURES

#ifdef DEBUG

#define ASSERT(COND) assert(COND)
#define REQUIRES(COND) assert(COND)
#define ENSURES(COND) assert(COND)

#else

// TODO make this nicer!
#define ASSERT(COND) (assert(1 || (COND)))
#define REQUIRES(COND) (assert(1 || (COND)))
#define ENSURES(COND) (assert(1 || (COND)))

#endif
