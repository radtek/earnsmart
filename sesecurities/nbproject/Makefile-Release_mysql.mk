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
	${OBJECTDIR}/CEarningCalendar.o \
	${OBJECTDIR}/CEconomincallyAdjustedPrices.o \
	${OBJECTDIR}/CEodDataProvider.o \
	${OBJECTDIR}/CGICList.o \
	${OBJECTDIR}/CMarketDataProvider.o \
	${OBJECTDIR}/COptionFeeder.o \
	${OBJECTDIR}/COptionPricing.o \
	${OBJECTDIR}/COptionYahooFeeder.o \
	${OBJECTDIR}/CParseCompanyProfile.o \
	${OBJECTDIR}/CPublicCompanyInfoList.o \
	${OBJECTDIR}/CSecurityNews.o \
	${OBJECTDIR}/CSecurityPrice.o \
	${OBJECTDIR}/CSecurityPriceRSSFeed.o \
	${OBJECTDIR}/CSecuritySelectionStatistics.o \
	${OBJECTDIR}/ExposedStructures.o \
	${OBJECTDIR}/GICSInfo.o \
	${OBJECTDIR}/PublicCompanyInfo.o \
	${OBJECTDIR}/Security.o \
	${OBJECTDIR}/SecurityAPI.o \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-fPIC
CXXFLAGS=-fPIC

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-Wl,-rpath,'../setraining/dist/Release_mysql/GNU-Linux' -L../setraining/dist/Release_mysql/GNU-Linux -lsetraining -Wl,-rpath,'../senet/dist/Release_mysql/GNU-Linux' -L../senet/dist/Release_mysql/GNU-Linux -lsenet -Wl,-rpath,'../semysql/dist/Release_mysql/GNU-Linux' -L../semysql/dist/Release_mysql/GNU-Linux -lsemysql -Wl,-rpath,'../sedata/dist/Release/GNU-Linux' -L../sedata/dist/Release/GNU-Linux -lsedata -Wl,-rpath,'../semath/dist/Release_mysql/GNU-Linux' -L../semath/dist/Release_mysql/GNU-Linux -lsemath -Wl,-rpath,'../seglobal/dist/Release/GNU-Linux' -L../seglobal/dist/Release/GNU-Linux -lseglobal -Wl,-rpath,'../external/mariadbpp/dist/Release_mysql/GNU-Linux' -L../external/mariadbpp/dist/Release_mysql/GNU-Linux -lmariadbpp -lboost_locale -lboost_regex -lboost_date_time -ljsoncpp -lxml2 -lcurl -ltidy

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsesecurities.${CND_DLIB_EXT}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsesecurities.${CND_DLIB_EXT}: ../setraining/dist/Release_mysql/GNU-Linux/libsetraining.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsesecurities.${CND_DLIB_EXT}: ../senet/dist/Release_mysql/GNU-Linux/libsenet.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsesecurities.${CND_DLIB_EXT}: ../semysql/dist/Release_mysql/GNU-Linux/libsemysql.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsesecurities.${CND_DLIB_EXT}: ../sedata/dist/Release/GNU-Linux/libsedata.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsesecurities.${CND_DLIB_EXT}: ../semath/dist/Release_mysql/GNU-Linux/libsemath.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsesecurities.${CND_DLIB_EXT}: ../seglobal/dist/Release/GNU-Linux/libseglobal.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsesecurities.${CND_DLIB_EXT}: ../external/mariadbpp/dist/Release_mysql/GNU-Linux/libmariadbpp.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsesecurities.${CND_DLIB_EXT}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsesecurities.${CND_DLIB_EXT} ${OBJECTFILES} ${LDLIBSOPTIONS} -shared -fPIC

${OBJECTDIR}/CEarningCalendar.o: CEarningCalendar.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mysql -I/usr/include/libxml2 -I../external/mariadbpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CEarningCalendar.o CEarningCalendar.cpp

${OBJECTDIR}/CEconomincallyAdjustedPrices.o: CEconomincallyAdjustedPrices.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mysql -I/usr/include/libxml2 -I../external/mariadbpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CEconomincallyAdjustedPrices.o CEconomincallyAdjustedPrices.cpp

${OBJECTDIR}/CEodDataProvider.o: CEodDataProvider.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mysql -I/usr/include/libxml2 -I../external/mariadbpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CEodDataProvider.o CEodDataProvider.cpp

${OBJECTDIR}/CGICList.o: CGICList.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mysql -I/usr/include/libxml2 -I../external/mariadbpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CGICList.o CGICList.cpp

${OBJECTDIR}/CMarketDataProvider.o: CMarketDataProvider.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mysql -I/usr/include/libxml2 -I../external/mariadbpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CMarketDataProvider.o CMarketDataProvider.cpp

${OBJECTDIR}/COptionFeeder.o: COptionFeeder.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mysql -I/usr/include/libxml2 -I../external/mariadbpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/COptionFeeder.o COptionFeeder.cpp

${OBJECTDIR}/COptionPricing.o: COptionPricing.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mysql -I/usr/include/libxml2 -I../external/mariadbpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/COptionPricing.o COptionPricing.cpp

${OBJECTDIR}/COptionYahooFeeder.o: COptionYahooFeeder.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mysql -I/usr/include/libxml2 -I../external/mariadbpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/COptionYahooFeeder.o COptionYahooFeeder.cpp

${OBJECTDIR}/CParseCompanyProfile.o: CParseCompanyProfile.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mysql -I/usr/include/libxml2 -I../external/mariadbpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CParseCompanyProfile.o CParseCompanyProfile.cpp

${OBJECTDIR}/CPublicCompanyInfoList.o: CPublicCompanyInfoList.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mysql -I/usr/include/libxml2 -I../external/mariadbpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CPublicCompanyInfoList.o CPublicCompanyInfoList.cpp

${OBJECTDIR}/CSecurityNews.o: CSecurityNews.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mysql -I/usr/include/libxml2 -I../external/mariadbpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CSecurityNews.o CSecurityNews.cpp

${OBJECTDIR}/CSecurityPrice.o: CSecurityPrice.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mysql -I/usr/include/libxml2 -I../external/mariadbpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CSecurityPrice.o CSecurityPrice.cpp

${OBJECTDIR}/CSecurityPriceRSSFeed.o: CSecurityPriceRSSFeed.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mysql -I/usr/include/libxml2 -I../external/mariadbpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CSecurityPriceRSSFeed.o CSecurityPriceRSSFeed.cpp

${OBJECTDIR}/CSecuritySelectionStatistics.o: CSecuritySelectionStatistics.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mysql -I/usr/include/libxml2 -I../external/mariadbpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CSecuritySelectionStatistics.o CSecuritySelectionStatistics.cpp

${OBJECTDIR}/ExposedStructures.o: ExposedStructures.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mysql -I/usr/include/libxml2 -I../external/mariadbpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ExposedStructures.o ExposedStructures.cpp

${OBJECTDIR}/GICSInfo.o: GICSInfo.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mysql -I/usr/include/libxml2 -I../external/mariadbpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/GICSInfo.o GICSInfo.cpp

${OBJECTDIR}/PublicCompanyInfo.o: PublicCompanyInfo.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mysql -I/usr/include/libxml2 -I../external/mariadbpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/PublicCompanyInfo.o PublicCompanyInfo.cpp

${OBJECTDIR}/Security.o: Security.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mysql -I/usr/include/libxml2 -I../external/mariadbpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Security.o Security.cpp

${OBJECTDIR}/SecurityAPI.o: SecurityAPI.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mysql -I/usr/include/libxml2 -I../external/mariadbpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SecurityAPI.o SecurityAPI.cpp

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mysql -I/usr/include/libxml2 -I../external/mariadbpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:
	cd ../setraining && ${MAKE}  -f Makefile CONF=Release_mysql
	cd ../senet && ${MAKE}  -f Makefile CONF=Release_mysql
	cd ../semysql && ${MAKE}  -f Makefile CONF=Release_mysql
	cd ../sedata && ${MAKE}  -f Makefile CONF=Release
	cd ../semath && ${MAKE}  -f Makefile CONF=Release_mysql
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Release
	cd ../external/mariadbpp && ${MAKE}  -f Makefile CONF=Release_mysql

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} -r ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsetraining.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsenet.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemysql.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsedata.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemath.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseglobal.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmariadbpp.so
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsesecurities.${CND_DLIB_EXT}

# Subprojects
.clean-subprojects:
	cd ../setraining && ${MAKE}  -f Makefile CONF=Release_mysql clean
	cd ../senet && ${MAKE}  -f Makefile CONF=Release_mysql clean
	cd ../semysql && ${MAKE}  -f Makefile CONF=Release_mysql clean
	cd ../sedata && ${MAKE}  -f Makefile CONF=Release clean
	cd ../semath && ${MAKE}  -f Makefile CONF=Release_mysql clean
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Release clean
	cd ../external/mariadbpp && ${MAKE}  -f Makefile CONF=Release_mysql clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
