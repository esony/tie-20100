#ifndef _UTILITY_HH
#define _UTILITY_HH

#ifndef _WIN32

  #ifndef _MEMORYLIMIT
    #define _MEMORYLIMIT 128
  #endif
  #define _MEMORYLIMITBYTES _MEMORYLIMIT * 1024 * 1024
  #ifndef _NOMEMORYLIMITS
    #include <sys/resource.h>
  #endif
#endif

void set_limits();

#endif
