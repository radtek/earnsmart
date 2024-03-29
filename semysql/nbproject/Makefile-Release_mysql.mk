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
	${OBJECTDIR}/CMysqlTable.o \
	${OBJECTDIR}/CMysqlUtilities.o \
	${OBJECTDIR}/CSEConnPool.o \
	${OBJECTDIR}/CSEConnections.o \
	${OBJECTDIR}/CStoreQueryResult.o


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
LDLIBSOPTIONS=-Wl,-rpath,'../seglobal/dist/Release/GNU-Linux' -L../seglobal/dist/Release/GNU-Linux -lseglobal

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemysql.${CND_DLIB_EXT}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemysql.${CND_DLIB_EXT}: ../seglobal/dist/Release/GNU-Linux/libseglobal.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemysql.${CND_DLIB_EXT}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemysql.${CND_DLIB_EXT} ${OBJECTFILES} ${LDLIBSOPTIONS} -shared -fPIC

${OBJECTDIR}/CMysqlTable.o: CMysqlTable.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CMysqlTable.o CMysqlTable.cpp

${OBJECTDIR}/CMysqlUtilities.o: CMysqlUtilities.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CMysqlUtilities.o CMysqlUtilities.cpp

${OBJECTDIR}/CSEConnPool.o: CSEConnPool.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CSEConnPool.o CSEConnPool.cpp

${OBJECTDIR}/CSEConnections.o: CSEConnections.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CSEConnections.o CSEConnections.cpp

${OBJECTDIR}/CStoreQueryResult.o: CStoreQueryResult.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CStoreQueryResult.o CStoreQueryResult.cpp

# Subprojects
.build-subprojects:
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Release

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} -r ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libseglobal.so
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsemysql.${CND_DLIB_EXT}

# Subprojects
.clean-subprojects:
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Release clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
