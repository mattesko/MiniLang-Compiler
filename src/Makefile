CC = gcc
CFLAGS = -Wall -lfl

minic: lex.yy.c minic.tab.c minic.tab.h main.o tree.o
	$(CC) minic.tab.c lex.yy.c main.o tree.o -o minic $(CFLAGS)

lex.yy.c: minic.l minic.tab.h
	flex --header-file=lex.yy.h minic.l

minic.tab.c minic.tab.h: minic.y
	bison -d --verbose -t minic.y

tree.o: tree.c tree.h
	$(CC) -c tree.c tree.h

test_lex: minic
	echo "1.00+200" | ./minic && echo ""
	echo "(a_*_b/_c__)==5" | ./minic && echo ""
	echo "True&&False!=3#hello" | ./minic && echo ""
	echo '"!#^*\\n./{};><?"' | ./minic && echo ""
	echo "var a: float = 0.0;" | ./minic && echo ""
	echo "0123" | ./minic && echo ""
	echo "\"True\"" | ./minic && echo ""
	echo "True" | ./minic && echo ""
	echo "^" | ./minic && echo ""

clean:
	rm minic minic.tab.c lex.yy.c minic.tab.h
