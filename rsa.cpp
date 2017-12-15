#include "cripto.h"
#include <iostream>
#include <string>
#include <cctype>
#include <windows.h>

int main()
{
	system("chcp 65001");
	using std::cout;
	using std::cin;
	using std::endl;
	using std::getline;
	using std::string;

	string msg;
	cout << endl << "Введите сообщение: ";
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	getline(cin, msg);
	string temp;
	SetConsoleCP(65001);
	SetConsoleOutputCP(65001);
	encryption(msg);
	cout << endl << "Зашифрованное сообщение: ";
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	cout << msg << endl;
	SetConsoleCP(65001);
	SetConsoleOutputCP(65001);
	decoding(msg);
	cout << endl << "Расшифрованное сообщение: ";
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	cout << msg << endl;
	SetConsoleCP(65001);
	SetConsoleOutputCP(65001);

	system("pause");
	return 0;
}
