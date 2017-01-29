#ifndef _CDC_ISO_H_
#define _CDC_ISO_H_

#include "data_def.h"
#include "ComDef.h"
#include "CppMySQL.h"
#include <list>

class CDC_MYSQL_API CDC_ISO
{
	DISABLE_COPY(CDC_ISO);
public:
	CDC_ISO(CppMySQLDB& db);
	~CDC_ISO();

	std::string CDC_ISO_Add(const std::string& req);
	std::string CDC_ISO_Del(const std::string& req);
	std::string CDC_ISO_Update(const std::string& req);
	std::string CDC_ISO_Find(const std::string& req);
	std::string CDC_ISO_FindCount(const std::string& req);
	////////////////////////////////////////////

	double ISO_Add(TCDC_ISO&);

	int ISO_Del(double id);
	int ISO_Del(const std::string& name);

	int ISO_Update(TCDC_ISO&);
	int ISO_Update(TCDC_ISO& src, std::list<std::string>& keyList);

	bool ISO_Find(double id);
	bool ISO_Find(const std::string& name);
	int ISO_Find(double id, TCDC_ISO& t);
	int ISO_Find(const std::string& name, TCDC_ISO& t);
	int ISO_Find2(const std::string& whereSql, TCDC_ISO& t);
	int ISO_Count();
	int ISO_Count(const std::string& whereSql);

private:
	double GetMaxID();
	std::list<TCDC_ISO> GetAll();

private:
	CppMySQLDB _db;
	CppMySQLStatement _stmt;
};

#endif

