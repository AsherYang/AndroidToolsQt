set logSpaceSuccess=%~dp0\logSpaceSuccess.txt
del %logSpaceSuccess%
cd ./../
ping -n 10 127.0.0.1 >>null
call jcshell.bat -f ./scripts/setkey5.jcsh
del null
echo "-------------get space end-----------">>%logSpaceSuccess%
pause