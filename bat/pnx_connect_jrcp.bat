set logfile=%~dp0\log.txt
adb forward tcp:8050 tcp:8050
adb shell pnx --mcc 0x0d>>%logfile%
adb shell pnx --mcc 0x0d>>%logfile%
adb shell pnx --jrcp>>%logfile%
pause
