CXXFLAGS = -std=c++17 -g3 -O3 -Wall
LDFLAGS = -lsqlite3

bkup: bkup.cc
	$(CXX) -o $@ $< $(CXXFLAGS) $(LDFLAGS)

.PHONY: clean run

clean:
	rm -f bkup *.db

run:
	rm -f *.db *.hex
	./bkup
	od -tx1z -Ax -v test1.db > test1.hex
	od -tx1z -Ax -v backup1.db > backup1.hex
