CXXFLAGS=-O3 -g -Wall -std=c++17
md5calc: md5.o main.o
	$(CXX) $(LDFLAGS) -o $@ $^

main.o: ArgParser.h md5.h
md5.o: md5.h

.PHONY: clean
clean:
	rm -f *.o md5calc
