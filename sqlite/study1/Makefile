
.SUFFIXES: .db .hex

.db.hex:
	od -tx1z -v -Ax $< > $@

all: a01.hex a02.hex a03.hex a04.hex a05.hex


a05.db: a04.db
	cp $< $@
	sqlite3 $@ "update t1 set name='kato shiho' where rowid=2"

a04.db: a03.db
	cp $< $@
	sqlite3 $@ "update t1 set name='shiho', age=21 where rowid=2"

a03.db: a02.db
	cp $< $@
	sqlite3 $@ "insert into t1 values('tamami', 17)"
	sqlite3 $@ "insert into t1 values('konoka', 19)"

a02.db: a01.db
	cp $< $@
	sqlite3 $@ "insert into t1 values('memi', 17)"

a01.db:
	sqlite3 $@ "PRAGMA page_size = 512; create table t1(name, age)"

.PHONY: clean
clean:
	rm -f *.db *.hex
