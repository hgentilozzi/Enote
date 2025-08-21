#include <iostream>
#include <string>
#include "Tensor.h"
#include "UcodeLib.h"
#include "EnoteException.h"
#include <sstream>

Tensor::Tensor(TensorConfig tc) : tensorName(tc.name), rank(tc.rank) {

    dim = Space::get_space()->get_dim();

    for (int i = 0; i < tc.tiv.size();i++) {
        TensorIndex ti = tc.tiv[i];

        if (ti.columnName == '\0')
            ti.columnName = tensorName;

        indices.push_back(ti);
    }

    // index names must name all indices
    if (indices.size() != rank.total()) {
        std::stringstream ss;
        ss << "Tensor::ctor: Wrong number of index names for rank. T=" << tensorName
            << " rank=(" << rank.p << "," << rank.q << ") Indices = " << indices.size();

        throw EnoteException(ss.str());
    }


}

/// <summary>
/// Prints information about the contraction of two tensors, including validation of their dimensions and types.
/// </summary>
/// <param name="v">The first (contravariant) tensor involved in the contraction.</param>
/// <param name="w">The second (covariant) tensor involved in the contraction.</param>
void Tensor::print_contraction(Tensor& t)
{
    int dim = t.dim;
    std::vector<char> bounded;

    // reset bound indices
    t.reset_bounded();
 
    bind_upper_to_Lower(t, bounded);

    int numbound = static_cast<int>(bounded.size());

    t.print_indices();

    if (numbound == 0 || t.dim < 2) {
        return;
    }

    std::wcout << " + ";

    inc_index_and_print(t, bounded, 0);

    std::wcout << std::endl;

}

/// <summary>
/// M   
/// </summary>
/// <param name="name"></param>
/// <param name="rank"></param>
/// <param name="tiv"></param>
/// <returns></returns>
std::shared_ptr<Tensor> Tensor::make_tensor(TensorConfig tc)
{
    return std::make_shared<Tensor>(tc);
}

/// <summary>
/// 
/// </summary>
/// <param name="name"></param>
/// <returns></returns>
std::shared_ptr<Tensor> Tensor::make_vector(char name, char idxname)
{
    return Tensor::make_tensor({ name,{0,1},{idxname} });
}

std::shared_ptr<Tensor> Tensor::make_covector(char name, char idxname)
{
    return Tensor::make_tensor({ name,{1,0},{idxname} });
}

/// <summary>
/// 
/// </summary>
/// <param name="name"></param>
/// <param name="tv"></param>
/// <returns></returns>
std::shared_ptr<Tensor> Tensor::tensor_product(char name, TensorVec tv)
{
    Rank totrank(0,0);
    TensorIndexVec tiv;

    // do covectors first
    for (TensorPtr t : tv) {
        totrank.p += t->rank.p;

        for (int cp = 0; cp < t->rank.p;cp++) {
            TensorIndex& ti = t->getIndices()[cp];
            tiv.push_back({ ti.columnName, ti.indexName });
            cp++;
            if (cp == t->rank.p)
                break;
        }
    }

    // do vectors next
    for (TensorPtr t : tv) {
        totrank.q += t->rank.q;

        for (int cp = t->rank.p; cp < t->rank.total();cp++) {
            TensorIndex& ti = t->getIndices()[cp];
            tiv.push_back({ ti.columnName, ti.indexName });
            cp++;
            if (cp == t->rank.p)
                break;
        }
    }

    return Tensor::make_tensor({ name,totrank,tiv });
}


/// <summary>
/// Recursively increments tensor indices based on a list of bounded index names and prints the indices for each step.
/// </summary>
/// <param name="v">The first tensor whose indices will be incremented and printed.</param>
/// <param name="w">The second tensor whose indices will be incremented and printed if it is not the same as v.</param>
/// <param name="bounded">A vector of characters representing the names of the bounded indices to iterate over.</param>
/// <param name="cidx">The current index in the bounded vector being processed.</param>
void Tensor::inc_index_and_print(Tensor& t, std::vector<char> bounded, size_t cidx)
{
    char iname = bounded[cidx];


    if (cidx < bounded.size() - 1) {
        inc_index_and_print(t, bounded, cidx + 1);
    }

    for (int dim_i = 1; dim_i < t.dim; dim_i++) {

        t.inc_index(iname);        
        t.print_indices();
        
        if (!t.indices_at_eof())
            std::wcout << " + ";

        if (cidx < bounded.size() - 1) {
            inc_index_and_print(t, bounded, cidx + 1);
        }

    }

    t.reset_index(iname);
 
}

/// <summary>
/// Resets all indices of the tensor to their initial state.
/// This is necessary after a contraction operation to reset index values
/// </summary>
void Tensor::reset_bounded()
{
    for (int i = 0;i < rank.total();i++)
        indices[i].reset_bounded();
}

/// <summary>
/// Resets the value of all tensor indices with the specified name.
/// </summary>
/// <param name="idxname">The name of the index to reset.</param>
void Tensor::reset_index(char idxname)
{
    for (int i = 0;i < rank.total();i++) {
        if (indices[i].indexName == idxname) {
            indices[i].reset_value();
        }
    }
}


void Tensor::inc_index(char idxname)
{
    for (int i = 0;i < rank.total();i++) {
        if (indices[i].indexName == idxname) {
            indices[i].increment_value();
        }
    }
}

/// <summary>
/// Prints the tensor's name and, if applicable, its upper indices to the standard output.
/// </summary>
void Tensor::print_indices()
{
    wchar_t tname = '\0';
    
    if (rank.p > 0) {
        for (int i = 0;i < rank.p;i++) {
            if (indices[i].columnName != tname) {
                tname = indices[i].columnName;
                std::wcout << tname;
            }
            std::wcout << char_to_superscript(indices[i].get_char_value());
        }
    }

    if (rank.q > 0) {
        for (int i = rank.p;i < rank.total();i++) {
            if (indices[i].columnName != tname) {
                tname = indices[i].columnName;
                std::wcout << tname;
            }
            std::wcout << char_to_subscript(indices[i].get_char_value());
        }
    }

    //std::wcout << std::endl;
}

/// <summary>
/// Checks if this tensor and another tensor have the same name.
/// </summary>
/// <param name="other">The tensor to compare with this tensor.</param>
/// <returns>true if both tensors have the same name; otherwise, false.</returns>
bool Tensor::same_tensor(Tensor& other) const
{
    return (tensorName == other.tensorName);
}


bool Tensor::indices_at_eof() const
{
    bool ret = true;

    for (int i = 0; i < rank.total();i++) {
        if (!indices[i].eof()) {
            ret = false;
            break;
        }

    }

    return ret;
}

/// <summary>
/// Returns a vector of tensor indices that match the specified name character.
/// </summary>
/// <param name="idxname">The character representing the name of the indices to search for.</param>
/// <returns>A vector containing all tensor indices with the specified name. Returns an empty vector if no matches are found.</returns>
std::vector<TensorIndex*> Tensor::get_indices_by_name(char idxname)
{
    std::vector<TensorIndex*> ret;

    for (int i = 0; i < rank.total();i++) {
        if (indices[i].indexName == idxname)
            ret.push_back(&indices[i]);
    }

    return ret;
}

void Tensor::bind_upper_to_Lower(Tensor& t, std::vector<char>& bounded)
{
    //  set bound indices
    for (int i = 0;i < t.rank.p;i++) {
        TensorIndex& ctv = t.indices[i];
        bool fndmatch = false;

        for (int j = t.rank.p;j < t.rank.total();j++) {
            TensorIndex& cov = t.indices[j];

            if (ctv.indexName == cov.indexName) {
                fndmatch = true;
                cov.bind();
            }
        }

        if (fndmatch) {
            ctv.bind();
            bounded.push_back(ctv.indexName);
        }
    }
}


/// <summary>
/// 
/// </summary>
/// <param name="ss"></param>
/// <param name="obj"></param>
/// <returns></returns>
std::ostream& operator << (std::ostream& ss, const Tensor& t) {
    int rp = t.rank.p;
    int rq = t.rank.q;
    int rpq = rp + rq;

    ss << "Tensor[name=" << t.get_name() << ",rank(" << rp << "," << rq << ")";
    ss << ",dim=" << Space::get_space()->get_dim();

 
    if (rp > 0) {
        ss << "\n  upper(";
        for (int i = 0;i < rp;i++) {
            ss << t.indices[i];
            if (i<(rp-1))
                ss << ",";
        }
        ss << ")";
        if (rp > 0)
            ss << ",";
    }

    if (rq > 0) {
        ss << "\n  lower(";
        for (int i = rp;i < rpq;i++) {
            ss << t.indices[i];
            if (i < (rpq - 1))
                ss << ",";
        }
        ss << ")";
    }

    ss << "]" << std::endl;
    return ss;
}
