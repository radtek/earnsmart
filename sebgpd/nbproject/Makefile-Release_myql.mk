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
CND_CONF=Release_myql
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
CFLAGS=

# CC Compiler Flags
CCFLAGS=-pipe
CXXFLAGS=-pipe

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L/usr/lib/x86_64-linux-gnu -Wl,-rpath,'./' -L../semain/dist/Release_mysql/GNU-Linux -lsemain -L../seadmin/dist/Release_mysql/GNU-Linux -lseadmin -L../seeoddata/dist/Release_mysql/GNU-Linux -lseeoddata -L../sesecurities/dist/Release_mysql/GNU-Linux -lsesecurities -L../seeconomy/dist/Release_mysql/GNU-Linux -lseeconomy -L../sepredict/dist/Release_mysql/GNU-Linux -lsepredict -L../seann/dist/Release_mysql/GNU-Linux -lseann -L../setraining/dist/Release_mysql/GNU-Linux -lsetraining -L../sexbrl/dist/Release_mysql/GNU-Linux -lsexbrl -L../senet/dist/Release_mysql/GNU-Linux -lsenet -L../semysql/dist/Release_mysql/GNU-Linux -lsemysql -L../semath/dist/Release_mysql/GNU-Linux -lsemath -L../secubist/dist/Release/GNU-Linux -lsecubist -L../sedata/dist/Release/GNU-Linux -lsedata -L../seglobal/dist/Release/GNU-Linux -lseglobal -L../external/mariadbpp/dist/Release_mysql/GNU-Linux -lmariadbpp -lmysqlclient -lcurl -lboost_filesystem -lboost_system -lZThread -lzmq -lxml2 -ljsoncpp -lboost_regex -lboost_date_time -lboost_locale -lPocoFoundation -luuid -ltar -lxml++-2.6

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd
	${CP} ../semain/dist/Release_mysql/GNU-Linux/libsemain.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${CP} ../seadmin/dist/Release_mysql/GNU-Linux/libseadmin.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${CP} ../seeoddata/dist/Release_mysql/GNU-Linux/libseeoddata.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${CP} ../sesecurities/dist/Release_mysql/GNU-Linux/libsesecurities.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${CP} ../seeconomy/dist/Release_mysql/GNU-Linux/libseeconomy.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${CP} ../sepredict/dist/Release_mysql/GNU-Linux/libsepredict.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${CP} ../seann/dist/Release_mysql/GNU-Linux/libseann.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${CP} ../setraining/dist/Release_mysql/GNU-Linux/libsetraining.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${CP} ../sexbrl/dist/Release_mysql/GNU-Linux/libsexbrl.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${CP} ../senet/dist/Release_mysql/GNU-Linux/libsenet.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${CP} ../semysql/dist/Release_mysql/GNU-Linux/libsemysql.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${CP} ../semath/dist/Release_mysql/GNU-Linux/libsemath.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${CP} ../secubist/dist/Release/GNU-Linux/libsecubist.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${CP} ../sedata/dist/Release/GNU-Linux/libsedata.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${CP} ../seglobal/dist/Release/GNU-Linux/libseglobal.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${CP} ../external/mariadbpp/dist/Release_mysql/GNU-Linux/libmariadbpp.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd: ../semain/dist/Release_mysql/GNU-Linux/libsemain.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd: ../seadmin/dist/Release_mysql/GNU-Linux/libseadmin.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd: ../seeoddata/dist/Release_mysql/GNU-Linux/libseeoddata.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd: ../sesecurities/dist/Release_mysql/GNU-Linux/libsesecurities.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd: ../seeconomy/dist/Release_mysql/GNU-Linux/libseeconomy.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd: ../sepredict/dist/Release_mysql/GNU-Linux/libsepredict.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd: ../seann/dist/Release_mysql/GNU-Linux/libseann.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd: ../setraining/dist/Release_mysql/GNU-Linux/libsetraining.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd: ../sexbrl/dist/Release_mysql/GNU-Linux/libsexbrl.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd: ../senet/dist/Release_mysql/GNU-Linux/libsenet.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd: ../semysql/dist/Release_mysql/GNU-Linux/libsemysql.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd: ../semath/dist/Release_mysql/GNU-Linux/libsemath.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd: ../secubist/dist/Release/GNU-Linux/libsecubist.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd: ../sedata/dist/Release/GNU-Linux/libsedata.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd: ../seglobal/dist/Release/GNU-Linux/libseglobal.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd: ../external/mariadbpp/dist/Release_mysql/GNU-Linux/libmariadbpp.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/CEventsManager.o: CEventsManager.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DELPP_DISABLE_LOGS -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/mysql -I/usr/local/include -I/usr/include/libxml2 -I/usr/include/zthread -I../external/mariadbpp -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CEventsManager.o CEventsManager.cpp

${OBJECTDIR}/CFunctionHandler.o: CFunctionHandler.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DELPP_DISABLE_LOGS -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/mysql -I/usr/local/include -I/usr/include/libxml2 -I/usr/include/zthread -I../external/mariadbpp -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CFunctionHandler.o CFunctionHandler.cpp

${OBJECTDIR}/CMapAllSymbolsToCIK.o: CMapAllSymbolsToCIK.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DELPP_DISABLE_LOGS -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/mysql -I/usr/local/include -I/usr/include/libxml2 -I/usr/include/zthread -I../external/mariadbpp -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CMapAllSymbolsToCIK.o CMapAllSymbolsToCIK.cpp

${OBJECTDIR}/CParseFormIndexFunctionJson.o: CParseFormIndexFunctionJson.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DELPP_DISABLE_LOGS -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/mysql -I/usr/local/include -I/usr/include/libxml2 -I/usr/include/zthread -I../external/mariadbpp -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CParseFormIndexFunctionJson.o CParseFormIndexFunctionJson.cpp

${OBJECTDIR}/CPriceHistoryProcessor.o: CPriceHistoryProcessor.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DELPP_DISABLE_LOGS -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/mysql -I/usr/local/include -I/usr/include/libxml2 -I/usr/include/zthread -I../external/mariadbpp -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CPriceHistoryProcessor.o CPriceHistoryProcessor.cpp

${OBJECTDIR}/CProcessEodData.o: CProcessEodData.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DELPP_DISABLE_LOGS -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/mysql -I/usr/local/include -I/usr/include/libxml2 -I/usr/include/zthread -I../external/mariadbpp -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CProcessEodData.o CProcessEodData.cpp

${OBJECTDIR}/CProcessStatements.o: CProcessStatements.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DELPP_DISABLE_LOGS -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/mysql -I/usr/local/include -I/usr/include/libxml2 -I/usr/include/zthread -I../external/mariadbpp -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CProcessStatements.o CProcessStatements.cpp

${OBJECTDIR}/CRSSReaderBase.o: CRSSReaderBase.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DELPP_DISABLE_LOGS -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/mysql -I/usr/local/include -I/usr/include/libxml2 -I/usr/include/zthread -I../external/mariadbpp -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CRSSReaderBase.o CRSSReaderBase.cpp

${OBJECTDIR}/CRepeatedDBAction.o: CRepeatedDBAction.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DELPP_DISABLE_LOGS -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/mysql -I/usr/local/include -I/usr/include/libxml2 -I/usr/include/zthread -I../external/mariadbpp -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CRepeatedDBAction.o CRepeatedDBAction.cpp

${OBJECTDIR}/CSecurityPriceProcessor.o: CSecurityPriceProcessor.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DELPP_DISABLE_LOGS -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/mysql -I/usr/local/include -I/usr/include/libxml2 -I/usr/include/zthread -I../external/mariadbpp -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CSecurityPriceProcessor.o CSecurityPriceProcessor.cpp

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DELPP_DISABLE_LOGS -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/mysql -I/usr/local/include -I/usr/include/libxml2 -I/usr/include/zthread -I../external/mariadbpp -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:
	cd ../semain && ${MAKE}  -f Makefile CONF=Release_mysql
	cd ../seadmin && ${MAKE}  -f Makefile CONF=Release_mysql
	cd ../seeoddata && ${MAKE}  -f Makefile CONF=Release_mysql
	cd ../sesecurities && ${MAKE}  -f Makefile CONF=Release_mysql
	cd ../seeconomy && ${MAKE}  -f Makefile CONF=Release_mysql
	cd ../sepredict && ${MAKE}  -f Makefile CONF=Release_mysql
	cd ../seann && ${MAKE}  -f Makefile CONF=Release_mysql
	cd ../setraining && ${MAKE}  -f Makefile CONF=Release_mysql
	cd ../sexbrl && ${MAKE}  -f Makefile CONF=Release_mysql
	cd ../senet && ${MAKE}  -f Makefile CONF=Release_mysql
	cd ../semysql && ${MAKE}  -f Makefile CONF=Release_mysql
	cd ../semath && ${MAKE}  -f Makefile CONF=Release_mysql
	cd ../secubist && ${MAKE}  -f Makefile CONF=Release
	cd ../sedata && ${MAKE}  -f Makefile CONF=Release
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Release
	cd ../external/mariadbpp && ${MAKE}  -f Makefile CONF=Release_mysql

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} -r ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemain.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseadmin.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeoddata.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsesecurities.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeconomy.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsepredict.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseann.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsetraining.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsexbrl.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsenet.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemysql.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemath.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsecubist.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsedata.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseglobal.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmariadbpp.so
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sebgpd

# Subprojects
.clean-subprojects:
	cd ../semain && ${MAKE}  -f Makefile CONF=Release_mysql clean
	cd ../seadmin && ${MAKE}  -f Makefile CONF=Release_mysql clean
	cd ../seeoddata && ${MAKE}  -f Makefile CONF=Release_mysql clean
	cd ../sesecurities && ${MAKE}  -f Makefile CONF=Release_mysql clean
	cd ../seeconomy && ${MAKE}  -f Makefile CONF=Release_mysql clean
	cd ../sepredict && ${MAKE}  -f Makefile CONF=Release_mysql clean
	cd ../seann && ${MAKE}  -f Makefile CONF=Release_mysql clean
	cd ../setraining && ${MAKE}  -f Makefile CONF=Release_mysql clean
	cd ../sexbrl && ${MAKE}  -f Makefile CONF=Release_mysql clean
	cd ../senet && ${MAKE}  -f Makefile CONF=Release_mysql clean
	cd ../semysql && ${MAKE}  -f Makefile CONF=Release_mysql clean
	cd ../semath && ${MAKE}  -f Makefile CONF=Release_mysql clean
	cd ../secubist && ${MAKE}  -f Makefile CONF=Release clean
	cd ../sedata && ${MAKE}  -f Makefile CONF=Release clean
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Release clean
	cd ../external/mariadbpp && ${MAKE}  -f Makefile CONF=Release_mysql clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
