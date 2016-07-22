CXX=g++

CPPFLAGS=-ggdb3 -std=c++11

all: test

test: tc.cpp tini.hpp
	$(CXX) $(CPPFLAGS) -o test tc.cpp
	./test

clean:
	rm test
