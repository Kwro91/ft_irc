#ifndef UTILS_HPP
#define UTILS_HPP

#include "Define.hpp"

size_t stringToSizeT(const std::string &value);

template <typename T>
std::string to_string(T value)
{
	std::stringstream ss;
	ss << value;
	return ss.str();
}

#endif

