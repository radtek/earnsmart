#!/bin/bash -x

#
# Generated - do not edit!
#

# Macros
TOP=`pwd`
CND_PLATFORM=GNU-Linux
CND_CONF=Debug_mysql
CND_DISTDIR=dist
CND_BUILDDIR=build
CND_DLIB_EXT=so
NBTMPDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tmp-packaging
TMPDIRNAME=tmp-packaging
OUTPUT_PATH=${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd
OUTPUT_BASENAME=sebgpd
PACKAGE_TOP_DIR=sebgpd/

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

# Setup
cd "${TOP}"
mkdir -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/package
rm -rf ${NBTMPDIR}
mkdir -p ${NBTMPDIR}

# Copy files and create directories and links
cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemain.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsemain.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseadmin.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseadmin.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsexbrl.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsexbrl.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeoddata.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseeoddata.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsesecurities.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsesecurities.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeconomy.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseeconomy.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsetraining.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsetraining.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseann.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseann.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsecubist.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsecubist.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsenet.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsenet.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemysql.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsemysql.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsedata.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsedata.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemath.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsemath.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseglobal.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseglobal.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmariadbpp.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libmariadbpp.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${OUTPUT_PATH}" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/${OUTPUT_BASENAME}" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemain.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsemain.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseadmin.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseadmin.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsexbrl.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsexbrl.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeoddata.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseeoddata.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsesecurities.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsesecurities.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeconomy.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseeconomy.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsetraining.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsetraining.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseann.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseann.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsecubist.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsecubist.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsenet.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsenet.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemysql.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsemysql.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsedata.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsedata.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemath.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsemath.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseglobal.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseglobal.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmariadbpp.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libmariadbpp.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${OUTPUT_PATH}" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/${OUTPUT_BASENAME}" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemain.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsemain.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseadmin.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseadmin.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsexbrl.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsexbrl.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeoddata.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseeoddata.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsesecurities.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsesecurities.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeconomy.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseeconomy.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsetraining.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsetraining.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseann.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseann.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsecubist.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsecubist.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsenet.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsenet.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemysql.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsemysql.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsedata.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsedata.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemath.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsemath.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseglobal.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseglobal.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmariadbpp.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libmariadbpp.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${OUTPUT_PATH}" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/${OUTPUT_BASENAME}" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemain.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsemain.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseadmin.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseadmin.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsexbrl.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsexbrl.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeoddata.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseeoddata.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsesecurities.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsesecurities.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeconomy.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseeconomy.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsetraining.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsetraining.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseann.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseann.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsecubist.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsecubist.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsenet.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsenet.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemysql.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsemysql.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsedata.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsedata.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemath.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsemath.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseglobal.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseglobal.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmariadbpp.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libmariadbpp.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${OUTPUT_PATH}" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/${OUTPUT_BASENAME}" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemain.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsemain.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseadmin.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseadmin.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsexbrl.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsexbrl.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeoddata.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseeoddata.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsesecurities.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsesecurities.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeconomy.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseeconomy.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsetraining.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsetraining.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseann.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseann.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsecubist.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsecubist.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsenet.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsenet.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemysql.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsemysql.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsedata.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsedata.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemath.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsemath.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseglobal.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseglobal.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmariadbpp.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libmariadbpp.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${OUTPUT_PATH}" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/${OUTPUT_BASENAME}" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemain.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsemain.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseadmin.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseadmin.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsexbrl.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsexbrl.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeoddata.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseeoddata.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsesecurities.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsesecurities.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeconomy.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseeconomy.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsetraining.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsetraining.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseann.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseann.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsecubist.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsecubist.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsenet.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsenet.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemysql.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsemysql.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsedata.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsedata.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemath.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsemath.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseglobal.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseglobal.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmariadbpp.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libmariadbpp.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${OUTPUT_PATH}" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/${OUTPUT_BASENAME}" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseglobal.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseglobal.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsesecurities.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsesecurities.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsetraining.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsetraining.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsedata.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsedata.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemysql.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsemysql.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseann.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseann.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsecubist.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsecubist.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemath.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsemath.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmariadbpp.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libmariadbpp.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemain.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsemain.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeconomy.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseeconomy.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeoddata.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseeoddata.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseadmin.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseadmin.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsexbrl.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsexbrl.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsenet.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsenet.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${OUTPUT_PATH}" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/${OUTPUT_BASENAME}" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemain.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsemain.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseadmin.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseadmin.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsexbrl.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsexbrl.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeoddata.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseeoddata.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsesecurities.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsesecurities.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeconomy.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseeconomy.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsetraining.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsetraining.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseann.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseann.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsecubist.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsecubist.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsenet.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsenet.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemysql.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsemysql.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsedata.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsedata.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemath.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsemath.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseglobal.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseglobal.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmariadbpp.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libmariadbpp.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${OUTPUT_PATH}" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/${OUTPUT_BASENAME}" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeconomy.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseeconomy.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsetraining.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsetraining.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsecubist.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsecubist.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsesecurities.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsesecurities.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemysql.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsemysql.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseann.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseann.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemain.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsemain.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeoddata.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseeoddata.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseglobal.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseglobal.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseadmin.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseadmin.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmariadbpp.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libmariadbpp.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsenet.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsenet.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsedata.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsedata.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsexbrl.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsexbrl.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemath.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsemath.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${OUTPUT_PATH}" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/${OUTPUT_BASENAME}" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeconomy.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseeconomy.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsetraining.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsetraining.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsecubist.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsecubist.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsesecurities.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsesecurities.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemysql.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsemysql.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseann.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseann.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemain.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsemain.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeoddata.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseeoddata.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseglobal.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseglobal.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseadmin.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseadmin.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmariadbpp.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libmariadbpp.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsenet.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsenet.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsedata.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsedata.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsexbrl.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsexbrl.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemath.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsemath.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${OUTPUT_PATH}" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/${OUTPUT_BASENAME}" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemain.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsemain.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseadmin.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseadmin.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsexbrl.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsexbrl.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeoddata.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseeoddata.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsesecurities.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsesecurities.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeconomy.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseeconomy.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsetraining.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsetraining.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseann.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseann.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsecubist.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsecubist.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsenet.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsenet.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemysql.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsemysql.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsedata.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsedata.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemath.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsemath.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseglobal.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseglobal.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmariadbpp.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libmariadbpp.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${OUTPUT_PATH}" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/${OUTPUT_BASENAME}" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemain.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsemain.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseadmin.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseadmin.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsexbrl.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsexbrl.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeoddata.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseeoddata.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsesecurities.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsesecurities.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeconomy.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseeconomy.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsetraining.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsetraining.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseann.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseann.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsecubist.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsecubist.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsenet.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsenet.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemysql.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsemysql.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsedata.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsedata.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemath.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsemath.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseglobal.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseglobal.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmariadbpp.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libmariadbpp.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${OUTPUT_PATH}" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/${OUTPUT_BASENAME}" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemain.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsemain.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseadmin.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseadmin.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsexbrl.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsexbrl.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeoddata.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseeoddata.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsesecurities.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsesecurities.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeconomy.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseeconomy.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsetraining.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsetraining.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseann.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseann.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsecubist.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsecubist.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsenet.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsenet.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemysql.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsemysql.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsedata.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsedata.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemath.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsemath.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseglobal.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseglobal.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmariadbpp.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libmariadbpp.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${OUTPUT_PATH}" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/${OUTPUT_BASENAME}" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemain.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsemain.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseadmin.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseadmin.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsexbrl.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsexbrl.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeoddata.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseeoddata.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsesecurities.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsesecurities.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeconomy.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseeconomy.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsetraining.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsetraining.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseann.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseann.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsecubist.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsecubist.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsenet.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsenet.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemysql.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsemysql.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsedata.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsedata.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemath.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsemath.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseglobal.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseglobal.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmariadbpp.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libmariadbpp.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${OUTPUT_PATH}" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/${OUTPUT_BASENAME}" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemain.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsemain.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseadmin.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseadmin.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsexbrl.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsexbrl.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeoddata.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseeoddata.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsesecurities.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsesecurities.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeconomy.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseeconomy.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsetraining.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsetraining.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseann.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseann.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsecubist.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsecubist.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsenet.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsenet.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemysql.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsemysql.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsedata.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsedata.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemath.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsemath.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseglobal.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseglobal.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmariadbpp.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libmariadbpp.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${OUTPUT_PATH}" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/${OUTPUT_BASENAME}" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemain.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsemain.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseadmin.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseadmin.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsexbrl.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsexbrl.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeoddata.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseeoddata.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsesecurities.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsesecurities.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeconomy.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseeconomy.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsetraining.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsetraining.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseann.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseann.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsecubist.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsecubist.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsenet.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsenet.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemysql.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsemysql.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsedata.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsedata.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemath.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsemath.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseglobal.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseglobal.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmariadbpp.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libmariadbpp.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${OUTPUT_PATH}" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/${OUTPUT_BASENAME}" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemain.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsemain.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseadmin.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseadmin.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsexbrl.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsexbrl.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeoddata.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseeoddata.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsesecurities.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsesecurities.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeconomy.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseeconomy.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsetraining.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsetraining.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseann.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseann.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsecubist.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsecubist.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsenet.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsenet.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemysql.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsemysql.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsedata.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsedata.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemath.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libsemath.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseglobal.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libseglobal.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmariadbpp.so" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/libmariadbpp.so" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/sebgpd/bin"
copyFileToTmpDir "${OUTPUT_PATH}" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/${OUTPUT_BASENAME}" 0755


# Generate tar file
cd "${TOP}"
rm -f ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/package/sebgpd.tar
cd ${NBTMPDIR}
tar -vcf ../../../../${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/package/sebgpd.tar *
checkReturnCode

# Cleanup
cd "${TOP}"
rm -rf ${NBTMPDIR}
