all: algo

algo: algo.cc fraction.cc fraction.h runtime.cc runtime.h taskset.cc taskset.h
	g++ algo.cc fraction.cc runtime.cc taskset.cc -o algo