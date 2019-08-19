#include <unistd.h>
#include <time.h>

/* Get configurable system variables.  */

long
sysconf(int name)
{
  unsigned long long timebase;

  switch (name)
    {
    case _SC_CLK_TCK:
      metal_timer_get_timebase_frequency(0, &timebase);
      return (long) timebase;
    }

  return -1;
}
