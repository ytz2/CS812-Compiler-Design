
// AST methods for semantic analysis

#include <iostream>
using namespace std;

#include "AST.h"
#include "SymbolTable.h"
#include "Type.h"

// global symbol table is in main.cxx
extern SymbolTable* symbolTable;

// global type module is in main.cxx
extern TypeModule* types;

AST_Node* AST_List::analyze() {
	if (restOfList != NULL)
		restOfList = (AST_List*) restOfList->analyze();
	item = item->analyze();
	return (AST_Node*) this;
}

AST_Node* AST_IntegerLiteral::analyze() {
	// nothing to be done; type already set
	if ((this->isParentNeg && this->value > 2147483648)
			|| (!this->isParentNeg && this->value >= 2147483648)) {
		cerr << line << ": Integer " << " overflows!\n";
		type = types->errorType();
	}
	return (AST_Node*) this;
}

AST_Node* AST_Variable::analyze() {
	Type* typeFromSymbolTable;

	if (symbolTable->lookup(name, typeFromSymbolTable)) {
		type = typeFromSymbolTable;
	} else {
		cerr << line << ": variable " << name << " is not declared!\n";
		type = types->errorType();
	}

	// always put Deref node on top of a variable
	AST_Expression* ret = (AST_Expression*) new AST_Deref(this);
	ret->type = type;

	return (AST_Node*) ret;
}

AST_Node* AST_Declaration::analyze() {
	// place declaration into the symbol table
	if (!symbolTable->install(name, type)) {
		cerr << line << ": duplicate declaration for " << name << endl;
	}

	return (AST_Node*) this;
}

AST_Node* AST_Print::analyze() {
	// analyze the variable - a Deref node will be added
	var = (AST_Expression*) var->analyze();

	return (AST_Node*) this;
}

AST_Node* AST_Divide::analyze() {
	// analyze both subtrees
	left = (AST_Expression*) left->analyze();
	right = (AST_Expression*) right->analyze();

	// check for error in either subtree to avoid cascade of errors
	if ((left->type == types->errorType())
			|| (right->type == types->errorType())) {
		type = types->errorType();
		return (AST_Node*) this;
	}

	// if types not the same then add a convert node to the int subtree
	// (only two legal types in this language: int and float)
	if (left->type != right->type) {
		if (left->type == types->intType()) {
			AST_Expression* newNode = new AST_Convert(left);
			newNode->type = types->floatType();
			left = newNode;
		} else if (right->type == types->intType()) {
			AST_Expression* newNode = new AST_Convert(right);
			newNode->type = types->floatType();
			right = newNode;
		} else {
			cerr << line << ": BUG in AST_Divide::analyze: types are different "
					<< "but neither is int\n";
			//exit(-1);
		}
		type = types->floatType();
	}

	// both types the same at this point
	type = left->type;

	return (AST_Node*) this;
}

AST_Node* AST_Convert::analyze() {
	// convert node is added in analyze but should never be analyzed itself
	cerr << line << ": BUG in AST_Convert::analyze: should never be called\n";

	return (AST_Node*) this;
}

AST_Node* AST_Deref::analyze() {
	// deref node is added in analyze but should never be analyzed itself
	cerr << line << ": BUG in AST_Deref::analyze: should never be called\n";

	return (AST_Node*) this;
}

/*************************************************************************
 * AST_MainBlock
 *************************************************************************/

AST_Node* AST_MainBlock::analyze() {

	if (this->mbs != NULL)
		this->mbs->analyze();
	return (AST_Node*) this;
}

/*************************************************************************
 * AST_Type
 *************************************************************************/

AST_Node* AST_Type::analyze() {
	return NULL;
}

/*************************************************************************
 * AST_Identifier
 *************************************************************************/
AST_Node* AST_Identifier::analyze() {
	return (AST_Node*) this;
}

/*************************************************************************
 * AST_VariableDeclarator
 *************************************************************************/

AST_Node* AST_VariableDeclarator::analyze() {

	type = ast_type->getType();
	id->set_type(type);
	id->analyze();
	// place declaration into the symbol table
	if (!symbolTable->install(name, type)) {
		cerr << line << ": duplicate declaration for " << name << endl;
	}
	return (AST_Node*) this;
}

/*************************************************************************
 * AST_MainVariableDeclaration
 *************************************************************************/

AST_Node* AST_MainVariableDeclaration::analyze() {
	assert(ast_type);
	assert(VD);
	ast_type->analyze();
	VD->set_type(ast_type);
	VD->analyze();
	return (AST_Node*) this;
}

/*************************************************************************
 * AST_MainVariableDeclarationStatement
 *************************************************************************/
AST_Node* AST_MainVariableDeclarationStatement::analyze() {

	if (this->MVD != NULL)
		this->MVD->analyze();
	return (AST_Node*) this;
}

/*************************************************************************
 * AST_IntegralType
 *************************************************************************/

AST_Node* AST_IntegralType::analyze() {

	return (AST_Node*) this;
}

/*************************************************************************
 * AST_LeftHandSide
 *************************************************************************/

AST_Node* AST_LeftHandSide::analyze() {

	Type* typeFromSymbolTable;

	if (symbolTable->lookup(name, typeFromSymbolTable)) {
		type = typeFromSymbolTable;
	} else {
		cerr << line << ": variable " << name << " is not declared!\n";
		type = types->errorType();
	}

	// always put Deref node on top of a variable
	AST_Expression* ret = (AST_Expression*) new AST_Deref(this);
	ret->type = type;
	id->set_type(type);
	id->analyze();
	return (AST_Node*) ret;
}

/*************************************************************************
 * AST_Assignment
 *************************************************************************/

AST_Node* AST_Assignment::analyze() {

	// analyze the lhs variable - a Deref node will be put on top
	AST_Deref* deref = (AST_Deref*) lhs->analyze();

	// strip off the Deref node
	lhs = deref->left;
	deref->left = NULL;
	delete deref;

	// analyze the expression
	rhs = (AST_Expression*) rhs->analyze();

	// check if error was detected in one of the subtrees
	// ie avoid a cascade of error messages
	if ((lhs->type == types->errorType())
			|| (rhs->type == types->errorType())) {
		return (AST_Node*) this;
	}

	// add a convert node if the types are not the same
	if (lhs->type != rhs->type) {
		AST_Expression* newNode = new AST_Convert(rhs);

		newNode->type = lhs->type;

		rhs = newNode;
	}
	return (AST_Node*) this;
}

/*************************************************************************
 * AST_ExpressionStatement
 *************************************************************************/

AST_Node* AST_ExpressionStatement::analyze() {

	if (assign != NULL)
		assign->analyze();
	return (AST_Node*) this;
}

/*************************************************************************
 * AST_OutputStatement
 *************************************************************************/

AST_Node* AST_OutputStatement::analyze() {
	// analyze the variable - a Deref node will be added
	var = (AST_Expression*) var->analyze();

	return (AST_Node*) this;
}

/*************************************************************************
 *AST_EqualityExpression
 *************************************************************************/

AST_Node* AST_EqualityExpression::analyze() {

	// analyze both subtrees
	left = (AST_Expression*) left->analyze();
	right = (AST_Expression*) right->analyze();

	// check for error in either subtree to avoid cascade of errors
	if ((left->type == types->errorType())
			|| (right->type == types->errorType())) {
		//type = types->errorType();
		return (AST_Node*) this;
	}
	type = left->type;
	/*
	 // if types not the same then add a convert node to the int subtree
	 // (only two legal types in this language: int and float)
	 if (left->type != right->type)
	 {
	 if (left->type == types->intType())
	 {
	 AST_Expression* newNode = new AST_Convert(left);
	 newNode->type = types->floatType();
	 left = newNode;
	 }
	 else if (right->type == types->intType())
	 {
	 AST_Expression* newNode = new AST_Convert(right);
	 newNode->type = types->floatType();
	 right = newNode;
	 }
	 else
	 {
	 cerr << line << ": BUG in AST_EqualityExpression::analyze: types are different "
	 << "but neither is int\n";
	 //exit(-1);
	 }
	 type = types->floatType();
	 }

	 // both types the same at this point
	 type = left->type;
	 */
	return (AST_Node*) this;

}

/*************************************************************************
 *AST_Primary
 *************************************************************************/

AST_Node* AST_Primary::analyze() {
	if (obj == 0) {
		name = ((AST_Identifier*) var)->getname();
		Type* typeFromSymbolTable;

		if (symbolTable->lookup(name, typeFromSymbolTable)) {
			type = typeFromSymbolTable;
		} else {
			cerr << line << ": variable " << name << " is not declared!\n";
			type = types->errorType();
		}

		// always put Deref node on top of a variable
		AST_Expression* ret = (AST_Expression*) new AST_Deref(this);
		ret->type = type;
		((AST_Identifier*) var)->set_type(type);
		((AST_Identifier*) var)->analyze();
		return (AST_Node*) ret;
	} else if (obj == 1) {
		var->analyze();
		type = var->type;
		return (AST_Node*) this;
	};
	return NULL;
}

/*************************************************************************
 *AST_UnaryExpression
 *************************************************************************/

AST_Node* AST_UnaryExpression::analyze() {
	left = (AST_Expression*) left->analyze();
	// check for error in either subtree to avoid cascade of errors
	if (left->type == types->errorType()) {
		type = types->errorType();
		return (AST_Node*) this;
	}
	// both types the same at this point
	type = left->type;
	return (AST_Node*) this;
}

/*************************************************************************
 *AST_Block
 *************************************************************************/

AST_Node * AST_Block::analyze()
{
	if (this->BlockStatements != NULL)
		this->BlockStatements->analyze();
	return (AST_Node*) this;
}


/*************************************************************************
 *AST_IfThenElseStatement
 *************************************************************************/
AST_Node *AST_IfThenElseStatement::analyze()
{
	this->expression->analyze();
	this->statement1->analyze();
	this->statement2->analyze();
	return (AST_Node*) this;
}

/*************************************************************************
 *AST_WhileStatement
 *************************************************************************/
AST_Node *AST_WhileStatement::analyze()
{
	this->expression->analyze();
	this->statement->analyze();
	return (AST_Node*) this;
}

/*************************************************************************
 *AST_WhileStatement
 *************************************************************************/
AST_Node *AST_ReturnStatement::analyze()
{
	if (this->expression!=NULL)
		this->expression->analyze();
	return (AST_Node*) this;
}


/*************************************************************************
 *AST_BreakStatement
 *************************************************************************/
AST_Node *AST_BreakStatement::analyze()
{
	return (AST_Node*) this;
}


