cd /d %~dp0
call copy_pnx_watch.vbs
start pnx_connect_jrcp.vbs
start do_check_key.vbs