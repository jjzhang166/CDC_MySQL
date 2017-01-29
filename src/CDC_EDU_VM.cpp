#include "CDC_EDU_VM.h"
#include "LogHelper.h"
#include "ComDef.h"
#include "cJSON.h"
#include "utility.h"
#include <stdio.h>
#include <assert.h> 
#include <stdio.h>
#include <algorithm>

using namespace std;

CDC_EDU_VM::CDC_EDU_VM(CppMySQLDB* pdb)
	:_pdb(pdb)
{
}


CDC_EDU_VM::~CDC_EDU_VM()
{
	_stmt.clear();
}


std::string CDC_EDU_VM::CDC_EDU_VM_Add(const std::string& req)
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

		TCDC_EDU_VM t;
		JSON_ADD_ONE_ELEMENT2(EDU_VM_ID, valuedouble);
		JSON_ADD_ONE_ELEMENT(EDU_VM_Template_ID, valuedouble);
		JSON_ADD_ONE_ELEMENT(EDU_VM_ThinClient_ID, valuedouble);
		JSON_ADD_ONE_ELEMENT(EDU_VM_State, valueint);
		JSON_ADD_ONE_ELEMENT(EDU_VM_SysPath, valuestring);
		JSON_ADD_ONE_ELEMENT(EDU_VM_SysFilename, valuestring);
		JSON_ADD_ONE_ELEMENT(EDU_VM_MAC, valuestring);

#undef JSON_ADD_ONE_ELEMENT
#undef JSON_ADD_ONE_ELEMENT2

		id = EDU_VM_Add(t);
	}

END:
	cJSON_Delete(json);

	ret = (id >= 0) ? 1 : 0;
	cJSON *result = cJSON_CreateObject();
	cJSON_AddNumberToObject(result, "Result", ret);
	// if success then return id
	if (ret > 0)
		cJSON_AddNumberToObject(result, "EDU_VM_ID", id);
	char *out = cJSON_Print(result);
	cJSON_Delete(result);

	return string(out);
}

std::string CDC_EDU_VM::CDC_EDU_VM_Del(const std::string& req)
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
		tmp = cJSON_GetObjectItem(json, "EDU_VM_ID");
		if (tmp)
			id = tmp->valuedouble;
		else
			goto END;
		ret = EDU_VM_Del(id);
	}

END:
	cJSON_Delete(json);

	cJSON *result = cJSON_CreateObject();
	cJSON_AddNumberToObject(result, "Result", ret);
	char *out = cJSON_Print(result);
	cJSON_Delete(result);

	return string(out);
}

std::string CDC_EDU_VM::CDC_EDU_VM_Update(const std::string& req)
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

		TCDC_EDU_VM t;
		const char *key = "EDU_VM_ID";
		tmp = cJSON_GetObjectItem(json, key);
		if (tmp)
		{
			if (!hasItem) hasItem = true;
			t.EDU_VM_ID = tmp->valuedouble;
			keyList.push_back(key);
		}

		JSON_GET_OBJECT_ITEM(EDU_VM_ID, valuedouble);
		JSON_GET_OBJECT_ITEM(EDU_VM_Template_ID, valuedouble);
		JSON_GET_OBJECT_ITEM(EDU_VM_ThinClient_ID, valuedouble);
		JSON_GET_OBJECT_ITEM(EDU_VM_State, valueint);
		JSON_GET_OBJECT_ITEM(EDU_VM_SysPath, valuestring);
		JSON_GET_OBJECT_ITEM(EDU_VM_SysFilename, valuestring);
		JSON_GET_OBJECT_ITEM(EDU_VM_MAC, valuestring);

	#undef JSON_GET_OBJECT_ITEM

		if (!hasItem) goto END;

		ret = EDU_VM_Update(t, keyList);
	}

END:
	cJSON_Delete(json);

	cJSON *result = cJSON_CreateObject();
	cJSON_AddNumberToObject(result, "Result", ret);
	char *out = cJSON_Print(result);
	cJSON_Delete(result);

	return string(out);
}

std::string CDC_EDU_VM::CDC_EDU_VM_Find(const std::string& req)
{
	int ret = 0;
	bool hasItem = false;
	bool isAll = true;
	cJSON *json, *tmp;
	TCDC_EDU_VM t;
	list<TCDC_EDU_VM> lst;
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
		JSON_GET_OBJECT_ITEM("EDU_VM_ID");
		if (tmp) ss << tmp->valuedouble;

		JSON_GET_OBJECT_ITEM("EDU_VM_Template_ID");
		if (tmp) ss << tmp->valuedouble;

		JSON_GET_OBJECT_ITEM("EDU_VM_ThinClient_ID");
		if (tmp) ss << tmp->valuedouble;

		JSON_GET_OBJECT_ITEM("EDU_VM_State");
		if (tmp) ss << tmp->valueint;

		JSON_GET_OBJECT_ITEM("EDU_VM_SysPath");
		if (tmp) ss << "'" << tmp->valuestring << "'";

		JSON_GET_OBJECT_ITEM("EDU_VM_SysFilename");
		if (tmp) ss << "'" << tmp->valuestring << "'";

		JSON_GET_OBJECT_ITEM("EDU_VM_MAC");
		if (tmp) ss << "'" << tmp->valuestring << "'";

	#undef JSON_GET_OBJECT_ITEM

		if (!hasItem) goto END;

		string whereSql = ss.str();
		ret = EDU_VM_Find2(whereSql, t);
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
		cJSON_AddNumberToObject(data, "EDU_VM_ID", t.EDU_VM_ID);\
		cJSON_AddNumberToObject(data, "EDU_VM_Template_ID", t.EDU_VM_Template_ID);\
		cJSON_AddNumberToObject(data, "EDU_VM_ThinClient_ID", t.EDU_VM_ThinClient_ID);\
		cJSON_AddNumberToObject(data, "EDU_VM_State", t.EDU_VM_State);\
		cJSON_AddStringToObject(data, "EDU_VM_SysPath", t.EDU_VM_SysPath.c_str());\
		cJSON_AddStringToObject(data, "EDU_VM_SysFilename", t.EDU_VM_SysFilename.c_str());\
		cJSON_AddStringToObject(data, "EDU_VM_MAC", t.EDU_VM_MAC.c_str());

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
				for (list<TCDC_EDU_VM>::iterator it = lst.begin(); it != lst.end(); ++it)
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

std::string CDC_EDU_VM::CDC_EDU_VM_FindCount(const std::string& req)
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
			count = EDU_VM_Count();
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
		JSON_GET_OBJECT_ITEM("EDU_VM_ID");
		if (tmp) ss << tmp->valuedouble;
		//
		JSON_GET_OBJECT_ITEM("EDU_VM_Template_ID");
		if (tmp) ss << tmp->valuedouble;
		//
		JSON_GET_OBJECT_ITEM("EDU_VM_ThinClient_ID");
		if (tmp) ss << tmp->valuedouble;
		//
		JSON_GET_OBJECT_ITEM("EDU_VM_State");
		if (tmp) ss << tmp->valueint;
		//
		JSON_GET_OBJECT_ITEM("EDU_VM_SysPath");
		if (tmp) ss << "'" << tmp->valuestring << "'";
		//
		JSON_GET_OBJECT_ITEM("EDU_VM_SysFilename");
		if (tmp) ss << "'" << tmp->valuestring << "'";
		//
		JSON_GET_OBJECT_ITEM("EDU_VM_MAC");
		if (tmp) ss << "'" << tmp->valuestring << "'";


#undef JSON_GET_OBJECT_ITEM

		if (!hasItem) goto END;

		string whereSql = ss.str();
		count = EDU_VM_Count(whereSql);
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

double CDC_EDU_VM::EDU_VM_Add(TCDC_EDU_VM& src)
{
	double id = -1;
	try
	{
		if (EDU_VM_Find(src.EDU_VM_ID))
			return exists;

		_stmt = _pdb->compileStatement("insert into CDC_EDU_VM values (?, ?, ?, ?, ?, ?, ?);");
		if (src.EDU_VM_ID != INVALID_NUM && src.EDU_VM_ID > 0)
			id = src.EDU_VM_ID;
		else
			id = GetMaxID() + 1;
		_stmt.bind(1, id);
		_stmt.bind(2, src.EDU_VM_Template_ID);
		_stmt.bind(3, src.EDU_VM_ThinClient_ID);
		_stmt.bind(4, src.EDU_VM_State);
		_stmt.bind(5, src.EDU_VM_SysPath);
		_stmt.bind(6, src.EDU_VM_SysFilename);
		_stmt.bind(7, src.EDU_VM_MAC);

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

int CDC_EDU_VM::EDU_VM_Del(double id)
{
	try
	{
		if (!EDU_VM_Find(id))
			return notExists;

		_stmt = _pdb->compileStatement("delete from CDC_EDU_VM where EDU_VM_ID = ?;");
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

int CDC_EDU_VM::EDU_VM_Update(TCDC_EDU_VM& src, std::list<std::string>& keyList)
{
	try
	{
		list<string>::iterator it = find(keyList.begin(), keyList.end(), "EDU_VM_ID");
		if (it == keyList.end())
			return inputConditionError;

		if (!EDU_VM_Find(src.EDU_VM_ID))
			return notExists;

		string updateSql = "update CDC_EDU_VM set ";
		for (list<string>::iterator it = keyList.begin(); it != keyList.end(); ++it)
		{
			if (*it != "EDU_VM_ID")
				updateSql = updateSql + *it + " = ?, ";
		}
		updateSql = Trim(updateSql);
		updateSql = updateSql.substr(0, updateSql.size() - 1);
		updateSql += " where EDU_VM_ID = ?;";

		_stmt = _pdb->compileStatement(updateSql.c_str());

		int index = 1;
		for (list<string>::iterator it = keyList.begin(); it != keyList.end(); ++it)
		{
			if (*it == "EDU_VM_Template_ID")
				_stmt.bind(index++, src.EDU_VM_Template_ID);
			else if (*it == "EDU_VM_ThinClient_ID")
				_stmt.bind(index++, src.EDU_VM_ThinClient_ID);
			else if (*it == "EDU_VM_State")
				_stmt.bind(index++, src.EDU_VM_State);
			else if (*it == "EDU_VM_SysPath")
				_stmt.bind(index++, src.EDU_VM_SysPath);
			else if (*it == "EDU_VM_SysFilename")
				_stmt.bind(index++, src.EDU_VM_SysFilename);
			else if (*it == "EDU_VM_MAC")
				_stmt.bind(index++, src.EDU_VM_MAC);
		}
		_stmt.bind(index, src.EDU_VM_ID);

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

bool CDC_EDU_VM::EDU_VM_Find(double id)
{
	if (id == INVALID_NUM)
		return false;
	try
	{
		char buf[1024] = { 0 };
		sprintf(buf, "select count(*) from CDC_EDU_VM where EDU_VM_ID = %f;", id);
		return (_pdb->execScalar(buf) != 0);
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
		return false;
	}
	return true;
}

int CDC_EDU_VM::EDU_VM_Find(double id, TCDC_EDU_VM& t)
{
	try
	{
		if (!EDU_VM_Find(id))
			return notExists;

		CppMySQLQuery q;
		char buf[1024] = { 0 };
		sprintf(buf, "select * from CDC_EDU_VM where EDU_VM_ID = %f;", id);

		q = _pdb->execQuery(buf);

		if (!q.eof())
		{
			t.EDU_VM_ID = q.getDoubleField(0);
			t.EDU_VM_Template_ID = q.getDoubleField(1);
			t.EDU_VM_ThinClient_ID = q.getDoubleField(2);
			t.EDU_VM_State = q.getIntField(3);
			t.EDU_VM_SysPath = q.fieldValue(4);
			t.EDU_VM_SysFilename = q.fieldValue(5);
			t.EDU_VM_MAC = q.fieldValue(6);
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

int CDC_EDU_VM::EDU_VM_Find2(const std::string& whereSql, TCDC_EDU_VM& t)
{
	try
	{
		CppMySQLQuery q;
		char buf[1024] = { 0 };
		sprintf(buf, "select * from CDC_EDU_VM where %s;", whereSql.c_str());

		q = _pdb->execQuery(buf);

		if (!q.eof())
		{
			t.EDU_VM_ID = q.getDoubleField(0);
			t.EDU_VM_Template_ID = q.getDoubleField(1);
			t.EDU_VM_ThinClient_ID = q.getDoubleField(2);
			t.EDU_VM_State = q.getIntField(3);
			t.EDU_VM_SysPath = q.fieldValue(4);
			t.EDU_VM_SysFilename = q.fieldValue(5);
			t.EDU_VM_MAC = q.fieldValue(6);
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

int CDC_EDU_VM::EDU_VM_Count()
{
	try
	{
		return _pdb->execScalar("select count(*) from CDC_EDU_VM;");
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
		return DBError;
	}
	return 0;
}

int CDC_EDU_VM::EDU_VM_Count(const std::string& whereSql)
{
	try
	{
		char buf[1024] = { 0 };
		sprintf(buf, "select count(*) from CDC_EDU_VM where %s;", whereSql.c_str());

		return _pdb->execScalar(buf);
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
		return DBError;
	}
	return 0;
}

std::list<TCDC_EDU_VM> CDC_EDU_VM::GetAll()
{
	std::list<TCDC_EDU_VM> lst;
	try
	{
		CppMySQLQuery q = _pdb->execQuery("select * from CDC_EDU_VM;");
		while (!q.eof())
		{
			TCDC_EDU_VM t;
			t.EDU_VM_ID = q.getDoubleField(0);
			t.EDU_VM_Template_ID = q.getDoubleField(1);
			t.EDU_VM_ThinClient_ID = q.getDoubleField(2);
			t.EDU_VM_State = q.getIntField(3);
			t.EDU_VM_SysPath = q.fieldValue(4);
			t.EDU_VM_SysFilename = q.fieldValue(5);
			t.EDU_VM_MAC = q.fieldValue(6);

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

double CDC_EDU_VM::GetMaxID()
{
	char buf[1024] = { 0 };
	sprintf(buf, "select max(EDU_VM_ID) from CDC_EDU_VM");
	CppMySQLQuery q = _pdb->execQuery(buf);

	if (q.eof() || q.numFields() < 1)
	{
		throw CppMySQLException(CPPMYSQL_DB_ERROR, "Invalid scalar query");
	}

	return q.getDoubleField(0);
}