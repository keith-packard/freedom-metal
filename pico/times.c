#include <sys/times.h>
#include <sys/time.h>
#include <metal/timer.h>

/*
 * Simple enough to return the tick count by
 * just getting it from the hardware.
 */


clock_t
times(struct tms *buf)
{
    unsigned long long mcc;
    metal_timer_get_cyclecount(0, &mcc);
    buf->tms_stime = 0;
    buf->tms_cutime = 0;
    buf->tms_cstime = 0;
    return buf->tms_utime = mcc;
}

clock_t
_times_r(struct reent *reent, struct tms *buf)
{
    (void) reent;
    return times(buf);
}
