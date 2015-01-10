
#ifndef __DAEMONIZE_H__
#define __DAEMONIZE_H__


#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <limits.h>
#include <termios.h>
#include <pthread.h>
#include <unistd.h>
#include <syslog.h>
#include <signal.h>

#include <errno.h>
#include <fcntl.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <time.h>
#include <sys/sysinfo.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ConfigDaemon.h"

static struct termio 	sSaveTermModes;
static int pidFilehandle;

void OnExit(void);

void OnSigint(int i);

void init_keyboard(void);

void daemonize(configuration configuration);

void daemonShutdown();

void signal_handler(int sig);

#endif