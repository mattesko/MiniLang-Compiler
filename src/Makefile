CC = gcc
CFLAGS = -Wall -lfl

main: lex.yy
	$(CC) -o minic lex.yy.c $(CFLAGS)

lex.yy:
	flex minic.l

clean:
	rm lex.yy.*

test: main
	echo "1.0+200" | ./minic
	echo "(a_*_b/c__)==5" | ./minic
	echo "True&&False!=3#hello" | ./minic
	echo '"!#^*\\n./{};><?"' | ./minic
	echo "var a: float = 0.0;" | ./minic
	echo "0HelLo" | ./minic
