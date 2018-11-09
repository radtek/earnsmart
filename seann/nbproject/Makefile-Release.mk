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
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/CFann.o \
	${OBJECTDIR}/doublefann.o \
	${OBJECTDIR}/fann.o \
	${OBJECTDIR}/fann_cascade.o \
	${OBJECTDIR}/fann_error.o \
	${OBJECTDIR}/fann_io.o \
	${OBJECTDIR}/fann_train.o \
	${OBJECTDIR}/fann_train_data.o \
	${OBJECTDIR}/floatfann.o


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
LDLIBSOPTIONS=-lm -Wl,-rpath,'../seglobal/dist/Release/GNU-Linux' -L../seglobal/dist/Release/GNU-Linux -lseglobal

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseann.${CND_DLIB_EXT}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseann.${CND_DLIB_EXT}: ../seglobal/dist/Release/GNU-Linux/libseglobal.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseann.${CND_DLIB_EXT}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseann.${CND_DLIB_EXT} ${OBJECTFILES} ${LDLIBSOPTIONS} -shared -fPIC

${OBJECTDIR}/CFann.o: CFann.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mariadb -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CFann.o CFann.cpp

${OBJECTDIR}/doublefann.o: doublefann.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -O2 -std=c11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/doublefann.o doublefann.c

${OBJECTDIR}/fann.o: fann.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -O2 -std=c11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/fann.o fann.c

${OBJECTDIR}/fann_cascade.o: fann_cascade.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -O2 -std=c11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/fann_cascade.o fann_cascade.c

${OBJECTDIR}/fann_error.o: fann_error.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -O2 -std=c11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/fann_error.o fann_error.c

${OBJECTDIR}/fann_io.o: fann_io.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -O2 -std=c11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/fann_io.o fann_io.c

${OBJECTDIR}/fann_train.o: fann_train.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -O2 -std=c11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/fann_train.o fann_train.c

${OBJECTDIR}/fann_train_data.o: fann_train_data.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -O2 -std=c11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/fann_train_data.o fann_train_data.c

${OBJECTDIR}/floatfann.o: floatfann.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -O2 -std=c11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/floatfann.o floatfann.c

# Subprojects
.build-subprojects:
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Release

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} -r ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseglobal.so
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseann.${CND_DLIB_EXT}

# Subprojects
.clean-subprojects:
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Release clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
