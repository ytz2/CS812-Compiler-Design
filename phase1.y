// T grammar for Phase 1
// supports only the main block

%{
#include <iostream>
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include "AST.h"
#include "Type.h"
#include "hw1.h"
// this routine is in scan.ll
int getCurrentSourceLineNumber();

// type module is in main.cxx
extern TypeModule* types;

void yyerror(const char *);
extern int yyparse(void);
extern int yylex(void);

%}

%union {

  int value;
  char* str;
  AST_MainBlock *main_block;
  AST_MainBlockStatements *main_block_statements;
  AST_MainVariableDeclarationStatement *main_variable_declaration_statement;
  AST_MainVariableDeclaration *main_variable_declaration;
  AST_VariableDeclarators *variable_declarators;
  AST_IntegralType *integral;
  AST_Identifier *identifier;
  AST_Statement* statement;
  AST_ExpressionStatement* expression_statement;
  AST_Assignment* assignment;
  AST_Expression* expression;
  AST_Literal * literal;
  AST_OutputStatement *output_statement;
  AST_LeftHandSide *lhs;  
  AST_VariableDeclarator *variable_declarator;
  AST_Primary *primary;
};

%token <value> INTEGER_LITERAL
%token <str> IDENTIFIER
%token <value> NULL_LITERAL
%token EQ_OP

%token INT
%token MAIN CLASS EXTENDS
%token THIS SUPER
%token IF ELSE WHILE NEW DELETE RETURN
%token OUT
%token BREAK CONTINUE
%token BAD

%start CompilationUnit
%type <main_block> CompilationUnit
%type <main_block> MainFunctionDeclaration
%type <main_block> MainFunctionBody
%type <main_block> MainBlock
%type <main_block_statements> MainBlockStatements
%type <statement> MainBlockStatement
%type <main_variable_declaration_statement> MainVariableDeclarationStatement 
%type <main_variable_declaration> MainVariableDeclaration
%type <integral> Type
%type <integral> PrimitiveType
%type <integral> NumericType
%type <integral> IntegralType
%type <variable_declarators> VariableDeclarators
%type <variable_declarator> VariableDeclarator
%type <identifier> Identifier
%type <statement> BlockStatement
%type <statement> Statement
%type <expression_statement> ExpressionStatement
%type <assignment> StatementExpression
%type <assignment> Assignment
%type <lhs> LeftHandSide
%type <expression> AssignmentExpression
%type <expression> EqualityExpression
%type <expression> RelationalExpression
%type <expression> AdditiveExpression
%type <expression> MultiplicativeExpression
%type <expression> UnaryExpression
%type <primary> CastExpression
%type <primary> Primary
%type <expression> PrimaryNoNewArray
%type <expression> ParenExpression
%type <expression> Expression
%type <literal> Literal
%type <output_statement> OutputStatement

%%


//Leone
CompilationUnit
        : MainFunctionDeclaration
	{
	  $$=$1;

	  if (before==1 && after==0)
	 {
	 	 $$->dump();
	 	 $$->analyze();
	 }
	  else if ((after==1 && before==0)||(after==0 && before==0))
	 {
	 	$$->analyze();
	 	$$->dump();
	 }
	 else if (before==1 && after==1)
	 {
	 	$$->dump();
	 	$$->analyze();
	 	$$->dump();
	 }

	 // generate code
	 $$->encode();
	 // cleanup the AST
	  delete $$;

	}
        ;

//Shea
MainFunctionDeclaration
	: INT MAIN '(' ')' MainFunctionBody
	{
	  $$=$5;
	}
	;

//Dupuis
MainFunctionBody
	: MainBlock
	{
	  $$=$1;
	}
	;

//Carpio-Mazariegos
VariableDeclarators
        : VariableDeclarators ',' VariableDeclarator
	{
	  $$=new AST_VariableDeclarators($3,$1);
	}
        | VariableDeclarator
	{
	  $$=new AST_VariableDeclarators($1,NULL);
	}
        ;

//Dobbins-Bucklad
VariableDeclarator
        : Identifier
	{
	  $$=new AST_VariableDeclarator($1);
	}
        ;

//Picard 
BlockStatement
        : Statement
	{
	  $$=$1;
	}
        ;

//Nappi
MainBlock
	: '{' MainBlockStatements '}'
	{
	  $$=new AST_MainBlock($2);
	}
	| '{' '}'
	{
	  $$=new AST_MainBlock(NULL);
	}
	;

//DeRossi
MainBlockStatements
	: MainBlockStatements MainBlockStatement 
	{
	  $$=new AST_MainBlockStatements($2,$1);
	}
	| MainBlockStatement
	{
	  $$=new AST_MainBlockStatements($1,NULL);
	}
	;

//Levin
MainBlockStatement
	: MainVariableDeclarationStatement
	{
	  $$=$1;
	}
	| BlockStatement
	{
	  $$=$1;
	}
	;

//Shidlovsky
MainVariableDeclarationStatement
	: MainVariableDeclaration ';'
	{
	  $$=new AST_MainVariableDeclarationStatement($1);
	}
	;

//Janowski
MainVariableDeclaration
	: Type VariableDeclarators
	{
	  $$=new AST_MainVariableDeclaration($1,$2);
	}
	;

//Piispanen
Statement
	: ExpressionStatement
	{
	  $$=$1;
	}
	| OutputStatement
	{
	  $$=$1;
	}
	;

//Gunapal
OutputStatement
	: OUT Expression ';'
	{
	  $$=new AST_OutputStatement($2);
	}
	;

//Pitchai
ParenExpression
	: '(' Expression ')'
	{
	  $$=$2;
	}
	;

//Xiong
ExpressionStatement
	: StatementExpression ';'
	{
	  $$=new AST_ExpressionStatement($1);
	}
	;

//Ma
StatementExpression
	: Assignment
	{
	  $$=$1;
	}
	;
//Shrestha
Expression
	: AssignmentExpression
	{
	  $$=$1;
	}
	;

//Mihtsentu
AssignmentExpression
	: Assignment
	{
	  $$=$1;
	}
	| EqualityExpression
	{
	  $$=$1;
	}
	;

//Liu
Assignment
	: LeftHandSide AssignmentOperator AssignmentExpression
	{
	  $$=new AST_Assignment($1,$3);
	}
	;

//Rajappan
LeftHandSide
	: Identifier
	{
	  $$=new AST_LeftHandSide($1);
	}
	;

//Hou
EqualityExpression
	: EqualityExpression EQ_OP RelationalExpression
	{
	  $$=new AST_EqualityExpression($1,$3);
	}
	| RelationalExpression
	{
	  $$=$1;
	}
	;

//Sun
RelationalExpression
	: RelationalExpression '<' AdditiveExpression
	{
	  $$=new AST_RelationalExpression($1,$3,true);
	}
	| RelationalExpression '>' AdditiveExpression
	{
	  $$=new AST_RelationalExpression($1,$3,false);
	}
	| AdditiveExpression
	{
	  $$=$1;
	}
	;

//Heidari
AdditiveExpression
	: AdditiveExpression '+' MultiplicativeExpression
	{
	  $$=new AST_AdditiveExpression($1,$3,true);
	}
	| AdditiveExpression '-' MultiplicativeExpression
	{
	  $$=new AST_AdditiveExpression($1,$3,false);
	}
	| MultiplicativeExpression
	{
	  $$=$1;
	}
	;

MultiplicativeExpression
	: MultiplicativeExpression '*' UnaryExpression
	{
	  $$=new AST_MultiplicativeExpression($1,$3,true);
	}
	| MultiplicativeExpression '/' UnaryExpression
	{
	  $$=new AST_MultiplicativeExpression($1,$3,false);
	}
	| UnaryExpression
	{
	  $$=$1;
	}
	;

UnaryExpression
	: '-' UnaryExpression
	{
	  $$=new AST_UnaryExpression($2,true);
	}
	| '!' UnaryExpression
	{
	  $$=new AST_UnaryExpression($2,false);
	}
	| CastExpression
	{
	  //$$=new AST_UnaryExpression($1);
	  $$=$1;
	}
	;

CastExpression
	: Primary
	{
	  $$=$1;
	}
        ;

Primary
	: Identifier
	{
	  $$=new AST_Primary($1,0); //member 0: identifier 1: PrimaryNoArray
	}
	| PrimaryNoNewArray
	{
	  $$=new AST_Primary($1,1);
	}
	;

PrimaryNoNewArray
	: ParenExpression
	{
	  $$=$1;
	}
	| Literal
	{
	  $$=$1;
	}
	;

AssignmentOperator
	: '='
	{
	  ;
	}
	;

Type
	: PrimitiveType
	{
	  $$=$1;
	}
	;

PrimitiveType
	: NumericType
	{
	  $$=$1;
	}
	;

NumericType
	: IntegralType
	{
	  $$=$1;
     	}

	;

IntegralType
	: INT
	{
	  $$=new AST_IntegralType();
	}
	;

Identifier
	: IDENTIFIER
	{
	  $$=new AST_Identifier($1);
	}
	;

Literal
	: INTEGER_LITERAL	
	{
	  $$=new AST_Literal($1);
	}
	;

%%
void yyerror(const char *s)
{
  cerr << getCurrentSourceLineNumber() << ": parse error" << endl;
}
