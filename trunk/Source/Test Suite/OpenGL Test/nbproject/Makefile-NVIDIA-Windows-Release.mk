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
CC=
CCC=
CXX=
FC=
AS=

# Macros
CND_PLATFORM=MinGW-Linux-x86
CND_CONF=NVIDIA-Windows-Release
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
LDLIBSOPTIONS=-L../../../Dependencies/GLee/Lib/MinGW -L../../../Dependencies/GLFW/Lib/MinGW -L../../Base\ Libraries/Base\ Render\ Library/dist/Windows-Release/MinGW-Windows -lbase_render_library -lglfw -lGLee -lopengl32 -lglu32 -lgdi32 -luser32 -lkernel32

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-NVIDIA-Windows-Release.mk dist/NVIDIA-Windows-Release/MinGW-Linux-x86/opengl_test

dist/NVIDIA-Windows-Release/MinGW-Linux-x86/opengl_test: ${OBJECTFILES}
	${MKDIR} -p dist/NVIDIA-Windows-Release/MinGW-Linux-x86
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/opengl_test ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/Source.o: nbproject/Makefile-${CND_CONF}.mk Source.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O3 -I../../../Dependencies/GLee/Include -I../../../Dependencies/GLFW/Include -I../../Base\ Libraries/Base\ Math\ Library -I../../Base\ Libraries/Base\ Render\ Library -MMD -MP -MF $@.d -o ${OBJECTDIR}/Source.o Source.cpp

# Subprojects
.build-subprojects:
	cd ../../Base\ Libraries/Base\ Render\ Library && ${MAKE}  -f Makefile CONF=Windows-Release

# Clean Targets
.clean-conf:
	${RM} -r build/NVIDIA-Windows-Release
	${RM} dist/NVIDIA-Windows-Release/MinGW-Linux-x86/opengl_test

# Subprojects
.clean-subprojects:
	cd ../../Base\ Libraries/Base\ Render\ Library && ${MAKE}  -f Makefile CONF=Windows-Release clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
