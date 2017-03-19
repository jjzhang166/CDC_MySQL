#ifndef _CDC_VM_H_
#define _CDC_VM_H_

#include "data_def.h"
#include "ComDef.h"
#include "CppMySQL.h"
#include <list>

class CDC_MYSQL_API CDC_VM
{
	DISABLE_COPY(CDC_VM);
public:
	CDC_VM();
	~CDC_VM();

	std::string CDC_VM_Add(const std::string& req);
	std::string CDC_VM_Del(const std::string& req);
	std::string CDC_VM_Update(const std::string& req);
	std::string CDC_VM_Find(const std::string& req);
	std::string CDC_VM_FindCount(const std::string& req);
	////////////////////////////////////////////

	double VM_Add(TCDC_VM&);

	int VM_Del(double id);

	int VM_Update(TCDC_VM& src, std::list<std::string>& keyList);

	bool VM_Find(double id);
	int VM_Find(double id, TCDC_VM& t);
	int VM_Find2(const std::string& whereSql, TCDC_VM& t);
	std::list<TCDC_VM> VM_Find2(const std::string& whereSql);
	int VM_Count();
	int VM_Count(const std::string& whereSql);

private:
	double GetMaxID();
	std::list<TCDC_VM> GetAll();

private:
	CppMySQLDB* _pdb;
	CppMySQLStatement _stmt;
};

#endif

