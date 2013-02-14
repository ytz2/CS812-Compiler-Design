#include <stdio.h>
#include <stdlib.h>
void RTS_outputInteger(int i) {
	printf("%d\n", i);
}

void RTS_printDivideByZeroError(int line) {
	printf("devided by zero at line %d \n", line);
	exit(0);
}

