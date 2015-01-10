#include "Daemonize.h"
#include "Utils.h"


//////////////////////////////////////////////////////////////////////////////////////////////////
void OnExit(void)
{
	ioctl(fileno(stdin), TCSETA, (char *) &sSaveTermModes);
}

void OnSigint(int i)
{
	exit(1);
}

void init_keyboard(void)
{
	struct termio	sCurrentTermModes;

	/* Get stdin terminal config */
	ioctl(fileno(stdin), TCGETA, (char *) &sSaveTermModes);

	/* Save them */
	memcpy(&sCurrentTermModes, &sSaveTermModes, sizeof(sCurrentTermModes));

	/* Remove stdin line mode edition */
	sCurrentTermModes.c_lflag &= ~(/*ISIG|*/ICANON|ECHO);
	sCurrentTermModes.c_cc[VMIN] = 1;
	sCurrentTermModes.c_cc[VTIME] = 1;
	ioctl(fileno(stdin), TCSETA, (char *) &sCurrentTermModes);

	/* Install at exit callback */
	atexit(OnExit);
	signal(SIGINT, OnSigint);
	signal(SIGQUIT, OnSigint);
	signal(SIGABRT, OnSigint);
	signal(SIGKILL, OnSigint);
	signal(SIGTERM, OnSigint);
}

void daemonize(configuration configuration)
{
	GlobalConfiguration = configuration;
	char *pidfile = GlobalConfiguration.pidFilePath;
	char str[20];

	/* Ensure only one copy */
	pidFilehandle = open(pidfile, O_RDWR|O_CREAT, 0600);
	if (pidFilehandle == -1)
	{
		/* Couldn't open lock file */
		syslog(LOG_INFO, "Could not open PID lock file %s, exiting", pidfile);
		exit(EXIT_FAILURE);
	}
 
	/* Try to lock file */
	if (lockf(pidFilehandle,F_TLOCK,0) == -1)
	{
		/* Couldn't get lock on lock file */
		syslog(LOG_INFO, "Could not lock PID lock file %s, exiting", pidfile);
		exit(EXIT_FAILURE);
	}
 
	/* Get and format PID */
	sprintf(str,"%d\n",getpid());
 
	/* write pid to lockfile */
	write(pidFilehandle, str, strlen(str));
}

void daemonShutdown()
{	
	syslog(LOG_INFO, "Shutdown daemon");
	close(pidFilehandle);
	// remove pidFile
	if (fileExists(GlobalConfiguration.pidFilePath))
	{
		syslog(LOG_INFO, "Removing Pid File '%s'", GlobalConfiguration.pidFilePath);
		remove(GlobalConfiguration.pidFilePath);
	} else {
		syslog(LOG_INFO, "Pid File '%s' does not exists", GlobalConfiguration.pidFilePath);
	}
}

void signal_handler(int sig)
{
	switch(sig)
	{
		case SIGHUP:
			syslog(LOG_WARNING, "Daemon Received SIGHUP signal.");
			break;
		case SIGINT:
		case SIGTERM:
			syslog(LOG_INFO, "Daemon exiting");
			daemonShutdown();
			exit(EXIT_SUCCESS);
			break;

		default:
			syslog(LOG_WARNING, "Unhandled signal %s", strsignal(sig));
			break;

	}
}
