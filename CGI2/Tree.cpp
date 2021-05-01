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

void get_postfix(tree Tree, char*& postfix) {
	queue<node> que;
	postfix = nullptr;
	DFS_RRL(Tree.root, que);
	int i = 0;
	while (que.size() > 0) {
		node curr;
		if (curr.operation == '\0') {
			char* num = new char[11];
			sprintf(num, "%d", curr.value);
			strcat(postfix, " ");
			strcat(postfix, num);
		}
		else {
			strcat(postfix, " ");
			char* temp = new char[2];
			temp[0] = curr.operation; temp[1] = '\0';
			strcat(postfix, temp);
		}
		curr = que.front();
		que.pop();
	}
}

void DFS_RRL(node* root, queue<node>& que) {
	if (root) {
		que.push(*root);
		DFS_RRL(root->right, que);
		DFS_RRL(root->left, que);
	}
}

void get_infix(tree Tree, char*& infix);

void process_exp(char* data) {
	if (!is_get()) {
		delete[] data;
		int buf_size = get_content_length();
		data = new char[buf_size + 1];
		fread_s(data, buf_size + 1, sizeof(char), buf_size, stdin);
		data[buf_size] = 0;

		char* exp = nullptr;
		get_param_value(exp, "exp", data);

		char* choice = nullptr;
		get_param_value(choice, "type", data);
		bool is_infix = strcmp(choice, "in") == 0 ? true : false;

		if (is_infix) {
			char* postfix = nullptr;
			translate_exp(exp, postfix);
			cout << postfix;
		}
		stack<node> subtrees;
		istringstream expression(exp);
		while (!expression.eof()) {
			char* str = new char[10];
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
		tree Tree; Tree.root = &subtrees.top();
		
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