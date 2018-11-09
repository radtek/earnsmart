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
CND_CONF=Debug_mysql
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/CAttributeDescription.o \
	${OBJECTDIR}/CBayesianDataProcessor.o \
	${OBJECTDIR}/CMVLRProcessor.o \
	${OBJECTDIR}/CPredictor.o \
	${OBJECTDIR}/CProductDescription.o \
	${OBJECTDIR}/CResult.o \
	${OBJECTDIR}/CTimeSeriesBuilder.o \
	${OBJECTDIR}/CTrainingSet.o \
	${OBJECTDIR}/CXXXAlgoProcessBase.o \
	${OBJECTDIR}/sepredictapi.o


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
LDLIBSOPTIONS=-Wl,-rpath,'../external/mariadbpp/dist/Debug_mysql/GNU-Linux' -L../external/mariadbpp/dist/Debug_mysql/GNU-Linux -lmariadbpp -Wl,-rpath,'../seeconomy/dist/Debug_mysql/GNU-Linux' -L../seeconomy/dist/Debug_mysql/GNU-Linux -lseeconomy

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsepredict.${CND_DLIB_EXT}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsepredict.${CND_DLIB_EXT}: ../external/mariadbpp/dist/Debug_mysql/GNU-Linux/libmariadbpp.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsepredict.${CND_DLIB_EXT}: ../seeconomy/dist/Debug_mysql/GNU-Linux/libseeconomy.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsepredict.${CND_DLIB_EXT}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsepredict.${CND_DLIB_EXT} ${OBJECTFILES} ${LDLIBSOPTIONS} -fPIC -fvisibility=hidden -shared -fPIC

${OBJECTDIR}/CAttributeDescription.o: CAttributeDescription.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -I../config/../external/mariadbpp -I/usr/include/bsoncxx/v0.2 -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CAttributeDescription.o CAttributeDescription.cpp

${OBJECTDIR}/CBayesianDataProcessor.o: CBayesianDataProcessor.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -I../config/../external/mariadbpp -I/usr/include/bsoncxx/v0.2 -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CBayesianDataProcessor.o CBayesianDataProcessor.cpp

${OBJECTDIR}/CMVLRProcessor.o: CMVLRProcessor.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -I../config/../external/mariadbpp -I/usr/include/bsoncxx/v0.2 -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CMVLRProcessor.o CMVLRProcessor.cpp

${OBJECTDIR}/CPredictor.o: CPredictor.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -I../config/../external/mariadbpp -I/usr/include/bsoncxx/v0.2 -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CPredictor.o CPredictor.cpp

${OBJECTDIR}/CProductDescription.o: CProductDescription.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -I../config/../external/mariadbpp -I/usr/include/bsoncxx/v0.2 -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CProductDescription.o CProductDescription.cpp

${OBJECTDIR}/CResult.o: CResult.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -I../config/../external/mariadbpp -I/usr/include/bsoncxx/v0.2 -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CResult.o CResult.cpp

${OBJECTDIR}/CTimeSeriesBuilder.o: CTimeSeriesBuilder.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -I../config/../external/mariadbpp -I/usr/include/bsoncxx/v0.2 -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CTimeSeriesBuilder.o CTimeSeriesBuilder.cpp

${OBJECTDIR}/CTrainingSet.o: CTrainingSet.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -I../config/../external/mariadbpp -I/usr/include/bsoncxx/v0.2 -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CTrainingSet.o CTrainingSet.cpp

${OBJECTDIR}/CXXXAlgoProcessBase.o: CXXXAlgoProcessBase.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -I../config/../external/mariadbpp -I/usr/include/bsoncxx/v0.2 -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CXXXAlgoProcessBase.o CXXXAlgoProcessBase.cpp

${OBJECTDIR}/sepredictapi.o: sepredictapi.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -I../config/../external/mariadbpp -I/usr/include/bsoncxx/v0.2 -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/sepredictapi.o sepredictapi.cpp

# Subprojects
.build-subprojects:
	cd ../external/mariadbpp && ${MAKE}  -f Makefile CONF=Debug_mysql
	cd ../seeconomy && ${MAKE}  -f Makefile CONF=Debug_mysql

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} -r ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmariadbpp.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeconomy.so
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsepredict.${CND_DLIB_EXT}

# Subprojects
.clean-subprojects:
	cd ../external/mariadbpp && ${MAKE}  -f Makefile CONF=Debug_mysql clean
	cd ../seeconomy && ${MAKE}  -f Makefile CONF=Debug_mysql clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
