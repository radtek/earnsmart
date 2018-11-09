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
LDLIBSOPTIONS=-L/usr/lib/x86_64-linux-gnu/ -Wl,-rpath,'../../semain/dist/Debug/GNU-Linux' -L../../semain/dist/Debug/GNU-Linux -lsemain -Wl,-rpath,'../../external/mariadbpp/dist/Debug/GNU-Linux' -L../../external/mariadbpp/dist/Debug/GNU-Linux -lmariadbpp -Wl,-rpath,'../../seadmin/dist/Debug/GNU-Linux' -L../../seadmin/dist/Debug/GNU-Linux -lseadmin -Wl,-rpath,'../../seeconomy/dist/Debug/GNU-Linux' -L../../seeconomy/dist/Debug/GNU-Linux -lseeconomy -Wl,-rpath,'../../sesecurities/dist/Debug/GNU-Linux' -L../../sesecurities/dist/Debug/GNU-Linux -lsesecurities -Wl,-rpath,'../../seeoddata/dist/Debug/GNU-Linux' -L../../seeoddata/dist/Debug/GNU-Linux -lseeoddata -Wl,-rpath,'../../sexbrl/dist/Debug/GNU-Linux' -L../../sexbrl/dist/Debug/GNU-Linux -lsexbrl -Wl,-rpath,'../../sepredict/dist/Debug/GNU-Linux' -L../../sepredict/dist/Debug/GNU-Linux -lsepredict -Wl,-rpath,'../../setraining/dist/Debug/GNU-Linux' -L../../setraining/dist/Debug/GNU-Linux -lsetraining -Wl,-rpath,'../../senet/dist/Debug/GNU-Linux' -L../../senet/dist/Debug/GNU-Linux -lsenet -Wl,-rpath,'../../seann/dist/Debug/GNU-Linux' -L../../seann/dist/Debug/GNU-Linux -lseann -Wl,-rpath,'../../secubist/dist/Debug/GNU-Linux' -L../../secubist/dist/Debug/GNU-Linux -lsecubist -Wl,-rpath,'../../semysql/dist/Debug/GNU-Linux' -L../../semysql/dist/Debug/GNU-Linux -lsemysql -Wl,-rpath,'../../semath/dist/Debug/GNU-Linux' -L../../semath/dist/Debug/GNU-Linux -lsemath -Wl,-rpath,'../../sedata/dist/Debug/GNU-Linux' -L../../sedata/dist/Debug/GNU-Linux -lsedata -Wl,-rpath,'../../seglobal/dist/Debug/GNU-Linux' -L../../seglobal/dist/Debug/GNU-Linux -lseglobal -lmariadb -lm -lz -lrt -lcppunit -ljsoncpp -lpthread

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/seunittest

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/seunittest: ../../semain/dist/Debug/GNU-Linux/libsemain.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/seunittest: ../../external/mariadbpp/dist/Debug/GNU-Linux/libmariadbpp.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/seunittest: ../../seadmin/dist/Debug/GNU-Linux/libseadmin.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/seunittest: ../../seeconomy/dist/Debug/GNU-Linux/libseeconomy.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/seunittest: ../../sesecurities/dist/Debug/GNU-Linux/libsesecurities.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/seunittest: ../../seeoddata/dist/Debug/GNU-Linux/libseeoddata.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/seunittest: ../../sexbrl/dist/Debug/GNU-Linux/libsexbrl.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/seunittest: ../../sepredict/dist/Debug/GNU-Linux/libsepredict.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/seunittest: ../../setraining/dist/Debug/GNU-Linux/libsetraining.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/seunittest: ../../senet/dist/Debug/GNU-Linux/libsenet.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/seunittest: ../../seann/dist/Debug/GNU-Linux/libseann.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/seunittest: ../../secubist/dist/Debug/GNU-Linux/libsecubist.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/seunittest: ../../semysql/dist/Debug/GNU-Linux/libsemysql.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/seunittest: ../../semath/dist/Debug/GNU-Linux/libsemath.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/seunittest: ../../sedata/dist/Debug/GNU-Linux/libsedata.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/seunittest: ../../seglobal/dist/Debug/GNU-Linux/libseglobal.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/seunittest: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/seunittest ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/CAdminAPITest.o: CAdminAPITest.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mariadb -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CAdminAPITest.o CAdminAPITest.cpp

${OBJECTDIR}/CCryptoTest.o: CCryptoTest.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mariadb -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CCryptoTest.o CCryptoTest.cpp

${OBJECTDIR}/CEmailTemplateTest.o: CEmailTemplateTest.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mariadb -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CEmailTemplateTest.o CEmailTemplateTest.cpp

${OBJECTDIR}/CEodDataTest.o: CEodDataTest.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mariadb -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CEodDataTest.o CEodDataTest.cpp

${OBJECTDIR}/CExceptionTest.o: CExceptionTest.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mariadb -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CExceptionTest.o CExceptionTest.cpp

${OBJECTDIR}/CHelpDocTreeTest.o: CHelpDocTreeTest.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mariadb -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CHelpDocTreeTest.o CHelpDocTreeTest.cpp

${OBJECTDIR}/CMonteCarloTests.o: CMonteCarloTests.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mariadb -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CMonteCarloTests.o CMonteCarloTests.cpp

${OBJECTDIR}/CPredictionsTest.o: CPredictionsTest.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mariadb -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CPredictionsTest.o CPredictionsTest.cpp

${OBJECTDIR}/CSecurityAPITest.o: CSecurityAPITest.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mariadb -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CSecurityAPITest.o CSecurityAPITest.cpp

${OBJECTDIR}/CSqlAstTest.o: CSqlAstTest.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mariadb -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CSqlAstTest.o CSqlAstTest.cpp

${OBJECTDIR}/CUnitTestBase.o: CUnitTestBase.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mariadb -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CUnitTestBase.o CUnitTestBase.cpp

${OBJECTDIR}/CUtilityTest.o: CUtilityTest.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mariadb -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CUtilityTest.o CUtilityTest.cpp

${OBJECTDIR}/CXbrlProcessingTests.o: CXbrlProcessingTests.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mariadb -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CXbrlProcessingTests.o CXbrlProcessingTests.cpp

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mariadb -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:
	cd ../../semain && ${MAKE}  -f Makefile CONF=Debug
	cd ../../external/mariadbpp && ${MAKE}  -f Makefile CONF=Debug
	cd ../../seadmin && ${MAKE}  -f Makefile CONF=Debug
	cd ../../seeconomy && ${MAKE}  -f Makefile CONF=Debug
	cd ../../sesecurities && ${MAKE}  -f Makefile CONF=Debug
	cd ../../seeoddata && ${MAKE}  -f Makefile CONF=Debug
	cd ../../sexbrl && ${MAKE}  -f Makefile CONF=Debug
	cd ../../sepredict && ${MAKE} CND_SOLUTIONDIR=/home/santony/Documents/smartearn/Linux/phases/phase3/lib -f Makefile CONF=Debug
	cd ../../setraining && ${MAKE}  -f Makefile CONF=Debug
	cd ../../senet && ${MAKE}  -f Makefile CONF=Debug
	cd ../../seann && ${MAKE}  -f Makefile CONF=Debug
	cd ../../secubist && ${MAKE}  -f Makefile CONF=Debug
	cd ../../semysql && ${MAKE}  -f Makefile CONF=Debug
	cd ../../semath && ${MAKE}  -f Makefile CONF=Debug
	cd ../../sedata && ${MAKE}  -f Makefile CONF=Debug
	cd ../../seglobal && ${MAKE}  -f Makefile CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} -r ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemain.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmariadbpp.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseadmin.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeconomy.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsesecurities.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeoddata.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsexbrl.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsepredict.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsetraining.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsenet.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseann.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsecubist.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemysql.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemath.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsedata.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseglobal.so
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/seunittest

# Subprojects
.clean-subprojects:
	cd ../../semain && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../../external/mariadbpp && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../../seadmin && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../../seeconomy && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../../sesecurities && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../../seeoddata && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../../sexbrl && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../../sepredict && ${MAKE} CND_SOLUTIONDIR=/home/santony/Documents/smartearn/Linux/phases/phase3/lib -f Makefile CONF=Debug clean
	cd ../../setraining && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../../senet && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../../seann && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../../secubist && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../../semysql && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../../semath && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../../sedata && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../../seglobal && ${MAKE}  -f Makefile CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
