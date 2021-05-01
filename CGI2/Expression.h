#pragma once
struct elem
{
	char x;
	elem* next = nullptr;
	int index;
};
bool process_exp(const char* expression, int*& error, elem*& top);

