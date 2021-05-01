#pragma once

/// <summary>
/// Структура для хранения элементов списка
/// </summary>
struct l2_elem
{
	/// <summary>
	/// Хранимое значение (информационное поле)
	/// </summary>
	double x;

	/// <summary>
	/// Указатель на следующий элемент списка (адресное поле)
	/// </summary>
	l2_elem* next = nullptr;

	/// <summary>
	/// Указатель на следующий элемент списка (адресное поле)
	/// </summary>
	l2_elem* prev = nullptr;
};

/// <summary>
/// Структура для объединения указателей, определяющих двусвязный список
/// </summary>
struct list2
{
	/// <summary>
	/// Указатель на первый элемент списка
	/// </summary>
	l2_elem* first = nullptr;

	/// <summary>
	/// Указатель на последний элемент списка
	/// </summary>
	l2_elem* last = nullptr;

	/// <summary>
	/// Поле для хранения количества элементов списка
	/// </summary>
	int count = 0;
};

/// <summary>
/// Функция для добавления элемента в список
/// </summary>
/// <param name="list">Список, в который нужно вставить значение</param>
/// <param name="data">Добавляемое в список значение</param>
void add(list2& list, double data);

/// <summary>
/// Функция для удаления элемента из списка
/// </summary>
/// <param name="list">Список, откуда удаляется элемент</param>
/// <param name="position">Позиция, с которой нужно удалить значение</param>
/// <returns></returns>
bool remove(list2& list, int position);

/// <summary>
/// Получение значения элемента списка
/// </summary>
/// <param name="list">Список, откуда получается значение</param>
/// <param name="position">Позиция, из которой нужно получить значение</param>
/// <returns>Указатель на значение элемента списка или nullptr, если элемента с указанным номером нет</returns>
const double* get(list2 list, int position);

/// <summary>
/// Удаление всех элементов списка
/// </summary>
/// <param name="list">Список, который нужно удалить из памяти</param>
void clear(list2& list);