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
	${OBJECTDIR}/About.o \
	${OBJECTDIR}/CAdminApp.o \
	${OBJECTDIR}/CBaseApplication.o \
	${OBJECTDIR}/CBaseContent.o \
	${OBJECTDIR}/CSPSerializable.o \
	${OBJECTDIR}/CSearchController.o \
	${OBJECTDIR}/CSession.o \
	${OBJECTDIR}/CSmartearnApp.o \
	${OBJECTDIR}/CompSearch.o \
	${OBJECTDIR}/CompanyFinancials.o \
	${OBJECTDIR}/CompanyPerformance.o \
	${OBJECTDIR}/CompanyProspects.o \
	${OBJECTDIR}/CompanySummary.o \
	${OBJECTDIR}/Economy.o \
	${OBJECTDIR}/FinancialStatement.o \
	${OBJECTDIR}/Footer.o \
	${OBJECTDIR}/Fundamentals.o \
	${OBJECTDIR}/Header.o \
	${OBJECTDIR}/Home.o \
	${OBJECTDIR}/Master.o \
	${OBJECTDIR}/MyAccount.o \
	${OBJECTDIR}/PortfolioWiz.o \
	${OBJECTDIR}/RecentFinds.o \
	${OBJECTDIR}/SGSkin.o \
	${OBJECTDIR}/Search.o \
	${OBJECTDIR}/Sitemap.o \
	${OBJECTDIR}/StockPicks.o \
	${OBJECTDIR}/Subscriptions.o \
	${OBJECTDIR}/UserManageModels.o \
	${OBJECTDIR}/admmodels.o \
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
LDLIBSOPTIONS=-L/usr/lib/x86_64-linux-gnu -Wl,-rpath,../sesecurities/dist/Release/GNU-Linux-x86 -L../sesecurities/dist/Release/GNU-Linux-x86 -lsesecurities -Wl,-rpath,../sexbrl/dist/Release/GNU-Linux-x86 -L../sexbrl/dist/Release/GNU-Linux-x86 -lsexbrl -Wl,-rpath,../seadmin/dist/Release/GNU-Linux-x86 -L../seadmin/dist/Release/GNU-Linux-x86 -lseadmin -Wl,-rpath,../seeconomy/dist/Release/GNU-Linux-x86 -L../seeconomy/dist/Release/GNU-Linux-x86 -lseeconomy -Wl,-rpath,../semain/dist/Release/GNU-Linux-x86 -L../semain/dist/Release/GNU-Linux-x86 -lsemain -Wl,-rpath,../setraining/dist/Release/GNU-Linux-x86 -L../setraining/dist/Release/GNU-Linux-x86 -lsetraining -Wl,-rpath,../secubist/dist/Release/GNU-Linux-x86 -L../secubist/dist/Release/GNU-Linux-x86 -lsecubist -Wl,-rpath,../seann/dist/Release/GNU-Linux-x86 -L../seann/dist/Release/GNU-Linux-x86 -lseann ../semysql/dist/Release/GNU-Linux-x86/libsemysql.a -Wl,-rpath,../senet/dist/Release/GNU-Linux-x86 -L../senet/dist/Release/GNU-Linux-x86 -lsenet ../seglobal/dist/Release/GNU-Linux-x86/libseglobal.a -Wl,-rpath,../external/mariadbpp/dist/Release/GNU-Linux-x86 -L../external/mariadbpp/dist/Release/GNU-Linux-x86 -lmariadbpp -lbooster -lxml2 -lcppcms -ljsoncpp -lboost_iostreams -lboost_date_time -lboost_regex -lcurl -lboost_system -lboost_filesystem -ltar -lPocoNet -lZThread -lPocoFoundation -lcgicc -lmariadbclient_r

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sgweb

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sgweb: ../sesecurities/dist/Release/GNU-Linux-x86/libsesecurities.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sgweb: ../sexbrl/dist/Release/GNU-Linux-x86/libsexbrl.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sgweb: ../seadmin/dist/Release/GNU-Linux-x86/libseadmin.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sgweb: ../seeconomy/dist/Release/GNU-Linux-x86/libseeconomy.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sgweb: ../semain/dist/Release/GNU-Linux-x86/libsemain.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sgweb: ../setraining/dist/Release/GNU-Linux-x86/libsetraining.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sgweb: ../secubist/dist/Release/GNU-Linux-x86/libsecubist.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sgweb: ../seann/dist/Release/GNU-Linux-x86/libseann.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sgweb: ../semysql/dist/Release/GNU-Linux-x86/libsemysql.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sgweb: ../senet/dist/Release/GNU-Linux-x86/libsenet.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sgweb: ../seglobal/dist/Release/GNU-Linux-x86/libseglobal.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sgweb: ../external/mariadbpp/dist/Release/GNU-Linux-x86/libmariadbpp.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sgweb: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sgweb ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/About.o: About.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/mysql -I/usr/include/libxml2 -I/usr/include/jsoncpp -I/usr/include/libxml++-2.6 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/About.o About.cpp

${OBJECTDIR}/CAdminApp.o: CAdminApp.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/mysql -I/usr/include/libxml2 -I/usr/include/jsoncpp -I/usr/include/libxml++-2.6 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CAdminApp.o CAdminApp.cpp

${OBJECTDIR}/CBaseApplication.o: CBaseApplication.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/mysql -I/usr/include/libxml2 -I/usr/include/jsoncpp -I/usr/include/libxml++-2.6 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CBaseApplication.o CBaseApplication.cpp

${OBJECTDIR}/CBaseContent.o: CBaseContent.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/mysql -I/usr/include/libxml2 -I/usr/include/jsoncpp -I/usr/include/libxml++-2.6 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CBaseContent.o CBaseContent.cpp

${OBJECTDIR}/CSPSerializable.o: CSPSerializable.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/mysql -I/usr/include/libxml2 -I/usr/include/jsoncpp -I/usr/include/libxml++-2.6 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CSPSerializable.o CSPSerializable.cpp

${OBJECTDIR}/CSearchController.o: CSearchController.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/mysql -I/usr/include/libxml2 -I/usr/include/jsoncpp -I/usr/include/libxml++-2.6 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CSearchController.o CSearchController.cpp

${OBJECTDIR}/CSession.o: CSession.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/mysql -I/usr/include/libxml2 -I/usr/include/jsoncpp -I/usr/include/libxml++-2.6 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CSession.o CSession.cpp

${OBJECTDIR}/CSmartearnApp.o: CSmartearnApp.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/mysql -I/usr/include/libxml2 -I/usr/include/jsoncpp -I/usr/include/libxml++-2.6 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CSmartearnApp.o CSmartearnApp.cpp

${OBJECTDIR}/CompSearch.o: CompSearch.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/mysql -I/usr/include/libxml2 -I/usr/include/jsoncpp -I/usr/include/libxml++-2.6 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CompSearch.o CompSearch.cpp

${OBJECTDIR}/CompanyFinancials.o: CompanyFinancials.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/mysql -I/usr/include/libxml2 -I/usr/include/jsoncpp -I/usr/include/libxml++-2.6 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CompanyFinancials.o CompanyFinancials.cpp

${OBJECTDIR}/CompanyPerformance.o: CompanyPerformance.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/mysql -I/usr/include/libxml2 -I/usr/include/jsoncpp -I/usr/include/libxml++-2.6 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CompanyPerformance.o CompanyPerformance.cpp

${OBJECTDIR}/CompanyProspects.o: CompanyProspects.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/mysql -I/usr/include/libxml2 -I/usr/include/jsoncpp -I/usr/include/libxml++-2.6 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CompanyProspects.o CompanyProspects.cpp

${OBJECTDIR}/CompanySummary.o: CompanySummary.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/mysql -I/usr/include/libxml2 -I/usr/include/jsoncpp -I/usr/include/libxml++-2.6 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CompanySummary.o CompanySummary.cpp

${OBJECTDIR}/Economy.o: Economy.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/mysql -I/usr/include/libxml2 -I/usr/include/jsoncpp -I/usr/include/libxml++-2.6 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Economy.o Economy.cpp

${OBJECTDIR}/FinancialStatement.o: FinancialStatement.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/mysql -I/usr/include/libxml2 -I/usr/include/jsoncpp -I/usr/include/libxml++-2.6 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/FinancialStatement.o FinancialStatement.cpp

${OBJECTDIR}/Footer.o: Footer.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/mysql -I/usr/include/libxml2 -I/usr/include/jsoncpp -I/usr/include/libxml++-2.6 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Footer.o Footer.cpp

${OBJECTDIR}/Fundamentals.o: Fundamentals.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/mysql -I/usr/include/libxml2 -I/usr/include/jsoncpp -I/usr/include/libxml++-2.6 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Fundamentals.o Fundamentals.cpp

${OBJECTDIR}/Header.o: Header.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/mysql -I/usr/include/libxml2 -I/usr/include/jsoncpp -I/usr/include/libxml++-2.6 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Header.o Header.cpp

${OBJECTDIR}/Home.o: Home.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/mysql -I/usr/include/libxml2 -I/usr/include/jsoncpp -I/usr/include/libxml++-2.6 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Home.o Home.cpp

${OBJECTDIR}/Master.o: Master.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/mysql -I/usr/include/libxml2 -I/usr/include/jsoncpp -I/usr/include/libxml++-2.6 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Master.o Master.cpp

${OBJECTDIR}/MyAccount.o: MyAccount.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/mysql -I/usr/include/libxml2 -I/usr/include/jsoncpp -I/usr/include/libxml++-2.6 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MyAccount.o MyAccount.cpp

${OBJECTDIR}/PortfolioWiz.o: PortfolioWiz.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/mysql -I/usr/include/libxml2 -I/usr/include/jsoncpp -I/usr/include/libxml++-2.6 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/PortfolioWiz.o PortfolioWiz.cpp

${OBJECTDIR}/RecentFinds.o: RecentFinds.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/mysql -I/usr/include/libxml2 -I/usr/include/jsoncpp -I/usr/include/libxml++-2.6 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/RecentFinds.o RecentFinds.cpp

${OBJECTDIR}/SGSkin.o: SGSkin.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/mysql -I/usr/include/libxml2 -I/usr/include/jsoncpp -I/usr/include/libxml++-2.6 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SGSkin.o SGSkin.cpp

${OBJECTDIR}/Search.o: Search.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/mysql -I/usr/include/libxml2 -I/usr/include/jsoncpp -I/usr/include/libxml++-2.6 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Search.o Search.cpp

${OBJECTDIR}/Sitemap.o: Sitemap.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/mysql -I/usr/include/libxml2 -I/usr/include/jsoncpp -I/usr/include/libxml++-2.6 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Sitemap.o Sitemap.cpp

${OBJECTDIR}/StockPicks.o: StockPicks.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/mysql -I/usr/include/libxml2 -I/usr/include/jsoncpp -I/usr/include/libxml++-2.6 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/StockPicks.o StockPicks.cpp

${OBJECTDIR}/Subscriptions.o: Subscriptions.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/mysql -I/usr/include/libxml2 -I/usr/include/jsoncpp -I/usr/include/libxml++-2.6 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Subscriptions.o Subscriptions.cpp

${OBJECTDIR}/UserManageModels.o: UserManageModels.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/mysql -I/usr/include/libxml2 -I/usr/include/jsoncpp -I/usr/include/libxml++-2.6 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/UserManageModels.o UserManageModels.cpp

${OBJECTDIR}/admmodels.o: admmodels.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/mysql -I/usr/include/libxml2 -I/usr/include/jsoncpp -I/usr/include/libxml++-2.6 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/admmodels.o admmodels.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/include/mysql -I/usr/include/libxml2 -I/usr/include/jsoncpp -I/usr/include/libxml++-2.6 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:
	cd ../sesecurities && ${MAKE}  -f Makefile CONF=Release
	cd ../sexbrl && ${MAKE}  -f Makefile CONF=Release
	cd ../seadmin && ${MAKE}  -f Makefile CONF=Release
	cd ../seeconomy && ${MAKE}  -f Makefile CONF=Release
	cd ../semain && ${MAKE}  -f Makefile CONF=Release
	cd ../setraining && ${MAKE}  -f Makefile CONF=Release
	cd ../secubist && ${MAKE}  -f Makefile CONF=Release
	cd ../seann && ${MAKE}  -f Makefile CONF=Release
	cd ../semysql && ${MAKE}  -f Makefile CONF=Release
	cd ../senet && ${MAKE}  -f Makefile CONF=Release
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Release
	cd ../external/mariadbpp && ${MAKE}  -f Makefile CONF=Release

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sgweb

# Subprojects
.clean-subprojects:
	cd ../sesecurities && ${MAKE}  -f Makefile CONF=Release clean
	cd ../sexbrl && ${MAKE}  -f Makefile CONF=Release clean
	cd ../seadmin && ${MAKE}  -f Makefile CONF=Release clean
	cd ../seeconomy && ${MAKE}  -f Makefile CONF=Release clean
	cd ../semain && ${MAKE}  -f Makefile CONF=Release clean
	cd ../setraining && ${MAKE}  -f Makefile CONF=Release clean
	cd ../secubist && ${MAKE}  -f Makefile CONF=Release clean
	cd ../seann && ${MAKE}  -f Makefile CONF=Release clean
	cd ../semysql && ${MAKE}  -f Makefile CONF=Release clean
	cd ../senet && ${MAKE}  -f Makefile CONF=Release clean
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Release clean
	cd ../external/mariadbpp && ${MAKE}  -f Makefile CONF=Release clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
