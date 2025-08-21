#pragma once

#include <iostream>
#include <vector>
#include "TensorIndex.h"


typedef int TensorDimension;

struct Rank {
    int p;
    int q;
    int total() const { return p + q; }
};

class Tensor {
public:
    Tensor(const char* name, TensorDimension dim, Rank rank, const char* idxnames);

    static void print_contraction(Tensor& t);
    static void print_contraction(Tensor& v, Tensor& w);


    std::string get_name() const { return name; }
    Rank& get_rank() { return rank; }
    

    friend std::ostream& operator << (std::ostream& ss, const Tensor& obj);

private:
    std::string name;
    std::shared_ptr<TensorIndex[]> indices;
    TensorDimension dim;
    Rank rank;

    void reset_bounded();
    void reset_index(char idxname);
    void inc_index(char idxname);
    void print_indices();
    bool same_tensor(Tensor& other) const;
    bool indices_at_eof() const;

    std::vector<TensorIndex*> get_indices_by_name(char idxname);

    static void bind_upper_to_Lower(Tensor& v, Tensor& w, std::vector<char>& bounded);
    static void inc_index_and_print(Tensor& v, Tensor& w, std::vector<char> bounded, size_t cidx);

};
