#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif

#include "random.h"
#define EXPORT_BOOLEAN
#include <mach/boolean.h>
#include <mach/kern_return.h>
#include <mach/message.h>
#include <mach/notify.h>
#include <mach/mach_types.h>
#include <mach/mig_errors.h>
#include <mach/mig_support.h>
#include <mach/msg_type.h>

#ifndef	mig_internal
#define	mig_internal	static
#endif

#ifndef	mig_external
#define mig_external
#endif

#ifndef	TypeCheck
#define	TypeCheck 1
#endif

#ifndef	UseExternRCSId
#define	UseExternRCSId		1
#endif

#define BAD_TYPECHECK(type, check) ({\
  union { mach_msg_type_t t; unsigned32_t w; } _t, _c;\
  _t.t = *(type); _c.t = *(check); _t.w != _c.w; })
#define msgh_request_port	msgh_remote_port
#define msgh_reply_port		msgh_local_port

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
mig_external kern_return_t S_get_random
(
	mach_port_t server_port,
	int *num
)
{
	typedef struct {
		mach_msg_header_t Head;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_t numType;
		int num;
	} Reply;

	union {
		Request In;
		Reply Out;
	} Mess;

	register Request *InP = &Mess.In;
	register Reply *OutP = &Mess.Out;

	mach_msg_return_t msg_result;
#if	TypeCheck
	unsigned int msgh_size;
#endif	/* TypeCheck */

	auto const mach_msg_type_t RetCodeCheck = {
		/* msgt_name = */		MACH_MSG_TYPE_INTEGER_32,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	auto const mach_msg_type_t numCheck = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	InP->Head.msgh_bits =
		MACH_MSGH_BITS(19, MACH_MSG_TYPE_MAKE_SEND_ONCE);
	/* msgh_size passed as argument */
	InP->Head.msgh_request_port = server_port;
	InP->Head.msgh_reply_port = mig_get_reply_port();
	InP->Head.msgh_seqno = 0;
	InP->Head.msgh_id = 500;

	msg_result = mach_msg(&InP->Head, MACH_SEND_MSG|MACH_RCV_MSG|MACH_MSG_OPTION_NONE, 24, sizeof(Reply), InP->Head.msgh_reply_port, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
	if (msg_result != MACH_MSG_SUCCESS) {
		mig_dealloc_reply_port(InP->Head.msgh_reply_port);
		return msg_result;
	}
	mig_put_reply_port(InP->Head.msgh_reply_port);

	if (OutP->Head.msgh_id != 600) {
		if (OutP->Head.msgh_id == MACH_NOTIFY_SEND_ONCE)
			return MIG_SERVER_DIED;
		else {
			mig_dealloc_reply_port(InP->Head.msgh_reply_port);
			return MIG_REPLY_MISMATCH;
		}
	}

#if	TypeCheck
	msgh_size = OutP->Head.msgh_size;

	if ((OutP->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    ((msgh_size != 40) &&
	     ((msgh_size != sizeof(mig_reply_header_t)) ||
	      (OutP->RetCode == KERN_SUCCESS))))
		return MIG_TYPE_ERROR;
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK (&OutP->RetCodeType, &RetCodeCheck))
		return MIG_TYPE_ERROR;
#endif	/* TypeCheck */

	if (OutP->RetCode != KERN_SUCCESS)
		return OutP->RetCode;

#if	TypeCheck
	if (BAD_TYPECHECK (&OutP->numType, &numCheck))
		return MIG_TYPE_ERROR;
#endif	/* TypeCheck */

	*num = OutP->num;

	return KERN_SUCCESS;
}
