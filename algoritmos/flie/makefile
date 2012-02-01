
CC=g++
FLAGS= -O3 -Wno-deprecated
OBJECTS= fuzzy_obstacle.o fuzzfier.o
OBJECTSFLIE= main.o fuzzy_obstacle.o fuzzy_control.o rule.o lingvar.o fuzzfier.o flie.o motor.o FIE.o
OBJECTSFMAP= fuzzy_obstacle.o fuzzfier.o fuzzymap.o
#LIBS= -L/local/home/joao/libstdc++-2.8.1/libraries/libstdc++ -lm 
#INCLUDES= -I /usr/include/g++-include -I/export/home/aci/joao/local/libstdc++-2.8.1/libio

main: $(OBJECTSFLIE)
	$(CC) $(FLAGS) $(OBJECTSFLIE) -o main

motor.o: motor.cc
	$(CC) $(FLAGS) -c motor.cc

FIE.o: FIE.cc
	$(CC) $(FLAGS) -c FIE.cc
	
fuzzy_obstacle.o: fuzzy_obstacle.cc
	$(CC) $(FLAGS) -c fuzzy_obstacle.cc

fuzzy_control.o: fuzzy_control.cc
	$(CC) $(FLAGS) -c fuzzy_control.cc

fuzzfier.o: fuzzfier.cc
	$(CC) $(FLAGS) -c fuzzfier.cc

rule.o: rule.cc
	$(CC) $(FLAGS) -c rule.cc

lingvar.o: lingvar.cc
	$(CC) $(FLAGS) -c lingvar.cc

flie.o: flie.cc
	$(CC) $(FLAGS) -c flie.cc

main.o: main.cc
	$(CC) $(FLAGS) -c main.cc

clean:
	rm *.o

 





