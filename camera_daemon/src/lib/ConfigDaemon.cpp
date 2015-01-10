// 
//
//

#include "ConfigDaemon.h"
#include "Utils.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int configurationHandler(
		void* user, 
		const char* section, 
		const char* name,
		const char* value)
{
	configuration* pconfig = (configuration*)user;

	#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0

	if (MATCH("config", "pidFilePath")) {
		pconfig->pidFilePath = strdup(value);
	} else if (MATCH("config", "version")) {
		pconfig->version = atoi(value);
	} else 
		
		   if (MATCH("config", "saveImages")) {
		pconfig->saveImages = strdup(value);
	} else if (MATCH("config", "cameraNumber")) {
		pconfig->cameraNumber = atoi(value);
	} else 
		
		   if (MATCH("config", "saveStatitics")) {
		pconfig->saveStatitics = strdup(value);
	} else if (MATCH("config", "statisticsPeriodSec")) {
		pconfig->statisticsPeriodSec = atoi(value);
	} else 

		   if (MATCH("config", "saveTimeLaps")) {
		pconfig->saveTimeLaps = strdup(value);
	} else if (MATCH("config", "timeLapsPeriodSec")) {
		pconfig->timeLapsPeriodSec = atoi(value);
	} else if (MATCH("config", "timeLapsStartMinute")) {
		pconfig->timeLapsStartMinute = atoi(value);
	} else if (MATCH("config", "timeLapsEndMinute")) {
		pconfig->timeLapsEndMinute = atoi(value);
	} else

	  	   if (MATCH("config", "checkTriggerFile")) {
		pconfig->checkTriggerFile = strdup(value);
	} else if (MATCH("config", "checkTriggerFilePeriodSec")) {
		pconfig->checkTriggerFilePeriodSec = atoi(value);
	} else

		   if (MATCH("config", "picsDirectory")) {
		pconfig->picsDirectory = strdup(value);
	} else if (MATCH("config", "logsDirectory")) {
		pconfig->logsDirectory = strdup(value);
	} else if (MATCH("config", "takeSnapshotTriggerFile")) {
		pconfig->takeSnapshotTriggerFile = strdup(value);
	} else 
	
	        if (MATCH("config", "imgExtension")) {
		pconfig->imgExtension = strdup(value);
	} else if (MATCH("config", "delaySnapshotMs")) {
		pconfig->delaySnapshotMs = atoi(value);
	} else if (MATCH("config", "delayMinSaveImgMs")) {
		pconfig->delayMinSaveImgMs = atoi(value);
	} else 

		   if (MATCH("config", "imageTreshold")) {
		pconfig->imageTreshold = atoi(value);
	} else if (MATCH("config", "cameraResolutionWidth")) {
		pconfig->cameraResolutionWidth = atoi(value);
	} else if (MATCH("config", "cameraResolutionHeight")) {
		pconfig->cameraResolutionHeight = atoi(value);
	} else 
		   if (MATCH("config", "processResolutionWidth")) {
		pconfig->processResolutionHeight = atoi(value);
	} else if (MATCH("config", "processResolutionHeight")) {
		pconfig->processResolutionHeight = atoi(value);
	} else
		
		   if (MATCH("config", "imageDirectoryFormat")) {
		pconfig->imageDirectoryFormat = strdup(value);
	} else if (MATCH("config", "imageFileFormat")) {
		pconfig->imageFileFormat = strdup(value);
	} else if (MATCH("config", "imageCroppedFileFormat")) {
		pconfig->imageCroppedFileFormat = strdup(value);
	} else if (MATCH("config", "imageCroppedDirectoryFormat")) {
		pconfig->imageCroppedDirectoryFormat = strdup(value);
	} else if (MATCH("config", "statsFileFormat")) {
		pconfig->statsFileFormat = strdup(value);
	} else if (MATCH("config", "timelapsFileFormat")) {
		pconfig->timelapsFileFormat = strdup(value);
	} else if (MATCH("config", "triggerFileFormat")) {
		pconfig->triggerFileFormat = strdup(value);
	} else

		  if (MATCH("config", "detectionNbZones")) {
		pconfig->detectionNbZones = atoi(value);
	} else  if (MATCH("config", "detectionWindowsX")) {
		pconfig->detectionWindowsX = atoi(value);
	} else if (MATCH("config", "detectionWindowsY")) {
		pconfig->detectionWindowsY = atoi(value);
	} else if (MATCH("config", "detectionWindowsWidth")) {
		pconfig->detectionWindowsWidth = atoi(value);
	} else if (MATCH("config", "detectionWindowsHeight")) {
		pconfig->detectionWindowsHeight = atoi(value);
	} else
	
		 if (MATCH("config", "detectionMotionLevel")) {
		pconfig->detectionMotionLevel = atoi(value);
	} else if (MATCH("config", "detectionMinimumPixelChangePct")) {
		pconfig->detectionMinimumPixelChangePct = atoi(value);
	} else{
		return 0;  /* unknown section/name, error */
	}
	return 1;
}

// load default values & from configuration file 
configuration LoadIniFile (char* iniFilePath)
{
	configuration config;
	config.pidFilePath = DAEMON_PIDFILE;

	config.saveImages = CONFIG_SAVEIMAGE;
	
	config.cameraNumber = CONFIG_CAMERANUMBER;

	config.saveStatitics = CONFIG_SAVESTATITICS;
	config.statisticsPeriodSec = CONFIG_STATISTICSPERIODSEC;

	config.saveTimeLaps = CONFIG_SAVETIMELAPS;
	config.timeLapsPeriodSec = CONFIG_TIMELAPSPERIODSEC;
	config.timeLapsStartMinute = CONFIG_TIMELAPSSTARTMINUTE;
	config.timeLapsEndMinute = CONFIG_TIMELAPSENDMINUTE;
	
	config.checkTriggerFile = CONFIG_CHECKTRIGGERFILE;
	config.checkTriggerFilePeriodSec = CONFIG_CHECKTRIGGERFILEPERIODSEC;

	config.picsDirectory = CONFIG_PICSDIRECTORY;
	config.logsDirectory = CONFIG_LOGSDIRECTORY;
	config.takeSnapshotTriggerFile = CONFIG_TAKESNAPSHOTTRIGGERFILE;
	
	config.imgExtension = CONFIG_IMGEXTENSION;
	
	config.delaySnapshotMs = CONFIG_DELAYSNAPSHOTMS;
	config.delayMinSaveImgMs = CONFIG_DELAYMINSAVEIMGMS;

	config.imageTreshold = CONFIG_IMAGETRESHOLD;
	config.cameraResolutionWidth = CONFIG_CAMERARESOLUTIONWIDTH;
	config.cameraResolutionHeight = CONFIG_CAMERARESOLUTIONHEIGHT;

	config.processResolutionWidth = CONFIG_PROCESSRESOLUTIONWIDTH;
	config.processResolutionHeight = CONFIG_PROCESSRESOLUTIONHEIGHT;

	config.imageDirectoryFormat = CONFIG_IMAGEDIRECTORYFORMAT;
	config.imageFileFormat = CONFIG_IMAGEFILEFORMAT;
	config.imageCroppedFileFormat = CONFIG_IMAGECROPPEDFILEFORMAT;
	config.imageCroppedDirectoryFormat = CONFIG_IMAGECROPPEDDIRECTORYFORMAT;
	config.statsFileFormat = CONFIG_STATSFILEFORMAT;
	config.timelapsFileFormat = CONFIG_TIMELAPSFILEFORMAT;

	config.detectionNbZones = CONFIG_DETECTIONNBZONES;
	config.detectionWindowsX = CONFIG_DETECTIONWINDOWSX;
	config.detectionWindowsY = CONFIG_DETECTIONWINDOWSY;
	config.detectionWindowsWidth = CONFIG_DETECTIONWINDOWSWIDTH;
	config.detectionWindowsHeight = CONFIG_DETECTIONWINDOWSHEIGHT;

	config.detectionMotionLevel = CONFIG_DETECTIONMOTIONLEVEL;
	config.detectionMinimumPixelChangePct = CONFIG_DETECTIONMINIMUMPIXELCHANGEPCT;

	if (ini_parse(iniFilePath, configurationHandler, &config) < 0) {
		syslog(LOG_INFO, "Can't load '%s'\n", iniFilePath);
		printf("Can't load '%s'\n",iniFilePath);
		config.isValid = false;
		return config;
	}

	config.isValid = true;
	return config;
}


// save on syslog file current configuration 
void LogConfiguration(configuration configuration)
{
	GlobalConfiguration = configuration;

	syslog(LOG_INFO, "isValid : %d", GlobalConfiguration.isValid);
	syslog(LOG_INFO, "version : %d", GlobalConfiguration.version);
	syslog(LOG_INFO, "pidFilePath : %s", GlobalConfiguration.pidFilePath);
	
	syslog(LOG_INFO, "saveImage : %s", GlobalConfiguration.saveImages ? "true" : "false" );
	syslog(LOG_INFO, "cameraNumber : %d", GlobalConfiguration.cameraNumber);
	
	syslog(LOG_INFO, "saveStatitics : %s", GlobalConfiguration.saveStatitics ? "true" : "false" );
	syslog(LOG_INFO, "statisticsPeriodSec : %d", GlobalConfiguration.statisticsPeriodSec);
	
	syslog(LOG_INFO, "saveTimeLaps : %s", GlobalConfiguration.saveTimeLaps ? "true" : "false" );
	syslog(LOG_INFO, "timeLapsPeriodSec : %d", GlobalConfiguration.timeLapsPeriodSec);
	syslog(LOG_INFO, "timeLapsStartMinute : %d", GlobalConfiguration.timeLapsStartMinute);
	syslog(LOG_INFO, "timeLapsEndMinute : %d", GlobalConfiguration.timeLapsEndMinute);

	syslog(LOG_INFO, "checkTriggerFile : %s", GlobalConfiguration.checkTriggerFile ? "true" : "false" );
	syslog(LOG_INFO, "checkTriggerFilePeriodSec : %d", GlobalConfiguration.checkTriggerFilePeriodSec);

	syslog(LOG_INFO, "picsDirectory : %s", GlobalConfiguration.picsDirectory == NULL ? "(null)" : GlobalConfiguration.picsDirectory);
	syslog(LOG_INFO, "logsDirectory : %s", GlobalConfiguration.logsDirectory == NULL ? "(null)" : GlobalConfiguration.logsDirectory);
	syslog(LOG_INFO, "takeSnapshotTriggerFile : %s", GlobalConfiguration.takeSnapshotTriggerFile == NULL ? "(null)" : GlobalConfiguration.takeSnapshotTriggerFile);

	syslog(LOG_INFO, "imgExtension : %s", GlobalConfiguration.imgExtension);
	syslog(LOG_INFO, "delaySnapshotMs : %d ms", GlobalConfiguration.delaySnapshotMs);
	syslog(LOG_INFO, "delayMinSaveImgMs : %d ms", GlobalConfiguration.delayMinSaveImgMs);
	syslog(LOG_INFO, "imageTreshold : %d", GlobalConfiguration.imageTreshold);
	syslog(LOG_INFO, "cameraResolutionWidth : %d pixel", GlobalConfiguration.cameraResolutionWidth);
	syslog(LOG_INFO, "cameraResolutionHeight : %d pixel", GlobalConfiguration.cameraResolutionHeight);

	syslog(LOG_INFO, "processResolutionWidth : %d pixel", GlobalConfiguration.processResolutionWidth);
	syslog(LOG_INFO, "processResolutionHeight : %d pixel", GlobalConfiguration.processResolutionHeight);

	syslog(LOG_INFO, "imageDirectoryFormat : %s", GlobalConfiguration.imageDirectoryFormat);
	syslog(LOG_INFO, "imageFileFormat : %s", GlobalConfiguration.imageFileFormat);
	syslog(LOG_INFO, "imageCroppedFileFormat : %s", GlobalConfiguration.imageCroppedFileFormat);
	syslog(LOG_INFO, "imageCroppedDirectoryFormat : %s", GlobalConfiguration.imageCroppedDirectoryFormat);
	syslog(LOG_INFO, "statsFileFormat : %s", GlobalConfiguration.statsFileFormat);
	syslog(LOG_INFO, "timelapsFileFormat : %s", GlobalConfiguration.timelapsFileFormat);
	syslog(LOG_INFO, "triggerFileFormat : %s", GlobalConfiguration.triggerFileFormat);

	syslog(LOG_INFO, "detectionNbZones : %d", GlobalConfiguration.detectionNbZones);
	syslog(LOG_INFO, "detectionWindowsX : %d pixel", GlobalConfiguration.detectionWindowsX);
	syslog(LOG_INFO, "detectionWindowsY : %d pixel", GlobalConfiguration.detectionWindowsY);
	syslog(LOG_INFO, "detectionWindowsWidth : %d pixel", GlobalConfiguration.detectionWindowsWidth);
	syslog(LOG_INFO, "detectionWindowsHeight : %d pixel", GlobalConfiguration.detectionWindowsHeight);
	
	syslog(LOG_INFO, "detectionMotionLevel : %d", GlobalConfiguration.detectionMotionLevel);
	syslog(LOG_INFO, "detectionMinimumPixelChangePct : %d %", GlobalConfiguration.detectionMinimumPixelChangePct);
}


string SaveConfigurationToString(
	configuration configuration)
{
	stringstream ss;
	GlobalConfiguration = configuration;
	char temp[100];
	sprintf(temp,"'cameraNumber'='%d', ", configuration.cameraNumber);
	ss << temp;

	sprintf(temp,"'statisticsPeriodSec'='%d', ", configuration.statisticsPeriodSec);
	ss << temp;
	
	sprintf(temp,"'timeLapsPeriodSec'='%d', ", configuration.timeLapsPeriodSec);
	ss << temp;

	sprintf(temp,"'timeLapsStartMinute'='%d', ", configuration.timeLapsStartMinute);
	ss << temp;
	
	sprintf(temp,"'timeLapsEndMinute'='%d', ", configuration.timeLapsEndMinute);
	ss << temp;

	sprintf(temp,"'delaySnapshotMs'='%d', ", configuration.delaySnapshotMs);
	ss << temp;

	sprintf(temp,"'delayMinSaveImgMs'='%d', ", configuration.delayMinSaveImgMs);
	ss << temp;
	
	sprintf(temp,"'imageTreshold'='%d', ", configuration.imageTreshold);
	ss << temp;

	sprintf(temp,"'cameraResolutionWidth'='%d', ", configuration.cameraResolutionWidth);
	ss << temp;

	sprintf(temp,"'cameraResolutionHeight'='%d', ", configuration.cameraResolutionHeight);
	ss << temp;
	
	sprintf(temp,"'processResolutionWidth'='%d', ", configuration.processResolutionWidth);
	ss << temp;

	sprintf(temp,"'processResolutionHeight'='%d', ", configuration.processResolutionHeight);
	ss << temp;
	
	sprintf(temp,"'detectionNbZones'='%d', ", configuration.detectionNbZones);
	ss << temp;
	
	sprintf(temp,"'detectionWindowsX'='%d', ", configuration.detectionWindowsX);
	ss << temp;
	
	sprintf(temp,"'detectionWindowsY'='%d', ", configuration.detectionWindowsY);
	ss << temp;

	sprintf(temp,"'detectionWindowsWidth'='%d', ", configuration.detectionWindowsWidth);
	ss << temp;

	sprintf(temp,"'detectionWindowsHeight'='%d', ", configuration.detectionWindowsHeight);
	ss << temp;
	
	sprintf(temp,"'detectionMotionLevel'='%d', ", configuration.detectionMotionLevel);
	ss << temp;

	sprintf(temp,"'detectionMinimumPixelChangePct'='%d', ", configuration.detectionMinimumPixelChangePct);
	ss << temp;

	return ss.str();
}

// --------------------------------------------------------