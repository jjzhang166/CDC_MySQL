#ifndef COM_DEF_H
#define COM_DEF_H

#if defined(WIN32)
#ifdef CDC_MYSQL_EXPORTS
#define CDC_MYSQL_API __declspec(dllexport)
#else
#define CDC_MYSQL_API __declspec(dllimport)
#endif
#else
#define CDC_MYSQL_API 
#endif

enum DBCode
{
	failed = 0,
	success = 1,
	notExists = -1,
	exists = -2,
	DBError = -3,
	notFind = -4,
	inputConditionError = -5
};


#define DISABLE_COPY(Class) \
	Class(const Class &); \
	Class &operator=(const Class &)

#endif