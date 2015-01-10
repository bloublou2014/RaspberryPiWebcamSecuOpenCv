#include "Detection.h"
#include "Utils.h"


void computeZoneSize(
	int x_start, int x_stop, 
	int y_start, int y_stop,
	int max_pixelDiffPerZonePct,
	int nbZones,
	int& zoneSize,
	int& nbZoneX,
	int& nbZoneY,
	int& max_pixelDiffPerZone)
{
	int size = floor( sqrt((float)((x_stop-x_start) * (y_stop-y_start)) / (float)nbZones));
	nbZoneX = floor( (float)(x_stop-x_start) / size);
	nbZoneY = floor( (float)(y_stop-y_start) / size);
	zoneSize = size;

	max_pixelDiffPerZone = floor( zoneSize*zoneSize * max_pixelDiffPerZonePct / (float)100);
}


int detectMotionBySquares2(
	const Mat & motion, Mat & result, Mat & result_cropped,
    int x_start, int y_start, 
	int zoneSize,
	int nbZoneX,
	int nbZoneY,
	int max_pixelDiffPerZone,
    Scalar & color,
	float* countZonesStats,
	bool doStats,
	int nbZones)
{
	int total =0;
	// compute zone size
	
	if (doStats && countZonesStats == NULL)
	{
		countZonesStats = new float[nbZones];
	}

	float* countZones = new float[nbZones];
	int minx, miny, maxx, maxy;
	minx = miny = -1;
	maxx = maxy = -1;

	// compute ratio between process & result image

	for (int i=0; i<nbZoneX; i++)
		for (int j=0; j<nbZoneY; j++)
		{
			int x = x_start + i*zoneSize;
			int y = y_start + j*zoneSize;
			Mat roi = motion(Rect(x==0 ? 1 : x, y==0 ? 1 : y,  zoneSize,  zoneSize));
			int nb = countNonZero(roi);
			countZones[i+j] =  (float)nb / (float)max_pixelDiffPerZone;
			
			if (doStats)
			{
				countZonesStats[i+j] +=  (float)nb / (float)max_pixelDiffPerZone;
			}

			if (countZones[i+j] >= 1)
				total++;

			if (countZones[i+j] >= 0.5)
			{
				if (minx < 0 || minx > x)
					minx = x;
				if (maxx < 0 || maxx < x+zoneSize)
					maxx = x+zoneSize;

				if (miny < 0 || miny > y)
					miny = y;
				if (maxy < 0 || maxy < y+zoneSize)
					maxy = y+zoneSize;
			}

			// draw on result
			/*
			if (countZones[i+j] >= 1)
			{
				rectangle(result, 
							Point(i*a*ratioProcessToResultX, j*a*ratioProcessToResultY), 
							Point((i+1)*a*ratioProcessToResultX, (j+1)*a*ratioProcessToResultY), 
							Scalar(0,255,0), 1);

			} else if (countZones[i+j] >= 0.5)
			{
				rectangle(result, 
							Point(i*a*ratioProcessToResultX, j*a*ratioProcessToResultY), 
							Point((i+1)*a*ratioProcessToResultX, (j+1)*a*ratioProcessToResultY), 
							Scalar(0,0,255), 1);
			}*/

			if (countZones[i+j] >= 1)
				total++;
		}

		if (minx > -1)
		{
			float ratioProcessToResultX = (float) result.cols  /(float) motion.cols;
			float ratioProcessToResultY = (float) result.rows  /(float) motion.rows;

			Point p1(minx*ratioProcessToResultX,miny*ratioProcessToResultY);
            Point p2(maxx*ratioProcessToResultX,maxy*ratioProcessToResultY);
            Rect rect12(p1,p2);

			rectangle(result, 
				p1, 
				p2, 
				color, 2);
			Mat cropped = result(rect12);
            cropped.copyTo(result_cropped);
		}

	return total;
}


int detectMotionBySquares(
	const Mat & motion, Mat & result, Mat & result_cropped,
    int x_start, int x_stop, int y_start, int y_stop,
    int max_pixelDiffPerZonePct,
    Scalar & color,
	int nbZones)
{
	int total =0;
	// compute zone size
	int a = floor( sqrt((float)((x_stop-x_start) * (y_stop-y_start)) / (float)nbZones));

	int nbXZones = floor( (float)(x_stop-x_start) / a);
	int nbYZones = floor( (float)(y_stop-y_start) / a);
	
	int max_pixelDiffPerZone = floor( a*a * max_pixelDiffPerZonePct / (float)100);

	float* countZones = new float[nbZones];
	int minx, miny, maxx, maxy;
	minx = miny = -1;
	maxx = maxy = -1;

	// compute ratio between process & result image
	float ratioProcessToResultX = (float) result.cols  /(float) motion.cols;
	float ratioProcessToResultY = (float) result.rows  /(float) motion.rows;

	for (int i=0; i<nbXZones; i++)
		for (int j=0; j<nbYZones; j++)
		{
			int x = x_start + i*a;
			int y = y_start + j*a;
			Mat roi = motion(Rect(x==0 ? 1 : x, y==0 ? 1 : y,  a,  a));
			int nb = countNonZero(roi);
			countZones[i+j] =  (float)nb / (float)max_pixelDiffPerZone;

			if (countZones[i+j] >= 1)
				total++;

			if (countZones[i+j] >= 0.5)
			{
				if (minx < 0 || minx > x)
					minx = x;
				if (maxx < 0 || maxx < x+a)
					maxx = x+a;

				if (miny < 0 || miny > y)
					miny = y;
				if (maxy < 0 || maxy < y+a)
					maxy = y+a;
			}

			// draw on result
			/*
			if (countZones[i+j] >= 1)
			{
				rectangle(result, 
							Point(i*a*ratioProcessToResultX, j*a*ratioProcessToResultY), 
							Point((i+1)*a*ratioProcessToResultX, (j+1)*a*ratioProcessToResultY), 
							Scalar(0,255,0), 1);

			} else if (countZones[i+j] >= 0.5)
			{
				rectangle(result, 
							Point(i*a*ratioProcessToResultX, j*a*ratioProcessToResultY), 
							Point((i+1)*a*ratioProcessToResultX, (j+1)*a*ratioProcessToResultY), 
							Scalar(0,0,255), 1);
			}*/

			if (countZones[i+j] >= 1)
				total++;
		}

		if (minx > -1)
		{
			Point p1(minx*ratioProcessToResultX,miny*ratioProcessToResultY);
            Point p2(maxx*ratioProcessToResultX,maxy*ratioProcessToResultY);
            Rect rect12(p1,p2);

			rectangle(result, 
				p1, 
				p2, 
				color, 2);
			Mat cropped = result(rect12);
            cropped.copyTo(result_cropped);
		}

	return total;
}


Mat visualizeLastChanges(int *history, int historyLength, int historyIndex, Mat image, int height ) // 20
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
	for (int i=0; i< nbElementToDisplay; i++)
	{
		if (maximum < history[index])
			maximum = history[index];
		index++;
		if (index >= nbElementToDisplay)
			index = 0;
	}

	if (maximum < 1)
		maximum = 1;

	float pixelHeightRatio =  (float)height / (float)maximum;

	// with all history draw a limit
	Mat output(image.rows + height,image.cols,image.type());
	Mat dstRoi = output(Rect(0, 0, image.cols, image.rows));
	image.copyTo(dstRoi);
	// on bottom of image create bloc of elements depending on max
	// pixelHeightRatio
	int bottom = output.rows;
	index = historyIndex;
	for (int i=0; i< nbElementToDisplay; i++)
	{
		int pixelHeightValue = floor( history[index] * pixelHeightRatio );
		// draw a rectangle 
		rectangle(output, Point(i,bottom), Point(i + pixelPerElement,bottom-pixelHeightValue), Scalar( 255, 0, 255 ),10);

		index++;
		if (index >= nbElementToDisplay)
			index = 0;
	}
	return output;
}

bool checkIfBetterLastSecond(int *history, int historyLength, int historyIndex, int lastHistoryIndexSaved, int deltaHistory)
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
	for (int i=0; i < deltaHistory; i++)
	{
		if (history[i] > history[historyIndex])
			return true;

		if (index >= historyLength)
			index = 0;
	}

	return false;
}
