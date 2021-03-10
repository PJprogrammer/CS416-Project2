mkdir submission
mkdir submission/benchmarks

cp rpthread.cpp submission/rpthread.cpp
cp rpthread.h   submission/rpthread.h
cp queue.h      submission/queue.h
cp hashmap.h    submission/hashmap.h
cp Makefile     submission/Makefile
cp Project2_Report.pdf submission/Project2_Report.pdf

cp benchmarks/vector_multiply.cpp submission/benchmarks/vector_multiply.cpp
cp benchmarks/parallel_cal.cpp submission/benchmarks/parallel_cal.cpp
cp benchmarks/external_cal.cpp submission/benchmarks/external_cal.cpp
cp benchmarks/test.cpp submission/benchmarks/test.cpp

cp benchmarks/genRecord.sh submission/benchmarks/genRecord.sh
cp benchmarks/Makefile submission/benchmarks/Makefile

tar -zcvf Project2_pj242_me470.tar.gz submission
rm -r submission
