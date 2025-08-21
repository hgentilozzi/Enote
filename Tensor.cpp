#include <iostream>
#include <string>
#include "Tensor.h"
#include "UcodeLib.h"
#include "EnoteException.h"
#include <sstream>

Tensor::Tensor(const char* name, TensorDimension dim, Rank rank, const char* idxnames)
    : name(name), dim(dim), rank(rank) {
    int numidxs = static_cast<int>(strlen(idxnames));
    
    // index names must name all indices
    if (strlen(idxnames) != (rank.p + rank.q)) {
        std::stringstream ss;
        ss << "Tensor::ctor: Wrong number of index names for rank. T=" << name
            << " rank=(" << rank.p << "," << rank.q << ") index names = \"" << idxnames << "\"";

        throw EnoteException(ss.str());
    }

    // create the indices
    indices = std::make_shared<TensorIndex[]>(numidxs);
    for (int i = 0;i < numidxs;i++) {
        indices[i].name = idxnames[i];
        indices[i].dim = dim;
    }

}

/// <summary>
/// Prints the contraction of the given tensor with itself.
/// </summary>
/// <param name="t">The tensor to be contracted and printed.</param>
void Tensor::print_contraction(Tensor& t) {
    print_contraction(t, t);
}

/// <summary>
/// Prints information about the contraction of two tensors, including validation of their dimensions and types.
/// </summary>
/// <param name="v">The first (contravariant) tensor involved in the contraction.</param>
/// <param name="w">The second (covariant) tensor involved in the contraction.</param>
void Tensor::print_contraction(Tensor& v, Tensor& w)
{
    bool sametensor = v.get_name() == w.get_name();
    int dim = v.dim;
    std::vector<char> bounded;

    //  must be same dimention
    if (!sametensor && v.dim != w.dim) {
        std::stringstream ss;
        ss << "Tensor::contract: Two tensors must have the same dimention to contract. " << v.name << ":dim=" << v.dim << " " << w.name << ":dim=" << w.dim;
        throw EnoteException(ss.str());
    }

    // reset bound indices
    v.reset_bounded();
    if (!sametensor)
        w.reset_bounded();

    bind_upper_to_Lower(v, w, bounded);
    if (!sametensor)
        bind_upper_to_Lower(w, v, bounded);

    int numbound = static_cast<int>(bounded.size());

    v.print_indices();
    if (!sametensor)
        w.print_indices();

    if (numbound == 0 || v.dim < 2) {
        return;
    }

    std::wcout << " + ";

    inc_index_and_print(v, w, bounded, 0);

    std::wcout << std::endl;

}


/// <summary>
/// Recursively increments tensor indices based on a list of bounded index names and prints the indices for each step.
/// </summary>
/// <param name="v">The first tensor whose indices will be incremented and printed.</param>
/// <param name="w">The second tensor whose indices will be incremented and printed if it is not the same as v.</param>
/// <param name="bounded">A vector of characters representing the names of the bounded indices to iterate over.</param>
/// <param name="cidx">The current index in the bounded vector being processed.</param>
void Tensor::inc_index_and_print(Tensor& v, Tensor& w, std::vector<char> bounded, size_t cidx)
{
    bool notsame = !v.same_tensor(w);
    char iname = bounded[cidx];


    if (cidx < bounded.size() - 1) {
        inc_index_and_print(v, w, bounded, cidx + 1);
    }

    for (int dim_i = 1; dim_i < v.dim; dim_i++) {

        //if (cidx < bounded.size() - 1) {
        //    inc_index_and_print(v, w, bounded, cidx + 1);
        //}

        v.inc_index(iname);
        if (notsame)
            w.inc_index(iname);

        v.print_indices();
        if (notsame)
            w.print_indices();
        
        if (!v.indices_at_eof())
            std::wcout << " + ";

        if (cidx < bounded.size() - 1) {
            inc_index_and_print(v, w, bounded, cidx + 1);
        }

    }

    v.reset_index(iname);
    if (notsame)
        w.reset_index(iname);

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
        if (indices[i].name == idxname) {
            indices[i].reset_value();
        }
    }
}


void Tensor::inc_index(char idxname)
{
    for (int i = 0;i < rank.total();i++) {
        if (indices[i].name == idxname) {
            indices[i].increment_value();
        }
    }
}

/// <summary>
/// Prints the tensor's name and, if applicable, its upper indices to the standard output.
/// </summary>
void Tensor::print_indices()
{
    int rp = rank.p;
    int rq = rank.q;
    int rpq = rp + rq;

    std::wcout << std::wstring (name.begin(), name.end());
    
    if (rp > 0) {
        for (int i = 0;i < rp;i++) {
            std::wcout << char_to_superscript(indices[i].get_char_value());
        }
    }

    if (rq > 0) {
//        std::cout << "l(";
        for (int i = rp;i < rpq;i++) {
            std::wcout << char_to_subscript(indices[i].get_char_value());
        }
//        std::cout << ")";
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
    return (name == other.name);
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
        if (indices[i].name == idxname)
            ret.push_back(&indices[i]);
    }

    return ret;
}

void Tensor::bind_upper_to_Lower(Tensor& v, Tensor& w, std::vector<char>& bounded)
{
    bool sametensor = v.get_name() == w.get_name();

    //  set bound indices
    for (int i = 0;i < v.rank.p;i++) {
        TensorIndex& ctv = v.indices[i];
        bool fndmatch = false;

        for (int j = w.rank.p;j < w.rank.total();j++) {
            TensorIndex& cov = w.indices[j];

            if (ctv.name == cov.name) {
                fndmatch = true;
                cov.bind();
            }
        }

        if (fndmatch) {
            ctv.bind();
            bounded.push_back(ctv.name);
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
    ss << ",dim=" << t.dim;

 
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
