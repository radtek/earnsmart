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
	${OBJECTDIR}/CEventsManager.o \
	${OBJECTDIR}/CFunctionHandler.o \
	${OBJECTDIR}/CMapAllSymbolsToCIK.o \
	${OBJECTDIR}/CParseFormIndexFunctionJson.o \
	${OBJECTDIR}/CPriceHistoryProcessor.o \
	${OBJECTDIR}/CProcessEodData.o \
	${OBJECTDIR}/CProcessStatements.o \
	${OBJECTDIR}/CRSSReaderBase.o \
	${OBJECTDIR}/CRepeatedDBAction.o \
	${OBJECTDIR}/CSecurityPriceProcessor.o \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=std=c++0x

# CC Compiler Flags
CCFLAGS=-pipe
CXXFLAGS=-pipe

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L/usr/lib/x86_64-linux-gnu -Wl,-rpath,'./' -L../semain/dist/Debug/GNU-Linux -lsemain -L../seadmin/dist/Debug/GNU-Linux -lseadmin -L../sexbrl/dist/Debug/GNU-Linux -lsexbrl -L../seeoddata/dist/Debug/GNU-Linux -lseeoddata -L../sesecurities/dist/Debug/GNU-Linux -lsesecurities -L../seeconomy/dist/Debug/GNU-Linux -lseeconomy -L../setraining/dist/Debug/GNU-Linux -lsetraining -L../seann/dist/Debug/GNU-Linux -lseann -L../secubist/dist/Debug/GNU-Linux -lsecubist -L../senet/dist/Debug/GNU-Linux -lsenet -L../semysql/dist/Debug/GNU-Linux -lsemysql -L../sedata/dist/Debug/GNU-Linux -lsedata -L../semath/dist/Debug/GNU-Linux -lsemath -L../seglobal/dist/Debug/GNU-Linux -lseglobal -L../external/mariadbpp/dist/Debug/GNU-Linux -lmariadbpp -lboost_filesystem -lboost_system -lZThread -lzmq -lxml2 -ljsoncpp -lboost_regex -lboost_date_time -lboost_locale -lPocoFoundation -lcurl -ltar -lxml++-2.6 -lmariadb

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd
	${CP} ../semain/dist/Debug/GNU-Linux/libsemain.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${CP} ../seadmin/dist/Debug/GNU-Linux/libseadmin.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${CP} ../sexbrl/dist/Debug/GNU-Linux/libsexbrl.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${CP} ../seeoddata/dist/Debug/GNU-Linux/libseeoddata.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${CP} ../sesecurities/dist/Debug/GNU-Linux/libsesecurities.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${CP} ../seeconomy/dist/Debug/GNU-Linux/libseeconomy.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${CP} ../setraining/dist/Debug/GNU-Linux/libsetraining.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${CP} ../seann/dist/Debug/GNU-Linux/libseann.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${CP} ../secubist/dist/Debug/GNU-Linux/libsecubist.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${CP} ../senet/dist/Debug/GNU-Linux/libsenet.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${CP} ../semysql/dist/Debug/GNU-Linux/libsemysql.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${CP} ../sedata/dist/Debug/GNU-Linux/libsedata.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${CP} ../semath/dist/Debug/GNU-Linux/libsemath.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${CP} ../seglobal/dist/Debug/GNU-Linux/libseglobal.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${CP} ../external/mariadbpp/dist/Debug/GNU-Linux/libmariadbpp.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd: ../semain/dist/Debug/GNU-Linux/libsemain.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd: ../seadmin/dist/Debug/GNU-Linux/libseadmin.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd: ../sexbrl/dist/Debug/GNU-Linux/libsexbrl.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd: ../seeoddata/dist/Debug/GNU-Linux/libseeoddata.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd: ../sesecurities/dist/Debug/GNU-Linux/libsesecurities.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd: ../seeconomy/dist/Debug/GNU-Linux/libseeconomy.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd: ../setraining/dist/Debug/GNU-Linux/libsetraining.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd: ../seann/dist/Debug/GNU-Linux/libseann.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd: ../secubist/dist/Debug/GNU-Linux/libsecubist.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd: ../senet/dist/Debug/GNU-Linux/libsenet.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd: ../semysql/dist/Debug/GNU-Linux/libsemysql.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd: ../sedata/dist/Debug/GNU-Linux/libsedata.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd: ../semath/dist/Debug/GNU-Linux/libsemath.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd: ../seglobal/dist/Debug/GNU-Linux/libseglobal.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd: ../external/mariadbpp/dist/Debug/GNU-Linux/libmariadbpp.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	g++ -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/CEventsManager.o: CEventsManager.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/mariadb -I/usr/local/include -I/usr/include/libxml2 -I/usr/include/zthread -I../external/mariadbpp -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CEventsManager.o CEventsManager.cpp

${OBJECTDIR}/CFunctionHandler.o: CFunctionHandler.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/mariadb -I/usr/local/include -I/usr/include/libxml2 -I/usr/include/zthread -I../external/mariadbpp -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CFunctionHandler.o CFunctionHandler.cpp

${OBJECTDIR}/CMapAllSymbolsToCIK.o: CMapAllSymbolsToCIK.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/mariadb -I/usr/local/include -I/usr/include/libxml2 -I/usr/include/zthread -I../external/mariadbpp -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CMapAllSymbolsToCIK.o CMapAllSymbolsToCIK.cpp

${OBJECTDIR}/CParseFormIndexFunctionJson.o: CParseFormIndexFunctionJson.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/mariadb -I/usr/local/include -I/usr/include/libxml2 -I/usr/include/zthread -I../external/mariadbpp -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CParseFormIndexFunctionJson.o CParseFormIndexFunctionJson.cpp

${OBJECTDIR}/CPriceHistoryProcessor.o: CPriceHistoryProcessor.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/mariadb -I/usr/local/include -I/usr/include/libxml2 -I/usr/include/zthread -I../external/mariadbpp -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CPriceHistoryProcessor.o CPriceHistoryProcessor.cpp

${OBJECTDIR}/CProcessEodData.o: CProcessEodData.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/mariadb -I/usr/local/include -I/usr/include/libxml2 -I/usr/include/zthread -I../external/mariadbpp -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CProcessEodData.o CProcessEodData.cpp

${OBJECTDIR}/CProcessStatements.o: CProcessStatements.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/mariadb -I/usr/local/include -I/usr/include/libxml2 -I/usr/include/zthread -I../external/mariadbpp -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CProcessStatements.o CProcessStatements.cpp

${OBJECTDIR}/CRSSReaderBase.o: CRSSReaderBase.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/mariadb -I/usr/local/include -I/usr/include/libxml2 -I/usr/include/zthread -I../external/mariadbpp -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CRSSReaderBase.o CRSSReaderBase.cpp

${OBJECTDIR}/CRepeatedDBAction.o: CRepeatedDBAction.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/mariadb -I/usr/local/include -I/usr/include/libxml2 -I/usr/include/zthread -I../external/mariadbpp -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CRepeatedDBAction.o CRepeatedDBAction.cpp

${OBJECTDIR}/CSecurityPriceProcessor.o: CSecurityPriceProcessor.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/mariadb -I/usr/local/include -I/usr/include/libxml2 -I/usr/include/zthread -I../external/mariadbpp -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CSecurityPriceProcessor.o CSecurityPriceProcessor.cpp

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/mariadb -I/usr/local/include -I/usr/include/libxml2 -I/usr/include/zthread -I../external/mariadbpp -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:
	cd ../semain && ${MAKE}  -f Makefile CONF=Debug
	cd ../seadmin && ${MAKE}  -f Makefile CONF=Debug
	cd ../sexbrl && ${MAKE}  -f Makefile CONF=Debug
	cd ../seeoddata && ${MAKE}  -f Makefile CONF=Debug
	cd ../sesecurities && ${MAKE}  -f Makefile CONF=Debug
	cd ../seeconomy && ${MAKE}  -f Makefile CONF=Debug
	cd ../setraining && ${MAKE}  -f Makefile CONF=Debug
	cd ../seann && ${MAKE}  -f Makefile CONF=Debug
	cd ../secubist && ${MAKE}  -f Makefile CONF=Debug
	cd ../senet && ${MAKE}  -f Makefile CONF=Debug
	cd ../semysql && ${MAKE}  -f Makefile CONF=Debug
	cd ../sedata && ${MAKE}  -f Makefile CONF=Debug
	cd ../semath && ${MAKE}  -f Makefile CONF=Debug
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Debug
	cd ../external/mariadbpp && ${MAKE}  -f Makefile CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} -r ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemain.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseadmin.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsexbrl.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeoddata.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsesecurities.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeconomy.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsetraining.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseann.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsecubist.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsenet.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemysql.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsedata.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemath.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseglobal.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmariadbpp.so
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd

# Subprojects
.clean-subprojects:
	cd ../semain && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../seadmin && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../sexbrl && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../seeoddata && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../sesecurities && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../seeconomy && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../setraining && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../seann && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../secubist && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../senet && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../semysql && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../sedata && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../semath && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../external/mariadbpp && ${MAKE}  -f Makefile CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
