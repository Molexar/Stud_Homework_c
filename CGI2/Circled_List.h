#pragma once
struct elem {
	int value = 0;
	elem* next = nullptr;
	elem* prev = nullptr;
};

struct Circled_List {
	elem* barrier = nullptr;
	size_t length = 0;
};

void append(Circled_List& list, int num);