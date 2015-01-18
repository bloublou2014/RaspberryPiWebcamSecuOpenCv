## Using Visual Studio as editor ##
You can use visual studio as IDE for your developments,
and compile on your Raspberry Pi without leaving your IDE.

Warning : this will not give you headers definition on Visual Studio,
you will still see warning & error editing but at least no needs to 
switch to winscp & putty manually

To do so :
1. create a project with "Empty project" template
2. add your source files & headers
3. in General configuration properties set project as "Utility"
4. on Pre-Build Event set custom command :
	$(SolutionDir)..\..\compil\compilFromWindowsToRaspberry.bat 

5. on VC++ Directories, set Include directories :	
	$(SolutionDir)..\headers
	$(SolutionDir)..\..\src\lib\include
	
	
5. Open output window on Visual Studio
6. Compile and check results


Here a result sample :

1>------ Build started: Project: CameraDaemonVsProject, Configuration: Debug Win32 ------
1>  === Copy Source to Raspberry Pi ===
1>  batch           abort     
1>  Searching for host...
1>  Connecting to host...
1>  Authenticating...
1>  Using username "pi".
1>  Authenticating with pre-entered password.
1>  Authenticated.
1>  Starting the session...
1>  Reading remote directory...
1>  Session started.
1>  Active session: [1] pi@192.168.1.66
1>  F:\GitHub\RaspberryPiWebcamSecuOpenCv\RaspberryPiWebcamSecuOpenCv\camera_daemon\compil\\..\src |            0 B |    0,0 KiB/s | binary |   0%
1>  F:\GitHub\RaspberryPiWebcamSecuOpenCv\RaspberryPiWebcamSecuOpenCv\camera_daemon\compil\\..\src\CMakeLists.txt |          140 B |    0,0 KiB/s | binary | 100%
1>  F:\GitHub\RaspberryPiWebcamSecuOpenCv\RaspberryPiWebcamSecuOpenCv\camera_daemon\compil\\..\src\code |            0 B |    0,0 KiB/s | binary |   0%
1>  F:\GitHub\RaspberryPiWebcamSecuOpenCv\RaspberryPiWebcamSecuOpenCv\camera_daemon\compil\\..\src\code\CMakeLists.txt |          423 B |   25,8 KiB/s | binary | 100%
1>  F:\GitHub\RaspberryPiWebcamSecuOpenCv\RaspberryPiWebcamSecuOpenCv\camera_daemon\compil\\..\src\code\Main.cpp |         17 KiB | 1132,4 KiB/s | binary | 100%
1>  F:\GitHub\RaspberryPiWebcamSecuOpenCv\RaspberryPiWebcamSecuOpenCv\camera_daemon\compil\\..\src\lib |            0 B |  584,5 KiB/s | binary |   0%
1>  F:\GitHub\RaspberryPiWebcamSecuOpenCv\RaspberryPiWebcamSecuOpenCv\camera_daemon\compil\\..\src\lib\CMakeLists.txt |          161 B |  589,5 KiB/s | binary | 100%
1>  F:\GitHub\RaspberryPiWebcamSecuOpenCv\RaspberryPiWebcamSecuOpenCv\camera_daemon\compil\\..\src\lib\ConfigDaemon.cpp |         13 KiB |  671,8 KiB/s | binary | 100%
1>  F:\GitHub\RaspberryPiWebcamSecuOpenCv\RaspberryPiWebcamSecuOpenCv\camera_daemon\compil\\..\src\lib\Daemonize.cpp |          3 KiB |  565,5 KiB/s | binary | 100%
1>  F:\GitHub\RaspberryPiWebcamSecuOpenCv\RaspberryPiWebcamSecuOpenCv\camera_daemon\compil\\..\src\lib\Detection.cpp |          8 KiB |  699,6 KiB/s | binary | 100%
1>  F:\GitHub\RaspberryPiWebcamSecuOpenCv\RaspberryPiWebcamSecuOpenCv\camera_daemon\compil\\..\src\lib\include |            0 B |  556,1 KiB/s | binary |   0%
1>  F:\GitHub\RaspberryPiWebcamSecuOpenCv\RaspberryPiWebcamSecuOpenCv\camera_daemon\compil\\..\src\lib\include\ConfigDaemon.h |          4 KiB |  618,7 KiB/s | binary | 100%
1>  F:\GitHub\RaspberryPiWebcamSecuOpenCv\RaspberryPiWebcamSecuOpenCv\camera_daemon\compil\\..\src\lib\include\Daemonize.h |          1 KiB |  532,5 KiB/s | binary | 100%
1>  F:\GitHub\RaspberryPiWebcamSecuOpenCv\RaspberryPiWebcamSecuOpenCv\camera_daemon\compil\\..\src\lib\include\Detection.h |          2 KiB |  560,5 KiB/s | binary | 100%
1>  F:\GitHub\RaspberryPiWebcamSecuOpenCv\RaspberryPiWebcamSecuOpenCv\camera_daemon\compil\\..\src\lib\include\Ini.h |          2 KiB |  506,1 KiB/s | binary | 100%
1>  F:\GitHub\RaspberryPiWebcamSecuOpenCv\RaspberryPiWebcamSecuOpenCv\camera_daemon\compil\\..\src\lib\include\Utils.h |          2 KiB |  527,8 KiB/s | binary | 100%
1>  F:\GitHub\RaspberryPiWebcamSecuOpenCv\RaspberryPiWebcamSecuOpenCv\camera_daemon\compil\\..\src\lib\Ini.cpp |          4 KiB |  499,9 KiB/s | binary | 100%
1>  F:\GitHub\RaspberryPiWebcamSecuOpenCv\RaspberryPiWebcamSecuOpenCv\camera_daemon\compil\\..\src\lib\Utils.cpp |          5 KiB |  480,8 KiB/s | binary | 100%.
1>  === CMAKE on Raspberry Pi === 
1>  -- Configuring done
1>  -- Generating done
1>  -- Build files have been written to: /home/pi/camera_daemon/src
1>  === Clean on Raspberry Pi === 
1>  make: Entering directory '/home/pi/camera_daemon/src'
1>  make[1]: Entering directory '/home/pi/camera_daemon/src'
1>  make[2]: Entering directory '/home/pi/camera_daemon/src'
1>  make[2]: Leaving directory '/home/pi/camera_daemon/src'
1>  make[2]: Entering directory '/home/pi/camera_daemon/src'
1>  make[2]: Leaving directory '/home/pi/camera_daemon/src'
1>  make[1]: Leaving directory '/home/pi/camera_daemon/src'
1>  make: Leaving directory '/home/pi/camera_daemon/src'
1>  === Build on Raspberry Pi === 
1>  make: Entering directory '/home/pi/camera_daemon/src'
1>  make[1]: Entering directory '/home/pi/camera_daemon/src'
1>  make[2]: Entering directory '/home/pi/camera_daemon/src'
1>  make[2]: Leaving directory '/home/pi/camera_daemon/src'
1>  make[2]: Entering directory '/home/pi/camera_daemon/src'
1>  [ 16%] Building CXX object lib/CMakeFiles/camera_daemon_lib.dir/Ini.cpp.o
1>  [ 33%] Building CXX object lib/CMakeFiles/camera_daemon_lib.dir/Utils.cpp.o
1>  [ 50%] Building CXX object lib/CMakeFiles/camera_daemon_lib.dir/Detection.cpp.o
1>  [ 66%] Building CXX object lib/CMakeFiles/camera_daemon_lib.dir/Daemonize.cpp.o
1>  [ 83%] Building CXX object lib/CMakeFiles/camera_daemon_lib.dir/ConfigDaemon.cpp.o
1>  /home/pi/camera_daemon/src/lib/ConfigDaemon.cpp: In function ÔÇÿconfiguration LoadIniFile(char*)ÔÇÖ:
1>  /home/pi/camera_daemon/src/lib/ConfigDaemon.cpp:144:23: warning: deprecated conversion from string constant to ÔÇÿchar*ÔÇÖ [-Wwrite-strings]
1>  /home/pi/camera_daemon/src/lib/ConfigDaemon.cpp:161:25: warning: deprecated conversion from string constant to ÔÇÿchar*ÔÇÖ [-Wwrite-strings]
1>  /home/pi/camera_daemon/src/lib/ConfigDaemon.cpp:162:25: warning: deprecated conversion from string constant to ÔÇÿchar*ÔÇÖ [-Wwrite-strings]
1>  /home/pi/camera_daemon/src/lib/ConfigDaemon.cpp:163:35: warning: deprecated conversion from string constant to ÔÇÿchar*ÔÇÖ [-Wwrite-strings]
1>  /home/pi/camera_daemon/src/lib/ConfigDaemon.cpp:165:24: warning: deprecated conversion from string constant to ÔÇÿchar*ÔÇÖ [-Wwrite-strings]
1>  /home/pi/camera_daemon/src/lib/ConfigDaemon.cpp:177:32: warning: deprecated conversion from string constant to ÔÇÿchar*ÔÇÖ [-Wwrite-strings]
1>  /home/pi/camera_daemon/src/lib/ConfigDaemon.cpp:178:27: warning: deprecated conversion from string constant to ÔÇÿchar*ÔÇÖ [-Wwrite-strings]
1>  /home/pi/camera_daemon/src/lib/ConfigDaemon.cpp:179:34: warning: deprecated conversion from string constant to ÔÇÿchar*ÔÇÖ [-Wwrite-strings]
1>  /home/pi/camera_daemon/src/lib/ConfigDaemon.cpp:180:39: warning: deprecated conversion from string constant to ÔÇÿchar*ÔÇÖ [-Wwrite-strings]
1>  /home/pi/camera_daemon/src/lib/ConfigDaemon.cpp:181:27: warning: deprecated conversion from string constant to ÔÇÿchar*ÔÇÖ [-Wwrite-strings]
1>  /home/pi/camera_daemon/src/lib/ConfigDaemon.cpp:182:30: warning: deprecated conversion from string constant to ÔÇÿchar*ÔÇÖ [-Wwrite-strings]
1>  Linking CXX static library libcamera_daemon_lib.a
1>  make[2]: Leaving directory '/home/pi/camera_daemon/src'
1>  [ 83%] Built target camera_daemon_lib
1>  make[2]: Entering directory '/home/pi/camera_daemon/src'
1>  make[2]: Leaving directory '/home/pi/camera_daemon/src'
1>  make[2]: Entering directory '/home/pi/camera_daemon/src'
1>  [100%] Building CXX object code/CMakeFiles/camera_daemon.dir/Main.cpp.o
1>  Linking CXX executable /home/pi/camera_daemon/bin/camera_daemon
1>  make[2]: Leaving directory '/home/pi/camera_daemon/src'
1>  [100%] Built target camera_daemon
1>  make[1]: Leaving directory '/home/pi/camera_daemon/src'
1>  make: Leaving directory '/home/pi/camera_daemon/src'
1>  Press any key to continue . . . 
========== Build: 1 succeeded, 0 failed, 0 up-to-date, 0 skipped ==========



	
