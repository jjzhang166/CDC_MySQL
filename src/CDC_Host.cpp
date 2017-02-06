#include "CDC_Host.h"
#include "LogHelper.h"
#include "ComDef.h"
#include "cJSON.h"
#include "utility.h"
#include <stdio.h>
#include <assert.h> 
#include <stdio.h>
#include <algorithm>

using namespace std;

CDC_Host::CDC_Host(CppMySQLDB* pdb)
	:_pdb(pdb)
{
}


CDC_Host::~CDC_Host()
{
	_stmt.clear();
}


std::string CDC_Host::CDC_Host_Add(const std::string& req)
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

		TCDC_Host t;
		JSON_ADD_ONE_ELEMENT(Host_Name, valuestring);
		JSON_ADD_ONE_ELEMENT(Host_IP, valuestring);
		JSON_ADD_ONE_ELEMENT(Host_VirtNet, valuestring);

#undef JSON_ADD_ONE_ELEMENT

		id = Host_Add(t);
	}

END:
	cJSON_Delete(json);

	ret = (id >= 0) ? 1 : 0;
	cJSON *result = cJSON_CreateObject();
	cJSON_AddNumberToObject(result, "Result", ret);
	// if success then return id
	if (ret > 0)
		cJSON_AddNumberToObject(result, "Host_ID", id);
	char *out = cJSON_Print(result);
	cJSON_Delete(result);

	return string(out);
}

std::string CDC_Host::CDC_Host_Del(const std::string& req)
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
		tmp = cJSON_GetObjectItem(json, "Host_ID");
		if (tmp)
			id = tmp->valuedouble;
		else
			goto END;
		ret = Host_Del(id);
	}

END:
	cJSON_Delete(json);

	cJSON *result = cJSON_CreateObject();
	cJSON_AddNumberToObject(result, "Result", ret);
	char *out = cJSON_Print(result);
	cJSON_Delete(result);

	return string(out);
}

std::string CDC_Host::CDC_Host_Update(const std::string& req)
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

		TCDC_Host t;
		const char *key = "Host_ID";
		tmp = cJSON_GetObjectItem(json, key);
		if (tmp)
		{
			if (!hasItem) hasItem = true;
			t.Host_ID = tmp->valuedouble;
			keyList.push_back(key);
		}
		JSON_GET_OBJECT_ITEM(Host_ID, valuedouble);
		JSON_GET_OBJECT_ITEM(Host_Name, valuestring);
		JSON_GET_OBJECT_ITEM(Host_IP, valuestring);
		JSON_GET_OBJECT_ITEM(Host_VirtNet, valuestring);

	#undef JSON_GET_OBJECT_ITEM

		if (!hasItem) goto END;

		ret = Host_Update(t, keyList);
	}

END:
	cJSON_Delete(json);

	cJSON *result = cJSON_CreateObject();
	cJSON_AddNumberToObject(result, "Result", ret);
	char *out = cJSON_Print(result);
	cJSON_Delete(result);

	return string(out);
}

std::string CDC_Host::CDC_Host_Find(const std::string& req)
{
	int ret = 0;
	bool hasItem = false;
	bool isAll = true;
	cJSON *json, *tmp;
	TCDC_Host t;
	list<TCDC_Host> lst;
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
		JSON_GET_OBJECT_ITEM("Host_ID");
		if (tmp) ss << tmp->valuedouble;

		JSON_GET_OBJECT_ITEM("Host_Name");
		if (tmp) ss << "'" << tmp->valuestring << "'";

		JSON_GET_OBJECT_ITEM("Host_IP");
		if (tmp) ss << "'" << tmp->valuestring << "'";

		JSON_GET_OBJECT_ITEM("Host_VirtNet");
		if (tmp) ss << "'" << tmp->valuestring << "'";

	#undef JSON_GET_OBJECT_ITEM

		if (!hasItem) goto END;

		string whereSql = ss.str();
		lst = Host_Find2(whereSql);
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
		cJSON_AddNumberToObject(data, "Host_ID", t.Host_ID);\
		cJSON_AddStringToObject(data, "Host_Name", t.Host_Name.c_str());\
		cJSON_AddStringToObject(data, "Host_IP", t.Host_IP.c_str());\
		cJSON_AddStringToObject(data, "Host_VirtNet", t.Host_VirtNet.c_str());

		cJSON *dataArr, *data = 0;
		cJSON_AddNumberToObject(result, "Result", lst.size());
		if (lst.size() > 1)
		{
			cJSON_AddItemToObject(result, "Data", dataArr = cJSON_CreateArray());
			for (list<TCDC_Host>::iterator it = lst.begin(); it != lst.end(); ++it)
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

std::string CDC_Host::CDC_Host_FindCount(const std::string& req)
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
			count = Host_Count();
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
		JSON_GET_OBJECT_ITEM("Host_ID");
		if (tmp) ss << tmp->valuedouble;

		JSON_GET_OBJECT_ITEM("Host_Name");
		if (tmp) ss << "'" << tmp->valuestring << "'";

		JSON_GET_OBJECT_ITEM("Host_IP");
		if (tmp) ss << "'" << tmp->valuestring << "'";

		JSON_GET_OBJECT_ITEM("Host_VirtNet");
		if (tmp) ss << "'" << tmp->valuestring << "'";

#undef JSON_GET_OBJECT_ITEM

		if (!hasItem) goto END;

		string whereSql = ss.str();
		count = Host_Count(whereSql);
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

double CDC_Host::Host_Add(TCDC_Host& src)
{
	double id = -1;
	try
	{
		if (Host_Find(src.Host_ID))
			return exists;

		_stmt = _pdb->compileStatement("insert into CDC_Host values (?, ?, ?, ?);");
		if (src.Host_ID != INVALID_NUM && src.Host_ID > 0)
			id = src.Host_ID;
		else
			id = GetMaxID() + 1;
		_stmt.bind(1, id);
		_stmt.bind(2, src.Host_Name);
		_stmt.bind(3, src.Host_IP);
		_stmt.bind(4, src.Host_VirtNet);

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

int CDC_Host::Host_Del(double id)
{
	try
	{
		if (!Host_Find(id))
			return notExists;

		_stmt = _pdb->compileStatement("delete from CDC_Host where Host_ID = ?;");
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

int CDC_Host::Host_Del(const std::string& name)
{
	try
	{
		if (!Host_Find(name))
			return notExists;

		char buf[1024] = { 0 };
		sprintf(buf, "delete from CDC_Host where Host_Name = %s;", name.c_str());
		_pdb->execDML(buf);
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
		return DBError;
	}
	return success;
}

int CDC_Host::Host_Update(TCDC_Host& src)
{
	try
	{
		if (!Host_Find(src.Host_ID))
			return notExists;

		_stmt = _pdb->compileStatement("update CDC_Host \
			set Host_Name = ?, Host_IP = ?, Host_VirtNet = ? where Host_ID = ?;");
		
		_stmt.bind(1, src.Host_Name);
		_stmt.bind(2, src.Host_IP);
		_stmt.bind(3, src.Host_VirtNet);
		_stmt.bind(4, src.Host_ID);

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

int CDC_Host::Host_Update(TCDC_Host& src, std::list<std::string>& keyList)
{
	try
	{
		list<string>::iterator it = find(keyList.begin(), keyList.end(), "Host_ID");
		if (it == keyList.end())
			return inputConditionError;

		if (!Host_Find(src.Host_ID))
			return notExists;

		string updateSql = "update CDC_Host set ";
		for (list<string>::iterator it = keyList.begin(); it != keyList.end(); ++it)
		{
			if (*it != "Host_ID")
				updateSql = updateSql + *it + " = ?, ";
		}
		updateSql = Trim(updateSql);
		updateSql = updateSql.substr(0, updateSql.size() - 1);
		updateSql += " where Host_ID = ?;";

		_stmt = _pdb->compileStatement(updateSql.c_str());

		int index = 1;
		for (list<string>::iterator it = keyList.begin(); it != keyList.end(); ++it)
		{
			if (*it == "Host_Name")
				_stmt.bind(index++, src.Host_Name);
			else if (*it == "Host_IP")
				_stmt.bind(index++, src.Host_IP);
			else if (*it == "Host_VirtNet")
				_stmt.bind(index++, src.Host_VirtNet);
		}
		_stmt.bind(index, src.Host_ID);

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

bool CDC_Host::Host_Find(double id)
{
	if (id == INVALID_NUM)
		return false;
	try
	{
		char buf[1024] = { 0 };
		sprintf(buf, "select count(*) from CDC_Host where Host_ID = %f;", id);
		return (_pdb->execScalar(buf) != 0);
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
		return false;
	}
	return true;
}

bool CDC_Host::Host_Find(const std::string& name)
{
	try
	{
		char buf[1024] = { 0 };
		sprintf(buf, "select count(*) from CDC_Host where Host_Name = '%s';", name.c_str());
		int dd = _pdb->execScalar(buf);
		return (_pdb->execScalar(buf) != 0);
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
		return false;
	}
	return true;
}


int CDC_Host::Host_Find(double id, TCDC_Host& t)
{
	try
	{
		if (!Host_Find(id))
			return notExists;

		CppMySQLQuery q;
		char buf[1024] = { 0 };
		sprintf(buf, "select * from CDC_Host where Host_ID = %f;", id);

		q = _pdb->execQuery(buf);

		if (!q.eof())
		{
			t.Host_ID = q.getDoubleField("Host_ID");
			t.Host_Name = q.fieldValue("Host_Name");
			t.Host_IP = q.fieldValue("Host_IP");
			t.Host_VirtNet = q.fieldValue("Host_VirtNet");
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

int CDC_Host::Host_Find(const std::string& name, TCDC_Host& t)
{
	try
	{
		if (!Host_Find(name))
			return notExists;

		CppMySQLQuery q;
		char buf[1024] = { 0 };
		sprintf(buf, "select * from CDC_Host where Host_Name = '%s';", name.c_str());

		q = _pdb->execQuery(buf);

		if (!q.eof())
		{
			t.Host_ID = q.getDoubleField("Host_ID");
			t.Host_Name = q.fieldValue("Host_Name");
			t.Host_IP = q.fieldValue("Host_IP");
			t.Host_VirtNet = q.fieldValue("Host_VirtNet");
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

int CDC_Host::Host_Find2(const std::string& whereSql, TCDC_Host& t)
{
	try
	{
		CppMySQLQuery q;
		char buf[1024] = { 0 };
		sprintf(buf, "select * from CDC_Host where %s;", whereSql.c_str());

		q = _pdb->execQuery(buf);

		if (!q.eof())
		{
			t.Host_ID = q.getDoubleField("Host_ID");
			t.Host_Name = q.fieldValue("Host_Name");
			t.Host_IP = q.fieldValue("Host_IP");
			t.Host_VirtNet = q.fieldValue("Host_VirtNet");
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

std::list<TCDC_Host> CDC_Host::Host_Find2(const std::string& whereSql)
{
	std::list<TCDC_Host> lst;
	try
	{
		CppMySQLQuery q;
		char buf[1024] = { 0 };
		sprintf(buf, "select * from CDC_Host where %s;", whereSql.c_str());

		q = _pdb->execQuery(buf);
		while (!q.eof())
		{
			TCDC_Host t;
			t.Host_ID = q.getDoubleField(0);
			t.Host_Name = q.getStringField(1);
			t.Host_IP = q.getStringField(2);
			t.Host_VirtNet = q.getStringField(3);

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

int CDC_Host::Host_Count()
{
	try
	{
		return _pdb->execScalar("select count(*) from CDC_Host;");
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
		return DBError;
	}
	return 0;
}

int CDC_Host::Host_Count(const std::string& whereSql)
{
	try
	{
		char buf[1024] = { 0 };
		sprintf(buf, "select count(*) from CDC_Host where %s;", whereSql.c_str());

		return _pdb->execScalar(buf);
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
		return DBError;
	}
	return 0;
}

std::list<TCDC_Host> CDC_Host::GetAll()
{
	std::list<TCDC_Host> lst;
	try
	{
		CppMySQLQuery q = _pdb->execQuery("select * from CDC_Host;");
		while (!q.eof())
		{
			TCDC_Host t;
			t.Host_ID = q.getDoubleField(0);
			t.Host_Name = q.getStringField(1);
			t.Host_IP = q.getStringField(2);
			t.Host_VirtNet = q.getStringField(3);

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

double CDC_Host::GetMaxID()
{
	char buf[1024] = { 0 };
	sprintf(buf, "select max(Host_ID) from CDC_Host");
	CppMySQLQuery q = _pdb->execQuery(buf);

	if (q.eof() || q.numFields() < 1)
	{
		throw CppMySQLException(CPPMYSQL_DB_ERROR, "Invalid scalar query");
	}

	return q.getDoubleField(0);
}