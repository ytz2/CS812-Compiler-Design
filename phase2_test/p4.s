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
#	Primary
	pushl	$mainvar$i
#	Deref
	popl	%eax
	movl	(%eax), %eax
	pushl	%eax
#	IntegerLiteral
	pushl	$13
#	RelationalExpression
	popl	%edx
	popl	%eax
	cmpl	%eax,%edx
	setg	%al
	movzbl	%al,%eax
	pushl	%eax
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
#	MainVariable
	pushl	$mainvar$value
#	Primary
	pushl	$mainvar$i
#	Deref
	popl	%eax
	movl	(%eax), %eax
	pushl	%eax
#	Primary
	pushl	$mainvar$value
#	Deref
	popl	%eax
	movl	(%eax), %eax
	pushl	%eax
#	MultiplicativeExpression
	popl	%eax
	popl	%edx
	imull	%edx, %eax
	pushl	%eax
#	Assignment
	popl	%eax
	popl	%edx
	movl	%eax, (%edx)
	pushl	%eax
#	ExpressionStatement
	addl $4, %esp
	jmp	CONTINUELABEL0
BREAKLABEL0:
MAINEXITLABEL:
#	Epilogue
main$exit:
	popl	%ebp
	ret
