#pragma once

void translate_exp(const char* expression, char*& postfix);
struct elem
{
	char x;
	elem* next = nullptr;
	int index;
};

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
	for (int i = 0; i < (int)strlen(expression); i++) {
		if (is_left_bracket(expression[i])) {
			push(top, expression[i], i);
		}
		else if (is_right_bracket(expression[i])) {
			if (!top) { error[0] = i; return false; }
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