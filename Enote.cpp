// Enote.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Tensor.h"
#include "UCodeLib.h"
#include "EnoteException.h"

int main()
{
    unicode_init();

    try {
        //Tensor v("V", 2, { 1,0 }, "i");
        //Tensor w("W", 2, { 0,1 }, "i");
        //Tensor v("G", 3, { 2,0 }, "ij");
        //Tensor w("T", 3, { 1,3 }, "kijl");
        Tensor t("T", 3, { 1,3 }, "ijik");


        //Tensor::print_contraction(v, w);
        Tensor::print_contraction(t,t);

    //    //std::cout << v;
    }
    catch (EnoteException ex) {
        std::cout << "Exception Caught: " << ex.what() << std::endl;
    }
    
}
