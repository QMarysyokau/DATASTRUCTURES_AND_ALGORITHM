/*
RPN Calculator (Reverse Polish Notation)
An implementation of  program that reads an infix expression from the standard input device (Keyboard).
 The program should then convert the expression into a postfix form, and output it on the standard output
device (screen). The program should support the following operators: \,/,*,+,%,(,) and other operators.
By Mary Syokau: SCT221-0237/2022
----------------------------------------------------------------------------------------------**/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

struct Stack {
    int top;
    unsigned capacity;
    int* array;
};

int ConvertToPostfix(char* exp); // Declaration

int main() {
    char exp[100];
    printf("Enter an infix expression: ");
    scanf("%99s", exp);
    printf("Expression:\n%s\n", exp);
    ConvertToPostfix(exp);
    printf("Result: %d\n", evaluatePostfixExpression(exp));
    return 0;
}

struct Stack* createStack(unsigned capacity) {
    struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack));
    if (!stack)
        return NULL;
    stack->top = -1;
    stack->capacity = capacity;
    stack->array = (int*)malloc(stack->capacity * sizeof(int));
    return stack;
}

int isEmpty(struct Stack* stack) {
    return stack->top == -1;
}

char peek(struct Stack* stack) {
    return stack->array[stack->top];
}

char pop(struct Stack* stack) {
    if (!isEmpty(stack))
        return stack->array[stack->top--];
    return '$';
}

void push(struct Stack* stack, char op) {
    stack->array[++stack->top] = op;
}

int isOperand(char ch) {
    return isdigit(ch);
}

int DeterminePrecision(char ch) {
    switch (ch) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        case '%':
            return 3;
    }
    return -1;
}

int ConvertToPostfix(char* exp) {
    int i, j;
    struct Stack* stack = createStack(strlen(exp));
    if (!stack)
        return -1;

    for (i = 0, j = -1; exp[i]; ++i) {
        if (isOperand(exp[i]))
            exp[++j] = exp[i];
        else if (exp[i] == '(')
            push(stack, exp[i]);
        else if (exp[i] == ')') {
            while (!isEmpty(stack) && peek(stack) != '(')
                exp[++j] = pop(stack);
            if (!isEmpty(stack) && peek(stack) != '(')
                return -1;
            else
                pop(stack);
        } else {
            while (!isEmpty(stack) && DeterminePrecision(exp[i]) <= DeterminePrecision(peek(stack)))
                exp[++j] = pop(stack);
            push(stack, exp[i]);
        }
    }

    while (!isEmpty(stack))
        exp[++j] = pop(stack);
    exp[++j] = '\0';

    printf("Reverse Polish Notation:\n");
    for (int i = 0; exp[i]; i++) {
        printf("%c ", exp[i]);
    }
    printf("\n");
}

int evaluatePostfixExpression(char* exp) {
    struct Stack* stack = createStack(strlen(exp));
    int i;
    if (!stack)
        return -1;

    for (i = 0; exp[i]; ++i) {
        if (isdigit(exp[i]))
            push(stack, exp[i] - '0');
        else {
            int val1 = pop(stack);
            int val2 = pop(stack);
            switch (exp[i]) {
                case '+':
                    push(stack, val2 + val1);
                    break;
                case '-':
                    push(stack, val2 - val1);
                    break;
                case '*':
                    push(stack, val2 * val1);
                    break;
                case '/':
                    push(stack, val2 / val1);
                    break;
                case '%':
                    push(stack, val2 % val1);
                    break;
            }
        }
    }
    return pop(stack);
}
