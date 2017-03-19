#include "CDC_User.h"
#include "LogHelper.h"
#include "ComDef.h"
#include "cJSON.h"
#include "utility.h"
#include <assert.h> 
#include <stdio.h>
#include <algorithm>

using namespace std;

CDC_User::CDC_User()
	:_pdb(NULL)
{
}


CDC_User::~CDC_User()
{
	_stmt.clear();
}

std::string CDC_User::CDC_User_Add(const std::string& req)
{
	int ret = 0;
	double id = -1;

	cJSON *json, *tmp;
	json = cJSON_Parse(req.c_str());
	if (!json)
	{
		MERR << "cJSON_Parse Error: " << cJSON_GetErrorPtr();
		goto END;
	}
	else
	{
		tmp = cJSON_GetObjectItem(json, "Method");
		string method;
		if (tmp)
		{
			method = tmp->valuestring;
			assert(ToLower(method) == "part");
		}

#define JSON_ADD_ONE_ELEMENT(X, Y) \
		tmp = cJSON_GetObjectItem(json, #X);\
		if (tmp) t.X = tmp->Y;\
				else goto END;

#define JSON_ADD_ONE_ELEMENT2(X, Y) \
		tmp = cJSON_GetObjectItem(json, #X);\
		if (tmp) t.X = tmp->Y;

		TCDC_User t;
		JSON_ADD_ONE_ELEMENT2(User_ID, valuedouble);
		JSON_ADD_ONE_ELEMENT(User_Role, valueint);
		JSON_ADD_ONE_ELEMENT(User_UserGroup_ID, valuedouble);
		JSON_ADD_ONE_ELEMENT(User_Name, valuestring);
		JSON_ADD_ONE_ELEMENT(User_LoginName, valuestring);
		JSON_ADD_ONE_ELEMENT(User_Password, valuestring);
		JSON_ADD_ONE_ELEMENT(User_Remarks, valuestring);

#undef JSON_ADD_ONE_ELEMENT
#undef JSON_ADD_ONE_ELEMENT2

		id = User_Add(t);
	}

END:
	cJSON_Delete(json);

	ret = (id >= 0) ? 1 : 0;
	cJSON *result = cJSON_CreateObject();
	cJSON_AddNumberToObject(result, "Result", ret);
	if (ret > 0)
		cJSON_AddNumberToObject(result, "User_ID", id);
	char *out = cJSON_Print(result);
	cJSON_Delete(result);

	return string(out);
}

std::string CDC_User::CDC_User_Del(const std::string& req)
{
	int ret = 0;
	cJSON *json, *tmp;
	json = cJSON_Parse(req.c_str());
	if (!json)
	{
		MERR << "cJSON_Parse Error: " << cJSON_GetErrorPtr();
		goto END;
	}
	else
	{
		tmp = cJSON_GetObjectItem(json, "Method");
		string method;
		if (tmp)
		{
			method = tmp->valuestring;
			assert(ToLower(method) == "part");
		}

		double id;
		tmp = cJSON_GetObjectItem(json, "User_ID");
		if (tmp)
			id = tmp->valuedouble;
		else
			goto END;
		ret = User_Del(id);
	}

END:
	cJSON_Delete(json);

	cJSON *result = cJSON_CreateObject();
	cJSON_AddNumberToObject(result, "Result", ret);
	char *out = cJSON_Print(result);
	cJSON_Delete(result);

	return string(out);
}

std::string CDC_User::CDC_User_Update(const std::string& req)
{
	int ret = 0;
	bool hasItem = false;
	cJSON *json, *tmp;
	json = cJSON_Parse(req.c_str());
	if (!json)
	{
		MERR << "cJSON_Parse Error: " << cJSON_GetErrorPtr();
		goto END;
	}
	else
	{
		std::list<string> keyList;

		tmp = cJSON_GetObjectItem(json, "Method");
		string method;
		if (tmp)
		{
			method = tmp->valuestring;
			assert(ToLower(method) == "part");
		}


#define JSON_GET_OBJECT_ITEM(X, Y) \
		tmp = cJSON_GetObjectItem(json, #X);\
		if (tmp){\
			if (!hasItem) hasItem = true;\
			t.X = tmp->Y;\
			keyList.push_back(#X);\
				}

		TCDC_User t;
		const char *key = "User_ID";
		tmp = cJSON_GetObjectItem(json, key);
		if (tmp)
		{
			if (!hasItem) hasItem = true;
			t.User_ID = tmp->valuedouble;
			keyList.push_back(key);
		}
		JSON_GET_OBJECT_ITEM(User_ID, valuedouble);
		JSON_GET_OBJECT_ITEM(User_Role, valueint);
		JSON_GET_OBJECT_ITEM(User_UserGroup_ID, valuedouble);
		JSON_GET_OBJECT_ITEM(User_Name, valuestring);
		JSON_GET_OBJECT_ITEM(User_LoginName, valuestring);
		JSON_GET_OBJECT_ITEM(User_Password, valuestring);
		JSON_GET_OBJECT_ITEM(User_Remarks, valuestring);
#undef JSON_GET_OBJECT_ITEM

		if (!hasItem) goto END;

		ret = User_Update(t, keyList);
	}

END:
	cJSON_Delete(json);

	cJSON *result = cJSON_CreateObject();
	cJSON_AddNumberToObject(result, "Result", ret);
	char *out = cJSON_Print(result);
	cJSON_Delete(result);

	return string(out);
}



std::string CDC_User::CDC_User_Find(const std::string& req)
{
	int ret = 0;
	bool hasItem = false;
	bool isAll = true;
	cJSON *json, *tmp;
	TCDC_User user;
	list<TCDC_User> lst;
	json = cJSON_Parse(req.c_str());
	if (!json)
	{
		MERR << "cJSON_Parse Error: " << cJSON_GetErrorPtr();
		goto END;
	}
	else
	{
		tmp = cJSON_GetObjectItem(json, "Method");
		string method;
		if (tmp)
		{
			method = tmp->valuestring;
			if (ToLower(method) == "part")
				isAll = false;
		}
		else
		{
			goto END;
		}

		if (isAll)
		{
			lst = GetAll();
			if (lst.size() != 0) ret = success;
			goto END;
		}

#define JSON_GET_OBJECT_ITEM(X) \
		tmp = cJSON_GetObjectItem(json, X);\
		if (tmp){\
			if (!hasItem){\
				ss << " " << X << " = ";\
				hasItem = true;\
						}\
					else\
				ss << " AND " << X << " = ";\
				}

		stringstream ss;
		JSON_GET_OBJECT_ITEM("User_ID");
		if (tmp) ss << tmp->valuedouble;

		JSON_GET_OBJECT_ITEM("User_Role");
		if (tmp) ss << tmp->valueint;

		JSON_GET_OBJECT_ITEM("User_UserGroup_ID");
		if (tmp) ss << tmp->valuedouble;

		JSON_GET_OBJECT_ITEM("User_Name");
		if (tmp) ss << "'" << tmp->valuestring << "'";

		JSON_GET_OBJECT_ITEM("User_LoginName");
		if (tmp) ss << "'" << tmp->valuestring << "'";

		JSON_GET_OBJECT_ITEM("User_Password");
		if (tmp) ss << "'" << tmp->valuestring << "'";

		JSON_GET_OBJECT_ITEM("User_Remarks");
		if (tmp) ss << "'" << tmp->valuestring << "'";

#undef JSON_GET_OBJECT_ITEM

		if (!hasItem) goto END;

		string whereSql = ss.str();
		lst = User_Find2(whereSql);
		if (lst.size() != 0) ret = success;
	}

END:
	cJSON_Delete(json);
	char *out = 0;
	cJSON *result = cJSON_CreateObject();
	if (ret != success)
	{
		cJSON_AddNumberToObject(result, "Result", ret);
	}
	else
	{
#define JSON_ADD_ONE_ELEMENT \
		cJSON_AddNumberToObject(data, "User_ID", user.User_ID);\
		cJSON_AddNumberToObject(data, "User_Role", user.User_Role);\
		cJSON_AddNumberToObject(data, "User_UserGroup_ID", user.User_UserGroup_ID);\
		cJSON_AddStringToObject(data, "User_Name", user.User_Name.c_str());\
		cJSON_AddStringToObject(data, "User_LoginName", user.User_LoginName.c_str());\
		cJSON_AddStringToObject(data, "User_Password", user.User_Password.c_str());\
		cJSON_AddStringToObject(data, "User_Remarks", user.User_Remarks.c_str());

		cJSON *dataArr, *data = 0;
		cJSON_AddNumberToObject(result, "Result", lst.size());
		if (lst.size() > 1)
		{
			cJSON_AddItemToObject(result, "Data", dataArr = cJSON_CreateArray());
			for (list<TCDC_User>::iterator it = lst.begin(); it != lst.end(); ++it)
			{
				user = *it;
				cJSON_AddItemToArray(dataArr, data = cJSON_CreateObject());
				JSON_ADD_ONE_ELEMENT;
			}
		}
		else if (lst.size() == 1)
		{
			cJSON_AddItemToObject(result, "Data", data = cJSON_CreateObject());
			user = lst.back();
			JSON_ADD_ONE_ELEMENT;
		}
#undef JSON_ADD_ONE_ELEMENT
	}
	out = cJSON_Print(result);
	cJSON_Delete(result);
	return string(out);
}


std::string CDC_User::CDC_User_FindCount(const std::string& req)
{
	int ret = 0;
	bool hasItem = false;
	bool isAll = true;
	cJSON *json, *tmp;
	double count = -1;
	json = cJSON_Parse(req.c_str());
	if (!json)
	{
		MERR << "cJSON_Parse Error: " << cJSON_GetErrorPtr();
		goto END;
	}
	else
	{
		tmp = cJSON_GetObjectItem(json, "Method");
		string method;
		if (tmp)
		{
			method = tmp->valuestring;
			if (ToLower(method) == "part")
				isAll = false;
		}
		else
		{
			goto END;
		}

		if (isAll)
		{
			count = User_Count();
			goto END;
		}

#define JSON_GET_OBJECT_ITEM(X) \
		tmp = cJSON_GetObjectItem(json, X);\
		if (tmp){\
			if (!hasItem){\
				ss << " " << X << " = ";\
				hasItem = true;\
									}\
							else\
				ss << " AND " << X << " = ";\
					}

		stringstream ss;
		JSON_GET_OBJECT_ITEM("User_ID");
		if (tmp) ss << tmp->valuedouble;

		JSON_GET_OBJECT_ITEM("User_Role");
		if (tmp) ss << tmp->valueint;

		JSON_GET_OBJECT_ITEM("User_UserGroup_ID");
		if (tmp) ss << tmp->valuedouble;

		JSON_GET_OBJECT_ITEM("User_Name");
		if (tmp) ss << "'" << tmp->valuestring << "'";

		JSON_GET_OBJECT_ITEM("User_LoginName");
		if (tmp) ss << "'" << tmp->valuestring << "'";

		JSON_GET_OBJECT_ITEM("User_Password");
		if (tmp) ss << "'" << tmp->valuestring << "'";

		JSON_GET_OBJECT_ITEM("User_Remarks");
		if (tmp) ss << "'" << tmp->valuestring << "'";

#undef JSON_GET_OBJECT_ITEM

		if (!hasItem) goto END;

		string whereSql = ss.str();
		count = User_Count(whereSql);
	}

END:
	cJSON_Delete(json);
	char *out = 0;
	cJSON *result = cJSON_CreateObject();
	if (count < 0)
		cJSON_AddNumberToObject(result, "Result", 0);
	else
		cJSON_AddNumberToObject(result, "Result", count);

	out = cJSON_Print(result);
	cJSON_Delete(result);
	return string(out);
}

/////////////////////////////
double CDC_User::User_Add(TCDC_User& src)
{
	double id = -1;
	try
	{
		if (User_Find(src.User_ID))
			return exists;

		_stmt = compileStatement("insert into CDC_User values (?, ?, ?, ?, ?, ?, ?);");
		if (src.User_ID != INVALID_NUM && src.User_ID > 0)
			id = src.User_ID;
		else
			id = GetMaxID() + 1;
		_stmt.bind(1, id);
		_stmt.bind(2, src.User_Role);
		_stmt.bind(3, src.User_UserGroup_ID);
		_stmt.bind(4, src.User_Name);
		_stmt.bind(5, src.User_LoginName);
		_stmt.bind(6, src.User_Password);
		_stmt.bind(7, src.User_Remarks);

		_stmt.execDML();
		_stmt.reset();
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
		return DBError;
	}
	return id;
}

int CDC_User::User_Del(double id)
{
	try
	{
		if (!User_Find(id))
			return notExists;

		_stmt = compileStatement("delete from CDC_User where User_ID = ?;");
		_stmt.bind(1, id);
		_stmt.execDML();
		_stmt.reset();
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
		return DBError;
	}
	return success;
}

int CDC_User::User_Del(const std::string& name)
{
	try
	{
		if (!User_Find(name))
			return notExists;

		char buf[1024] = { 0 };
		sprintf(buf, "delete from CDC_User where User_Name = %s;", name.c_str());
		execDML(buf);
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
		return DBError;
	}
	return success;
}

int CDC_User::User_Update(TCDC_User& src)
{
	try
	{
		if (!User_Find(src.User_ID))
			return notExists;

		_stmt = compileStatement("update CDC_User \
									   			set User_Role = ?, User_UserGroup_ID = ?, User_Name = ?, \
															User_LoginName = ?, User_Password = ?, User_Remarks = ? \
																	where User_ID = ?;");

		_stmt.bind(1, src.User_Role);
		_stmt.bind(2, src.User_UserGroup_ID);
		_stmt.bind(3, src.User_Name);
		_stmt.bind(4, src.User_LoginName);
		_stmt.bind(5, src.User_Password);
		_stmt.bind(6, src.User_Remarks);
		_stmt.bind(7, src.User_ID);

		_stmt.execDML();
		_stmt.reset();
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
		return DBError;
	}
	return success;
}

int CDC_User::User_Update(TCDC_User& src, std::list<string>& keyList)
{
	try
	{
		list<string>::iterator it = find(keyList.begin(), keyList.end(), "User_ID");
		if (it == keyList.end())
			return inputConditionError;

		if (!User_Find(src.User_ID))
			return notExists;

		string updateSql = "update CDC_User set ";
		for (list<string>::iterator it = keyList.begin(); it != keyList.end(); ++it)
		{
			if (*it != "User_ID")
				updateSql = updateSql + *it + " = ?, ";
		}
		updateSql = Trim(updateSql);
		updateSql = updateSql.substr(0, updateSql.size() - 1);
		updateSql += " where User_ID = ?;";

		_stmt = compileStatement(updateSql.c_str());

		int index = 1;
		for (list<string>::iterator it = keyList.begin(); it != keyList.end(); ++it)
		{
			if (*it == "User_Role")
				_stmt.bind(index++, src.User_Role);
			else if (*it == "User_UserGroup_ID")
				_stmt.bind(index++, src.User_UserGroup_ID);
			else if (*it == "User_Name")
				_stmt.bind(index++, src.User_Name);
			else if (*it == "User_LoginName")
				_stmt.bind(index++, src.User_LoginName);
			else if (*it == "User_Password")
				_stmt.bind(index++, src.User_Password);
			else if (*it == "User_Remarks")
				_stmt.bind(index++, src.User_Remarks);
		}
		_stmt.bind(index, src.User_ID);

		_stmt.execDML();
		_stmt.reset();
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
		return DBError;
	}
	return success;
}

bool CDC_User::User_Find(double id)
{
	if (id == INVALID_NUM)
		return false;

	try
	{
		char buf[1024] = { 0 };
		sprintf(buf, "select count(*) from CDC_User where User_ID = %f;", id);
		return (execScalar(buf) != 0);
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
		return false;
	}
	return true;
}

bool CDC_User::User_Find(const std::string& name)
{
	try
	{
		char buf[1024] = { 0 };
		sprintf(buf, "select count(*) from CDC_User where User_LoginName = '%s';", name.c_str());
		int dd = execScalar(buf);
		return (execScalar(buf) != 0);
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
		return false;
	}
	return true;
}


int CDC_User::User_Find(double id, TCDC_User& t)
{
	try
	{
		if (!User_Find(id))
			return notExists;

		CppMySQLQuery q;
		char buf[1024] = { 0 };
		sprintf(buf, "select * from CDC_User where User_ID = %f;", id);

		q = execQuery(buf);

		if (!q.eof())
		{
			t.User_ID = q.getDoubleField(0);
			t.User_Role = q.getIntField(1);
			t.User_UserGroup_ID = q.getDoubleField(2);
			t.User_Name = q.getStringField(3);
			t.User_LoginName = q.getStringField(4);
			t.User_Password = q.getStringField(5);
			t.User_Remarks = q.getStringField(6);
			return success;
		}
		MDEBUG << "not find, id: " << id;
		return notExists;
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
		return DBError;
	}
	return success;
}

int CDC_User::User_Find(const std::string& name, TCDC_User& t)
{
	try
	{
		if (!User_Find(name))
			return notExists;

		CppMySQLQuery q;
		char buf[1024] = { 0 };
		sprintf(buf, "select * from CDC_User where User_LoginName = '%s';", name.c_str());

		q = execQuery(buf);

		if (!q.eof())
		{
			t.User_ID = q.getDoubleField(0);
			t.User_Role = q.getIntField(1);
			t.User_UserGroup_ID = q.getDoubleField(2);
			t.User_Name = q.getStringField(3);
			t.User_LoginName = q.getStringField(4);
			t.User_Password = q.getStringField(5);
			t.User_Remarks = q.getStringField(6);
			return success;
		}
		MDEBUG << "not find, name: " << name;
		return notExists;
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
		return DBError;
	}
	return success;
}


int CDC_User::User_Find2(const std::string& whereSql, TCDC_User& t)
{
	try
	{
		CppMySQLQuery q;
		char buf[1024] = { 0 };
		sprintf(buf, "select * from CDC_User where %s;", whereSql.c_str());

		q = execQuery(buf);

		if (!q.eof())
		{
			t.User_ID = q.getDoubleField(0);
			t.User_Role = q.getIntField(1);
			t.User_UserGroup_ID = q.getDoubleField(2);
			t.User_Name = q.getStringField(3);
			t.User_LoginName = q.getStringField(4);
			t.User_Password = q.getStringField(5);
			t.User_Remarks = q.getStringField(6);
			return success;
		}
		MDEBUG << "not find, whereSql: " << whereSql;
		return notExists;
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
		return DBError;
	}
	return success;
}

std::list<TCDC_User> CDC_User::User_Find2(const std::string& whereSql)
{
	std::list<TCDC_User> lst;
	try
	{
		CppMySQLQuery q;
		char buf[1024] = { 0 };
		sprintf(buf, "select * from CDC_User where %s;", whereSql.c_str());

		q = execQuery(buf);

		while (!q.eof())
		{
			TCDC_User t;
			t.User_ID = q.getDoubleField(0);
			t.User_Role = q.getIntField(1);
			t.User_UserGroup_ID = q.getDoubleField(2);
			t.User_Name = q.getStringField(3);
			t.User_LoginName = q.getStringField(4);
			t.User_Password = q.getStringField(5);
			t.User_Remarks = q.getStringField(6);
			lst.push_back(t);
			q.nextRow();
		}
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
		return lst;
	}
	if (lst.size() == 0)
		MDEBUG << "not find, whereSql: " << whereSql;
	return lst;
}

int CDC_User::User_Count()
{
	try
	{
		return execScalar("select count(*) from CDC_User;");
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
		return DBError;
	}
	return 0;
}

int CDC_User::User_Count(const std::string& whereSql)
{
	try
	{
		char buf[1024] = { 0 };
		sprintf(buf, "select count(*) from CDC_User where %s;", whereSql.c_str());

		return execScalar(buf);
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
		return DBError;
	}
	return 0;
}


std::list<TCDC_User> CDC_User::GetAll()
{
	std::list<TCDC_User> lst;
	try
	{
		CppMySQLQuery q = execQuery("select * from CDC_User;");
		while (!q.eof())
		{
			TCDC_User t;
			t.User_ID = q.getDoubleField(0);
			t.User_Role = q.getIntField(1);
			t.User_UserGroup_ID = q.getDoubleField(2);
			t.User_Name = q.getStringField(3);
			t.User_LoginName = q.getStringField(4);
			t.User_Password = q.getStringField(5);
			t.User_Remarks = q.getStringField(6);

			lst.push_back(t);
			q.nextRow();
		}
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
		return lst;
	}
	return lst;
}


double CDC_User::GetMaxID()
{
	char buf[1024] = { 0 };
	sprintf(buf, "select max(User_ID) from CDC_User");
	CppMySQLQuery q = execQuery(buf);

	if (q.eof() || q.numFields() < 1)
	{
		throw CppMySQLException(CPPMYSQL_DB_ERROR, "Invalid scalar query");
	}

	return q.getDoubleField(0);
}