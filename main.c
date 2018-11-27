#include <stdio.h>
#include "stack.h"
#include "stack.c"
#include <string.h>
#include <ctype.h>

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

int operatorPrecedence(char currentOperand, Expression possibleOperand )
/**
 *
 * @param currentOperand current operand being evaluated
 * @param possibleOperand operand at stack top
 * @return 1 if the precedence of currentOperand <= possibleOperand, else 0
 */
{
    assert(possibleOperand.currentActive != OPERATOR);
    if(possibleOperand.operator == '^')
    {
        return 1;
    }
    if(possibleOperand.operator == '/' || possibleOperand.operator == '*')
    {
        if(currentOperand != '^')
        {
            return 1;
        }
    }
    if(possibleOperand.operator == '-' || possibleOperand.operator == '+')
    {
        if(currentOperand == '-' || currentOperand == '+')
        {
            return 1;
        }
    }
    return 0;
}


void switchStack(Stack *notSortedP, Stack *sortedP)
/**
 * switch a stack around from back to front
 * @param notSortedP the start stack
 * @param sortedP the finish stack
 */
{
    Expression currentItem;
    while (!isEmptyStack(notSortedP))
    {
        pop(notSortedP, &currentItem);
        push(sortedP, &currentItem);
    }

}


//int postFixCalculator(Stack *sortedP)
//{
//    Expression currentExpression;
//    Stack *calculatorStack = stackAlloc(sizeof(Expression));
//    while (!isEmptyStack(sortedP))
//    {
//        pop()
//    }
//}

int convertToInt(const char currentLine[], int i, int size, int *value)
/**
 * convert the next letters to their int value,
 * @param currentLine the current buffer
 * @param i the index to start
 * @param size buffersize
 * @param value pointer to the value we are summing
 * @return the new index
 */
{
    int k;
    for(k = i; k < size; k++)
    {
        if (isdigit(currentLine[k]))
        {
            *value = *value*10 + currentLine[k] - '0';
        }
        else
        {
            return k;
        }
    }
    return k;
}

int infixToPostFix(const char currentLine[], int size)
{
    int i;
    Stack *qStack = stackAlloc(sizeof(Expression));
    Stack *pStack = stackAlloc(sizeof(Expression));
    Stack *infixStack = stackAlloc(sizeof(Expression));
    for(i = 0; i<size; i++)
    {

        if(isdigit(currentLine[i]))  //found operand (number)
        {
            int value = 0;
            int *valuePointer = &value;
            i = convertToInt(currentLine, i, size, valuePointer);
            Expression operand;
            operand.currentActive = NUMBER;
            operand.number = value;
//            int a = isEmptyStack(pStack);
//            printf("%d\n", a);
            push(pStack, &operand);
//            a = isEmptyStack(pStack);
//            printf("%d\n", a);
            push(infixStack, &operand);
            continue;
        }
        if(currentLine[i] == '(')  // left parenthesis
        {
            Expression leftParenthesis;
            leftParenthesis.currentActive = OPEN_PARENTHESES;
            leftParenthesis.openParentheses = currentLine[i];
            push(qStack, &leftParenthesis);
            push(infixStack, &leftParenthesis);
            continue;
        }
        if(currentLine[i] == ')')  // right parenthesis
        {
            Expression close;
            close.currentActive = CLOSE_PARENTHESES;
            close.closeParentheses = currentLine[i];
            push(infixStack, &close);
            Expression popped;
            while(!isEmptyStack(qStack))  // push all items aside from ( to pStack
            {
                pop(qStack, &popped);
                if(popped.currentActive != OPEN_PARENTHESES)
                {
                    push(pStack, &popped);
                }
                else  // is open parenthesis
                {
                    break;
                }
            }
            continue;
        }


        if(currentLine[i] == '^' || currentLine[i] ==  '/' || currentLine[i] ==  '*'  ||
            currentLine[i] ==  '-' || currentLine[i] == '+') // if operator
        {
            Expression operatorPush;
            if(isEmptyStack(qStack))
            {
                operatorPush.currentActive = OPERATOR;
                operatorPush.operator = currentLine[i];
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
                    operatorPush.operator = currentLine[i];
                    push(qStack, &operatorPush);
                }
                else
                {
                    while(!isEmptyStack(qStack))
                    {
                        Expression currentTop;
                        pop(qStack, &currentTop);
                        if(currentTop.currentActive != OPEN_PARENTHESES && operatorPrecedence
                        (currentLine[i], currentTop))
                        {
                            push(pStack, &currentTop);
                        }
                        else
                        {
                            push(qStack, &currentTop);
                            break;
                        }
                    }
                    operatorPush.currentActive = OPERATOR;
                    operatorPush.operator = currentLine[i];
                    push(qStack, &operatorPush);
                    push(infixStack, &operatorPush);
                }
            }
        }
    }
    while(!isEmptyStack(qStack))
    {
        Expression expressionToPop;
        pop(qStack, &expressionToPop);
        push(pStack, &expressionToPop);
    }
    freeStack(&qStack);
    Stack *sortedP = stackAlloc(sizeof(Expression));;
    Stack *sortedInfix = stackAlloc(sizeof(Expression));;
    switchStack(pStack, sortedP);
    switchStack(infixStack, sortedInfix);
    freeStack(&pStack);
    freeStack(&infixStack);
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