
#ifndef _CDC_MySQL_H_
#define _CDC_MySQL_H_

#include "CppMySQL.h"

#if defined(WIN32)
#ifdef CDC_MYSQL_EXPORTS
#define CDC_MYSQL_API __declspec(dllexport)
#else
#define CDC_MYSQL_API __declspec(dllimport)
#endif
#else
#define CDC_MYSQL_API 
#endif

#include <string>

enum DBCode
{
	failed = 0,
	success = 1,
	notExists = -1,
	exists = -2,
	DBError = -3,
	notFind = -4,
	inputConditionError = -5
};


#define DISABLE_COPY(Class) \
	Class(const Class &); \
	Class &operator=(const Class &)

class CDC_MYSQL_API CDC_UserGroup
{
	DISABLE_COPY(CDC_UserGroup);
public:
	CDC_UserGroup();
	~CDC_UserGroup();

	std::string CDC_UserGroup_Add(const std::string& req);
	std::string CDC_UserGroup_Del(const std::string& req);
	std::string CDC_UserGroup_Update(const std::string& req);
	std::string CDC_UserGroup_Find(const std::string& req);
	std::string CDC_UserGroup_FindCount(const std::string& req);

private:
	CppMySQLDB* _pdb;
	CppMySQLStatement _stmt;
};

class CDC_MYSQL_API CDC_User
{
    DISABLE_COPY(CDC_User);
public:
	CDC_User();
	~CDC_User();

	std::string CDC_User_Add(const std::string& req);
	std::string CDC_User_Del(const std::string& req);
	std::string CDC_User_Update(const std::string& req);
	std::string CDC_User_Find(const std::string& req);
	std::string CDC_User_FindCount(const std::string& req);

private:
	CppMySQLDB* _pdb;
	CppMySQLStatement _stmt;
};

class CDC_MYSQL_API CDC_Host
{
    DISABLE_COPY(CDC_Host);
public:
	CDC_Host();
	~CDC_Host();

	std::string CDC_Host_Add(const std::string& req);
	std::string CDC_Host_Del(const std::string& req);
	std::string CDC_Host_Update(const std::string& req);
	std::string CDC_Host_Find(const std::string& req);
	std::string CDC_Host_FindCount(const std::string& req);

private:
	CppMySQLDB* _pdb;
	CppMySQLStatement _stmt;
};

class CDC_MYSQL_API CDC_Template
{
    DISABLE_COPY(CDC_Template);
public:
	CDC_Template();
	~CDC_Template();

	std::string CDC_Template_Add(const std::string& req);
	std::string CDC_Template_Del(const std::string& req);
	std::string CDC_Template_Update(const std::string& req);
	std::string CDC_Template_Find(const std::string& req);
	std::string CDC_Template_FindCount(const std::string& req);

private:
	CppMySQLDB* _pdb;
	CppMySQLStatement _stmt;
};

class CDC_MYSQL_API CDC_ISO
{
    DISABLE_COPY(CDC_ISO);
public:
	CDC_ISO();
	~CDC_ISO();

	std::string CDC_ISO_Add(const std::string& req);
	std::string CDC_ISO_Del(const std::string& req);
	std::string CDC_ISO_Update(const std::string& req);
	std::string CDC_ISO_Find(const std::string& req);
	std::string CDC_ISO_FindCount(const std::string& req);

private:
	CppMySQLDB* _pdb;
	CppMySQLStatement _stmt;
};

class CDC_MYSQL_API CDC_ThinClientGroup
{
    DISABLE_COPY(CDC_ThinClientGroup);
public:
	CDC_ThinClientGroup();
	~CDC_ThinClientGroup();

	std::string CDC_ThinClientGroup_Add(const std::string& req);
	std::string CDC_ThinClientGroup_Del(const std::string& req);
	std::string CDC_ThinClientGroup_Update(const std::string& req);
	std::string CDC_ThinClientGroup_Find(const std::string& req);
	std::string CDC_ThinClientGroup_FindCount(const std::string& req);

private:
	CppMySQLDB* _pdb;
	CppMySQLStatement _stmt;
};

class CDC_MYSQL_API CDC_ThinClient
{
    DISABLE_COPY(CDC_ThinClient);
public:
	CDC_ThinClient();
	~CDC_ThinClient();

	std::string CDC_ThinClient_Add(const std::string& req);
	std::string CDC_ThinClient_Del(const std::string& req);
	std::string CDC_ThinClient_Update(const std::string& req);
	std::string CDC_ThinClient_Find(const std::string& req);
	std::string CDC_ThinClient_FindCount(const std::string& req);

private:
	CppMySQLDB* _pdb;
	CppMySQLStatement _stmt;
};

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

private:
	CppMySQLDB* _pdb;
	CppMySQLStatement _stmt;
};

class CDC_MYSQL_API CDC_EDU_VM
{
    DISABLE_COPY(CDC_EDU_VM);
public:
	CDC_EDU_VM();
	~CDC_EDU_VM();

	std::string CDC_EDU_VM_Add(const std::string& req);
	std::string CDC_EDU_VM_Del(const std::string& req);
	std::string CDC_EDU_VM_Update(const std::string& req);
	std::string CDC_EDU_VM_Find(const std::string& req);
	std::string CDC_EDU_VM_FindCount(const std::string& req);

private:
	CppMySQLDB* _pdb;
	CppMySQLStatement _stmt;
};

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

private:
	CppMySQLDB* _pdb;
	CppMySQLStatement _stmt;
};

#endif