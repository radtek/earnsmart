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
CND_PLATFORM=GNU-Linux-x86
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
	${OBJECTDIR}/CTimer.o \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-lrt -lxml2 -lboost_filesystem -lboost_system -lboost_date_time -lpthread_workqueue -llog4cxx -Wl,-rpath,../seadmin/dist/Debug/GNU-Linux-x86 -L../seadmin/dist/Debug/GNU-Linux-x86 -lseadmin -ljsoncpp ../semysql/dist/Debug/GNU-Linux-x86/libsemysql.a -Wl,-rpath,../semain/dist/Debug/GNU-Linux-x86 -L../semain/dist/Debug/GNU-Linux-x86 -lsemain ../senet/dist/Debug/GNU-Linux-x86/libsenet.a ../sedata/dist/Debug/GNU-Linux-x86/libsedata.a -lboost_regex -lmysqlpp -lcurl ../seglobal/dist/Debug/GNU-Linux-x86/libseglobal.a

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/misctests

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/misctests: ../seadmin/dist/Debug/GNU-Linux-x86/libseadmin.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/misctests: ../semysql/dist/Debug/GNU-Linux-x86/libsemysql.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/misctests: ../semain/dist/Debug/GNU-Linux-x86/libsemain.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/misctests: ../senet/dist/Debug/GNU-Linux-x86/libsenet.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/misctests: ../sedata/dist/Debug/GNU-Linux-x86/libsedata.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/misctests: ../seglobal/dist/Debug/GNU-Linux-x86/libseglobal.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/misctests: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/misctests ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/CTimer.o: CTimer.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/libxml2 -I../../../external/google/cstl -I/usr/include/mysql -I/usr/include/mysql++ -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/CTimer.o CTimer.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/libxml2 -I../../../external/google/cstl -I/usr/include/mysql -I/usr/include/mysql++ -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:
	cd ../seadmin && ${MAKE}  -f Makefile CONF=Debug
	cd ../semysql && ${MAKE}  -f Makefile CONF=Debug
	cd ../semain && ${MAKE}  -f Makefile CONF=Debug
	cd ../senet && ${MAKE}  -f Makefile CONF=Debug
	cd ../sedata && ${MAKE}  -f Makefile CONF=Debug
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/misctests

# Subprojects
.clean-subprojects:
	cd ../seadmin && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../semysql && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../semain && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../senet && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../sedata && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
