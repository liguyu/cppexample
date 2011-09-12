// program68.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>

using namespace std;

class C 
{
public:
    void NonStaticFunc() 
    {    
        cout << "NonStaticFun" << endl;
    }
	
    static void StaticFun(C* pC) 
    {
        cout << "StaticFun" << endl;
        pC->NonStaticFunc();
    }
};

int main()
{
    C objC;
    C::StaticFun(&objC);
    return 0;
}