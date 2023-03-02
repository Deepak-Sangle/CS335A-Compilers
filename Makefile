all:
	bison -d -v parser.y
	flex scanner.l
	g++ -stdc++17 lex.yy.c parser.tab.c -o ASTGenerator

clean:
	rm -f lex.yy.c parser.tab.c parser.tab.h parser.output ASTGenerator parser.exe