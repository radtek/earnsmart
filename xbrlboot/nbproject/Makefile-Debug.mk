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
	${OBJECTDIR}/CXbrlCalculation.o \
	${OBJECTDIR}/CXbrlDoc.o \
	${OBJECTDIR}/CXbrlElement.o \
	${OBJECTDIR}/CXbrlLine.o \
	${OBJECTDIR}/CXbrlStatementModel.o \
	${OBJECTDIR}/CXbrlStatementParser.o \
	${OBJECTDIR}/CXbrlStatementPersistor.o \
	${OBJECTDIR}/c_algorithms.o \
	${OBJECTDIR}/c_array.o \
	${OBJECTDIR}/c_deque.o \
	${OBJECTDIR}/c_map.o \
	${OBJECTDIR}/c_rb.o \
	${OBJECTDIR}/c_set.o \
	${OBJECTDIR}/c_slist.o \
	${OBJECTDIR}/c_util.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/pugixml.o \
	${OBJECTDIR}/xbrlcalparsingtest.o


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
LDLIBSOPTIONS=-lxml2 -lboost_filesystem -lboost_system ../sedata/dist/Debug/GNU-Linux-x86/libsedata.a ../seglobal/dist/Debug/GNU-Linux-x86/libseglobal.a -lboost_regex-mt -lboost_date_time -lpthread_workqueue -llog4cxx

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/xbrlboot

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/xbrlboot: ../sedata/dist/Debug/GNU-Linux-x86/libsedata.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/xbrlboot: ../seglobal/dist/Debug/GNU-Linux-x86/libseglobal.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/xbrlboot: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/xbrlboot ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/CXbrlCalculation.o: CXbrlCalculation.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/libxml2 -I../../../external/google/cstl -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/CXbrlCalculation.o CXbrlCalculation.cpp

${OBJECTDIR}/CXbrlDoc.o: CXbrlDoc.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/libxml2 -I../../../external/google/cstl -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/CXbrlDoc.o CXbrlDoc.cpp

${OBJECTDIR}/CXbrlElement.o: CXbrlElement.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/libxml2 -I../../../external/google/cstl -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/CXbrlElement.o CXbrlElement.cpp

${OBJECTDIR}/CXbrlLine.o: CXbrlLine.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/libxml2 -I../../../external/google/cstl -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/CXbrlLine.o CXbrlLine.cpp

${OBJECTDIR}/CXbrlStatementModel.o: CXbrlStatementModel.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/libxml2 -I../../../external/google/cstl -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/CXbrlStatementModel.o CXbrlStatementModel.cpp

${OBJECTDIR}/CXbrlStatementParser.o: CXbrlStatementParser.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/libxml2 -I../../../external/google/cstl -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/CXbrlStatementParser.o CXbrlStatementParser.cpp

${OBJECTDIR}/CXbrlStatementPersistor.o: CXbrlStatementPersistor.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/libxml2 -I../../../external/google/cstl -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/CXbrlStatementPersistor.o CXbrlStatementPersistor.cpp

${OBJECTDIR}/c_algorithms.o: c_algorithms.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -I/usr/include/libxml2 -I../../../external/google/cstl -MMD -MP -MF $@.d -o ${OBJECTDIR}/c_algorithms.o c_algorithms.c

${OBJECTDIR}/c_array.o: c_array.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -I/usr/include/libxml2 -I../../../external/google/cstl -MMD -MP -MF $@.d -o ${OBJECTDIR}/c_array.o c_array.c

${OBJECTDIR}/c_deque.o: c_deque.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -I/usr/include/libxml2 -I../../../external/google/cstl -MMD -MP -MF $@.d -o ${OBJECTDIR}/c_deque.o c_deque.c

${OBJECTDIR}/c_map.o: c_map.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -I/usr/include/libxml2 -I../../../external/google/cstl -MMD -MP -MF $@.d -o ${OBJECTDIR}/c_map.o c_map.c

${OBJECTDIR}/c_rb.o: c_rb.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -I/usr/include/libxml2 -I../../../external/google/cstl -MMD -MP -MF $@.d -o ${OBJECTDIR}/c_rb.o c_rb.c

${OBJECTDIR}/c_set.o: c_set.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -I/usr/include/libxml2 -I../../../external/google/cstl -MMD -MP -MF $@.d -o ${OBJECTDIR}/c_set.o c_set.c

${OBJECTDIR}/c_slist.o: c_slist.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -I/usr/include/libxml2 -I../../../external/google/cstl -MMD -MP -MF $@.d -o ${OBJECTDIR}/c_slist.o c_slist.c

${OBJECTDIR}/c_util.o: c_util.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -I/usr/include/libxml2 -I../../../external/google/cstl -MMD -MP -MF $@.d -o ${OBJECTDIR}/c_util.o c_util.c

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/libxml2 -I../../../external/google/cstl -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/pugixml.o: pugixml.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/libxml2 -I../../../external/google/cstl -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/pugixml.o pugixml.cpp

${OBJECTDIR}/xbrlcalparsingtest.o: xbrlcalparsingtest.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -I/usr/include/libxml2 -I../../../external/google/cstl -MMD -MP -MF $@.d -o ${OBJECTDIR}/xbrlcalparsingtest.o xbrlcalparsingtest.c

# Subprojects
.build-subprojects:
	cd ../sedata && ${MAKE}  -f Makefile CONF=Debug
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/xbrlboot

# Subprojects
.clean-subprojects:
	cd ../sedata && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
