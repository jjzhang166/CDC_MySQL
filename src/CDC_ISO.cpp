#include "CDC_ISO.h"
#include "LogHelper.h"
#include "ComDef.h"
#include "cJSON.h"
#include "utility.h"
#include <stdio.h>
#include <assert.h> 
#include <stdio.h>
#include <algorithm>

using namespace std;

CDC_ISO::CDC_ISO()
	:_pdb(NULL)
{
}


CDC_ISO::~CDC_ISO()
{
	_stmt.clear();
}


std::string CDC_ISO::CDC_ISO_Add(const std::string& req)
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

		TCDC_ISO t;
		JSON_ADD_ONE_ELEMENT(ISO_HostID, valuedouble);
		JSON_ADD_ONE_ELEMENT(ISO_Name, valuestring);
		JSON_ADD_ONE_ELEMENT(ISO_Path, valuestring);

#undef JSON_ADD_ONE_ELEMENT

		id = ISO_Add(t);
	}

END:
	cJSON_Delete(json);

	ret = (id >= 0) ? 1 : 0;
	cJSON *result = cJSON_CreateObject();
	cJSON_AddNumberToObject(result, "Result", ret);
	// if success then return id
	if (ret > 0)
		cJSON_AddNumberToObject(result, "ISO_ID", id);
	char *out = cJSON_Print(result);
	cJSON_Delete(result);

	return string(out);
}

std::string CDC_ISO::CDC_ISO_Del(const std::string& req)
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
		tmp = cJSON_GetObjectItem(json, "ISO_ID");
		if (tmp)
			id = tmp->valuedouble;
		else
			goto END;
		ret = ISO_Del(id);
	}

END:
	cJSON_Delete(json);

	cJSON *result = cJSON_CreateObject();
	cJSON_AddNumberToObject(result, "Result", ret);
	char *out = cJSON_Print(result);
	cJSON_Delete(result);

	return string(out);
}

std::string CDC_ISO::CDC_ISO_Update(const std::string& req)
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

		TCDC_ISO t;
		const char *key = "ISO_ID";
		tmp = cJSON_GetObjectItem(json, key);
		if (tmp)
		{
			if (!hasItem) hasItem = true;
			t.ISO_ID = tmp->valuedouble;
			keyList.push_back(key);
		}
		JSON_GET_OBJECT_ITEM(ISO_ID, valuedouble);
		JSON_GET_OBJECT_ITEM(ISO_HostID, valuedouble);
		JSON_GET_OBJECT_ITEM(ISO_Name, valuestring);
		JSON_GET_OBJECT_ITEM(ISO_Path, valuestring);

	#undef JSON_GET_OBJECT_ITEM

		if (!hasItem) goto END;

		ret = ISO_Update(t, keyList);
	}

END:
	cJSON_Delete(json);

	cJSON *result = cJSON_CreateObject();
	cJSON_AddNumberToObject(result, "Result", ret);
	char *out = cJSON_Print(result);
	cJSON_Delete(result);

	return string(out);
}

std::string CDC_ISO::CDC_ISO_Find(const std::string& req)
{
	int ret = 0;
	bool hasItem = false;
	bool isAll = true;
	cJSON *json, *tmp;
	TCDC_ISO t;
	list<TCDC_ISO> lst;
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
						}else\
				ss << " AND " << X << " = ";}

		stringstream ss;
		JSON_GET_OBJECT_ITEM("ISO_ID");
		if (tmp) ss << tmp->valuedouble;

		JSON_GET_OBJECT_ITEM("ISO_HostID");
		if (tmp) ss << tmp->valuedouble;

		JSON_GET_OBJECT_ITEM("ISO_Name");
		if (tmp) ss << "'" << tmp->valuestring << "'";

		JSON_GET_OBJECT_ITEM("ISO_Path");
		if (tmp) ss << "'" << tmp->valuestring << "'";

	#undef JSON_GET_OBJECT_ITEM

		if (!hasItem) goto END;

		string whereSql = ss.str();
		lst = ISO_Find2(whereSql);
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
		cJSON_AddNumberToObject(data, "ISO_ID", t.ISO_ID);\
		cJSON_AddNumberToObject(data, "ISO_HostID", t.ISO_HostID);\
		cJSON_AddStringToObject(data, "ISO_Name", t.ISO_Name.c_str());\
		cJSON_AddStringToObject(data, "ISO_Path", t.ISO_Path.c_str());

		cJSON *dataArr, *data = 0;
		cJSON_AddNumberToObject(result, "Result", lst.size());
		if (lst.size() > 1)
		{
			cJSON_AddItemToObject(result, "Data", dataArr = cJSON_CreateArray());
			for (list<TCDC_ISO>::iterator it = lst.begin(); it != lst.end(); ++it)
			{
				t = *it;
				cJSON_AddItemToArray(dataArr, data = cJSON_CreateObject());
				JSON_ADD_ONE_ELEMENT;
			}
		}
		else if (lst.size() == 1)
		{
			cJSON_AddItemToObject(result, "Data", data = cJSON_CreateObject());
			t = lst.back();
			JSON_ADD_ONE_ELEMENT;
		}
	#undef JSON_ADD_ONE_ELEMENT
	}
	out = cJSON_Print(result);
	cJSON_Delete(result);
	return string(out);
}

std::string CDC_ISO::CDC_ISO_FindCount(const std::string& req)
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
			count = ISO_Count();
			goto END;
		}

#define JSON_GET_OBJECT_ITEM(X) \
		tmp = cJSON_GetObjectItem(json, X);\
		if (tmp){\
			if (!hasItem){\
				ss << " " << X << " = ";\
				hasItem = true;\
			}else\
				ss << " AND " << X << " = ";}

		stringstream ss;
		JSON_GET_OBJECT_ITEM("ISO_ID");
		if (tmp) ss << tmp->valuedouble;

		JSON_GET_OBJECT_ITEM("ISO_HostID");
		if (tmp) ss << tmp->valuedouble;

		JSON_GET_OBJECT_ITEM("ISO_Name");
		if (tmp) ss << "'" << tmp->valuestring << "'";

		JSON_GET_OBJECT_ITEM("ISO_Path");
		if (tmp) ss << "'" << tmp->valuestring << "'";

#undef JSON_GET_OBJECT_ITEM

		if (!hasItem) goto END;

		string whereSql = ss.str();
		count = ISO_Count(whereSql);
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

////////////////////////////////////////////////////////////

double CDC_ISO::ISO_Add(TCDC_ISO& src)
{
	double id = -1;
	try
	{
		if (ISO_Find(src.ISO_ID))
			return exists;

		_stmt = compileStatement("insert into CDC_ISO values (?, ?, ?, ?);");
		if (src.ISO_ID != INVALID_NUM && src.ISO_ID > 0)
			id = src.ISO_ID;
		else
			id = GetMaxID() + 1;
		_stmt.bind(1, id);
		_stmt.bind(2, src.ISO_HostID);
		_stmt.bind(3, src.ISO_Name);
		_stmt.bind(4, src.ISO_Path);

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

int CDC_ISO::ISO_Del(double id)
{
	try
	{
		if (!ISO_Find(id))
			return notExists;

		_stmt = compileStatement("delete from CDC_ISO where ISO_ID = ?;");
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

int CDC_ISO::ISO_Del(const std::string& name)
{
	try
	{
		if (!ISO_Find(name))
			return notExists;

		char buf[1024] = { 0 };
		sprintf(buf, "delete from CDC_ISO where ISO_Name = %s;", name.c_str());
		execDML(buf);
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
		return DBError;
	}
	return success;
}

int CDC_ISO::ISO_Update(TCDC_ISO& src)
{
	try
	{
		if (!ISO_Find(src.ISO_ID))
			return notExists;

		_stmt = compileStatement("update CDC_ISO \
			set ISO_HostID = ?, ISO_Name = ?, ISO_Path = ? where ISO_ID = ?;");
		
		_stmt.bind(1, src.ISO_HostID);
		_stmt.bind(2, src.ISO_Name);
		_stmt.bind(3, src.ISO_Path);
		_stmt.bind(4, src.ISO_ID);

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

int CDC_ISO::ISO_Update(TCDC_ISO& src, std::list<std::string>& keyList)
{
	try
	{
		list<string>::iterator it = find(keyList.begin(), keyList.end(), "ISO_ID");
		if (it == keyList.end())
			return inputConditionError;

		if (!ISO_Find(src.ISO_ID))
			return notExists;

		string updateSql = "update CDC_ISO set ";
		for (list<string>::iterator it = keyList.begin(); it != keyList.end(); ++it)
		{
			if (*it != "ISO_ID")
				updateSql = updateSql + *it + " = ?, ";
		}
		updateSql = Trim(updateSql);
		updateSql = updateSql.substr(0, updateSql.size() - 1);
		updateSql += " where ISO_ID = ?;";

		_stmt = compileStatement(updateSql.c_str());

		int index = 1;
		for (list<string>::iterator it = keyList.begin(); it != keyList.end(); ++it)
		{
			if (*it == "ISO_HostID")
				_stmt.bind(index++, src.ISO_HostID);
			else if (*it == "ISO_Name")
				_stmt.bind(index++, src.ISO_Name);
			else if (*it == "ISO_Path")
				_stmt.bind(index++, src.ISO_Path);
		}
		_stmt.bind(index, src.ISO_ID);

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

bool CDC_ISO::ISO_Find(double id)
{
	if (id == INVALID_NUM)
		return false;
	try
	{
		char buf[1024] = { 0 };
		sprintf(buf, "select count(*) from CDC_ISO where ISO_ID = %f;", id);
		return (execScalar(buf) != 0);
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
		return false;
	}
	return true;
}

bool CDC_ISO::ISO_Find(const std::string& name)
{
	try
	{
		char buf[1024] = { 0 };
		sprintf(buf, "select count(*) from CDC_ISO where ISO_Name = '%s';", name.c_str());
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


int CDC_ISO::ISO_Find(double id, TCDC_ISO& t)
{
	try
	{
		if (!ISO_Find(id))
			return notExists;

		CppMySQLQuery q;
		char buf[1024] = { 0 };
		sprintf(buf, "select * from CDC_ISO where ISO_ID = %f;", id);

		q = execQuery(buf);

		if (!q.eof())
		{
			t.ISO_ID = q.getDoubleField("ISO_ID");
			t.ISO_HostID = q.getDoubleField("ISO_HostID");
			t.ISO_Name = q.fieldValue("ISO_Name");
			t.ISO_Path = q.fieldValue("ISO_Path");
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

int CDC_ISO::ISO_Find(const std::string& name, TCDC_ISO& t)
{
	try
	{
		if (!ISO_Find(name))
			return notExists;

		CppMySQLQuery q;
		char buf[1024] = { 0 };
		sprintf(buf, "select * from CDC_ISO where ISO_Name = '%s';", name.c_str());

		q = execQuery(buf);

		if (!q.eof())
		{
			t.ISO_ID = q.getDoubleField("ISO_ID");
			t.ISO_HostID = q.getDoubleField("ISO_HostID");
			t.ISO_Name = q.fieldValue("ISO_Name");
			t.ISO_Path = q.fieldValue("ISO_Path");
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

int CDC_ISO::ISO_Find2(const std::string& whereSql, TCDC_ISO& t)
{
	try
	{
		CppMySQLQuery q;
		char buf[1024] = { 0 };
		sprintf(buf, "select * from CDC_ISO where %s;", whereSql.c_str());

		q = execQuery(buf);

		if (!q.eof())
		{
			t.ISO_ID = q.getDoubleField("ISO_ID");
			t.ISO_HostID = q.getDoubleField("ISO_HostID");
			t.ISO_Name = q.fieldValue("ISO_Name");
			t.ISO_Path = q.fieldValue("ISO_Path");
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


std::list<TCDC_ISO> CDC_ISO::ISO_Find2(const std::string& whereSql)
{
	std::list<TCDC_ISO> lst;
	try
	{
		CppMySQLQuery q;
		char buf[1024] = { 0 };
		sprintf(buf, "select * from CDC_ISO where %s;", whereSql.c_str());

		q = execQuery(buf);
		while (!q.eof())
		{
			TCDC_ISO t;
			t.ISO_ID = q.getDoubleField(0);
			t.ISO_HostID = q.getDoubleField(1);
			t.ISO_Name = q.getStringField(2);
			t.ISO_Path = q.getStringField(3);

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

int CDC_ISO::ISO_Count()
{
	try
	{
		return execScalar("select count(*) from CDC_ISO;");
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
		return DBError;
	}
	return 0;
}

int CDC_ISO::ISO_Count(const std::string& whereSql)
{
	try
	{
		char buf[1024] = { 0 };
		sprintf(buf, "select count(*) from CDC_ISO where %s;", whereSql.c_str());

		return execScalar(buf);
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
		return DBError;
	}
	return 0;
}

std::list<TCDC_ISO> CDC_ISO::GetAll()
{
	std::list<TCDC_ISO> lst;
	try
	{
		CppMySQLQuery q = execQuery("select * from CDC_ISO;");
		while (!q.eof())
		{
			TCDC_ISO t;
			t.ISO_ID = q.getDoubleField(0);
			t.ISO_HostID = q.getDoubleField(1);
			t.ISO_Name = q.getStringField(2);
			t.ISO_Path = q.getStringField(3);

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

double CDC_ISO::GetMaxID()
{
	char buf[1024] = { 0 };
	sprintf(buf, "select max(ISO_ID) from CDC_ISO");
	CppMySQLQuery q = execQuery(buf);

	if (q.eof() || q.numFields() < 1)
	{
		throw CppMySQLException(CPPMYSQL_DB_ERROR, "Invalid scalar query");
	}

	return q.getDoubleField(0);
}