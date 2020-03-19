cd ./../
ping -n 10 127.0.0.1 >>null
call jcshell.bat -f ./scripts/setkey6.jcsh
del null
pause