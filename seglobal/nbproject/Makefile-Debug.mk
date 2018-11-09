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
	${OBJECTDIR}/CAddress.o \
	${OBJECTDIR}/CAst.o \
	${OBJECTDIR}/CBsonSerializedData.o \
	${OBJECTDIR}/CDate.o \
	${OBJECTDIR}/CDictionary.o \
	${OBJECTDIR}/CDirectory.o \
	${OBJECTDIR}/CEnumerator.o \
	${OBJECTDIR}/CFibnacciQueue.o \
	${OBJECTDIR}/CFile.o \
	${OBJECTDIR}/CFinancialPeriod.o \
	${OBJECTDIR}/CFunctionCache.o \
	${OBJECTDIR}/CFutex.o \
	${OBJECTDIR}/CGlobalStringCache.o \
	${OBJECTDIR}/CIdName.o \
	${OBJECTDIR}/CJsonSerializedData.o \
	${OBJECTDIR}/CList.o \
	${OBJECTDIR}/CMisc.o \
	${OBJECTDIR}/CObjectRoot.o \
	${OBJECTDIR}/CQuarter.o \
	${OBJECTDIR}/CROList.o \
	${OBJECTDIR}/CSecure.o \
	${OBJECTDIR}/CSecurityExchange.o \
	${OBJECTDIR}/CString.o \
	${OBJECTDIR}/CStringToNumber.o \
	${OBJECTDIR}/CStringUtility.o \
	${OBJECTDIR}/CTarExtractor.o \
	${OBJECTDIR}/CThreadPool.o \
	${OBJECTDIR}/CTimeStamp.o \
	${OBJECTDIR}/CTimer.o \
	${OBJECTDIR}/CTimerServices.o \
	${OBJECTDIR}/CodeMessages.o \
	${OBJECTDIR}/FileSystemMonitor.o \
	${OBJECTDIR}/OmniType.o \
	${OBJECTDIR}/SecurityDescription.o \
	${OBJECTDIR}/TypeCheck.o \
	${OBJECTDIR}/exceptions.o \
	${OBJECTDIR}/globallogger.o \
	${OBJECTDIR}/segvcatch.o \
	${OBJECTDIR}/stringconversions.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-fvisibility=default -O2
CXXFLAGS=-fvisibility=default -O2

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-ltar -lrt -lboost_date_time -lboost_system -lboost_regex

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseglobal.${CND_DLIB_EXT}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseglobal.${CND_DLIB_EXT}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseglobal.${CND_DLIB_EXT} ${OBJECTFILES} ${LDLIBSOPTIONS} -shared -fPIC

${OBJECTDIR}/CAddress.o: CAddress.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CAddress.o CAddress.cpp

${OBJECTDIR}/CAst.o: CAst.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CAst.o CAst.cpp

${OBJECTDIR}/CBsonSerializedData.o: CBsonSerializedData.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CBsonSerializedData.o CBsonSerializedData.cpp

${OBJECTDIR}/CDate.o: CDate.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CDate.o CDate.cpp

${OBJECTDIR}/CDictionary.o: CDictionary.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CDictionary.o CDictionary.cpp

${OBJECTDIR}/CDirectory.o: CDirectory.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CDirectory.o CDirectory.cpp

${OBJECTDIR}/CEnumerator.o: CEnumerator.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CEnumerator.o CEnumerator.cpp

${OBJECTDIR}/CFibnacciQueue.o: CFibnacciQueue.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CFibnacciQueue.o CFibnacciQueue.cpp

${OBJECTDIR}/CFile.o: CFile.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CFile.o CFile.cpp

${OBJECTDIR}/CFinancialPeriod.o: CFinancialPeriod.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CFinancialPeriod.o CFinancialPeriod.cpp

${OBJECTDIR}/CFunctionCache.o: CFunctionCache.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CFunctionCache.o CFunctionCache.cpp

${OBJECTDIR}/CFutex.o: CFutex.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CFutex.o CFutex.cpp

${OBJECTDIR}/CGlobalStringCache.o: CGlobalStringCache.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CGlobalStringCache.o CGlobalStringCache.cpp

${OBJECTDIR}/CIdName.o: CIdName.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CIdName.o CIdName.cpp

${OBJECTDIR}/CJsonSerializedData.o: CJsonSerializedData.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CJsonSerializedData.o CJsonSerializedData.cpp

${OBJECTDIR}/CList.o: CList.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CList.o CList.cpp

${OBJECTDIR}/CMisc.o: CMisc.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CMisc.o CMisc.cpp

${OBJECTDIR}/CObjectRoot.o: CObjectRoot.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -std=c++14 -fPIC -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CObjectRoot.o CObjectRoot.cpp

${OBJECTDIR}/CQuarter.o: CQuarter.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CQuarter.o CQuarter.cpp

${OBJECTDIR}/CROList.o: CROList.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CROList.o CROList.cpp

${OBJECTDIR}/CSecure.o: CSecure.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CSecure.o CSecure.cpp

${OBJECTDIR}/CSecurityExchange.o: CSecurityExchange.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CSecurityExchange.o CSecurityExchange.cpp

${OBJECTDIR}/CString.o: CString.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CString.o CString.cpp

${OBJECTDIR}/CStringToNumber.o: CStringToNumber.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CStringToNumber.o CStringToNumber.cpp

${OBJECTDIR}/CStringUtility.o: CStringUtility.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CStringUtility.o CStringUtility.cpp

${OBJECTDIR}/CTarExtractor.o: CTarExtractor.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CTarExtractor.o CTarExtractor.cpp

${OBJECTDIR}/CThreadPool.o: CThreadPool.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CThreadPool.o CThreadPool.cpp

${OBJECTDIR}/CTimeStamp.o: CTimeStamp.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CTimeStamp.o CTimeStamp.cpp

${OBJECTDIR}/CTimer.o: CTimer.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CTimer.o CTimer.cpp

${OBJECTDIR}/CTimerServices.o: CTimerServices.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CTimerServices.o CTimerServices.cpp

${OBJECTDIR}/CodeMessages.o: CodeMessages.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CodeMessages.o CodeMessages.cpp

${OBJECTDIR}/FileSystemMonitor.o: FileSystemMonitor.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/FileSystemMonitor.o FileSystemMonitor.cpp

${OBJECTDIR}/OmniType.o: OmniType.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/OmniType.o OmniType.cpp

${OBJECTDIR}/SecurityDescription.o: SecurityDescription.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SecurityDescription.o SecurityDescription.cpp

${OBJECTDIR}/TypeCheck.o: TypeCheck.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/TypeCheck.o TypeCheck.cpp

${OBJECTDIR}/exceptions.o: exceptions.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/exceptions.o exceptions.cpp

${OBJECTDIR}/globallogger.o: globallogger.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/globallogger.o globallogger.cpp

${OBJECTDIR}/segvcatch.o: segvcatch.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/segvcatch.o segvcatch.cpp

${OBJECTDIR}/stringconversions.o: stringconversions.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/stringconversions.o stringconversions.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
