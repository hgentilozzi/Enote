#pragma once
#include <iostream>
#include <vector>
#include "Space.h"

class TensorIndex;

typedef std::vector<TensorIndex> TensorIndexVec;

class TensorIndex {
public:

    TensorIndex(char indexName);
    TensorIndex(char columnName, char indexName);

    void init(char columnName, char indexName);

    char columnName;
    char indexName;
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

private:
    int one_based_value() const;
};

