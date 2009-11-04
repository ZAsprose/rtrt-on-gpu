#Common programs
CXX=g++
CPPFLAGS=
CXXFLAGS=-g -O2 -Wall
DEFS=-DHAVE_CONFIG_H
LDFLAGS=
LIBS=-lGL 
prefix=/usr/local
SRCDIR=.
exec_prefix=${prefix}
INCLUDEDIR=${prefix}/include
LIBDIR=${exec_prefix}/lib
INSTALL=/usr/bin/install -c
INSTALLP=${INSTALL}
INSTALLD=${INSTALL} -m 644
LDCONFIG=/sbin/ldconfig
TAR=/bin/tar
DATADIR=${prefix}/share
MKDIR=/bin/mkdir

#Package files
PACKAGE=GLee-3.03
SOURCE=GLee.c
HEADER=GLee.h
LIBNAME=libGLee.so
README=readme.txt
BINSTALL=inst
DISTDIR=dists

#Distribution files
DISTFILES=glee/${SOURCE} glee/${README} glee/${HEADER} glee/Makefile.in \
    glee/config.h.in glee/configure glee/install-sh glee/extensionList.txt
REDISTFILES=glee/Makefile.in glee/config.h.in glee/configure.ac \
    glee/install-sh glee/${README} glee/${SOURCE} glee/${HEADER} glee/extensionList.txt
BINDISTFILES=glee/${LIBNAME} glee/${README} glee/${HEADER} glee/${BINSTALL} glee/extensionList.txt



all:
	${CXX} ${SRCDIR}/${SOURCE} -shared -I${SRCDIR} -I${INCLUDEDIR} ${LIBS} \
		${CXXFLAGS} ${DEFS} -o ${SRCDIR}/${LIBNAME}

install:
	${INSTALLD} ${HEADER} ${INCLUDEDIR}/GL
	${INSTALLP} ${LIBNAME} ${LIBDIR}
	${MKDIR} -p ${DATADIR}/doc/GLee
	${INSTALLD} ${README} ${DATADIR}/doc/GLee
	${LDCONFIG}

dists: dist bindist redist

dist:
	${TAR} -C ../ -czf ${DISTDIR}/${PACKAGE}-src.tar.gz ${DISTFILES}

redist:
	${TAR} -C ../ -czf ${DISTDIR}/${PACKAGE}-redist.tar.gz ${REDISTFILES}

bindist:
	${TAR} -C ../ -czf ${DISTDIR}/${PACKAGE}-bin.tar.gz ${BINDISTFILES}
