#pragma once

#include <iostream>
#include <vector>
#include "TensorIndex.h"
#include "Space.h"

struct Rank {
    int p;
    int q;
    int total() const { return p + q; }
};

struct TensorConfig {
    //TensorConfig(char name, Rank rank, TensorIndexVec tiv) : name(name), rank(rank), tiv(tiv) {}
    char name;
    Rank rank;
    TensorIndexVec tiv;
};

class Tensor;

typedef std::shared_ptr<Tensor> TensorPtr;
typedef std::vector<TensorPtr> TensorVec;

class Tensor {
public:
    static void print_contraction(Tensor& t);
    static std::shared_ptr<Tensor> make_tensor(TensorConfig tc);
    static std::shared_ptr<Tensor> make_vector(char name,char idxname);
    static std::shared_ptr<Tensor> make_covector(char name,char idxname);
    static std::shared_ptr<Tensor> tensor_product(char name, TensorVec tv);

    Tensor(TensorConfig tc);

    char get_name() const { return tensorName; }
    Rank& get_rank() { return rank; }
    TensorIndexVec& getIndices() { return indices; }
    
    friend std::ostream& operator << (std::ostream& ss, const Tensor& obj);

private:
    char tensorName;
    TensorIndexVec indices;
    Rank rank;
    TensorDimension dim;


    void reset_bounded();
    void reset_index(char idxname);
    void inc_index(char idxname);
    void print_indices();
    bool same_tensor(Tensor& other) const;
    bool indices_at_eof() const;

    std::vector<TensorIndex*> get_indices_by_name(char idxname);

    static void bind_upper_to_Lower(Tensor& t, std::vector<char>& bounded);
    static void inc_index_and_print(Tensor& t, std::vector<char> bounded, size_t cidx);

};



