/**********************************************/
/* Matthew Millar's Marvelous Magical Macros! */
/**********************************************/

// Uncomment to enable debug prints OR compile with -DDEBUG
// #define DEBUG

// Macro to print file name, line #, and func() name before error string
#define ERROR_PRINT(...)                                                      \
    do {                                                                      \
    fprintf(stderr, "%s:%d:%s(): ", __FILE__, __LINE__, __func__);            \
    fprintf(stderr, __VA_ARGS__);                                             \
    } while (0)                                                               \

// Macro to print file name, line #, and func() name before info string, only
// enabled when DEBUG is defined. Either by #define DEBUG or compiled with
// the option -DDEBUG
#ifdef DEBUG
   #define DEBUG_PRINT(...)                                                   \
      do {                                                                    \
      fprintf(stderr, "%s:%d:%s(): ", __FILE__, __LINE__, __func__);          \
      fprintf(stderr, __VA_ARGS__);                                           \
      } while (0)
#else
   #define DEBUG_PRINT(...)
#endif
