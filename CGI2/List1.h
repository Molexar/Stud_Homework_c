#pragma once
struct l1_elem
{
	/// <summary>
	/// Информационное поле элемента списка
	/// </summary>
	int a;

	/// <summary>
	/// Адресное поле элемента списка
	/// </summary>
	l1_elem* next = nullptr;
};

/// <summary>
/// Структура для хранения указателя на начало односвязного списка
/// </summary>
struct list1
{
	l1_elem* first = nullptr;
};

/// <summary>
/// Функция добавления элемента в список. Элемент вставляется в конец списка.
/// </summary>
/// <param name="list">Список, куда нужно добавить новое значение</param>
/// <param name="val">Добавляемое в список значение.</param>
void add(list1& list, int val);

/// <summary>
/// Функция добавления элемента в список в указанную позицию
/// </summary>
/// <param name="list">Список, куда нужно добавить элемент</param>
/// <param name="val">Размещаемое в списке значение</param>
/// <param name="pos">Позиция, куда добавляется элемент (начиная с 0).</param>
void insert(list1& list, int val, int pos);

/// <summary>
/// Получение значения элемента списка, расположенного в указанной позиции
/// </summary>
/// <param name="list">Список, значение элемента которого требуется получить</param>
/// <param name="val">Получаемое значение</param>
/// <param name="pos">Позиция элемента, значение которого нужно получить</param>
/// <returns>
/// true - если значение успешно получено и
/// false - если была указана позиция несуществующего элемента
/// </returns>
bool get(const list1& list, int& val, int pos);

/// <summary>
/// Функция удаляет элемент списка, расположенный на указанной позиции
/// </summary>
/// <param name="list">Список, элемент которого нужно удалить</param>
/// <param name="pos">Позиция, с которой нужно удалить значение</param>
/// <returns>
/// true - если значение успешно удалено
/// false - если была указана позиция несуществующего элемента
/// </returns>
bool remove(list1& list, int pos);

/// <summary>
/// Функция удаляет все элементы списка из памяти
/// </summary>
/// <param name="list">Список, который нужно очистить</param>
void clear(list1& list);