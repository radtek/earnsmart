#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/CCurlClient.o \
	${OBJECTDIR}/CEmailTemplate.o \
	${OBJECTDIR}/CFtpClient.o \
	${OBJECTDIR}/CGeoIPProvider.o \
	${OBJECTDIR}/CHttpClient.o \
	${OBJECTDIR}/CRSSFeedManager.o \
	${OBJECTDIR}/CSmtpClient.o \
	${OBJECTDIR}/CSmtpVmimeClient.o \
	${OBJECTDIR}/CZMQClient.o \
	${OBJECTDIR}/senetapi.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-fPIC
CXXFLAGS=-fPIC

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-Wl,-rpath,'../seglobal/dist/Debug/GNU-Linux' -L../seglobal/dist/Debug/GNU-Linux -lseglobal -lcurl -lPocoNet -lzmq -lmrss -lboost_regex -lPocoNetSSL -lvmime

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsenet.${CND_DLIB_EXT}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsenet.${CND_DLIB_EXT}: ../seglobal/dist/Debug/GNU-Linux/libseglobal.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsenet.${CND_DLIB_EXT}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsenet.${CND_DLIB_EXT} ${OBJECTFILES} ${LDLIBSOPTIONS} -fPIC -shared -fPIC

${OBJECTDIR}/CCurlClient.o: CCurlClient.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -s -D__DEBUG__ -I/usr/include/libxml2 -I/usr/include/libxml++-2.6 -I/usr/include/mariadb -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CCurlClient.o CCurlClient.cpp

${OBJECTDIR}/CEmailTemplate.o: CEmailTemplate.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -s -D__DEBUG__ -I/usr/include/libxml2 -I/usr/include/libxml++-2.6 -I/usr/include/mariadb -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CEmailTemplate.o CEmailTemplate.cpp

${OBJECTDIR}/CFtpClient.o: CFtpClient.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -s -D__DEBUG__ -I/usr/include/libxml2 -I/usr/include/libxml++-2.6 -I/usr/include/mariadb -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CFtpClient.o CFtpClient.cpp

${OBJECTDIR}/CGeoIPProvider.o: CGeoIPProvider.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -s -D__DEBUG__ -I/usr/include/libxml2 -I/usr/include/libxml++-2.6 -I/usr/include/mariadb -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CGeoIPProvider.o CGeoIPProvider.cpp

${OBJECTDIR}/CHttpClient.o: CHttpClient.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -s -D__DEBUG__ -I/usr/include/libxml2 -I/usr/include/libxml++-2.6 -I/usr/include/mariadb -std=c++14 -fPIC -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CHttpClient.o CHttpClient.cpp

${OBJECTDIR}/CRSSFeedManager.o: CRSSFeedManager.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -s -D__DEBUG__ -I/usr/include/libxml2 -I/usr/include/libxml++-2.6 -I/usr/include/mariadb -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CRSSFeedManager.o CRSSFeedManager.cpp

${OBJECTDIR}/CSmtpClient.o: CSmtpClient.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -s -D__DEBUG__ -I/usr/include/libxml2 -I/usr/include/libxml++-2.6 -I/usr/include/mariadb -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CSmtpClient.o CSmtpClient.cpp

${OBJECTDIR}/CSmtpVmimeClient.o: CSmtpVmimeClient.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -s -D__DEBUG__ -I/usr/include/libxml2 -I/usr/include/libxml++-2.6 -I/usr/include/mariadb -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CSmtpVmimeClient.o CSmtpVmimeClient.cpp

${OBJECTDIR}/CZMQClient.o: CZMQClient.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -s -D__DEBUG__ -I/usr/include/libxml2 -I/usr/include/libxml++-2.6 -I/usr/include/mariadb -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CZMQClient.o CZMQClient.cpp

${OBJECTDIR}/senetapi.o: senetapi.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -s -D__DEBUG__ -I/usr/include/libxml2 -I/usr/include/libxml++-2.6 -I/usr/include/mariadb -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/senetapi.o senetapi.cpp

# Subprojects
.build-subprojects:
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} -r ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseglobal.so
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsenet.${CND_DLIB_EXT}

# Subprojects
.clean-subprojects:
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
