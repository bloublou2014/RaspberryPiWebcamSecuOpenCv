
#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <stdio.h>

#include <stdexcept>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>


#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <unistd.h>
#include <syslog.h>
#include <fcntl.h>
#include <string.h>
#include <sstream>

#include "Ini.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct
{
	bool isValid;
	int version;

	char* pidFilePath;

	bool saveImages;
	int cameraNumber;

	bool saveStatitics;
	int statisticsPeriodSec;
	
	bool saveTimeLaps;
	int timeLapsPeriodSec;
	int timeLapsStartMinute;
	int timeLapsEndMinute;

	bool checkTriggerFile;
	int checkTriggerFilePeriodSec;

	char* picsDirectory;
	char* logsDirectory;
	char* takeSnapshotTriggerFile;

	char* imgExtension;
	int delaySnapshotMs;
	int delayMinSaveImgMs;

	int imageTreshold;
	int cameraResolutionWidth;
	int cameraResolutionHeight;

	int processResolutionWidth;
	int processResolutionHeight;

	char* imageDirectoryFormat;
	char* imageFileFormat;
	char* imageCroppedDirectoryFormat;
	char* imageCroppedFileFormat;
	char* statsFileFormat;
	char* timelapsFileFormat;
	char* triggerFileFormat;

	int detectionNbZones;
	int detectionWindowsX;
	int detectionWindowsY;
	int detectionWindowsWidth;
	int detectionWindowsHeight;

	int detectionMotionLevel;
	int detectionMinimumPixelChangePct;
} configuration;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define DAEMON_NAME "CameraMonitorDaemon"
#define DAEMON_PIDFILE "/tmp/cameraMonitorDaemon.pid"
#define CONFIG_FILEPATH "/etc/cameraMonitorDaemon.conf"

#define CONFIG_VISUALIZEIMAGE false
#define CONFIG_SAVEIMAGE true
#define CONFIG_CAMERANUMBER 0

#define CONFIG_SAVESTATITICS false
#define CONFIG_STATISTICSPERIODSEC 600

#define CONFIG_SAVETIMELAPS false
#define CONFIG_TIMELAPSPERIODSEC 1200 // 10 min
#define CONFIG_TIMELAPSSTARTMINUTE 300 // 5h
#define CONFIG_TIMELAPSENDMINUTE 1260 // 21h


#define CONFIG_CHECKTRIGGERFILE false
#define CONFIG_CHECKTRIGGERFILEPERIODSEC 60

#define CONFIG_PICSDIRECTORY ""
#define CONFIG_LOGSDIRECTORY ""
#define CONFIG_TAKESNAPSHOTTRIGGERFILE ""
#define CONFIG_IMGEXTENSION ".jpg"
#define CONFIG_DELAYSNAPSHOTMS 500
#define CONFIG_DELAYMINSAVEIMGMS 1000
#define CONFIG_IMAGETRESHOLD 30
#define CONFIG_CAMERARESOLUTIONWIDTH 1280
#define CONFIG_CAMERARESOLUTIONHEIGHT 720

#define CONFIG_PROCESSRESOLUTIONWIDTH 1280
#define CONFIG_PROCESSRESOLUTIONHEIGHT 720

#define CONFIG_IMAGEDIRECTORYFORMAT "%Y%m%d"
#define CONFIG_IMAGEFILEFORMAT "%Y%m%d_%H%M%S"
#define CONFIG_IMAGECROPPEDFILEFORMAT "%Y%m%d_%H%M%S_cropped"
#define CONFIG_IMAGECROPPEDDIRECTORYFORMAT "%Y%m%d/cropped"
#define CONFIG_STATSFILEFORMAT "%Y%m%d.log"
#define CONFIG_TIMELAPSFILEFORMAT "%Y%m%d_%H%M%S_tl"
#define CONFIG_TRIGGERFILEFORMAT "%Y%m%d_%H%M%S_snapshot"


#define CONFIG_DETECTIONNBZONES 120
#define CONFIG_DETECTIONWINDOWSX 0
#define CONFIG_DETECTIONWINDOWSY 0
#define CONFIG_DETECTIONWINDOWSWIDTH 0
#define CONFIG_DETECTIONWINDOWSHEIGHT 0
#define CONFIG_DETECTIONMOTIONLEVEL 30
#define CONFIG_DETECTIONMINIMUMPIXELCHANGEPCT 4

static int configurationHandler(
		void* user, 
		const char* section, 
		const char* name,
		const char* value);

configuration LoadIniFile (char* iniFilePath);

void LogConfiguration(configuration configuration);

string SaveConfigurationToString(configuration configuration);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static configuration GlobalConfiguration;

// --------------------------------------------------------
#endif