// program73.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>

using namespace std;

struct S
{
    char ch1;
    int i;
};

int main()
{
    cout << "Size of character = " << sizeof(char) << endl;
    cout << "Size of integer = " << sizeof(int) << endl;
    cout << "Size of structure = " << sizeof(S) << endl;
    return 0;
}
