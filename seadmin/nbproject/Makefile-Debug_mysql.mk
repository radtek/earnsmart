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
LDLIBSOPTIONS=-Wl,-rpath,'../sesecurities/dist/Debug_mysql/GNU-Linux' -L../sesecurities/dist/Debug_mysql/GNU-Linux -lsesecurities -Wl,-rpath,'../setraining/dist/Debug_mysql/GNU-Linux' -L../setraining/dist/Debug_mysql/GNU-Linux -lsetraining -Wl,-rpath,'../semysql/dist/Debug_mysql/GNU-Linux' -L../semysql/dist/Debug_mysql/GNU-Linux -lsemysql -Wl,-rpath,'../sedata/dist/Debug/GNU-Linux' -L../sedata/dist/Debug/GNU-Linux -lsedata -Wl,-rpath,'../semath/dist/Debug_mysql/GNU-Linux' -L../semath/dist/Debug_mysql/GNU-Linux -lsemath -Wl,-rpath,'../seglobal/dist/Debug/GNU-Linux' -L../seglobal/dist/Debug/GNU-Linux -lseglobal -Wl,-rpath,'../external/mariadbpp/dist/Debug_mysql/GNU-Linux' -L../external/mariadbpp/dist/Debug_mysql/GNU-Linux -lmariadbpp -lZThread -lcurl -lssl -luuid

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseadmin.${CND_DLIB_EXT}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseadmin.${CND_DLIB_EXT}: ../sesecurities/dist/Debug_mysql/GNU-Linux/libsesecurities.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseadmin.${CND_DLIB_EXT}: ../setraining/dist/Debug_mysql/GNU-Linux/libsetraining.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseadmin.${CND_DLIB_EXT}: ../semysql/dist/Debug_mysql/GNU-Linux/libsemysql.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseadmin.${CND_DLIB_EXT}: ../sedata/dist/Debug/GNU-Linux/libsedata.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseadmin.${CND_DLIB_EXT}: ../semath/dist/Debug_mysql/GNU-Linux/libsemath.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseadmin.${CND_DLIB_EXT}: ../seglobal/dist/Debug/GNU-Linux/libseglobal.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseadmin.${CND_DLIB_EXT}: ../external/mariadbpp/dist/Debug_mysql/GNU-Linux/libmariadbpp.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseadmin.${CND_DLIB_EXT}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseadmin.${CND_DLIB_EXT} ${OBJECTFILES} ${LDLIBSOPTIONS} -shared -fPIC

${OBJECTDIR}/CAdminAPI.o: CAdminAPI.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CAdminAPI.o CAdminAPI.cpp

${OBJECTDIR}/CAudit.o: CAudit.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CAudit.o CAudit.cpp

${OBJECTDIR}/CCountry.o: CCountry.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CCountry.o CCountry.cpp

${OBJECTDIR}/CCountryState.o: CCountryState.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CCountryState.o CCountryState.cpp

${OBJECTDIR}/CCustomer.o: CCustomer.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CCustomer.o CCustomer.cpp

${OBJECTDIR}/CCustomerStatus.o: CCustomerStatus.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CCustomerStatus.o CCustomerStatus.cpp

${OBJECTDIR}/CCustomerSubscriptionInfo.o: CCustomerSubscriptionInfo.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CCustomerSubscriptionInfo.o CCustomerSubscriptionInfo.cpp

${OBJECTDIR}/CCustomerSubscriptionType.o: CCustomerSubscriptionType.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CCustomerSubscriptionType.o CCustomerSubscriptionType.cpp

${OBJECTDIR}/CHelpDocTree.o: CHelpDocTree.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CHelpDocTree.o CHelpDocTree.cpp

${OBJECTDIR}/CPaypal.o: CPaypal.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CPaypal.o CPaypal.cpp

${OBJECTDIR}/CPaypalExpressCheckout.o: CPaypalExpressCheckout.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CPaypalExpressCheckout.o CPaypalExpressCheckout.cpp

${OBJECTDIR}/CPortfolio.o: CPortfolio.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CPortfolio.o CPortfolio.cpp

${OBJECTDIR}/CPositions.o: CPositions.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CPositions.o CPositions.cpp

${OBJECTDIR}/CProduct.o: CProduct.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CProduct.o CProduct.cpp

${OBJECTDIR}/CPurchase.o: CPurchase.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CPurchase.o CPurchase.cpp

${OBJECTDIR}/CPurgeUserSessions.o: CPurgeUserSessions.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CPurgeUserSessions.o CPurgeUserSessions.cpp

${OBJECTDIR}/CUser.o: CUser.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CUser.o CUser.cpp

${OBJECTDIR}/CUserType.o: CUserType.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CUserType.o CUserType.cpp

${OBJECTDIR}/CWebSession.o: CWebSession.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CWebSession.o CWebSession.cpp

${OBJECTDIR}/seadminapi.o: seadminapi.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/seadminapi.o seadminapi.cpp

# Subprojects
.build-subprojects:
	cd ../sesecurities && ${MAKE}  -f Makefile CONF=Debug_mysql
	cd ../setraining && ${MAKE}  -f Makefile CONF=Debug_mysql
	cd ../semysql && ${MAKE}  -f Makefile CONF=Debug_mysql
	cd ../sedata && ${MAKE}  -f Makefile CONF=Debug
	cd ../semath && ${MAKE}  -f Makefile CONF=Debug_mysql
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Debug
	cd ../external/mariadbpp && ${MAKE}  -f Makefile CONF=Debug_mysql

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} -r ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsesecurities.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsetraining.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemysql.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsedata.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemath.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseglobal.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmariadbpp.so
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseadmin.${CND_DLIB_EXT}

# Subprojects
.clean-subprojects:
	cd ../sesecurities && ${MAKE}  -f Makefile CONF=Debug_mysql clean
	cd ../setraining && ${MAKE}  -f Makefile CONF=Debug_mysql clean
	cd ../semysql && ${MAKE}  -f Makefile CONF=Debug_mysql clean
	cd ../sedata && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../semath && ${MAKE}  -f Makefile CONF=Debug_mysql clean
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../external/mariadbpp && ${MAKE}  -f Makefile CONF=Debug_mysql clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
