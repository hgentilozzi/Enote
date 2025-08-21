#pragma once
#include <iostream>
#include "Space.h"

struct TensorIndex {
    TensorIndex();

    char name;
    int  value;
    TensorDimension dim;
    bool zeroBased;

    void reset_bounded();
    void reset_value();

    char get_char_value() const;

    void bind();
    bool is_bounded() const;
    void increment_value();
    bool eof() const;

    friend std::ostream& operator << (std::ostream& os, const TensorIndex& obj);

};
