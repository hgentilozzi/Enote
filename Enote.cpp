// Enote.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Space.h"
#include "Tensor.h"
#include "UCodeLib.h"
#include "EnoteException.h"

int main()
{
    unicode_init();

    try {
        SpacePtr space = Space::make_space("HSpace",2, true);

        Tensor t("T",{ 1,1 }, "ii");

        Tensor::print_contraction(t,t);

    //    //std::cout << v;
    }
    catch (EnoteException ex) {
        std::cout << "Exception Caught: " << ex.what() << std::endl;
    }
    
}
