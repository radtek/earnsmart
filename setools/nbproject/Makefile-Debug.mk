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
LDLIBSOPTIONS=-Wl,-rpath,../semain/dist/Debug/GNU-Linux -L../semain/dist/Debug/GNU-Linux -lsemain -Wl,-rpath,../semysql/dist/Debug/GNU-Linux -L../semysql/dist/Debug/GNU-Linux -lsemysql -Wl,-rpath,../seglobal/dist/Debug/GNU-Linux -L../seglobal/dist/Debug/GNU-Linux -lseglobal -Wl,-rpath,../external/mariadbpp/dist/Debug/GNU-Linux -L../external/mariadbpp/dist/Debug/GNU-Linux -lmariadbpp -ljsoncpp -lZThread -lboost_system -lboost_date_time -lrt

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/setools

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/setools: ../semain/dist/Debug/GNU-Linux/libsemain.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/setools: ../semysql/dist/Debug/GNU-Linux/libsemysql.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/setools: ../seglobal/dist/Debug/GNU-Linux/libseglobal.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/setools: ../external/mariadbpp/dist/Debug/GNU-Linux/libmariadbpp.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/setools: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/setools ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mariadb -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:
	cd ../semain && ${MAKE} CND_SOLUTIONDIR=/home/santony/Documents/smartearn/Linux/phases/phase3/lib -f Makefile CONF=Debug
	cd ../semysql && ${MAKE} CND_SOLUTIONDIR=/home/santony/Documents/smartearn/Linux/phases/phase3/lib -f Makefile CONF=Debug
	cd ../seglobal && ${MAKE} CND_SOLUTIONDIR=/home/santony/Documents/smartearn/Linux/phases/phase3/lib -f Makefile CONF=Debug
	cd ../external/mariadbpp && ${MAKE} CND_SOLUTIONDIR=/home/santony/Documents/smartearn/Linux/phases/phase3/lib -f Makefile CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/setools

# Subprojects
.clean-subprojects:
	cd ../semain && ${MAKE} CND_SOLUTIONDIR=/home/santony/Documents/smartearn/Linux/phases/phase3/lib -f Makefile CONF=Debug clean
	cd ../semysql && ${MAKE} CND_SOLUTIONDIR=/home/santony/Documents/smartearn/Linux/phases/phase3/lib -f Makefile CONF=Debug clean
	cd ../seglobal && ${MAKE} CND_SOLUTIONDIR=/home/santony/Documents/smartearn/Linux/phases/phase3/lib -f Makefile CONF=Debug clean
	cd ../external/mariadbpp && ${MAKE} CND_SOLUTIONDIR=/home/santony/Documents/smartearn/Linux/phases/phase3/lib -f Makefile CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
