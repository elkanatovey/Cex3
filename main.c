#include <stdio.h>
#include "stack.h"
#include "stack.c"
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_LINE_LENGTH 101
#define OPERATORS = ('^' | '/' | '*'  | '-' | '+')
#define OPEN_PARENTHESES 1
#define CLOSE_PARENTHESES 2
#define OPERATOR 3
#define NUMBER 4
#define DIVISION_ERROR "Division by 0!"
#define SUM_MESSAGE "The value is %d\n"


/**
 * struct to hold the expressions
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


int calculateMath(int a, int b, char operator)
/**
 * performs calcualtions between two operands, with a given operator
 * @param a operand 1
 * @param b operand 2
 * @param operator given operator
 * @return the result
 */
{
    int result = 0;
    if(operator == '/')
    {
        if(a == 0)
        {
            fprintf(stderr, DIVISION_ERROR);
            exit(1);
        }
        result = b / a;
    }
    if(operator == '*')
    {
        result = b*a;

    }
    if(operator == '-')
    {
        result = b - a;
    }
    if(operator == '+')
    {
        result = b + a;
    }
    if(operator == '^')
    {
        result = (int)pow(b, a);
    }
    return result;
}

void postFixCalculator(Stack *sortedP)
/**
 * postfix algorithm for calculations
 * @param sortedP sorted postfix
 */
{
//    Expression a;
//    while (!isEmptyStack(sortedP))
//    {
//        pop(sortedP, &a);
//        if(a.currentActive == NUMBER)
//        {
//            printf("%d\n", a.number);
//        } else
//        {
//            printf("%c\n", a.operator);
//        }
//    }
    Expression currentExpression;
    Expression aExpression;
    Expression bExpression;
    Expression sum;
    Stack *calculatorStack = stackAlloc(sizeof(Expression));
    while (!isEmptyStack(sortedP))
    {
        pop(sortedP, &currentExpression);
        if(currentExpression.currentActive == NUMBER)
        {
            push(calculatorStack, &currentExpression);
        }
        else
        {
            pop(calculatorStack, &aExpression);
            pop(calculatorStack, &bExpression);
            sum.currentActive = NUMBER;
            sum.number = calculateMath(aExpression.number, bExpression.number,
                    currentExpression.operator);
            push(calculatorStack, &sum);
        }
    }
    pop(calculatorStack, &sum);
    printf(SUM_MESSAGE, sum.number);
    freeStack(&calculatorStack);
    freeStack(&sortedP);
}

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
            return k - 1;
        }
    }
    return k;
}

void isDigitCase(Expression operand, int value, Stack *pStack, Stack *infixStack)
/**
 * helper function to initialize variables
 * @param operand current operand
 * @param value value to put
 * @param pStack stack to push to
 * @param infixStack the normally ordered stack
 */
{
    operand.currentActive = NUMBER;
    operand.number = value;
    push(pStack, &operand);
    push(infixStack, &operand);
}

void isLeftParenthesisCase(Expression leftParenthesis, Stack *qStack, Stack *infixStack, int i,
                            const char currentLine[] )
/**
 * helper function for left parenthesis case
 * @param leftParenthesis duh..
 * @param qStack q from algorithm
 * @param infixStack he normally ordered stack
 * @param i current buffer index
 * @param currentLine buffer
 */
{
    leftParenthesis.currentActive = OPEN_PARENTHESES;
    leftParenthesis.openParentheses = currentLine[i];
    push(qStack, &leftParenthesis);
    push(infixStack, &leftParenthesis);
}

void isRightParenthesisCase(Expression close, Expression popped, const char currentLine[], int i,
                            Stack *qStack, Stack *pStack, Stack *infixStack)
/**
 * helper function for right parentheses
 * @param close expression pointer
 * @param popped expression pointer
 * @param currentLine buffer
 * @param i buffer index
 * @param qStack same as algorithm
 * @param pStack see previous
 * @param infixStack for infix purposes
 */
{
    close.currentActive = CLOSE_PARENTHESES;
    close.closeParentheses = currentLine[i];
    push(infixStack, &close);
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
}

void operatorHelper(Expression operatorPop, Expression operatorPush, Expression currentTop, Stack
                    *qStack, Stack *pStack, Stack *infixStack, const char currentLine[], int i)
/**
 * helper function for operator
 * @param operatorPop expression pointer
 * @param operatorPush expression pointer
 * @param currentTop self explanatory
 * @param qStack same as algorithm
 * @param pStack see previous
 * @param infixStack for infix purposes
 * @param currentLine buffer
 * @param i buffer index
 */
{
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

void mainOperatorHelper(Expression operatorPop, Expression operatorPush, Expression currentTop,
                         Stack *qStack, Stack *pStack, Stack *infixStack,
                         const char currentLine[], int i)
/**
 * helper function for operator
 * @param operatorPop expression pointer
 * @param operatorPush expression pointer
 * @param currentTop self explanatory
 * @param qStack same as algorithm
 * @param pStack see previous
 * @param infixStack for infix purposes
 * @param currentLine buffer
 * @param i buffer index
 */
{
    if(isEmptyStack(qStack))
    {
        operatorPush.currentActive = OPERATOR;
        operatorPush.operator = currentLine[i];
        push(qStack, &operatorPush);
    }
    else
    {
        operatorHelper(operatorPop, operatorPush, currentTop, qStack, pStack, infixStack,
                       currentLine, i);
    }
}


void prepareCalculations(Stack *qStack, Stack *pStack, Stack *infixStack)
/**
 * helper function for calculations
 * @param qStack qStack same as algorithm
 * @param pStack Stack see previous
 * @param infixStack original infix stack
 * @param expressionToPop expression pointer
 */
{
    Expression expressionToPop = {};
    Stack *sortedP = stackAlloc(sizeof(Expression));
    Stack *sortedInfix = stackAlloc(sizeof(Expression));
    while(!isEmptyStack(qStack))
    {
        pop(qStack, &expressionToPop);
        push(pStack, &expressionToPop);
    }
    freeStack(&qStack);
    switchStack(pStack, sortedP);
    switchStack(infixStack, sortedInfix);
    freeStack(&pStack);
    freeStack(&infixStack);
    postFixCalculator(sortedP);
}

void infixToPostFix(const char currentLine[], int size)
/**
 * postfix algorithm
 * @param currentLine the buffer
 * @param size buffersize
 */
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
            Expression operand = {};
            isDigitCase(operand, value, pStack, infixStack);
            continue;
        }
        if(currentLine[i] == '(')  // left parenthesis
        {
            Expression leftParenthesis = {};
            isLeftParenthesisCase(leftParenthesis, qStack, infixStack, i, currentLine);
            continue;
        }
        if(currentLine[i] == ')')  // right parenthesis
        {
            Expression close = {};
            Expression popped = {};
            isRightParenthesisCase(close, popped, currentLine, i, qStack, pStack, infixStack);
            continue;
        }
        if(currentLine[i] == '^' || currentLine[i] ==  '/' || currentLine[i] ==  '*'  ||
            currentLine[i] ==  '-' || currentLine[i] == '+') // if operator
        {
            Expression operatorPush = {};
            Expression operatorPop = {};
            Expression currentTop = {};
            mainOperatorHelper(operatorPop, operatorPush, currentTop, qStack, pStack,
                               infixStack, currentLine, i);
        }
    }
    prepareCalculations(qStack, pStack, infixStack);
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