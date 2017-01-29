#ifndef _CDC_Host_H_
#define _CDC_Host_H_

#include "data_def.h"
#include "ComDef.h"
#include "CppMySQL.h"
#include <list>

class CDC_MYSQL_API CDC_Host
{
	DISABLE_COPY(CDC_Host);
public:
	CDC_Host(CppMySQLDB& db);
	~CDC_Host();

	std::string CDC_Host_Add(const std::string& req);
	std::string CDC_Host_Del(const std::string& req);
	std::string CDC_Host_Update(const std::string& req);
	std::string CDC_Host_Find(const std::string& req);
	std::string CDC_Host_FindCount(const std::string& req);
	////////////////////////////////////////////

	double Host_Add(TCDC_Host&);

	int Host_Del(double id);
	int Host_Del(const std::string& name);

	int Host_Update(TCDC_Host&);
	int Host_Update(TCDC_Host& src, std::list<std::string>& keyList);

	bool Host_Find(double id);
	bool Host_Find(const std::string& name);
	int Host_Find(double id, TCDC_Host& t);
	int Host_Find(const std::string& name, TCDC_Host& t);
	int Host_Find2(const std::string& whereSql, TCDC_Host& t);
	int Host_Count();
	int Host_Count(const std::string& whereSql);

private:
	double GetMaxID();
	std::list<TCDC_Host> GetAll();

private:
	CppMySQLDB _db;
	CppMySQLStatement _stmt;
};

#endif

