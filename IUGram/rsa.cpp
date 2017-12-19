#include "rsa.h"

using namespace std;

Rsa::Rsa()
{
}

Rsa::~Rsa()
{
}

long Rsa::endecrypt(const long msg, const long key, const long pkey)
{
    long msg_des = 1;
    long root = msg;
    long index = key;
    while (index)
    {
        if (index & 1)
            msg_des = (msg_des * root) % pkey;
        index >>= 1;
        root = (root * root) % pkey;
    }
    return msg_des;
}

/************************************
 * Генерация ключей
 * *********************************/
Key Rsa::produce_keys()
{
    long prime1 = produce_prime();
    long prime2 = produce_prime();
    while (prime2 == prime1)
        prime2 = produce_prime();

    Key key;
    long orla = produce_orla(prime1, prime2);
    key.pkey = produce_pkey(prime1, prime2);
    key.ekey = produce_ekey(orla);
    key.dkey = produce_dkey(key.ekey, orla);
    return key;
}

/************************************
 * Генерация публичного ключа
 * *********************************/
long Rsa::produce_pkey(const long prime1, const long prime2)
{
    return prime1 * prime2;
}

/************************************
 * Генерация значения функции Эйлера
 * *********************************/
long Rsa::produce_orla(const long prime1, const long prime2)
{
    return (prime1 - 1) * (prime2 - 1);
}

/************************************
 *	Генерация открытой экспоненты
 * *********************************/
long Rsa::produce_ekey(const long orla)
{
    long ekey;
    while (true)
    {
        ekey = qrand() % orla;
        if (ekey >= 2 && produce_gcd(ekey, orla) == 1)
            break;
    }
    return ekey;
}

/************************************
 * Генерация приватного ключа (декриптора)
 * *********************************/
long Rsa::produce_dkey(const long ekey, const long orla)
{
    long dkey = orla / ekey;
    while (true)
    {
        if (((dkey * ekey) % orla) == 1)
            break;
        else
            ++dkey;
    }
    return dkey;
}

/************************************
 * Генерация случайных чисел в промежутке
 * от 100 включительно до 200
 * *********************************/
long Rsa::produce_prime()
{
    long prime = 0;
    qsrand(QTime::currentTime().msec());
    while (true)
    {
        prime = qrand() % 100 + 100;
        if (is_prime(prime))
            break;
    }
    return prime;
}

/************************************
 * Вычисление НОД
 * *********************************/
long Rsa::produce_gcd(const long a, const long b)
{
    long dividend = a;
    long divisor = b;
    long residual = dividend % divisor;
    while (residual)
    {
        dividend = divisor;
        divisor = residual;
        residual = dividend % divisor;
    }
    return divisor;
}

/************************************
 * проверка числа на простоту
 * *********************************/
bool Rsa::is_prime(const long digit)
{
    int tmp = 2;
    while (tmp < digit)
        if (!(digit % tmp++))
            break;

    if (tmp == digit)
        return true;
    return false;
}
