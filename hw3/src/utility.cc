
#include "utility.hh"

void set_limits()
{
#ifndef _WIN32
  #ifndef _NOMEMORYLIMITS
    struct rlimit new_limit;
    struct rlimit *newp;
    new_limit.rlim_cur = _MEMORYLIMITBYTES;
    new_limit.rlim_max = _MEMORYLIMITBYTES;
    newp = &new_limit;
    setrlimit(RLIMIT_AS, newp );
  #endif
#endif
}
