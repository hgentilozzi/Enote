// Enote.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <sstream>
#include <exception>
#include "Tensor.h"
#include "UCodeLib.h"
#include "EnoteException.h"

int main()
{
    unicode_init();

   /* char c = 'k';
    std::wcout << "t" << char_to_superscript(c) << char_to_subscript(c)  << std::endl;
    std::cin >> c;*/

    try {
        //Tensor v("V", 2, { 1,0 }, "i");
        //Tensor w("W", 2, { 0,1 }, "i");
        Tensor v("V", 3, { 2,0 }, "ij");
        Tensor w("W", 3, { 0,2 }, "ij");
        //Tensor t("T", 3, { 1,1 }, "ii");


        Tensor::print_contraction(v, w);
        //Tensor::print_contraction(t,t);

    //    //std::cout << v;
    }
    catch (EnoteException ex) {
        std::cout << "Exception Caught: " << ex.what() << std::endl;
    }
    
}
