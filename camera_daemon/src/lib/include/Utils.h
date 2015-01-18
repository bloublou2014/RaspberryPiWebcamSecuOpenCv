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