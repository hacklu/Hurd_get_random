CC = gcc
MIG = mig
CFLAGS = -Wall -g  -D_GNU_SOURCE
LDFLAGS = -lthreads -lports -ltrivfs -lfshelp -lshouldbeinlibc
INCLUDES = -I.
LCHDRS = 
MIGCOMSFLAGS = -prefix S_
OBJS = $(SRCS:.c=.o)

all: random-user random-server

stubs: random.defs
	$(MIG) $(MIGCOMSFLAGS) $^
random-server: random-server.c randomServer.c 
	$(CC) $^ $(CFLAGS) $(INCLUDES) $(LDFLAGS) -o $@
random-user: random-user.c randomUser.c
	$(CC) $^ $(CFLAGS) $(INCLUDES) -o $@
clean:
	rm -f *.o random-server random-user

start: random-server random-user
	settrans -ac /tmp/trans random-server
	ps -x | grep random-server
end:
	settrans -fg /tmp/trans
