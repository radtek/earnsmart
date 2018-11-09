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
CCFLAGS=-O2
CXXFLAGS=-O2

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L/usr/lib/x86_64-linux-gnu -L/usr/lib/x86_64-linux-gnu/glib-2.0 -Wl,-rpath,'../../../../commonlib/nx/dist/Debug/None-Linux' -L../../../../commonlib/nx/dist/Debug/None-Linux -lnx -Wl,-rpath,'../semain/dist/Debug/GNU-Linux' -L../semain/dist/Debug/GNU-Linux -lsemain -Wl,-rpath,'../seadmin/dist/Debug/GNU-Linux' -L../seadmin/dist/Debug/GNU-Linux -lseadmin -Wl,-rpath,'../sexbrl/dist/Debug/GNU-Linux' -L../sexbrl/dist/Debug/GNU-Linux -lsexbrl -Wl,-rpath,'../sesecurities/dist/Debug/GNU-Linux' -L../sesecurities/dist/Debug/GNU-Linux -lsesecurities -Wl,-rpath,'../seeconomy/dist/Debug/GNU-Linux' -L../seeconomy/dist/Debug/GNU-Linux -lseeconomy -Wl,-rpath,'../seeoddata/dist/Debug/GNU-Linux' -L../seeoddata/dist/Debug/GNU-Linux -lseeoddata -Wl,-rpath,'../sepredict/dist/Debug/GNU-Linux' -L../sepredict/dist/Debug/GNU-Linux -lsepredict -Wl,-rpath,'../seann/dist/Debug/GNU-Linux' -L../seann/dist/Debug/GNU-Linux -lseann -Wl,-rpath,'../setraining/dist/Debug/GNU-Linux' -L../setraining/dist/Debug/GNU-Linux -lsetraining -Wl,-rpath,'../secubist/dist/Debug/GNU-Linux' -L../secubist/dist/Debug/GNU-Linux -lsecubist -Wl,-rpath,'../sedata/dist/Debug/GNU-Linux' -L../sedata/dist/Debug/GNU-Linux -lsedata -Wl,-rpath,'../semysql/dist/Debug/GNU-Linux' -L../semysql/dist/Debug/GNU-Linux -lsemysql -Wl,-rpath,'../senet/dist/Debug/GNU-Linux' -L../senet/dist/Debug/GNU-Linux -lsenet -Wl,-rpath,'../semath/dist/Debug/GNU-Linux' -L../semath/dist/Debug/GNU-Linux -lsemath -Wl,-rpath,'../seglobal/dist/Debug/GNU-Linux' -L../seglobal/dist/Debug/GNU-Linux -lseglobal -lpthread -lgnutls -lMagickWand-6.Q16 -lMagickCore-6.Q16 -lpython2.7 -lglib-2.0 -ldl -ljsoncpp -lboost_date_time

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libserpc.${CND_DLIB_EXT}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libserpc.${CND_DLIB_EXT}: ../../../../commonlib/nx/dist/Debug/None-Linux/libnx.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libserpc.${CND_DLIB_EXT}: ../semain/dist/Debug/GNU-Linux/libsemain.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libserpc.${CND_DLIB_EXT}: ../seadmin/dist/Debug/GNU-Linux/libseadmin.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libserpc.${CND_DLIB_EXT}: ../sexbrl/dist/Debug/GNU-Linux/libsexbrl.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libserpc.${CND_DLIB_EXT}: ../sesecurities/dist/Debug/GNU-Linux/libsesecurities.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libserpc.${CND_DLIB_EXT}: ../seeconomy/dist/Debug/GNU-Linux/libseeconomy.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libserpc.${CND_DLIB_EXT}: ../seeoddata/dist/Debug/GNU-Linux/libseeoddata.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libserpc.${CND_DLIB_EXT}: ../sepredict/dist/Debug/GNU-Linux/libsepredict.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libserpc.${CND_DLIB_EXT}: ../seann/dist/Debug/GNU-Linux/libseann.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libserpc.${CND_DLIB_EXT}: ../setraining/dist/Debug/GNU-Linux/libsetraining.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libserpc.${CND_DLIB_EXT}: ../secubist/dist/Debug/GNU-Linux/libsecubist.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libserpc.${CND_DLIB_EXT}: ../sedata/dist/Debug/GNU-Linux/libsedata.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libserpc.${CND_DLIB_EXT}: ../semysql/dist/Debug/GNU-Linux/libsemysql.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libserpc.${CND_DLIB_EXT}: ../senet/dist/Debug/GNU-Linux/libsenet.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libserpc.${CND_DLIB_EXT}: ../semath/dist/Debug/GNU-Linux/libsemath.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libserpc.${CND_DLIB_EXT}: ../seglobal/dist/Debug/GNU-Linux/libseglobal.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libserpc.${CND_DLIB_EXT}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libserpc.${CND_DLIB_EXT} ${OBJECTFILES} ${LDLIBSOPTIONS} -fPIC -shared -shared -fPIC

${OBJECTDIR}/CAdminController.o: CAdminController.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I../../../../commonlib/nx -I/usr/include/mariadb -I/usr/include/jsoncpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CAdminController.o CAdminController.cpp

${OBJECTDIR}/CCommonController.o: CCommonController.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I../../../../commonlib/nx -I/usr/include/mariadb -I/usr/include/jsoncpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CCommonController.o CCommonController.cpp

${OBJECTDIR}/CEarnsmartController.o: CEarnsmartController.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I../../../../commonlib/nx -I/usr/include/mariadb -I/usr/include/jsoncpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CEarnsmartController.o CEarnsmartController.cpp

${OBJECTDIR}/CEconomicsController.o: CEconomicsController.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I../../../../commonlib/nx -I/usr/include/mariadb -I/usr/include/jsoncpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CEconomicsController.o CEconomicsController.cpp

${OBJECTDIR}/CEmailNotifications.o: CEmailNotifications.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I../../../../commonlib/nx -I/usr/include/mariadb -I/usr/include/jsoncpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CEmailNotifications.o CEmailNotifications.cpp

${OBJECTDIR}/CHelpDocTreeController.o: CHelpDocTreeController.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I../../../../commonlib/nx -I/usr/include/mariadb -I/usr/include/jsoncpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CHelpDocTreeController.o CHelpDocTreeController.cpp

${OBJECTDIR}/CHttpConnection.o: CHttpConnection.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I../../../../commonlib/nx -I/usr/include/mariadb -I/usr/include/jsoncpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CHttpConnection.o CHttpConnection.cpp

${OBJECTDIR}/CHttpContext.o: CHttpContext.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I../../../../commonlib/nx -I/usr/include/mariadb -I/usr/include/jsoncpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CHttpContext.o CHttpContext.cpp

${OBJECTDIR}/CHttpCookie.o: CHttpCookie.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I../../../../commonlib/nx -I/usr/include/mariadb -I/usr/include/jsoncpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CHttpCookie.o CHttpCookie.cpp

${OBJECTDIR}/CHttpHeaders.o: CHttpHeaders.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I../../../../commonlib/nx -I/usr/include/mariadb -I/usr/include/jsoncpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CHttpHeaders.o CHttpHeaders.cpp

${OBJECTDIR}/CHttpParameters.o: CHttpParameters.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I../../../../commonlib/nx -I/usr/include/mariadb -I/usr/include/jsoncpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CHttpParameters.o CHttpParameters.cpp

${OBJECTDIR}/CHttpRequest.o: CHttpRequest.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I../../../../commonlib/nx -I/usr/include/mariadb -I/usr/include/jsoncpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CHttpRequest.o CHttpRequest.cpp

${OBJECTDIR}/CHttpResponse.o: CHttpResponse.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I../../../../commonlib/nx -I/usr/include/mariadb -I/usr/include/jsoncpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CHttpResponse.o CHttpResponse.cpp

${OBJECTDIR}/CHttpUtils.o: CHttpUtils.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I../../../../commonlib/nx -I/usr/include/mariadb -I/usr/include/jsoncpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CHttpUtils.o CHttpUtils.cpp

${OBJECTDIR}/CMapIterator.o: CMapIterator.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I../../../../commonlib/nx -I/usr/include/mariadb -I/usr/include/jsoncpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CMapIterator.o CMapIterator.cpp

${OBJECTDIR}/CRPCServiceController.o: CRPCServiceController.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I../../../../commonlib/nx -I/usr/include/mariadb -I/usr/include/jsoncpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CRPCServiceController.o CRPCServiceController.cpp

${OBJECTDIR}/CRequest.o: CRequest.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I../../../../commonlib/nx -I/usr/include/mariadb -I/usr/include/jsoncpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CRequest.o CRequest.cpp

${OBJECTDIR}/CResponse.o: CResponse.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I../../../../commonlib/nx -I/usr/include/mariadb -I/usr/include/jsoncpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CResponse.o CResponse.cpp

${OBJECTDIR}/CSecurityController.o: CSecurityController.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I../../../../commonlib/nx -I/usr/include/mariadb -I/usr/include/jsoncpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CSecurityController.o CSecurityController.cpp

${OBJECTDIR}/CServiceRouter.o: CServiceRouter.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I../../../../commonlib/nx -I/usr/include/mariadb -I/usr/include/jsoncpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CServiceRouter.o CServiceRouter.cpp

${OBJECTDIR}/CSessionFilter.o: CSessionFilter.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I../../../../commonlib/nx -I/usr/include/mariadb -I/usr/include/jsoncpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CSessionFilter.o CSessionFilter.cpp

${OBJECTDIR}/rpc.o: rpc.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -D__DEBUG__ -I/usr/include/python2.7 -I/usr/include/ImageMagick-6 -I/usr/include/x86_64-linux-gnu/ImageMagick-6 -I/usr/lib/x86_64-linux-gnu/ImageMagick-6.8.9 -I../../../../commonlib/nx -std=c11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/rpc.o rpc.c

${OBJECTDIR}/seappmodule++.o: seappmodule++.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I../../../../commonlib/nx -I/usr/include/mariadb -I/usr/include/jsoncpp -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/seappmodule++.o seappmodule++.cpp

${OBJECTDIR}/seappmodule.o: seappmodule.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -D__DEBUG__ -I/usr/include/python2.7 -I/usr/include/ImageMagick-6 -I/usr/include/x86_64-linux-gnu/ImageMagick-6 -I/usr/lib/x86_64-linux-gnu/ImageMagick-6.8.9 -I../../../../commonlib/nx -std=c11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/seappmodule.o seappmodule.c

${OBJECTDIR}/sessionfilter.o: sessionfilter.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -D__DEBUG__ -I/usr/include/python2.7 -I/usr/include/ImageMagick-6 -I/usr/include/x86_64-linux-gnu/ImageMagick-6 -I/usr/lib/x86_64-linux-gnu/ImageMagick-6.8.9 -I../../../../commonlib/nx -std=c11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/sessionfilter.o sessionfilter.c

# Subprojects
.build-subprojects:
	cd ../../../../commonlib/nx && ${MAKE} CND_SOLUTIONDIR=/home/santony/Documents/smartearn/Linux/phases/phase3/lib -f Makefile CONF=Debug
	cd ../semain && ${MAKE} CND_SOLUTIONDIR=/home/santony/Documents/smartearn/Linux/phases/phase3/lib -f Makefile CONF=Debug
	cd ../seadmin && ${MAKE} CND_SOLUTIONDIR=/home/santony/Documents/smartearn/Linux/phases/phase3/lib -f Makefile CONF=Debug
	cd ../sexbrl && ${MAKE} CND_SOLUTIONDIR=/home/santony/Documents/smartearn/Linux/phases/phase3/lib -f Makefile CONF=Debug
	cd ../sesecurities && ${MAKE} CND_SOLUTIONDIR=/home/santony/Documents/smartearn/Linux/phases/phase3/lib -f Makefile CONF=Debug
	cd ../seeconomy && ${MAKE} CND_SOLUTIONDIR=/home/santony/Documents/smartearn/Linux/phases/phase3/lib -f Makefile CONF=Debug
	cd ../seeoddata && ${MAKE} CND_SOLUTIONDIR=/home/santony/Documents/smartearn/Linux/phases/phase3/lib -f Makefile CONF=Debug
	cd ../sepredict && ${MAKE} CND_SOLUTIONDIR=/home/santony/Documents/smartearn/Linux/phases/phase3/lib -f Makefile CONF=Debug
	cd ../seann && ${MAKE} CND_SOLUTIONDIR=/home/santony/Documents/smartearn/Linux/phases/phase3/lib -f Makefile CONF=Debug
	cd ../setraining && ${MAKE} CND_SOLUTIONDIR=/home/santony/Documents/smartearn/Linux/phases/phase3/lib -f Makefile CONF=Debug
	cd ../secubist && ${MAKE} CND_SOLUTIONDIR=/home/santony/Documents/smartearn/Linux/phases/phase3/lib -f Makefile CONF=Debug
	cd ../sedata && ${MAKE} CND_SOLUTIONDIR=/home/santony/Documents/smartearn/Linux/phases/phase3/lib -f Makefile CONF=Debug
	cd ../semysql && ${MAKE} CND_SOLUTIONDIR=/home/santony/Documents/smartearn/Linux/phases/phase3/lib -f Makefile CONF=Debug
	cd ../senet && ${MAKE} CND_SOLUTIONDIR=/home/santony/Documents/smartearn/Linux/phases/phase3/lib -f Makefile CONF=Debug
	cd ../semath && ${MAKE} CND_SOLUTIONDIR=/home/santony/Documents/smartearn/Linux/phases/phase3/lib -f Makefile CONF=Debug
	cd ../seglobal && ${MAKE} CND_SOLUTIONDIR=/home/santony/Documents/smartearn/Linux/phases/phase3/lib -f Makefile CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} -r ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libnx.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemain.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseadmin.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsexbrl.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsesecurities.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeconomy.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeoddata.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsepredict.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseann.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsetraining.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsecubist.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsedata.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemysql.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsenet.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemath.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseglobal.so
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libserpc.${CND_DLIB_EXT}

# Subprojects
.clean-subprojects:
	cd ../../../../commonlib/nx && ${MAKE} CND_SOLUTIONDIR=/home/santony/Documents/smartearn/Linux/phases/phase3/lib -f Makefile CONF=Debug clean
	cd ../semain && ${MAKE} CND_SOLUTIONDIR=/home/santony/Documents/smartearn/Linux/phases/phase3/lib -f Makefile CONF=Debug clean
	cd ../seadmin && ${MAKE} CND_SOLUTIONDIR=/home/santony/Documents/smartearn/Linux/phases/phase3/lib -f Makefile CONF=Debug clean
	cd ../sexbrl && ${MAKE} CND_SOLUTIONDIR=/home/santony/Documents/smartearn/Linux/phases/phase3/lib -f Makefile CONF=Debug clean
	cd ../sesecurities && ${MAKE} CND_SOLUTIONDIR=/home/santony/Documents/smartearn/Linux/phases/phase3/lib -f Makefile CONF=Debug clean
	cd ../seeconomy && ${MAKE} CND_SOLUTIONDIR=/home/santony/Documents/smartearn/Linux/phases/phase3/lib -f Makefile CONF=Debug clean
	cd ../seeoddata && ${MAKE} CND_SOLUTIONDIR=/home/santony/Documents/smartearn/Linux/phases/phase3/lib -f Makefile CONF=Debug clean
	cd ../sepredict && ${MAKE} CND_SOLUTIONDIR=/home/santony/Documents/smartearn/Linux/phases/phase3/lib -f Makefile CONF=Debug clean
	cd ../seann && ${MAKE} CND_SOLUTIONDIR=/home/santony/Documents/smartearn/Linux/phases/phase3/lib -f Makefile CONF=Debug clean
	cd ../setraining && ${MAKE} CND_SOLUTIONDIR=/home/santony/Documents/smartearn/Linux/phases/phase3/lib -f Makefile CONF=Debug clean
	cd ../secubist && ${MAKE} CND_SOLUTIONDIR=/home/santony/Documents/smartearn/Linux/phases/phase3/lib -f Makefile CONF=Debug clean
	cd ../sedata && ${MAKE} CND_SOLUTIONDIR=/home/santony/Documents/smartearn/Linux/phases/phase3/lib -f Makefile CONF=Debug clean
	cd ../semysql && ${MAKE} CND_SOLUTIONDIR=/home/santony/Documents/smartearn/Linux/phases/phase3/lib -f Makefile CONF=Debug clean
	cd ../senet && ${MAKE} CND_SOLUTIONDIR=/home/santony/Documents/smartearn/Linux/phases/phase3/lib -f Makefile CONF=Debug clean
	cd ../semath && ${MAKE} CND_SOLUTIONDIR=/home/santony/Documents/smartearn/Linux/phases/phase3/lib -f Makefile CONF=Debug clean
	cd ../seglobal && ${MAKE} CND_SOLUTIONDIR=/home/santony/Documents/smartearn/Linux/phases/phase3/lib -f Makefile CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
