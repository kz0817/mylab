CXXFLAGS = -g3 -Wall -O0 -std=c++17

code.txt: pages
	objdump -S $< > $@

pages: funcs.o

funcs.c: gen_source.py
	./$^ > $@

.PHONY: show_addr

show_addr: code.txt
	@grep "<_init>:" $^
	@grep "<main>:" $^
	@grep "<func9999>:" $^
	@grep "<_fini>:" $^
