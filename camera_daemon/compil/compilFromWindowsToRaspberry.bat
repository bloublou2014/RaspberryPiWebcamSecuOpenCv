ECHO OFF
REM Compil code on Raspberry Pi from Windows to avoid any copy pasting
SET BATCH_PATH=%~dp0

SET RASPBERRYPI_IP=192.168.1.66
SET RASPBERRYPI_LOGIN=pi
SET RASPBERRYPI_PASS=pop

SET LOCAL_SRCPATH=%BATCH_PATH%\..\src
SET RASPBERRYPI_SRCPATH=/home/pi/camera_daemon/src

SET WINSCP=%BATCH_PATH%\..\..\tools\winscp\WinSCP.com
SET PLINK=%BATCH_PATH%\..\..\tools\putty\plink.exe


ECHO === Copy Source to Raspberry Pi ===

%WINSCP% /command ^
    "option batch abort" ^
    "open sftp://%RASPBERRYPI_LOGIN%:%RASPBERRYPI_PASS%@%RASPBERRYPI_IP%/" ^
    "put %LOCAL_SRCPATH% %RASPBERRYPI_SRCPATH%" ^
    "exit"

IF %ERRORLEVEL% EQU 0 GOTO COMPIL

ECHO "ERROR: Cannot copy file to RaspberryPi."
EXIT 1
GOTO :END

	
:COMPIL
ECHO .
ECHO === CMAKE on Raspberry Pi === 

%PLINK% -ssh -batch %RASPBERRYPI_LOGIN%@%RASPBERRYPI_IP% -pw "%RASPBERRYPI_PASS%"  "cmake %RASPBERRYPI_SRCPATH%"

IF %ERRORLEVEL% EQU 0 GOTO CLEAN

ECHO "ERROR: Cannot camke on RaspberryPi. Review error"
PAUSE
EXIT 1
GOTO :END

:CLEAN
ECHO === Clean on Raspberry Pi === 

%PLINK% -ssh -batch %RASPBERRYPI_LOGIN%@%RASPBERRYPI_IP% -pw "%RASPBERRYPI_PASS%"  "make -C %RASPBERRYPI_SRCPATH% clean"

IF %ERRORLEVEL% EQU 0 GOTO BUILD

ECHO "ERROR: Cannot clean on RaspberryPi. Review error"
PAUSE
EXIT 1
GOTO :END

:BUILD
ECHO === Build on Raspberry Pi === 

%PLINK% -ssh -batch %RASPBERRYPI_LOGIN%@%RASPBERRYPI_IP% -pw "%RASPBERRYPI_PASS%"  "make -C %RASPBERRYPI_SRCPATH%"

IF %ERRORLEVEL% EQU 0 GOTO END

ECHO "ERROR: Cannot build on RaspberryPi. Review error"
PAUSE
EXIT 1

GOTO :END




:END
REM PAUSE