set logAuthKeySuccess=%~dp0\logAuthKeySuccess.txt
del %logAuthKeySuccess%
cd ./../
ping -n 10 127.0.0.1 >>null
call jcshell.bat -f ./scripts/setkey4.jcsh
rem call jcshell.bat -f ./scripts/setkey2.jcsh
del null
echo "-------------auth key end-----------">>%logAuthKeySuccess%
pause
rem exit