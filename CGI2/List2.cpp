#include "list2.h"

void add(list2& list, double data)
{
	l2_elem* new_el = new l2_elem;
	new_el->x = data;
	if (!list.count)
	{
		list.first = list.last = new_el;
	}
	else
	{
		if (new_el->x < list.first->x)
		{
			list.first->prev = new_el;
			new_el->next = list.first;
			list.first = new_el;
		}
		else if (new_el->x > list.last->x)
		{
			list.last->next = new_el;
			new_el->prev = list.last;
			list.last = new_el;
		}
		else
		{
			auto curr = list.first;
			while (curr->x < new_el->x)
			{
				curr = curr->next;
			}
			new_el->next = curr;
			new_el->prev = curr->prev;
			curr->prev = new_el;
			new_el->prev->next = new_el;
		}
	}
	list.count++;
}

bool remove(list2& list, int pos)
{
	if (pos < 0 || pos >= list.count)
		return false;
	list.count--;
	if (pos == 0)
	{
		auto del = list.first;
		list.first = list.first->next;
		if (list.first) list.first->prev = nullptr;
		delete del;
		return true;
	}
	if (pos == list.count)
	{
		auto del = list.last;
		list.last = list.last->prev;
		list.last->next = nullptr;
		delete del;
		return true;
	}
	auto frw = (pos <= list.count / 2);
	int p = 1;
	l2_elem* curr;
	if (frw)
	{
		curr = list.first->next;
	}
	else
	{
		curr = list.last->prev;
		pos = list.count - pos;
	}
	while (p < pos)
	{
		curr = frw ? curr->next : curr->prev;
		p++;
	}
	curr->prev->next = curr->next;
	curr->next->prev = curr->prev;
	delete curr;
	return true;
}

const double* get(list2 list, int pos)
{
	if (pos < 0 || pos >= list.count || !list.first)
		return nullptr;

	if (!pos) return &list.first->x;
	if (pos == list.count - 1) return &list.last->x;
	int p = 1;
	bool fwd = pos <= list.count / 2;
	l2_elem* curr;
	if (fwd)
	{
		curr = list.first->next;
	}
	else
	{
		curr = list.last->prev;
		pos = list.count - pos - 1;
	}
	while (curr && p < pos)
	{
		p++;
		curr = fwd ? curr->next : curr->prev;
	}
	return &curr->x;
}

void clear(list2& list)
{
	l2_elem* rem;
	while (list.first)
	{
		rem = list.first;
		list.first = list.first->next;
		delete rem;
	}
	list.last = nullptr;
	list.count = 0;
}