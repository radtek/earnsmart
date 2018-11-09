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
LDLIBSOPTIONS=-L/usr/local/lib -L/usr/lib/x86_64-linux-gnu -Wl,-rpath,../seeconomy/dist/Debug/GNU-Linux-x86 -L../seeconomy/dist/Debug/GNU-Linux-x86 -lseeconomy -Wl,-rpath,../sexbrl/dist/Debug/GNU-Linux-x86 -L../sexbrl/dist/Debug/GNU-Linux-x86 -lsexbrl -Wl,-rpath,../sesecurities/dist/Debug/GNU-Linux-x86 -L../sesecurities/dist/Debug/GNU-Linux-x86 -lsesecurities -Wl,-rpath,../senet/dist/Debug/GNU-Linux-x86 -L../senet/dist/Debug/GNU-Linux-x86 -lsenet ../semysql/dist/Debug/GNU-Linux-x86/libsemysql.a -Wl,-rpath,../semain/dist/Debug/GNU-Linux-x86 -L../semain/dist/Debug/GNU-Linux-x86 -lsemain -Wl,-rpath,../setraining/dist/Debug/GNU-Linux-x86 -L../setraining/dist/Debug/GNU-Linux-x86 -lsetraining -Wl,-rpath,../seann/dist/Debug/GNU-Linux-x86 -L../seann/dist/Debug/GNU-Linux-x86 -lseann ../semath/dist/Debug/GNU-Linux-x86/libsemath.a -Wl,-rpath,../secubist/dist/Debug/GNU-Linux-x86 -L../secubist/dist/Debug/GNU-Linux-x86 -lsecubist ../seglobal/dist/Debug/GNU-Linux-x86/libseglobal.a -Wl,-rpath,../external/mariadbpp/dist/Debug/GNU-Linux-x86 -L../external/mariadbpp/dist/Debug/GNU-Linux-x86 -lmariadbpp -lxml2 -lboost_system -lboost_filesystem -lboost_iostreams -lcurl -ltar -lboost_regex -ljsoncpp -lboost_date_time -lZThread -lrt -lPocoFoundation -lmrss -lmariadbclient_r

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sexbrl_tests

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sexbrl_tests: ../seeconomy/dist/Debug/GNU-Linux-x86/libseeconomy.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sexbrl_tests: ../sexbrl/dist/Debug/GNU-Linux-x86/libsexbrl.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sexbrl_tests: ../sesecurities/dist/Debug/GNU-Linux-x86/libsesecurities.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sexbrl_tests: ../senet/dist/Debug/GNU-Linux-x86/libsenet.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sexbrl_tests: ../semysql/dist/Debug/GNU-Linux-x86/libsemysql.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sexbrl_tests: ../semain/dist/Debug/GNU-Linux-x86/libsemain.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sexbrl_tests: ../setraining/dist/Debug/GNU-Linux-x86/libsetraining.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sexbrl_tests: ../seann/dist/Debug/GNU-Linux-x86/libseann.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sexbrl_tests: ../semath/dist/Debug/GNU-Linux-x86/libsemath.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sexbrl_tests: ../secubist/dist/Debug/GNU-Linux-x86/libsecubist.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sexbrl_tests: ../seglobal/dist/Debug/GNU-Linux-x86/libseglobal.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sexbrl_tests: ../external/mariadbpp/dist/Debug/GNU-Linux-x86/libmariadbpp.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sexbrl_tests: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sexbrl_tests ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/CCursesConsole.o: CCursesConsole.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I${CND_BASEDIR}/../sexbrl/ -I/usr/include/mysql -I/usr/include/libxml2 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CCursesConsole.o CCursesConsole.cpp

${OBJECTDIR}/CXbrlListener.o: CXbrlListener.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I${CND_BASEDIR}/../sexbrl/ -I/usr/include/mysql -I/usr/include/libxml2 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CXbrlListener.o CXbrlListener.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I${CND_BASEDIR}/../sexbrl/ -I/usr/include/mysql -I/usr/include/libxml2 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/xbrldatawork.o: xbrldatawork.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I${CND_BASEDIR}/../sexbrl/ -I/usr/include/mysql -I/usr/include/libxml2 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/xbrldatawork.o xbrldatawork.cpp

# Subprojects
.build-subprojects:
	cd ../seeconomy && ${MAKE}  -f Makefile CONF=Debug
	cd ../sexbrl && ${MAKE}  -f Makefile CONF=Debug
	cd ../sesecurities && ${MAKE}  -f Makefile CONF=Debug
	cd ../senet && ${MAKE}  -f Makefile CONF=Debug
	cd ../semysql && ${MAKE}  -f Makefile CONF=Debug
	cd ../semain && ${MAKE}  -f Makefile CONF=Debug
	cd ../setraining && ${MAKE}  -f Makefile CONF=Debug
	cd ../seann && ${MAKE}  -f Makefile CONF=Debug
	cd ../semath && ${MAKE}  -f Makefile CONF=Debug
	cd ../secubist && ${MAKE}  -f Makefile CONF=Debug
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Debug
	cd ../external/mariadbpp && ${MAKE}  -f Makefile CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sexbrl_tests

# Subprojects
.clean-subprojects:
	cd ../seeconomy && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../sexbrl && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../sesecurities && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../senet && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../semysql && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../semain && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../setraining && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../seann && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../semath && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../secubist && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../external/mariadbpp && ${MAKE}  -f Makefile CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
