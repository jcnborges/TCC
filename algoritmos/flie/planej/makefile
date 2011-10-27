CC=g++
cc=gcc
CFLAGS= -O3
OBJECTS= 
INCLUDES= -I /usr/include/g++  -I /usr/openwin/include -I/export/home/aci/joao/local/libstdc++-2.8.1/libio -I../../user/galib -I../../src

LIBS= -L/local/home/joao/libstdc++-2.8.1/libraries/libstdc++ -L../../user/galib/ga -lm -lga

teste: fuzzy_map.o
	$(CC) $(FLAGS) fuzzy_map.o ../fuzzy_obstacle.o ../fuzzfier.o  -o teste $(LIBS) 

fuzzy_map.o: $(OBJECTS)
	$(CC) $(CFLAGS) -c fuzzy_map.cc $(INCLUDES) -o fuzzy_map.o

clean:
	rm *.o;rm teste
