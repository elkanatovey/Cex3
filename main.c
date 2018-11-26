#include <stdio.h>
#include "stack.h"
#include "stack.c"
#include <string.h>

#define MAX_LINE_LENGTH 101
#define OPERATORS = ('^' | '/' | '*'  | '-' | '+')
#define OPEN_PARENTHESES 1
#define CLOSE_PARENTHESES 2
#define OPERATOR 3
#define NUMBER 4

/**
 * struct to hold the sequences as pointers
 */
typedef struct Expression
{
    int currentActive;
    union
    {
        char openParentheses;  //flag 1
        char closeParentheses;  //flag 2
        char operator;  //flag 3
        int number;  //flag 4
    };
}Expression;


int postFixCalculator(Stack *sortedP)
{
    Expression currentExpression;
    Stack *calculatorStack = stackAlloc(sizeof(Expression));
    while (!isEmptyStack(sortedP))
    {
        pop()
    }
}


int infixToPostFix(char currentLine[], int size)
{
    int i;
    Stack *qStack = stackAlloc(sizeof(Expression));
    Stack *pStack = stackAlloc(sizeof(Expression));
    for(i = 0; i<size; i++)
    {

        if(currentLine[i] == )  //found operand (number)
        {
            Expression operand;
            operand.currentActive = NUMBER;
            operand.number = currentLine[i];
            push(pStack, &operand);
            continue;
        }
        if(currentLine[i] == '(')  // left parenthesis
        {
            Expression leftParenthesis;
            leftParenthesis.currentActive = OPEN_PARENTHESES;
            leftParenthesis.openParentheses = currentLine[i];
            push(qStack, &leftParenthesis);
            continue;
        }
        if(currentLine[i] == ')')  // right parenthesis
        {
            Expression popped;
            while((!isEmptyStack(qStack)))  // push all items aside from ( to pStack
            {
                pop(qStack, &popped);
                if(popped.currentActive != OPEN_PARENTHESES)
                {
                    push(pStack, &popped);
                }
            }
            continue;
        }


        if(currentLine[i] == ('^' | '/' | '*'  | '-' | '+')) // if operator
        {
            Expression operatorPush;
            if(isEmptyStack(qStack))
            {
                operatorPush.currentActive = OPERATOR;
                operatorPush.openParentheses = currentLine[i];
                push(qStack, &operatorPush);
            }
            else
            {
                Expression operatorPop;
                pop(qStack, &operatorPop);
                push(qStack, &operatorPop);  // instead of peek
                if (operatorPop.currentActive == OPEN_PARENTHESES)
                {
                    operatorPush.currentActive = OPERATOR;
                    operatorPush.openParentheses = currentLine[i];
                    push(qStack, &operatorPush);
                }
                else
                {
                    while(isEmptyStack(qStack))
                    {
                        Expression currentTop;
                        pop(qStack, &currentTop);
                        if(currentTop.currentActive != OPEN_PARENTHESES && )
                        {
                            push(pStack, &currentTop);
                        }
                        else
                        {
                            push(qStack, &currentTop);
                            break;
                        }
                    }
                    Expression newOperator;
                    newOperator.currentActive = OPERATOR;
                    newOperator.operator = currentLine[i];
                }
            }
        }
    }
    while(isEmptyStack(qStack))
    {
        Expression expressionToPop;
        pop(qStack, &expressionToPop);
        push(pStack, &expressionToPop);
    }
    freeStack(&qStack);
}



int main()
{
    char inputBuffer[MAX_LINE_LENGTH] = {0};
    while(*fgets(inputBuffer, MAX_LINE_LENGTH, stdin) != EOF)
    {
        if(inputBuffer[0] == '\n')
        {
            printf("Infix:\nPostfix:\nCan't evaluate expression");
            break;
        }
        inputBuffer[strcspn(inputBuffer, "\r\n")] = 0;
        infixToPostFix(inputBuffer, (int)strnlen(inputBuffer, MAX_LINE_LENGTH));
    }

}





//int i, num = 10;
//printf("Generating list with %d ints\n", num);
//Stack *stack = stackAlloc(sizeof(int));
//for(i = 1; i <= num; i++) {
//push(stack,&i);
//}
//for(i = 1; i <= num-2; i++) {
//int headData;
//pop(stack,&headData);
//printf("top value is: %d\n",headData);
//}
//freeStack(&stack);
//return 0;



//int i;
//Stack *operandStack = stackAlloc(sizeof(Expression));
//for(i = 0; i<size; i++)
//{
//if(currentLine[i] == '(')  // left parenthesis
//{
//Expression leftParenthesis;
//leftParenthesis.currentActive = 1;
//leftParenthesis.openParentheses = currentLine[i];
//push(operandStack, &leftParenthesis);
//continue;
//}
//if(currentLine[i] == ')')  // right parenthasis
//{
//Expression popped;
//while(!isEmptyStack(operandStack) & (peek(operandStack, &popped))  )
//{
//pop(operandStack, &popped);
//}
//}
//
//
//if(currentLine[i] == ()) // if operator
//{
//
//}
//if
//}