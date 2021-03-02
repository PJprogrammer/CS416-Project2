CC = g++
CFLAGS = -g -w

all:: parallel_cal vector_multiply external_cal test

parallel_cal:
	$(CC) $(CFLAGS) -pthread -o parallel_cal parallel_cal.cpp -L../ -lrpthread

vector_multiply:
	$(CC) $(CFLAGS) -pthread -o vector_multiply vector_multiply.cpp -L../ -lrpthread

external_cal:
	$(CC) $(CFLAGS) -pthread -o external_cal external_cal.cpp -L../ -lrpthread

test:
	$(CC) $(CFLAGS) -pthread -o test test.cpp -L../ -lrpthread

clean:
	rm -rf testcase test parallel_cal vector_multiply external_cal *.o ./record/
