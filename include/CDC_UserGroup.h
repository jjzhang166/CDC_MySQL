#ifndef _CDC_UserGroup_H_
#define _CDC_UserGroup_H_

#include "data_def.h"
#include "ComDef.h"
#include "CppMySQL.h"

#include <list>


class CDC_MYSQL_API CDC_UserGroup
{
	DISABLE_COPY(CDC_UserGroup);
public:
	CDC_UserGroup(CppMySQLDB* pdb);
	~CDC_UserGroup();

	std::string CDC_UserGroup_Add(const std::string& req);
	std::string CDC_UserGroup_Del(const std::string& req);
	std::string CDC_UserGroup_Update(const std::string& req);
	std::string CDC_UserGroup_Find(const std::string& req);
	std::string CDC_UserGroup_FindCount(const std::string& req);
	////////////////////////////////////////////
	
	double UserGroup_Add(TCDC_UserGroup&);

	int UserGroup_Del(double id);
	int UserGroup_Del(std::string& name);
	
	int UserGroup_Update(TCDC_UserGroup&);
	
	bool UserGroup_Find(double id);
	bool UserGroup_Find(std::string& name);
	int UserGroup_Find(double id, TCDC_UserGroup& t);
	int UserGroup_Find(std::string& name, TCDC_UserGroup& t);
	int UserGroup_Find2(std::string& whereSql, TCDC_UserGroup& t);

	int UserGroup_Count();
	int UserGroup_Count(std::string& whereSql);

private:
	double GetMaxID();
	std::list<TCDC_UserGroup> GetAll();

private:
	CppMySQLDB* _pdb;
	CppMySQLStatement _stmt;
};

#endif

