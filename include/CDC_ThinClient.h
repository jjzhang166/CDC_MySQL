#ifndef _CDC_ThinClient_H_
#define _CDC_ThinClient_H_

#include "data_def.h"
#include "ComDef.h"
#include "CppMySQL.h"
#include <list>

class CDC_MYSQL_API CDC_ThinClient
{
	DISABLE_COPY(CDC_ThinClient);
public:
	CDC_ThinClient(CppMySQLDB& db);
	~CDC_ThinClient();

	std::string CDC_ThinClient_Add(const std::string& req);
	std::string CDC_ThinClient_Del(const std::string& req);
	std::string CDC_ThinClient_Update(const std::string& req);
	std::string CDC_ThinClient_Find(const std::string& req);
	std::string CDC_ThinClient_FindCount(const std::string& req);
	////////////////////////////////////////////

	double ThinClient_Add(TCDC_ThinClient&);

	int ThinClient_Del(double id);

	int ThinClient_Update(TCDC_ThinClient&);
	int ThinClient_Update(TCDC_ThinClient& src, std::list<std::string>& keyList);

	bool ThinClient_Find(double id);
	int ThinClient_Find(double id, TCDC_ThinClient& t);
	int ThinClient_Find2(const std::string& whereSql, TCDC_ThinClient& t);
	int ThinClient_Count();
	int ThinClient_Count(const std::string& whereSql);

private:
	double GetMaxID();
	std::list<TCDC_ThinClient> GetAll();

private:
	CppMySQLDB _db;
	CppMySQLStatement _stmt;
};

#endif

