
#include "utility.h"

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <iostream>
#include <functional>
#include <cctype>
#include <algorithm>


std::string ToLower(std::string str)
{
	for (int i = 0; i < str.length(); i++)
	{
		/* Only convert upper-case letters */
		if (str[i] > 64 && str[i] < 91)
			str[i] += 32;
	}
	return str;
}


std::string Trim(std::string s)
{
	return LTrim(RTrim(s));
}

// Trim from start
std::string LTrim(std::string s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

// Trim from end
std::string RTrim(std::string s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

