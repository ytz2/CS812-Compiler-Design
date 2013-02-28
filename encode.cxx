// Jan 2008
// AST methods for code generation

#include <stdlib.h>
#include <string>
#include <iostream>
using namespace std;

#include "AST.h"
#include "Type.h"
#include "hw1.h"
// global type module is in main.cxx
extern TypeModule* types;

// output the prelude code
void encodeInitialize()
{
  cout << "#\tPrologue\n";
  cout << "\t.text\n";
  cout << "\t.align\t4\n";
  cout << "\t.globl\tmain\n";
  cout << "main:\n";
  cout << "\tpushl\t%ebp\n";
  cout << "\tmovl\t%esp, %ebp\n";
}

// output the epilogue code
void encodeFinish()
{
  cout<< "MAINEXITLABEL:"<<endl;
  cout << "#\tEpilogue\n";
  cout <<"main$exit:\n";
  cout << "\tpopl\t%ebp\n";
  cout << "\tret\n";
}

void AST_List::encode()
{
	if (restOfList != NULL) restOfList->encode();
	item->encode();
}


void AST_Variable::encode()
{
  cout << "#\tVariable\n";
  cout << "\tpushl\t$" << name << endl;
}

void AST_Declaration::encode()
{
  cout << "#\tDeclaration\n";
  cout << "\t.data\n";
  cout << "\t.comm\t" << name << ", 4\n";
  cout << "\t.text\n";
}

void AST_Assignment::encode()
{
  lhs->encode();
  rhs->encode();

  cout << "#\tAssignment\n";
  cout << "\tpopl\t%eax\n";
  cout << "\tpopl\t%edx\n";
  cout << "\tmovl\t%eax, (%edx)\n";
  cout <<"\tpushl\t%eax\n";
}

void AST_Print::encode()
{
  var->encode();

  if (var->type == types->intType())
  {
    cout << "#\tPrint int\n";
    cout << "\tcall\tSIMP_RT_printInt\n";
    cout << "\taddl\t$4, %esp\n";
  }
  else if (var->type == types->floatType())
  {
    cout << "#\tPrint float\n";
    cout << "\tcall\tSIMP_RT_printFloat\n";
    cout << "\taddl\t$4, %esp\n";
  }
  else if (var->type == types->errorType())
  {
    // do nothing: there was a semantic error
  }
  else
  {
    cerr << line << ": BUG in AST_Print::encode: unknown type\n"; 
    exit(-1);
  }
}

void AST_Divide::encode()
{
  left->encode();
  right->encode();

  if (type == types->intType())
  {
    cout << "#\tDivide int\n";
    cout << "\tpopl\t%ecx\n";
    cout << "\tpopl\t%eax\n";
    cout << "\tcltd\n";
    cout << "\tidivl\t%ecx, %eax\n";
    cout << "\tpushl\t%eax\n";
  }
  else if (type == types->floatType())
  {
    cout << "#\tDivide float\n";
    cout << "\tflds\t(%esp)\n";
    cout << "\taddl\t$4, %esp\n";
    cout << "\tflds\t(%esp)\n";
    cout << "\tfdivp\n";
    cout << "\tfstps\t(%esp)\n";
  }
  else if (type == types->errorType())
  {
    // do nothing: there was a semantic error
  }
  else
  {
    cerr << line << ": BUG in AST_Divide::encode: unexpected type\n"; 
    exit(-1);
  }
}

void AST_Convert::encode()
{
  left->encode();

  if (type == types->intType())
  {
    cout << "#\tConvert to int\n";

    // load value into fp register
    cout << "\tflds\t(%esp)\n";

    // set rounding mode to truncate
      // store fp control word in first word
      cout << "\tfnstcw\t(%esp)\n";
      // put control word in %ax
      cout << "\tmovw\t(%esp), %ax\n";
      // set RC bits to "truncate"
      cout << "\tmovb\t$12, %ah\n";
      // put modified control word in 2nd slot
      cout << "\tmovw\t%ax, 2(%esp)\n";
      // load modified control word
      cout << "\tfldcw\t2(%esp)\n";
      // put old control word in %ax
      cout << "\tmovw\t(%esp), %ax\n";

    // convert value to signed int
    cout << "\tfistpl\t(%esp)\n";

    // re-establish old fp control word */
      // alloc one word on top of stack
        cout << "\tsubl\t$2, %esp\n";
      // put original cntrl word on stack
        cout << "\tmovw\t%ax, (%esp)\n";
      // re-load original control word
        cout << "\tfldcw\t(%esp)\n";
     // free word on top of stack
        cout << "\taddl\t$2, %esp\n";
  }
  else if (type == types->floatType())
  {
    cout << "#\tConvert to float\n";
    cout << "\tfildl\t(%esp)\n";
    cout << "\tfstps\t(%esp)\n";
  }
  else
  {
    cerr << line << ": BUG in AST_Convert::encode: unexpected type\n"; 
    exit(-1);
  }
}

void AST_Deref::encode()
{
  left->encode();

  // both types are 32 bits so no need to distinguish types here?
  cout << "#\tDeref\n";
  cout << "\tpopl\t%eax\n";
  cout << "\tmovl\t(%eax), %eax\n";
  cout << "\tpushl\t%eax\n";
}



/*************************************************************************
 * AST_MainBlockStatements
 *************************************************************************/


void AST_MainBlock::encode()
{
	encodeInitialize();
	this->mbs->encode();
	encodeFinish();
}

void AST_MainVariableDeclarationStatement::encode()
{
	this->MVD->encode();
}

void AST_ExpressionStatement::encode()
{
	this->assign->encode();
    cout<<"#\tExpressionStatement\n";
    cout<<"\taddl $4, %esp\n";
}
void AST_MainVariableDeclaration::encode()
{
	this->VD->encode();
}
void AST_VariableDeclarator::encode()
{
	cout << "#\tMainVarDeclaration\n";
	cout << "\t.data\n";
	cout << "\tmainvar$"<<name<<": .long 0\n";
	cout << "\t.text\n";
}
void AST_LeftHandSide::encode()
{
	cout << "#\tMainVariable\n";
	cout << "\tpushl\t$mainvar$" << name << endl;
}

void AST_EqualityExpression::encode()
{
	this->left->encode();
	this->right->encode();
	cout << "#\tEqualityExpression\n";
	cout<<"\tpopl\t%eax\n";
	cout<<"\tpopl\t%edx\n";
	cout<<"\tcmpl\t%eax,%edx\n";
	cout<<"\tsete\t%al\n";
	cout<<"\tmovzbl\t%al,%eax\n";
	cout<<"\tpushl\t%eax\n";
}

void AST_RelationalExpression::encode()
{
	this->left->encode();
	this->right->encode();
	cout << "#\tRelationalExpression\n";
	if (this->sign==">")
	{
		cout<<"\tpopl\t%edx\n";
		cout<<"\tpopl\t%eax\n";
		cout<<"\tcmpl\t%eax,%edx\n";
		cout<<"\tsetl\t%al\n";
		cout<<"\tmovzbl\t%al, %eax\n";
		cout<<"\tpushl\t%eax\n";
	}
	if (this->sign=="<")
	{
		cout<<"\tpopl\t%edx\n";
		cout<<"\tpopl\t%eax\n";
		cout<<"\tcmpl\t%eax,%edx\n";
		cout<<"\tsetg\t%al\n";
		cout<<"\tmovzbl\t%al,%eax\n";
		cout<<"\tpushl\t%eax\n";

	}
}

void AST_AdditiveExpression::encode()
{
	this->left->encode();
	this->right->encode();
	cout << "#\tAdditiveExpression\n";
	if (this->sign=="+")
	{
		cout<<"\tpopl\t%edx\n";
		cout<<"\tpopl\t%eax\n";
		cout<<"\taddl\t%edx, %eax\n";
		cout<<"\tpushl\t%eax\n";
	}
	if (this->sign=="-")
	{
		cout<<"\tpopl\t%edx\n";
		cout<<"\tpopl\t%eax\n";
		cout<<"\tsubl\t%edx, %eax\n";
		cout<<"\tpushl\t%eax\n";
	}
}

void AST_MultiplicativeExpression::encode()
{
	this->left->encode();
	this->right->encode();
	cout << "#\tMultiplicativeExpression\n";
	if (this->sign=="*")
	{
		cout<<"\tpopl\t%eax\n";
		cout<<"\tpopl\t%edx\n";
		cout<<"\timull\t%edx, %eax\n";
		cout<<"\tpushl\t%eax\n";
	}
	if (this->sign=="/")
	{
		cout<<"\tpopl\t%ecx\n";
		cout<<"\tcmpl\t$0, %ecx\n";
		cout<<"\tje\tLABEL"<<count_divide++<<endl;
		cout<<"\tpopl\t%eax\n";
		cout<<"\tcltd\n";
		cout<<"\tidivl\t%ecx, %eax\n";
		cout<<"\tpushl\t%eax\n";
		cout<<"\tjmp\tLABEL"<<count_divide++<<endl;
		cout<<"LABEL"<<count_divide-2<<":\n";
		cout<<"\tpushl\t$"<<this->getLineNumber()<<endl;
		cout<<"\tcall\tRTS_printDivideByZeroError\n";
		cout<<"LABEL"<<count_divide-1<<":\n";
	}
}

void AST_UnaryExpression::encode()
{
	this->left->encode();
	cout << "#\tUnaryExpression\n";
	if (this->sign=="!")
	{
		cout<<"\tpopl\t%eax\n";
		cout<<"\tcmpl\t$0,%eax\n";
		cout<<"\tsete\t%al\n";
		cout<<"\tmovzbl\t%al, %eax\n";
		cout<<"\tpushl\t%eax\n";
	}
	if (this->sign=="-")
	{
		cout<<"\tpopl\t%eax\n";
		cout<<"\tnegl\t%eax\n";
		cout<<"\tpushl\t%eax\n";
	}
}
void AST_Primary::encode()
{
	if (this->obj==1)
		this->var->encode();
	if (this->obj==0)
	{
		cout << "#\tPrimary\n";
		cout << "\tpushl\t$mainvar$" << name << endl;
	}
}

void AST_IntegerLiteral::encode()
{
	cout << "#\tIntegerLiteral\n";
	cout << "\tpushl\t$" << value << endl;
}

void AST_OutputStatement::encode()
{
	this->var->encode();
	cout << "#\tOutputStatement\n";
	cout<<"\tcall\tRTS_outputInteger\n";
	cout<<"\taddl\t$4, %esp\n";
}

void AST_Block::encode()
{
  if (this->BlockStatements!=NULL)
    this->BlockStatements->encode();
}

void AST_IfThenElseStatement::encode()
{
	this->expression->encode();
	cout << "#\tIfThenElseStatement\n";
	cout<<"\tpopl\t%eax"<<endl;
	cout<<"\tcmpl\t$0,\t%eax"<<endl;
	cout<<"\tje\tLABEL"<<count_divide++<<endl;
	this->statement1->encode();
	cout<<"\tjmp\tLABEL"<<count_divide++<<endl;
	cout<<"LABEL"<<count_divide-2<<":"<<endl;
	this->statement2->encode();
	cout<<"LABEL"<<count_divide-1<<":"<<endl;
}

void AST_WhileStatement::encode()
{
	string blabel,clabel;
	break_label->push();
	continue_label->push();
	blabel=break_label->top();
	clabel=continue_label->top();
	cout << "#\tWhileStatement\n";
	cout <<clabel<<":"<<endl;
	this->expression->encode();
	cout<<"\tpopl\t%eax"<<endl;
	cout<<"\tcmpl\t$0,\t%eax"<<endl;
	cout<<"\tje\t"<<blabel<<endl;
	this->statement->encode();
	cout<<"\tjmp\t"<<clabel<<endl;
	cout<<blabel<<":"<<endl;
	break_label->pop();
	continue_label->pop();
}

void AST_BreakStatement::encode()
{
	if (break_label->isEmpty())
	{
		 cerr << line << ": No block to break\n";
		 exit(-1);
	}
	cout << "#\tBreaktatement\n";
	cout<<"\tjmp\t"<<break_label->top()<<endl;
}

void AST_ContinueStatement::encode()
{
	if (continue_label->isEmpty())
	{
		cerr << line << ": No block to continue\n";
		exit(-1);
	}
	cout << "#\tContinuetatement\n";
	cout<<"\tjmp\t"<<continue_label->top()<<endl;
}

void AST_ReturnStatement::encode()
{
	if (this->expression!=NULL)
	{
		this->expression->encode();
		cout << "#\tReturnStatement\n";
		cout<<"\tpopl\t%eax"<<endl;
	}
	cout<<"\tjmp\tMAINEXITLABEL"<<endl;
}

void AST_EmptyStatement::encode()
{
	//do nothing
}

