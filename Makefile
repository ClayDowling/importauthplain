
all: importplain importtest
	./importtest

test: importtest
	./importtest

clean:
	rm -f *.o
	rm -f importtest
	rm -f importplain

importtest:	CuTest.o CuTest.h importtest.o importplain.o
	$(CC) CuTest.o importtest.o importplain.o -o $@

importplain:	importplain.o main.o sqlite3.o
	$(CC) importplain.o main.o sqlite3.o -o $@ -pthread -ldl
