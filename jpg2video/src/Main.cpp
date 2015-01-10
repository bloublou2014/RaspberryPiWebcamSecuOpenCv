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

#include <functional>
#include <cstdlib>

#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/tracking.hpp"
#include "opencv2/video/background_segm.hpp"

using namespace std;
using namespace cv;

static void help()
{
    cout
        << "------------------------------------------------------------------------------" << endl
		<< "JPG to Video"																	<< endl
        << "This program add jpg file to video file."										<< endl
        << "Usage:"                                                                         << endl
        << "./jpg2video [input jpg FileMask] [output video filename]"						<< endl
        << "------------------------------------------------------------------------------" << endl
        << endl;
}

bool fileExists(const char* filePath)
{
	struct stat info;

	if( stat( filePath, &info ) != 0 )
		return false;

	if (info.st_mode & S_IFDIR )
		return false;
	return true;
}

bool directoryExists(const char* directoryName)
{
	struct stat info;

	if( stat( directoryName, &info ) != 0 )
		return false;

	if( info.st_mode & S_IFDIR )
		return true;

	return false;
}

bool startWith(string filename, string startWith)
{
	return (filename.find(startWith) == 0);
}

string toLowerString (string input)
{
	string output;
	for (int i=0; i<input.size(); i++)
		output += tolower( input[i] );
	return output;
}

int compare_strings(const void* a, const void* b)   // comparison function
{
    string arg1 = *reinterpret_cast<const string*>(a);
    string arg2 = *reinterpret_cast<const string*>(b);
    if(arg1 < arg2) return -1;
    if(arg1 > arg2) return 1;
    return 0;
}

int main(int argc, char *argv[])
{
	help();

    if (argc != 3)
    {
        cout << "Not enough parameters" << endl;
        return -1;
    }
	const string source			= argv[1];
	const string destination    = argv[2];

	string sourceDirectory = source;
	string sourceFilename;
	string::size_type pAt = sourceDirectory.find_last_of('/');
	if (pAt < source.size()-1 && pAt != string::npos) 
	{
		if (!directoryExists(sourceDirectory.c_str()))
		{
			// it's not a directory
			sourceFilename = sourceDirectory.substr(pAt+1); 
			sourceDirectory = sourceDirectory.substr(0, pAt); 
		}
	}

	if (sourceDirectory.substr(sourceDirectory.size() - 1)[0] != '/')
	{
		sourceDirectory += "/";
	}

	DIR *pDir;
	Mat sourceImage ;
	Size S ;
	string filename;
	VideoWriter outputVideo;    
	struct stat info;
	
	string* files = new string[10240];
	int currentFile = 0;

	int totalSizeImages = 0;
    // directory doesn't exists -> create it
	if ((pDir = opendir (sourceDirectory.c_str())) != NULL)
	{
		dirent* file;
		
		while ((file = readdir(pDir))!= NULL )
		{
			if (file == NULL)
				break;

			string filename = file->d_name;
			string::size_type pAt = filename.find_last_of('.');
			if (pAt == std::string::npos) 
				continue;

			string fullpath = sourceDirectory + filename;

			if( stat( fullpath.c_str(), &info ) != 0 )
				continue;

			string extension = filename.substr(pAt); 

			if (toLowerString (extension) == ".jpg" && (sourceFilename == "" || startWith(filename,sourceFilename)))
			{
				cout << "adding file " << filename << " size : " <<  info.st_size << " bytes" << endl;
				totalSizeImages += info.st_size;
				files[currentFile] = fullpath;
				currentFile++;
			}
		}
        (void) closedir (pDir);
	}

	 std::qsort(files, currentFile, sizeof(string), compare_strings);

	for (int i=0; i<currentFile; i++)
	{
		string fullpath = files[i];
		sourceImage = imread(fullpath);
		cout << "processing " << fullpath << endl;
		if (!outputVideo.isOpened())
		{
			if (sourceImage.empty())
			{
				cout  << "Could not open input image : " << fullpath << endl;
				return -1;
			}
			S = sourceImage.size();

			outputVideo.open(destination, CV_FOURCC('M','J','P','G') , 1, S, true);

			if (!outputVideo.isOpened())
			{
				cout  << "Could not open the output video for write: " << source << endl;
				return -1;
			}
		}
		outputVideo << sourceImage;
	}

    cout << "Finished writing" << endl;
	cout << "Total file size added : " << totalSizeImages << " bytes" << endl;
	
	if( stat( destination.c_str(), &info ) != 0 )
		return 0;

	cout << "Total video output file size : " << info.st_size << " bytes" << endl;

	float ratio = (float)info.st_size * 100 / (float)totalSizeImages;
	cout << "Ratio in/out, video is  " << ratio << " % of total source files size" << endl;

    return 0;
}