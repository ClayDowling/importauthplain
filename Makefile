
all: importplain importtest
	importtest

test: importtest
	./importtest

importtest:	CuTest.o CuTest.h importtest.o importplain.o
	$(CC) CuTest.o importtest.o importplain.o -o $@

importplain:	importplain.o main.o
	$(CC) importtest.o main.o -o $@
