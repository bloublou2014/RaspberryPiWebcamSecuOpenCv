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

#include "Utils.h"

// Check if the directory exists, if not create it
// This function will create a new directory if the image is the first
// image taken for a specific day
void directoryExistsOrCreate(const char* pzPath)
{
    DIR *pDir;
    // directory doesn't exists -> create it
    if ( pzPath == NULL || (pDir = opendir (pzPath)) == NULL)
	{
		syslog(LOG_DEBUG, "Creating directory %s", pzPath);
        mkdir(pzPath, 0777);
		// if directory exists we opened it and we
		// have to close the directory again.
	}
    else 
		if(pDir != NULL)
	{
        (void) closedir (pDir);
	}
}

bool directoryExists(char* directoryName)
{
	struct stat info;

	if( stat( directoryName, &info ) != 0 )
		return false;

	if( info.st_mode & S_IFDIR )  // S_ISDIR() doesn't exist on my windows 
		return true;

	return false;
}


bool fileExists(const char* filePath)
{
	struct stat info;

	if( stat( filePath, &info ) != 0 )
		return false;

	if (info.st_mode & S_IFDIR )  // S_ISDIR() doesn't exist on my windows 
		return false;

	return true;
}

// When motion is detected we write the image to disk
//    - Check if the directory exists where the image will be stored.
//    - Build the directory and image names.
bool saveImg(
	Mat image,
	struct tm * timeinfo, 
	const string baseDirectory, 
	const string imageExtension, 
	const char * directoryFormat, 
	const char * fileFormat)
{
    stringstream ss;
    
    char TIME[80];

    // Create name for the date directory
    strftime (TIME, 80, directoryFormat, timeinfo);
    
	// reset ss
	ss.str("");
	ss << baseDirectory ;	

	int length = baseDirectory.size();
	if (baseDirectory[length-1] != '/')
	{
		ss << "/";
	}

	ss << TIME;
    directoryExistsOrCreate(ss.str().c_str());

    // Create name for the image
    strftime (TIME, 80, fileFormat, timeinfo);

	ss << "/";
    ss << TIME << imageExtension; 

	string imagePath = ss.str();

	bool result = imwrite(ss.str().c_str(), image);
	if (!result)
	{
		syslog(LOG_ERR, "CANNOT save image to %s", ss.str().c_str());
	}
    return result;
}

// save text to file
void saveToFile(
	struct tm * timeinfo, 
	const string baseDirectory, 
	const char * directoryFormat, 
	const char * fileFormat,
	string textToSave)
{
	stringstream ss;
    
    char TIME[80];

    // Create name for the date directory
    strftime (TIME, 80, directoryFormat, timeinfo);
    
	// reset ss
	ss.str("");
	ss << baseDirectory ;	

	int length = baseDirectory.size();
	if (baseDirectory[length-1] != '/')
	{
		ss << "/";
	}

	ss << TIME;
    directoryExistsOrCreate(ss.str().c_str());

    // Create name for the image
    strftime (TIME, 80, fileFormat, timeinfo);

	ss << "/";
    ss << TIME;
	FILE	*psFile;
	if (fileExists(ss.str().c_str()))
	{
		psFile	= fopen(ss.str().c_str(), "a");
	}
	else {
		psFile	= fopen(ss.str().c_str(), "w");
	}

	fprintf(psFile, textToSave.c_str());
	fclose(psFile);
}

void resizeImage(Mat &src, int width, int height)
{
	if (src.cols == width && src.rows == height)
		return;

	resize(src,src,Size(width,height));
}

time_t getTime()
{
	time_t seconds;
	time (&seconds);
	return seconds;
}

struct tm * getUtcTimeInfo()
{
	time_t seconds;
	struct tm * timeinfo;
	time (&seconds);
	// Get the current time
	timeinfo = gmtime (&seconds);
	return timeinfo;
}

time_t getNextStepSecond(time_t currentSecond, int stepSec, int startDayLimitSec, int endDayLimitSec)
{
	struct tm * nextUtcTimeinfo;

	currentSecond += stepSec;

	if ( (startDayLimitSec == 0 && (endDayLimitSec == 0 || endDayLimitSec == 24*60*60))
		|| (startDayLimitSec == endDayLimitSec))
	{
		// end == start
		return currentSecond;
	}

	nextUtcTimeinfo = gmtime (&currentSecond);

	int startSecondToday = nextUtcTimeinfo->tm_hour * 60 * 60 + nextUtcTimeinfo->tm_min * 60 + nextUtcTimeinfo->tm_sec ;
	
	if (startSecondToday < startDayLimitSec)
	{
		currentSecond -= startSecondToday;
		currentSecond += startDayLimitSec;
		return currentSecond;
	}

	if (startSecondToday > endDayLimitSec)
	{
		currentSecond -= startSecondToday ;
		currentSecond += 24*60*60 + startDayLimitSec;
		return currentSecond;
	}

	// otherwise it's a valid next time
	return currentSecond;
}



