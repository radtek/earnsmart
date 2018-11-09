#!/bin/bash -x

#
# Generated - do not edit!
#

# Macros
#TOP=`pwd`
TOP=/usr/local/smartearn
PWD=`pwd`
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Functions
function checkReturnCode
{
    rc=$?
    if [ $rc != 0 ]
    then
        exit $rc
    fi
}
function makeDirectory
# $1 directory path
# $2 permission (optional)
{
    mkdir -p "$1"
    checkReturnCode
    if [ "$2" != "" ]
    then
      chmod $2 "$1"
      checkReturnCode
    fi
}
function copyFileToTmpDir
# $1 from-file path
# $2 to-file path
# $3 permission
{
    cp "$1" "$2"
    checkReturnCode
    if [ "$3" != "" ]
    then
        chmod $3 "$2"
        checkReturnCode
    fi
}

function copyBinary
{
# $1 project folder
# $2 binary file
  cp -f "$1/${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/$2" "${TOP}/${CND_CONF}/"
}

# Setup
#cd "${TOP}"
rm -rf ${TOP}/${CND_CONF}/se*
rm -rf ${TOP}/${CND_CONF}/se*.*
mkdir -p ${TOP}/${CND_CONF}/

# Copy files and create directories and links
#cd "${TOP}"

#copyBinary "seglobal" "libseglobal.a" # need only so libraries.
#copyBinary "sedata" "libsedata.so"
copyBinary "senet" "libsenet.so"
copyBinary "sexbrl" "libsexbrl.so"
copyBinary "setraining" "libsetraining.so"
copyBinary "seann" "libseann.so"
copyBinary "secubist" "libsecubist.so"
copyBinary "seeconomy" "libseeconomy.so"
copyBinary "sesecurities" "libsesecurities.so"
copyBinary "semain" "libsemain.so"
copyBinary "seadmin" "libseadmin.so"
copyBinary "sebgpd" "sebgpd"
copyBinary "sgweb" "sgweb"

# copy blob files to www
service apache2 stop
echo "Stopping supervisor"
supervisorctl stop all

cp -f ${PWD}/sgweb/${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/blob/* "${TOP}/www/blob"
cp -f "${TOP}/${CND_CONF}/sgweb" "${TOP}/www"
cp -f "${TOP}/${CND_CONF}/sebgpd" "${TOP}/bg/bg1"
cp -f "${TOP}/${CND_CONF}/sebgpd" "${TOP}/bg/bg2"
cp -f "${TOP}/${CND_CONF}/sebgpd" "${TOP}/bg/bg3"
cp -f "${TOP}/${CND_CONF}/sebgpd" "${TOP}/bg/bg4"
cp -fR ${TOP}/www/* ${TOP}/w3admin
cp ${TOP}/config/w3adminconfig.json ${TOP}/w3admin/config.json

ldconfig ${TOP}/${CND_CONF}/

service apache2 start
echo "Starting supervisor"
supervisorctl reload all

# Cleanup
cd "${TOP}"

