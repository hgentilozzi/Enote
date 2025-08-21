#pragma once
#include <exception>
#include <string>

class EnoteException : public std::exception {
public:
    // Constructor to set a custom message
    EnoteException(const std::string& message) : errorMessage(message) {}

    // Override the what() method to return the custom message
    const char* what() const noexcept override {
        return errorMessage.c_str();
    }

private:
    std::string errorMessage;
};
