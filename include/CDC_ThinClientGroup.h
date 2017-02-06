#ifndef _CDC_ThinClientGroup_H_
#define _CDC_ThinClientGroup_H_

#include "data_def.h"
#include "ComDef.h"
#include "CppMySQL.h"

#include <list>


class CDC_MYSQL_API CDC_ThinClientGroup
{
	DISABLE_COPY(CDC_ThinClientGroup);
public:
	CDC_ThinClientGroup(CppMySQLDB* pdb);
	~CDC_ThinClientGroup();

	std::string CDC_ThinClientGroup_Add(const std::string& req);
	std::string CDC_ThinClientGroup_Del(const std::string& req);
	std::string CDC_ThinClientGroup_Update(const std::string& req);
	std::string CDC_ThinClientGroup_Find(const std::string& req);
	std::string CDC_ThinClientGroup_FindCount(const std::string& req);
	////////////////////////////////////////////
	
	double ThinClientGroup_Add(TCDC_ThinClientGroup&);

	int ThinClientGroup_Del(double id);
	
	int ThinClientGroup_Update(TCDC_ThinClientGroup&);
	
	bool ThinClientGroup_Find(double id);
	int ThinClientGroup_Find(double id, TCDC_ThinClientGroup& t);
	int ThinClientGroup_Find2(std::string& whereSql, TCDC_ThinClientGroup& t);
	std::list<TCDC_ThinClientGroup> ThinClientGroup_Find2(std::string& whereSql);

	int ThinClientGroup_Count();
	int ThinClientGroup_Count(std::string& whereSql);

private:
	double GetMaxID();
	std::list<TCDC_ThinClientGroup> GetAll();

private:
	CppMySQLDB* _pdb;
	CppMySQLStatement _stmt;
};

#endif

