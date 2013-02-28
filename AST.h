#ifndef _AST_H
#define _AST_H
// Abstract Syntax Tree for T language
//
// the dump methods are for debugging - they display the AST to stderr.
//
// the analyze methods perform semantic analysis
//
// the encode methods perform code generation

#include "Type.h"
#include <string>
#include "SymbolTable.h"
#include <cassert>
#include <iostream>
using namespace std;

extern TypeModule* types;
extern SymbolTable* symbolTable;
// abstract class: all AST nodes derived from this
class AST_Node {
protected:
	int line;

public:
	virtual ~AST_Node();

	virtual void dump() = 0;

	virtual AST_Node* analyze() = 0;
	virtual void encode() = 0;

	// set the line number of the node
	virtual void setLineNumber(int);

	// get the line number of the node
	virtual int getLineNumber();

protected:
	AST_Node();
};

// abstract class: all list nodes derived from this
class AST_List: public AST_Node {
protected:
	AST_Node* item;
	AST_List* restOfList;

public:
	virtual ~AST_List();

	// default behavior for lists: recurse left, then recurse right
	virtual void dump();
	virtual AST_Node* analyze();
	virtual void encode();

protected:
	AST_List(AST_Node* newItem, AST_List* list);
};

// abstract class: all statements derived from this
class AST_Statement: public AST_Node {
public:
	virtual ~AST_Statement();

protected:
	AST_Statement();
};

// abstract class: all expression nodes dervived from this
class AST_Expression: public AST_Node {
public:
	virtual ~AST_Expression();

	Type* type;

protected:
	AST_Expression();
};
// print statement
class AST_Print: public AST_Statement {
protected:
	// represent with a AST_Expression because a Deref will be added
	AST_Expression* var;

public:
	~AST_Print();
	AST_Print(AST_Expression* var);

	void dump();
	AST_Node* analyze();
	void encode();
};

// abstract class: all binary operation expression nodes derived from this
class AST_BinaryOperator: public AST_Expression {
public:
	virtual ~AST_BinaryOperator();
	AST_Expression* left;
	AST_Expression* right;

protected:
	AST_BinaryOperator(AST_Expression* l, AST_Expression* r);
};

// abstract class: all unary operation expression nodes dervived from this
class AST_UnaryOperator: public AST_Expression {
public:
	virtual ~AST_UnaryOperator();
	AST_Expression* left;

protected:
	AST_UnaryOperator(AST_Expression* l);
};

// variable
class AST_Variable: public AST_Expression {
protected:
	char* name;

public:
	~AST_Variable();
	AST_Variable(char *id);

	void dump();
	AST_Node* analyze();
	void encode();
};

// integer literal
class AST_IntegerLiteral: public AST_Expression {
public:

	unsigned int value;
	bool isParentNeg;
	~AST_IntegerLiteral();
	AST_IntegerLiteral(unsigned int in);

	void dump();
	AST_Node* analyze();
	void encode();
};

// list of statements
class AST_StatementList: public AST_List {
public:
	~AST_StatementList();
	AST_StatementList(AST_Statement* statement, AST_List* restOfList);

	// inherit default behavior for dump, analyze, encode
};

// declaration statement
class AST_Declaration: public AST_Statement {
protected:
	Type* type;
	char* name;

public:
	~AST_Declaration();
	AST_Declaration(Type* type, char* name);

	void dump();
	AST_Node* analyze();
	void encode();
};

// divide 
class AST_Divide: public AST_BinaryOperator {
public:
	~AST_Divide();
	AST_Divide(AST_Expression* left, AST_Expression* right);

	void dump();
	AST_Node* analyze();
	void encode();
};

// convert unary operator
class AST_Convert: public AST_UnaryOperator {
public:
	~AST_Convert();
	AST_Convert(AST_Expression* left);

	void dump();
	AST_Node* analyze();
	void encode();
};

// dereference unary operator
class AST_Deref: public AST_UnaryOperator {
public:
	~AST_Deref();
	AST_Deref(AST_Expression* left);

	void dump();
	AST_Node* analyze();
	void encode();
};

/////////////////////////////////////////////////////////////////////Yanhua Liu

/*************************************************************************
 * AST_MainBlockStatements
 *************************************************************************/

class AST_MainBlockStatements: public AST_StatementList {
public:
	AST_MainBlockStatements(AST_Statement* st,
			AST_MainBlockStatements* st_list);

	virtual ~AST_MainBlockStatements();
	virtual void dump();
	;

};

/*************************************************************************
 * AST_MainBlock
 *************************************************************************/

class AST_MainBlock: public AST_Node {
public:
	AST_MainBlockStatements *mbs;
	AST_MainBlock(AST_MainBlockStatements *mbs);
	virtual void dump();
	virtual AST_Node* analyze();
	virtual void encode();
	virtual ~AST_MainBlock();
};

/*************************************************************************
 * AST_Type
 *************************************************************************/

class AST_Type: public AST_Node {
public:
	Type* type;
	virtual Type* getType();
	virtual void dump();
	virtual AST_Node* analyze();
	virtual void encode() {
	}
	;
};

/*************************************************************************
 * AST_Identifier
 *************************************************************************/

class AST_Identifier: public AST_Expression {
public:
	char *name;
	AST_Type *ast_type;
	AST_Identifier(char* name);
	~AST_Identifier();
	virtual void dump();
	virtual void set_type(AST_Type* t);
	virtual void set_type(Type* t);
	AST_Node* analyze();
	void encode() {
	}
	;
	char* getname();
};

/*************************************************************************
 * AST_VariableDeclarator
 *************************************************************************/

class AST_VariableDeclarator: public AST_Expression {
public:
	char *name;
	AST_Type * ast_type;
	AST_Identifier *id;
	AST_VariableDeclarator(AST_Identifier* id);
	~AST_VariableDeclarator();
	virtual void dump();
	virtual void set_type(AST_Type* t);
	virtual AST_Node* analyze();
	void encode();
};

/*************************************************************************
 * AST_VariableDeclarators
 *************************************************************************/

class AST_VariableDeclarators: public AST_List {
public:

	AST_Type * ast_type;
	AST_VariableDeclarators(AST_Node *item, AST_List *rest);
	virtual void dump();
	virtual void set_type(AST_Type* t);
};

/*************************************************************************
 * AST_MainVariableDeclaration
 *************************************************************************/
class AST_MainVariableDeclaration: public AST_Expression {
public:
	AST_VariableDeclarators *VD;
	AST_Type *ast_type;
	AST_MainVariableDeclaration(AST_Type* ast_type,
			AST_VariableDeclarators * VD);
	virtual void dump();
	virtual AST_Node* analyze();
	virtual void encode();
	~AST_MainVariableDeclaration();
};

/*************************************************************************
 * AST_MainVariableDeclarationStatement
 *************************************************************************/

class AST_MainVariableDeclarationStatement: public AST_Statement {
public:
	AST_MainVariableDeclaration *MVD;
	AST_MainVariableDeclarationStatement(AST_MainVariableDeclaration *MVD);
	virtual void dump();
	virtual AST_Node* analyze();
	virtual void encode();
	~AST_MainVariableDeclarationStatement();
};

/*************************************************************************
 * AST_IntegralType
 *************************************************************************/

class AST_IntegralType: public AST_Type {
public:
	Type* type;
	virtual Type* getType();
	AST_IntegralType();
	~AST_IntegralType();
	virtual void dump();
	virtual AST_Node* analyze();
	virtual void encode() {
	}
	;
};

/*************************************************************************
 * AST_LeftHandSide
 *************************************************************************/

class AST_LeftHandSide: public AST_VariableDeclarator {
public:
	AST_LeftHandSide(AST_Identifier *id);
	~AST_LeftHandSide();
	virtual void dump();
	AST_Node *analyze();
	virtual void encode();
};

// assignment statement

/*************************************************************************
 * AST_Assignment
 *************************************************************************/
class AST_Assignment: public AST_Expression {
protected:
	AST_Expression* lhs;
	AST_Expression* rhs;

public:
	~AST_Assignment();
	AST_Assignment(AST_Expression* lhs, AST_Expression* rhs);
	virtual void dump();
	AST_Node* analyze();
	void encode();
};

/*************************************************************************
 * AST_ExpressionStatement
 *************************************************************************/
class AST_ExpressionStatement: public AST_Statement {
public:
	AST_Assignment *assign;
	AST_ExpressionStatement(AST_Assignment* assign);
	virtual ~AST_ExpressionStatement();
	virtual void dump();
	AST_Node* analyze();
	void encode();
};

/*************************************************************************
 * AST_Literal
 *************************************************************************/

class AST_Literal: public AST_IntegerLiteral {
public:
	AST_Literal(unsigned int v);
	~AST_Literal();
};

/*************************************************************************
 * AST_OutputStatement
 *************************************************************************/

class AST_OutputStatement: public AST_Statement {
protected:
	// represent with a AST_Expression because a Deref will be added
	AST_Expression* var;

public:
	~AST_OutputStatement();
	AST_OutputStatement(AST_Expression* var);
	void dump();
	AST_Node* analyze();
	void encode();
};

/*************************************************************************
 *AST_EqualityExpression
 *************************************************************************/

class AST_EqualityExpression: public AST_BinaryOperator {

public:
	string name;
	virtual ~AST_EqualityExpression();
	AST_EqualityExpression(AST_Expression* left, AST_Expression* right);
	virtual void dump();
	virtual AST_Node* analyze();
	virtual void encode();
};

/*************************************************************************
 *AST_RelationalExpression
 *************************************************************************/

class AST_RelationalExpression: public AST_EqualityExpression {
public:
	string sign;
	virtual ~AST_RelationalExpression();
	AST_RelationalExpression(AST_Expression* left, AST_Expression* right,
			bool mark);
	virtual void dump();
	virtual void encode();
};

/*************************************************************************
 *AST_AdditiveExpression
 *************************************************************************/
class AST_AdditiveExpression: public AST_RelationalExpression {
public:
	virtual ~AST_AdditiveExpression();
	AST_AdditiveExpression(AST_Expression* left, AST_Expression* right,
			bool mark);
	virtual void encode();
};



/*************************************************************************
 *AST_MultiplicativeExpression
 *************************************************************************/
class AST_MultiplicativeExpression: public AST_AdditiveExpression {
public:
	virtual ~ AST_MultiplicativeExpression();
	AST_MultiplicativeExpression(AST_Expression* left, AST_Expression* right,
			bool mark);
	virtual void encode();
};



/*************************************************************************
 *AST_Primary
 *************************************************************************/

class AST_Primary: public AST_Expression {
public:
	char *name;
	int obj; //0:identifier 1: expression
	AST_Expression* var;
	AST_Primary(AST_Expression* var, int obj);
	virtual ~AST_Primary();
	virtual void dump();
	virtual AST_Node* analyze();
	virtual void encode();
};


/*************************************************************************
 *AST_UnaryExpression
 *************************************************************************/

class AST_UnaryExpression: public AST_UnaryOperator {
public:
	string name;
	string sign;
	AST_UnaryExpression(AST_Expression* left, bool mark);
	~AST_UnaryExpression();
	void dump();
	AST_Node* analyze();
	void encode();
};

<<<<<<< HEAD

////////////////////////   PHASE 2    ////////////////////////////////////


/*************************************************************************
 *AST_BlockStatements
 *************************************************************************/

class AST_BlockStatements:public AST_StatementList {
public:
	AST_BlockStatements(AST_Statement* st,
			AST_BlockStatements* st_list);
	virtual ~AST_BlockStatements();
	virtual void dump();
};

/*************************************************************************
 *AST_Block
 *************************************************************************/
class AST_Block:public AST_Statement
{
public:
	AST_BlockStatements *BlockStatements;
	AST_Block(AST_BlockStatements *BlockStatements);
	void dump();
	AST_Node* analyze();
	void encode();
	virtual ~AST_Block();
};


/*************************************************************************
 *AST_IfThenElseStatement
 *************************************************************************/
class AST_IfThenElseStatement:public AST_Statement
{
public:
	AST_Expression *expression;
	AST_Statement *statement1;
	AST_Statement *statement2;
	AST_IfThenElseStatement(AST_Expression *expression,AST_Statement *statement1,AST_Statement *statement2);
	~AST_IfThenElseStatement();
	void dump();
	AST_Node* analyze();
	void encode();
};


/*************************************************************************
 *AST_WhileStatement
 *************************************************************************/
class AST_WhileStatement:public AST_Statement
{
public:
	AST_Expression *expression;
	AST_Statement *statement;
	AST_WhileStatement(AST_Expression *expression,AST_Statement *statement);
	~AST_WhileStatement();
	void dump();
	AST_Node* analyze();
	void encode();
};



/*************************************************************************
 *AST_ReturnStatement
 *************************************************************************/
class AST_ReturnStatement:public AST_Statement
{
public:
	AST_Expression *expression;
	AST_ReturnStatement(AST_Expression *expression);
	~AST_ReturnStatement();
	void dump();
	AST_Node* analyze();
	void encode();
};


/*************************************************************************
 *AST_BreakStatement
 *************************************************************************/
class AST_BreakStatement:public AST_Statement
{
public:
	string name;
	AST_BreakStatement(string uname="AST_BreakStatement");
	void dump();
	AST_Node* analyze();
	void encode();
};

/*************************************************************************
 *AST_ContinueStatement
 *************************************************************************/
class AST_ContinueStatement:public AST_BreakStatement
{
public:
	AST_ContinueStatement(string uname="AST_ContinueStatement");
	void encode();
};

/*************************************************************************
 *AST_EmptyStatement
 *************************************************************************/
class AST_EmptyStatement:public AST_BreakStatement
{
public:
	AST_EmptyStatement(string uname="AST_EmptyStatement");
	void encode();
};

=======
>>>>>>> a3052501d3fde445dd8ab41d969fe1de0be0999d
#endif
