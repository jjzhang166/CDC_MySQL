#ifndef _CDC_UserGroup_H_
#define _CDC_UserGroup_H_

#include "data_def.h"
#include "ComDef.h"
#include "CppMySQL.h"

#include <list>


class CDC_MYSQL_API CDC_UserGroup
{
public:
	CDC_UserGroup(CppMySQLDB& db);
	~CDC_UserGroup();

	std::string CDC_UserGroup_add(const std::string& req);
	std::string CDC_UserGroup_del(const std::string& req);
	std::string CDC_UserGroup_update(const std::string& req);
	std::string CDC_UserGroup_find(const std::string& req);
	std::string CDC_UserGroup_FindCount(const std::string& req);
	////////////////////////////////////////////
	
	double UserGroup_add(TCDC_UserGroup&);

	int UserGroup_del(double id);
	int UserGroup_del(std::string& name);
	
	int UserGroup_update(TCDC_UserGroup&);
	
	bool UserGroup_find(double id);
	bool UserGroup_find(std::string& name);
	int UserGroup_find(double id, TCDC_UserGroup& t);
	int UserGroup_find(std::string& name, TCDC_UserGroup& t);
	int UserGroup_find2(std::string& whereSql, TCDC_UserGroup& t);

	int UserGroup_Count();
	int UserGroup_Count(std::string& whereSql);

private:
	double GetMaxID();
	std::list<TCDC_UserGroup> GetAll();

private:
	CppMySQLDB _db;
	CppMySQLStatement _stmt;
};

#endif

