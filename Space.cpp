#include <sstream>
#include "Space.h"
#include "EnoteException.h"

std::shared_ptr<Space> _instance;

/// <summary>
/// Creates and returns a singleton instance of the Space class with the specified name, dimension, and zero-based flag.
/// </summary>
/// <param name="spacename">The name to assign to the Space instance.</param>
/// <param name="dim">The dimensionality of the Space.</param>
/// <param name="zeroBased">Indicates whether the Space uses zero or one based indexing.</param>
/// <returns>A shared pointer to the singleton Space instance.</returns>
std::shared_ptr<Space> Space::make_space(const char* spacename, TensorDimension dim, bool zeroBased)
{
	if (_instance.get() != nullptr) {
        std::stringstream ss;
        ss << "Space::make_instance: A Space is singular and has alread been created. Name=" 
           << _instance.get()->get_name();
        throw EnoteException(ss.str());
    }

    _instance = std::make_shared<Space>(spacename, dim, zeroBased);
	return _instance;
}

/// <summary>
/// Returns the singleton instance of the Space class.
/// </summary>
/// <returns>A shared pointer to the singleton Space instance. Throws an EnoteException if the instance has not been created yet.</returns>
SpacePtr Space::get_space()
{
    if (_instance.get() == nullptr) {
        throw EnoteException("Space::get_instance: A Space has not been created yet");
    }

    return _instance;
}

/// <summary>
///  Create a Space object
/// </summary>
/// <param name="spacename"></param>
/// <param name="dim"></param>
/// <param name="zeroBased"></param>
Space::Space(const char* spacename, TensorDimension dim, bool zeroBased)
    : name(spacename), dim(dim), zeroBased(zeroBased)
{
}
