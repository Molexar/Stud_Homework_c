#include "List1.h"

void add(list1& list, int val)
{
	// Создание нового элемента
	auto new_el = new l1_elem;
	new_el->a = val;

	// Определяем наличие списка
	if (!list.first)
	{
		// Списка не было. Элемент становится первым в этом списке
		list.first = new_el;
	}
	else
	{
		// Список был, переходим к его последнему элементу
		auto curr = list.first;
		while (curr->next) curr = curr->next;
		// Производим вставку нового элемента в список
		curr->next = new_el;
	}
}

void insert(list1& list, int val, int pos)
{
	// Создание нового элемента
	auto new_el = new l1_elem;
	new_el->a = val;

	if (list.first && pos > 0)
	{
		// Указатель на текущий элемент в списке
		l1_elem* curr = list.first;
		// Счетчик элементов
		int p = 0;
		// Проходим по элементам списка до позиции, предшествующей позиции вставки
		// либо до последнего элемента списка.
		while (curr->next && p < pos - 1)
		{
			curr = curr->next;
			p++;
		}
		// Размещение нового элемента в списке.
		new_el->next = curr->next;
		curr->next = new_el;
	}
	else
	{
		// Списка или нет, или позиция вставки = 0
		new_el->next = list.first;
		list.first = new_el;
	}
}

bool get(const list1& list, int& val, int pos)
{
	// Если списка нет или указана отрицательная позиция.
	if (!list.first || pos < 0) return false;

	int p = 0; // Счетчик элементов
	l1_elem* curr = list.first; // Указатель на текущий элемент списка
	// Находим указанный элемент в списке
	while (curr && p < pos)
	{
		curr = curr->next;
		p++;
	}
	// После цикла указатель curr может содержать адрес искомого элемента,
	// либо быть равным nullptr, если запрошенная позиция превышала число элементов списка.
	if (curr)
	{
		// Возвращаем найденное значение
		val = curr->a;
		return true;
	}
	return false;
}

bool remove(list1& list, int pos)
{
	// Проверяем, что список не пуст и позиция удаления неотрицательная
	if (pos < 0 || !list.first) return false;
	// Если удаляется первый элемент
	if (pos == 0)
	{
		l1_elem* del = list.first;
		list.first = list.first->next;
		delete del;
		return true;
	}
	// Если удаляется элемент из середины
	int p = 0;
	l1_elem* curr = list.first;
	// Поиск элемента, предшествующего удаляемому
	while (curr->next && p < pos - 1)
	{
		curr = curr->next;
		p++;
	}
	// Если за текущим элементом есть еще один, значит
	// мы нашли удаляемое значение
	if (curr->next)
	{
		// Выполняем удаление
		l1_elem* del = curr->next;
		curr->next = curr->next->next;
		delete del;
		return true;
	}
	// Удалять нечего.
	return false;
}

void clear(list1& list)
{
	while (list.first)
	{
		l1_elem* del = list.first;
		list.first = list.first->next;
		delete del;
	}
}