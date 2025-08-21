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
        SpacePtr space = Space::make_space("HSpace",3, false);
        
        TensorPtr v = Tensor::make_vector('v', 'i');
        TensorPtr b = Tensor::make_vector('b', 'j');
        TensorPtr w = Tensor::make_covector('w', 'i');
        TensorPtr c = Tensor::make_covector('c', 'j');
        TensorPtr t = Tensor::tensor_product('t', { v,b,w,c});
        Tensor::print_contraction(*t);

    }
    catch (EnoteException ex) {
        std::wcout << "Exception Caught: " << ex.what() << std::endl;
    }
    
}
