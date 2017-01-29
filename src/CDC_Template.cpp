#include "CDC_Template.h"
#include "LogHelper.h"
#include "ComDef.h"
#include "cJSON.h"
#include "utility.h"
#include <stdio.h>
#include <assert.h> 
#include <stdio.h>
#include <algorithm>

using namespace std;

CDC_Template::CDC_Template(CppMySQLDB* pdb)
	:_pdb(pdb)
{
}


CDC_Template::~CDC_Template()
{
	_stmt.clear();
}


std::string CDC_Template::CDC_Template_Add(const std::string& req)
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

		TCDC_Template t;
		JSON_ADD_ONE_ELEMENT2(Template_ID, valuedouble);
		JSON_ADD_ONE_ELEMENT(Template_HostID, valuedouble);
		JSON_ADD_ONE_ELEMENT(Template_CoreNum, valueint);
		JSON_ADD_ONE_ELEMENT(Template_MemSize, valueint);
		JSON_ADD_ONE_ELEMENT(Template_MemSize, valueint);
		JSON_ADD_ONE_ELEMENT(Template_SysSize, valueint);
		JSON_ADD_ONE_ELEMENT(Template_Version, valueint);
		JSON_ADD_ONE_ELEMENT(Template_Mode, valueint);
		JSON_ADD_ONE_ELEMENT(Template_State, valueint);
		JSON_ADD_ONE_ELEMENT(Template_Name, valuestring);
		JSON_ADD_ONE_ELEMENT(Template_OS, valuestring);
		JSON_ADD_ONE_ELEMENT(Template_Path, valuestring);
		JSON_ADD_ONE_ELEMENT(Template_FileName, valuestring);
		JSON_ADD_ONE_ELEMENT2(Template_RollBackFile, valuestring);
		JSON_ADD_ONE_ELEMENT(Template_BackUpFile, valuestring);
		JSON_ADD_ONE_ELEMENT(Template_SpicePwd, valuestring);
		JSON_ADD_ONE_ELEMENT2(Template_Uuid, valuestring);
		JSON_ADD_ONE_ELEMENT2(Template_Create_Time, valuestring);
		JSON_ADD_ONE_ELEMENT2(Template_Update_Time, valuestring);

#undef JSON_ADD_ONE_ELEMENT
#undef JSON_ADD_ONE_ELEMENT2

		id = Template_Add(t);
	}

END:
	cJSON_Delete(json);

	ret = (id >= 0) ? 1 : 0;
	cJSON *result = cJSON_CreateObject();
	cJSON_AddNumberToObject(result, "Result", ret);
	// if success then return id
	if (ret > 0)
		cJSON_AddNumberToObject(result, "Template_ID", id);
	char *out = cJSON_Print(result);
	cJSON_Delete(result);

	return string(out);
}

std::string CDC_Template::CDC_Template_Del(const std::string& req)
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
		tmp = cJSON_GetObjectItem(json, "Template_ID");
		if (tmp)
			id = tmp->valuedouble;
		else
			goto END;
		ret = Template_Del(id);
	}

END:
	cJSON_Delete(json);

	cJSON *result = cJSON_CreateObject();
	cJSON_AddNumberToObject(result, "Result", ret);
	char *out = cJSON_Print(result);
	cJSON_Delete(result);

	return string(out);
}

std::string CDC_Template::CDC_Template_Update(const std::string& req)
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

		TCDC_Template t;
		const char *key = "Template_ID";
		tmp = cJSON_GetObjectItem(json, key);
		if (tmp)
		{
			if (!hasItem) hasItem = true;
			t.Template_ID = tmp->valuedouble;
			keyList.push_back(key);
		}

		JSON_GET_OBJECT_ITEM(Template_ID, valuedouble);
		JSON_GET_OBJECT_ITEM(Template_HostID, valuedouble);
		JSON_GET_OBJECT_ITEM(Template_CoreNum, valueint);
		JSON_GET_OBJECT_ITEM(Template_MemSize, valueint);
		JSON_GET_OBJECT_ITEM(Template_MemSize, valueint);
		JSON_GET_OBJECT_ITEM(Template_SysSize, valueint);
		JSON_GET_OBJECT_ITEM(Template_Version, valueint);
		JSON_GET_OBJECT_ITEM(Template_Mode, valueint);
		JSON_GET_OBJECT_ITEM(Template_State, valueint);
		JSON_GET_OBJECT_ITEM(Template_Name, valuestring);
		JSON_GET_OBJECT_ITEM(Template_OS, valuestring);
		JSON_GET_OBJECT_ITEM(Template_Path, valuestring);
		JSON_GET_OBJECT_ITEM(Template_FileName, valuestring);
		JSON_GET_OBJECT_ITEM(Template_RollBackFile, valuestring);
		JSON_GET_OBJECT_ITEM(Template_BackUpFile, valuestring);
		JSON_GET_OBJECT_ITEM(Template_SpicePwd, valuestring);
		JSON_GET_OBJECT_ITEM(Template_Uuid, valuestring);
		JSON_GET_OBJECT_ITEM(Template_Create_Time, valuestring);
		JSON_GET_OBJECT_ITEM(Template_Update_Time, valuestring);

	#undef JSON_GET_OBJECT_ITEM

		if (!hasItem) goto END;

		ret = Template_Update(t, keyList);
	}

END:
	cJSON_Delete(json);

	cJSON *result = cJSON_CreateObject();
	cJSON_AddNumberToObject(result, "Result", ret);
	char *out = cJSON_Print(result);
	cJSON_Delete(result);

	return string(out);
}

std::string CDC_Template::CDC_Template_Find(const std::string& req)
{
	int ret = 0;
	bool hasItem = false;
	bool isAll = true;
	cJSON *json, *tmp;
	TCDC_Template t;
	list<TCDC_Template> lst;
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
		JSON_GET_OBJECT_ITEM("Template_ID");
		if (tmp) ss << tmp->valuedouble;

		JSON_GET_OBJECT_ITEM("Template_Name");
		if (tmp) ss << "'" << tmp->valuestring << "'";

		JSON_GET_OBJECT_ITEM("Template_IP");
		if (tmp) ss << "'" << tmp->valuestring << "'";

		JSON_GET_OBJECT_ITEM("Template_VirtNet");
		if (tmp) ss << "'" << tmp->valuestring << "'";

	#undef JSON_GET_OBJECT_ITEM

		if (!hasItem) goto END;

		string whereSql = ss.str();
		ret = Template_Find2(whereSql, t);
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
		cJSON_AddNumberToObject(data, "Template_ID", t.Template_ID);\
		cJSON_AddNumberToObject(data, "Template_HostID", t.Template_HostID);\
		cJSON_AddNumberToObject(data, "Template_CoreNum", t.Template_CoreNum);\
		cJSON_AddNumberToObject(data, "Template_MemSize", t.Template_MemSize);\
		cJSON_AddNumberToObject(data, "Template_SysSize", t.Template_SysSize);\
		cJSON_AddNumberToObject(data, "Template_Version", t.Template_Version);\
		cJSON_AddNumberToObject(data, "Template_Mode", t.Template_Mode);\
		cJSON_AddNumberToObject(data, "Template_State", t.Template_State);\
		cJSON_AddStringToObject(data, "Template_Name", t.Template_Name.c_str());\
		cJSON_AddStringToObject(data, "Template_OS", t.Template_OS.c_str());\
		cJSON_AddStringToObject(data, "Template_Path", t.Template_Path.c_str());\
		cJSON_AddStringToObject(data, "Template_FileName", t.Template_FileName.c_str());\
		cJSON_AddStringToObject(data, "Template_RollBackFile", t.Template_RollBackFile.c_str());\
		cJSON_AddStringToObject(data, "Template_BackUpFile", t.Template_BackUpFile.c_str());\
		cJSON_AddStringToObject(data, "Template_SpicePwd", t.Template_SpicePwd.c_str());\
		cJSON_AddStringToObject(data, "Template_Uuid", t.Template_Uuid.c_str());\
		cJSON_AddStringToObject(data, "Template_Create_Time", t.Template_Create_Time.c_str());\
		cJSON_AddStringToObject(data, "Template_Update_Time", t.Template_Update_Time.c_str());

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
				for (list<TCDC_Template>::iterator it = lst.begin(); it != lst.end(); ++it)
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

std::string CDC_Template::CDC_Template_FindCount(const std::string& req)
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
			count = Template_Count();
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
		JSON_GET_OBJECT_ITEM("Template_ID");
		if (tmp) ss << tmp->valuedouble;
		//
		JSON_GET_OBJECT_ITEM("Template_HostID");
		if (tmp) ss << tmp->valuedouble;
		//
		JSON_GET_OBJECT_ITEM("Template_CoreNum");
		if (tmp) ss << tmp->valueint;
		//
		JSON_GET_OBJECT_ITEM("Template_MemSize");
		if (tmp) ss << tmp->valueint;
		//
		JSON_GET_OBJECT_ITEM("Template_SysSize");
		if (tmp) ss << tmp->valueint;
		//
		JSON_GET_OBJECT_ITEM("Template_Version");
		if (tmp) ss << tmp->valueint;
		//
		JSON_GET_OBJECT_ITEM("Template_Mode");
		if (tmp) ss << tmp->valueint;
		//
		JSON_GET_OBJECT_ITEM("Template_State");
		if (tmp) ss << tmp->valueint;
		//
		JSON_GET_OBJECT_ITEM("Template_Name");
		if (tmp) ss << "'" << tmp->valuestring << "'";
		//
		JSON_GET_OBJECT_ITEM("Template_OS");
		if (tmp) ss << "'" << tmp->valuestring << "'";
		//
		JSON_GET_OBJECT_ITEM("Template_Path");
		if (tmp) ss << "'" << tmp->valuestring << "'";
		//
		JSON_GET_OBJECT_ITEM("Template_FileName");
		if (tmp) ss << "'" << tmp->valuestring << "'";
		//
		JSON_GET_OBJECT_ITEM("Template_RollBackFile");
		if (tmp) ss << "'" << tmp->valuestring << "'";
		//
		JSON_GET_OBJECT_ITEM("Template_BackUpFile");
		if (tmp) ss << "'" << tmp->valuestring << "'";
		//
		JSON_GET_OBJECT_ITEM("Template_SpicePwd");
		if (tmp) ss << "'" << tmp->valuestring << "'";
		//
		JSON_GET_OBJECT_ITEM("Template_Uuid");
		if (tmp) ss << "'" << tmp->valuestring << "'";
		//
		JSON_GET_OBJECT_ITEM("Template_Create_Time");
		if (tmp) ss << "'" << tmp->valuestring << "'";
		//
		JSON_GET_OBJECT_ITEM("Template_Update_Time");
		if (tmp) ss << "'" << tmp->valuestring << "'";


#undef JSON_GET_OBJECT_ITEM

		if (!hasItem) goto END;

		string whereSql = ss.str();
		count = Template_Count(whereSql);
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

double CDC_Template::Template_Add(TCDC_Template& src)
{
	double id = -1;
	try
	{
		if (Template_Find(src.Template_ID))
			return exists;

		_stmt = _pdb->compileStatement("insert into CDC_Template values (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
		if (src.Template_ID != INVALID_NUM && src.Template_ID > 0)
			id = src.Template_ID;
		else
			id = GetMaxID() + 1;
		_stmt.bind(1, id);
		_stmt.bind(2, src.Template_HostID);
		_stmt.bind(3, src.Template_CoreNum);
		_stmt.bind(4, src.Template_MemSize);
		_stmt.bind(5, src.Template_SysSize);
		_stmt.bind(6, src.Template_Version);
		_stmt.bind(7, src.Template_Mode);
		_stmt.bind(8, src.Template_State);
		_stmt.bind(9, src.Template_Name);
		_stmt.bind(10, src.Template_OS);
		_stmt.bind(11, src.Template_Path);
		_stmt.bind(12, src.Template_FileName);
		_stmt.bind(13, src.Template_RollBackFile);
		_stmt.bind(14, src.Template_BackUpFile);
		_stmt.bind(15, src.Template_SpicePwd);
		_stmt.bind(16, src.Template_Uuid);
		_stmt.bind(17, String2MySQLTime(src.Template_Create_Time));
		_stmt.bind(18, String2MySQLTime(src.Template_Update_Time));

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

int CDC_Template::Template_Del(double id)
{
	try
	{
		if (!Template_Find(id))
			return notExists;

		_stmt = _pdb->compileStatement("delete from CDC_Template where Template_ID = ?;");
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

int CDC_Template::Template_Update(TCDC_Template& src, std::list<std::string>& keyList)
{
	try
	{
		list<string>::iterator it = find(keyList.begin(), keyList.end(), "Template_ID");
		if (it == keyList.end())
			return inputConditionError;

		if (!Template_Find(src.Template_ID))
			return notExists;

		string updateSql = "update CDC_Template set ";
		for (list<string>::iterator it = keyList.begin(); it != keyList.end(); ++it)
		{
			if (*it != "Template_ID")
				updateSql = updateSql + *it + " = ?, ";
		}
		updateSql = Trim(updateSql);
		updateSql = updateSql.substr(0, updateSql.size() - 1);
		updateSql += " where Template_ID = ?;";

		_stmt = _pdb->compileStatement(updateSql.c_str());

		int index = 1;
		for (list<string>::iterator it = keyList.begin(); it != keyList.end(); ++it)
		{
			if (*it == "Template_HostID")
				_stmt.bind(index++, src.Template_HostID);
			else if (*it == "Template_CoreNum")
				_stmt.bind(index++, src.Template_CoreNum);
			else if (*it == "Template_MemSize")
				_stmt.bind(index++, src.Template_MemSize);
			else if (*it == "Template_SysSize")
				_stmt.bind(index++, src.Template_SysSize);
			else if (*it == "Template_Version")
				_stmt.bind(index++, src.Template_Version);
			else if (*it == "Template_Mode")
				_stmt.bind(index++, src.Template_Mode);
			else if (*it == "Template_State")
				_stmt.bind(index++, src.Template_State);
			else if (*it == "Template_Name")
				_stmt.bind(index++, src.Template_Name);
			else if (*it == "Template_OS")
				_stmt.bind(index++, src.Template_OS);
			else if (*it == "Template_Path")
				_stmt.bind(index++, src.Template_Path);
			else if (*it == "Template_FileName")
				_stmt.bind(index++, src.Template_FileName);
			else if (*it == "Template_RollBackFile")
				_stmt.bind(index++, src.Template_RollBackFile);
			else if (*it == "Template_BackUpFile")
				_stmt.bind(index++, src.Template_BackUpFile);
			else if (*it == "Template_SpicePwd")
				_stmt.bind(index++, src.Template_SpicePwd);
			else if (*it == "Template_Uuid")
				_stmt.bind(index++, src.Template_Uuid);
			else if (*it == "Template_Create_Time")
				_stmt.bind(index++, src.Template_Create_Time);
			else if (*it == "Template_Update_Time")
				_stmt.bind(index++, src.Template_Update_Time);

		}
		_stmt.bind(index, src.Template_ID);

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

bool CDC_Template::Template_Find(double id)
{
	if (id == INVALID_NUM) 
		return false;
	try
	{
		char buf[1024] = { 0 };
		sprintf(buf, "select count(*) from CDC_Template where Template_ID = %f;", id);
		return (_pdb->execScalar(buf) != 0);
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
		return false;
	}
	return true;
}

int CDC_Template::Template_Find(double id, TCDC_Template& t)
{
	try
	{
		if (!Template_Find(id))
			return notExists;

		CppMySQLQuery q;
		char buf[1024] = { 0 };
		sprintf(buf, "select * from CDC_Template where Template_ID = %f;", id);

		q = _pdb->execQuery(buf);

		if (!q.eof())
		{
			t.Template_ID = q.getDoubleField(0);
			t.Template_HostID = q.getDoubleField(1);
			t.Template_CoreNum = q.getIntField(2);
			t.Template_MemSize = q.getIntField(3);
			t.Template_SysSize = q.getIntField(4);
			t.Template_Version = q.getIntField(5);
			t.Template_Mode = q.getIntField(6);
			t.Template_State = q.getIntField(7);
			t.Template_Name = q.fieldValue(8);
			t.Template_OS = q.fieldValue(9);
			t.Template_Path = q.fieldValue(10);
			t.Template_FileName = q.fieldValue(11);
			t.Template_RollBackFile = q.fieldValue(12);
			t.Template_BackUpFile = q.fieldValue(13);
			t.Template_SpicePwd = q.fieldValue(14);
			t.Template_Uuid = q.fieldValue(15);
			t.Template_Create_Time = q.fieldValue(16);
			t.Template_Update_Time = q.fieldValue(17);
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

int CDC_Template::Template_Find2(const std::string& whereSql, TCDC_Template& t)
{
	try
	{
		CppMySQLQuery q;
		char buf[1024] = { 0 };
		sprintf(buf, "select * from CDC_Template where %s;", whereSql.c_str());

		q = _pdb->execQuery(buf);

		if (!q.eof())
		{
			t.Template_ID = q.getDoubleField(0);
			t.Template_HostID = q.getDoubleField(1);
			t.Template_CoreNum = q.getIntField(2);
			t.Template_MemSize = q.getIntField(3);
			t.Template_SysSize = q.getIntField(4);
			t.Template_Version = q.getIntField(5);
			t.Template_Mode = q.getIntField(6);
			t.Template_State = q.getIntField(7);
			t.Template_Name = q.fieldValue(8);
			t.Template_OS = q.fieldValue(9);
			t.Template_Path = q.fieldValue(10);
			t.Template_FileName = q.fieldValue(11);
			t.Template_RollBackFile = q.fieldValue(12);
			t.Template_BackUpFile = q.fieldValue(13);
			t.Template_SpicePwd = q.fieldValue(14);
			t.Template_Uuid = q.fieldValue(15);
			t.Template_Create_Time = q.fieldValue(16);
			t.Template_Update_Time = q.fieldValue(17);
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

int CDC_Template::Template_Count()
{
	try
	{
		return _pdb->execScalar("select count(*) from CDC_Template;");
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
		return DBError;
	}
	return 0;
}

int CDC_Template::Template_Count(const std::string& whereSql)
{
	try
	{
		char buf[1024] = { 0 };
		sprintf(buf, "select count(*) from CDC_Template where %s;", whereSql.c_str());

		return _pdb->execScalar(buf);
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
		return DBError;
	}
	return 0;
}

std::list<TCDC_Template> CDC_Template::GetAll()
{
	std::list<TCDC_Template> lst;
	try
	{
		CppMySQLQuery q = _pdb->execQuery("select * from CDC_Template;");
		while (!q.eof())
		{
			TCDC_Template t;
			t.Template_ID = q.getDoubleField(0);
			t.Template_HostID = q.getDoubleField(1);
			t.Template_CoreNum = q.getIntField(2);
			t.Template_MemSize = q.getIntField(3);
			t.Template_SysSize = q.getIntField(4);
			t.Template_Version = q.getIntField(5);
			t.Template_Mode = q.getIntField(6);
			t.Template_State = q.getIntField(7);
			t.Template_Name = q.fieldValue(8);
			t.Template_OS = q.fieldValue(9);
			t.Template_Path = q.fieldValue(10);
			t.Template_FileName = q.fieldValue(11);
			t.Template_RollBackFile = q.fieldValue(12);
			t.Template_BackUpFile = q.fieldValue(13);
			t.Template_SpicePwd = q.fieldValue(14);
			t.Template_Uuid = q.fieldValue(15);
			t.Template_Create_Time = q.fieldValue(16);
			t.Template_Update_Time = q.fieldValue(17);

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

double CDC_Template::GetMaxID()
{
	char buf[1024] = { 0 };
	sprintf(buf, "select max(Template_ID) from CDC_Template");
	CppMySQLQuery q = _pdb->execQuery(buf);

	if (q.eof() || q.numFields() < 1)
	{
		throw CppMySQLException(CPPMYSQL_DB_ERROR, "Invalid scalar query");
	}

	return q.getDoubleField(0);
}