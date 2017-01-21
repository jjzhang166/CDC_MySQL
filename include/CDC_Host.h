#ifndef _CDC_Host_H_
#define _CDC_Host_H_

#include "data_def.h"
#include "ComDef.h"
#include "CppMySQL.h"
#include <list>

class CDC_MYSQL_API CDC_Host
{
public:
	CDC_Host(CppMySQLDB& db);
	~CDC_Host();

	std::string CDC_Host_add(const std::string& req);
	std::string CDC_Host_del(const std::string& req);
	std::string CDC_Host_update(const std::string& req);
	std::string CDC_Host_find(const std::string& req);
	std::string CDC_Host_FindCount(const std::string& req);
	////////////////////////////////////////////

	double Host_add(TCDC_Host&);

	int Host_del(double id);
	int Host_del(const std::string& name);

	int Host_update(TCDC_Host&);
	int Host_update(TCDC_Host& src, std::list<std::string>& keyList);

	bool Host_find(double id);
	bool Host_find(const std::string& name);
	int Host_find(double id, TCDC_Host& t);
	int Host_find(const std::string& name, TCDC_Host& t);
	int Host_find2(const std::string& whereSql, TCDC_Host& t);
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

