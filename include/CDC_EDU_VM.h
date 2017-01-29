#ifndef _CDC_EDU_VM_H_
#define _CDC_EDU_VM_H_

#include "data_def.h"
#include "ComDef.h"
#include "CppMySQL.h"
#include <list>

class CDC_MYSQL_API CDC_EDU_VM
{
	DISABLE_COPY(CDC_EDU_VM);
public:
	CDC_EDU_VM(CppMySQLDB& db);
	~CDC_EDU_VM();

	std::string CDC_EDU_VM_Add(const std::string& req);
	std::string CDC_EDU_VM_Del(const std::string& req);
	std::string CDC_EDU_VM_Update(const std::string& req);
	std::string CDC_EDU_VM_Find(const std::string& req);
	std::string CDC_EDU_VM_FindCount(const std::string& req);
	////////////////////////////////////////////

	double EDU_VM_Add(TCDC_EDU_VM&);

	int EDU_VM_Del(double id);

	int EDU_VM_Update(TCDC_EDU_VM& src, std::list<std::string>& keyList);

	bool EDU_VM_Find(double id);
	int EDU_VM_Find(double id, TCDC_EDU_VM& t);
	int EDU_VM_Find2(const std::string& whereSql, TCDC_EDU_VM& t);
	int EDU_VM_Count();
	int EDU_VM_Count(const std::string& whereSql);

private:
	double GetMaxID();
	std::list<TCDC_EDU_VM> GetAll();

private:
	CppMySQLDB _db;
	CppMySQLStatement _stmt;
};

#endif

