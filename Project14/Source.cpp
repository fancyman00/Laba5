#include<fstream>
#include<Windows.h>
#include<functional> 
#include<iostream> 

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::istream;
using std::function;

bool check_file(ifstream& file);
void ending(int n);
template <typename T, typename Predicat>
void read_and_check(T& x, istream& stream, Predicat condition, const char* message);
int get_random_in_interval(int a, int b);
void print(int* arr, int size, const char* message);
void preamb(short choice, ifstream& file, int& size);

int* memory_allocation(int size);
void free_memory(int*& arr);

void fill(int* arr, int size, istream& stream = cin);
void fill(int* begin, int* end, int A, int B);

bool task1(int* begin, int* end, int& result, function<bool(int)> predicate);
int task2(int* min_index, int* max_index);
void task3(int* arr, int size);

int* find_min_index(int* begin, int* end);
int* find_max_index(int* begin, int* end);

template<typename Printable>
int menu(Printable print_menu);

int main()
{
	SetConsoleOutputCP(1251);

	short mainChoice;
	do
	{
		mainChoice = menu([]() {
			cout << "Меню" << endl;
			cout << "1. Вычислить сумму отрицательных элементов массива" << endl;
			cout << "2. Вычислить произведение элементов массива, расположенных максимальным и минимальными элементами массива" << endl;
			cout << "3. Упорядочить по возрастанию элементы методом простого выбора" << endl;
			cout << "4. Завершить работу" << endl; 
		});
		if (mainChoice != 4)
		{
			short choice;
			do
			{
				choice = menu([]() {
					cout << "1. Ввод чисел с клавиатуры" << endl;
					cout << "2. Ввод чисел из файла" << endl;
					cout << "3. Случайный набор чисел" << endl;
					cout << "4. Выйти в главное меню" << endl;
				});
				if (choice != 4)
				{
					int size;
					ifstream file("data.txt");
					preamb(choice, file, size);
					int* arr = memory_allocation(size);

					switch (choice)
					{
					case 1:
					{
						ending(size);
						fill(arr, size, cin);
						break;
					}
					case 2:
					{
						fill(arr, size, file);
						print(arr, size, "Элементы массива: ");
						break;
					}
					default:
					{
						int a, b;
						cout << "Введите допустимый диапазон чисел (от A до B): ";
						read_and_check(a, cin, [](int x) {return true; }, "-> ");
						read_and_check(b, cin, [](int x) {return true; }, "");
						cin.ignore(cin.rdbuf()->in_avail());
						fill(arr, arr + size, a, b);
						print(arr, size, "Элементы массива : ");
					}
					}

					switch (mainChoice)
					{
					case 1:
					{
						int product;
						if (task1(arr, arr + size, product, [](int x) {return x < 0; }))
						{
							cout << "Сумма отрицательных элементов: " << product << endl;
						}
						else
							cout << "Нет отрицательных элементов! " << endl;
						break;
					}
					case 2:
					{
						int* min_index = find_min_index(arr, arr + size);
						int* max_index = find_max_index(arr, arr + size);
						if (min_index == max_index || max_index - min_index == 1 || min_index - max_index == 1)
							cout << "Пустой диапазон!" << endl;
						else
							cout << "Произведение заданных элементов: " << task2(min_index, max_index) << endl;
						break;
					}
					default:
					{
						task3(arr, size);
						print(arr, size, "Отсортированные по данному правилу элементы массива: ");
					}
					}
					free_memory(arr);
				}
			} while (choice != 4);

		}
	} while (mainChoice != 4);

}

int* find_min_index(int* begin, int* end)
{
	int* min_index = begin;
	for (int* ptr = begin; ptr < end; ++ptr)
	{
		if (*ptr < *min_index)
		{
			min_index = ptr;
		}
	}
	return min_index;
}
int* find_max_index(int* begin, int* end)
{
	int* max_index = begin;
	for (int* ptr = begin; ptr < end; ++ptr)
	{
		if (*ptr >= *max_index)
		{
			max_index = ptr;
		}
	}
	return max_index;
}

bool task1(int* begin, int* end, int& result, function<bool(int)> predicate)
{
	bool isFind = false;
	result = 0;
	for (int* ptr = begin; ptr < end; ++ptr)
	{
		if (predicate(*ptr))
		{
			result += *ptr;
			isFind = true;
		}
	}
	return isFind;
}

int task2(int* begin, int* end)
{
	int sum = 1;
	for (int* ptr = (begin > end ? end : begin) + 1; ptr < (begin > end ? begin : end); ++ptr)
	{
		sum = sum * *ptr;
	}
	return sum;
}

void task3(int* arr, int size)
{
	bool isSorted;

	do
	{
		isSorted = true;
		for (int i = 0; i < size - 1; i++)
		{
			if (arr[i] > arr[i + 1])
			{
				int temp = arr[i];
				arr[i] = arr[i + 1];
				arr[i + 1] = temp;
				isSorted = false;
			}
		}
	} while (!isSorted);
}
template<typename Printable>
int menu(Printable print_menu)
{
	print_menu();
	function<bool(int)> Lambda = [](int x)->bool
	{
		return x >= 1 && x <= 4;
	};
	int choice = 0;
	read_and_check(choice, cin, Lambda, "-> ");
	std::cin.ignore(cin.rdbuf()->in_avail());
	return choice;
}


bool check_file(ifstream& file)
{
	int result = 1;
	if (!file)
	{
		result = -1;
		cout << "Файл не найден!" << endl;
	}
	else if (file.peek() == EOF) //==-1
	{
		result = 0;
		cout << "Файл пустой!" << endl;
	}
	return result == 1;
}

void ending(int n)
{
	cout << "Введите " << n << " элемент";
	if (n < 21 && n>10)
		cout << "ов: ";
	else
	{
		switch (n % 10)
		{
		case 1:
			cout << ": ";
			break;
		case 2:
		case 3:
		case 4:
			cout << "а: ";
			break;
		default:
			cout << "ов: ";
			break;

		}
	}
}

int get_random_in_interval(int a, int b)
{
	return a + rand() % (b - a + 1);
}

int* memory_allocation(int size)
{
	return new int[size];
}

void free_memory(int*& arr)
{
	delete[]arr;
	arr = nullptr;
}

void print(int* arr, int size, const char* message)
{
	cout << message;
	for (int i = 0; i < size; i++)
	{
		cout << arr[i] << ' ';
	}
	cout << endl;
}

void preamb(short choice, ifstream& file, int& size)
{
	switch (choice)
	{
	case 1:
	{
		cout << "Введите количество элементов: ";
		read_and_check(size, cin, [](int x) {return x > 0; }, "");
		cin.ignore(cin.rdbuf()->in_avail());
		break;
	}
	case 2:
	{
		if (check_file(file))
			file >> size;
		break;
	}
	default:
	{
		cout << "Введите количество случайных слагаемых: ";
		read_and_check(size, cin, [](int x) {return x > 0; }, "\n-> ");
		cin.ignore(cin.rdbuf()->in_avail());
	}
	}
}

void fill(int* arr, int size, istream& stream)
{
	for (int i = 0; i < size; i++)
	{
		read_and_check(arr[i], stream, [](int x) {return true; }, "");
	}
	cin.ignore(cin.rdbuf()->in_avail());
}

void fill(int* begin, int* end, int a, int b)
{
	for (int* ptr{ begin }; ptr < end; ++ptr)
	{
		*ptr = get_random_in_interval(a, b);
	}
}

template<typename T, typename Predicat>
void read_and_check(T& x, istream& stream, Predicat condition, const char* message)
{
	cout << message;
	while (!(stream >> x && condition(x)))
	{
		cout << "Ошибка ввода!" << endl;
		cin.clear();
		cin.ignore(cin.rdbuf()->in_avail());
		cout << message;
	}
}