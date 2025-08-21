#include "TensorIndex.h"
#include "EnoteException.h"
#include <sstream>

/// <summary>
/// 
/// </summary>
/// <param name="id"></param>
/// <param name="name"></param>
/// <param name="parent"></param>
TensorIndex::TensorIndex() : name('\0'), value(-1), dim(1) {}

/// <summary>
/// 
/// </summary>
void TensorIndex::reset_bounded() {
    value = -1;
}

void TensorIndex::reset_value() {
    if (is_bounded())
        value = 1;
}

bool TensorIndex::eof() const {
    return (!is_bounded() || value==dim);
}


/// <summary>
/// 
/// </summary>
/// <returns></returns>
char TensorIndex::get_char_value() const {
    if (value < 0)
        return name;

    return '0' + value;
}

/// <summary>
/// 
/// </summary>
void TensorIndex::bind()
{
    value = 1;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
bool TensorIndex::is_bounded() const
{
    return value > -1;
}

/// <summary>
/// 
/// </summary>
void TensorIndex::increment_value()
{
    if (is_bounded())
        value++;

    if (value>dim) {
        std::stringstream ss;
        ss << "TensorIndex::increment_value: Value exceeds dimenstion. val=" << value << " dim=" << dim;
 
        throw EnoteException(ss.str());
    }

}

/// <summary>
/// 
/// </summary>
/// <param name="os"></param>
/// <param name="obj"></param>
/// <returns></returns>
std::ostream& operator << (std::ostream& os, const TensorIndex& ti) {
    os << "TensorIndex[name=" << ti.name;

    if (ti.is_bounded())
        os << ",val=" << ti.get_char_value();

    os << "]";
    return os;
}