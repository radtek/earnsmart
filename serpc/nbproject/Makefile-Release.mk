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
	${OBJECTDIR}/CAdminController.o \
	${OBJECTDIR}/CCommonController.o \
	${OBJECTDIR}/CEarnsmartController.o \
	${OBJECTDIR}/CEconomicsController.o \
	${OBJECTDIR}/CEmailNotifications.o \
	${OBJECTDIR}/CHelpDocTreeController.o \
	${OBJECTDIR}/CHttpConnection.o \
	${OBJECTDIR}/CHttpContext.o \
	${OBJECTDIR}/CHttpCookie.o \
	${OBJECTDIR}/CHttpHeaders.o \
	${OBJECTDIR}/CHttpParameters.o \
	${OBJECTDIR}/CHttpRequest.o \
	${OBJECTDIR}/CHttpResponse.o \
	${OBJECTDIR}/CHttpUtils.o \
	${OBJECTDIR}/CMapIterator.o \
	${OBJECTDIR}/CRPCServiceController.o \
	${OBJECTDIR}/CRequest.o \
	${OBJECTDIR}/CResponse.o \
	${OBJECTDIR}/CSecurityController.o \
	${OBJECTDIR}/CServiceRouter.o \
	${OBJECTDIR}/CSessionFilter.o \
	${OBJECTDIR}/rpc.o \
	${OBJECTDIR}/seappmodule++.o \
	${OBJECTDIR}/seappmodule.o \
	${OBJECTDIR}/sessionfilter.o


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
LDLIBSOPTIONS=-L/usr/lib/x86_64-linux-gnu -L/usr/lib/x86_64-linux-gnu/glib-2.0

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libserpc.${CND_DLIB_EXT}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libserpc.${CND_DLIB_EXT}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libserpc.${CND_DLIB_EXT} ${OBJECTFILES} ${LDLIBSOPTIONS} -fPIC -shared -shared -fPIC

${OBJECTDIR}/CAdminController.o: CAdminController.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CAdminController.o CAdminController.cpp

${OBJECTDIR}/CCommonController.o: CCommonController.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CCommonController.o CCommonController.cpp

${OBJECTDIR}/CEarnsmartController.o: CEarnsmartController.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CEarnsmartController.o CEarnsmartController.cpp

${OBJECTDIR}/CEconomicsController.o: CEconomicsController.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CEconomicsController.o CEconomicsController.cpp

${OBJECTDIR}/CEmailNotifications.o: CEmailNotifications.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CEmailNotifications.o CEmailNotifications.cpp

${OBJECTDIR}/CHelpDocTreeController.o: CHelpDocTreeController.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CHelpDocTreeController.o CHelpDocTreeController.cpp

${OBJECTDIR}/CHttpConnection.o: CHttpConnection.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CHttpConnection.o CHttpConnection.cpp

${OBJECTDIR}/CHttpContext.o: CHttpContext.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CHttpContext.o CHttpContext.cpp

${OBJECTDIR}/CHttpCookie.o: CHttpCookie.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CHttpCookie.o CHttpCookie.cpp

${OBJECTDIR}/CHttpHeaders.o: CHttpHeaders.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CHttpHeaders.o CHttpHeaders.cpp

${OBJECTDIR}/CHttpParameters.o: CHttpParameters.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CHttpParameters.o CHttpParameters.cpp

${OBJECTDIR}/CHttpRequest.o: CHttpRequest.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CHttpRequest.o CHttpRequest.cpp

${OBJECTDIR}/CHttpResponse.o: CHttpResponse.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CHttpResponse.o CHttpResponse.cpp

${OBJECTDIR}/CHttpUtils.o: CHttpUtils.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CHttpUtils.o CHttpUtils.cpp

${OBJECTDIR}/CMapIterator.o: CMapIterator.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CMapIterator.o CMapIterator.cpp

${OBJECTDIR}/CRPCServiceController.o: CRPCServiceController.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CRPCServiceController.o CRPCServiceController.cpp

${OBJECTDIR}/CRequest.o: CRequest.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CRequest.o CRequest.cpp

${OBJECTDIR}/CResponse.o: CResponse.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CResponse.o CResponse.cpp

${OBJECTDIR}/CSecurityController.o: CSecurityController.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CSecurityController.o CSecurityController.cpp

${OBJECTDIR}/CServiceRouter.o: CServiceRouter.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CServiceRouter.o CServiceRouter.cpp

${OBJECTDIR}/CSessionFilter.o: CSessionFilter.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CSessionFilter.o CSessionFilter.cpp

${OBJECTDIR}/rpc.o: rpc.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/rpc.o rpc.c

${OBJECTDIR}/seappmodule++.o: seappmodule++.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/seappmodule++.o seappmodule++.cpp

${OBJECTDIR}/seappmodule.o: seappmodule.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/seappmodule.o seappmodule.c

${OBJECTDIR}/sessionfilter.o: sessionfilter.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/sessionfilter.o sessionfilter.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
