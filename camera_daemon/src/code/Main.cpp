/*
Copyright (c) 2015 bloublou

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#define LINUX


#ifdef LINUX
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
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#endif

#include "Utils.h"
#include "Detection.h"

#include "ConfigDaemon.h"
#include "Daemonize.h"

using namespace std;
using namespace cv;

//////////////////////////////////////////////////////////////////////////////////////////////////

#define VERSION "2.0.0"

//////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char * const argv[])
{
	char*	iniFilePath;

	// get output directory for files

	if (argc < 2)
	{
		printf("First argument should be -INI [ini file path] or -STOP\n");
		return -1;
	}

	if (strcasecmp(argv[1], "-STOP") == 0)
	{
		printf("Stopping daemon here...\n");
		return -1;
	}

	if (strcasecmp(argv[1], "-INI") != 0)
	{
		printf("First argument should -OUTPUT [output directory]\n");
		return -1;
	}

	iniFilePath = argv[2];

	syslog(LOG_INFO, "==================================================");
	syslog(LOG_INFO, "Camera Daemon for Raspberry Pi with USB Camera");
	syslog(LOG_INFO, "Version :");
	syslog(LOG_INFO, VERSION);


	if (!fileExists(iniFilePath))
	{
		syslog(LOG_ERR, "Ini file '%s' does not exists\n", iniFilePath);
		printf("Ini file '%s' does not exists\n", iniFilePath);
		return -1;
	}

	GlobalConfiguration = LoadIniFile(iniFilePath);
	LogConfiguration(GlobalConfiguration);

	if (GlobalConfiguration.isValid == false)
	{
		syslog(LOG_ERR, "Invalid Ini file '%s'\n", iniFilePath);
		return -1;
	}

	// check configuration
	if (GlobalConfiguration.saveImages)
	{
		char* picsDirectory = GlobalConfiguration.picsDirectory;

		if (directoryExists(picsDirectory) == false)
		{
			syslog(LOG_ERR, "Directory '%s' must exists if pics must be saved\n", picsDirectory);
			return -1;
		}
	}

	char* logsDirectory = GlobalConfiguration.logsDirectory;

	if (directoryExists(logsDirectory) == false)
	{
		syslog(LOG_ERR, "Directory '%s' must exists for logs.\n", logsDirectory);
		return -1;
	}

	/* Our process ID and Session ID */
	pid_t pid, sid;
	struct sigaction newSigAction;
	sigset_t newSigSet;

	/* Check if parent process id is set */
	if (getppid() == 1)
	{
		/* PPID exists, therefore we are already a daemon */
		return 0;
	}

	/* Set signal mask - signals we want to block */
	sigemptyset(&newSigSet);
	sigaddset(&newSigSet, SIGCHLD);  /* ignore child - i.e. we don't need to wait for it */
	sigaddset(&newSigSet, SIGTSTP);  /* ignore Tty stop signals */
	sigaddset(&newSigSet, SIGTTOU);  /* ignore Tty background writes */
	sigaddset(&newSigSet, SIGTTIN);  /* ignore Tty background reads */
	sigprocmask(SIG_BLOCK, &newSigSet, NULL);   /* Block the above specified signals */

	/* Set up a signal handler */
	newSigAction.sa_handler = signal_handler;
	sigemptyset(&newSigAction.sa_mask);
	newSigAction.sa_flags = 0;

	/* Signals to handle */
	sigaction(SIGHUP, &newSigAction, NULL);     /* catch hangup signal */
	sigaction(SIGTERM, &newSigAction, NULL);    /* catch term signal */
	sigaction(SIGINT, &newSigAction, NULL);     /* catch interrupt signal */


	/* Fork off the parent process */
	pid = fork();
	if (pid < 0) {
		exit(EXIT_FAILURE);
	}
	/* If we got a good PID, then
		we can exit the parent process. */
	if (pid > 0) {
		exit(EXIT_SUCCESS);
	}

	/* Change the file mode mask */
	umask(0);

	/* Open any logs here */

	/* Create a new SID for the child process */
	sid = setsid();
	if (sid < 0) {
		/* Log the failure */
		exit(EXIT_FAILURE);
	}

	/* Change the current working directory */
	if ((chdir(GlobalConfiguration.picsDirectory)) < 0) {
		/* Log the failure */
		exit(EXIT_FAILURE);
	}

	/* Close out the standard file descriptors */
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	/* Daemon-specific initialization goes here */
	daemonize(GlobalConfiguration);

	syslog(LOG_INFO, "Daemonize done");

	/* =================================================== Init =================================================== */
	int nbHistoryToCheck;
	Mat d1, d2, motion, motionTreshold, motionErode;
	int number_of_changes, number_of_sequence = 0;
	Scalar mean_, color(0, 255, 255); // yellow

	int historyLength = GlobalConfiguration.cameraResolutionWidth;
	int lastHistoryIndexSaved = -1;
	int historyIndex = 0;
	int *history;
	Mat historyOutput;

	Mat kernel_ero;
	Mat result, result_cropped;
	Mat prev_frame, current_frame, next_frame;

	// algorithm

	// stats	
	time_t statsNextSeconds;
	time_t statsStartSeconds;
	char statsStartSecondsChars[20];

	int nbDetections;
	int nbPositiveDetections;

	// timelaps
	time_t timelapsNextSeconds;

	// check 
	time_t checkTriggerNextSeconds;


	VideoCapture camera(GlobalConfiguration.cameraNumber);
	Detector *detector;

	try {
		nbHistoryToCheck = GlobalConfiguration.delayMinSaveImgMs / GlobalConfiguration.delaySnapshotMs;
		history = new int[historyLength];

		if (!camera.isOpened())
			throw "Cannot connect to camera during initialization";

		camera.set(CV_CAP_PROP_FRAME_WIDTH, GlobalConfiguration.cameraResolutionWidth); // width of viewport of camera
		camera.set(CV_CAP_PROP_FRAME_HEIGHT, GlobalConfiguration.cameraResolutionHeight); // height of ...

		// Take images and convert them to gray
		camera >> result;
		prev_frame = result;

		camera >> current_frame;
		camera >> next_frame;

		// convert all frames to grayscale
		cvtColor(current_frame, current_frame, CV_RGB2GRAY);
		cvtColor(prev_frame, prev_frame, CV_RGB2GRAY);
		cvtColor(next_frame, next_frame, CV_RGB2GRAY);

		resizeImage(prev_frame, GlobalConfiguration.processResolutionWidth, GlobalConfiguration.processResolutionHeight);
		resizeImage(current_frame, GlobalConfiguration.processResolutionWidth, GlobalConfiguration.processResolutionHeight);
		resizeImage(next_frame, GlobalConfiguration.processResolutionWidth, GlobalConfiguration.processResolutionHeight);

		// d1 and d2 for calculating the differences
		// result, the result of and operation, calculated on d1 and d2
		// number_of_changes, the amount of changes in the result matrix.
		// color, the color for drawing the rectangle when something has changed.

		// init history 
		for (int i = 0; i < historyLength; i++)
			history[i] = 0;

		if (GlobalConfiguration.detectionWindowsWidth < 1)
		{
			GlobalConfiguration.detectionWindowsWidth = GlobalConfiguration.processResolutionWidth - GlobalConfiguration.detectionWindowsX;
			syslog(LOG_INFO, "detectionWindowsWidth : %d pixel", GlobalConfiguration.detectionWindowsWidth);
		}

		if (GlobalConfiguration.detectionWindowsHeight < 1)
		{
			GlobalConfiguration.detectionWindowsHeight = GlobalConfiguration.processResolutionHeight - GlobalConfiguration.detectionWindowsY;
			syslog(LOG_INFO, "detectionWindowsHeight : %d pixel", GlobalConfiguration.detectionWindowsHeight);
		}

		// Erode kernel
		kernel_ero = getStructuringElement(MORPH_RECT, Size(2, 2));

		/* Initalize algorithm parameters */
		syslog(LOG_INFO, "Initialize Detector");
		detector = new Detector(
			Rect(GlobalConfiguration.detectionWindowsX,
			GlobalConfiguration.detectionWindowsY,
			GlobalConfiguration.detectionWindowsWidth,
			GlobalConfiguration.detectionWindowsHeight),
			GlobalConfiguration.detectionMinimumPixelChangePct,
			GlobalConfiguration.detectionNbZones
			);

		detector->motionColor = color;

		/* Initialize statistics */
		if (GlobalConfiguration.saveStatitics)
		{
			time(&statsNextSeconds);
			time(&statsStartSeconds);
			struct tm * statsStartSecondsTimeinfo;
			statsStartSecondsTimeinfo = gmtime(&statsStartSeconds);
			strftime(statsStartSecondsChars, 20, "%Y/%m/%d %H:%M:%S", statsStartSecondsTimeinfo);

			statsNextSeconds += GlobalConfiguration.statisticsPeriodSec;
			nbDetections = 0;
			nbPositiveDetections = 0;
		}

		/* Initialize timelaps */
		if (GlobalConfiguration.saveTimeLaps)
		{
			timelapsNextSeconds = getTime();
			timelapsNextSeconds += GlobalConfiguration.timeLapsPeriodSec;

			// get next start time
			timelapsNextSeconds =
				getNextStepSecond(
				getTime(),
				GlobalConfiguration.timeLapsPeriodSec,
				GlobalConfiguration.timeLapsStartMinute * 60,
				GlobalConfiguration.timeLapsEndMinute * 60);

			char TIME[20];
			struct tm * tmpInfo;
			tmpInfo = gmtime(&timelapsNextSeconds);
			strftime(TIME, 80, "%Y/%m/%d %H:%M:%S", tmpInfo);
			syslog(LOG_INFO, "Next timelaps start : %s", TIME);
		}

		/* Initialize file checker */
		if (GlobalConfiguration.checkTriggerFile)
		{
			checkTriggerNextSeconds = getTime();
			checkTriggerNextSeconds += GlobalConfiguration.checkTriggerFilePeriodSec;
		}

		syslog(LOG_INFO, "Init successful");
	}
	catch (std::exception& e)
	{
		stringstream ss;
		ss << "Error initializing with message '" << e.what() << "'\n";
		syslog(LOG_ERR, ss.str().c_str());
		exit(EXIT_FAILURE);
	}

	/* The Big Loop */
	try {
		/* ================================================================================= */
		bool saveFirstFrame = true;
		syslog(LOG_INFO, "Starting infinite Loop");
		time_t currentSeconds;
		struct tm * currentUtcTimeinfo;
		long noFrame = 0;

		while (1) {
			noFrame++;
			time(&currentSeconds);
			currentUtcTimeinfo = gmtime(&currentSeconds);

			// Take a new image
			prev_frame = current_frame;
			current_frame = next_frame;

			camera >> next_frame;
			result = next_frame.clone();

			// save timelaps ?
			if (GlobalConfiguration.saveTimeLaps && currentSeconds >= timelapsNextSeconds)
			{
				saveImg(
					detector->visualizeLastChanges(history, historyLength, historyIndex, result, 50),
					currentUtcTimeinfo,
					GlobalConfiguration.picsDirectory,
					GlobalConfiguration.imgExtension,
					GlobalConfiguration.imageDirectoryFormat,
					GlobalConfiguration.timelapsFileFormat);

				timelapsNextSeconds =
					getNextStepSecond(
					currentSeconds,
					GlobalConfiguration.timeLapsPeriodSec,
					GlobalConfiguration.timeLapsStartMinute * 60,
					GlobalConfiguration.timeLapsEndMinute * 60);

				char TIME[20];
				struct tm * tmpInfo;
				tmpInfo = gmtime(&timelapsNextSeconds);
				strftime(TIME, 80, "%Y/%m/%d %H:%M:%S", tmpInfo);
				syslog(LOG_INFO, "Next timelaps start : %s", TIME);
			}

			// save stats ?
			if (GlobalConfiguration.saveStatitics && currentSeconds >= statsNextSeconds)
			{
				stringstream textToSave;
				textToSave.str("");
				textToSave << "{'version'='1',";

				char TIME[20];

				strftime(TIME, 80, "%Y/%m/%d %H:%M:%S", currentUtcTimeinfo);

				textToSave << "'startdate'='" << statsStartSecondsChars << "', ";
				textToSave << "'enddate'='" << TIME << "', ";


				// save parameters
				string ss = SaveConfigurationToString(GlobalConfiguration);

				textToSave << ss;

				textToSave << "'zoneSize'='" << detector->zoneSize << "', ";
				textToSave << "'nbZoneX'='" << detector->cols << "', ";
				textToSave << "'nbZoneY'='" << detector->rows << "', ";
				textToSave << "'zoneThresholdPixel'='" << detector->zoneThresholdPixel << "', ";

				if (detector->IsStatsEnabled && detector->countZonesStats != NULL)
				{
					textToSave << "'countZonesStats'=[";
					for (int i = 0; i < (detector->cols*detector->rows); i++)
					{
						textToSave << "'" << detector->countZonesStats[i] << "', ";
					}
					textToSave << "], ";
					detector->countZonesStats = NULL;
				}
				else {
					textToSave << "'countZonesStats'=[], ";
				}

				textToSave << "'nbDetections'='" << nbDetections << "', ";
				textToSave << "'nbPositiveDetections'='" << nbPositiveDetections << "'";


				textToSave << "}\n";

				// save statistics to file
				saveToFile(
					currentUtcTimeinfo,
					GlobalConfiguration.logsDirectory,
					GlobalConfiguration.imageDirectoryFormat,
					GlobalConfiguration.statsFileFormat,
					textToSave.str());

				statsStartSeconds = currentSeconds;
				strftime(statsStartSecondsChars, 20, "%Y/%m/%d %H:%M:%S", currentUtcTimeinfo);

				statsNextSeconds = currentSeconds + GlobalConfiguration.statisticsPeriodSec;
				nbDetections = 0;
				nbPositiveDetections = 0;
			}

			/* check trigger file ? */
			if (GlobalConfiguration.checkTriggerFile && currentSeconds >= checkTriggerNextSeconds)
			{
				if (fileExists(GlobalConfiguration.takeSnapshotTriggerFile))
				{
					// take a snapshot 
					saveImg(
						result,
						currentUtcTimeinfo,
						GlobalConfiguration.picsDirectory,
						GlobalConfiguration.imgExtension,
						GlobalConfiguration.imageDirectoryFormat,
						GlobalConfiguration.triggerFileFormat);

					remove(GlobalConfiguration.takeSnapshotTriggerFile);
				}

				checkTriggerNextSeconds = currentSeconds + GlobalConfiguration.checkTriggerFilePeriodSec;
			}

			// convert to grayscale
			cvtColor(next_frame, next_frame, CV_RGB2GRAY);
			resizeImage(next_frame, GlobalConfiguration.processResolutionWidth, GlobalConfiguration.processResolutionHeight);

			// Compute a diff between previous and current image
			// output a motion picture
			absdiff(prev_frame, next_frame, d1);
			absdiff(next_frame, current_frame, d2);
			bitwise_and(d1, d2, motion);

			// threshold motion image & erode to avoid too many false positive
			threshold(motion, motionTreshold, GlobalConfiguration.imageTreshold, 255, CV_THRESH_BINARY);
			erode(motionTreshold, motionErode, kernel_ero);

			// lastFrameTotalActiveZone
			detector->process(
				motionErode,
				result,
				result_cropped);
			number_of_changes = detector->lastFrameTotalActiveZone;

			// history 
			history[historyIndex] = number_of_changes;
			historyIndex++;

			if (lastHistoryIndexSaved <= historyIndex)
			{
				lastHistoryIndexSaved = -1;
			}

			if (historyIndex >= historyLength)
			{
				historyIndex = 0;
			}

			nbDetections++;

			// If a lot of changes happened, we assume something changed.
			if (number_of_changes >= GlobalConfiguration.detectionMotionLevel || saveFirstFrame)
			{
				nbPositiveDetections++;

				bool saveIt = detector->checkIfBetterLastSecond(history, historyLength, historyIndex, lastHistoryIndexSaved, nbHistoryToCheck);
				lastHistoryIndexSaved = historyIndex;

				if ((GlobalConfiguration.saveImages) && (number_of_sequence > 0 || saveFirstFrame || saveIt))
				{
					historyOutput = detector->visualizeLastChanges(history, historyLength, historyIndex, result, 50);
					if (saveFirstFrame)
					{
						result_cropped = result;
					}

					// Get the current time

					// check if this motion is over last second one
					//saveImg(result,DIR,EXT,DIR_FORMAT.c_str(),FILE_FORMAT.c_str());
					saveImg(
						historyOutput,
						currentUtcTimeinfo,
						GlobalConfiguration.picsDirectory,
						GlobalConfiguration.imgExtension,
						GlobalConfiguration.imageDirectoryFormat,
						GlobalConfiguration.imageFileFormat);

					saveImg(
						result_cropped,
						currentUtcTimeinfo,
						GlobalConfiguration.picsDirectory,
						GlobalConfiguration.imgExtension,
						GlobalConfiguration.imageCroppedDirectoryFormat,
						GlobalConfiguration.imageCroppedFileFormat);
				}
				saveFirstFrame = false;
				number_of_sequence++;

			}
			else
			{
				number_of_sequence = 0;
				// Delay, wait a 1/2 second.
				cvWaitKey(GlobalConfiguration.delaySnapshotMs);
			}
		}
		/* ================================================================================= */
	}
	catch (std::exception& e)
	{
		stringstream ss;
		ss << "Error initializing with message '" << e.what() << "'\n";
		syslog(LOG_ERR, ss.str().c_str());
		exit(EXIT_FAILURE);
	}
	syslog(LOG_INFO, "Exit successful");
	exit(EXIT_SUCCESS);

	/* ================================================================================= */
}

