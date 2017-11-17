#pragma once
#include <exception>

class InvalidOp : public std::exception
{
	const char* what() const noexcept override { return "Invalid OP code or parameters."; }
};

class FileLoadException : public std::exception
{
    const char* what() const noexcept override { return "error loading file"; }
};

