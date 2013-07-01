#include <stdio.h>
#include <hurd.h>
#include <hurd/hurd_types.h>
#include "random.h"

#ifndef _GNU_SOURCE
  #define _GNU_SOURCE
#endif

int
main(int argc, char *argv[])
{
  int value=0;
  mach_port_t random_server_port;

  random_server_port = file_name_lookup ("/tmp/trans", 0, 0);
  printf ("random_server_port [%u]\n", (unsigned int)random_server_port);
  S_get_random (random_server_port, &value);
  printf ("get_random: [%d]\n", value);

  return 0;
}
