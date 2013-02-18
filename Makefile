# Makefile for the compiler for the Simp language
#
# target simpc builds the standard compiler
#
# target lexdbg builds the stand-alone scanner
#
# target parsedbg builds the compiler with bison parse-time debugging output
#

tc: lex.yy.o y.tab.o main.o AST.o Type.o SymbolTable.o StringPool.o \
       analyze.o encode.o
	g++ -g lex.yy.o y.tab.o main.o AST.o Type.o SymbolTable.o \
	  StringPool.o analyze.o encode.o -o tc

lex.yy.o: lex.yy.c y.tab.h
	g++ -c -g lex.yy.c

lex.yy.c: scan.ll AST.h StringPool.h
	flex scan.ll

y.tab.o: y.tab.c
	g++ -c -Wall -g y.tab.c

y.tab.h y.tab.c: phase1.y AST.h Type.h
	bison -y -d phase1.y

main.o: main.cxx SymbolTable.h Type.h StringPool.h
	g++ -c -Wall -g main.cxx

AST.o: AST.cxx AST.h Type.h
	g++ -c -Wall -g AST.cxx

analyze.o: analyze.cxx AST.h SymbolTable.h Type.h
	g++ -c -Wall -g analyze.cxx

encode.o: encode.cxx AST.h Type.h
	g++ -c -Wall -g encode.cxx

Type.o: Type.cxx Type.h
	g++ -c -Wall -g Type.cxx

SymbolTable.o: SymbolTable.cxx SymbolTable.h Type.h
	g++ -c -Wall -g SymbolTable.cxx

StringPool.o: StringPool.cxx StringPool.h
	g++ -c -Wall -g StringPool.cxx

y.output: phase1.y
	bison -y -d -v phase1.y

lexdbg: scan.ll y.tab.h StringPool.o
	flex scan.ll
	mv lex.yy.c scan.c
	g++ -DDEBUG scan.c StringPool.o -lfl -o lexdbg

parsedbg: lex.yy.o y.tab.c main.cxx AST.o Type.o SymbolTable.o StringPool.o \
            analyze.o encode.o
	g++ -c -g -DYYDEBUG=1 main.cxx
	g++ -c -g -DYYDEBUG=1 y.tab.c
	g++ -g lex.yy.o y.tab.o main.o AST.o Type.o SymbolTable.o \
	  StringPool.o analyze.o encode.o -o tc

clean:
	-rm lex.yy.*  y.tab.*  y.output *.o  lexdbg scan.c

