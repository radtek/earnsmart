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
LDLIBSOPTIONS=-Wl,-rpath,'../seadmin/dist/Debug_mysql/GNU-Linux' -L../seadmin/dist/Debug_mysql/GNU-Linux -lseadmin -Wl,-rpath,'../seeconomy/dist/Debug_mysql/GNU-Linux' -L../seeconomy/dist/Debug_mysql/GNU-Linux -lseeconomy -Wl,-rpath,'../sesecurities/dist/Debug_mysql/GNU-Linux' -L../sesecurities/dist/Debug_mysql/GNU-Linux -lsesecurities -Wl,-rpath,'../sexbrl/dist/Debug_mysql/GNU-Linux' -L../sexbrl/dist/Debug_mysql/GNU-Linux -lsexbrl -Wl,-rpath,'../seeoddata/dist/Debug_mysql/GNU-Linux' -L../seeoddata/dist/Debug_mysql/GNU-Linux -lseeoddata -Wl,-rpath,'../semysql/dist/Debug_mysql/GNU-Linux' -L../semysql/dist/Debug_mysql/GNU-Linux -lsemysql -Wl,-rpath,'../sedata/dist/Debug/GNU-Linux' -L../sedata/dist/Debug/GNU-Linux -lsedata -Wl,-rpath,'../seglobal/dist/Debug/GNU-Linux' -L../seglobal/dist/Debug/GNU-Linux -lseglobal -Wl,-rpath,'../external/mariadbpp/dist/Debug_mysql/GNU-Linux' -L../external/mariadbpp/dist/Debug_mysql/GNU-Linux -lmariadbpp -lrt -lPocoFoundation -lZThread -lcrypto++

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemain.${CND_DLIB_EXT}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemain.${CND_DLIB_EXT}: ../seadmin/dist/Debug_mysql/GNU-Linux/libseadmin.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemain.${CND_DLIB_EXT}: ../seeconomy/dist/Debug_mysql/GNU-Linux/libseeconomy.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemain.${CND_DLIB_EXT}: ../sesecurities/dist/Debug_mysql/GNU-Linux/libsesecurities.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemain.${CND_DLIB_EXT}: ../sexbrl/dist/Debug_mysql/GNU-Linux/libsexbrl.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemain.${CND_DLIB_EXT}: ../seeoddata/dist/Debug_mysql/GNU-Linux/libseeoddata.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemain.${CND_DLIB_EXT}: ../semysql/dist/Debug_mysql/GNU-Linux/libsemysql.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemain.${CND_DLIB_EXT}: ../sedata/dist/Debug/GNU-Linux/libsedata.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemain.${CND_DLIB_EXT}: ../seglobal/dist/Debug/GNU-Linux/libseglobal.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemain.${CND_DLIB_EXT}: ../external/mariadbpp/dist/Debug_mysql/GNU-Linux/libmariadbpp.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemain.${CND_DLIB_EXT}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemain.${CND_DLIB_EXT} ${OBJECTFILES} ${LDLIBSOPTIONS} -fPIC -fvisibility=hidden -shared -fPIC

${OBJECTDIR}/CApplication.o: CApplication.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CApplication.o CApplication.cpp

${OBJECTDIR}/CConfigItem.o: CConfigItem.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CConfigItem.o CConfigItem.cpp

${OBJECTDIR}/CConfigurator.o: CConfigurator.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CConfigurator.o CConfigurator.cpp

${OBJECTDIR}/CMain.o: CMain.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CMain.o CMain.cpp

${OBJECTDIR}/CNamedObjectList.o: CNamedObjectList.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CNamedObjectList.o CNamedObjectList.cpp

${OBJECTDIR}/CSession.o: CSession.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CSession.o CSession.cpp

${OBJECTDIR}/CSessionActivity.o: CSessionActivity.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CSessionActivity.o CSessionActivity.cpp

# Subprojects
.build-subprojects:
	cd ../seadmin && ${MAKE}  -f Makefile CONF=Debug_mysql
	cd ../seeconomy && ${MAKE}  -f Makefile CONF=Debug_mysql
	cd ../sesecurities && ${MAKE}  -f Makefile CONF=Debug_mysql
	cd ../sexbrl && ${MAKE}  -f Makefile CONF=Debug_mysql
	cd ../seeoddata && ${MAKE}  -f Makefile CONF=Debug_mysql
	cd ../semysql && ${MAKE}  -f Makefile CONF=Debug_mysql
	cd ../sedata && ${MAKE}  -f Makefile CONF=Debug
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Debug
	cd ../external/mariadbpp && ${MAKE}  -f Makefile CONF=Debug_mysql

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} -r ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseadmin.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeconomy.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsesecurities.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsexbrl.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseeoddata.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemysql.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsedata.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseglobal.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmariadbpp.so
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemain.${CND_DLIB_EXT}

# Subprojects
.clean-subprojects:
	cd ../seadmin && ${MAKE}  -f Makefile CONF=Debug_mysql clean
	cd ../seeconomy && ${MAKE}  -f Makefile CONF=Debug_mysql clean
	cd ../sesecurities && ${MAKE}  -f Makefile CONF=Debug_mysql clean
	cd ../sexbrl && ${MAKE}  -f Makefile CONF=Debug_mysql clean
	cd ../seeoddata && ${MAKE}  -f Makefile CONF=Debug_mysql clean
	cd ../semysql && ${MAKE}  -f Makefile CONF=Debug_mysql clean
	cd ../sedata && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../external/mariadbpp && ${MAKE}  -f Makefile CONF=Debug_mysql clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
