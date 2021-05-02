#pragma once
#include "stack_char.h"
#include <stack>

struct node
{
	int value;
	char operation = '\0';
	int height = 1;
	node* left = nullptr;
	node* right = nullptr;
};

struct tree
{
	node* root = nullptr;
};

void add_to_tree(int value, std::stack<node*>& subtrees);
void add_to_tree(char operation, std::stack<node*>& subtrees);
