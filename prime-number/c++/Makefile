CXXFLAGS=-g -Wall -std=c++11

all: pn-c++-O3 pn-c++-O0 pn-c++-O3-odd pn-c++-O3-odd-smart pn-c++-O3-long


pn-c++-O0: pn-c++.cc
	$(CXX) $(CXXFLAGS) -O0 -DSTD $< -o $@

pn-c++-O3: pn-c++.cc
	$(CXX) $(CXXFLAGS) -O3 -DSTD $< -o $@

pn-c++-O3-odd: pn-c++.cc
	$(CXX) $(CXXFLAGS) -O3 -DODD $< -o $@

pn-c++-O3-odd-smart: pn-c++.cc
	$(CXX) $(CXXFLAGS) -O3 -DODD_SMART $< -o $@

pn-c++-O3-long: pn-c++.cc
	$(CXX) $(CXXFLAGS) -O3 -DTYPE_LONG $< -o $@
