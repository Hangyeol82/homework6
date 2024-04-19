/** 
 * postfix.c
 *
 * School of Computer Science, 
 * Chungbuk National University
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_STACK_SIZE 100
#define MAX_EXPRESSION_SIZE 100

/* stack 내에서 우선순위, lparen = 0 가장 낮음 */
typedef enum{
	lparen = 0,  /* ( 왼쪽 괄호 */
	rparen = 9,  /* ) 오른쪽 괄호*/
	times = 8,   /* * 곱셈 */					// 곱셈과 나눗셈의 우선순위는 같다
	divide = 7,  /* / 나눗셈 */
	plus = 6,    /* + 덧셈 */					// 덧셈과 뺄셈의 우선순위는 같다
	minus = 5,   /* - 뺄셈 */
	operand = 1 /* 피연산자 */
} precedence;

char infixExp[MAX_EXPRESSION_SIZE];   	/* infix expression을 저장하는 배열 */
char postfixExp[MAX_EXPRESSION_SIZE];	/* postfix로 변경된 문자열을 저장하는 배열 */
char postfixStack[MAX_STACK_SIZE] = {'0',};	/* postfix로 변환을 위해 필요한 스택 */
int evalStack[MAX_STACK_SIZE];		/* 계산을 위해 필요한 스택 */

int postfixStackTop = -1;  /* postfixStack용 top */
int evalStackTop = -1;	   /* evalStack용 top */

int evalResult = 0;	   /* 계산 결과를 저장 */

void postfixPush(char x);
char postfixPop();
void evalPush(int x);
int evalPop();
void getInfix();
precedence getToken(char symbol);
precedence getPriority(char x);
void charCat(char* c);
void toPostfix();
void debug();
void reset();
void evaluation();

int main()
{
	char command;

	do{
		printf("-----------------이한결--------------2021041055------------------\n");
		printf("               Infix to Postfix, then Evaluation               \n");
		printf("----------------------------------------------------------------\n");
		printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'i': case 'I':
			getInfix();
			break;
		case 'p': case 'P':
			toPostfix();
			break;
		case 'e': case 'E':
			evaluation();
			break;
		case 'd': case 'D':
			debug();
			break;
		case 'r': case 'R':
			reset();
			break;
		case 'q': case 'Q':
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}

void postfixPush(char x)
{
    postfixStack[++postfixStackTop] = x;
}

char postfixPop()
{
    char x;
    if(postfixStackTop == -1)
        return '\0';
    else {
    	x = postfixStack[postfixStackTop--];
    }
    return x;
}

void evalPush(int x)
{
    evalStack[++evalStackTop] = x;
}

int evalPop()
{
    if(evalStackTop == -1)
        return -1;
    else
        return evalStack[evalStackTop--];
}

/**
 * infix expression을 입력받는다.
 * infixExp에는 입력된 값을 저장한다.
 */
void getInfix()
{
    printf("Type the expression >>> ");
    scanf("%s",infixExp);
}

precedence getToken(char symbol)
{
	switch(symbol) {
	case '(' : return lparen;
	case ')' : return rparen;
	case '+' : return plus;
	case '-' : return minus;
	case '/' : return divide;
	case '*' : return times;
	default : return operand;
	}
}

precedence getPriority(char x)
{
	return getToken(x);
}

/**
 * 문자하나를 전달받아, postfixExp에 추가
 */
void charCat(char* c)
{
	if (postfixExp == '\0')
		strncpy(postfixExp, c, 1);
	else
		strncat(postfixExp, c, 1);
}

/**
 * infixExp의 문자를 하나씩 읽어가면서 stack을 이용하여 postfix로 변경한다.
 * 변경된 postfix는 postFixExp에 저장된다.
 */
void toPostfix()
{
	/* infixExp의 문자 하나씩을 읽기위한 포인터 */
	char *exp = infixExp;
	char x; /* 문자하나를 임시로 저장하기 위한 변수 */
	int num = 0;
	/* exp를 증가시켜가면서, 문자를 읽고 postfix로 변경 */
	while(*exp != '\0')
	{
		x = getToken(*exp);
		if (x == 0) x = 10; 				// '(' 연산자가 stack에 쌓일때는 우선순위가 제일 높음
		if(x == operand){
			charCat(exp);
			num++;
		}
		else if(x == rparen){
			while(getToken(postfixStack[postfixStackTop]) != lparen){		// x가 ')' 연산자이면 (' 연산자를 만나기 전까지 모두 pop 처리
				postfixExp[num] = postfixPop();
				num++;
			}
			postfixPop();												// '(' 연산자를 pop
		}
		else{
			if(postfixStack[0] == '0') postfixPush(*exp);			// stack이 비어있을때 스텍에 넣음
			else{
				if(getToken(postfixStack[postfixStackTop]) < x){		// stack의 마지막보다 우선순위가 높으면 stack에 쌓임
					postfixPush(*exp);
				}
				else{
					while(getToken(postfixStack[postfixStackTop]) >= x){		// x가 stack의 마지막보다 우선순위가 낮으면 stack의 변수를 pop
						postfixExp[num] = postfixPop();
						num++;							
					}
					postfixPush(*exp);											// x보다 우선순위가 낮은 변수를 모두 pop 시킨 후에 x를 push
				}
			}
		}
		exp++;
	}
	while(postfixStackTop != -1){					// stack에 남아있는 연산자를 모두 pop
		postfixExp[num] = postfixPop();
		num++;
	}
	/* 필요한 로직 완성 */

}
void debug()
{
	printf("\n---DEBUG\n");
	printf("infixExp =  %s\n", infixExp);
	printf("postExp =  %s\n", postfixExp);
	printf("eval result = %d\n", evalResult);

	printf("postfixStack : ");
	for(int i = 0; i < MAX_STACK_SIZE; i++)
		printf("%c  ", postfixStack[i]);

	printf("\n");

}

void reset()
{
	infixExp[0] = '\0';
	postfixExp[0] = '\0';

	for(int i = 0; i < MAX_STACK_SIZE; i++)
		postfixStack[i] = '\0';
         
	postfixStackTop = -1;
	evalStackTop = -1;
	evalResult = 0;
}

void evaluation()
{
    /* postfixExp, evalStack을 이용한 계산 */
    char *exp = postfixExp;							// exp는 postfixExp에 하나씩 접근하기 위해 선언
    if (evalResult == 0) evalResult = *exp - '0';	// evalResult의 값에 계산을 할건데 처음에는 evalResult의 값을 초기화해야함
    exp++;
    precedence a;
    while(*exp != '\0' && (a = getToken(*exp))){
        if (a == 1) evalPush(*exp - '0');			// 피연산자는 스택에 Push
        else{
            switch (a){
                case times:
                    evalResult *= evalPop();		// 곱하기 연산자 계산
                    break;
                case divide:
                    evalResult /= evalPop();		// 나누기 연산자 계산
                    break;
                case plus:
                    evalResult += evalPop();		// 더하기 연산자 계산
                    break;
                case minus:
                    evalResult -= evalPop();		// 빼기 연산자 계산
                    break;
            }
        }
        exp++;
    }
}