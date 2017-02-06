#include "CDC_VM.h"
#include "LogHelper.h"
#include "ComDef.h"
#include "cJSON.h"
#include "utility.h"
#include <stdio.h>
#include <assert.h> 
#include <stdio.h>
#include <algorithm>

using namespace std;

CDC_VM::CDC_VM(CppMySQLDB* pdb)
	:_pdb(pdb)
{
}


CDC_VM::~CDC_VM()
{
	_stmt.clear();
}


std::string CDC_VM::CDC_VM_Add(const std::string& req)
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

		TCDC_VM t;
		JSON_ADD_ONE_ELEMENT2(VM_ID, valuedouble);
		JSON_ADD_ONE_ELEMENT(VM_Template_ID, valuedouble);
		JSON_ADD_ONE_ELEMENT(VM_Host_ID, valuedouble);
		JSON_ADD_ONE_ELEMENT2(VM_ThinClient_ID, valuedouble);
		JSON_ADD_ONE_ELEMENT(VM_CloneMode, valueint);
		JSON_ADD_ONE_ELEMENT(VM_SysReadOnly, valueint);
		JSON_ADD_ONE_ELEMENT2(VM_DataSize, valueint);
		JSON_ADD_ONE_ELEMENT(VM_UserGroup_ID, valuedouble);
		JSON_ADD_ONE_ELEMENT(VM_User_ID, valuedouble);
		JSON_ADD_ONE_ELEMENT(VM_State, valueint);
		JSON_ADD_ONE_ELEMENT(VM_OS, valuestring);
		JSON_ADD_ONE_ELEMENT(VM_SysPath, valuestring);
		JSON_ADD_ONE_ELEMENT(VM_SysFilename, valuestring);
		JSON_ADD_ONE_ELEMENT2(VM_DataPath, valuestring);
		JSON_ADD_ONE_ELEMENT2(VM_DataFilename, valuestring);
		JSON_ADD_ONE_ELEMENT(VM_MAC, valuestring);
		JSON_ADD_ONE_ELEMENT(VM_SpicePwd, valuestring);
		JSON_ADD_ONE_ELEMENT2(VM_Uuid, valuestring);
		JSON_ADD_ONE_ELEMENT2(VM_Create_Time, valuestring);
		JSON_ADD_ONE_ELEMENT2(VM_Update_Time, valuestring);

#undef JSON_ADD_ONE_ELEMENT
#undef JSON_ADD_ONE_ELEMENT2

		id = VM_Add(t);
	}

END:
	cJSON_Delete(json);

	ret = (id >= 0) ? 1 : 0;
	cJSON *result = cJSON_CreateObject();
	cJSON_AddNumberToObject(result, "Result", ret);
	// if success then return id
	if (ret > 0)
		cJSON_AddNumberToObject(result, "VM_ID", id);
	char *out = cJSON_Print(result);
	cJSON_Delete(result);

	return string(out);
}

std::string CDC_VM::CDC_VM_Del(const std::string& req)
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
		tmp = cJSON_GetObjectItem(json, "VM_ID");
		if (tmp)
			id = tmp->valuedouble;
		else
			goto END;
		ret = VM_Del(id);
	}

END:
	cJSON_Delete(json);

	cJSON *result = cJSON_CreateObject();
	cJSON_AddNumberToObject(result, "Result", ret);
	char *out = cJSON_Print(result);
	cJSON_Delete(result);

	return string(out);
}

std::string CDC_VM::CDC_VM_Update(const std::string& req)
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

		TCDC_VM t;
		const char *key = "VM_ID";
		tmp = cJSON_GetObjectItem(json, key);
		if (tmp)
		{
			if (!hasItem) hasItem = true;
			t.VM_ID = tmp->valuedouble;
			keyList.push_back(key);
		}

		JSON_GET_OBJECT_ITEM(VM_ID, valuedouble);
		JSON_GET_OBJECT_ITEM(VM_Template_ID, valuedouble);
		JSON_GET_OBJECT_ITEM(VM_Host_ID, valuedouble);
		JSON_GET_OBJECT_ITEM(VM_ThinClient_ID, valuedouble);
		JSON_GET_OBJECT_ITEM(VM_CloneMode, valueint);
		JSON_GET_OBJECT_ITEM(VM_SysReadOnly, valueint);
		JSON_GET_OBJECT_ITEM(VM_DataSize, valueint);
		JSON_GET_OBJECT_ITEM(VM_UserGroup_ID, valuedouble);
		JSON_GET_OBJECT_ITEM(VM_User_ID, valuedouble);
		JSON_GET_OBJECT_ITEM(VM_State, valueint);
		JSON_GET_OBJECT_ITEM(VM_OS, valuestring);
		JSON_GET_OBJECT_ITEM(VM_SysPath, valuestring);
		JSON_GET_OBJECT_ITEM(VM_SysFilename, valuestring);
		JSON_GET_OBJECT_ITEM(VM_DataPath, valuestring);
		JSON_GET_OBJECT_ITEM(VM_DataFilename, valuestring);
		JSON_GET_OBJECT_ITEM(VM_MAC, valuestring);
		JSON_GET_OBJECT_ITEM(VM_SpicePwd, valuestring);
		JSON_GET_OBJECT_ITEM(VM_Uuid, valuestring);
		JSON_GET_OBJECT_ITEM(VM_Create_Time, valuestring);
		JSON_GET_OBJECT_ITEM(VM_Update_Time, valuestring);

	#undef JSON_GET_OBJECT_ITEM

		if (!hasItem) goto END;

		ret = VM_Update(t, keyList);
	}

END:
	cJSON_Delete(json);

	cJSON *result = cJSON_CreateObject();
	cJSON_AddNumberToObject(result, "Result", ret);
	char *out = cJSON_Print(result);
	cJSON_Delete(result);

	return string(out);
}

std::string CDC_VM::CDC_VM_Find(const std::string& req)
{
	int ret = 0;
	bool hasItem = false;
	bool isAll = true;
	cJSON *json, *tmp;
	TCDC_VM t;
	list<TCDC_VM> lst;
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
		JSON_GET_OBJECT_ITEM("VM_ID");
		if (tmp) ss << tmp->valuedouble;

		JSON_GET_OBJECT_ITEM("VM_Template_ID");
		if (tmp) ss << tmp->valuedouble;

		JSON_GET_OBJECT_ITEM("VM_Host_ID");
		if (tmp) ss << tmp->valuedouble;

		JSON_GET_OBJECT_ITEM("VM_ThinClient_ID");
		if (tmp) ss << tmp->valuedouble;

		JSON_GET_OBJECT_ITEM("VM_CloneMode");
		if (tmp) ss << tmp->valueint;

		JSON_GET_OBJECT_ITEM("VM_SysReadOnly");
		if (tmp) ss << tmp->valueint;

		JSON_GET_OBJECT_ITEM("VM_DataSize");
		if (tmp) ss << tmp->valueint;

		JSON_GET_OBJECT_ITEM("VM_UserGroup_ID");
		if (tmp) ss << tmp->valuedouble;

		JSON_GET_OBJECT_ITEM("VM_User_ID");
		if (tmp) ss << tmp->valuedouble;

		JSON_GET_OBJECT_ITEM("VM_State");
		if (tmp) ss << tmp->valueint;

		JSON_GET_OBJECT_ITEM("VM_OS");
		if (tmp) ss << "'" << tmp->valuestring << "'";

		JSON_GET_OBJECT_ITEM("VM_SysPath");
		if (tmp) ss << "'" << tmp->valuestring << "'";

		JSON_GET_OBJECT_ITEM("VM_SysFilename");
		if (tmp) ss << "'" << tmp->valuestring << "'";

		JSON_GET_OBJECT_ITEM("VM_DataPath");
		if (tmp) ss << "'" << tmp->valuestring << "'";

		JSON_GET_OBJECT_ITEM("VM_DataFilename");
		if (tmp) ss << "'" << tmp->valuestring << "'";

		JSON_GET_OBJECT_ITEM("VM_MAC");
		if (tmp) ss << "'" << tmp->valuestring << "'";

		JSON_GET_OBJECT_ITEM("VM_SpicePwd");
		if (tmp) ss << "'" << tmp->valuestring << "'";

		JSON_GET_OBJECT_ITEM("VM_Uuid");
		if (tmp) ss << "'" << tmp->valuestring << "'";

		JSON_GET_OBJECT_ITEM("VM_Create_Time");
		if (tmp) ss << "'" << tmp->valuestring << "'";

		JSON_GET_OBJECT_ITEM("VM_Update_Time");
		if (tmp) ss << "'" << tmp->valuestring << "'";

	#undef JSON_GET_OBJECT_ITEM

		if (!hasItem) goto END;

		string whereSql = ss.str();
		lst = VM_Find2(whereSql);
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
		cJSON_AddNumberToObject(data, "VM_ID", t.VM_ID);\
		cJSON_AddNumberToObject(data, "VM_Template_ID", t.VM_Template_ID);\
		cJSON_AddNumberToObject(data, "VM_Host_ID", t.VM_Host_ID);\
		cJSON_AddNumberToObject(data, "VM_ThinClient_ID", t.VM_ThinClient_ID);\
		cJSON_AddNumberToObject(data, "VM_CloneMode", t.VM_CloneMode);\
		cJSON_AddNumberToObject(data, "VM_SysReadOnly", t.VM_SysReadOnly);\
		cJSON_AddNumberToObject(data, "VM_DataSize", t.VM_DataSize);\
		cJSON_AddNumberToObject(data, "VM_UserGroup_ID", t.VM_UserGroup_ID);\
		cJSON_AddNumberToObject(data, "VM_User_ID", t.VM_User_ID);\
		cJSON_AddNumberToObject(data, "VM_State", t.VM_State);\
		cJSON_AddStringToObject(data, "VM_OS", t.VM_OS.c_str());\
		cJSON_AddStringToObject(data, "VM_SysPath", t.VM_SysPath.c_str());\
		cJSON_AddStringToObject(data, "VM_SysFilename", t.VM_SysFilename.c_str());\
		cJSON_AddStringToObject(data, "VM_DataPath", t.VM_DataPath.c_str());\
		cJSON_AddStringToObject(data, "VM_DataFilename", t.VM_DataFilename.c_str());\
		cJSON_AddStringToObject(data, "VM_MAC", t.VM_MAC.c_str());\
		cJSON_AddStringToObject(data, "VM_SpicePwd", t.VM_SpicePwd.c_str());\
		cJSON_AddStringToObject(data, "VM_Uuid", t.VM_Uuid.c_str());\
		cJSON_AddStringToObject(data, "VM_Create_Time", t.VM_Create_Time.c_str());\
		cJSON_AddStringToObject(data, "VM_Update_Time", t.VM_Update_Time.c_str());

		cJSON *dataArr, *data = 0;
		cJSON_AddNumberToObject(result, "Result", lst.size());
		if (lst.size() > 1)
		{
			cJSON_AddItemToObject(result, "Data", dataArr = cJSON_CreateArray());
			for (list<TCDC_VM>::iterator it = lst.begin(); it != lst.end(); ++it)
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

std::string CDC_VM::CDC_VM_FindCount(const std::string& req)
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
			count = VM_Count();
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
		JSON_GET_OBJECT_ITEM("VM_ID");
		if (tmp) ss << tmp->valuedouble;
		//
		JSON_GET_OBJECT_ITEM("VM_Template_ID");
		if (tmp) ss << tmp->valuedouble;
		//
		JSON_GET_OBJECT_ITEM("VM_Host_ID");
		if (tmp) ss << tmp->valuedouble;
		//
		JSON_GET_OBJECT_ITEM("VM_ThinClient_ID");
		if (tmp) ss << tmp->valuedouble;
		//
		JSON_GET_OBJECT_ITEM("VM_CloneMode");
		if (tmp) ss << tmp->valueint;
		//
		JSON_GET_OBJECT_ITEM("VM_SysReadOnly");
		if (tmp) ss << tmp->valueint;
		//
		JSON_GET_OBJECT_ITEM("VM_DataSize");
		if (tmp) ss << tmp->valueint;
		//
		JSON_GET_OBJECT_ITEM("VM_UserGroup_ID");
		if (tmp) ss << tmp->valuedouble;
		//
		JSON_GET_OBJECT_ITEM("VM_User_ID");
		if (tmp) ss << tmp->valuedouble;
		//
		JSON_GET_OBJECT_ITEM("VM_State");
		if (tmp) ss << tmp->valueint;
		//
		JSON_GET_OBJECT_ITEM("VM_OS");
		if (tmp) ss << "'" << tmp->valuestring << "'";
		//
		JSON_GET_OBJECT_ITEM("VM_SysPath");
		if (tmp) ss << "'" << tmp->valuestring << "'";
		//
		JSON_GET_OBJECT_ITEM("VM_SysFilename");
		if (tmp) ss << "'" << tmp->valuestring << "'";
		//
		JSON_GET_OBJECT_ITEM("VM_DataPath");
		if (tmp) ss << "'" << tmp->valuestring << "'";
		//
		JSON_GET_OBJECT_ITEM("VM_DataFilename");
		if (tmp) ss << "'" << tmp->valuestring << "'";
		//
		JSON_GET_OBJECT_ITEM("VM_MAC");
		if (tmp) ss << "'" << tmp->valuestring << "'";
		//
		JSON_GET_OBJECT_ITEM("VM_SpicePwd");
		if (tmp) ss << "'" << tmp->valuestring << "'";
		//
		JSON_GET_OBJECT_ITEM("VM_Uuid");
		if (tmp) ss << "'" << tmp->valuestring << "'";
		//
		JSON_GET_OBJECT_ITEM("VM_Create_Time");
		if (tmp) ss << "'" << tmp->valuestring << "'";
		//
		JSON_GET_OBJECT_ITEM("VM_Update_Time");
		if (tmp) ss << "'" << tmp->valuestring << "'";


#undef JSON_GET_OBJECT_ITEM
		if (!hasItem) goto END;

		string whereSql = ss.str();
		count = VM_Count(whereSql);
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

double CDC_VM::VM_Add(TCDC_VM& src)
{
	double id = -1;
	try
	{
		if (VM_Find(src.VM_ID))
			return exists;

		_stmt = _pdb->compileStatement("insert into CDC_VM values (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
		if (src.VM_ID != INVALID_NUM && src.VM_ID > 0)
			id = src.VM_ID;
		else
			id = GetMaxID() + 1;
		_stmt.bind(1, id);
		_stmt.bind(2, src.VM_Template_ID);
		_stmt.bind(3, src.VM_Host_ID);
		_stmt.bind(4, src.VM_ThinClient_ID);
		_stmt.bind(5, src.VM_CloneMode);
		_stmt.bind(6, src.VM_SysReadOnly);
		_stmt.bind(7, src.VM_DataSize);
		_stmt.bind(8, src.VM_UserGroup_ID);
		_stmt.bind(9, src.VM_User_ID);
		_stmt.bind(10, src.VM_State);
		_stmt.bind(11, src.VM_OS);
		_stmt.bind(12, src.VM_SysPath);
		_stmt.bind(13, src.VM_SysFilename);
		_stmt.bind(14, src.VM_DataPath);
		_stmt.bind(15, src.VM_DataFilename);
		_stmt.bind(16, src.VM_MAC);
		_stmt.bind(17, src.VM_SpicePwd);
		_stmt.bind(18, src.VM_Uuid);
		_stmt.bind(19, String2MySQLTime(src.VM_Create_Time));
		_stmt.bind(20, String2MySQLTime(src.VM_Update_Time));

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

int CDC_VM::VM_Del(double id)
{
	try
	{
		if (!VM_Find(id))
			return notExists;

		_stmt = _pdb->compileStatement("delete from CDC_VM where VM_ID = ?;");
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

int CDC_VM::VM_Update(TCDC_VM& src, std::list<std::string>& keyList)
{
	try
	{
		list<string>::iterator it = find(keyList.begin(), keyList.end(), "VM_ID");
		if (it == keyList.end())
			return inputConditionError;

		if (!VM_Find(src.VM_ID))
			return notExists;

		string updateSql = "update CDC_VM set ";
		for (list<string>::iterator it = keyList.begin(); it != keyList.end(); ++it)
		{
			if (*it != "VM_ID")
				updateSql = updateSql + *it + " = ?, ";
		}
		updateSql = Trim(updateSql);
		updateSql = updateSql.substr(0, updateSql.size() - 1);
		updateSql += " where VM_ID = ?;";

		_stmt = _pdb->compileStatement(updateSql.c_str());

		int index = 1;
		for (list<string>::iterator it = keyList.begin(); it != keyList.end(); ++it)
		{
			if (*it == "VM_Template_ID")
				_stmt.bind(index++, src.VM_Template_ID);
			else if (*it == "VM_Host_ID")
				_stmt.bind(index++, src.VM_Host_ID);
			else if (*it == "VM_ThinClient_ID")
				_stmt.bind(index++, src.VM_ThinClient_ID);
			else if (*it == "VM_CloneMode")
				_stmt.bind(index++, src.VM_CloneMode);
			else if (*it == "VM_SysReadOnly")
				_stmt.bind(index++, src.VM_SysReadOnly);
			else if (*it == "VM_DataSize")
				_stmt.bind(index++, src.VM_DataSize);
			else if (*it == "VM_UserGroup_ID")
				_stmt.bind(index++, src.VM_UserGroup_ID);
			else if (*it == "VM_User_ID")
				_stmt.bind(index++, src.VM_User_ID);
			else if (*it == "VM_State")
				_stmt.bind(index++, src.VM_State);
			else if (*it == "VM_OS")
				_stmt.bind(index++, src.VM_OS);
			else if (*it == "VM_SysPath")
				_stmt.bind(index++, src.VM_SysPath);
			else if (*it == "VM_SysFilename")
				_stmt.bind(index++, src.VM_SysFilename);
			else if (*it == "VM_DataPath")
				_stmt.bind(index++, src.VM_DataPath);
			else if (*it == "VM_MAC")
				_stmt.bind(index++, src.VM_MAC);
			else if (*it == "VM_DataFilename")
				_stmt.bind(index++, src.VM_DataFilename);
			else if (*it == "VM_SpicePwd")
				_stmt.bind(index++, src.VM_SpicePwd);
			else if (*it == "VM_Uuid")
				_stmt.bind(index++, src.VM_Uuid);
			else if (*it == "VM_Create_Time")
				_stmt.bind(index++, src.VM_Create_Time);
			else if (*it == "VM_Update_Time")
				_stmt.bind(index++, src.VM_Update_Time);

		}
		_stmt.bind(index, src.VM_ID);

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

bool CDC_VM::VM_Find(double id)
{
	if (id == INVALID_NUM)
		return false;
	try
	{
		char buf[1024] = { 0 };
		sprintf(buf, "select count(*) from CDC_VM where VM_ID = %f;", id);
		return (_pdb->execScalar(buf) != 0);
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
		return false;
	}
	return true;
}

int CDC_VM::VM_Find(double id, TCDC_VM& t)
{
	try
	{
		if (!VM_Find(id))
			return notExists;

		CppMySQLQuery q;
		char buf[1024] = { 0 };
		sprintf(buf, "select * from CDC_VM where VM_ID = %f;", id);

		q = _pdb->execQuery(buf);

		if (!q.eof())
		{
			t.VM_ID = q.getDoubleField(0);
			t.VM_Template_ID = q.getDoubleField(1);
			t.VM_Host_ID = q.getDoubleField(2);
			t.VM_ThinClient_ID = q.getDoubleField(3);
			t.VM_CloneMode = q.getIntField(4);
			t.VM_SysReadOnly = q.getIntField(5);
			t.VM_DataSize = q.getIntField(6);
			t.VM_UserGroup_ID = q.getDoubleField(7);
			t.VM_User_ID = q.getDoubleField(8);
			t.VM_State = q.getIntField(9);
			t.VM_OS = q.fieldValue(10);
			t.VM_SysPath = q.fieldValue(11);
			t.VM_SysFilename = q.fieldValue(12);
			t.VM_DataPath = q.fieldValue(13);
			t.VM_DataFilename = q.fieldValue(14);
			t.VM_MAC = q.fieldValue(15);
			t.VM_SpicePwd = q.fieldValue(16);
			t.VM_Uuid = q.fieldValue(17);
			t.VM_Create_Time = q.fieldValue(18);
			t.VM_Update_Time = q.fieldValue(19);
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

int CDC_VM::VM_Find2(const std::string& whereSql, TCDC_VM& t)
{
	try
	{
		CppMySQLQuery q;
		char buf[1024] = { 0 };
		sprintf(buf, "select * from CDC_VM where %s;", whereSql.c_str());

		q = _pdb->execQuery(buf);

		if (!q.eof())
		{
			t.VM_ID = q.getDoubleField(0);
			t.VM_Template_ID = q.getDoubleField(1);
			t.VM_Host_ID = q.getDoubleField(2);
			t.VM_ThinClient_ID = q.getDoubleField(3);
			t.VM_CloneMode = q.getIntField(4);
			t.VM_SysReadOnly = q.getIntField(5);
			t.VM_DataSize = q.getIntField(6);
			t.VM_UserGroup_ID = q.getDoubleField(7);
			t.VM_User_ID = q.getDoubleField(8);
			t.VM_State = q.getIntField(9);
			t.VM_OS = q.fieldValue(10);
			t.VM_SysPath = q.fieldValue(11);
			t.VM_SysFilename = q.fieldValue(12);
			t.VM_DataPath = q.fieldValue(13);
			t.VM_DataFilename = q.fieldValue(14);
			t.VM_MAC = q.fieldValue(15);
			t.VM_SpicePwd = q.fieldValue(16);
			t.VM_Uuid = q.fieldValue(17);
			t.VM_Create_Time = q.fieldValue(18);
			t.VM_Update_Time = q.fieldValue(19);
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

std::list<TCDC_VM> CDC_VM::VM_Find2(const std::string& whereSql)
{
	std::list<TCDC_VM> lst;
	try
	{
		CppMySQLQuery q;
		char buf[1024] = { 0 };
		sprintf(buf, "select * from CDC_VM where %s;", whereSql.c_str());

		q = _pdb->execQuery(buf);
		while (!q.eof())
		{
			TCDC_VM t;
			t.VM_ID = q.getDoubleField(0);
			t.VM_Template_ID = q.getDoubleField(1);
			t.VM_Host_ID = q.getDoubleField(2);
			t.VM_ThinClient_ID = q.getDoubleField(3);
			t.VM_CloneMode = q.getIntField(4);
			t.VM_SysReadOnly = q.getIntField(5);
			t.VM_DataSize = q.getIntField(6);
			t.VM_UserGroup_ID = q.getDoubleField(7);
			t.VM_User_ID = q.getDoubleField(8);
			t.VM_State = q.getIntField(9);
			t.VM_OS = q.fieldValue(10);
			t.VM_SysPath = q.fieldValue(11);
			t.VM_SysFilename = q.fieldValue(12);
			t.VM_DataPath = q.fieldValue(13);
			t.VM_DataFilename = q.fieldValue(14);
			t.VM_MAC = q.fieldValue(15);
			t.VM_SpicePwd = q.fieldValue(16);
			t.VM_Uuid = q.fieldValue(17);
			t.VM_Create_Time = q.fieldValue(18);
			t.VM_Update_Time = q.fieldValue(19);

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

int CDC_VM::VM_Count()
{
	try
	{
		return _pdb->execScalar("select count(*) from CDC_VM;");
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
		return DBError;
	}
	return 0;
}

int CDC_VM::VM_Count(const std::string& whereSql)
{
	try
	{
		char buf[1024] = { 0 };
		sprintf(buf, "select count(*) from CDC_VM where %s;", whereSql.c_str());

		return _pdb->execScalar(buf);
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
		return DBError;
	}
	return 0;
}

std::list<TCDC_VM> CDC_VM::GetAll()
{
	std::list<TCDC_VM> lst;
	try
	{
		CppMySQLQuery q = _pdb->execQuery("select * from CDC_VM;");
		while (!q.eof())
		{
			TCDC_VM t;
			t.VM_ID = q.getDoubleField(0);
			t.VM_Template_ID = q.getDoubleField(1);
			t.VM_Host_ID = q.getDoubleField(2);
			t.VM_ThinClient_ID = q.getDoubleField(3);
			t.VM_CloneMode = q.getIntField(4);
			t.VM_SysReadOnly = q.getIntField(5);
			t.VM_DataSize = q.getIntField(6);
			t.VM_UserGroup_ID = q.getDoubleField(7);
			t.VM_User_ID = q.getDoubleField(8);
			t.VM_State = q.getIntField(9);
			t.VM_OS = q.fieldValue(10);
			t.VM_SysPath = q.fieldValue(11);
			t.VM_SysFilename = q.fieldValue(12);
			t.VM_DataPath = q.fieldValue(13);
			t.VM_DataFilename = q.fieldValue(14);
			t.VM_MAC = q.fieldValue(15);
			t.VM_SpicePwd = q.fieldValue(16);
			t.VM_Uuid = q.fieldValue(17);
			t.VM_Create_Time = q.fieldValue(18);
			t.VM_Update_Time = q.fieldValue(19);

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

double CDC_VM::GetMaxID()
{
	char buf[1024] = { 0 };
	sprintf(buf, "select max(VM_ID) from CDC_VM");
	CppMySQLQuery q = _pdb->execQuery(buf);

	if (q.eof() || q.numFields() < 1)
	{
		throw CppMySQLException(CPPMYSQL_DB_ERROR, "Invalid scalar query");
	}

	return q.getDoubleField(0);
}