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
CND_CONF=Release_mysql
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/CCIKToSymbolMapper.o \
	${OBJECTDIR}/CCaptureFiscalPeriods.o \
	${OBJECTDIR}/CExplicitFormulaManager.o \
	${OBJECTDIR}/CFinPeriodList.o \
	${OBJECTDIR}/CFinancialItem.o \
	${OBJECTDIR}/CFinancialPerformance.o \
	${OBJECTDIR}/CFinancialPerformanceList.o \
	${OBJECTDIR}/CFinancialRatio.o \
	${OBJECTDIR}/CFinancialRatioList.o \
	${OBJECTDIR}/CFinancialsConsolidated.o \
	${OBJECTDIR}/CFormulaParser.o \
	${OBJECTDIR}/CListEdgarForms.o \
	${OBJECTDIR}/CPersistPerformanceData.o \
	${OBJECTDIR}/CProcessEdgarStatementsJson.o \
	${OBJECTDIR}/CSpecialPerformanceTrainingData.o \
	${OBJECTDIR}/CStatementUtilities.o \
	${OBJECTDIR}/CXbrl.o \
	${OBJECTDIR}/CXbrlAccountMatcher.o \
	${OBJECTDIR}/CXbrlDataNode.o \
	${OBJECTDIR}/CXbrlDataTree.o \
	${OBJECTDIR}/CXbrlDocType.o \
	${OBJECTDIR}/CXbrlIdList.o \
	${OBJECTDIR}/CXbrlJsonOutputs.o \
	${OBJECTDIR}/CXbrlStatement.o \
	${OBJECTDIR}/CXbrlStatementList.o \
	${OBJECTDIR}/CXbrlUtilities.o \
	${OBJECTDIR}/sexbrlinternal.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-fPIC -fvisibility=hidden
CXXFLAGS=-fPIC -fvisibility=hidden

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L/usr/lib -L/usr/local/lib -Wl,-rpath,'../semath/dist/Release_mysql/GNU-Linux' -L../semath/dist/Release_mysql/GNU-Linux -lsemath -Wl,-rpath,'../setraining/dist/Release_mysql/GNU-Linux' -L../setraining/dist/Release_mysql/GNU-Linux -lsetraining -Wl,-rpath,'../senet/dist/Release_mysql/GNU-Linux' -L../senet/dist/Release_mysql/GNU-Linux -lsenet -Wl,-rpath,'../seeconomy/dist/Release_mysql/GNU-Linux' -L../seeconomy/dist/Release_mysql/GNU-Linux -lseeconomy -Wl,-rpath,'../secubist/dist/Release/GNU-Linux' -L../secubist/dist/Release/GNU-Linux -lsecubist -Wl,-rpath,'../semysql/dist/Release_mysql/GNU-Linux' -L../semysql/dist/Release_mysql/GNU-Linux -lsemysql -Wl,-rpath,'../seglobal/dist/Release/GNU-Linux' -L../seglobal/dist/Release/GNU-Linux -lseglobal -Wl,-rpath,'../external/mariadbpp/dist/Release_mysql/GNU-Linux' -L../external/mariadbpp/dist/Release_mysql/GNU-Linux -lmariadbpp -lpthread_workqueue -lboost_regex -lxml2 -lcurl -ljsoncpp -lboost_date_time -lZThread -lboost_filesystem

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsexbrl.${CND_DLIB_EXT}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsexbrl.${CND_DLIB_EXT}: ../semath/dist/Release_mysql/GNU-Linux/libsemath.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsexbrl.${CND_DLIB_EXT}: ../setraining/dist/Release_mysql/GNU-Linux/libsetraining.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsexbrl.${CND_DLIB_EXT}: ../senet/dist/Release_mysql/GNU-Linux/libsenet.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsexbrl.${CND_DLIB_EXT}: ../seeconomy/dist/Release_mysql/GNU-Linux/libseeconomy.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsexbrl.${CND_DLIB_EXT}: ../secubist/dist/Release/GNU-Linux/libsecubist.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsexbrl.${CND_DLIB_EXT}: ../semysql/dist/Release_mysql/GNU-Linux/libsemysql.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsexbrl.${CND_DLIB_EXT}: ../seglobal/dist/Release/GNU-Linux/libseglobal.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsexbrl.${CND_DLIB_EXT}: ../external/mariadbpp/dist/Release_mysql/GNU-Linux/libmariadbpp.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsexbrl.${CND_DLIB_EXT}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsexbrl.${CND_DLIB_EXT} ${OBJECTFILES} ${LDLIBSOPTIONS} -fPIC -fvisibility=hidden -shared -fPIC

${OBJECTDIR}/CCIKToSymbolMapper.o: CCIKToSymbolMapper.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DELPP_DISABLE_LOGS -DXBRL_EXPORT -I/usr/include -I/usr/include/libxml2 -I/usr/local -I../semysql -I/usr/include/mysql -I../external/mariadbpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CCIKToSymbolMapper.o CCIKToSymbolMapper.cpp

${OBJECTDIR}/CCaptureFiscalPeriods.o: CCaptureFiscalPeriods.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DELPP_DISABLE_LOGS -DXBRL_EXPORT -I/usr/include -I/usr/include/libxml2 -I/usr/local -I../semysql -I/usr/include/mysql -I../external/mariadbpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CCaptureFiscalPeriods.o CCaptureFiscalPeriods.cpp

${OBJECTDIR}/CExplicitFormulaManager.o: CExplicitFormulaManager.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DELPP_DISABLE_LOGS -DXBRL_EXPORT -I/usr/include -I/usr/include/libxml2 -I/usr/local -I../semysql -I/usr/include/mysql -I../external/mariadbpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CExplicitFormulaManager.o CExplicitFormulaManager.cpp

${OBJECTDIR}/CFinPeriodList.o: CFinPeriodList.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DELPP_DISABLE_LOGS -DXBRL_EXPORT -I/usr/include -I/usr/include/libxml2 -I/usr/local -I../semysql -I/usr/include/mysql -I../external/mariadbpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CFinPeriodList.o CFinPeriodList.cpp

${OBJECTDIR}/CFinancialItem.o: CFinancialItem.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DELPP_DISABLE_LOGS -DXBRL_EXPORT -I/usr/include -I/usr/include/libxml2 -I/usr/local -I../semysql -I/usr/include/mysql -I../external/mariadbpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CFinancialItem.o CFinancialItem.cpp

${OBJECTDIR}/CFinancialPerformance.o: CFinancialPerformance.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DELPP_DISABLE_LOGS -DXBRL_EXPORT -I/usr/include -I/usr/include/libxml2 -I/usr/local -I../semysql -I/usr/include/mysql -I../external/mariadbpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CFinancialPerformance.o CFinancialPerformance.cpp

${OBJECTDIR}/CFinancialPerformanceList.o: CFinancialPerformanceList.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DELPP_DISABLE_LOGS -DXBRL_EXPORT -I/usr/include -I/usr/include/libxml2 -I/usr/local -I../semysql -I/usr/include/mysql -I../external/mariadbpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CFinancialPerformanceList.o CFinancialPerformanceList.cpp

${OBJECTDIR}/CFinancialRatio.o: CFinancialRatio.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DELPP_DISABLE_LOGS -DXBRL_EXPORT -I/usr/include -I/usr/include/libxml2 -I/usr/local -I../semysql -I/usr/include/mysql -I../external/mariadbpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CFinancialRatio.o CFinancialRatio.cpp

${OBJECTDIR}/CFinancialRatioList.o: CFinancialRatioList.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DELPP_DISABLE_LOGS -DXBRL_EXPORT -I/usr/include -I/usr/include/libxml2 -I/usr/local -I../semysql -I/usr/include/mysql -I../external/mariadbpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CFinancialRatioList.o CFinancialRatioList.cpp

${OBJECTDIR}/CFinancialsConsolidated.o: CFinancialsConsolidated.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DELPP_DISABLE_LOGS -DXBRL_EXPORT -I/usr/include -I/usr/include/libxml2 -I/usr/local -I../semysql -I/usr/include/mysql -I../external/mariadbpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CFinancialsConsolidated.o CFinancialsConsolidated.cpp

${OBJECTDIR}/CFormulaParser.o: CFormulaParser.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DELPP_DISABLE_LOGS -DXBRL_EXPORT -I/usr/include -I/usr/include/libxml2 -I/usr/local -I../semysql -I/usr/include/mysql -I../external/mariadbpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CFormulaParser.o CFormulaParser.cpp

${OBJECTDIR}/CListEdgarForms.o: CListEdgarForms.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DELPP_DISABLE_LOGS -DXBRL_EXPORT -I/usr/include -I/usr/include/libxml2 -I/usr/local -I../semysql -I/usr/include/mysql -I../external/mariadbpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CListEdgarForms.o CListEdgarForms.cpp

${OBJECTDIR}/CPersistPerformanceData.o: CPersistPerformanceData.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DELPP_DISABLE_LOGS -DXBRL_EXPORT -I/usr/include -I/usr/include/libxml2 -I/usr/local -I../semysql -I/usr/include/mysql -I../external/mariadbpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CPersistPerformanceData.o CPersistPerformanceData.cpp

${OBJECTDIR}/CProcessEdgarStatementsJson.o: CProcessEdgarStatementsJson.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DELPP_DISABLE_LOGS -DXBRL_EXPORT -I/usr/include -I/usr/include/libxml2 -I/usr/local -I../semysql -I/usr/include/mysql -I../external/mariadbpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CProcessEdgarStatementsJson.o CProcessEdgarStatementsJson.cpp

${OBJECTDIR}/CSpecialPerformanceTrainingData.o: CSpecialPerformanceTrainingData.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DELPP_DISABLE_LOGS -DXBRL_EXPORT -I/usr/include -I/usr/include/libxml2 -I/usr/local -I../semysql -I/usr/include/mysql -I../external/mariadbpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CSpecialPerformanceTrainingData.o CSpecialPerformanceTrainingData.cpp

${OBJECTDIR}/CStatementUtilities.o: CStatementUtilities.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DELPP_DISABLE_LOGS -DXBRL_EXPORT -I/usr/include -I/usr/include/libxml2 -I/usr/local -I../semysql -I/usr/include/mysql -I../external/mariadbpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CStatementUtilities.o CStatementUtilities.cpp

${OBJECTDIR}/CXbrl.o: CXbrl.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DELPP_DISABLE_LOGS -DXBRL_EXPORT -I/usr/include -I/usr/include/libxml2 -I/usr/local -I../semysql -I/usr/include/mysql -I../external/mariadbpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CXbrl.o CXbrl.cpp

${OBJECTDIR}/CXbrlAccountMatcher.o: CXbrlAccountMatcher.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DELPP_DISABLE_LOGS -DXBRL_EXPORT -I/usr/include -I/usr/include/libxml2 -I/usr/local -I../semysql -I/usr/include/mysql -I../external/mariadbpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CXbrlAccountMatcher.o CXbrlAccountMatcher.cpp

${OBJECTDIR}/CXbrlDataNode.o: CXbrlDataNode.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DELPP_DISABLE_LOGS -DXBRL_EXPORT -I/usr/include -I/usr/include/libxml2 -I/usr/local -I../semysql -I/usr/include/mysql -I../external/mariadbpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CXbrlDataNode.o CXbrlDataNode.cpp

${OBJECTDIR}/CXbrlDataTree.o: CXbrlDataTree.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DELPP_DISABLE_LOGS -DXBRL_EXPORT -I/usr/include -I/usr/include/libxml2 -I/usr/local -I../semysql -I/usr/include/mysql -I../external/mariadbpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CXbrlDataTree.o CXbrlDataTree.cpp

${OBJECTDIR}/CXbrlDocType.o: CXbrlDocType.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DELPP_DISABLE_LOGS -DXBRL_EXPORT -I/usr/include -I/usr/include/libxml2 -I/usr/local -I../semysql -I/usr/include/mysql -I../external/mariadbpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CXbrlDocType.o CXbrlDocType.cpp

${OBJECTDIR}/CXbrlIdList.o: CXbrlIdList.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DELPP_DISABLE_LOGS -DXBRL_EXPORT -I/usr/include -I/usr/include/libxml2 -I/usr/local -I../semysql -I/usr/include/mysql -I../external/mariadbpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CXbrlIdList.o CXbrlIdList.cpp

${OBJECTDIR}/CXbrlJsonOutputs.o: CXbrlJsonOutputs.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DELPP_DISABLE_LOGS -DXBRL_EXPORT -I/usr/include -I/usr/include/libxml2 -I/usr/local -I../semysql -I/usr/include/mysql -I../external/mariadbpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CXbrlJsonOutputs.o CXbrlJsonOutputs.cpp

${OBJECTDIR}/CXbrlStatement.o: CXbrlStatement.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DELPP_DISABLE_LOGS -DXBRL_EXPORT -I/usr/include -I/usr/include/libxml2 -I/usr/local -I../semysql -I/usr/include/mysql -I../external/mariadbpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CXbrlStatement.o CXbrlStatement.cpp

${OBJECTDIR}/CXbrlStatementList.o: CXbrlStatementList.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DELPP_DISABLE_LOGS -DXBRL_EXPORT -I/usr/include -I/usr/include/libxml2 -I/usr/local -I../semysql -I/usr/include/mysql -I../external/mariadbpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CXbrlStatementList.o CXbrlStatementList.cpp

${OBJECTDIR}/CXbrlUtilities.o: CXbrlUtilities.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DELPP_DISABLE_LOGS -DXBRL_EXPORT -I/usr/include -I/usr/include/libxml2 -I/usr/local -I../semysql -I/usr/include/mysql -I../external/mariadbpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CXbrlUtilities.o CXbrlUtilities.cpp

${OBJECTDIR}/sexbrlinternal.o: sexbrlinternal.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DELPP_DISABLE_LOGS -DXBRL_EXPORT -I/usr/include -I/usr/include/libxml2 -I/usr/local -I../semysql -I/usr/include/mysql -I../external/mariadbpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/sexbrlinternal.o sexbrlinternal.cpp

# Subprojects
.build-subprojects:
	cd ../semath && ${MAKE}  -f Makefile CONF=Release_mysql
	cd ../setraining && ${MAKE}  -f Makefile CONF=Release_mysql
	cd ../senet && ${MAKE}  -f Makefile CONF=Release_mysql
	cd ../seeconomy && ${MAKE}  -f Makefile CONF=Release_mysql
	cd ../secubist && ${MAKE}  -f Makefile CONF=Release
	cd ../semysql && ${MAKE}  -f Makefile CONF=Release_mysql
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Release
	cd ../external/mariadbpp && ${MAKE}  -f Makefile CONF=Release_mysql

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} -r ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemath.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsetraining.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsenet.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeconomy.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsecubist.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemysql.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseglobal.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmariadbpp.so
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsexbrl.${CND_DLIB_EXT}

# Subprojects
.clean-subprojects:
	cd ../semath && ${MAKE}  -f Makefile CONF=Release_mysql clean
	cd ../setraining && ${MAKE}  -f Makefile CONF=Release_mysql clean
	cd ../senet && ${MAKE}  -f Makefile CONF=Release_mysql clean
	cd ../seeconomy && ${MAKE}  -f Makefile CONF=Release_mysql clean
	cd ../secubist && ${MAKE}  -f Makefile CONF=Release clean
	cd ../semysql && ${MAKE}  -f Makefile CONF=Release_mysql clean
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Release clean
	cd ../external/mariadbpp && ${MAKE}  -f Makefile CONF=Release_mysql clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
