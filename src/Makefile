CC = gcc
CFLAGS = -Wall -lfl

minic: lex.yy.c minic.tab.c minic.tab.h tree.o pretty.o symbol.o type.o codegen.o main.o
	$(CC) minic.tab.c lex.yy.c tree.o pretty.o symbol.o type.o codegen.o main.o -o minic $(CFLAGS)

lex.yy.c: minic.l minic.tab.h
	flex --header-file=lex.yy.h minic.l

minic.tab.c minic.tab.h: minic.y
	bison -d --verbose -t minic.y

main.o: main.c pretty.h symbol.h
	$(CC) -c main.c pretty.h symbol.h

tree.o: tree.c tree.h
	$(CC) -c tree.c tree.h

pretty.o: pretty.c pretty.h
	$(CC) -c pretty.c pretty.h

symbol.o: symbol.c symbol.h
	$(CC) -c symbol.c symbol.h

type.o: type.c type.h
	$(CC) -c type.c type.h

codegen.o: codegen.c codegen.h
	$(CC) -c codegen.c codegen.h

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
	rm *.gch
	rm *.o
	rm lex.yy.c 
	rm minic.tab.h
	rm minic.tab.c 
	rm minic 
