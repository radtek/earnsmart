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
	${OBJECTDIR}/CAPIGateway.o \
	${OBJECTDIR}/CJsonWrapper.o \
	${OBJECTDIR}/FCgiIO.o \
	${OBJECTDIR}/JsonWrappers.o \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-std=c++0x
CXXFLAGS=-std=c++0x

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=../seglobal/dist/Debug/GNU-Linux/libseglobal.a ../semysql/dist/Debug/GNU-Linux/libsemysql.a -Wl,-rpath,'../seadmin/dist/Debug/GNU-Linux' -L../seadmin/dist/Debug/GNU-Linux -lseadmin -Wl,-rpath,'../sexbrl/dist/Debug/GNU-Linux' -L../sexbrl/dist/Debug/GNU-Linux -lsexbrl -Wl,-rpath,'../sesecurities/dist/Debug/GNU-Linux' -L../sesecurities/dist/Debug/GNU-Linux -lsesecurities -Wl,-rpath,'../semain/dist/Debug/GNU-Linux' -L../semain/dist/Debug/GNU-Linux -lsemain -ljsoncpp -lcgicc -lfcgi -lfcgi++ -llog4cxx -lboost_regex-mt -lboost_date_time

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/segw.jac

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/segw.jac: ../seglobal/dist/Debug/GNU-Linux/libseglobal.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/segw.jac: ../semysql/dist/Debug/GNU-Linux/libsemysql.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/segw.jac: ../seadmin/dist/Debug/GNU-Linux/libseadmin.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/segw.jac: ../sexbrl/dist/Debug/GNU-Linux/libsexbrl.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/segw.jac: ../sesecurities/dist/Debug/GNU-Linux/libsesecurities.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/segw.jac: ../semain/dist/Debug/GNU-Linux/libsemain.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/segw.jac: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/segw.jac ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/CAPIGateway.o: CAPIGateway.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CAPIGateway.o CAPIGateway.cpp

${OBJECTDIR}/CJsonWrapper.o: CJsonWrapper.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CJsonWrapper.o CJsonWrapper.cpp

${OBJECTDIR}/FCgiIO.o: FCgiIO.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/FCgiIO.o FCgiIO.cpp

${OBJECTDIR}/JsonWrappers.o: JsonWrappers.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/JsonWrappers.o JsonWrappers.cpp

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Debug
	cd ../semysql && ${MAKE}  -f Makefile CONF=Debug
	cd ../seadmin && ${MAKE}  -f Makefile CONF=Debug
	cd ../sexbrl && ${MAKE}  -f Makefile CONF=Debug
	cd ../sesecurities && ${MAKE}  -f Makefile CONF=Debug
	cd ../semain && ${MAKE}  -f Makefile CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} -r ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseadmin.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsexbrl.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsesecurities.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemain.so
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/segw.jac

# Subprojects
.clean-subprojects:
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../semysql && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../seadmin && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../sexbrl && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../sesecurities && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../semain && ${MAKE}  -f Makefile CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
