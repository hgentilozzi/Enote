#include "TensorIndex.h"
#include "EnoteException.h"
#include <sstream>

const int NOT_BOUND_VALUE = -1;

TensorIndex::TensorIndex(char indexName)
{
    init('\0', indexName);
}

TensorIndex::TensorIndex(char columnName, char indexName)
{
    init(columnName, indexName);
}

void TensorIndex::init(char columnName, char indexName)
{
    SpacePtr s = Space::get_space();
    dim = s->get_dim();
    zeroBased = s->get_zero_based();
    this->columnName = columnName;
    this->indexName = indexName;
}


void TensorIndex::reset_bounded() {
    value = NOT_BOUND_VALUE;
}

void TensorIndex::reset_value() {
    if (is_bounded())
        bind(); // rebind resets value
}

bool TensorIndex::eof() const {
    return (!is_bounded() || one_based_value()==dim);
}

int TensorIndex::one_based_value() const
{
    return (zeroBased) ? value + 1 : value;
}


char TensorIndex::get_char_value() const {
    if (value == NOT_BOUND_VALUE)
        return indexName;

    return '0' + value;
}

void TensorIndex::bind()
{
    value = (zeroBased) ? 0 : 1;
}

bool TensorIndex::is_bounded() const
{
    return value != NOT_BOUND_VALUE;
}

/// <summary>
/// Increment the index value. 
/// Throw and exception if index value exceeds dimention
/// </summary>
void TensorIndex::increment_value()
{
    if (is_bounded())
        value++;

    if (one_based_value() >dim) {
        std::stringstream ss;
        ss << "TensorIndex::increment_value: Value exceeds dimenstion. val=" << value << " dim=" << dim;
        throw EnoteException(ss.str());
    }

}

/// <summary>
/// Print the object
/// </summary>
/// <param name="os"></param>
/// <param name="ti"></param>
/// <returns></returns>
std::ostream& operator << (std::ostream& os, const TensorIndex& ti) {
    os << "TensorIndex[name=" << ti.indexName;

    if (ti.is_bounded())
        os << ",val=" << ti.get_char_value();

    os << "]";
    return os;
}