sprdec: sprdec.cpp sprlib.a
	g++ -std=c++11 -Wall sprdec.cpp sprlib.a -lpng -o sprdec

sprlib.a: sprlib.cpp sprframe.cpp sprframe.h sprlib.h palette.h
	g++ -std=c++11 -Wall -c sprlib.cpp sprframe.cpp
	ar rvs sprlib.a sprlib.o sprframe.o

clean:
	rm *.o
