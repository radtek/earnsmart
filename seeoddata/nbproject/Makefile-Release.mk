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
	${OBJECTDIR}/CEODDataWrappers.o \
	${OBJECTDIR}/CEodData.o \
	${OBJECTDIR}/CWSClientSession.o \
	${OBJECTDIR}/ReadEodFile.o \
	${OBJECTDIR}/ReadExchanges.o \
	${OBJECTDIR}/envC.o \
	${OBJECTDIR}/nsC.o \
	${OBJECTDIR}/nsDataSoapProxy.o \
	${OBJECTDIR}/seeoddata.o


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
LDLIBSOPTIONS=-L/usr/lib/x86_64-linux-gnu/ -Wl,-rpath,'../semysql/dist/Release/GNU-Linux' -L../semysql/dist/Release/GNU-Linux -lsemysql -Wl,-rpath,'../seglobal/dist/Release/GNU-Linux' -L../seglobal/dist/Release/GNU-Linux -lseglobal -Wl,-rpath,'../external/mariadbpp/dist/Release/GNU-Linux' -L../external/mariadbpp/dist/Release/GNU-Linux -lmariadbpp /usr/lib/x86_64-linux-gnu/libgsoap++.so /usr/lib/x86_64-linux-gnu/libgsoap.so /usr/lib/x86_64-linux-gnu/libgsoapck++.so /usr/lib/x86_64-linux-gnu/libgsoapck.so /usr/lib/x86_64-linux-gnu/libgsoapssl++.so /usr/lib/x86_64-linux-gnu/libgsoapssl.so -lxml2

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeoddata.${CND_DLIB_EXT}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeoddata.${CND_DLIB_EXT}: ../semysql/dist/Release/GNU-Linux/libsemysql.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeoddata.${CND_DLIB_EXT}: ../seglobal/dist/Release/GNU-Linux/libseglobal.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeoddata.${CND_DLIB_EXT}: ../external/mariadbpp/dist/Release/GNU-Linux/libmariadbpp.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeoddata.${CND_DLIB_EXT}: /usr/lib/x86_64-linux-gnu/libgsoap++.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeoddata.${CND_DLIB_EXT}: /usr/lib/x86_64-linux-gnu/libgsoap.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeoddata.${CND_DLIB_EXT}: /usr/lib/x86_64-linux-gnu/libgsoapck++.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeoddata.${CND_DLIB_EXT}: /usr/lib/x86_64-linux-gnu/libgsoapck.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeoddata.${CND_DLIB_EXT}: /usr/lib/x86_64-linux-gnu/libgsoapssl++.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeoddata.${CND_DLIB_EXT}: /usr/lib/x86_64-linux-gnu/libgsoapssl.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeoddata.${CND_DLIB_EXT}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeoddata.${CND_DLIB_EXT} ${OBJECTFILES} ${LDLIBSOPTIONS} -shared -fPIC

${OBJECTDIR}/CEODDataWrappers.o: CEODDataWrappers.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mariadb -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CEODDataWrappers.o CEODDataWrappers.cpp

${OBJECTDIR}/CEodData.o: CEodData.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mariadb -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CEodData.o CEodData.cpp

${OBJECTDIR}/CWSClientSession.o: CWSClientSession.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mariadb -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CWSClientSession.o CWSClientSession.cpp

${OBJECTDIR}/ReadEodFile.o: ReadEodFile.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mariadb -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ReadEodFile.o ReadEodFile.cpp

${OBJECTDIR}/ReadExchanges.o: ReadExchanges.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mariadb -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ReadExchanges.o ReadExchanges.cpp

${OBJECTDIR}/envC.o: envC.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mariadb -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/envC.o envC.cpp

${OBJECTDIR}/nsC.o: nsC.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mariadb -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/nsC.o nsC.cpp

${OBJECTDIR}/nsDataSoapProxy.o: nsDataSoapProxy.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mariadb -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/nsDataSoapProxy.o nsDataSoapProxy.cpp

${OBJECTDIR}/seeoddata.o: seeoddata.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mariadb -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/seeoddata.o seeoddata.cpp

# Subprojects
.build-subprojects:
	cd ../semysql && ${MAKE}  -f Makefile CONF=Release
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Release
	cd ../external/mariadbpp && ${MAKE}  -f Makefile CONF=Release

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} -r ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemysql.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseglobal.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmariadbpp.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libgsoap++.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libgsoap.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libgsoapck++.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libgsoapck.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libgsoapssl++.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libgsoapssl.so
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeoddata.${CND_DLIB_EXT}

# Subprojects
.clean-subprojects:
	cd ../semysql && ${MAKE}  -f Makefile CONF=Release clean
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Release clean
	cd ../external/mariadbpp && ${MAKE}  -f Makefile CONF=Release clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
