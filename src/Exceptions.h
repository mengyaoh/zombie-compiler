#pragma once
#include <exception>

class InvalidOp : public std::exception
{
public:
	const char* what() const noexcept override { return "Invalid OP code or parameters."; }
};

class FileLoadException : public std::exception
{
public:
    const char* what() const noexcept override { return "error loading file"; }
};

class RangeAttackException : public std::exception
{
public:
    const char* what() const noexcept override { return "Zombie can not do range attack."; }
};

class MachineLoadException : public std::exception
{
public:
    const char* what() const noexcept override { return "please Load Machine for both Human & Zombie."; }
};

class LoopOperationException : public std::exception
{
public:
    const char* what() const noexcept override { return "please Load Machine for both Human & Zombie."; }
};

