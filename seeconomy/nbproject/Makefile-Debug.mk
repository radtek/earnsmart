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
	${OBJECTDIR}/CAbstractFeeder.o \
	${OBJECTDIR}/CEconomicIndicator.o \
	${OBJECTDIR}/CEconomicIndicatorFeeder.o \
	${OBJECTDIR}/CEconomicNewsFeeds.o \
	${OBJECTDIR}/CEconomicNewsStorageProvider.o \
	${OBJECTDIR}/CEconomyAPI.o \
	${OBJECTDIR}/CFredDataSource.o \
	${OBJECTDIR}/CFredFeeder.o \
	${OBJECTDIR}/CIndicatorCategory.o \
	${OBJECTDIR}/CNcdcFeeder.o \
	${OBJECTDIR}/seeconomyapi.o


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
LDLIBSOPTIONS=-Wl,-rpath,'../setraining/dist/Debug/GNU-Linux' -L../setraining/dist/Debug/GNU-Linux -lsetraining -Wl,-rpath,'../senet/dist/Debug/GNU-Linux' -L../senet/dist/Debug/GNU-Linux -lsenet -Wl,-rpath,'../semysql/dist/Debug/GNU-Linux' -L../semysql/dist/Debug/GNU-Linux -lsemysql -Wl,-rpath,'../seglobal/dist/Debug/GNU-Linux' -L../seglobal/dist/Debug/GNU-Linux -lseglobal -lgsl -lgslcblas -lm

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeconomy.${CND_DLIB_EXT}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeconomy.${CND_DLIB_EXT}: ../setraining/dist/Debug/GNU-Linux/libsetraining.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeconomy.${CND_DLIB_EXT}: ../senet/dist/Debug/GNU-Linux/libsenet.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeconomy.${CND_DLIB_EXT}: ../semysql/dist/Debug/GNU-Linux/libsemysql.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeconomy.${CND_DLIB_EXT}: ../seglobal/dist/Debug/GNU-Linux/libseglobal.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeconomy.${CND_DLIB_EXT}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeconomy.${CND_DLIB_EXT} ${OBJECTFILES} ${LDLIBSOPTIONS} -fPIC -fvisibility=hidden -shared -fPIC

${OBJECTDIR}/CAbstractFeeder.o: CAbstractFeeder.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mariadb -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CAbstractFeeder.o CAbstractFeeder.cpp

${OBJECTDIR}/CEconomicIndicator.o: CEconomicIndicator.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mariadb -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CEconomicIndicator.o CEconomicIndicator.cpp

${OBJECTDIR}/CEconomicIndicatorFeeder.o: CEconomicIndicatorFeeder.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mariadb -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CEconomicIndicatorFeeder.o CEconomicIndicatorFeeder.cpp

${OBJECTDIR}/CEconomicNewsFeeds.o: CEconomicNewsFeeds.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mariadb -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CEconomicNewsFeeds.o CEconomicNewsFeeds.cpp

${OBJECTDIR}/CEconomicNewsStorageProvider.o: CEconomicNewsStorageProvider.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mariadb -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CEconomicNewsStorageProvider.o CEconomicNewsStorageProvider.cpp

${OBJECTDIR}/CEconomyAPI.o: CEconomyAPI.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mariadb -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CEconomyAPI.o CEconomyAPI.cpp

${OBJECTDIR}/CFredDataSource.o: CFredDataSource.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mariadb -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CFredDataSource.o CFredDataSource.cpp

${OBJECTDIR}/CFredFeeder.o: CFredFeeder.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mariadb -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CFredFeeder.o CFredFeeder.cpp

${OBJECTDIR}/CIndicatorCategory.o: CIndicatorCategory.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mariadb -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CIndicatorCategory.o CIndicatorCategory.cpp

${OBJECTDIR}/CNcdcFeeder.o: CNcdcFeeder.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mariadb -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CNcdcFeeder.o CNcdcFeeder.cpp

${OBJECTDIR}/seeconomyapi.o: seeconomyapi.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mariadb -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/seeconomyapi.o seeconomyapi.cpp

# Subprojects
.build-subprojects:
	cd ../setraining && ${MAKE} CND_SOLUTIONDIR=/home/santony/Documents/smartearn/Linux/phases/phase3/lib -f Makefile CONF=Debug
	cd ../senet && ${MAKE} CND_SOLUTIONDIR=/home/santony/Documents/smartearn/Linux/phases/phase3/lib -f Makefile CONF=Debug
	cd ../semysql && ${MAKE}  -f Makefile CONF=Debug
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} -r ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsetraining.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsenet.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemysql.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseglobal.so
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeconomy.${CND_DLIB_EXT}

# Subprojects
.clean-subprojects:
	cd ../setraining && ${MAKE} CND_SOLUTIONDIR=/home/santony/Documents/smartearn/Linux/phases/phase3/lib -f Makefile CONF=Debug clean
	cd ../senet && ${MAKE} CND_SOLUTIONDIR=/home/santony/Documents/smartearn/Linux/phases/phase3/lib -f Makefile CONF=Debug clean
	cd ../semysql && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
