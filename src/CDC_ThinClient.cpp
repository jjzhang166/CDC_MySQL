#include "CDC_ThinClient.h"
#include "LogHelper.h"
#include "ComDef.h"
#include "cJSON.h"
#include "utility.h"
#include <stdio.h>
#include <assert.h> 
#include <stdio.h>
#include <algorithm>

using namespace std;

CDC_ThinClient::CDC_ThinClient(CppMySQLDB& db)
	:_db(db)
{
}


CDC_ThinClient::~CDC_ThinClient()
{
	_stmt.clear();
}


std::string CDC_ThinClient::CDC_ThinClient_Add(const std::string& req)
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

#define JSON_ADD_ONE_ELEMENT(X, Y) \
		tmp = cJSON_GetObjectItem(json, #X);\
		if (tmp) t.X = tmp->Y;\
				else goto END;

		TCDC_ThinClient t;
		JSON_ADD_ONE_ELEMENT(ThinClient_ThinGroup_ID, valuedouble);
		JSON_ADD_ONE_ELEMENT(ThinClient_Mode, valueint);
		JSON_ADD_ONE_ELEMENT(ThinClient_Version, valuestring);
		JSON_ADD_ONE_ELEMENT(ThinClient_State, valueint);
		JSON_ADD_ONE_ELEMENT(ThinClient_Protocol, valueint);
		JSON_ADD_ONE_ELEMENT(ThinClient_Name, valuestring);
		JSON_ADD_ONE_ELEMENT(ThinClient_IP, valuestring);
		JSON_ADD_ONE_ELEMENT(ThinClient_MAC, valuestring);

#undef JSON_ADD_ONE_ELEMENT

		id = ThinClient_Add(t);
	}

END:
	cJSON_Delete(json);

	ret = (id >= 0) ? 1 : 0;
	cJSON *result = cJSON_CreateObject();
	cJSON_AddNumberToObject(result, "Result", ret);
	// if success then return id
	if (ret > 0)
		cJSON_AddNumberToObject(result, "ThinClient_ID", id);
	char *out = cJSON_Print(result);
	cJSON_Delete(result);

	return string(out);
}

std::string CDC_ThinClient::CDC_ThinClient_Del(const std::string& req)
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
		tmp = cJSON_GetObjectItem(json, "ThinClient_ID");
		if (tmp)
			id = tmp->valuedouble;
		else
			goto END;
		ret = ThinClient_Del(id);
	}

END:
	cJSON_Delete(json);

	cJSON *result = cJSON_CreateObject();
	cJSON_AddNumberToObject(result, "Result", ret);
	char *out = cJSON_Print(result);
	cJSON_Delete(result);

	return string(out);
}

std::string CDC_ThinClient::CDC_ThinClient_Update(const std::string& req)
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

		TCDC_ThinClient t;
		const char *key = "ThinClient_ID";
		tmp = cJSON_GetObjectItem(json, key);
		if (tmp)
		{
			if (!hasItem) hasItem = true;
			t.ThinClient_ID = tmp->valuedouble;
			keyList.push_back(key);
		}
		JSON_GET_OBJECT_ITEM(ThinClient_ThinGroup_ID, valuedouble);
		JSON_GET_OBJECT_ITEM(ThinClient_Mode, valueint);
		JSON_GET_OBJECT_ITEM(ThinClient_Version, valuestring);
		JSON_GET_OBJECT_ITEM(ThinClient_State, valueint);
		JSON_GET_OBJECT_ITEM(ThinClient_Protocol, valueint);
		JSON_GET_OBJECT_ITEM(ThinClient_Name, valuestring);
		JSON_GET_OBJECT_ITEM(ThinClient_IP, valuestring);
		JSON_GET_OBJECT_ITEM(ThinClient_MAC, valuestring);

	#undef JSON_GET_OBJECT_ITEM

		if (!hasItem) goto END;

		ret = ThinClient_Update(t, keyList);
	}

END:
	cJSON_Delete(json);

	cJSON *result = cJSON_CreateObject();
	cJSON_AddNumberToObject(result, "Result", ret);
	char *out = cJSON_Print(result);
	cJSON_Delete(result);

	return string(out);
}

std::string CDC_ThinClient::CDC_ThinClient_Find(const std::string& req)
{
	int ret = 0;
	bool hasItem = false;
	bool isAll = true;
	cJSON *json, *tmp, *element;
	TCDC_ThinClient t;
	list<TCDC_ThinClient> lst;
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
		JSON_GET_OBJECT_ITEM("ThinClient_ID");
		if (tmp) ss << tmp->valuedouble;

		JSON_GET_OBJECT_ITEM("ThinClient_ThinGroup_ID");
		if (tmp) ss << tmp->valuedouble;

		JSON_GET_OBJECT_ITEM("ThinClient_Mode");
		if (tmp) ss << tmp->valueint;

		JSON_GET_OBJECT_ITEM("ThinClient_Version");
		if (tmp) ss << "'" << tmp->valuestring << "'";

		JSON_GET_OBJECT_ITEM("ThinClient_State");
		if (tmp) ss << tmp->valueint;

		JSON_GET_OBJECT_ITEM("ThinClient_Protocol");
		if (tmp) ss << tmp->valueint;

		JSON_GET_OBJECT_ITEM("ThinClient_Name");
		if (tmp) ss << "'" << tmp->valuestring << "'";

		JSON_GET_OBJECT_ITEM("ThinClient_IP");
		if (tmp) ss << "'" << tmp->valuestring << "'";

		JSON_GET_OBJECT_ITEM("ThinClient_MAC");
		if (tmp) ss << "'" << tmp->valuestring << "'";

	#undef JSON_GET_OBJECT_ITEM

		if (!hasItem) goto END;

		string whereSql = ss.str();
		ret = ThinClient_Find2(whereSql, t);
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
		cJSON_AddNumberToObject(data, "ThinClient_ID", t.ThinClient_ID);\
		cJSON_AddNumberToObject(data, "ThinClient_ThinGroup_ID", t.ThinClient_ThinGroup_ID);\
		cJSON_AddNumberToObject(data, "ThinClient_Mode", t.ThinClient_Mode);\
		cJSON_AddStringToObject(data, "ThinClient_Version", t.ThinClient_Version.c_str());\
		cJSON_AddNumberToObject(data, "ThinClient_State", t.ThinClient_State);\
		cJSON_AddNumberToObject(data, "ThinClient_Protocol", t.ThinClient_Protocol);\
		cJSON_AddStringToObject(data, "ThinClient_Name", t.ThinClient_Name.c_str());\
		cJSON_AddStringToObject(data, "ThinClient_IP", t.ThinClient_IP.c_str());\
		cJSON_AddStringToObject(data, "ThinClient_MAC", t.ThinClient_MAC.c_str());

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
				for (list<TCDC_ThinClient>::iterator it = lst.begin(); it != lst.end(); ++it)
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

std::string CDC_ThinClient::CDC_ThinClient_FindCount(const std::string& req)
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
			count = ThinClient_Count();
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
		JSON_GET_OBJECT_ITEM("ThinClient_ID");
		if (tmp) ss << tmp->valuedouble;

		JSON_GET_OBJECT_ITEM("ThinClient_ThinGroup_ID");
		if (tmp) ss << tmp->valuedouble;

		JSON_GET_OBJECT_ITEM("ThinClient_Mode");
		if (tmp) ss << tmp->valueint;

		JSON_GET_OBJECT_ITEM("ThinClient_Version");
		if (tmp) ss << "'" << tmp->valuestring << "'";

		JSON_GET_OBJECT_ITEM("ThinClient_State");
		if (tmp) ss << tmp->valueint;

		JSON_GET_OBJECT_ITEM("ThinClient_Protocol");
		if (tmp) ss << tmp->valueint;

		JSON_GET_OBJECT_ITEM("ThinClient_Name");
		if (tmp) ss << "'" << tmp->valuestring << "'";

		JSON_GET_OBJECT_ITEM("ThinClient_IP");
		if (tmp) ss << "'" << tmp->valuestring << "'";

		JSON_GET_OBJECT_ITEM("ThinClient_MAC");
		if (tmp) ss << "'" << tmp->valuestring << "'";

#undef JSON_GET_OBJECT_ITEM

		if (!hasItem) goto END;

		string whereSql = ss.str();
		count = ThinClient_Count(whereSql);
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

double CDC_ThinClient::ThinClient_Add(TCDC_ThinClient& src)
{
	double id = -1;
	try
	{
		if (ThinClient_Find(src.ThinClient_ID))
			return exists;

		_stmt = _db.compileStatement("insert into CDC_ThinClient values (?, ?, ?, ?, ?, ?, ?, ?, ?);");
		if (src.ThinClient_ID != INVALID_NUM && src.ThinClient_ID > 0)
			id = src.ThinClient_ID;
		else
			id = GetMaxID() + 1;
		_stmt.bind(1, id);
		_stmt.bind(2, src.ThinClient_ThinGroup_ID);
		_stmt.bind(3, src.ThinClient_Mode);
		_stmt.bind(4, src.ThinClient_Version);
		_stmt.bind(5, src.ThinClient_State);
		_stmt.bind(6, src.ThinClient_Protocol);
		_stmt.bind(7, src.ThinClient_Name);
		_stmt.bind(8, src.ThinClient_IP);
		_stmt.bind(9, src.ThinClient_MAC);

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

int CDC_ThinClient::ThinClient_Del(double id)
{
	try
	{
		if (!ThinClient_Find(id))
			return notExists;

		_stmt = _db.compileStatement("delete from CDC_ThinClient where ThinClient_ID = ?;");
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

int CDC_ThinClient::ThinClient_Update(TCDC_ThinClient& src)
{
	try
	{
		if (!ThinClient_Find(src.ThinClient_ID))
			return notExists;

		_stmt = _db.compileStatement("update CDC_ThinClient \
			set ThinClient_HostID = ?, ThinClient_Name = ?, ThinClient_Path = ? where ThinClient_ID = ?;");
		
		_stmt.bind(1, src.ThinClient_ID);
		_stmt.bind(2, src.ThinClient_ThinGroup_ID);
		_stmt.bind(3, src.ThinClient_Mode);
		_stmt.bind(4, src.ThinClient_Version);
		_stmt.bind(5, src.ThinClient_State);
		_stmt.bind(6, src.ThinClient_Protocol);
		_stmt.bind(7, src.ThinClient_Name);
		_stmt.bind(8, src.ThinClient_IP);
		_stmt.bind(9, src.ThinClient_MAC);

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

int CDC_ThinClient::ThinClient_Update(TCDC_ThinClient& src, std::list<std::string>& keyList)
{
	try
	{
		list<string>::iterator it = find(keyList.begin(), keyList.end(), "ThinClient_ID");
		if (it == keyList.end())
			return inputConditionError;

		if (!ThinClient_Find(src.ThinClient_ID))
			return notExists;

		string updateSql = "update CDC_ThinClient set ";
		for (list<string>::iterator it = keyList.begin(); it != keyList.end(); ++it)
		{
			if (*it != "ThinClient_ID")
				updateSql = updateSql + *it + " = ?, ";
		}
		updateSql = Trim(updateSql);
		updateSql = updateSql.substr(0, updateSql.size() - 1);
		updateSql += " where ThinClient_ID = ?;";

		_stmt = _db.compileStatement(updateSql.c_str());

		int index = 1;
		for (list<string>::iterator it = keyList.begin(); it != keyList.end(); ++it)
		{
			if (*it == "ThinClient_ThinGroup_ID")
				_stmt.bind(index++, src.ThinClient_ThinGroup_ID);
			else if (*it == "ThinClient_Mode")
				_stmt.bind(index++, src.ThinClient_Mode);
			else if (*it == "ThinClient_Version")
				_stmt.bind(index++, src.ThinClient_Version);
			else if (*it == "ThinClient_State")
				_stmt.bind(index++, src.ThinClient_State);
			else if (*it == "ThinClient_Protocol")
				_stmt.bind(index++, src.ThinClient_Protocol);
			else if (*it == "ThinClient_Name")
				_stmt.bind(index++, src.ThinClient_Name);
			else if (*it == "ThinClient_IP")
				_stmt.bind(index++, src.ThinClient_IP);
			else if (*it == "ThinClient_MAC")
				_stmt.bind(index++, src.ThinClient_MAC);
		}
		_stmt.bind(index, src.ThinClient_ID);

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

bool CDC_ThinClient::ThinClient_Find(double id)
{
	if (id == INVALID_NUM)
		return false;
	try
	{
		char buf[1024] = { 0 };
		sprintf(buf, "select count(*) from CDC_ThinClient where ThinClient_ID = %f;", id);
		return (_db.execScalar(buf) != 0);
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
		return false;
	}
	return true;
}

int CDC_ThinClient::ThinClient_Find(double id, TCDC_ThinClient& t)
{
	try
	{
		if (!ThinClient_Find(id))
			return notExists;

		CppMySQLQuery q;
		char buf[1024] = { 0 };
		sprintf(buf, "select * from CDC_ThinClient where ThinClient_ID = %f;", id);

		q = _db.execQuery(buf);

		if (!q.eof())
		{
			t.ThinClient_ID = q.getDoubleField(0);
			t.ThinClient_ThinGroup_ID = q.getDoubleField(1);
			t.ThinClient_Mode = q.getIntField(2);
			t.ThinClient_Version = q.getStringField(3);
			t.ThinClient_State = q.getIntField(4);
			t.ThinClient_Protocol = q.getIntField(5);
			t.ThinClient_Name = q.getStringField(6);
			t.ThinClient_IP = q.getStringField(7);
			t.ThinClient_MAC = q.getStringField(8);
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

int CDC_ThinClient::ThinClient_Find2(const std::string& whereSql, TCDC_ThinClient& t)
{
	try
	{
		CppMySQLQuery q;
		char buf[1024] = { 0 };
		sprintf(buf, "select * from CDC_ThinClient where %s;", whereSql.c_str());

		q = _db.execQuery(buf);

		if (!q.eof())
		{
			t.ThinClient_ID = q.getDoubleField(0);
			t.ThinClient_ThinGroup_ID = q.getDoubleField(1);
			t.ThinClient_Mode = q.getIntField(2);
			t.ThinClient_Version = q.getStringField(3);
			t.ThinClient_State = q.getIntField(4);
			t.ThinClient_Protocol = q.getIntField(5);
			t.ThinClient_Name = q.getStringField(6);
			t.ThinClient_IP = q.getStringField(7);
			t.ThinClient_MAC = q.getStringField(8);
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

int CDC_ThinClient::ThinClient_Count()
{
	try
	{
		return _db.execScalar("select count(*) from CDC_ThinClient;");
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
		return DBError;
	}
	return 0;
}

int CDC_ThinClient::ThinClient_Count(const std::string& whereSql)
{
	try
	{
		char buf[1024] = { 0 };
		sprintf(buf, "select count(*) from CDC_ThinClient where %s;", whereSql.c_str());

		return _db.execScalar(buf);
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
		return DBError;
	}
	return 0;
}

std::list<TCDC_ThinClient> CDC_ThinClient::GetAll()
{
	std::list<TCDC_ThinClient> lst;
	try
	{
		CppMySQLQuery q = _db.execQuery("select * from CDC_ThinClient;");
		while (!q.eof())
		{
			TCDC_ThinClient t;
			t.ThinClient_ID = q.getDoubleField(0);
			t.ThinClient_ThinGroup_ID = q.getDoubleField(1);
			t.ThinClient_Mode = q.getIntField(2);
			t.ThinClient_Version = q.getStringField(3);
			t.ThinClient_State = q.getIntField(4);
			t.ThinClient_Protocol = q.getIntField(5);
			t.ThinClient_Name = q.getStringField(6);
			t.ThinClient_IP = q.getStringField(7);
			t.ThinClient_MAC = q.getStringField(8);

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

double CDC_ThinClient::GetMaxID()
{
	char buf[1024] = { 0 };
	sprintf(buf, "select max(ThinClient_ID) from CDC_ThinClient");
	CppMySQLQuery q = _db.execQuery(buf);

	if (q.eof() || q.numFields() < 1)
	{
		throw CppMySQLException(CPPMYSQL_DB_ERROR, "Invalid scalar query");
	}

	return q.getDoubleField(0);
}