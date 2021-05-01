#include "stack_char.h"
#include "tree_struct.h"

void push(stack_char*& top, char elem)
{
	//Создаем элемент стека (размещаем в памяти)
	stack_char* new_el = new stack_char;
	new_el->value = elem;
	//Изменяем указатель на следующий элемент стека
	new_el->next = top;
	//Корректируем указатель на вершину стека
	top = new_el;
}


bool pop(stack_char*& top, char& x)
{
	//Проверка стека на пустоту
	if (!top) return false;
	//Сохранение значения элемента стека
	x = top->value;
	//Сохранение указателя на текущую вершину во временной переменной
	stack_char* old_el = top;
	//Изменение вершины стека
	top = top->next;
	//Очищаем память из-под удаляемого элемента
	delete old_el;
	return true;
}

bool peek(const stack_char* top, char& x)
{
	//Проверка стека на пустоту
	if (!top) return false;
	//Сохранение значения элемента стека
	x = top->value;
	return true;
}

void clear(stack_char*& top)
{
	char x;
	while (top) pop(top, x);
}
