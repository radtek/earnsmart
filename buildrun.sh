#!/bin/bash -x

cd /usr/local/smartearn
echo "svn up"
svn up .
cd sebgpd
echo "make build"
make build
cd ..
echo "debugsetup.bash"
bash debugsetup.bash
echo "cd /usr.../Gnu.."
cd /usr/local/smartearn/dist/Debug/GNU-Linux-x86
