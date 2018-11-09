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
CND_CONF=Release
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
LDLIBSOPTIONS=-L/usr/lib/x86_64-linux-gnu -Wl,-rpath,../seadmin/dist/Release/GNU-Linux-x86 -L../seadmin/dist/Release/GNU-Linux-x86 -lseadmin -Wl,-rpath,../sesecurities/dist/Release/GNU-Linux-x86 -L../sesecurities/dist/Release/GNU-Linux-x86 -lsesecurities -Wl,-rpath,../semain/dist/Release/GNU-Linux-x86 -L../semain/dist/Release/GNU-Linux-x86 -lsemain -Wl,-rpath,../senet/dist/Release/GNU-Linux-x86 -L../senet/dist/Release/GNU-Linux-x86 -lsenet -Wl,-rpath,../external/mariadbpp/dist/Release/GNU-Linux-x86 -L../external/mariadbpp/dist/Release/GNU-Linux-x86 -lmariadbpp ../seglobal/dist/Release/GNU-Linux-x86/libseglobal.a -lboost_date_time -lcurl -lboost_filesystem -lboost_system -lPocoNet -ljsoncpp -lmariadbclient_r

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/seadmin_tests

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/seadmin_tests: ../seadmin/dist/Release/GNU-Linux-x86/libseadmin.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/seadmin_tests: ../sesecurities/dist/Release/GNU-Linux-x86/libsesecurities.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/seadmin_tests: ../semain/dist/Release/GNU-Linux-x86/libsemain.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/seadmin_tests: ../senet/dist/Release/GNU-Linux-x86/libsenet.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/seadmin_tests: ../external/mariadbpp/dist/Release/GNU-Linux-x86/libmariadbpp.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/seadmin_tests: ../seglobal/dist/Release/GNU-Linux-x86/libseglobal.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/seadmin_tests: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/seadmin_tests ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mysql -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:
	cd ../seadmin && ${MAKE}  -f Makefile CONF=Release
	cd ../sesecurities && ${MAKE}  -f Makefile CONF=Release
	cd ../semain && ${MAKE}  -f Makefile CONF=Release
	cd ../senet && ${MAKE}  -f Makefile CONF=Release
	cd ../external/mariadbpp && ${MAKE}  -f Makefile CONF=Release
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Release

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/seadmin_tests

# Subprojects
.clean-subprojects:
	cd ../seadmin && ${MAKE}  -f Makefile CONF=Release clean
	cd ../sesecurities && ${MAKE}  -f Makefile CONF=Release clean
	cd ../semain && ${MAKE}  -f Makefile CONF=Release clean
	cd ../senet && ${MAKE}  -f Makefile CONF=Release clean
	cd ../external/mariadbpp && ${MAKE}  -f Makefile CONF=Release clean
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Release clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
