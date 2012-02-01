CC=arm-linux-g++
FLAGS= -O3 -Wno-deprecated
OBJECTSFLIE= tslogic.o fuzzy_obstacle.o fuzzy_control.o rule.o lingvar.o fuzzfier.o flie.o motor.o FIE.o fcm.o

all: $(OBJECTSFLIE)
	$(CC) $(FLAGS) $(OBJECTSFLIE) -o tslogic

tslogic.o: tslogic.cpp
	$(CC) $(FLAGS) -c tslogic.cpp

motor.o: flie/motor.cc
	$(CC) $(FLAGS) -c flie/motor.cc

fcm.o: fcm.cpp
	$(CC) $(FLAGS) -c fcm.cpp

FIE.o: flie/FIE.cc
	$(CC) $(FLAGS) -c flie/FIE.cc
	
fuzzy_obstacle.o: flie/fuzzy_obstacle.cc
	$(CC) $(FLAGS) -c flie/fuzzy_obstacle.cc

fuzzy_control.o: flie/fuzzy_control.cc
	$(CC) $(FLAGS) -c flie/fuzzy_control.cc

fuzzfier.o: flie/fuzzfier.cc
	$(CC) $(FLAGS) -c flie/fuzzfier.cc

rule.o: flie/rule.cc
	$(CC) $(FLAGS) -c flie/rule.cc

lingvar.o: flie/lingvar.cc
	$(CC) $(FLAGS) -c flie/lingvar.cc

flie.o: flie/flie.cc
	$(CC) $(FLAGS) -c flie/flie.cc

clean:
	rm *.o

