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
	${OBJECTDIR}/CAccount.o \
	${OBJECTDIR}/CAppSearch.o \
	${OBJECTDIR}/CError.o \
	${OBJECTDIR}/CFooter.o \
	${OBJECTDIR}/CHeader.o \
	${OBJECTDIR}/CHome.o \
	${OBJECTDIR}/CLogOn.o \
	${OBJECTDIR}/CLoginOrRegister.o \
	${OBJECTDIR}/CMaster.o \
	${OBJECTDIR}/CModelBase.o \
	${OBJECTDIR}/CPwd.o \
	${OBJECTDIR}/CRegistration.o \
	${OBJECTDIR}/CResetPwd.o \
	${OBJECTDIR}/CSecurityCharts.o \
	${OBJECTDIR}/CSerializationTraits.o \
	${OBJECTDIR}/CSessionManageMethods.o \
	${OBJECTDIR}/CToolbar.o \
	${OBJECTDIR}/CValidationMethods.o \
	${OBJECTDIR}/CWebControl.o \
	${OBJECTDIR}/cppcmsfilters.o


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
LDLIBSOPTIONS=-Wl,-rpath,'../seadmin/dist/Release/None-Linux' -L../seadmin/dist/Release/None-Linux -lseadmin -Wl,-rpath,'../senet/dist/Release/None-Linux' -L../senet/dist/Release/None-Linux -lsenet ../seglobal/dist/Release/None-Linux/libseglobal.a -Wl,-rpath,'../external/mariadbpp/dist/Release/GNU-Linux' -L../external/mariadbpp/dist/Release/GNU-Linux -lmariadbpp -lbooster -lcppcms

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsewebmodels.${CND_DLIB_EXT}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsewebmodels.${CND_DLIB_EXT}: ../seadmin/dist/Release/None-Linux/libseadmin.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsewebmodels.${CND_DLIB_EXT}: ../senet/dist/Release/None-Linux/libsenet.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsewebmodels.${CND_DLIB_EXT}: ../seglobal/dist/Release/None-Linux/libseglobal.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsewebmodels.${CND_DLIB_EXT}: ../external/mariadbpp/dist/Release/GNU-Linux/libmariadbpp.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsewebmodels.${CND_DLIB_EXT}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsewebmodels.${CND_DLIB_EXT} ${OBJECTFILES} ${LDLIBSOPTIONS} -shared -fPIC

${OBJECTDIR}/CAccount.o: CAccount.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mysql -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CAccount.o CAccount.cpp

${OBJECTDIR}/CAppSearch.o: CAppSearch.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mysql -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CAppSearch.o CAppSearch.cpp

${OBJECTDIR}/CError.o: CError.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mysql -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CError.o CError.cpp

${OBJECTDIR}/CFooter.o: CFooter.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mysql -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CFooter.o CFooter.cpp

${OBJECTDIR}/CHeader.o: CHeader.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mysql -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CHeader.o CHeader.cpp

${OBJECTDIR}/CHome.o: CHome.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mysql -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CHome.o CHome.cpp

${OBJECTDIR}/CLogOn.o: CLogOn.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mysql -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CLogOn.o CLogOn.cpp

${OBJECTDIR}/CLoginOrRegister.o: CLoginOrRegister.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mysql -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CLoginOrRegister.o CLoginOrRegister.cpp

${OBJECTDIR}/CMaster.o: CMaster.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mysql -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CMaster.o CMaster.cpp

${OBJECTDIR}/CModelBase.o: CModelBase.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mysql -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CModelBase.o CModelBase.cpp

${OBJECTDIR}/CPwd.o: CPwd.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mysql -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CPwd.o CPwd.cpp

${OBJECTDIR}/CRegistration.o: CRegistration.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mysql -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CRegistration.o CRegistration.cpp

${OBJECTDIR}/CResetPwd.o: CResetPwd.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mysql -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CResetPwd.o CResetPwd.cpp

${OBJECTDIR}/CSecurityCharts.o: CSecurityCharts.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mysql -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CSecurityCharts.o CSecurityCharts.cpp

${OBJECTDIR}/CSerializationTraits.o: CSerializationTraits.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mysql -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CSerializationTraits.o CSerializationTraits.cpp

${OBJECTDIR}/CSessionManageMethods.o: CSessionManageMethods.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mysql -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CSessionManageMethods.o CSessionManageMethods.cpp

${OBJECTDIR}/CToolbar.o: CToolbar.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mysql -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CToolbar.o CToolbar.cpp

${OBJECTDIR}/CValidationMethods.o: CValidationMethods.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mysql -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CValidationMethods.o CValidationMethods.cpp

${OBJECTDIR}/CWebControl.o: CWebControl.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mysql -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CWebControl.o CWebControl.cpp

${OBJECTDIR}/cppcmsfilters.o: cppcmsfilters.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mysql -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cppcmsfilters.o cppcmsfilters.cpp

# Subprojects
.build-subprojects:
	cd ../seadmin && ${MAKE}  -f Makefile CONF=Release
	cd ../senet && ${MAKE}  -f Makefile CONF=Release
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Release
	cd ../external/mariadbpp && ${MAKE}  -f Makefile CONF=Release

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} -r ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseadmin.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsenet.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmariadbpp.so
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsewebmodels.${CND_DLIB_EXT}

# Subprojects
.clean-subprojects:
	cd ../seadmin && ${MAKE}  -f Makefile CONF=Release clean
	cd ../senet && ${MAKE}  -f Makefile CONF=Release clean
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Release clean
	cd ../external/mariadbpp && ${MAKE}  -f Makefile CONF=Release clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
