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
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_CONF=AMD-Linux-Debug
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/Source.o

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
LDLIBSOPTIONS=-L../../../Dependencies/GLee/Lib/GCC -L../../../Dependencies/GLFW/Lib/GCC -L../../../Dependencies/OpenCL/Lib/AMD/GCC -L../../Base\ Libraries/Base\ Render\ Library/dist/Linux-Debug/GNU-Linux-x86 -Wl,-rpath ../../../Dependencies/GLee/Lib/GCC -Wl,-rpath ../../../Dependencies/GLFW/Lib/GCC -Wl,-rpath ../../../Dependencies/OpenCL/Lib/AMD/GCC -lGLee -lglfw -lOpenCL -lGL -lGLU -lbase_render_library

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-AMD-Linux-Debug.mk dist/AMD-Linux-Debug/GNU-Linux-x86/opengl_test

dist/AMD-Linux-Debug/GNU-Linux-x86/opengl_test: ${OBJECTFILES}
	${MKDIR} -p dist/AMD-Linux-Debug/GNU-Linux-x86
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/opengl_test ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/Source.o: nbproject/Makefile-${CND_CONF}.mk Source.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -I../../../Dependencies/GLee/Include -I../../../Dependencies/GLFW/Include -I../../../Dependencies/OpenCL/Include/AMD -I../../Base\ Libraries/Base\ Math\ Library -I../../Base\ Libraries/Base\ Render\ Library -MMD -MP -MF $@.d -o ${OBJECTDIR}/Source.o Source.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/AMD-Linux-Debug
	${RM} dist/AMD-Linux-Debug/GNU-Linux-x86/opengl_test

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
