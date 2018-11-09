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
CND_PLATFORM=GNU-Linux-x86
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
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/my_skin.o


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
LDLIBSOPTIONS=-lcppcms -lbooster -Wl,-rpath,../semain/dist/Debug/GNU-Linux-x86 -L../semain/dist/Debug/GNU-Linux-x86 -lsemain -Wl,-rpath,../sexbrl/dist/Debug/GNU-Linux-x86 -L../sexbrl/dist/Debug/GNU-Linux-x86 -lsexbrl ../senet/dist/Debug/GNU-Linux-x86/libsenet.a ../seglobal/dist/Debug/GNU-Linux-x86/libseglobal.a ../semysql/dist/Debug/GNU-Linux-x86/libsemysql.a -lboost_regex -lboost_date_time -lcurl -llog4cxx -lboost_filesystem -lboost_system -lzmq -lmysqlpp -lxml2 -ljsoncpp -lmysqlclient

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/cppcmstest1

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/cppcmstest1: ../semain/dist/Debug/GNU-Linux-x86/libsemain.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/cppcmstest1: ../sexbrl/dist/Debug/GNU-Linux-x86/libsexbrl.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/cppcmstest1: ../senet/dist/Debug/GNU-Linux-x86/libsenet.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/cppcmstest1: ../seglobal/dist/Debug/GNU-Linux-x86/libseglobal.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/cppcmstest1: ../semysql/dist/Debug/GNU-Linux-x86/libsemysql.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/cppcmstest1: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/cppcmstest1 ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/mysql -I/usr/include/mysql++ -I/usr/local/include -I/usr/include/libxml2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/my_skin.o: my_skin.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/mysql -I/usr/include/mysql++ -I/usr/local/include -I/usr/include/libxml2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/my_skin.o my_skin.cpp

# Subprojects
.build-subprojects:
	cd ../semain && ${MAKE}  -f Makefile CONF=Debug
	cd ../sexbrl && ${MAKE}  -f Makefile CONF=Debug
	cd ../senet && ${MAKE}  -f Makefile CONF=Debug
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Debug
	cd ../semysql && ${MAKE}  -f Makefile CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/cppcmstest1

# Subprojects
.clean-subprojects:
	cd ../semain && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../sexbrl && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../senet && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../seglobal && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../semysql && ${MAKE}  -f Makefile CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
