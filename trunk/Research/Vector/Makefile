all: main.cpp
	g++ -O2 -msse2 -fpermissive main.cpp

asm: main.cpp
	g++ -O2 -msse2 -fpermissive -S  main.cpp

clean:
	rm a.out main.s
