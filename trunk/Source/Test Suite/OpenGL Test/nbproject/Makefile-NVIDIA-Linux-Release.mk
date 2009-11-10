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
CND_CONF=NVIDIA-Linux-Release
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
LDLIBSOPTIONS=-L../../../Dependencies/GLee/Lib/gcc -L../../../Dependencies/GLFW/Lib/gcc -L../../Base\ Libraries/Base\ Render\ Library/dist/Linux-Debug/GNU-Linux-x86 -Wl,-rpath /home/denis/Desktop/RTRT\ on\ GPU/Dependencies/GLee/Lib/gcc -Wl,-rpath /home/denis/Desktop/RTRT\ on\ GPU/Dependencies/GLFW/Lib/gcc -lGLee -lglfw -lGL -lGLU -lbase_render_library

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-NVIDIA-Linux-Release.mk dist/NVIDIA-Linux-Release/GNU-Linux-x86/opengl_test

dist/NVIDIA-Linux-Release/GNU-Linux-x86/opengl_test: ${OBJECTFILES}
	${MKDIR} -p dist/NVIDIA-Linux-Release/GNU-Linux-x86
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/opengl_test ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/Source.o: nbproject/Makefile-${CND_CONF}.mk Source.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../Dependencies/GLee/Include -I../../../Dependencies/GLFW/Include -I../../Base\ Libraries/Base\ Render\ Library -I../../Base\ Libraries/Base\ Math\ Library -MMD -MP -MF $@.d -o ${OBJECTDIR}/Source.o Source.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/NVIDIA-Linux-Release
	${RM} dist/NVIDIA-Linux-Release/GNU-Linux-x86/opengl_test

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
