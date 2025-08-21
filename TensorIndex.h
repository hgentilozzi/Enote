#pragma once
#include <iostream>

struct TensorIndex {
    TensorIndex();

    char name;
    int  value;
    int  dim;

    void reset_bounded();
    void reset_value();

    char get_char_value() const;

    void bind();
    bool is_bounded() const;
    void increment_value();
    bool eof() const;

    friend std::ostream& operator << (std::ostream& os, const TensorIndex& obj);

};
