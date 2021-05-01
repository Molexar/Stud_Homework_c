#pragma once

#include "stack_char.h"
#include "Calculator.h"
#include <string.h>

bool is_operation(const char ch) {
    return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^';
}

bool is_operand(const char ch) {
    return !(ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^' || ch == '(' || ch == ')');
}

int get_priority(const char ch) {
    if (ch == '(') { return 0; }
    if (ch == '+' || ch == '-') { return 1; }
    if (ch == '*' || ch == '/' || ch == '~') { return 2; }
    if (ch == '^') { return 3; }
    return 0;
}

bool check_brackets(const char* expression) {
    int* errors = new int[2]{ -1, -1 };
    elem* top = nullptr;
    bool is_right = process_exp(expression, errors, top);
    clear(top);
    return is_right;
}

void pop_to_br(stack_char*& top, char*& postfix, int& index_post) {
    char temp; peek(top, temp);
    while (temp != '(') {
        pop(top, temp);
        postfix[index_post++] = ' ';
        postfix[index_post++] = temp;
        peek(top, temp);
    }
    pop(top, temp);
}

bool is_unary(const char* exp, int index) {
    if (!is_operand(exp[index - 1]) || exp[index - 1] == '(' || (index == 0 && !is_operand(exp[index]))) {
        return true;
    }
    return false;
}

void translate_exp(const char* expression, char*& postfix) {
	stack_char* top = nullptr;
	int index_post = 0;
	for (int i = 0; i < strlen(expression); i++) {
		if (is_operand(expression[i])) {
			postfix[index_post++] = expression[i];
		}
		else {
			postfix[index_post++] = ' ';
			char temp; peek(top, temp);
			if (expression[i] == '-' && is_unary(expression, i)) {
				push(top, '~');
			}
			else if (!top) {
				push(top, expression[i]);
			}
			else if (is_operation(expression[i]) && (get_priority(expression[i]) > get_priority(temp))) {
				push(top, expression[i]);
			}
			else if (is_operation(expression[i]) && get_priority(expression[i]) <= get_priority(temp)) {

				while (get_priority(expression[i]) <= get_priority(temp) && top) {
					pop(top, temp);
					postfix[index_post++] = ' ';
					postfix[index_post++] = temp;
					peek(top, temp);
				}
				postfix[index_post++] = ' ';
				push(top, expression[i]);
			}
			else if (expression[i] == '(') {
				push(top, expression[i]);
			}
			else if (expression[i] == ')') {
				while (top && temp != '(') {
					postfix[index_post++] = ' ';
					postfix[index_post++] = temp;
					pop(top, temp);
					peek(top, temp);
				}
				pop(top, temp);
			}
		}
	}
	while (top) {
		char temp; pop(top, temp);
		postfix[index_post++] = ' ';
		postfix[index_post++] = temp;
	}
	postfix[index_post] = '\0';
}