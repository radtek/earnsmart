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
LDLIBSOPTIONS=../seglobal/dist/Release/GNU-Linux-x86/libseglobal.a ../semysql/dist/Release/GNU-Linux-x86/libsemysql.a -Wl,-rpath,../sexbrl/dist/Debug/GNU-Linux-x86 -L../sexbrl/dist/Debug/GNU-Linux-x86 -lsexbrl -Wl,-rpath,../sesecurities/dist/Release/GNU-Linux-x86 -L../sesecurities/dist/Release/GNU-Linux-x86 -lsesecurities -ljsoncpp -lcgicc -lfcgi -lfcgi++ -llog4cxx -lboost_regex-mt -ljson -Wl,-rpath,../semain/dist/Debug/GNU-Linux-x86 -L../semain/dist/Debug/GNU-Linux-x86 -lsemain

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/segw.jac

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/segw.jac: ../seglobal/dist/Release/GNU-Linux-x86/libseglobal.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/segw.jac: ../semysql/dist/Release/GNU-Linux-x86/libsemysql.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/segw.jac: ../sexbrl/dist/Debug/GNU-Linux-x86/libsexbrl.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/segw.jac: ../sesecurities/dist/Release/GNU-Linux-x86/libsesecurities.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/segw.jac: ../semain/dist/Debug/GNU-Linux-x86/libsemain.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/segw.jac: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/segw.jac ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/CAPIGateway.o: CAPIGateway.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CAPIGateway.o CAPIGateway.cpp

${OBJECTDIR}/CJsonWrapper.o: CJsonWrapper.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CJsonWrapper.o CJsonWrapper.cpp

${OBJECTDIR}/FCgiIO.o: FCgiIO.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/FCgiIO.o FCgiIO.cpp

${OBJECTDIR}/JsonWrappers.o: JsonWrappers.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/JsonWrappers.o JsonWrappers.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Release
	cd ../semysql && ${MAKE}  -f Makefile CONF=Release
	cd ../sexbrl && ${MAKE}  -f Makefile CONF=Debug
	cd ../sesecurities && ${MAKE}  -f Makefile CONF=Release
	cd ../semain && ${MAKE}  -f Makefile CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/segw.jac

# Subprojects
.clean-subprojects:
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Release clean
	cd ../semysql && ${MAKE}  -f Makefile CONF=Release clean
	cd ../sexbrl && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../sesecurities && ${MAKE}  -f Makefile CONF=Release clean
	cd ../semain && ${MAKE}  -f Makefile CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
