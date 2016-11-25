PROGRAMS = opti opti-o3 opti-has opti-o3-has
all: $(PROGRAMS)

opti: opti.cc
	$(CXX) -o $@ $<

opti-o3: opti.cc
	$(CXX) -O3 -o $@ $<

opti-has: opti.cc
	$(CXX) -march=haswell -o $@ $<

opti-o3-has: opti.cc
	$(CXX) -O3 -march=haswell -o $@ $<

.PHONY: clean
clean:
	rm -f $(PROGRAMS)
