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

#include "Detection.h"
#include "Utils.h"

Detector::Detector(
	Rect detectionZone,
	int zoneThresholdPercentage,
	int nbZones)
{
	this->detectionZone = detectionZone;
	this->nbZones = nbZones;
	this->zoneThresholdPercentage = zoneThresholdPercentage;
	this->Init();
	this->drawDetectedZoneWidth = 2;
	this->lastFrameTotalActiveZone = 0;
}

void Detector::Init()
{
	syslog(LOG_INFO, "Init detector", nbZones);
	this->detectionArea = detectionZone.height * detectionZone.width;

	int squareSize = floor(sqrt((float)(this->detectionZone.width * this->detectionZone.height) / (float)this->nbZones));

	this->cols = floor((float)(this->detectionZone.width) / squareSize);
	this->rows = floor((float)(this->detectionZone.height) / squareSize);
	this->zoneSize = squareSize;

	this->zoneThresholdPixel = floor(this->zoneSize * this->zoneSize * this->zoneThresholdPercentage / (float)100);

	this->IsStatsEnabled = false;
}

bool Detector::process(
	const Mat & motion,
	Mat & result,
	Mat & result_cropped)
{
	int totalPositiveZones = 0;
	// compute zone size

	float ratioMotionToResultWidth = (float)result.cols / (float)motion.cols;
	float ratioMotionToResultHeight = (float)result.rows / (float)motion.rows;

	if (this->IsStatsEnabled && this->countZonesStats == NULL)
	{
		syslog(LOG_INFO, "Init stats with  %i zones", nbZones);
		this->countZonesStats = new float[nbZones]();
	}

	float* countZones = new float[nbZones]();

	// compute ratio between process & result image
	int x, y;
	int zoneNo;
	Mat roi;
	int nbPositivePixels;

	Point topLeft;
	Point bottomRight;

	topLeft.x = topLeft.y = bottomRight.x = bottomRight.y = -1;

	float currentZoneCount;

	for (int i = 0; i < this->cols; i++)
		for (int j = 0; j < this->rows; j++)
		{
			x = this->detectionZone.x + i*this->zoneSize;
			y = this->detectionZone.y + j*this->zoneSize;

			zoneNo = i + j;

			// Count number of positive pixels on motion image
			roi = motion(Rect(x == 0 ? 1 : x, y == 0 ? 1 : y, this->zoneSize, this->zoneSize));
			nbPositivePixels = countNonZero(roi);
			currentZoneCount = (float)nbPositivePixels / (float)this->zoneThresholdPixel;
			countZones[zoneNo] = currentZoneCount;

			if (this->IsStatsEnabled)
			{
				this->countZonesStats[zoneNo] += currentZoneCount;
			}

			if (countZones[zoneNo] >= 1)
				totalPositiveZones++;

			// over 50% of the threshold set draw zone
			if (IsDrawDetectedZoneEnabled && countZones[zoneNo] >= 0.5)
			{
				if (topLeft.y < 0 || topLeft.y > y)
					topLeft.y = y;

				if (topLeft.x < 0 || topLeft.x > x)
					topLeft.x = x;


				if (bottomRight.x < 0 || bottomRight.x < x + this->zoneSize)
					bottomRight.x = x + this->zoneSize;

				if (bottomRight.y < 0 || bottomRight.y < y + this->zoneSize)
					bottomRight.y = y + this->zoneSize;
			}

			// draw on result for debug
			if (this->IsDebugEnabled){
				/*if (countZones[zoneNo] >= 1)
				{
				rectangle(result,
				Point(i*a*ratioProcessToResultX, j*a*ratioProcessToResultY),
				Point((i + 1)*a*ratioProcessToResultX, (j + 1)*a*ratioProcessToResultY),
				Scalar(0, 255, 0), 1);

				}
				else if (countZones[i + j] >= 0.5)
				{
				rectangle(result,
				Point(i*a*ratioProcessToResultX, j*a*ratioProcessToResultY),
				Point((i + 1)*a*ratioProcessToResultX, (j + 1)*a*ratioProcessToResultY),
				Scalar(0, 0, 255), 1);
				}*/
			}
		}


	if ((IsDrawDetectedZoneEnabled || IsCroppedEnabled) && topLeft.y > -1)
	{

		topLeft.x = topLeft.x * ratioMotionToResultWidth;
		bottomRight.x = bottomRight.x * ratioMotionToResultWidth;

		topLeft.y = topLeft.y * ratioMotionToResultHeight;
		bottomRight.y = bottomRight.y * ratioMotionToResultHeight;

		// draw on result
		if (IsDrawDetectedZoneEnabled){
			rectangle(result, topLeft, bottomRight, this->motionColor, this->drawDetectedZoneWidth);
		}

		if (this->IsCroppedEnabled){
			Rect detectedZone(topLeft, bottomRight);
			Mat cropped = result(detectedZone);
			cropped.copyTo(result_cropped);
		}
	}

	this->lastFrameTotalActiveZone = totalPositiveZones;

	return (this->gridThreshold <= totalPositiveZones);
}


Mat Detector::visualizeLastChanges(
	int *history,
	int historyLength,
	int historyIndex,
	Mat image,
	int height) // 20
{
	// compute number of pixel per history length
	float pixelPerHistoryElement = (float)image.cols / (float)historyLength;
	// if pixelPerHistory < 1 then only display 1 pixel per history element
	int pixelPerElement = floor(pixelPerHistoryElement);
	int nbElementToDisplay = historyLength;
	if (pixelPerHistoryElement < 1)
	{
		pixelPerElement = 1;
		nbElementToDisplay = image.cols;
	}

	int maximum = 0;
	int index = historyIndex;
	for (int i = 0; i < nbElementToDisplay; i++)
	{
		if (maximum < history[index])
			maximum = history[index];
		index++;
		if (index >= nbElementToDisplay)
			index = 0;
	}

	if (maximum < 1)
		maximum = 1;

	float pixelHeightRatio = (float)height / (float)maximum;

	// with all history draw a limit
	Mat output(image.rows + height, image.cols, image.type());
	Mat dstRoi = output(Rect(0, 0, image.cols, image.rows));
	image.copyTo(dstRoi);
	// on bottom of image create bloc of elements depending on max
	// pixelHeightRatio
	int bottom = output.rows;
	index = historyIndex;
	for (int i = 0; i < nbElementToDisplay; i++)
	{
		int pixelHeightValue = floor(history[index] * pixelHeightRatio);
		// draw a rectangle 
		rectangle(output, Point(i, bottom), Point(i + pixelPerElement, bottom - pixelHeightValue), Scalar(255, 0, 255), 10);

		index++;
		if (index >= nbElementToDisplay)
			index = 0;
	}
	return output;
}

bool Detector::checkIfBetterLastSecond(
	int *history,
	int historyLength,
	int historyIndex,
	int lastHistoryIndexSaved,
	int deltaHistory)
{
	if (lastHistoryIndexSaved < 0)
		return true;

	int diff = (historyIndex - lastHistoryIndexSaved);
	if (diff < 0)
	{
		diff = (historyLength - lastHistoryIndexSaved) + historyIndex;
	}

	if (diff > deltaHistory)
		return true;

	int index = historyIndex;
	for (int i = 0; i < deltaHistory; i++)
	{
		if (history[i] > history[historyIndex])
			return true;

		if (index >= historyLength)
			index = 0;
	}

	return false;
}
