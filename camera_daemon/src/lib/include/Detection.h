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

void computeZoneSize(
	int x_start, int x_stop, int y_start, int y_stop,
	int max_pixelDiffPerZonePct,
	int nbZones,
	int& zoneSize,
	int& nbZoneX,
	int& nbZoneY,
	int& max_pixelDiffPerZone );

int detectMotionBySquares(
	const Mat & motion, Mat & result, Mat & result_cropped,
    int x_start, int x_stop, int y_start, int y_stop,
    int max_pixelDiffPerZonePct,
    Scalar & color,
	int nbZones = 120);

int detectMotionBySquares2(
	const Mat & motion, Mat & result, Mat & result_cropped,
    int x_start, int y_start, 
	int zoneSize,
	int nbZoneX,
	int nbZoneY,
	int max_pixelDiffPerZone,
    Scalar & color,
	float* countZonesStats,
	bool doStats = false,
	int nbZones = 120);


Mat visualizeLastChanges(
	int *history, 
	int historyLength, 
	int historyIndex, 
	Mat image, 
	int height = 20);

bool checkIfBetterLastSecond(
	int *history, 
	int historyLength, 
	int historyIndex, 
	int lastHistoryIndexSaved, 
	int deltaHistory);

#endif