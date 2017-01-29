#ifndef _CDC_Userr_H_
#define _CDC_Userr_H_

#include "data_def.h"
#include "ComDef.h"
#include "CppMySQL.h"
#include <list>


class CDC_MYSQL_API CDC_User
{
	DISABLE_COPY(CDC_User);
public:

	std::string CDC_User_Add(const std::string& req);
	std::string CDC_User_Del(const std::string& req);
	std::string CDC_User_Update(const std::string& req);
	std::string CDC_User_Find(const std::string& req);
	std::string CDC_User_FindCount(const std::string& req);

	////////////////////////////////////////////

	CDC_User(CppMySQLDB* pdb);
	~CDC_User();
	double User_Add(TCDC_User&);

	int User_Del(double id);
	int User_Del(const std::string& name);

	int User_Update(TCDC_User&);
	int User_Update(TCDC_User& src, std::list<std::string>& keyList);

	bool User_Find(double id);
	bool User_Find(const std::string& name);
	int User_Find(double id, TCDC_User& t);
	int User_Find(const std::string& name, TCDC_User& t);
	int User_Find2(const std::string& whereSql, TCDC_User& t);

	int User_Count();
	int User_Count(const std::string& whereSql);
private:
	double GetMaxID();
	std::list<TCDC_User> GetAll();

private:
	CppMySQLDB* _pdb;
	CppMySQLStatement _stmt;
};

#endif

