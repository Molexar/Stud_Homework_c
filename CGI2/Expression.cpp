#include "cgi.h"
#include <fstream>
#include <iostream>
#include "Expression.h"

using namespace std;
const char* header = "Expression";

void push(elem*& stack, char x, int i) {
	//Создаем элемент стека (размещаем в памяти)
	elem* new_el = new elem;
	new_el->x = x;
	new_el->index = i;
	//Изменяем указатель на следующий элемент стека
	new_el->next = stack;

	//Корректируем указатель на вершину стека
	stack = new_el;
}

bool pop(elem*& stack, char& x, int& i) {
	//Проверка стека на пустоту
	if (!stack) return false;
	//Сохранение значения элемента стека
	x = stack->x;
	i = stack->index;
	//Сохранение указателя на текущую вершину во временной переменной
	elem* old_el = stack;
	//Изменение вершины стека
	stack = stack->next;
	//Очищаем память из-под удаляемого элемента
	delete old_el;
	return true;
}

bool peek(const elem* stack, char& x, int& i) {
	//Проверка стека на пустоту
	if (!stack) return false;
	//Сохранение значения элемента стека
	x = stack->x;
	i = stack->index;
	return true;
}

void clear(elem*& stack) {
	char x;
	int i;
	while (stack) pop(stack, x, i);
}

bool is_left_bracket(const char ch) {
	if (ch == '(' || ch == '[' || ch == '{' || ch == '<') { return true; }
	return false;
}

bool is_right_bracket(const char ch) {
	if ((ch == ')') || (ch == ']') || (ch == '}') || (ch == '>')) { return true; }
	return false;
}

bool is_valid(const char bracket, const char ch) {
	if ((bracket == '(' && ch == ')') || (bracket == '[' && ch == ']') || (bracket == '{' && ch == '}') || (bracket == '<' && ch == '>')) {
		return true;
	}
	return false;
}

bool process_exp(const char* expression, int*& error, elem*& top) {
	for (int i = 0; i < strlen(expression); i++) {
		if (is_left_bracket(expression[i])) {
			push(top, expression[i], i);
		}
		else if (is_right_bracket(expression[i])) {
			if (!top) {error[0] = i; return false;}
			char sym; int index;
			if (is_valid(top->x, expression[i])) {
				pop(top, sym, index);
			}
			else {
				error[1] = i;
				error[0] = top->index;
				return false;
			}
		}
	}
	if (top) { error[0] = top->index; return false; }
	return true;
}

void show_form() {
	cout << "<form action='expression.cgi' method='post'>"
		<< "<p>Type expression:<br>"
		<< "<input type='text' size = 100 name='str'></p>"
		<< "<input type='submit'>"
		<< "</form>";
}

void mark_symbol(char ch) {
	cout << "<b style='color: red;'>"
		<< ch
		<< "</b>";
}

void get_form_data(char*& data) {
	if (!is_get()) {
		delete[] data;
		int buf_size = get_content_length();
		data = new char[buf_size + 1];
		fread_s(data, buf_size + 1, sizeof(char), buf_size, stdin);
		data[buf_size] = 0;

		char* expression = nullptr;
		get_param_value(expression, "str", data);

		int* error = new int[2]{-1, -1};
		elem* top = nullptr;
		bool is_right = process_exp(expression, error, top);

		clear(top);

		if (is_right) {
			cout << "<p>"
				<< expression
				<< "</p>";
		}
		else {
			cout << "<p>";
			for (int i = 0; i < strlen(expression); i++) {
				if (i == error[0] || i == error[1]) {
					mark_symbol(expression[i]);
				}
				else {
					cout << expression[i];
				}
			}
			cout << "</p>";
		}
		delete[] data;
		clear(top);

	}
}

void show_content() {
	char* data = nullptr;
	get_form_data(data);
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

			}
			cout << line;
		}
		f.close();
	}
	else
	{
		cout << "Can't open template file :(";
	}

	return 0;
}