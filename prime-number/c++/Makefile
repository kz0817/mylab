CXXFLAGS=-g -Wall -std=c++11

all: pn-c++-O3 pn-c++-O0


pn-c++-O0: pn-c++.cc
	$(CXX) $(CXXFLAGS) -O0 $< -o $@

pn-c++-O3: pn-c++.cc
	$(CXX) $(CXXFLAGS) -O3 $< -o $@
