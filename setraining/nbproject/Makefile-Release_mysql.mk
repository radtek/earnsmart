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
CND_CONF=Release_mysql
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/CPredictionSetup.o \
	${OBJECTDIR}/CTrainingAPI.o \
	${OBJECTDIR}/IndicatorTrainingClasses.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-fPIC -fvisibility=hidden
CXXFLAGS=-fPIC -fvisibility=hidden

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-Wl,-rpath,'../seann/dist/Release_mysql/GNU-Linux' -L../seann/dist/Release_mysql/GNU-Linux -lseann -Wl,-rpath,'../sedata/dist/Release/GNU-Linux' -L../sedata/dist/Release/GNU-Linux -lsedata -Wl,-rpath,'../semysql/dist/Release_mysql/GNU-Linux' -L../semysql/dist/Release_mysql/GNU-Linux -lsemysql -Wl,-rpath,'../seglobal/dist/Release/GNU-Linux' -L../seglobal/dist/Release/GNU-Linux -lseglobal -lZThread

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsetraining.${CND_DLIB_EXT}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsetraining.${CND_DLIB_EXT}: ../seann/dist/Release_mysql/GNU-Linux/libseann.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsetraining.${CND_DLIB_EXT}: ../sedata/dist/Release/GNU-Linux/libsedata.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsetraining.${CND_DLIB_EXT}: ../semysql/dist/Release_mysql/GNU-Linux/libsemysql.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsetraining.${CND_DLIB_EXT}: ../seglobal/dist/Release/GNU-Linux/libseglobal.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsetraining.${CND_DLIB_EXT}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsetraining.${CND_DLIB_EXT} ${OBJECTFILES} ${LDLIBSOPTIONS} -fPIC -fvisibility=hidden -shared -fPIC

${OBJECTDIR}/CPredictionSetup.o: CPredictionSetup.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mysql -I/usr/include/libxml2 -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CPredictionSetup.o CPredictionSetup.cpp

${OBJECTDIR}/CTrainingAPI.o: CTrainingAPI.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mysql -I/usr/include/libxml2 -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CTrainingAPI.o CTrainingAPI.cpp

${OBJECTDIR}/IndicatorTrainingClasses.o: IndicatorTrainingClasses.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mysql -I/usr/include/libxml2 -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/IndicatorTrainingClasses.o IndicatorTrainingClasses.cpp

# Subprojects
.build-subprojects:
	cd ../seann && ${MAKE}  -f Makefile CONF=Release_mysql
	cd ../sedata && ${MAKE}  -f Makefile CONF=Release
	cd ../semysql && ${MAKE}  -f Makefile CONF=Release_mysql
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Release

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} -r ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseann.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsedata.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemysql.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseglobal.so
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsetraining.${CND_DLIB_EXT}

# Subprojects
.clean-subprojects:
	cd ../seann && ${MAKE}  -f Makefile CONF=Release_mysql clean
	cd ../sedata && ${MAKE}  -f Makefile CONF=Release clean
	cd ../semysql && ${MAKE}  -f Makefile CONF=Release_mysql clean
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Release clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
