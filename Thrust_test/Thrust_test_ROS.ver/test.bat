echo off
set "remote_username=mrl@192.168.0.59"
set "remote_directory=~/catkin_ws/src/thrust_test/src/bag"
set "local_directory=C:\bag_ros"




powershell.exe -ExecutionPolicy Bypass -Command "& {del temp.txt}"
echo on
ssh %remote_username% "ls -t %remote_directory%" > temp.txt
echo off
set /p latest_file=<temp.txt
set latest_file=%latest_file:~0,-3%
echo on
echo %latest_file%
del temp.txt
scp %remote_username%:%remote_directory%\25.bag %local_directory%

pause