
// Abstract Syntax Tree implementation for Simp language
//
// semantic analysis methods are in analyze.cxx

#include <iostream>
using namespace std;

#include "AST.h"
#include "Type.h"

// this routine is in scan.ll
int getCurrentSourceLineNumber();

// global type module is in main.cxx
extern TypeModule* types;

AST_Node::AST_Node() {
	// by default get the current line number in the scanner
	line = getCurrentSourceLineNumber();
}

AST_Node::~AST_Node() {
}

void AST_Node::setLineNumber(int lineNumber) {
	line = lineNumber;
}

int AST_Node::getLineNumber() {
	return line;
}

AST_List::AST_List(AST_Node* newItem, AST_List* list) {
	item = newItem;
	restOfList = list;
}

AST_List::~AST_List() {
	delete item;
	if (restOfList != NULL)
		delete restOfList;
}

void AST_List::dump() {
	if (restOfList != NULL)
		restOfList->dump();
//  else cerr << "First iterm in the List\n";
//  cerr << "List Item\n";
	item->dump();
}

AST_Statement::~AST_Statement() {
}

AST_Statement::AST_Statement() {
}

AST_Expression::~AST_Expression() {
}

AST_Expression::AST_Expression() {
	type = types->noType();
}

AST_BinaryOperator::~AST_BinaryOperator() {
	delete left;
	delete right;
}

AST_BinaryOperator::AST_BinaryOperator(AST_Expression* l, AST_Expression* r) {
	left = l;
	right = r;
}

AST_UnaryOperator::~AST_UnaryOperator() {
	delete left;
}

AST_UnaryOperator::AST_UnaryOperator(AST_Expression* l) {
	left = l;
}

AST_IntegerLiteral::~AST_IntegerLiteral() {
}

AST_IntegerLiteral::AST_IntegerLiteral(unsigned int in) {
	type = types->intType();
	value = in;
	this->isParentNeg = false;
}

void AST_IntegerLiteral::dump() {
	cerr << "IntegerLiteral" << "  " << type->toString() << "  " << value
			<< endl;
}

AST_Variable::AST_Variable(char* in) {
	name = in;
}

AST_Variable::~AST_Variable() {
}

void AST_Variable::dump() {
	cerr << "Variable " << name << " " << type->toString() << endl;
}

AST_StatementList::AST_StatementList(AST_Statement* s, AST_List* l) :
		AST_List((AST_Node*) s, l) {
}

AST_StatementList::~AST_StatementList() {
}

AST_Declaration::AST_Declaration(Type* t, char* n) {
	type = t;
	name = n;
}

AST_Declaration::~AST_Declaration() {
}

void AST_Declaration::dump() {
	cerr << "Declaration " << type->toString() << " " << name << endl;
}

AST_Divide::AST_Divide(AST_Expression* l, AST_Expression* r) :
		AST_BinaryOperator(l, r) {
}

AST_Divide::~AST_Divide() {
}

void AST_Divide::dump() {
	cerr << "Divide " << type->toString() << endl;
	left->dump();
	right->dump();
}

AST_Convert::AST_Convert(AST_Expression* l) :
		AST_UnaryOperator(l) {
}

AST_Convert::~AST_Convert() {
}

void AST_Convert::dump() {
	cerr << "Convert " << type->toString() << endl;
	left->dump();
}

AST_Deref::AST_Deref(AST_Expression* l) :
		AST_UnaryOperator(l) {
}

AST_Deref::~AST_Deref() {
}

void AST_Deref::dump() {
	cerr << "Deref " << type->toString() << endl;
	left->dump();
}

/*************************************************************************
 * AST_MainBlockStatements
 *************************************************************************/

AST_MainBlockStatements::AST_MainBlockStatements(AST_Statement* st,
		AST_MainBlockStatements* st_list) :
		AST_StatementList(st, st_list) {
}
;

AST_MainBlockStatements::~AST_MainBlockStatements() {

}

void AST_MainBlockStatements::dump() {
	cerr << "AST_MainBlockStatements\n";

	if (restOfList != NULL)
		restOfList->dump();
	//else cerr << "First iterm in the List\n";
	//cerr << "List Item\n";
	item->dump();
}

/*************************************************************************
 * AST_MainBlock
 *************************************************************************/

AST_MainBlock::AST_MainBlock(AST_MainBlockStatements *mbs) {
	this->mbs = mbs;
}

AST_MainBlock::~AST_MainBlock() {
	if (mbs)
		delete mbs;
}

void AST_MainBlock::dump() {
	cerr << endl;
	if (this->mbs != NULL) {
		cerr << "AST_MainBlock" << endl;
		mbs->dump();
	} else {
		cerr << endl;
		cerr << "AST_MainBlock with empty statements" << endl;
	}
}

/*************************************************************************
 * AST_Type
 *************************************************************************/
void AST_Type::dump() {
	cerr << "AST_Type: I should not be called" << endl;
}

Type* AST_Type::getType() {
	return types->errorType();
}

/*************************************************************************
 * AST_Identifier
 *************************************************************************/

AST_Identifier::AST_Identifier(char* name) {
	this->name = name;
	type = types->noType();
}

AST_Identifier::~AST_Identifier() {
}

void AST_Identifier::dump() {
	cerr << "AST_Identifier" << "  " << type->toString() << "   " << name
			<< endl;
}

void AST_Identifier::set_type(AST_Type* t) {
	this->ast_type = t;
}

void AST_Identifier::set_type(Type* t) {
	type = t;
}

char* AST_Identifier::getname() {
	return name;
}

/*************************************************************************
 * AST_VariableDeclarator
 *************************************************************************/

AST_VariableDeclarator::AST_VariableDeclarator(AST_Identifier* id) {
	this->id = id;
	name = id->getname();
	type = types->noType();
}

AST_VariableDeclarator::~AST_VariableDeclarator() {
}

void AST_VariableDeclarator::dump() {
	cerr << "AST_VariableDeclarator" << "  " << type->toString() << endl;
	id->dump();
}

void AST_VariableDeclarator::set_type(AST_Type* t) {
	this->ast_type = t;
}

/*************************************************************************
 * AST_VariableDeclarators
 *************************************************************************/

AST_VariableDeclarators::AST_VariableDeclarators(AST_Node *item, AST_List *rest) :
		AST_List(item, rest) {
}

void AST_VariableDeclarators::dump() {

	cerr << "AST_VariableDeclarators\n";
	if (restOfList != NULL)
		restOfList->dump();
	//	else
	//		cerr << "List Items:\n";
	item->dump();
}
void AST_VariableDeclarators::set_type(AST_Type* t) {
	ast_type = t;
	if (item != NULL)
		((AST_VariableDeclarator*) item)->set_type(ast_type);
	if (restOfList != NULL)
		((AST_VariableDeclarators*) restOfList)->set_type(ast_type);
}

/*************************************************************************
 * AST_MainVariableDeclaration
 *************************************************************************/

AST_MainVariableDeclaration::AST_MainVariableDeclaration(AST_Type* ast_type,
		AST_VariableDeclarators * VD) {
	type = types->noType();
	this->ast_type = ast_type;
	this->VD = VD;
}
void AST_MainVariableDeclaration::dump() {
	cerr << "AST_MainVariableDeclaration" << endl;
	ast_type->dump();
	VD->dump();
}
AST_MainVariableDeclaration::~AST_MainVariableDeclaration() {
	if (ast_type != NULL)
		delete ast_type;
	if (VD != NULL)
		delete VD;

}

/*************************************************************************
 * AST_MainVariableDeclaration
 *************************************************************************/

AST_MainVariableDeclarationStatement::AST_MainVariableDeclarationStatement(
		AST_MainVariableDeclaration *MVD) {
	this->MVD = MVD;
}
void AST_MainVariableDeclarationStatement::dump() {
	cerr << "AST_MainVariableDeclarationStatement" << endl;
	if (this->MVD != NULL) {
		MVD->dump();
	}
}
AST_MainVariableDeclarationStatement::~AST_MainVariableDeclarationStatement() {
	if (MVD != NULL)
		delete MVD;
}

/*************************************************************************
 * AST_IntegralType
 *************************************************************************/
Type* AST_IntegralType::getType() {
	return type;
}

AST_IntegralType::AST_IntegralType() {
	type = types->intType();
}

AST_IntegralType::~AST_IntegralType() {
}

void AST_IntegralType::dump() {
	cerr << "AST_IntegeralType " << "  " << type->toString() << endl;
}

/*************************************************************************
 * AST_LeftHandSide
 *************************************************************************/

AST_LeftHandSide::AST_LeftHandSide(AST_Identifier *id) :
		AST_VariableDeclarator(id) {
}

AST_LeftHandSide::~AST_LeftHandSide() {
}

void AST_LeftHandSide::dump() {
	cerr << "AST_LeftHandSide " << name << " " << type->toString() << endl;
	id->dump();
}

/*************************************************************************
 * AST_Assignment
 *************************************************************************/

AST_Assignment::AST_Assignment(AST_Expression* l, AST_Expression* r) {
	lhs = l;
	rhs = r;
}

AST_Assignment::~AST_Assignment() {
	if (this->lhs != NULL)
		delete lhs;
	if (this->rhs != NULL)
		delete rhs;
}

void AST_Assignment::dump() {
	cerr << "AST_Assignment" << endl;
	lhs->dump();
	rhs->dump();
}

/*************************************************************************
 * AST_ExpressionStatement
 *************************************************************************/

AST_ExpressionStatement::AST_ExpressionStatement(AST_Assignment* assign) {
	this->assign = assign;
}

AST_ExpressionStatement::~AST_ExpressionStatement() {
	if (assign)
		delete assign;
}
void AST_ExpressionStatement::dump() {
	cerr << "AST_ExpressionStatement" << endl;
	assign->dump();
}

/*************************************************************************
 * AST_Literal
 *************************************************************************/
AST_Literal::AST_Literal(unsigned int v) :
		AST_IntegerLiteral(v) {
}

AST_Literal::~AST_Literal() {
}

/*************************************************************************
 * AST_OutputStatement
 *************************************************************************/

AST_OutputStatement::AST_OutputStatement(AST_Expression* v) {
	var = (AST_Expression*) v;
}

AST_OutputStatement::~AST_OutputStatement() {
	if (var != NULL)
		delete var;
}

void AST_OutputStatement::dump() {
	cerr << "AST_OutputStatement" << endl;
	var->dump();
}

/*************************************************************************
 *AST_EqualityExpression
 *************************************************************************/

AST_EqualityExpression::~AST_EqualityExpression() {
}

AST_EqualityExpression::AST_EqualityExpression(AST_Expression* left,
		AST_Expression* right) :
		AST_BinaryOperator(left, right) {
	name = "AST_EqualityExpression";
}

void AST_EqualityExpression::dump() {
	cerr << name << "  " << type->toString() << endl;
	left->dump();
	right->dump();
}

/*************************************************************************
 *AST_RelationalExpression
 *************************************************************************/
AST_RelationalExpression::~AST_RelationalExpression() {
}
AST_RelationalExpression::AST_RelationalExpression(AST_Expression* left,
		AST_Expression* right, bool mark) :
		AST_EqualityExpression(left, right) {
	if (mark == true)
		sign = "<";
	else if (mark == false)
		sign = ">";
	else
		sign = "Unkown Operator";
	name = "AST_RelationalExpression";
}
void AST_RelationalExpression::dump() {
	cerr << name << "  " << type->toString() << endl;
	left->dump();
	right->dump();
}

/*************************************************************************
 *AST_AdditiveExpression
 *************************************************************************/

AST_AdditiveExpression::~AST_AdditiveExpression() {
}
AST_AdditiveExpression::AST_AdditiveExpression(AST_Expression* left,
		AST_Expression* right, bool mark) :
		AST_RelationalExpression(left, right, mark) {
	if (mark == true)
		sign = "+";
	else if (mark == false)
		sign = "-";
	else
		sign = "Unkown Operator";
	name = "AST_AdditivelExpression";
}

/*************************************************************************
 *AST_MultiplicativeExpression
 *************************************************************************/

AST_MultiplicativeExpression::~AST_MultiplicativeExpression() {
}

AST_MultiplicativeExpression::AST_MultiplicativeExpression(AST_Expression* left,
		AST_Expression* right, bool mark) :
		AST_AdditiveExpression(left, right, mark) {
	if (mark == true)
		sign = "*";
	else if (mark == false)
		sign = "/";
	else
		sign = "Unkown Operator";
	name = "AST_MultiplicativeExpression";
}

/*************************************************************************
 *AST_Primary
 *************************************************************************/

AST_Primary::AST_Primary(AST_Expression* var, int obj) {
	this->obj = obj;
	this->var = var;
	type = types->noType();
}

AST_Primary::~AST_Primary() {
	if (var != NULL)
		delete var;
}
void AST_Primary::dump() {
	cerr << "AST_Primary" << "  " << type->toString() << endl;
	if (obj == 0)
		((AST_Identifier*) var)->dump();
	else if (obj == 1)
		var->dump();
}

/*************************************************************************
 *AST_UnaryExpression
 *************************************************************************/

AST_UnaryExpression::AST_UnaryExpression(AST_Expression* left, bool mark) :
		AST_UnaryOperator(left) {
	name = "AST_UnarayExpression ";
	if (mark == true) {
		sign = "-";
		AST_Primary *temp = dynamic_cast<AST_Primary*>(left);
		if (temp != NULL && temp->obj == 1) {
			AST_IntegerLiteral *temp2 =
					dynamic_cast<AST_IntegerLiteral*>(temp->var);
			if (temp2 != NULL)
				temp2->isParentNeg = true;
			temp2 = NULL;
		}
		temp = NULL;
	} else if (mark == false)
		sign = "!";
}

AST_UnaryExpression::~AST_UnaryExpression() {
}

void AST_UnaryExpression::dump() {
	cerr << name << "  " << type->toString() << endl;
	left->dump();
}

<<<<<<< HEAD


/*************************************************************************
 *AST_BlockStatements
 *************************************************************************/


AST_BlockStatements::AST_BlockStatements(AST_Statement* st,
		AST_BlockStatements* st_list) :
		AST_StatementList(st, st_list) {
}


AST_BlockStatements::~AST_BlockStatements() {

}

void AST_BlockStatements::dump() {
	cerr << "AST_BlockStatements\n";

	if (restOfList != NULL)
		restOfList->dump();
	//else cerr << "First iterm in the List\n";
	//cerr << "List Item\n";
	item->dump();
}


/*************************************************************************
 *AST_Block
 *************************************************************************/
AST_Block::AST_Block(AST_BlockStatements *BlockStatements)
{
	this->BlockStatements=BlockStatements;
}

AST_Block::~AST_Block()
{
	if (this->BlockStatements)
		delete this->BlockStatements;
}

void AST_Block::dump()
{
	cerr<<"AST_Block"<<endl;
	if (this->BlockStatements!=NULL)
		this->BlockStatements->dump();
}

/*************************************************************************
 *AST_IfThenElseStatement
 *************************************************************************/
AST_IfThenElseStatement::AST_IfThenElseStatement(AST_Expression *expression,AST_Statement *statement1,AST_Statement *statement2)
{
	this->expression=expression;
	this->statement1=statement1;
	this->statement2=statement2;
}
AST_IfThenElseStatement::~AST_IfThenElseStatement()
{
	if (this->expression!=NULL)
		delete this->expression;
	if (this->statement1!=NULL)
		delete this->statement1;
	if (this->statement2!=NULL)
		delete this->statement2;
}
void AST_IfThenElseStatement::dump()
{
	cerr<<"AST_IfThenElseStatement"<<endl;
	this->expression->dump();
	this->statement1->dump();
	this->statement2->dump();
}

/*************************************************************************
 *AST_WhileStatement
 *************************************************************************/
AST_WhileStatement::AST_WhileStatement(AST_Expression *expression,AST_Statement *statement)
{
	this->expression=expression;
	this->statement=statement;
}
AST_WhileStatement::~AST_WhileStatement()
{
	if (this->expression!=NULL)
		delete this->expression;
	if (this->statement!=NULL)
		delete this->statement;
}
void AST_WhileStatement::dump()
{
	cerr<<"AST_WhileStatement"<<endl;
	this->expression->dump();
	this->statement->dump();
}


/*************************************************************************
 *AST_ReturnStatement
 *************************************************************************/
AST_ReturnStatement::AST_ReturnStatement(AST_Expression *expression)
{
	this->expression=expression;
}
AST_ReturnStatement::~AST_ReturnStatement()
{
	if (this->expression!=NULL)
		delete this->expression;
}
void AST_ReturnStatement::dump()
{
	cerr<<"AST_ReturnStatement"<<endl;
	if (this->expression!=NULL)
		this->expression->dump();
}


/*************************************************************************
 *AST_BreakStatement
 *************************************************************************/
void AST_BreakStatement::dump()
{
	cerr<<this->name<<endl;
}

AST_BreakStatement::AST_BreakStatement(string uname)
{
	this->name=uname;
}


/*************************************************************************
 *AST_ContinueStatement
 *************************************************************************/
AST_ContinueStatement::AST_ContinueStatement(string uname)
	:AST_BreakStatement(uname)
{

}


/*************************************************************************
 *AST_EmptyStatement
 *************************************************************************/
AST_EmptyStatement::AST_EmptyStatement(string uname)
	:AST_BreakStatement(uname)
{

}

=======
>>>>>>> a3052501d3fde445dd8ab41d969fe1de0be0999d
