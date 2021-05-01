#include "tree_struct.h"
#include <stack>

void add_to_tree(int value, std::stack<node>& subtrees)
{
	node* new_sheet = new node;
	new_sheet->value = value;
	subtrees.push(*new_sheet);
}

void add_to_tree(char operation, std::stack<node>& subtrees) {

	node* right = &subtrees.top(); subtrees.pop();
	node* left = &subtrees.top(); subtrees.pop();
	node* subtree = new node; subtree->operation = operation;
	subtree->height = (right->height >= left->height) ? right->height : left->height;
	subtree->left = left;
	subtree->right = right;
	subtrees.push(*subtree);
}
