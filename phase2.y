%{
  // additional productions from the T grammar required for Phase 2
%}

%token IF ELSE WHILE RETURN
%token BREAK CONTINUE

%%

Block
	: '{' BlockStatements '}'
	| '{' '}'
	;

BlockStatements
	: BlockStatements BlockStatement
	| BlockStatement
	;

BlockStatement
	: Statement
	;

Statement
	: Block
	| EmptyStatement
	| ExpressionStatement // $$ = $1
	| IfThenElseStatement
	| WhileStatement
	| ReturnStatement
	| OutputStatement // $$ = $1
	| BreakStatement
	| ContinueStatement
	;

IfThenElseStatement
	: IF ParenExpression Statement ELSE Statement
	;

WhileStatement
	: WHILE ParenExpression Statement
	;

ReturnStatement
	: RETURN ';'
	| RETURN Expression ';'
	;

BreakStatement
	: BREAK ';'
	;

ContinueStatement
	: CONTINUE ';'
	;

EmptyStatement
	: ';'
	;

%%
