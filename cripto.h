#ifndef CRIPTO_H_
#define CRIPTO_H_
#include <string>
void get_open_key(unsigned int& e, unsigned int& n);
void get_private_key(unsigned int& d);
void encryption(std::string& message);
void decoding(std::string& message);
void get_stable_value(int& value);
void get_real_value(long long& value, unsigned int index);
const unsigned int p = 3; //1867;
const unsigned int q = 11;   //1877;
static unsigned int n = {0};
static unsigned int e = {0};
static unsigned int f = {0};
static unsigned int d = {0};
#endif
