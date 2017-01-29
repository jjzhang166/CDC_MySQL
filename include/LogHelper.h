#ifndef _LOGHELPER_H
#define _LOGHELPER_H

#include "ComDef.h"

#include <sstream>
#include <iostream>

#define ENABLE_DEBUG false
#define ENABLE_INFO true

class CLogInfo
{
	DISABLE_COPY(CLogInfo);
public:
	CLogInfo(){}
	~CLogInfo()
	{
		if (ENABLE_INFO)
			std::cout << "INFO-" << _stream.str().c_str();
	}
	std::ostream& stream() { return _stream; }
private:
	std::stringstream _stream;
};

class CLogDebug
{
	DISABLE_COPY(CLogDebug);
public:
	CLogDebug(){}
	~CLogDebug()
	{
		if (ENABLE_DEBUG)
			std::cout << "DEBUG-" << _stream.str().c_str();
	}
	std::ostream& stream() { return _stream; }
private:
	std::stringstream _stream;
};

class CLogError
{
	DISABLE_COPY(CLogError);
public:
	CLogError(){}
	~CLogError()
	{
		std::cerr << "ERROR-" << _stream.str().c_str();
	}
	std::ostream& stream() { return _stream; }
private:
	std::stringstream _stream;
};


#define	MINFO	CLogInfo().stream()
#define MDEBUG  CLogDebug().stream()
#define MERR	CLogError().stream()

#endif