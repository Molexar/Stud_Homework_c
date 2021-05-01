#include "stack_char.h"
#include "tree_struct.h"

void push(stack_char*& top, char elem)
{
	//������� ������� ����� (��������� � ������)
	stack_char* new_el = new stack_char;
	new_el->value = elem;
	//�������� ��������� �� ��������� ������� �����
	new_el->next = top;
	//������������ ��������� �� ������� �����
	top = new_el;
}


bool pop(stack_char*& top, char& x)
{
	//�������� ����� �� �������
	if (!top) return false;
	//���������� �������� �������� �����
	x = top->value;
	//���������� ��������� �� ������� ������� �� ��������� ����������
	stack_char* old_el = top;
	//��������� ������� �����
	top = top->next;
	//������� ������ ��-��� ���������� ��������
	delete old_el;
	return true;
}

bool peek(const stack_char* top, char& x)
{
	//�������� ����� �� �������
	if (!top) return false;
	//���������� �������� �������� �����
	x = top->value;
	return true;
}

void clear(stack_char*& top)
{
	char x;
	while (top) pop(top, x);
}
