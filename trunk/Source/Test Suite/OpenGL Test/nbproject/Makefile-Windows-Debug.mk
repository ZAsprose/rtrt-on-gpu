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
CC=gcc.exe
CCC=g++.exe
CXX=g++.exe
FC=
AS=as.exe

# Macros
CND_PLATFORM=MinGW-Windows
CND_CONF=Windows-Debug
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
CCFLAGS=-m32
CXXFLAGS=-m32

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L../../../Dependencies/GLee/Lib/MinGW -L../../../Dependencies/GLFW/Lib/MinGW -L../../Base\ Libraries/Base\ Render\ Library/dist/Windows-Debug/MinGW-Windows -lbase_render_library -lglfw -lGLee -lopengl32 -lglu32 -lgdi32 -luser32 -lkernel32

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-Windows-Debug.mk dist/Windows-Debug/MinGW-Windows/opengl_test.exe

dist/Windows-Debug/MinGW-Windows/opengl_test.exe: ${OBJECTFILES}
	${MKDIR} -p dist/Windows-Debug/MinGW-Windows
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/opengl_test ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/Source.o: nbproject/Makefile-${CND_CONF}.mk Source.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../Dependencies/GLee/Include -I../../../Dependencies/GLFW/Include -I../../Base\ Libraries/Base\ Math\ Library -I../../Base\ Libraries/Base\ Render\ Library -MMD -MP -MF $@.d -o ${OBJECTDIR}/Source.o Source.cpp

# Subprojects
.build-subprojects:
	cd ../../Base\ Libraries/Base\ Render\ Library && ${MAKE}  -f Makefile CONF=Windows-Debug

# Clean Targets
.clean-conf:
	${RM} -r build/Windows-Debug
	${RM} dist/Windows-Debug/MinGW-Windows/opengl_test.exe

# Subprojects
.clean-subprojects:
	cd ../../Base\ Libraries/Base\ Render\ Library && ${MAKE}  -f Makefile CONF=Windows-Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
