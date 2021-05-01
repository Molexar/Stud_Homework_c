#include <iostream>
#include <fstream>
#include "cgi.h"
#include "List1.h"

using namespace std;

const char* header = "Lists";

void show_list(const list1& list) {
	l1_elem* curr = list.first;
	cout << "<p>";
	while (curr) {
		cout << curr->a << " ";
		curr = curr->next;
	}
	cout << "</p>";
}

void reverse_list(list1& list) {
	l1_elem* curr = list.first;
	l1_elem* next = nullptr;
	l1_elem* prev = nullptr;

	while (curr) {
		next = curr->next;
		curr->next = prev;
		prev = curr;
		curr = next;
	}
	list.first = prev;
}

bool is_circled(const list1& list) {
	l1_elem* first = list.first;
	l1_elem* second = list.first->next;
	while (first != second && second->next && first) {
		first = first->next;
		second = second->next->next;
	}
	if (first == second) {
		return true;
	}
	else {
		return false;
	}
}

void process_form_data() {
	if (is_get()) {
		ifstream input("static/input.txt");
		list1 list;

		if (input.is_open()) {
			while (!input.eof()) {
				int num; input >> num;
				add(list, num);
			}
			show_list(list);
			reverse_list(list);
			bool circled = is_circled(list);
			if (circled) { cout << "Circled"; }
			else { cout << "Usual"; }
			show_list(list);

			//Вручную ссылаю последний элемент на второй
			l1_elem* curr = list.first;
			while (curr->next) {
				curr = curr->next;
			}
			curr->next = list.first->next;
			bool cirled = is_circled(list);
			cout << "<p>" << cirled ? cout << "Circled" : cout << "Usual";
		}
		else {
			cout << "Cannot open file";
		}
	}
}

void show_content() {
	//char* data = nullptr;
	process_form_data();
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