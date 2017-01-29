
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

std::vector<std::string> Split(const std::string &s, const std::string &seperator)
{
	std::vector<std::string> result;
	typedef std::string::size_type string_size;
	string_size i = 0;

	while (i != s.size())
	{
		//找到字符串中首个不等于分隔符的字母；
		int flag = 0;
		while (i != s.size() && flag == 0)
		{
			flag = 1;
			for (string_size x = 0; x < seperator.size(); ++x)
				if (s[i] == seperator[x])
				{
					++i;
					flag = 0;
					break;
				}
		}

		//找到又一个分隔符，将两个分隔符之间的字符串取出；
		flag = 0;
		string_size j = i;
		while (j != s.size() && flag == 0)
		{
			for (string_size x = 0; x < seperator.size(); ++x)
				if (s[j] == seperator[x])
				{
					flag = 1;
					break;
				}
			if (flag == 0)
				++j;
		}
		if (i != j)
		{
			result.push_back(s.substr(i, j - i));
			i = j;
		}
	}
	return result;
}

// 2017-12-31 12:56:30
MYSQL_TIME String2MySQLTime(const std::string& s)
{
	MYSQL_TIME mt = { 0 };
	try
	{
		std::vector<std::string> vec = Split(s, " -:");
		mt.year = atoi(vec[0].c_str());
		mt.month = atoi(vec[1].c_str());
		mt.day = atoi(vec[2].c_str());
		mt.hour = atoi(vec[3].c_str());
		mt.minute = atoi(vec[4].c_str());
		mt.second = atoi(vec[5].c_str());

		mt.time_type = MYSQL_TIMESTAMP_DATETIME;
	}
	catch (std::exception)
	{
		return mt;
	}
	return mt;
}