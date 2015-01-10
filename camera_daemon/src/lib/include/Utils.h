
#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>

#include <stdexcept>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

#include <time.h>
#include <dirent.h>
#include <stdlib.h>

#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>

#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/tracking.hpp"
#include "opencv2/video/background_segm.hpp"

#include "ConfigDaemon.h"

using namespace std;
using namespace cv;

#define MAX_PATH          260

void directoryExistsOrCreate(const char* pzPath);
bool directoryExists(char* directoryName);
bool fileExists(const char* filePath);

bool saveImg(Mat image,struct tm * timeinfo, const string DIRECTORY, const string EXTENSION, const char * DIR_FORMAT, const char * FILE_FORMAT);
void resizeImage(Mat &src, int width, int height);

void saveToFile(
	struct tm * timeinfo, 
	const string baseDirectory, 
	const char * directoryFormat, 
	const char * fileFormat,
	string textToSave);

time_t getTime();
struct tm * getUtcTimeInfo();

time_t getNextStepSecond(time_t currentSecond, int stepSec,	int startDayLimitSec, int endDayLimitSec);

#endif