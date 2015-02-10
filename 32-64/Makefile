all: run

BINS = hello32 hello

hello32: hello.c
	gcc -m32 $< -o $@

.PHONY: run clean
run: $(BINS)
	gdb hello32 -x show-proc.gdb -batch
	gdb hello -x show-proc.gdb -batch

clean:
	rm -f $(BINS)

