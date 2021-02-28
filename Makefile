CC = clang++ -w -std=c++11
CFLAGS = -g -c
AR = ar -rc
RANLIB = ranlib

SCHED = RR
#TSLICE=5 ##timeslice variable

#all: rpthread.a
all: clean main run

rpthread.a: rpthread.o
	$(AR) librpthread.a rpthread.o
	$(RANLIB) librpthread.a

rpthread.o: rpthread.h

main: 
	$(CC) *.cpp -o main

run:
	./main

ifeq ($(SCHED), RR)
	$(CC) -pthread $(CFLAGS) rpthread.cpp -DTIMESLICE $(TSLICE)
else ifeq ($(SCHED), MLFQ)
	$(CC) -pthread $(CFLAGS) rpthread.cpp -DMLFQ -DTIMESLICE $(TSLICE)
else
	echo "no such scheduling algorithm"
endif

clean:
	rm -rf testfile *.o *.a main
