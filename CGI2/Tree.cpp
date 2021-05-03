#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <sstream>
#include "cgi.h"
#include "Calculator.h"
#include "tree_struct.h"
#include "Tree.h"
#include <stack>
#include <queue>

using namespace std;

const char* header = "Tree";

void show_form() {
	cout << "<form action='tree.cgi' method='post'>"
		<< "<p>Выберите формат входного выражения :</p>"
		<< "<p><input name='type' type='radio' value='post'>Постфиксная</p>"
		<< "<p><input name='type' type='radio' value='in'>Инфиксная</p>"
		<< "<p><input name='exp' type='text'</p>"
		<< "<input type='submit'>"
		<< "</form>";
}

void show_tree(tree Tree) {
	cout << "<table>";
	for (int i = 1; i <= Tree.root->height; i++) {
		
	}
	cout << "</table>";
}

void DFS_LRR(node* root, queue<node*>& que);
void DFS_RRL(node* root, queue<node*>& que);

void get_que(queue<node*>& que, char*& str, bool tg_infix) {
	int i = 0;
	str = new char[1]{ '\0' };
	while (que.size() > 0) {
		node* curr = que.front();
		if (!curr) break;
		if (curr->operation == '\0') {
			char* num = new char[11];
			sprintf(num, "%d", curr->value);
			strcat(str, num);
			strcat(str, " ");
		}
		else {
			char* temp = new char[2];
			temp[0] = curr->operation; temp[1] = '\0';
			strcat(str, temp);
			if (curr->operation != '(' && curr->operation != ')')
			strcat(str, " ");
		}
		que.pop();
	}
	
}

void get_infix(tree& Tree, char*& infix) {
	queue<node*> que;
	DFS_LRR(Tree.root, que);
	get_que(que, infix, true);
}

void get_postfix(tree Tree, char*& postfix) {
	queue<node*> que;
	DFS_RRL(Tree.root, que);
	get_que(que, postfix, false);
}

void DFS_RRL(node* root, queue<node*>& que) {
	if (root) {
		que.push(root);
		DFS_RRL(root->right, que);
		DFS_RRL(root->left, que);
	}
}

void DFS_LRR(node* root, queue<node*>& que) {
	if (root) {
		if (root->operation != '\0')
		{
			node* temp = new node; temp->operation = '(';
			que.push(temp);
		}
		DFS_LRR(root->left, que);
		que.push(root);
		DFS_LRR(root->right, que);
		if (root->operation != '\0')
		{
			node* temp = new node; temp->operation = ')';
			que.push(temp);
		}
	}
}

void process_exp(char* data) {
	if (!is_get()) {
#ifndef DEBUG
		delete[] data;
		int buf_size = get_content_length();
		data = new char[buf_size + 1];
		fread_s(data, buf_size + 1, sizeof(char), buf_size, stdin);
		data[buf_size] = 0; 

		char* exp = nullptr;
		get_param_value(exp, "exp", data);
#endif // DEBUG
#ifdef DEBUG
		const char* exp = "1 2 + 3 *";
#endif // !DEBUG

#ifndef DEBUG
		char* choice = nullptr;
		get_param_value(choice, "type", data);
#endif
#ifdef DEBUG
		const char* choice = "post";
#endif
		bool is_infix = strcmp(choice, "in") == 0 ? true : false;

		if (is_infix) {
			char* postfix = nullptr;
			translate_exp(exp, postfix);
			exp = postfix;
		}
		stack<node*> subtrees;
		istringstream expression(exp);
		while (!expression.eof()) {
			char* str = new char[11];
			expression >> str;
			if (str[1] == '\0' && !isdigit(str[0])) {
				char operation = str[0];
				add_to_tree(operation, subtrees);
			}
			else {
				int num = stoi(str);
				add_to_tree(num, subtrees);
			}
		}
		tree Tree; Tree.root = subtrees.top();
		char* returned_postfix = new char[strlen(exp)];
		get_postfix(Tree, returned_postfix);
		returned_postfix = _strrev(returned_postfix);
		char* returned_infix = new char[strlen(exp)];
		get_infix(Tree, returned_infix);
		cout << "<p>" << returned_postfix << "</p>";
		cout << "<p>" << returned_infix << "</p>";
	}
}

void show_content() {
	char* data = nullptr;
	process_exp(data);
	show_form();
}

int main() {
	cout << "Content-type: text/html; charset=Windows-1251\n\n";
	ifstream f;
	f.open("template/main.html");
	if (f.is_open())
	{
		while (!f.eof())
		{
			char* line = new char[65536];
			f.getline(line, 65535);
			if (strcmp(line, "<!--header-->") == 0)
				cout << header;
			else if (strcmp(line, "<!--menu-->") == 0)
				show_menu();
			else if (strcmp(line, "<!--content-->") == 0)
				show_content();
			else
			{
				cout << line;
			}

		}
		f.close();
	}
	else
	{
		cout << "Can't open template file :(";
	}

	return 0;
}