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
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/CAdminAPI.o \
	${OBJECTDIR}/CAudit.o \
	${OBJECTDIR}/CCountry.o \
	${OBJECTDIR}/CCountryState.o \
	${OBJECTDIR}/CCustomer.o \
	${OBJECTDIR}/CCustomerStatus.o \
	${OBJECTDIR}/CCustomerSubscriptionInfo.o \
	${OBJECTDIR}/CCustomerSubscriptionType.o \
	${OBJECTDIR}/CHelpDocTree.o \
	${OBJECTDIR}/CPaypal.o \
	${OBJECTDIR}/CPaypalExpressCheckout.o \
	${OBJECTDIR}/CPortfolio.o \
	${OBJECTDIR}/CPositions.o \
	${OBJECTDIR}/CProduct.o \
	${OBJECTDIR}/CPurchase.o \
	${OBJECTDIR}/CPurgeUserSessions.o \
	${OBJECTDIR}/CUser.o \
	${OBJECTDIR}/CUserType.o \
	${OBJECTDIR}/CWebSession.o \
	${OBJECTDIR}/seadminapi.o


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
LDLIBSOPTIONS=-Wl,-rpath,'../sesecurities/dist/Release/GNU-Linux' -L../sesecurities/dist/Release/GNU-Linux -lsesecurities -Wl,-rpath,'../setraining/dist/Release/GNU-Linux' -L../setraining/dist/Release/GNU-Linux -lsetraining -Wl,-rpath,'../sedata/dist/Release/GNU-Linux' -L../sedata/dist/Release/GNU-Linux -lsedata -Wl,-rpath,'../semysql/dist/Release/GNU-Linux' -L../semysql/dist/Release/GNU-Linux -lsemysql -Wl,-rpath,'../seglobal/dist/Release/GNU-Linux' -L../seglobal/dist/Release/GNU-Linux -lseglobal -Wl,-rpath,'../external/mariadbpp/dist/Release/GNU-Linux' -L../external/mariadbpp/dist/Release/GNU-Linux -lmariadbpp -lboost_date_time -lZThread -lcurl -lboost_system -lboost_filesystem -lssl -luuid

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseadmin.${CND_DLIB_EXT}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseadmin.${CND_DLIB_EXT}: ../sesecurities/dist/Release/GNU-Linux/libsesecurities.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseadmin.${CND_DLIB_EXT}: ../setraining/dist/Release/GNU-Linux/libsetraining.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseadmin.${CND_DLIB_EXT}: ../sedata/dist/Release/GNU-Linux/libsedata.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseadmin.${CND_DLIB_EXT}: ../semysql/dist/Release/GNU-Linux/libsemysql.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseadmin.${CND_DLIB_EXT}: ../seglobal/dist/Release/GNU-Linux/libseglobal.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseadmin.${CND_DLIB_EXT}: ../external/mariadbpp/dist/Release/GNU-Linux/libmariadbpp.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseadmin.${CND_DLIB_EXT}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseadmin.${CND_DLIB_EXT} ${OBJECTFILES} ${LDLIBSOPTIONS} -shared -fPIC

${OBJECTDIR}/CAdminAPI.o: CAdminAPI.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mariadb -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CAdminAPI.o CAdminAPI.cpp

${OBJECTDIR}/CAudit.o: CAudit.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mariadb -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CAudit.o CAudit.cpp

${OBJECTDIR}/CCountry.o: CCountry.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mariadb -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CCountry.o CCountry.cpp

${OBJECTDIR}/CCountryState.o: CCountryState.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mariadb -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CCountryState.o CCountryState.cpp

${OBJECTDIR}/CCustomer.o: CCustomer.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mariadb -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CCustomer.o CCustomer.cpp

${OBJECTDIR}/CCustomerStatus.o: CCustomerStatus.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mariadb -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CCustomerStatus.o CCustomerStatus.cpp

${OBJECTDIR}/CCustomerSubscriptionInfo.o: CCustomerSubscriptionInfo.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mariadb -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CCustomerSubscriptionInfo.o CCustomerSubscriptionInfo.cpp

${OBJECTDIR}/CCustomerSubscriptionType.o: CCustomerSubscriptionType.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mariadb -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CCustomerSubscriptionType.o CCustomerSubscriptionType.cpp

${OBJECTDIR}/CHelpDocTree.o: CHelpDocTree.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mariadb -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CHelpDocTree.o CHelpDocTree.cpp

${OBJECTDIR}/CPaypal.o: CPaypal.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mariadb -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CPaypal.o CPaypal.cpp

${OBJECTDIR}/CPaypalExpressCheckout.o: CPaypalExpressCheckout.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mariadb -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CPaypalExpressCheckout.o CPaypalExpressCheckout.cpp

${OBJECTDIR}/CPortfolio.o: CPortfolio.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mariadb -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CPortfolio.o CPortfolio.cpp

${OBJECTDIR}/CPositions.o: CPositions.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mariadb -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CPositions.o CPositions.cpp

${OBJECTDIR}/CProduct.o: CProduct.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mariadb -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CProduct.o CProduct.cpp

${OBJECTDIR}/CPurchase.o: CPurchase.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mariadb -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CPurchase.o CPurchase.cpp

${OBJECTDIR}/CPurgeUserSessions.o: CPurgeUserSessions.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mariadb -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CPurgeUserSessions.o CPurgeUserSessions.cpp

${OBJECTDIR}/CUser.o: CUser.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mariadb -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CUser.o CUser.cpp

${OBJECTDIR}/CUserType.o: CUserType.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mariadb -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CUserType.o CUserType.cpp

${OBJECTDIR}/CWebSession.o: CWebSession.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mariadb -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CWebSession.o CWebSession.cpp

${OBJECTDIR}/seadminapi.o: seadminapi.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mariadb -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/seadminapi.o seadminapi.cpp

# Subprojects
.build-subprojects:
	cd ../sesecurities && ${MAKE}  -f Makefile CONF=Release
	cd ../setraining && ${MAKE}  -f Makefile CONF=Release
	cd ../sedata && ${MAKE}  -f Makefile CONF=Release
	cd ../semysql && ${MAKE}  -f Makefile CONF=Release
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Release
	cd ../external/mariadbpp && ${MAKE}  -f Makefile CONF=Release

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} -r ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsesecurities.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsetraining.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsedata.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemysql.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseglobal.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmariadbpp.so
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseadmin.${CND_DLIB_EXT}

# Subprojects
.clean-subprojects:
	cd ../sesecurities && ${MAKE}  -f Makefile CONF=Release clean
	cd ../setraining && ${MAKE}  -f Makefile CONF=Release clean
	cd ../sedata && ${MAKE}  -f Makefile CONF=Release clean
	cd ../semysql && ${MAKE}  -f Makefile CONF=Release clean
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Release clean
	cd ../external/mariadbpp && ${MAKE}  -f Makefile CONF=Release clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
