#include <iostream>
#include "cgi.h"
#include "Circled_List.h"
#include <fstream>

using namespace std;

const char* header = "Circled List";

void add(Circled_List& list, int num) {
	elem* new_elem = new elem;
	new_elem->value = num;
	if (list.barrier) {
		if (list.length > 1) {
			elem* prev = list.barrier->prev;
			new_elem->prev = prev;
			new_elem->next = list.barrier;
			prev->next = new_elem;
			list.barrier->prev = new_elem;
		}
		else {
			list.barrier->next = new_elem;
			list.barrier->prev = new_elem;
			new_elem->next = list.barrier;
			new_elem->prev = list.barrier;
		}
	}
	else {
		list.barrier = new_elem;
	}
	list.length++;
}

inline elem* iter(bool right, const elem* curr) {
	return (right ? curr->next : curr->prev);
}

int get_absolute_index(int pos, int length) {
	int index = pos % length;
	index = index >= 0 ? index : length + index;
	return index;
}

elem* _get_elem(Circled_List& list, int pos) {
	if (!list.barrier) {
		return nullptr;
	}
	int index = get_absolute_index(pos, list.length);
	elem* curr = list.barrier;
	if (index > list.length / 2) {
		int i = list.length - 1;
		while (i >= index) {
			curr = iter(false, curr);
			i--;
		}
	}
	else {
		int i = 1;
		while (i <= index) {
			curr = iter(true, curr);
			i++;
		}
	}
	return curr;
}


void insert(Circled_List& list, int num, int pos) {
	pos = get_absolute_index(pos, list.length);
	if (list.barrier) {
		elem* curr = _get_elem(list, pos);
		elem* new_elem = new elem{ num };
		if (pos == 0) {
			list.barrier = new_elem;
		}
		if (pos > list.length / 2) {
			new_elem->next = curr->next;
			new_elem->prev = curr;
			if (curr->next) { curr->next->prev = new_elem; }
			curr->next = new_elem;
		}
		else {
			new_elem->next = curr;
			new_elem->prev = curr->prev; // Поправить
			curr->prev->next = new_elem;
			curr->next->prev = new_elem;
		}
		list.length++;
	}
}

int get(Circled_List& list, int pos) {
	elem* getting = _get_elem(list, pos);
	if (!getting) return NAN;
	return getting->value;
}

bool remove(Circled_List& list, int num) {
	if (list.barrier) {
		elem* curr = list.barrier;
		if (curr->value == num) {
			if (list.length == 1) { delete list.barrier; list.barrier = nullptr; }
			else if (list.length == 2) { 
				list.barrier = list.barrier->next; delete list.barrier->next;
				list.barrier->next = nullptr;
				list.barrier->prev = nullptr;
			}
			else {
				list.barrier = curr->next;
				curr->prev->next = curr->next;
				curr->next->prev = curr->prev;
				delete curr; curr = nullptr;
			}
			return true;
		}
		int pt = 0;
		while (curr->value != num && pt < list.length) { curr = curr->next; pt++; }
		if (curr->value == num) {
			curr->prev->next = curr->next;
			curr->next->prev = curr->prev;
			delete curr; curr = nullptr;
		}
		else { return false; }
	}
	else {
		return false;
	}
}

bool remove_all(Circled_List& list, int num) {
	bool flag = remove(list, num);
	while (remove(list, num));
	if (flag == false) {
		return false;
	}
	return true;
}

void clear(Circled_List& list) {
	if (list.barrier) {
		elem* barrier = list.barrier;
		elem* curr = barrier->next;
		while (curr && (curr != barrier)) {
			elem* next = curr->next;
			delete curr; curr = nullptr;
			curr = next;
		}
		delete barrier; barrier = nullptr;
		list.length = 0;
		list.barrier = nullptr;
	}
}

void show_list(Circled_List list) {
	elem* curr = list.barrier->next;
	cout << list.barrier->value << " ";
	while (curr != list.barrier) {
		cout << curr->value << " ";
		curr = curr->next;
	}
	cout << "<br>";
}

void show_content() {
	ifstream input("static/list.txt");
	if (input.is_open()) {
		Circled_List list;
		while (!input.eof()) {
			int num = 0;
			input >> num;
			add(list, num);
		}
		show_list(list);
		remove(list, 1);
		show_list(list);
		cout << remove(list, -1) << "<br>";
		insert(list, 25, list.length+1);
		insert(list, 26, -2);
		show_list(list);
		cout << get(list, -1) << "<br>";
		clear(list);
		cout << bool(list.barrier);
	}
	else {
		cout << "Error opening the file";
	}
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