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
	${OBJECTDIR}/CApplication.o \
	${OBJECTDIR}/CConfigItem.o \
	${OBJECTDIR}/CConfigurator.o \
	${OBJECTDIR}/CMain.o \
	${OBJECTDIR}/CNamedObjectList.o \
	${OBJECTDIR}/CSession.o \
	${OBJECTDIR}/CSessionActivity.o


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
LDLIBSOPTIONS=-Wl,-rpath,'../seadmin/dist/Release/GNU-Linux' -L../seadmin/dist/Release/GNU-Linux -lseadmin -Wl,-rpath,'../sesecurities/dist/Release/GNU-Linux' -L../sesecurities/dist/Release/GNU-Linux -lsesecurities -Wl,-rpath,'../sexbrl/dist/Release/GNU-Linux' -L../sexbrl/dist/Release/GNU-Linux -lsexbrl -Wl,-rpath,'../sedata/dist/Release/GNU-Linux' -L../sedata/dist/Release/GNU-Linux -lsedata -Wl,-rpath,'../semysql/dist/Release/GNU-Linux' -L../semysql/dist/Release/GNU-Linux -lsemysql -Wl,-rpath,'../seglobal/dist/Release/GNU-Linux' -L../seglobal/dist/Release/GNU-Linux -lseglobal -lrt -lboost_system -lboost_filesystem -lboost_locale -luuid

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemain.${CND_DLIB_EXT}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemain.${CND_DLIB_EXT}: ../seadmin/dist/Release/GNU-Linux/libseadmin.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemain.${CND_DLIB_EXT}: ../sesecurities/dist/Release/GNU-Linux/libsesecurities.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemain.${CND_DLIB_EXT}: ../sexbrl/dist/Release/GNU-Linux/libsexbrl.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemain.${CND_DLIB_EXT}: ../sedata/dist/Release/GNU-Linux/libsedata.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemain.${CND_DLIB_EXT}: ../semysql/dist/Release/GNU-Linux/libsemysql.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemain.${CND_DLIB_EXT}: ../seglobal/dist/Release/GNU-Linux/libseglobal.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemain.${CND_DLIB_EXT}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemain.${CND_DLIB_EXT} ${OBJECTFILES} ${LDLIBSOPTIONS} -fPIC -fvisibility=hidden -shared -fPIC

${OBJECTDIR}/CApplication.o: CApplication.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mariadb -I/usr/include/mysql++ -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CApplication.o CApplication.cpp

${OBJECTDIR}/CConfigItem.o: CConfigItem.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mariadb -I/usr/include/mysql++ -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CConfigItem.o CConfigItem.cpp

${OBJECTDIR}/CConfigurator.o: CConfigurator.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mariadb -I/usr/include/mysql++ -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CConfigurator.o CConfigurator.cpp

${OBJECTDIR}/CMain.o: CMain.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mariadb -I/usr/include/mysql++ -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CMain.o CMain.cpp

${OBJECTDIR}/CNamedObjectList.o: CNamedObjectList.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mariadb -I/usr/include/mysql++ -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CNamedObjectList.o CNamedObjectList.cpp

${OBJECTDIR}/CSession.o: CSession.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mariadb -I/usr/include/mysql++ -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CSession.o CSession.cpp

${OBJECTDIR}/CSessionActivity.o: CSessionActivity.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mariadb -I/usr/include/mysql++ -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CSessionActivity.o CSessionActivity.cpp

# Subprojects
.build-subprojects:
	cd ../seadmin && ${MAKE}  -f Makefile CONF=Release
	cd ../sesecurities && ${MAKE}  -f Makefile CONF=Release
	cd ../sexbrl && ${MAKE}  -f Makefile CONF=Release
	cd ../sedata && ${MAKE}  -f Makefile CONF=Release
	cd ../semysql && ${MAKE}  -f Makefile CONF=Release
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Release

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} -r ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseadmin.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsesecurities.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsexbrl.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsedata.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemysql.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseglobal.so
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemain.${CND_DLIB_EXT}

# Subprojects
.clean-subprojects:
	cd ../seadmin && ${MAKE}  -f Makefile CONF=Release clean
	cd ../sesecurities && ${MAKE}  -f Makefile CONF=Release clean
	cd ../sexbrl && ${MAKE}  -f Makefile CONF=Release clean
	cd ../sedata && ${MAKE}  -f Makefile CONF=Release clean
	cd ../semysql && ${MAKE}  -f Makefile CONF=Release clean
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Release clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
