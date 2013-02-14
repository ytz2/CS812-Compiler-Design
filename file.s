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
	mainvar$j: .long 0
	.text
#	MainVarDeclaration
	.data
	mainvar$k: .long 0
	.text
#	MainVarDeclaration
	.data
	mainvar$l: .long 0
	.text
#	MainVariable
	pushl	$mainvar$i
#	IntegerLiteral
	pushl	$1
#	Assignment
	popl	%eax
	popl	%edx
	movl	%eax, (%edx)
	pushl	%eax
#	ExpressionStatement
	addl $4, %esp
#	MainVariable
	pushl	$mainvar$j
#	IntegerLiteral
	pushl	$2
#	Assignment
	popl	%eax
	popl	%edx
	movl	%eax, (%edx)
	pushl	%eax
#	ExpressionStatement
	addl $4, %esp
#	MainVariable
	pushl	$mainvar$k
#	IntegerLiteral
	pushl	$3
#	Assignment
	popl	%eax
	popl	%edx
	movl	%eax, (%edx)
	pushl	%eax
#	ExpressionStatement
	addl $4, %esp
#	MainVariable
	pushl	$mainvar$l
#	Primary
	pushl	$mainvar$i
#	Deref
	popl	%eax
	movl	(%eax), %eax
	pushl	%eax
#	Primary
	pushl	$mainvar$j
#	Deref
	popl	%eax
	movl	(%eax), %eax
	pushl	%eax
#	Primary
	pushl	$mainvar$k
#	Deref
	popl	%eax
	movl	(%eax), %eax
	pushl	%eax
#	AdditiveExpression
	popl	%edx
	popl	%eax
	addl	%edx, %eax
	pushl	%eax
#	Primary
	pushl	$mainvar$j
#	Deref
	popl	%eax
	movl	(%eax), %eax
	pushl	%eax
#	Primary
	pushl	$mainvar$i
#	Deref
	popl	%eax
	movl	(%eax), %eax
	pushl	%eax
#	AdditiveExpression
	popl	%edx
	popl	%eax
	addl	%edx, %eax
	pushl	%eax
#	RelationalExpression
	popl	%edx
	popl	%eax
	cmpl	%eax,%edx
	setl	%al
	movzbl	%al, %eax
	pushl	%eax
#	Primary
	pushl	$mainvar$k
#	Deref
	popl	%eax
	movl	(%eax), %eax
	pushl	%eax
#	Primary
	pushl	$mainvar$k
#	Deref
	popl	%eax
	movl	(%eax), %eax
	pushl	%eax
#	UnaryExpression
	popl	%eax
	cmpl	$0,%eax
	sete	%al
	movzbl	%al, %eax
	pushl	%eax
#	AdditiveExpression
	popl	%edx
	popl	%eax
	addl	%edx, %eax
	pushl	%eax
#	RelationalExpression
	popl	%edx
	popl	%eax
	cmpl	%eax,%edx
	setg	%al
	movzbl	%al,%eax
	pushl	%eax
#	EqualityExpression
	popl	%eax
	popl	%edx
	cmpl	%eax,%edx
	sete	%al
	movzbl	%al,%eax
	pushl	%eax
#	Assignment
	popl	%eax
	popl	%edx
	movl	%eax, (%edx)
	pushl	%eax
#	ExpressionStatement
	addl $4, %esp
#	MainVariable
	pushl	$mainvar$l
#	Primary
	pushl	$mainvar$l
#	Deref
	popl	%eax
	movl	(%eax), %eax
	pushl	%eax
#	Primary
	pushl	$mainvar$i
#	Deref
	popl	%eax
	movl	(%eax), %eax
	pushl	%eax
#	Primary
	pushl	$mainvar$j
#	Deref
	popl	%eax
	movl	(%eax), %eax
	pushl	%eax
#	EqualityExpression
	popl	%eax
	popl	%edx
	cmpl	%eax,%edx
	sete	%al
	movzbl	%al,%eax
	pushl	%eax
#	AdditiveExpression
	popl	%edx
	popl	%eax
	addl	%edx, %eax
	pushl	%eax
#	Primary
	pushl	$mainvar$k
#	Deref
	popl	%eax
	movl	(%eax), %eax
	pushl	%eax
#	Primary
	pushl	$mainvar$j
#	Deref
	popl	%eax
	movl	(%eax), %eax
	pushl	%eax
#	RelationalExpression
	popl	%edx
	popl	%eax
	cmpl	%eax,%edx
	setl	%al
	movzbl	%al, %eax
	pushl	%eax
#	AdditiveExpression
	popl	%edx
	popl	%eax
	addl	%edx, %eax
	pushl	%eax
#	Primary
	pushl	$mainvar$i
#	Deref
	popl	%eax
	movl	(%eax), %eax
	pushl	%eax
#	Primary
	pushl	$mainvar$k
#	Deref
	popl	%eax
	movl	(%eax), %eax
	pushl	%eax
#	RelationalExpression
	popl	%edx
	popl	%eax
	cmpl	%eax,%edx
	setg	%al
	movzbl	%al,%eax
	pushl	%eax
#	AdditiveExpression
	popl	%edx
	popl	%eax
	addl	%edx, %eax
	pushl	%eax
#	Primary
	pushl	$mainvar$k
#	Deref
	popl	%eax
	movl	(%eax), %eax
	pushl	%eax
#	UnaryExpression
	popl	%eax
	cmpl	$0,%eax
	sete	%al
	movzbl	%al, %eax
	pushl	%eax
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
	pushl	$mainvar$l
#	Primary
	pushl	$mainvar$l
#	Deref
	popl	%eax
	movl	(%eax), %eax
	pushl	%eax
#	Primary
	pushl	$mainvar$i
#	Deref
	popl	%eax
	movl	(%eax), %eax
	pushl	%eax
#	Primary
	pushl	$mainvar$j
#	Deref
	popl	%eax
	movl	(%eax), %eax
	pushl	%eax
#	EqualityExpression
	popl	%eax
	popl	%edx
	cmpl	%eax,%edx
	sete	%al
	movzbl	%al,%eax
	pushl	%eax
#	UnaryExpression
	popl	%eax
	cmpl	$0,%eax
	sete	%al
	movzbl	%al, %eax
	pushl	%eax
#	AdditiveExpression
	popl	%edx
	popl	%eax
	addl	%edx, %eax
	pushl	%eax
#	Primary
	pushl	$mainvar$k
#	Deref
	popl	%eax
	movl	(%eax), %eax
	pushl	%eax
#	Primary
	pushl	$mainvar$j
#	Deref
	popl	%eax
	movl	(%eax), %eax
	pushl	%eax
#	RelationalExpression
	popl	%edx
	popl	%eax
	cmpl	%eax,%edx
	setl	%al
	movzbl	%al, %eax
	pushl	%eax
#	UnaryExpression
	popl	%eax
	cmpl	$0,%eax
	sete	%al
	movzbl	%al, %eax
	pushl	%eax
#	AdditiveExpression
	popl	%edx
	popl	%eax
	addl	%edx, %eax
	pushl	%eax
#	Primary
	pushl	$mainvar$i
#	Deref
	popl	%eax
	movl	(%eax), %eax
	pushl	%eax
#	Primary
	pushl	$mainvar$k
#	Deref
	popl	%eax
	movl	(%eax), %eax
	pushl	%eax
#	RelationalExpression
	popl	%edx
	popl	%eax
	cmpl	%eax,%edx
	setg	%al
	movzbl	%al,%eax
	pushl	%eax
#	UnaryExpression
	popl	%eax
	cmpl	$0,%eax
	sete	%al
	movzbl	%al, %eax
	pushl	%eax
#	AdditiveExpression
	popl	%edx
	popl	%eax
	addl	%edx, %eax
	pushl	%eax
#	Primary
	pushl	$mainvar$k
#	Deref
	popl	%eax
	movl	(%eax), %eax
	pushl	%eax
#	UnaryExpression
	popl	%eax
	cmpl	$0,%eax
	sete	%al
	movzbl	%al, %eax
	pushl	%eax
#	UnaryExpression
	popl	%eax
	cmpl	$0,%eax
	sete	%al
	movzbl	%al, %eax
	pushl	%eax
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
#	MainVarDeclaration
	.data
	mainvar$n: .long 0
	.text
#	MainVariable
	pushl	$mainvar$n
#	Primary
	pushl	$mainvar$l
#	Deref
	popl	%eax
	movl	(%eax), %eax
	pushl	%eax
#	IntegerLiteral
	pushl	$8
#	MultiplicativeExpression
	popl	%eax
	popl	%edx
	imull	%edx, %eax
	pushl	%eax
#	IntegerLiteral
	pushl	$2
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
	pushl	$mainvar$n
#	Deref
	popl	%eax
	movl	(%eax), %eax
	pushl	%eax
#	OutputStatement
	call	RTS_outputInteger
	addl	$4, %esp
#	Epilogue
main$exit:
	popl	%ebp
	ret
