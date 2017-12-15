#include "cripto.h"
#include <iostream>
#include <string>
#include <cmath>
#include <windows.h>
#include <vector>
#include <cmath>

std::vector<unsigned int> delta;
std::vector<unsigned int> rus_flag;
unsigned int count = {0};

void get_open_key(unsigned int& e, unsigned int& n)
{
	n = p * q; // Модуль N
	f = (p - 1)*(q - 1); // Функция Эйлера
	if (f > 65537) e = {65537}; // Открытая экспонента
	else if (f > 257) e = {257};
	else if (f > 17) e = {7}; // 17
	else if (f > 5) e = {5};
	else e = {3};
}

void get_private_key(unsigned int& d)
{
	double temp = {0.0};
	unsigned int k = {0};
	double eps = {0.00001};
	do
	{
		k++;
		temp = double(k * f + 1) / e;
	}
	while (temp - int(temp) > eps);
	d = int(temp); // Секретная экспонента
}

void encryption(std::string& message)
{
	SetConsoleCP(65001); 
	SetConsoleOutputCP(65001);
	get_open_key(e,n); // Получаем открытый ключ

	std::cout << "До шифрования: ";
	for (auto it : message)
	{
		if (int(it) > 0) // Если буква английская
		{
			std::cout << int(it) << " ";
			rus_flag.push_back(0); // Показываем, что буква английская
		}
		else // Если буква русская
		{
			 std::cout << 256 + int(it) << " ";
			 if (-int(it) < 22) // Если модуль ее кода меньше 22
			 	rus_flag.push_back(2); // Показываем, что буква русская с кодом < 22
			 else
			 	rus_flag.push_back(1); // Показываем, что буква русская с кодом > 22
		}
	}
	std::cout << std::endl << "После шифрования: ";
	for (unsigned int i = {0}; i < message.size(); ++i)
	{
		int basis = {0};

		if (int(message[i]) > 0) // Английская буква
			basis = int(message[i]); // Берем код буквы
		else // Русская буква
			basis =  -int(message[i]); // Берем модуль кода буквы

		get_stable_value(basis); // Вычитаем k*21 из кода буквы
		basis = abs(basis);

		// Производим шифрование
		long long temp = pow(basis,e);
		temp = temp % n;
		std::cout << temp << " ";
		message[i] = temp;
	}
	std::cout << std::endl;
}

void decoding(std::string& message)
{
	SetConsoleCP(65001); 
	SetConsoleOutputCP(65001);
	get_private_key(d); // Получаем закрытый ключ
	
	std::cout << "До расшифрования: ";
	for (auto it : message)
	{
		std::cout << int(it) << " ";
	}

	std::cout << std::endl << "После расшифрования: ";
	for (unsigned int i = {0}; i < message.size(); ++i)
	{
		int basis = int(message[i]); // Получаем код зашифрованной буквы

		// Производим расшифрование
		long long temp = pow(basis,d);
		temp = temp % n;

		if (rus_flag[i] == 2) // Если буква русская, у которой код был < 22
			temp = -temp;

		get_real_value(temp, i); // Добавляем k*21
		if (rus_flag[i] == 1 || rus_flag[i] == 2) // Усли буква русская
		{
			temp = 256-temp;
		}
		std::cout << temp << " ";
		message[i] = temp;
	}
	std::cout << std::endl;
}

void get_stable_value(int& value)
{
	if (value < 2*21 + 1)
	{
		value -= 21;
		delta.push_back(1);
	}
	else if (value > 2*21 + 1 && value < 3*21 + 1)
	{
		value -= 2*21;
		delta.push_back(2);
	}
	else if (value > 3*21 + 1 && value < 4*21 + 1)
	{
		value -= 3*21;
		delta.push_back(3);
	}
	else if (value > 4*21 + 1 && value < 5*21 + 1)
	{
		value -= 4*21;
		delta.push_back(4);
	}
	else if (value > 5*21 + 1 && value < 6*21 + 1)
	{
		value -= 5*21;
		delta.push_back(5);
	}
	else if (value > 6*21 + 1 && value < 7*21 + 1)
	{
		value -= 6*21;
		delta.push_back(6);
	}
	else if (value > 7*21 + 1 && value < 8*21 + 1)
	{
		value -= 7*21;
		delta.push_back(7);
	}
	else if (value > 8*21 + 1 && value < 9*21 + 1)
	{
		value -= 8*21;
		delta.push_back(8);
	}
	else if (value > 9*21 + 1 && value < 10*21 + 1)
	{
		value -= 9*21;
		delta.push_back(9);
	}
	else if (value > 10*21 + 1 && value < 11*21 + 1)
	{
		value -= 10*21;
		delta.push_back(10);
	}
	else if (value > 11*21 + 1 && value < 12*21 + 1)
	{
		value -= 11*21;
		delta.push_back(11);
	}
	else if (value > 12*21 + 1 && value < 13*21 + 1)
	{
		value -= 12*21;
		delta.push_back(12);
	}
	else if (value > 13*21 + 1 && value < 14*21 + 1)
	{
		value -= 13*21;
		delta.push_back(13);
	}
}

void get_real_value(long long& value, unsigned int index) // index - номер буквы в сообщении
{
	if (delta[index] == 1)
	{
		value += 21;
	}
	else if (delta[index] == 2)
	{
		value += 2*21;
	}
	else if (delta[index] == 3)
	{
		value += 3*21;
	}
	else if (delta[index] == 4)
	{
		value += 4*21;
	}
	else if (delta[index] == 5)
	{
		value += 5*21;
	}
	else if (delta[index] == 6)
	{
		value += 6*21;
	}
	else if (delta[index] == 7)
	{
		value += 7*21;
	}
	else if (delta[index] == 8)
	{
		value += 8*21;
	}
	else if (delta[index] == 9)
	{
		value += 9*21;
	}
	else if (delta[index] == 10)
	{
		value += 10*21;
	}
	else if (delta[index] == 11)
	{
		value += 11*21;
	}
	else if (delta[index] == 12)
	{
		value += 12*21;
	}
	else if (delta[index] == 13)
	{
		value += 13*21;
	}
}
