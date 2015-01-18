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

#ifndef __DETECTION_H__
#define __DETECTION_H__
#include <stdio.h>

#include <stdexcept>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

#include <time.h>

#ifdef LINUX
#include <dirent.h>
#endif

#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>

#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/tracking.hpp"
#include "opencv2/video/background_segm.hpp"


using namespace std;
using namespace cv;


/// <summary>
///		Detect 
/// </summary>
class IDetector
{
public:
	virtual bool process(
		const Mat & motion,
		Mat & result,
		Mat & result_cropped) { }

};


class Detector : public IDetector {

public:
	CvRect detectionZone;
	int detectionArea;
	int nbZones;
	int zoneThresholdPercentage;
	int zoneThresholdPixel;
	int gridThreshold;
	float* countZonesStats;
	
	int cols;
	int rows;
	int zoneSize;

	Scalar motionColor;

	bool IsStatsEnabled;
	bool IsDebugEnabled;
	bool IsDrawDetectedZoneEnabled;
	bool IsCroppedEnabled;

	int drawDetectedZoneWidth;

	int lastFrameTotalActiveZone;


	Detector(
		Rect detectionZone,
		int zoneThresholdPercentage,
		int nbZones);

	virtual bool process(
		const Mat & motion,
		Mat & result,
		Mat & result_cropped);

	void Init();

	bool checkIfBetterLastSecond(int *history, int historyLength, int historyIndex, int lastHistoryIndexSaved, int deltaHistory);
	Mat visualizeLastChanges(int *history, int historyLength, int historyIndex, Mat image, int height);
};


//
//Mat visualizeLastChanges(
//	int *history, 
//	int historyLength, 
//	int historyIndex, 
//	Mat image, 
//	int height = 20);
//
//bool checkIfBetterLastSecond(
//	int *history, 
//	int historyLength, 
//	int historyIndex, 
//	int lastHistoryIndexSaved, 
//	int deltaHistory);

#endif