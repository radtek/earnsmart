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
	${OBJECTDIR}/CAdminAPITest.o \
	${OBJECTDIR}/CCryptoTest.o \
	${OBJECTDIR}/CEmailTemplateTest.o \
	${OBJECTDIR}/CEodDataTest.o \
	${OBJECTDIR}/CExceptionTest.o \
	${OBJECTDIR}/CHelpDocTreeTest.o \
	${OBJECTDIR}/CMonteCarloTests.o \
	${OBJECTDIR}/CPredictionsTest.o \
	${OBJECTDIR}/CSecurityAPITest.o \
	${OBJECTDIR}/CSqlAstTest.o \
	${OBJECTDIR}/CUnitTestBase.o \
	${OBJECTDIR}/CUtilityTest.o \
	${OBJECTDIR}/CXbrlProcessingTests.o \
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
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/seunittest

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/seunittest: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/seunittest ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/CAdminAPITest.o: CAdminAPITest.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CAdminAPITest.o CAdminAPITest.cpp

${OBJECTDIR}/CCryptoTest.o: CCryptoTest.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CCryptoTest.o CCryptoTest.cpp

${OBJECTDIR}/CEmailTemplateTest.o: CEmailTemplateTest.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CEmailTemplateTest.o CEmailTemplateTest.cpp

${OBJECTDIR}/CEodDataTest.o: CEodDataTest.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CEodDataTest.o CEodDataTest.cpp

${OBJECTDIR}/CExceptionTest.o: CExceptionTest.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CExceptionTest.o CExceptionTest.cpp

${OBJECTDIR}/CHelpDocTreeTest.o: CHelpDocTreeTest.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CHelpDocTreeTest.o CHelpDocTreeTest.cpp

${OBJECTDIR}/CMonteCarloTests.o: CMonteCarloTests.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CMonteCarloTests.o CMonteCarloTests.cpp

${OBJECTDIR}/CPredictionsTest.o: CPredictionsTest.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CPredictionsTest.o CPredictionsTest.cpp

${OBJECTDIR}/CSecurityAPITest.o: CSecurityAPITest.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CSecurityAPITest.o CSecurityAPITest.cpp

${OBJECTDIR}/CSqlAstTest.o: CSqlAstTest.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CSqlAstTest.o CSqlAstTest.cpp

${OBJECTDIR}/CUnitTestBase.o: CUnitTestBase.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CUnitTestBase.o CUnitTestBase.cpp

${OBJECTDIR}/CUtilityTest.o: CUtilityTest.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CUtilityTest.o CUtilityTest.cpp

${OBJECTDIR}/CXbrlProcessingTests.o: CXbrlProcessingTests.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CXbrlProcessingTests.o CXbrlProcessingTests.cpp

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

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
