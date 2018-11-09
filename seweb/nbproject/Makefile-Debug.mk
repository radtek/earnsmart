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
	${OBJECTDIR}/CAppBase.o \
	${OBJECTDIR}/CAppMain.o \
	${OBJECTDIR}/CBrowserSession.o \
	${OBJECTDIR}/CInvestmentProducts.o \
	${OBJECTDIR}/CJsonRPCApp.o \
	${OBJECTDIR}/CSPSerializable.o \
	${OBJECTDIR}/emailtmpls.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/skdefault.o


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
LDLIBSOPTIONS=-Wl,-rpath,'../sesecurities/dist/Debug/GNU-Linux' -L../sesecurities/dist/Debug/GNU-Linux -lsesecurities -Wl,-rpath,'../sexbrl/dist/Debug/GNU-Linux' -L../sexbrl/dist/Debug/GNU-Linux -lsexbrl -Wl,-rpath,'../secubist/dist/Debug/GNU-Linux' -L../secubist/dist/Debug/GNU-Linux -lsecubist -Wl,-rpath,'../sewebmodels/dist/Debug/GNU-Linux' -L../sewebmodels/dist/Debug/GNU-Linux -lsewebmodels -Wl,-rpath,'../semain/dist/Debug/GNU-Linux' -L../semain/dist/Debug/GNU-Linux -lsemain -Wl,-rpath,'../seadmin/dist/Debug/GNU-Linux' -L../seadmin/dist/Debug/GNU-Linux -lseadmin -Wl,-rpath,'../seeconomy/dist/Debug/GNU-Linux' -L../seeconomy/dist/Debug/GNU-Linux -lseeconomy -Wl,-rpath,'../setraining/dist/Debug/GNU-Linux' -L../setraining/dist/Debug/GNU-Linux -lsetraining -Wl,-rpath,'../seann/dist/Debug/GNU-Linux' -L../seann/dist/Debug/GNU-Linux -lseann ../semysql/dist/Debug/GNU-Linux/libsemysql.a ../seglobal/dist/Debug/GNU-Linux/libseglobal.a -Wl,-rpath,'../external/mariadbpp/dist/Debug/GNU-Linux' -L../external/mariadbpp/dist/Debug/GNU-Linux -lmariadbpp -Wl,-rpath,'../senet/dist/Debug/GNU-Linux' -L../senet/dist/Debug/GNU-Linux -lsenet -lZThread -ljsoncpp -lmariadbclient_r -lPocoFoundation -lcgicc -lbooster -lcppcms -luuid

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/seweb

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/seweb: ../sesecurities/dist/Debug/GNU-Linux/libsesecurities.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/seweb: ../sexbrl/dist/Debug/GNU-Linux/libsexbrl.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/seweb: ../secubist/dist/Debug/GNU-Linux/libsecubist.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/seweb: ../sewebmodels/dist/Debug/GNU-Linux/libsewebmodels.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/seweb: ../semain/dist/Debug/GNU-Linux/libsemain.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/seweb: ../seadmin/dist/Debug/GNU-Linux/libseadmin.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/seweb: ../seeconomy/dist/Debug/GNU-Linux/libseeconomy.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/seweb: ../setraining/dist/Debug/GNU-Linux/libsetraining.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/seweb: ../seann/dist/Debug/GNU-Linux/libseann.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/seweb: ../semysql/dist/Debug/GNU-Linux/libsemysql.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/seweb: ../seglobal/dist/Debug/GNU-Linux/libseglobal.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/seweb: ../external/mariadbpp/dist/Debug/GNU-Linux/libmariadbpp.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/seweb: ../senet/dist/Debug/GNU-Linux/libsenet.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/seweb: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/seweb ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/CAppBase.o: CAppBase.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -D__PAID__ -I/usr/include/mysql -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CAppBase.o CAppBase.cpp

${OBJECTDIR}/CAppMain.o: CAppMain.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -D__PAID__ -I/usr/include/mysql -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CAppMain.o CAppMain.cpp

${OBJECTDIR}/CBrowserSession.o: CBrowserSession.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -D__PAID__ -I/usr/include/mysql -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CBrowserSession.o CBrowserSession.cpp

${OBJECTDIR}/CInvestmentProducts.o: CInvestmentProducts.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -D__PAID__ -I/usr/include/mysql -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CInvestmentProducts.o CInvestmentProducts.cpp

${OBJECTDIR}/CJsonRPCApp.o: CJsonRPCApp.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -D__PAID__ -I/usr/include/mysql -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CJsonRPCApp.o CJsonRPCApp.cpp

${OBJECTDIR}/CSPSerializable.o: CSPSerializable.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -D__PAID__ -I/usr/include/mysql -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CSPSerializable.o CSPSerializable.cpp

${OBJECTDIR}/emailtmpls.o: emailtmpls.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -D__PAID__ -I/usr/include/mysql -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/emailtmpls.o emailtmpls.cpp

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -D__PAID__ -I/usr/include/mysql -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/skdefault.o: skdefault.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -D__PAID__ -I/usr/include/mysql -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/skdefault.o skdefault.cpp

# Subprojects
.build-subprojects:
	cd ../sesecurities && ${MAKE}  -f Makefile CONF=Debug
	cd ../sexbrl && ${MAKE}  -f Makefile CONF=Debug
	cd ../secubist && ${MAKE}  -f Makefile CONF=Debug
	cd ../sewebmodels && ${MAKE}  -f Makefile CONF=Debug
	cd ../semain && ${MAKE}  -f Makefile CONF=Debug
	cd ../seadmin && ${MAKE}  -f Makefile CONF=Debug
	cd ../seeconomy && ${MAKE}  -f Makefile CONF=Debug
	cd ../setraining && ${MAKE}  -f Makefile CONF=Debug
	cd ../seann && ${MAKE}  -f Makefile CONF=Debug
	cd ../semysql && ${MAKE}  -f Makefile CONF=Debug
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Debug
	cd ../external/mariadbpp && ${MAKE}  -f Makefile CONF=Debug
	cd ../senet && ${MAKE}  -f Makefile CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} -r ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsesecurities.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsexbrl.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsecubist.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsewebmodels.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemain.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseadmin.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeconomy.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsetraining.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseann.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmariadbpp.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsenet.so
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/seweb

# Subprojects
.clean-subprojects:
	cd ../sesecurities && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../sexbrl && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../secubist && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../sewebmodels && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../semain && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../seadmin && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../seeconomy && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../setraining && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../seann && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../semysql && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../external/mariadbpp && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../senet && ${MAKE}  -f Makefile CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
