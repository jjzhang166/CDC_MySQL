#include "CDC_ThinClientGroup.h"
#include "LogHelper.h"
#include "ComDef.h"
#include "cJSON.h"
#include "utility.h"

#include <assert.h> 
#include <stdio.h>

using namespace std;

CDC_ThinClientGroup::CDC_ThinClientGroup(CppMySQLDB& db)
	:_db(db)
{
}


CDC_ThinClientGroup::~CDC_ThinClientGroup()
{
	_stmt.clear();
}


std::string CDC_ThinClientGroup::CDC_ThinClientGroup_Add(const std::string& req)
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
		
		TCDC_ThinClientGroup ThinClientGroup;
		tmp = cJSON_GetObjectItem(json, "ThinClientGroup_Name");
		if (tmp)
			ThinClientGroup.ThinClientGroup_Name = tmp->valuestring;
		else
			goto END;
		id = ThinClientGroup_Add(ThinClientGroup);
	}

END:
	cJSON_Delete(json);

	ret = (id >= 0) ? 1 : 0;
	cJSON *result = cJSON_CreateObject();
	cJSON_AddNumberToObject(result, "Result", ret);
	if (ret > 0)
		cJSON_AddNumberToObject(result, "ThinClientGroup_ID", id);
	char *out = cJSON_Print(result);
	cJSON_Delete(result);

	return string(out);
}

std::string CDC_ThinClientGroup::CDC_ThinClientGroup_Del(const std::string& req)
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
		tmp = cJSON_GetObjectItem(json, "ThinClientGroup_ID");
		if (tmp)
			id = tmp->valuedouble;
		else
			goto END;
		ret = ThinClientGroup_Del(id);
	}

END:
	cJSON_Delete(json);

	cJSON *result = cJSON_CreateObject();
	cJSON_AddNumberToObject(result, "Result", ret);
	char *out = cJSON_Print(result);
	cJSON_Delete(result);

	return string(out);
}


std::string CDC_ThinClientGroup::CDC_ThinClientGroup_Update(const std::string& req)
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

		TCDC_ThinClientGroup ThinClientGroup;
		tmp = cJSON_GetObjectItem(json, "ThinClientGroup_ID");
		if (tmp)
		{
			if (!hasItem) hasItem = true;
			ThinClientGroup.ThinClientGroup_ID = tmp->valuedouble;
		}
		tmp = cJSON_GetObjectItem(json, "ThinClientGroup_Name");
		if (tmp)
		{
			if (!hasItem) hasItem = true;
			ThinClientGroup.ThinClientGroup_Name = tmp->valuestring;
		}
		
		if (!hasItem) goto END;

		ret = ThinClientGroup_Update(ThinClientGroup);
	}

END:
	cJSON_Delete(json);

	cJSON *result = cJSON_CreateObject();
	cJSON_AddNumberToObject(result, "Result", ret);
	char *out = cJSON_Print(result);
	cJSON_Delete(result);

	return string(out);
}

std::string CDC_ThinClientGroup::CDC_ThinClientGroup_Find(const std::string& req)
{
	int ret = 0;
	bool hasItem = false;
	bool isAll = true;
	cJSON *json, *tmp, *element;
	TCDC_ThinClientGroup ThinClientGroup;
	list<TCDC_ThinClientGroup> lst;
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
		tmp = cJSON_GetObjectItem(json, "ThinClientGroup_ID");
		if (tmp)
		{
			if (!hasItem)
			{
				ss << " ThinClientGroup_ID = ";
				hasItem = true;
			}
			else
				ss << " AND ThinClientGroup_ID = ";
			ss << tmp->valuedouble;
		}
		tmp = cJSON_GetObjectItem(json, "ThinClientGroup_Name");
		if (tmp)
		{
			if (!hasItem)
			{
				ss << " ThinClientGroup_Name = ";
				hasItem = true;
			}
			else
			{
				ss << " AND ThinClientGroup_Name = ";
			}
			ss << "'" << tmp->valuestring << "'";
		}

		if (!hasItem) goto END;

		string whereSql = ss.str();
		ret = ThinClientGroup_Find2(whereSql, ThinClientGroup);
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
			cJSON_AddItemToObject(result, "Data", data = cJSON_CreateObject());
			cJSON_AddNumberToObject(data, "ThinClientGroup_ID", ThinClientGroup.ThinClientGroup_ID);
			cJSON_AddStringToObject(data, "ThinClientGroup_Name", ThinClientGroup.ThinClientGroup_Name.c_str());
		}
		else
		{
			cJSON_AddNumberToObject(result, "Result", lst.size());
			if (lst.size() > 1)
			{
				cJSON_AddItemToObject(result, "Data", dataArr = cJSON_CreateArray());
				for (list<TCDC_ThinClientGroup>::iterator it = lst.begin(); it != lst.end(); ++it)
				{
					cJSON_AddItemToArray(dataArr, data = cJSON_CreateObject());
					cJSON_AddNumberToObject(data, "ThinClientGroup_ID", (*it).ThinClientGroup_ID);
					cJSON_AddStringToObject(data, "ThinClientGroup_Name", (*it).ThinClientGroup_Name.c_str());
				}
			}
			else if (lst.size() == 1)
			{
				cJSON_AddItemToObject(result, "Data", data = cJSON_CreateObject());
				ThinClientGroup = lst.back();
				cJSON_AddNumberToObject(data, "ThinClientGroup_ID", ThinClientGroup.ThinClientGroup_ID);
				cJSON_AddStringToObject(data, "ThinClientGroup_Name", ThinClientGroup.ThinClientGroup_Name.c_str());
			}

		}
	}
	out = cJSON_Print(result);
	cJSON_Delete(result);
	return string(out);
}


std::string CDC_ThinClientGroup::CDC_ThinClientGroup_FindCount(const std::string& req)
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
			count = ThinClientGroup_Count();
			goto END;
		}

		stringstream ss;
		tmp = cJSON_GetObjectItem(json, "ThinClientGroup_ID");
		if (tmp)
		{
			if (!hasItem)
			{
				ss << " ThinClientGroup_ID = ";
				hasItem = true;
			}
			else
				ss << " AND ThinClientGroup_ID = ";
			ss << tmp->valuedouble;
		}
		tmp = cJSON_GetObjectItem(json, "ThinClientGroup_Name");
		if (tmp)
		{
			if (!hasItem)
			{
				ss << " ThinClientGroup_Name = ";
				hasItem = true;
			}
			else
				ss << " AND ThinClientGroup_Name = ";
			ss << "'" << tmp->valuestring << "'";
		}

		if (!hasItem) goto END;

		string whereSql = ss.str();
		count = ThinClientGroup_Count(whereSql);
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

double CDC_ThinClientGroup::GetMaxID()
{
	char buf[1024] = { 0 };
	sprintf(buf, "select max(ThinClientGroup_ID) from CDC_ThinClientGroup");
	CppMySQLQuery q = _db.execQuery(buf);

	if (q.eof() || q.numFields() < 1)
	{
		throw CppMySQLException(CPPMYSQL_DB_ERROR, "Invalid scalar query");
	}

	return q.getDoubleField(0);
}

double CDC_ThinClientGroup::ThinClientGroup_Add(TCDC_ThinClientGroup& src)
{
	double id = -1;
	try
	{
		if (ThinClientGroup_Find(src.ThinClientGroup_ID))
			return exists;

		_stmt = _db.compileStatement("insert into CDC_ThinClientGroup values (?, ?);");

		if (src.ThinClientGroup_ID != INVALID_NUM)
			id = src.ThinClientGroup_ID;
		else
			id = GetMaxID() + 1;
		_stmt.bind(1, id);
		_stmt.bind(2, src.ThinClientGroup_Name);

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

int CDC_ThinClientGroup::ThinClientGroup_Del(double id)
{
	try
	{
		if (!ThinClientGroup_Find(id))
			return notExists;

		char buf[1024] = { 0 };
		sprintf(buf, "delete from CDC_ThinClientGroup where ThinClientGroup_ID = %f;", id);
		_db.execDML(buf);
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
		return DBError;
	}
	return success;
}

int CDC_ThinClientGroup::ThinClientGroup_Update(TCDC_ThinClientGroup& src)
{
	try
	{
		if (!ThinClientGroup_Find(src.ThinClientGroup_ID))
			return notExists;

		_stmt = _db.compileStatement("update CDC_ThinClientGroup set ThinClientGroup_Name = ? where ThinClientGroup_ID = ?;");
		_stmt.bind(1, src.ThinClientGroup_Name);
		_stmt.bind(2, src.ThinClientGroup_ID);
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


bool CDC_ThinClientGroup::ThinClientGroup_Find(double id)
{
	if (id == INVALID_NUM)
		return false;

	try
	{
		char buf[1024] = {0};
		sprintf(buf, "select count(*) from CDC_ThinClientGroup where ThinClientGroup_ID = %f;", id);
		return (_db.execScalar(buf) != 0);
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
		return false;
	}
	return true;
}


int CDC_ThinClientGroup::ThinClientGroup_Find(double id, TCDC_ThinClientGroup& t)
{
	try
	{
		if (!ThinClientGroup_Find(id))
			return notExists;

		CppMySQLQuery q;
		char buf[1024] = { 0 };
		sprintf(buf, "select * from CDC_ThinClientGroup where ThinClientGroup_ID = %f;", id);

		q = _db.execQuery(buf);

		if (!q.eof())
		{
			t.ThinClientGroup_ID = q.getDoubleField("ThinClientGroup_ID");
			t.ThinClientGroup_Name = q.fieldValue("ThinClientGroup_Name");
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

int CDC_ThinClientGroup::ThinClientGroup_Find2(std::string& whereSql, TCDC_ThinClientGroup& t)
{
	try
	{
		CppMySQLQuery q;
		char buf[1024] = { 0 };
		sprintf(buf, "select * from CDC_ThinClientGroup where %s;", whereSql.c_str());

		q = _db.execQuery(buf);

		if (!q.eof())
		{
			t.ThinClientGroup_ID = q.getDoubleField("ThinClientGroup_ID");
			t.ThinClientGroup_Name = q.fieldValue("ThinClientGroup_Name");
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

int CDC_ThinClientGroup::ThinClientGroup_Count()
{
	try
	{
		return _db.execScalar("select count(*) from CDC_ThinClientGroup;");
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
		return DBError;
	}
	return 0;
}

int CDC_ThinClientGroup::ThinClientGroup_Count(std::string& whereSql)
{
	try
	{
		char buf[1024] = { 0 };
		sprintf(buf, "select count(*) from CDC_ThinClientGroup where %s;", whereSql.c_str());

		return _db.execScalar(buf);
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
		return DBError;
	}
	return 0;
}

std::list<TCDC_ThinClientGroup> CDC_ThinClientGroup::GetAll()
{
	std::list<TCDC_ThinClientGroup> lst;
	try
	{
		CppMySQLQuery q = _db.execQuery("select * from CDC_ThinClientGroup;");
		while (!q.eof())
		{
			TCDC_ThinClientGroup t;
			t.ThinClientGroup_ID = q.getDoubleField(0);
			t.ThinClientGroup_Name = q.getStringField(1);
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
