#ifndef _CDC_Userr_H_
#define _CDC_Userr_H_

#include "data_def.h"
#include "ComDef.h"
#include "CppMySQL.h"
#include <list>


class CDC_MYSQL_API CDC_User
{
public:

	std::string CDC_User_add(const std::string& req);
	std::string CDC_User_del(const std::string& req);
	std::string CDC_User_update(const std::string& req);
	std::string CDC_User_find(const std::string& req);
	std::string CDC_User_FindCount(const std::string& req);

	////////////////////////////////////////////

	CDC_User(CppMySQLDB& db);
	~CDC_User();
	double User_add(TCDC_User&);

	int User_del(double id);
	int User_del(const std::string& name);

	int User_update(TCDC_User&);
	int User_update(TCDC_User& src, std::list<std::string>& keyList);

	bool User_find(double id);
	bool User_find(const std::string& name);
	int User_find(double id, TCDC_User& t);
	int User_find(const std::string& name, TCDC_User& t);
	int User_find2(const std::string& whereSql, TCDC_User& t);

	int User_Count();
	int User_Count(const std::string& whereSql);
private:
	double GetMaxID();
	std::list<TCDC_User> GetAll();

private:
	CppMySQLDB _db;
	CppMySQLStatement _stmt;
};

#endif

