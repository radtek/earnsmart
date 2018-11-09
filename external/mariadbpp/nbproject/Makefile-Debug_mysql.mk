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
	${OBJECTDIR}/beemutex.o \
	${OBJECTDIR}/cmdline.o \
	${OBJECTDIR}/connection.o \
	${OBJECTDIR}/cpool.o \
	${OBJECTDIR}/datetime.o \
	${OBJECTDIR}/dbdriver.o \
	${OBJECTDIR}/field_names.o \
	${OBJECTDIR}/field_types.o \
	${OBJECTDIR}/manip.o \
	${OBJECTDIR}/myset.o \
	${OBJECTDIR}/mysql++.o \
	${OBJECTDIR}/mystring.o \
	${OBJECTDIR}/null.o \
	${OBJECTDIR}/options.o \
	${OBJECTDIR}/qparms.o \
	${OBJECTDIR}/query.o \
	${OBJECTDIR}/result.o \
	${OBJECTDIR}/row.o \
	${OBJECTDIR}/scopedconnection.o \
	${OBJECTDIR}/sql_buffer.o \
	${OBJECTDIR}/sqlstream.o \
	${OBJECTDIR}/ssqls2.o \
	${OBJECTDIR}/stadapter.o \
	${OBJECTDIR}/tcp_connection.o \
	${OBJECTDIR}/transaction.o \
	${OBJECTDIR}/type_info.o \
	${OBJECTDIR}/uds_connection.o \
	${OBJECTDIR}/utility.o \
	${OBJECTDIR}/vallist.o \
	${OBJECTDIR}/wnp_connection.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-m64 -fPIC
CXXFLAGS=-m64 -fPIC

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L/usr/lib/x86_64-linux-gnu/ -lmysqlclient -lnsl -lz -lm

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmariadbpp.${CND_DLIB_EXT}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmariadbpp.${CND_DLIB_EXT}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmariadbpp.${CND_DLIB_EXT} ${OBJECTFILES} ${LDLIBSOPTIONS} -shared -fPIC

${OBJECTDIR}/beemutex.o: beemutex.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/beemutex.o beemutex.cpp

${OBJECTDIR}/cmdline.o: cmdline.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cmdline.o cmdline.cpp

${OBJECTDIR}/connection.o: connection.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/connection.o connection.cpp

${OBJECTDIR}/cpool.o: cpool.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cpool.o cpool.cpp

${OBJECTDIR}/datetime.o: datetime.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/datetime.o datetime.cpp

${OBJECTDIR}/dbdriver.o: dbdriver.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/dbdriver.o dbdriver.cpp

${OBJECTDIR}/field_names.o: field_names.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/field_names.o field_names.cpp

${OBJECTDIR}/field_types.o: field_types.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/field_types.o field_types.cpp

${OBJECTDIR}/manip.o: manip.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/manip.o manip.cpp

${OBJECTDIR}/myset.o: myset.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/myset.o myset.cpp

${OBJECTDIR}/mysql++.o: mysql++.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/mysql++.o mysql++.cpp

${OBJECTDIR}/mystring.o: mystring.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/mystring.o mystring.cpp

${OBJECTDIR}/null.o: null.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/null.o null.cpp

${OBJECTDIR}/options.o: options.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/options.o options.cpp

${OBJECTDIR}/qparms.o: qparms.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/qparms.o qparms.cpp

${OBJECTDIR}/query.o: query.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/query.o query.cpp

${OBJECTDIR}/result.o: result.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/result.o result.cpp

${OBJECTDIR}/row.o: row.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/row.o row.cpp

${OBJECTDIR}/scopedconnection.o: scopedconnection.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/scopedconnection.o scopedconnection.cpp

${OBJECTDIR}/sql_buffer.o: sql_buffer.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/sql_buffer.o sql_buffer.cpp

${OBJECTDIR}/sqlstream.o: sqlstream.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/sqlstream.o sqlstream.cpp

${OBJECTDIR}/ssqls2.o: ssqls2.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ssqls2.o ssqls2.cpp

${OBJECTDIR}/stadapter.o: stadapter.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/stadapter.o stadapter.cpp

${OBJECTDIR}/tcp_connection.o: tcp_connection.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/tcp_connection.o tcp_connection.cpp

${OBJECTDIR}/transaction.o: transaction.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/transaction.o transaction.cpp

${OBJECTDIR}/type_info.o: type_info.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/type_info.o type_info.cpp

${OBJECTDIR}/uds_connection.o: uds_connection.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/uds_connection.o uds_connection.cpp

${OBJECTDIR}/utility.o: utility.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/utility.o utility.cpp

${OBJECTDIR}/vallist.o: vallist.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/vallist.o vallist.cpp

${OBJECTDIR}/wnp_connection.o: wnp_connection.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D__DEBUG__ -I/usr/include/mysql -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/wnp_connection.o wnp_connection.cpp

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
