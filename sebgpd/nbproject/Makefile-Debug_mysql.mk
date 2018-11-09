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
LDLIBSOPTIONS=-L/usr/lib/x86_64-linux-gnu -Wl,-rpath,'./' -L../semain/dist/Debug_mysql/GNU-Linux -lsemain -L../seadmin/dist/Debug_mysql/GNU-Linux -lseadmin -L../sexbrl/dist/Debug_mysql/GNU-Linux -lsexbrl -L../seeoddata/dist/Debug_mysql/GNU-Linux -lseeoddata -L../sesecurities/dist/Debug_mysql/GNU-Linux -lsesecurities -L../seeconomy/dist/Debug_mysql/GNU-Linux -lseeconomy -L../setraining/dist/Debug_mysql/GNU-Linux -lsetraining -L../seann/dist/Debug_mysql/GNU-Linux -lseann -L../secubist/dist/Debug/GNU-Linux -lsecubist -L../senet/dist/Debug_mysql/GNU-Linux -lsenet -L../semysql/dist/Debug_mysql/GNU-Linux -lsemysql -L../sedata/dist/Debug/GNU-Linux -lsedata -L../semath/dist/Debug_mysql/GNU-Linux -lsemath -L../seglobal/dist/Debug/GNU-Linux -lseglobal -L../external/mariadbpp/dist/Debug_mysql/GNU-Linux -lmariadbpp -lboost_filesystem -lboost_system -lZThread -lzmq -lxml2 -ljsoncpp -lboost_regex -lboost_date_time -lboost_locale -lPocoFoundation -lmysqlclient -lcurl -ltar -lxml++-2.6

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd
	${CP} ../semain/dist/Debug_mysql/GNU-Linux/libsemain.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${CP} ../seadmin/dist/Debug_mysql/GNU-Linux/libseadmin.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${CP} ../sexbrl/dist/Debug_mysql/GNU-Linux/libsexbrl.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${CP} ../seeoddata/dist/Debug_mysql/GNU-Linux/libseeoddata.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${CP} ../sesecurities/dist/Debug_mysql/GNU-Linux/libsesecurities.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${CP} ../seeconomy/dist/Debug_mysql/GNU-Linux/libseeconomy.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${CP} ../setraining/dist/Debug_mysql/GNU-Linux/libsetraining.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${CP} ../seann/dist/Debug_mysql/GNU-Linux/libseann.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${CP} ../secubist/dist/Debug/GNU-Linux/libsecubist.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${CP} ../senet/dist/Debug_mysql/GNU-Linux/libsenet.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${CP} ../semysql/dist/Debug_mysql/GNU-Linux/libsemysql.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${CP} ../sedata/dist/Debug/GNU-Linux/libsedata.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${CP} ../semath/dist/Debug_mysql/GNU-Linux/libsemath.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${CP} ../seglobal/dist/Debug/GNU-Linux/libseglobal.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${CP} ../external/mariadbpp/dist/Debug_mysql/GNU-Linux/libmariadbpp.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd: ../semain/dist/Debug_mysql/GNU-Linux/libsemain.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd: ../seadmin/dist/Debug_mysql/GNU-Linux/libseadmin.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd: ../sexbrl/dist/Debug_mysql/GNU-Linux/libsexbrl.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd: ../seeoddata/dist/Debug_mysql/GNU-Linux/libseeoddata.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd: ../sesecurities/dist/Debug_mysql/GNU-Linux/libsesecurities.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd: ../seeconomy/dist/Debug_mysql/GNU-Linux/libseeconomy.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd: ../setraining/dist/Debug_mysql/GNU-Linux/libsetraining.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd: ../seann/dist/Debug_mysql/GNU-Linux/libseann.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd: ../secubist/dist/Debug/GNU-Linux/libsecubist.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd: ../senet/dist/Debug_mysql/GNU-Linux/libsenet.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd: ../semysql/dist/Debug_mysql/GNU-Linux/libsemysql.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd: ../sedata/dist/Debug/GNU-Linux/libsedata.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd: ../semath/dist/Debug_mysql/GNU-Linux/libsemath.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd: ../seglobal/dist/Debug/GNU-Linux/libseglobal.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd: ../external/mariadbpp/dist/Debug_mysql/GNU-Linux/libmariadbpp.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	g++ -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/CEventsManager.o: CEventsManager.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/mysql -I/usr/local/include -I/usr/include/libxml2 -I/usr/include/zthread -I../external/mariadbpp -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CEventsManager.o CEventsManager.cpp

${OBJECTDIR}/CFunctionHandler.o: CFunctionHandler.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/mysql -I/usr/local/include -I/usr/include/libxml2 -I/usr/include/zthread -I../external/mariadbpp -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CFunctionHandler.o CFunctionHandler.cpp

${OBJECTDIR}/CMapAllSymbolsToCIK.o: CMapAllSymbolsToCIK.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/mysql -I/usr/local/include -I/usr/include/libxml2 -I/usr/include/zthread -I../external/mariadbpp -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CMapAllSymbolsToCIK.o CMapAllSymbolsToCIK.cpp

${OBJECTDIR}/CParseFormIndexFunctionJson.o: CParseFormIndexFunctionJson.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/mysql -I/usr/local/include -I/usr/include/libxml2 -I/usr/include/zthread -I../external/mariadbpp -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CParseFormIndexFunctionJson.o CParseFormIndexFunctionJson.cpp

${OBJECTDIR}/CPriceHistoryProcessor.o: CPriceHistoryProcessor.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/mysql -I/usr/local/include -I/usr/include/libxml2 -I/usr/include/zthread -I../external/mariadbpp -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CPriceHistoryProcessor.o CPriceHistoryProcessor.cpp

${OBJECTDIR}/CProcessEodData.o: CProcessEodData.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/mysql -I/usr/local/include -I/usr/include/libxml2 -I/usr/include/zthread -I../external/mariadbpp -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CProcessEodData.o CProcessEodData.cpp

${OBJECTDIR}/CProcessStatements.o: CProcessStatements.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/mysql -I/usr/local/include -I/usr/include/libxml2 -I/usr/include/zthread -I../external/mariadbpp -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CProcessStatements.o CProcessStatements.cpp

${OBJECTDIR}/CRSSReaderBase.o: CRSSReaderBase.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/mysql -I/usr/local/include -I/usr/include/libxml2 -I/usr/include/zthread -I../external/mariadbpp -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CRSSReaderBase.o CRSSReaderBase.cpp

${OBJECTDIR}/CRepeatedDBAction.o: CRepeatedDBAction.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/mysql -I/usr/local/include -I/usr/include/libxml2 -I/usr/include/zthread -I../external/mariadbpp -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CRepeatedDBAction.o CRepeatedDBAction.cpp

${OBJECTDIR}/CSecurityPriceProcessor.o: CSecurityPriceProcessor.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/mysql -I/usr/local/include -I/usr/include/libxml2 -I/usr/include/zthread -I../external/mariadbpp -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CSecurityPriceProcessor.o CSecurityPriceProcessor.cpp

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/mysql -I/usr/local/include -I/usr/include/libxml2 -I/usr/include/zthread -I../external/mariadbpp -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:
	cd ../semain && ${MAKE}  -f Makefile CONF=Debug_mysql
	cd ../seadmin && ${MAKE}  -f Makefile CONF=Debug_mysql
	cd ../sexbrl && ${MAKE}  -f Makefile CONF=Debug_mysql
	cd ../seeoddata && ${MAKE}  -f Makefile CONF=Debug_mysql
	cd ../sesecurities && ${MAKE}  -f Makefile CONF=Debug_mysql
	cd ../seeconomy && ${MAKE}  -f Makefile CONF=Debug_mysql
	cd ../setraining && ${MAKE}  -f Makefile CONF=Debug_mysql
	cd ../seann && ${MAKE}  -f Makefile CONF=Debug_mysql
	cd ../secubist && ${MAKE}  -f Makefile CONF=Debug
	cd ../senet && ${MAKE}  -f Makefile CONF=Debug_mysql
	cd ../semysql && ${MAKE}  -f Makefile CONF=Debug_mysql
	cd ../sedata && ${MAKE}  -f Makefile CONF=Debug
	cd ../semath && ${MAKE}  -f Makefile CONF=Debug_mysql
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Debug
	cd ../external/mariadbpp && ${MAKE}  -f Makefile CONF=Debug_mysql

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} -r ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemain.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseadmin.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsexbrl.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeoddata.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsesecurities.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeconomy.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsetraining.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseann.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsecubist.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsenet.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemysql.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsedata.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemath.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseglobal.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmariadbpp.so
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd

# Subprojects
.clean-subprojects:
	cd ../semain && ${MAKE}  -f Makefile CONF=Debug_mysql clean
	cd ../seadmin && ${MAKE}  -f Makefile CONF=Debug_mysql clean
	cd ../sexbrl && ${MAKE}  -f Makefile CONF=Debug_mysql clean
	cd ../seeoddata && ${MAKE}  -f Makefile CONF=Debug_mysql clean
	cd ../sesecurities && ${MAKE}  -f Makefile CONF=Debug_mysql clean
	cd ../seeconomy && ${MAKE}  -f Makefile CONF=Debug_mysql clean
	cd ../setraining && ${MAKE}  -f Makefile CONF=Debug_mysql clean
	cd ../seann && ${MAKE}  -f Makefile CONF=Debug_mysql clean
	cd ../secubist && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../senet && ${MAKE}  -f Makefile CONF=Debug_mysql clean
	cd ../semysql && ${MAKE}  -f Makefile CONF=Debug_mysql clean
	cd ../sedata && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../semath && ${MAKE}  -f Makefile CONF=Debug_mysql clean
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../external/mariadbpp && ${MAKE}  -f Makefile CONF=Debug_mysql clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
