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
	${OBJECTDIR}/_ext/76b77321/crypt_blowfish.o \
	${OBJECTDIR}/_ext/76b77321/crypt_gensalt.o \
	${OBJECTDIR}/_ext/76b77321/wrapper.o


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
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_SOLUTIONDIR}/${CND_CONF}/${CND_PLATFORM}/libsesecure.a

${CND_SOLUTIONDIR}/${CND_CONF}/${CND_PLATFORM}/libsesecure.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_SOLUTIONDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_SOLUTIONDIR}/${CND_CONF}/${CND_PLATFORM}/libsesecure.a
	${AR} -rv ${CND_SOLUTIONDIR}/${CND_CONF}/${CND_PLATFORM}/libsesecure.a ${OBJECTFILES} 
	$(RANLIB) ${CND_SOLUTIONDIR}/${CND_CONF}/${CND_PLATFORM}/libsesecure.a

${OBJECTDIR}/_ext/76b77321/crypt_blowfish.o: ../bcrypt/1.2/crypt_blowfish.c
	${MKDIR} -p ${OBJECTDIR}/_ext/76b77321
	${RM} "$@.d"
	$(COMPILE.c) -g -D__DEBUG__ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/76b77321/crypt_blowfish.o ../bcrypt/1.2/crypt_blowfish.c

${OBJECTDIR}/_ext/76b77321/crypt_gensalt.o: ../bcrypt/1.2/crypt_gensalt.c
	${MKDIR} -p ${OBJECTDIR}/_ext/76b77321
	${RM} "$@.d"
	$(COMPILE.c) -g -D__DEBUG__ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/76b77321/crypt_gensalt.o ../bcrypt/1.2/crypt_gensalt.c

${OBJECTDIR}/_ext/76b77321/wrapper.o: ../bcrypt/1.2/wrapper.c
	${MKDIR} -p ${OBJECTDIR}/_ext/76b77321
	${RM} "$@.d"
	$(COMPILE.c) -g -D__DEBUG__ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/76b77321/wrapper.o ../bcrypt/1.2/wrapper.c

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
