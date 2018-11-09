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
	${OBJECTDIR}/CCursesConsole.o \
	${OBJECTDIR}/CXbrlListener.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/xbrldatawork.o


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
LDLIBSOPTIONS=-L/usr/local/lib -L/usr/lib/x86_64-linux-gnu -Wl,-rpath,../seeconomy/dist/Release/GNU-Linux-x86 -L../seeconomy/dist/Release/GNU-Linux-x86 -lseeconomy -Wl,-rpath,../sexbrl/dist/Release/GNU-Linux-x86 -L../sexbrl/dist/Release/GNU-Linux-x86 -lsexbrl -Wl,-rpath,../sesecurities/dist/Release/GNU-Linux-x86 -L../sesecurities/dist/Release/GNU-Linux-x86 -lsesecurities -Wl,-rpath,../senet/dist/Release/GNU-Linux-x86 -L../senet/dist/Release/GNU-Linux-x86 -lsenet ../semysql/dist/Release/GNU-Linux-x86/libsemysql.a -Wl,-rpath,../setraining/dist/Release/GNU-Linux-x86 -L../setraining/dist/Release/GNU-Linux-x86 -lsetraining -Wl,-rpath,../seann/dist/Release/GNU-Linux-x86 -L../seann/dist/Release/GNU-Linux-x86 -lseann -Wl,-rpath,../semain/dist/Release/GNU-Linux-x86 -L../semain/dist/Release/GNU-Linux-x86 -lsemain -Wl,-rpath,../secubist/dist/Release/GNU-Linux-x86 -L../secubist/dist/Release/GNU-Linux-x86 -lsecubist ../seglobal/dist/Release/GNU-Linux-x86/libseglobal.a -Wl,-rpath,../external/mariadbpp/dist/Release/GNU-Linux-x86 -L../external/mariadbpp/dist/Release/GNU-Linux-x86 -lmariadbpp -lxml2 -lboost_system -lboost_filesystem -lboost_iostreams -lcurl -ltar -lboost_regex -ljsoncpp -lboost_date_time -lZThread -lrt -lPocoFoundation ../semath/dist/Release/GNU-Linux-x86/libsemath.a -lmrss -lmariadbclient_r

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sexbrl_tests

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sexbrl_tests: ../seeconomy/dist/Release/GNU-Linux-x86/libseeconomy.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sexbrl_tests: ../sexbrl/dist/Release/GNU-Linux-x86/libsexbrl.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sexbrl_tests: ../sesecurities/dist/Release/GNU-Linux-x86/libsesecurities.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sexbrl_tests: ../senet/dist/Release/GNU-Linux-x86/libsenet.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sexbrl_tests: ../semysql/dist/Release/GNU-Linux-x86/libsemysql.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sexbrl_tests: ../setraining/dist/Release/GNU-Linux-x86/libsetraining.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sexbrl_tests: ../seann/dist/Release/GNU-Linux-x86/libseann.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sexbrl_tests: ../semain/dist/Release/GNU-Linux-x86/libsemain.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sexbrl_tests: ../secubist/dist/Release/GNU-Linux-x86/libsecubist.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sexbrl_tests: ../seglobal/dist/Release/GNU-Linux-x86/libseglobal.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sexbrl_tests: ../external/mariadbpp/dist/Release/GNU-Linux-x86/libmariadbpp.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sexbrl_tests: ../semath/dist/Release/GNU-Linux-x86/libsemath.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sexbrl_tests: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sexbrl_tests ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/CCursesConsole.o: CCursesConsole.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I${CND_BASEDIR}/../sexbrl/ -I/usr/include/mysql -I/usr/include/libxml2 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CCursesConsole.o CCursesConsole.cpp

${OBJECTDIR}/CXbrlListener.o: CXbrlListener.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I${CND_BASEDIR}/../sexbrl/ -I/usr/include/mysql -I/usr/include/libxml2 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CXbrlListener.o CXbrlListener.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I${CND_BASEDIR}/../sexbrl/ -I/usr/include/mysql -I/usr/include/libxml2 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/xbrldatawork.o: xbrldatawork.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I${CND_BASEDIR}/../sexbrl/ -I/usr/include/mysql -I/usr/include/libxml2 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/xbrldatawork.o xbrldatawork.cpp

# Subprojects
.build-subprojects:
	cd ../seeconomy && ${MAKE}  -f Makefile CONF=Release
	cd ../sexbrl && ${MAKE}  -f Makefile CONF=Release
	cd ../sesecurities && ${MAKE}  -f Makefile CONF=Release
	cd ../senet && ${MAKE}  -f Makefile CONF=Release
	cd ../semysql && ${MAKE}  -f Makefile CONF=Release
	cd ../setraining && ${MAKE}  -f Makefile CONF=Release
	cd ../seann && ${MAKE}  -f Makefile CONF=Release
	cd ../semain && ${MAKE}  -f Makefile CONF=Release
	cd ../secubist && ${MAKE}  -f Makefile CONF=Release
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Release
	cd ../external/mariadbpp && ${MAKE}  -f Makefile CONF=Release
	cd ../semath && ${MAKE}  -f Makefile CONF=Release

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sexbrl_tests

# Subprojects
.clean-subprojects:
	cd ../seeconomy && ${MAKE}  -f Makefile CONF=Release clean
	cd ../sexbrl && ${MAKE}  -f Makefile CONF=Release clean
	cd ../sesecurities && ${MAKE}  -f Makefile CONF=Release clean
	cd ../senet && ${MAKE}  -f Makefile CONF=Release clean
	cd ../semysql && ${MAKE}  -f Makefile CONF=Release clean
	cd ../setraining && ${MAKE}  -f Makefile CONF=Release clean
	cd ../seann && ${MAKE}  -f Makefile CONF=Release clean
	cd ../semain && ${MAKE}  -f Makefile CONF=Release clean
	cd ../secubist && ${MAKE}  -f Makefile CONF=Release clean
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Release clean
	cd ../external/mariadbpp && ${MAKE}  -f Makefile CONF=Release clean
	cd ../semath && ${MAKE}  -f Makefile CONF=Release clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
