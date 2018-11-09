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
LDLIBSOPTIONS=-L/usr/lib/x86_64-linux-gnu -Wl,-rpath,'../sepredict/dist/Debug/GNU-Linux' -L../sepredict/dist/Debug/GNU-Linux -lsepredict -Wl,-rpath,'../semain/dist/Debug/GNU-Linux' -L../semain/dist/Debug/GNU-Linux -lsemain -Wl,-rpath,'../sesecurities/dist/Debug/GNU-Linux' -L../sesecurities/dist/Debug/GNU-Linux -lsesecurities -Wl,-rpath,'../sexbrl/dist/Debug/GNU-Linux' -L../sexbrl/dist/Debug/GNU-Linux -lsexbrl -Wl,-rpath,'../setraining/dist/Debug/GNU-Linux' -L../setraining/dist/Debug/GNU-Linux -lsetraining -Wl,-rpath,'../seeconomy/dist/Debug/GNU-Linux' -L../seeconomy/dist/Debug/GNU-Linux -lseeconomy ../semysql/dist/Debug/GNU-Linux/libsemysql.a -Wl,-rpath,'../senet/dist/Debug/GNU-Linux' -L../senet/dist/Debug/GNU-Linux -lsenet ../semath/dist/Debug/GNU-Linux/libsemath.a -Wl,-rpath,'../secubist/dist/Debug/GNU-Linux' -L../secubist/dist/Debug/GNU-Linux -lsecubist -Wl,-rpath,'../seann/dist/Debug/GNU-Linux' -L../seann/dist/Debug/GNU-Linux -lseann ../seglobal/dist/Debug/GNU-Linux/libseglobal.a -Wl,-rpath,'../external/mariadbpp/dist/Debug/GNU-Linux' -L../external/mariadbpp/dist/Debug/GNU-Linux -lmariadbpp -Wl,-rpath,'../seeoddata/dist/Debug/GNU-Linux' -L../seeoddata/dist/Debug/GNU-Linux -lseeoddata -lcurl -ljsoncpp -lboost_date_time -lboost_regex -lgsl -lZThread -lPocoFoundation -lgsl -lgslcblas -lm -lbooster -lmariadbclient_r -lmongocxx -lpthread

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sesecurities_tests

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sesecurities_tests: ../sepredict/dist/Debug/GNU-Linux/libsepredict.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sesecurities_tests: ../semain/dist/Debug/GNU-Linux/libsemain.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sesecurities_tests: ../sesecurities/dist/Debug/GNU-Linux/libsesecurities.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sesecurities_tests: ../sexbrl/dist/Debug/GNU-Linux/libsexbrl.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sesecurities_tests: ../setraining/dist/Debug/GNU-Linux/libsetraining.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sesecurities_tests: ../seeconomy/dist/Debug/GNU-Linux/libseeconomy.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sesecurities_tests: ../semysql/dist/Debug/GNU-Linux/libsemysql.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sesecurities_tests: ../senet/dist/Debug/GNU-Linux/libsenet.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sesecurities_tests: ../semath/dist/Debug/GNU-Linux/libsemath.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sesecurities_tests: ../secubist/dist/Debug/GNU-Linux/libsecubist.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sesecurities_tests: ../seann/dist/Debug/GNU-Linux/libseann.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sesecurities_tests: ../seglobal/dist/Debug/GNU-Linux/libseglobal.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sesecurities_tests: ../external/mariadbpp/dist/Debug/GNU-Linux/libmariadbpp.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sesecurities_tests: ../seeoddata/dist/Debug/GNU-Linux/libseeoddata.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sesecurities_tests: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	g++ -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sesecurities_tests ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include/mysql -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:
	cd ../sepredict && ${MAKE}  -f Makefile CONF=Debug
	cd ../semain && ${MAKE}  -f Makefile CONF=Debug
	cd ../sesecurities && ${MAKE}  -f Makefile CONF=Debug
	cd ../sexbrl && ${MAKE}  -f Makefile CONF=Debug
	cd ../setraining && ${MAKE}  -f Makefile CONF=Debug
	cd ../seeconomy && ${MAKE}  -f Makefile CONF=Debug
	cd ../semysql && ${MAKE}  -f Makefile CONF=Debug
	cd ../senet && ${MAKE}  -f Makefile CONF=Debug
	cd ../semath && ${MAKE}  -f Makefile CONF=Debug
	cd ../secubist && ${MAKE}  -f Makefile CONF=Debug
	cd ../seann && ${MAKE}  -f Makefile CONF=Debug
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Debug
	cd ../external/mariadbpp && ${MAKE}  -f Makefile CONF=Debug
	cd ../seeoddata && ${MAKE}  -f Makefile CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} -r ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsepredict.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemain.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsesecurities.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsexbrl.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsetraining.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeconomy.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsenet.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsecubist.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseann.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmariadbpp.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeoddata.so
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sesecurities_tests

# Subprojects
.clean-subprojects:
	cd ../sepredict && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../semain && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../sesecurities && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../sexbrl && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../setraining && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../seeconomy && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../semysql && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../senet && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../semath && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../secubist && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../seann && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../external/mariadbpp && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../seeoddata && ${MAKE}  -f Makefile CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
