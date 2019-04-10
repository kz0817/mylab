
.SUFFIXES: .db .hex

.db.hex:
	od -tx1z -v -Ax $< > $@

a03.hex: a03.db

a03.db: a02.db
	cp $< $@
	sqlite3 $@ "insert into t1 values('tamami', 17)"
	sqlite3 $@ "insert into t1 values('konoka', 19)"

a02.hex: a02.db

a02.db: a01.db
	cp $< $@
	sqlite3 $@ "insert into t1 values('memi', 17)"

a01.hex: a01.db

a01.db:
	sqlite3 $@ "create table t1(name, age)"

.PHONY: clean
clean:
	rm -f *.db *.txt
