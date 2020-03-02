set logCplcSuccess=%~dp0\logCplcSuccess.txt
del %logCplcSuccess%
cd ./../
ping -n 10 127.0.0.1 >>null
call jcshell.bat -f ./scripts/setkey0.jcsh
del null
rem start notepad ./scripts/setkey4.jcsh
echo "-------------get cplc end-----------">>%logCplcSuccess%