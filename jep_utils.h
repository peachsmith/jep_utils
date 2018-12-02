
#ifdef _WIN32
#ifdef JEP_UTILS_EXPORTS
#define JEP_UTILS_API __declspec(dllexport)
#else
#define JEP_UTILS_API __declspec(dllimport)
#endif
#elif defined(__linux__)
#define JEP_UTILS_API
#elif defined(__APPLE__)
#define JEP_UTILS_API
#endif

/**
 * A test function
 *
 * Returns:
 *   int - an integer
 */
JEP_UTILS_API int test_func(void);

/**
 * Another test function. This increments
 * an integer.
 *
 * Params:
 *   int - an integer to increment
 *
 * Returns:
 *   int - the incremented integer
 */
JEP_UTILS_API int add_one(int n);
