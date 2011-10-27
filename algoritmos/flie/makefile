
CC=g++
FLAGS= -O3
OBJECTS= fuzzy_obstacle.o fuzzfier.o
OBJECTSFLIE= fuzzy_obstacle.o fuzzy_control.o rule.o lingvar.o fuzzfier.o flie.o
OBJECTSFMAP= fuzzy_obstacle.o fuzzfier.o fuzzymap.o
LIBS= -L/local/home/joao/libstdc++-2.8.1/libraries/libstdc++ -lm 
INCLUDES= -I /usr/include/g++-include -I/export/home/aci/joao/local/libstdc++-2.8.1/libio

objects: $(OBJECTS)

flie:  flie.o $(OBJECTSFLIE)
	$(CC) $(FLAGS) $(OBJECTSFLIE) -o flie $(LIBS) 

fuzzymap: $(OBJECTSFMAP) fuzzymap.o
	$(CC) $(FLAGS) $(OBJECTSFMAP) -o fuzzymap $(LIBS)

fuzzymap.o: fuzzymap.cc
	$(CC) $(FLAGS) -c fuzzymap.cc $(INCLUDES)

fuzzy_obstacle.o: fuzzy_obstacle.cc
	$(CC) $(FLAGS) -c fuzzy_obstacle.cc  $(INCLUDES)

fuzzy_control.o: fuzzy_control.cc
	$(CC) $(FLAGS) -c fuzzy_control.cc $(INCLUDES)

fuzzfier.o: fuzzfier.cc
	$(CC) $(FLAGS) -c fuzzfier.cc  $(INCLUDES)

rule.o: rule.cc
	$(CC) $(FLAGS) -c rule.cc  $(INCLUDES)

lingvar.o: lingvar.cc
	$(CC) $(FLAGS) -c lingvar.cc  $(INCLUDES)

flie.o: flie.cc
	$(CC) $(FLAGS) -c flie.cc $(INCLUDES)

clean:
	rm *.o;rm flie;rm fuzzymap

 





