/**

@brief	Declares the utility class.
 */

#ifndef _COMMON_FUNC_INCLUDE_
#define _COMMON_FUNC_INCLUDE_

#include <cstring>
#include <string>

#define DISABLE_COPY(Class) \
	Class(const Class &); \
	Class &operator=(const Class &)

std::string ToLower(std::string str);

std::string Trim(std::string s);
std::string RTrim(std::string s);
std::string LTrim(std::string s);

#endif