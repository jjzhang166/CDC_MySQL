#include "CDC_UserGroup.h"
#include "LogHelper.h"
#include "ComDef.h"
#include "cJSON.h"
#include "utility.h"

#include <assert.h> 
#include <stdio.h>

using namespace std;

CDC_UserGroup::CDC_UserGroup(CppMySQLDB& db)
	:_db(db)
{
}


CDC_UserGroup::~CDC_UserGroup()
{
	_stmt.clear();
}


std::string CDC_UserGroup::CDC_UserGroup_add(const std::string& req)
{
	int ret = 0;
	double id = -1;

	cJSON *json, *tmp, *element;
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
		
		TCDC_UserGroup userGroup;
		tmp = cJSON_GetObjectItem(json, "UserGroup_Name");
		if (tmp)
			userGroup.UserGroup_Name = tmp->valuestring;
		else
			goto END;
		id = UserGroup_add(userGroup);
	}

END:
	cJSON_Delete(json);

	ret = (id >= 0) ? 1 : 0;
	cJSON *result = cJSON_CreateObject();
	cJSON_AddNumberToObject(result, "Result", ret);
	if (ret > 0)
		cJSON_AddNumberToObject(result, "UserGroup_ID", id);
	char *out = cJSON_Print(result);
	cJSON_Delete(result);

	return string(out);
}

std::string CDC_UserGroup::CDC_UserGroup_del(const std::string& req)
{
	int ret = 0;
	cJSON *json, *tmp, *element;
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
		tmp = cJSON_GetObjectItem(json, "UserGroup_ID");
		if (tmp)
			id = tmp->valuedouble;
		else
			goto END;
		ret = UserGroup_del(id);
	}

END:
	cJSON_Delete(json);

	cJSON *result = cJSON_CreateObject();
	cJSON_AddNumberToObject(result, "Result", ret);
	char *out = cJSON_Print(result);
	cJSON_Delete(result);

	return string(out);
}


std::string CDC_UserGroup::CDC_UserGroup_update(const std::string& req)
{
	int ret = 0;
	bool hasItem = false;
	cJSON *json, *tmp, *element;
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

		TCDC_UserGroup userGroup;
		tmp = cJSON_GetObjectItem(json, "UserGroup_ID");
		if (tmp)
		{
			if (!hasItem) hasItem = true;
			userGroup.UserGroup_ID = tmp->valuedouble;
		}
		tmp = cJSON_GetObjectItem(json, "UserGroup_Name");
		if (tmp)
		{
			if (!hasItem) hasItem = true;
			userGroup.UserGroup_Name = tmp->valuestring;
		}
		
		if (!hasItem) goto END;

		ret = UserGroup_update(userGroup);
	}

END:
	cJSON_Delete(json);

	cJSON *result = cJSON_CreateObject();
	cJSON_AddNumberToObject(result, "Result", ret);
	char *out = cJSON_Print(result);
	cJSON_Delete(result);

	return string(out);
}

std::string CDC_UserGroup::CDC_UserGroup_find(const std::string& req)
{
	int ret = 0;
	bool hasItem = false;
	bool isAll = true;
	cJSON *json, *tmp, *element;
	TCDC_UserGroup userGroup;
	list<TCDC_UserGroup> lst;
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

		stringstream ss;
		tmp = cJSON_GetObjectItem(json, "UserGroup_ID");
		if (tmp)
		{
			if (!hasItem)
			{
				ss << " UserGroup_ID = ";
				hasItem = true;
			}
			else
				ss << " AND UserGroup_ID = ";
			ss << tmp->valuedouble;
		}
		tmp = cJSON_GetObjectItem(json, "UserGroup_Name");
		if (tmp)
		{
			if (!hasItem)
			{
				ss << " UserGroup_Name = ";
				hasItem = true;
			}
			else
			{
				ss << " AND UserGroup_Name = ";
			}
			ss << "'" << tmp->valuestring << "'";
		}

		if (!hasItem) goto END;

		string whereSql = ss.str();
		ret = UserGroup_find2(whereSql, userGroup);
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
		cJSON *dataArr, *data = 0;
		if (!isAll)
		{
			cJSON_AddNumberToObject(result, "Result", 1);
			cJSON_AddItemToObject(result, "Data", dataArr = cJSON_CreateArray());
			cJSON_AddItemToArray(dataArr, data = cJSON_CreateObject());
			cJSON_AddNumberToObject(data, "UserGroup_ID", userGroup.UserGroup_ID);
			cJSON_AddStringToObject(data, "UserGroup_Name", userGroup.UserGroup_Name.c_str());
		}
		else
		{
			cJSON_AddNumberToObject(result, "Result", lst.size());
			cJSON_AddItemToObject(result, "Data", dataArr = cJSON_CreateArray());
			for (list<TCDC_UserGroup>::iterator it = lst.begin(); it != lst.end(); ++it)
			{
				cJSON_AddItemToArray(dataArr, data = cJSON_CreateObject());
				cJSON_AddNumberToObject(data, "UserGroup_ID", (*it).UserGroup_ID);
				cJSON_AddStringToObject(data, "UserGroup_Name", (*it).UserGroup_Name.c_str());
			}
		}
	}
	out = cJSON_Print(result);
	cJSON_Delete(result);
	return string(out);
}


std::string CDC_UserGroup::CDC_UserGroup_FindCount(const std::string& req)
{
	int ret = 0;
	bool hasItem = false;
	bool isAll = true;
	cJSON *json, *tmp, *element;
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
			count = UserGroup_Count();
			goto END;
		}

		stringstream ss;
		tmp = cJSON_GetObjectItem(json, "UserGroup_ID");
		if (tmp)
		{
			if (!hasItem)
			{
				ss << " UserGroup_ID = ";
				hasItem = true;
			}
			else
				ss << " AND UserGroup_ID = ";
			ss << tmp->valuedouble;
		}
		tmp = cJSON_GetObjectItem(json, "UserGroup_Name");
		if (tmp)
		{
			if (!hasItem)
			{
				ss << " UserGroup_Name = ";
				hasItem = true;
			}
			else
				ss << " AND UserGroup_Name = ";
			ss << "'" << tmp->valuestring << "'";
		}

		if (!hasItem) goto END;

		string whereSql = ss.str();
		count = UserGroup_Count(whereSql);
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

double CDC_UserGroup::GetMaxID()
{
	char buf[1024] = { 0 };
	sprintf(buf, "select max(UserGroup_ID) from CDC_UserGroup");
	CppMySQLQuery q = _db.execQuery(buf);

	if (q.eof() || q.numFields() < 1)
	{
		throw CppMySQLException(CPPMYSQL_DB_ERROR, "Invalid scalar query");
	}

	return q.getDoubleField(0);
}

double CDC_UserGroup::UserGroup_add(TCDC_UserGroup& src)
{
	double id = -1;
	try
	{
		if (UserGroup_find(src.UserGroup_ID))
			return exists;

		_stmt = _db.compileStatement("insert into CDC_UserGroup values (?, ?);");

		if (src.UserGroup_ID != INVALID_ID)
			id = src.UserGroup_ID;
		else
			id = GetMaxID() + 1;
		_stmt.bind(1, id);
		_stmt.bind(2, src.UserGroup_Name);

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

int CDC_UserGroup::UserGroup_del(double id)
{
	try
	{
		if (!UserGroup_find(id))
			return notExists;

		char buf[1024] = { 0 };
		sprintf(buf, "delete from CDC_UserGroup where UserGroup_ID = %f;", id);
		_db.execDML(buf);
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
		return DBError;
	}
	return success;
}

int CDC_UserGroup::UserGroup_del(std::string& name)
{
	try
	{
		if (!UserGroup_find(name))
			return notExists;

		char buf[1024] = { 0 };
		sprintf(buf, "delete from CDC_UserGroup where UserGroup_Name = '%s';", name.c_str());
		_db.execDML(buf);
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
		return DBError;
	}
	return success;
}

int CDC_UserGroup::UserGroup_update(TCDC_UserGroup& src)
{
	try
	{
		_stmt = _db.compileStatement("update CDC_UserGroup set UserGroup_Name = ? where UserGroup_ID = ?;");
		_stmt.bind(1, src.UserGroup_Name);
		_stmt.bind(2, src.UserGroup_ID);
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


bool CDC_UserGroup::UserGroup_find(double id)
{
	try
	{
		char buf[1024] = {0};
		sprintf(buf, "select count(*) from CDC_UserGroup where UserGroup_ID = %f;", id);
		return (_db.execScalar(buf) != 0);
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
		return false;
	}
	return true;
}

bool CDC_UserGroup::UserGroup_find(std::string& name)
{
	try
	{
		char buf[1024] = { 0 };
		sprintf(buf, "select count(*) from CDC_UserGroup where UserGroup_Name = '%s';", name.c_str());
		return (_db.execScalar(buf) != 0);
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
		return false;
	}
	return true;
}


int CDC_UserGroup::UserGroup_find(double id, TCDC_UserGroup& t)
{
	try
	{
		if (!UserGroup_find(id))
			return notExists;

		CppMySQLQuery q;
		char buf[1024] = { 0 };
		sprintf(buf, "select * from CDC_UserGroup where UserGroup_ID = %f;", id);

		q = _db.execQuery(buf);

		if (!q.eof())
		{
			t.UserGroup_ID = q.getDoubleField("UserGroup_ID");
			t.UserGroup_Name = q.fieldValue("UserGroup_Name");
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

int CDC_UserGroup::UserGroup_find(std::string& name, TCDC_UserGroup& t)
{
	try
	{
		if (!UserGroup_find(name))
			return notExists;

		CppMySQLQuery q;
		char buf[1024] = { 0 };
		sprintf(buf, "select * from CDC_UserGroup where UserGroup_Name = '%s';", name.c_str());

		q = _db.execQuery(buf);

		if (!q.eof())
		{
			t.UserGroup_ID = q.getDoubleField("UserGroup_ID");
			t.UserGroup_Name = q.fieldValue("UserGroup_Name");
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

int CDC_UserGroup::UserGroup_find2(std::string& whereSql, TCDC_UserGroup& t)
{
	try
	{
		CppMySQLQuery q;
		char buf[1024] = { 0 };
		sprintf(buf, "select * from CDC_UserGroup where %s;", whereSql.c_str());

		q = _db.execQuery(buf);

		if (!q.eof())
		{
			t.UserGroup_ID = q.getDoubleField("UserGroup_ID");
			t.UserGroup_Name = q.fieldValue("UserGroup_Name");
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

int CDC_UserGroup::UserGroup_Count()
{
	try
	{
		return _db.execScalar("select count(*) from CDC_UserGroup;");
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
		return DBError;
	}
	return 0;
}

int CDC_UserGroup::UserGroup_Count(std::string& whereSql)
{
	try
	{
		char buf[1024] = { 0 };
		sprintf(buf, "select count(*) from CDC_UserGroup where %s;", whereSql.c_str());

		return _db.execScalar(buf);
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
		return DBError;
	}
	return 0;
}

std::list<TCDC_UserGroup> CDC_UserGroup::GetAll()
{
	std::list<TCDC_UserGroup> lst;
	try
	{
		CppMySQLQuery q = _db.execQuery("select * from CDC_UserGroup;");
		while (!q.eof())
		{
			TCDC_UserGroup t;
			t.UserGroup_ID = q.getDoubleField(0);
			t.UserGroup_Name = q.getStringField(1);
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