#ifndef _GNU_SOURCE
  #define _GNU_SOURCE
#endif

#include <stdio.h>
#include <getopt.h>
#include <errno.h>
#include <sys/stat.h>
#include <error.h>

#include <hurd/ports.h>
#include <hurd/hurd_types.h>
#include <hurd/trivfs.h>

#include "random.h"
#include <mach.h>

extern boolean_t S_random_server
(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP);

int trivfs_fstype = FSTYPE_MISC;
int trivfs_fsid = 0;
int trivfs_support_read = 0;
int trivfs_support_write = 0;
int trivfs_support_exec = 0;
int trivfs_allow_open = 0x00;
int trivfs_protid_nportclasses = 0;
int trivfs_cntl_nportclasses = 0;

/*int data_value;*/

int demuxer (mach_msg_header_t *inp, mach_msg_header_t *outp)
{
  return (S_random_server(inp,outp)||trivfs_demuxer(inp,outp));
}

void trivfs_modify_stat (struct trivfs_protid *cred, io_statbuf_t  *st)
{
}
error_t trivfs_goaway (struct trivfs_control *fsys, int flags)
{
  exit (0);
}

kern_return_t S_get_random (mach_port_t data_port, int *value)
{
  *value = (int) random()%1000;
  return 0;
}

int
main(int argc, char *argv[])
{
  int err;
  mach_port_t bootstrap;
  struct trivfs_control *fsys;

  if (argc > 1)
    {
      fprintf(stderr, "Usage: settrans [opts] node %s\n", program_invocation_name); 
      exit (1);
    }

  task_get_bootstrap_port (mach_task_self (), &bootstrap);
  if (bootstrap == MACH_PORT_NULL)
    error(2, 0, "Must be started as a translator");

  /* Reply to our parent */
  err = trivfs_startup (bootstrap, 0, 0, 0, 0, 0,&fsys);
  mach_port_deallocate (mach_task_self (), bootstrap);
  if (err) {
    return (0);
  }

  ports_manage_port_operations_one_thread (fsys->pi.bucket, demuxer, 0);

  return 0;
}
