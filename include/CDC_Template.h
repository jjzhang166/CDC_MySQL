#ifndef _CDC_Template_H_
#define _CDC_Template_H_

#include "data_def.h"
#include "ComDef.h"
#include "CppMySQL.h"
#include <list>

class CDC_MYSQL_API CDC_Template
{
	DISABLE_COPY(CDC_Template);
public:
	CDC_Template(CppMySQLDB* pdb);
	~CDC_Template();

	std::string CDC_Template_Add(const std::string& req);
	std::string CDC_Template_Del(const std::string& req);
	std::string CDC_Template_Update(const std::string& req);
	std::string CDC_Template_Find(const std::string& req);
	std::string CDC_Template_FindCount(const std::string& req);
	////////////////////////////////////////////

	double Template_Add(TCDC_Template&);

	int Template_Del(double id);

	int Template_Update(TCDC_Template& src, std::list<std::string>& keyList);

	bool Template_Find(double id);
	int Template_Find(double id, TCDC_Template& t);
	int Template_Find2(const std::string& whereSql, TCDC_Template& t);
	std::list<TCDC_Template> Template_Find2(const std::string& whereSql);
	int Template_Count();
	int Template_Count(const std::string& whereSql);

private:
	double GetMaxID();
	std::list<TCDC_Template> GetAll();

private:
	CppMySQLDB* _pdb;
	CppMySQLStatement _stmt;
};

#endif

