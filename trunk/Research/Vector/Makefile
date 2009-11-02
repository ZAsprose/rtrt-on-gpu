
#comp = g++
comp = icpc

all: main.cpp vector.hpp nvec.hpp
	${comp} -O2 -msse2  main.cpp
#	${comp} -O2 -msse2 -fpermissive main.cpp

asm: main.cpp vector.hpp nvec.hpp
	${comp} -O2 -msse2 -fpermissive -S  main.cpp

clean:
	rm a.out main.s
