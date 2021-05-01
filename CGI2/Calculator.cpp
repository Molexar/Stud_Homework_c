#include "cgi.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "stack.h"
#include "Calculator.h"

using namespace std;
const char* header = "Calculator";

void show_form() {
	cout << "<form method='post' action='calculator.cgi'>"
		<< "<input type='text' size = 30 name='exp'>"
		<< "<input type='submit'>"
		<< "</form>";
}

bool is_num(const char ch) {
	if (ch >= '0' && ch <= '9') {
		return true;
	}
	return false;
}

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

void pop_to_br(stack*& top, char*& postfix, int& index_post) {
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
    stack* top = nullptr;
    int index_post = 0;
    for (int i = 0; i < strlen(expression); i++) {
        if (is_operand(expression[i])) {
            postfix[index_post++] = expression[i];
        }
        else {
            postfix[index_post++] = ' ';
            char temp; peek(top, temp);
            if (expression[i]=='-' && is_unary(expression, i)) {
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

double calc_operation(double first, double second, char ch) {
    if (ch == '+') { return first + second; }
    if (ch == '-') { return first - second; }
    if (ch == '*') { return first * second; }
    if (ch == '/') { return first / second; }
    if (ch == '^') { return pow(first, second); }
    return 0;
}

double calc_expression(const char* exp) {
    stack_num* top = nullptr;
    istringstream ss(exp);

    ss.seekg(0, ss.end);
    streamoff end = ss.tellg();
    ss.seekg(0, ss.beg);
    
    while (!ss.eof()) {
        char operation; double num;
        ss >> num;
        if (ss.fail()) {
            ss.clear();
            streamoff err_index = ss.tellg();
            ss.seekg(err_index - (streamoff)1);
            ss >> operation;
            if (operation != '~') {
                double f, s; pop(top, s); pop(top, f);
                push(top, calc_operation(f, s, operation));
            }
            else {
                double f; pop(top, f);
                push(top, -f);
            }
        }
        else {
            push(top, num);
        }
        if (ss.tellg() == end) {
            break;
        }
    }

    double ans=0; pop(top, ans);
    clear(top);
    return ans;
}

    void get_form_data(char*& data) {
            if (!is_get()) {
#if !_DEBUG
            delete[] data;
            int buf_size = get_content_length();
            data = new char[buf_size + 1];
            fread_s(data, buf_size + 1, sizeof(char), buf_size, stdin);
            data[buf_size] = 0;

            char* expression = nullptr;
#endif
#if _DEBUG
            const char* expression = "1+2*3";
#else
            //Получаю значение поля формы
            get_param_value(expression, "exp", data);
#endif
            char* postfix = new char[2 * strlen(expression)];

            // Проверка корректности расстановки скобок
            bool is_right = check_brackets(expression);

            if (is_right) {
                translate_exp(expression, postfix);
                double result = calc_expression(postfix);
                cout << "<p>" << expression << "</p>";
                cout << "<p>" << postfix << "</p>";
                cout << "<p>" << result << "</p>";
            }
            else {
                cout << "<p>Brackets in expression is incorrect</p>";
            }
            delete[] data;
            delete[] expression;
            delete[] postfix;
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
//мурчик.яратам ^^