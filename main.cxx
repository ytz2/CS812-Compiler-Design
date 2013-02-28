// main routine for compiler for Simp language

#include "SymbolTable.h"
#include "StringPool.h"
#include "Type.h"
#include <cassert>
#include <cstring>
#include <iostream>
#include "AST.h"
<<<<<<< HEAD
#include "LabelStack.h"
=======
>>>>>>> a3052501d3fde445dd8ab41d969fe1de0be0999d
using namespace std;
// prototype for bison-generated parser
int yyparse();

// prototypes for code generation functions in encode.cxx
//void encodeInitialize();
//void encodeFinish();

// to enable bison parse-time debugging
#if YYDEBUG
extern int yydebug;
#endif

// global string pool
StringPool* stringPool;

// global symbol table
SymbolTable* symbolTable;

// global type module
TypeModule* types;

<<<<<<< HEAD
LabelStack* break_label;
LabelStack* continue_label;
=======
>>>>>>> a3052501d3fde445dd8ab41d969fe1de0be0999d
int before = 0;
int after = 0;

int count_divide = 1;

void grep_cmd(int argc, char* argv[], int& before, int &after) {
	char bf[8] = "-before";
	char af[8] = "-after";
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], bf) == 0)
			before = 1;
		if (strcmp(argv[i], af) == 0)
			after = 1;
	};
}
;

int main(int argc, char* argv[]) {
	// create a string pool
	stringPool = new StringPool();

	// create a type module
	types = new TypeModule();

	// create a symbol table
	symbolTable = new SymbolTable();

<<<<<<< HEAD
	//create break&continue label
	break_label=new LabelStack("BREAKLABEL");
	continue_label=new LabelStack("CONTINUELABEL");

=======
>>>>>>> a3052501d3fde445dd8ab41d969fe1de0be0999d
	grep_cmd(argc, argv, before, after);
	// set yydebug to 1 to enable bison debugging
	// (preprocessor symbol YYDEBUG must also be 1 here and in parse.yy)
#if YYDEBUG
	yydebug = 1;
#endif

	// generate prologue code
	//encodeInitialize();

	// syntax directed compilation!
	yyparse();

	// generate epilogue code
	//encodeFinish();

	// cleanup symbol table
	delete symbolTable;

	// cleanup the types
	delete types;

	// cleanup the string pool
	delete stringPool;
<<<<<<< HEAD

	// cleanup label stacks
	delete break_label;
	delete continue_label;
=======
>>>>>>> a3052501d3fde445dd8ab41d969fe1de0be0999d
}
