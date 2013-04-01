#pragma once

#include <sstream>

class StringConverterHelper
{
public:
	template<typename T>
	static std::string toString(const T &subject)
	{
		std::stringstream ss;
		ss << subject;
		return ss.str();
	}

	template<typename T>
	static T fromString(const std::string &subject)
	{
		std::stringstream ss(subject);
		T target;
		ss >> target;
		return target;
	}
};
