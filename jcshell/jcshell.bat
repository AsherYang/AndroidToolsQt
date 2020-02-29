@echo off
REM ==========================================================================
REM     jcshell.bat
REM     This file launches the NXP JCShell for command line usage
REM ==========================================================================

REM --------------------------------------------------------------------------
REM Check that JAVA_HOME is properly set
REM --------------------------------------------------------------------------
if not "%JAVA_HOME%" == "" goto prepare_environment
	echo Missing JAVA_HOME environment variable.
	goto end

REM --------------------------------------------------------------------------
REM Modifications of the environment variables are valid in the scope of this
REM file only. The scope begins on next line
REM --------------------------------------------------------------------------
:prepare_environment
setlocal

REM --------------------------------------------------------------------------
REM Include the native DLL folder into the PATH environment
REM --------------------------------------------------------------------------
set JCSHELL_BAT_PATH=%~dp0
set TCP_INTERFACE_PATH=tcpserver
set DLL_SUBDIR=os\win32\x86
set PATH=%JCSHELL_BAT_PATH%\%DLL_SUBDIR%;%PATH%
set PATH=%PATH%;%JCSHELL_BAT_PATH%\%DLL_SUBDIR%\32bit
set PATH=%PATH%;%JCSHELL_BAT_PATH%\

REM --------------------------------------------------------------------------
REM Include the JAR files from lib folder in CLASSPATH for JVM
REM --------------------------------------------------------------------------
set CLASSPATH_JCSHELL=%JCSHELL_BAT_PATH%\lib\offcard.jar
set CLASSPATH_JCSHELL=%CLASSPATH_JCSHELL%;%JCSHELL_BAT_PATH%\lib\JNISCCommWrapper.jar
set CLASSPATH_JCSHELL=%CLASSPATH_JCSHELL%;%JCSHELL_BAT_PATH%\lib\JNASCRWrapper.jar
set CLASSPATH_JCSHELL=%CLASSPATH_JCSHELL%;%JCSHELL_BAT_PATH%\lib\JNAMP65Wrapper.jar
set CLASSPATH_JCSHELL=%CLASSPATH_JCSHELL%;%JCSHELL_BAT_PATH%\lib\JNAMPReaderWrapper.jar
set CLASSPATH_JCSHELL=%CLASSPATH_JCSHELL%;%JCSHELL_BAT_PATH%\lib\jna.jar
set CLASSPATH_JCSHELL=%CLASSPATH_JCSHELL%;%JCSHELL_BAT_PATH%\lib\jline.jar
set CLASSPATH_JCSHELL=%CLASSPATH_JCSHELL%;%JCSHELL_BAT_PATH%\lib\nxp-cli-lib.jar
set CLASSPATH_JCSHELL=%CLASSPATH_JCSHELL%;%JCSHELL_BAT_PATH%\lib\bcprov-jdk15on.jar
set CLASSPATH_JCSHELL=%CLASSPATH_JCSHELL%;%JCSHELL_BAT_PATH%\lib\bcpkix-jdk15on.jar
set CLASSPATH_JCSHELL=%CLASSPATH_JCSHELL%;%JCSHELL_BAT_PATH%\lib\nxp-crypto-lib.jar
set CLASSPATH_JCSHELL=%CLASSPATH_JCSHELL%;%JCSHELL_BAT_PATH%\lib\bcprov-ext-jdk15on.jar
set CLASSPATH_JCSHELL=%CLASSPATH_JCSHELL%;%JCSHELL_BAT_PATH%\lib\groovy-all.jar
set CLASSPATH_JCSHELL=%CLASSPATH_JCSHELL%;%JCSHELL_BAT_PATH%\lib\nxp-jrcp-lib.jar
set CLASSPATH_JCSHELL=%CLASSPATH_JCSHELL%;%JCSHELL_BAT_PATH%\lib\commons-io.jar

if not "%NUT_HOME%" == "" set CLASSPATH_JCSHELL=%CLASSPATH_JCSHELL%;%NUT_HOME%\nut.jar

set CLASSPATH_JCSHELL=%CLASSPATH_JCSHELL%;

REM --------------------------------------------------------------------------
REM To handle -noPause argument. This will not pause the jcshell.bat
REM if not set then jcshell.bat will pause.
REM --------------------------------------------------------------------------
set NOPAUSE_SET=""
if "%1" == "-noPause" goto nopause_tag

set NOPAUSE_SET=PAUSE
goto continue

REM --------------------------------------------------------------------------
REM -noPause argument is passed. handle no pause.
REM --------------------------------------------------------------------------
:nopause_tag
set NOPAUSE_SET=NOPAUSE
if "%2" == "" goto nomoreparam_tag
>tmp ECHO(%*
SET /P t=<tmp
SETLOCAL EnableDelayedExpansion
IF DEFINED t SET "t=!t:%1 =!"
goto continue

:continue


REM --------------------------------------------------------------------------
REM Launch the JVM with jcshell as main application and pass in the command
REM line from the call to this batch file.
REM The java library path is set to find the wrapper DLls while the PATH is
REM is extended to find the right native DLLs as childs of the wrapped DLL.
REM --------------------------------------------------------------------------

@echo.
@echo ------------------------------------------------------------------------------
@echo Welcome to NXP JCShell
@echo (c) 2016 NXP Semiconductors
@echo ------------------------------------------------------------------------------
@echo.
if %NOPAUSE_SET% == NOPAUSE goto executepause

"%JAVA_HOME%"\bin\java -Djava.library.path="%JCSHELL_BAT_PATH%\%DLL_SUBDIR%" -cp "%CLASSPATH_JCSHELL%" com.nxp.id.jc.tools.JCShell %*
goto afterexecute

:executepause
"%JAVA_HOME%"\bin\java -Djava.library.path="%JCSHELL_BAT_PATH%\%DLL_SUBDIR%" -cp "%CLASSPATH_JCSHELL%" com.nxp.id.jc.tools.JCShell !t!
goto afterexecute

:nomoreparam_tag
"%JAVA_HOME%"\bin\java -Djava.library.path="%JCSHELL_BAT_PATH%\%DLL_SUBDIR%" -cp "%CLASSPATH_JCSHELL%" com.nxp.id.jc.tools.JCShell
goto afterexecute

:afterexecute

REM --------------------------------------------------------------------------
REM if -noPause argument is passed, bypass pause.
REM --------------------------------------------------------------------------
if %NOPAUSE_SET% == NOPAUSE goto endpause

REM --------------------------------------------------------------------------
REM Scope of environment variable changes/extensions ends here
REM --------------------------------------------------------------------------
:end
endlocal
@EXIT /B %ERRORLEVEL%
pause

:endpause
@EXIT /B %ERRORLEVEL%