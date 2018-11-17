GENERATOR = ./generator.py

all: test-1MiB.dat test-1MiBp1.dat

test-1MiB.dat: $(GENERATOR)
	$(GENERATOR) -s 1048576 -o $@

test-1MiBp1.dat: $(GENERATOR)
	$(GENERATOR) -s 1048577 -o $@
