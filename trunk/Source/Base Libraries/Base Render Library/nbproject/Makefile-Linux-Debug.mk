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
CND_CONF=Linux-Debug
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/Keyboard.o \
	${OBJECTDIR}/ShaderManager.o \
	${OBJECTDIR}/Mouse.o \
	${OBJECTDIR}/Texture3D.o \
	${OBJECTDIR}/Camera.o \
	${OBJECTDIR}/Texture2D.o \
	${OBJECTDIR}/RenderBuffer.o \
	${OBJECTDIR}/FrameBuffer.o \
	${OBJECTDIR}/Texture1D.o

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
	${MAKE}  -f nbproject/Makefile-Linux-Debug.mk dist/Linux-Debug/GNU-Linux-x86/libbase_render_library.a

dist/Linux-Debug/GNU-Linux-x86/libbase_render_library.a: ${OBJECTFILES}
	${MKDIR} -p dist/Linux-Debug/GNU-Linux-x86
	${RM} dist/Linux-Debug/GNU-Linux-x86/libbase_render_library.a
	${AR} rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libbase_render_library.a ${OBJECTFILES} 
	$(RANLIB) dist/Linux-Debug/GNU-Linux-x86/libbase_render_library.a

${OBJECTDIR}/Keyboard.o: nbproject/Makefile-${CND_CONF}.mk Keyboard.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../Dependencies/GLee/Include -I../../../Dependencies/GLFW/Include -MMD -MP -MF $@.d -o ${OBJECTDIR}/Keyboard.o Keyboard.cpp

${OBJECTDIR}/ShaderManager.o: nbproject/Makefile-${CND_CONF}.mk ShaderManager.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../Dependencies/GLee/Include -I../../../Dependencies/GLFW/Include -MMD -MP -MF $@.d -o ${OBJECTDIR}/ShaderManager.o ShaderManager.cpp

${OBJECTDIR}/Mouse.o: nbproject/Makefile-${CND_CONF}.mk Mouse.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../Dependencies/GLee/Include -I../../../Dependencies/GLFW/Include -MMD -MP -MF $@.d -o ${OBJECTDIR}/Mouse.o Mouse.cpp

${OBJECTDIR}/Texture3D.o: nbproject/Makefile-${CND_CONF}.mk Texture3D.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../Dependencies/GLee/Include -I../../../Dependencies/GLFW/Include -MMD -MP -MF $@.d -o ${OBJECTDIR}/Texture3D.o Texture3D.cpp

${OBJECTDIR}/Camera.o: nbproject/Makefile-${CND_CONF}.mk Camera.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../Dependencies/GLee/Include -I../../../Dependencies/GLFW/Include -MMD -MP -MF $@.d -o ${OBJECTDIR}/Camera.o Camera.cpp

${OBJECTDIR}/Texture2D.o: nbproject/Makefile-${CND_CONF}.mk Texture2D.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../Dependencies/GLee/Include -I../../../Dependencies/GLFW/Include -MMD -MP -MF $@.d -o ${OBJECTDIR}/Texture2D.o Texture2D.cpp

${OBJECTDIR}/RenderBuffer.o: nbproject/Makefile-${CND_CONF}.mk RenderBuffer.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../Dependencies/GLee/Include -I../../../Dependencies/GLFW/Include -MMD -MP -MF $@.d -o ${OBJECTDIR}/RenderBuffer.o RenderBuffer.cpp

${OBJECTDIR}/FrameBuffer.o: nbproject/Makefile-${CND_CONF}.mk FrameBuffer.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../Dependencies/GLee/Include -I../../../Dependencies/GLFW/Include -MMD -MP -MF $@.d -o ${OBJECTDIR}/FrameBuffer.o FrameBuffer.cpp

${OBJECTDIR}/Texture1D.o: nbproject/Makefile-${CND_CONF}.mk Texture1D.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../Dependencies/GLee/Include -I../../../Dependencies/GLFW/Include -MMD -MP -MF $@.d -o ${OBJECTDIR}/Texture1D.o Texture1D.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Linux-Debug
	${RM} dist/Linux-Debug/GNU-Linux-x86/libbase_render_library.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
