#ifndef _CDC_Authorization_H_
#define _CDC_Authorization_H_

#include "data_def.h"
#include "ComDef.h"
#include "CppMySQL.h"
#include <list>

class CDC_MYSQL_API CDC_Authorization
{
	DISABLE_COPY(CDC_Authorization);
public:
	CDC_Authorization();
	~CDC_Authorization();

	std::string CDC_Authorization_Add(const std::string& req);
	std::string CDC_Authorization_Del(const std::string& req);
	std::string CDC_Authorization_Update(const std::string& req);
	std::string CDC_Authorization_Find(const std::string& req);
	std::string CDC_Authorization_FindCount(const std::string& req);
	////////////////////////////////////////////

	std::string Authorization_Add(TCDC_Authorization&);

	int Authorization_Del(const std::string& id);

	int Authorization_Update(TCDC_Authorization&);
	int Authorization_Update(TCDC_Authorization& src, std::list<std::string>& keyList);

	bool Authorization_Find(const std::string& id);
	int Authorization_Find(const std::string& id, TCDC_Authorization& t);
	int Authorization_Find2(const std::string& whereSql, TCDC_Authorization& t);
	std::list<TCDC_Authorization> Authorization_Find2(const std::string& whereSql);
	int Authorization_Count();
	int Authorization_Count(const std::string& whereSql);

private:
	double GetMaxID();
	std::list<TCDC_Authorization> GetAll();

private:
	CppMySQLDB* _pdb;
	CppMySQLStatement _stmt;
};

#endif

