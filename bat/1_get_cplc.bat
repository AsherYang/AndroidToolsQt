set logfile=%~dp0\log.txt
del %logfile%
cd /d %~dp0
echo ------------get cplc start------------->>%logfile%
call copy_pnx_watch.vbs
start pnx_connect_jrcp.vbs
start do_cplc.vbs