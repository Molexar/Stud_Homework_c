#pragma once

struct stack_char {
	char value;
	stack_char* next = nullptr;
};

void push(stack_char*& stack_char, char x);

bool pop(stack_char*& stack_char, char& x);

bool peek(const stack_char* stack_char, char& x);

void clear(stack_char*& stack_char);
