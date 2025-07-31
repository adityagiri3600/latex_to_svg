all: l2s

l2s: main.o ast.o lexer.o parser.o
	gcc -o l2s main.o ast.o lexer.o parser.o

main.o: main.c ast.h
	gcc -c main.c

ast.o: ast.c ast.h
	gcc -c ast.c

lexer.o: lexer.c parser.h
	gcc -c lexer.c

parser.o: parser.c
	gcc -c parser.c

lexer.c: lexer.l parser.h
	flex -o lexer.c lexer.l

parser.c parser.h: parser.y
	bison -d -o parser.c parser.y

clean:
	rm -f *.o l2s lexer.c parser.c parser.h
