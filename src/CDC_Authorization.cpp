#include "CDC_Authorization.h"
#include "LogHelper.h"
#include "ComDef.h"
#include "cJSON.h"
#include "utility.h"
#include <stdio.h>
#include <assert.h> 
#include <stdio.h>
#include <algorithm>

using namespace std;

CDC_Authorization::CDC_Authorization(CppMySQLDB& db)
	:_db(db)
{
}


CDC_Authorization::~CDC_Authorization()
{
	_stmt.clear();
}


std::string CDC_Authorization::CDC_Authorization_Add(const std::string& req)
{
	int ret = 0;
	std::string id;

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

#define JSON_ADD_ONE_ELEMENT(X, Y) \
		tmp = cJSON_GetObjectItem(json, #X);\
		if (tmp) t.X = tmp->Y;\
				else goto END;

		TCDC_Authorization t;
		JSON_ADD_ONE_ELEMENT(Authorization_MachineID, valuestring);
		JSON_ADD_ONE_ELEMENT(Authorization_MaxClientNum, valuedouble);
		JSON_ADD_ONE_ELEMENT(Authorization_Deadline, valuestring);
		JSON_ADD_ONE_ELEMENT(Authorization_Company, valuestring);

#undef JSON_ADD_ONE_ELEMENT

		id = Authorization_Add(t);
	}

END:
	cJSON_Delete(json);

	ret = (id != "") ? 1 : 0;
	cJSON *result = cJSON_CreateObject();
	cJSON_AddNumberToObject(result, "Result", ret);
	// if success then return id
	if (ret > 0)
		cJSON_AddStringToObject(result, "Authorization_MachineID", id.c_str());
	char *out = cJSON_Print(result);
	cJSON_Delete(result);

	return string(out);
}

std::string CDC_Authorization::CDC_Authorization_Del(const std::string& req)
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

		std::string id;
		tmp = cJSON_GetObjectItem(json, "Authorization_MachineID");
		if (tmp)
			id = tmp->valuestring;
		else
			goto END;
		ret = Authorization_Del(id);
	}

END:
	cJSON_Delete(json);

	cJSON *result = cJSON_CreateObject();
	cJSON_AddNumberToObject(result, "Result", ret);
	char *out = cJSON_Print(result);
	cJSON_Delete(result);

	return string(out);
}

std::string CDC_Authorization::CDC_Authorization_Update(const std::string& req)
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

		TCDC_Authorization t;
		const char *key = "Authorization_ID";
		tmp = cJSON_GetObjectItem(json, key);
		if (tmp)
		{
			if (!hasItem) hasItem = true;
			t.Authorization_MachineID = tmp->valuestring;
			keyList.push_back(key);
		}
		JSON_GET_OBJECT_ITEM(Authorization_MachineID, valuestring);
		JSON_GET_OBJECT_ITEM(Authorization_MaxClientNum, valuedouble);
		JSON_GET_OBJECT_ITEM(Authorization_Deadline, valuestring);
		JSON_GET_OBJECT_ITEM(Authorization_Company, valuestring);

	#undef JSON_GET_OBJECT_ITEM

		if (!hasItem) goto END;

		ret = Authorization_Update(t, keyList);
	}

END:
	cJSON_Delete(json);

	cJSON *result = cJSON_CreateObject();
	cJSON_AddNumberToObject(result, "Result", ret);
	char *out = cJSON_Print(result);
	cJSON_Delete(result);

	return string(out);
}

std::string CDC_Authorization::CDC_Authorization_Find(const std::string& req)
{
	int ret = 0;
	bool hasItem = false;
	bool isAll = true;
	cJSON *json, *tmp, *element;
	TCDC_Authorization t;
	list<TCDC_Authorization> lst;
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
		JSON_GET_OBJECT_ITEM("Authorization_MachineID");
		if (tmp) ss << "'" << tmp->valuestring << "'";
		

		JSON_GET_OBJECT_ITEM("Authorization_MaxClientNum");
		if (tmp) ss << tmp->valuedouble;

		JSON_GET_OBJECT_ITEM("Authorization_Deadline");
		if (tmp) ss << "'" << tmp->valuestring << "'";

		JSON_GET_OBJECT_ITEM("Authorization_Company");
		if (tmp) ss << "'" << tmp->valuestring << "'";

	#undef JSON_GET_OBJECT_ITEM

		if (!hasItem) goto END;

		string whereSql = ss.str();
		ret = Authorization_Find2(whereSql, t);
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
		cJSON_AddStringToObject(data, "Authorization_MachineID", t.Authorization_MachineID.c_str());\
		cJSON_AddNumberToObject(data, "Authorization_MaxClientNum", t.Authorization_MaxClientNum);\
		cJSON_AddStringToObject(data, "Authorization_Deadline", t.Authorization_Deadline.c_str());\
		cJSON_AddStringToObject(data, "Authorization_Company", t.Authorization_Company.c_str());

		cJSON *dataArr, *data = 0;
		if (!isAll)
		{
			cJSON_AddNumberToObject(result, "Result", 1);
			cJSON_AddItemToObject(result, "Data", data = cJSON_CreateObject());
			JSON_ADD_ONE_ELEMENT;
		}
		else
		{
			cJSON_AddNumberToObject(result, "Result", lst.size());
			if (lst.size() > 1)
			{
				cJSON_AddItemToObject(result, "Data", dataArr = cJSON_CreateArray());
				for (list<TCDC_Authorization>::iterator it = lst.begin(); it != lst.end(); ++it)
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
		}
	#undef JSON_ADD_ONE_ELEMENT
	}
	out = cJSON_Print(result);
	cJSON_Delete(result);
	return string(out);
}

std::string CDC_Authorization::CDC_Authorization_FindCount(const std::string& req)
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
			count = Authorization_Count();
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
		JSON_GET_OBJECT_ITEM("Authorization_MachineID");
		if (tmp) ss << "'" << tmp->valuestring << "'";
		
		JSON_GET_OBJECT_ITEM("Authorization_MaxClientNum");
		if (tmp) ss << tmp->valuedouble;

		JSON_GET_OBJECT_ITEM("Authorization_Deadline");
		if (tmp) ss << "'" << tmp->valuestring << "'";

		JSON_GET_OBJECT_ITEM("Authorization_Company");
		if (tmp) ss << "'" << tmp->valuestring << "'";

#undef JSON_GET_OBJECT_ITEM

		if (!hasItem) goto END;

		string whereSql = ss.str();
		count = Authorization_Count(whereSql);
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

std::string CDC_Authorization::Authorization_Add(TCDC_Authorization& src)
{
	try
	{
		if (Authorization_Find(src.Authorization_MachineID))
			return "";

		_stmt = _db.compileStatement("insert into CDC_Authorization values (?, ?, ?, ?);");
		_stmt.bind(1, src.Authorization_MachineID);
		_stmt.bind(2, src.Authorization_MaxClientNum);
		_stmt.bind(3, String2MySQLTime(src.Authorization_Deadline));
		_stmt.bind(4, src.Authorization_Company);

		_stmt.execDML();
		_stmt.reset();
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
		return "";
	}
	return src.Authorization_MachineID;
}

int CDC_Authorization::Authorization_Del(const std::string& id)
{
	try
	{
		if (!Authorization_Find(id))
			return notExists;

		_stmt = _db.compileStatement("delete from CDC_Authorization where Authorization_MachineID = ?;");
		_stmt.bind(1, id.c_str());
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

int CDC_Authorization::Authorization_Update(TCDC_Authorization& src)
{
	try
	{
		if (!Authorization_Find(src.Authorization_MachineID))
			return notExists;

		_stmt = _db.compileStatement("update CDC_Authorization \
			set Authorization_MaxClientNum = ?, Authorization_Deadline = ?, Authorization_Company = ? where Authorization_MachineID = ?;");
		
		_stmt.bind(1, src.Authorization_MaxClientNum);
		_stmt.bind(2, String2MySQLTime(src.Authorization_Deadline));
		_stmt.bind(3, src.Authorization_Company);
		_stmt.bind(4, src.Authorization_MachineID);

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

int CDC_Authorization::Authorization_Update(TCDC_Authorization& src, std::list<std::string>& keyList)
{
	try
	{
		list<string>::iterator it = find(keyList.begin(), keyList.end(), "Authorization_MachineID");
		if (it == keyList.end())
			return inputConditionError;

		if (!Authorization_Find(src.Authorization_MachineID))
			return notExists;

		string updateSql = "update CDC_Authorization set ";
		for (list<string>::iterator it = keyList.begin(); it != keyList.end(); ++it)
		{
			if (*it != "Authorization_MachineID")
				updateSql = updateSql + *it + " = ?, ";
		}
		updateSql = Trim(updateSql);
		updateSql = updateSql.substr(0, updateSql.size() - 1);
		updateSql += " where Authorization_MachineID = ?;";

		_stmt = _db.compileStatement(updateSql.c_str());

		int index = 1;
		for (list<string>::iterator it = keyList.begin(); it != keyList.end(); ++it)
		{
			if (*it == "Authorization_MaxClientNum")
				_stmt.bind(index++, src.Authorization_MaxClientNum);
			else if (*it == "Authorization_Deadline")
				_stmt.bind(index++, String2MySQLTime(src.Authorization_Deadline));
			else if (*it == "Authorization_Company")
				_stmt.bind(index++, src.Authorization_Company);
		}
		_stmt.bind(index, src.Authorization_MachineID);

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

bool CDC_Authorization::Authorization_Find(const std::string& id)
{
	if (id == "")
		return false;
	try
	{
		char buf[1024] = { 0 };
		sprintf(buf, "select count(*) from CDC_Authorization where Authorization_MachineID = '%s';", id.c_str());
		return (_db.execScalar(buf) != 0);
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
		return false;
	}
	return true;
}


int CDC_Authorization::Authorization_Find(const std::string& id, TCDC_Authorization& t)
{
	try
	{
		if (!Authorization_Find(id))
			return notExists;

		CppMySQLQuery q;
		char buf[1024] = { 0 };
		sprintf(buf, "select * from CDC_Authorization where Authorization_MachineID = '%s';", id);

		q = _db.execQuery(buf);

		if (!q.eof())
		{
			t.Authorization_MachineID = q.getStringField(0);
			t.Authorization_MaxClientNum = q.getDoubleField(1);
			t.Authorization_Deadline = q.getStringField(2);
			t.Authorization_Company = q.getStringField(3);
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

int CDC_Authorization::Authorization_Find2(const std::string& whereSql, TCDC_Authorization& t)
{
	try
	{
		CppMySQLQuery q;
		char buf[1024] = { 0 };
		sprintf(buf, "select * from CDC_Authorization where %s;", whereSql.c_str());

		q = _db.execQuery(buf);

		if (!q.eof())
		{
			t.Authorization_MachineID = q.getStringField(0);
			t.Authorization_MaxClientNum = q.getDoubleField(1);
			t.Authorization_Deadline = q.getStringField(2);
			t.Authorization_Company = q.getStringField(3);
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

int CDC_Authorization::Authorization_Count()
{
	try
	{
		return _db.execScalar("select count(*) from CDC_Authorization;");
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
		return DBError;
	}
	return 0;
}

int CDC_Authorization::Authorization_Count(const std::string& whereSql)
{
	try
	{
		char buf[1024] = { 0 };
		sprintf(buf, "select count(*) from CDC_Authorization where %s;", whereSql.c_str());

		return _db.execScalar(buf);
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
		return DBError;
	}
	return 0;
}

std::list<TCDC_Authorization> CDC_Authorization::GetAll()
{
	std::list<TCDC_Authorization> lst;
	try
	{
		CppMySQLQuery q = _db.execQuery("select * from CDC_Authorization;");
		while (!q.eof())
		{
			TCDC_Authorization t;
			t.Authorization_MachineID = q.getStringField(0);
			t.Authorization_MaxClientNum = q.getDoubleField(1);
			t.Authorization_Deadline = q.getStringField(2);
			t.Authorization_Company = q.getStringField(3);

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

double CDC_Authorization::GetMaxID()
{
	char buf[1024] = { 0 };
	sprintf(buf, "select max(Authorization_ID) from CDC_Authorization");
	CppMySQLQuery q = _db.execQuery(buf);

	if (q.eof() || q.numFields() < 1)
	{
		throw CppMySQLException(CPPMYSQL_DB_ERROR, "Invalid scalar query");
	}

	return q.getDoubleField(0);
}