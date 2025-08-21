#pragma once
#include <memory>

typedef int TensorDimension;
class Space;
typedef std::shared_ptr<Space> SpacePtr;



class Space
{
public:
	static SpacePtr make_space(const char* spacename, TensorDimension dim, bool zeroBased);
	static SpacePtr get_space();

	Space(const char* spacename, TensorDimension dim, bool zeroBased);

	TensorDimension get_dim() const { return dim; }
	bool get_zero_based() const { return zeroBased; }
	std::string& get_name() { return name; }

private:

	TensorDimension dim;
	bool zeroBased;
	std::string name;
};

