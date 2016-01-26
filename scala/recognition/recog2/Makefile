WEIGTH_MOVE_DATA = \
weigth-move1.dat \
weigth-move2.dat \
weigth-move3.dat \
weigth-move4.dat \
weigth-move5.dat

all: $(WEIGTH_MOVE_DATA)

$(WEIGTH_MOVE_DATA): raw-weight-ex2.dat extract-weights.sh
	./extract-weights.sh $< $@ | sh > $@

raw-weight-ex2.dat: rawoutput-ex2.dat
	grep "^c: " $< > $@

rawoutput-ex2.dat: samples/learning2.dat
	sbt "run $<" > $@
