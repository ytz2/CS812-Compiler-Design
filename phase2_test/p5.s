#	Prologue
	.text
	.align	4
	.globl	main
main:
	pushl	%ebp
	movl	%esp, %ebp
#	MainVarDeclaration
	.data
	mainvar$i: .long 0
	.text
#	MainVarDeclaration
	.data
	mainvar$value: .long 0
	.text
#	MainVariable
	pushl	$mainvar$i
#	IntegerLiteral
	pushl	$0
#	Assignment
	popl	%eax
	popl	%edx
	movl	%eax, (%edx)
	pushl	%eax
#	ExpressionStatement
	addl $4, %esp
#	MainVariable
	pushl	$mainvar$value
#	IntegerLiteral
	pushl	$1
#	Assignment
	popl	%eax
	popl	%edx
	movl	%eax, (%edx)
	pushl	%eax
#	ExpressionStatement
	addl $4, %esp
#	WhileStatement
CONTINUELABEL0:
#	IntegerLiteral
	pushl	$1
	popl	%eax
	cmpl	$0,	%eax
	je	BREAKLABEL0
#	Primary
	pushl	$mainvar$i
#	Deref
	popl	%eax
	movl	(%eax), %eax
	pushl	%eax
#	OutputStatement
	call	RTS_outputInteger
	addl	$4, %esp
#	Primary
	pushl	$mainvar$value
#	Deref
	popl	%eax
	movl	(%eax), %eax
	pushl	%eax
#	OutputStatement
	call	RTS_outputInteger
	addl	$4, %esp
#	MainVariable
	pushl	$mainvar$i
#	Primary
	pushl	$mainvar$i
#	Deref
	popl	%eax
	movl	(%eax), %eax
	pushl	%eax
#	IntegerLiteral
	pushl	$1
#	AdditiveExpression
	popl	%edx
	popl	%eax
	addl	%edx, %eax
	pushl	%eax
#	Assignment
	popl	%eax
	popl	%edx
	movl	%eax, (%edx)
	pushl	%eax
#	ExpressionStatement
	addl $4, %esp
#	Primary
	pushl	$mainvar$i
#	Deref
	popl	%eax
	movl	(%eax), %eax
	pushl	%eax
#	IntegerLiteral
	pushl	$13
#	EqualityExpression
	popl	%eax
	popl	%edx
	cmpl	%eax,%edx
	sete	%al
	movzbl	%al,%eax
	pushl	%eax
#	IfThenElseStatement
	popl	%eax
	cmpl	$0,	%eax
	je	LABEL1
#	Breaktatement
	jmp	BREAKLABEL0
	jmp	LABEL2
LABEL1:
