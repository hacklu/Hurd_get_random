#ifndef	_random_user_
#define	_random_user_

/* Module random */

#include <mach/kern_return.h>
#include <mach/port.h>
#include <mach/message.h>

#include <mach/std_types.h>
#include <mach/mach_types.h>
#include <device/device_types.h>
#include <device/net_status.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/statfs.h>
#include <sys/resource.h>
#include <sys/utsname.h>
#include <hurd/hurd_types.h>

/* Routine get_random */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t S_get_random
(
	mach_port_t server_port,
	int *num
);

#endif	/* not defined(_random_user_) */
