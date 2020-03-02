set logCheckKeySuccess=%~dp0\logCheckKeySuccess.txt
del %logCheckKeySuccess%
cd ./../
ping -n 10 127.0.0.1 >>null
call jcshell.bat -f ./scripts/setkey3.jcsh
del null
echo "-------------check key end-----------">>%logCheckKeySuccess%