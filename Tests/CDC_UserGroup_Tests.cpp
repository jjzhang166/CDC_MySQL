#include "CDC_UserGroup_Tests.h"
#include "CppMySQL.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "mysql.h"
#include "TestsComDef.h"
#include "cJSON.h"
#include "LogHelper.h"

#include <iostream>
using namespace std;

namespace
{
	CppMySQLDB* pDB;
	const char* gszDB = "CDCMySQLTest";
	#define Host		"127.0.0.1"
	#define USER		"root"
	#define PASSWORD	"123456"
	int ret = -1;
	double id = -1;

	cJSON *json = 0;
	char *out = 0;
	string req;
	string result;
}

CDC_UserGroup_Tests::CDC_UserGroup_Tests(const std::string& name) 
	: CppUnit::TestCase(name)
{
}

CDC_UserGroup_Tests::~CDC_UserGroup_Tests()
{
}

void CDC_UserGroup_Tests::setUp()
{	
	pDB = new CppMySQLDB();
	pDB->setOptions(MYSQL_SET_CHARSET_NAME, "gbk");	
	pDB->connect(Host, USER, PASSWORD);
	pDB->dropDB(gszDB);
	pDB->createDB(gszDB);
	pDB->open(gszDB);

	string sql = "CREATE TABLE IF NOT EXISTS CDC_UserGroup (\
		UserGroup_ID bigint(20) not null AUTO_INCREMENT,\
		UserGroup_Name Varchar(255) not null,\
		PRIMARY KEY(UserGroup_ID)\
		) ENGINE = InnoDB; ";

	pDB->execDML(sql);
	_pObj = new CDC_UserGroup(pDB);
}


void CDC_UserGroup_Tests::tearDown()
{
	pDB->dropDB(gszDB);
	pDB->close();
	delete pDB;
	pDB = NULL;
}

void CDC_UserGroup_Tests::testCount()
{
	int count = _pObj->UserGroup_Count();
	assert(count == 0);

	TCDC_UserGroup userGroup;
	userGroup.UserGroup_Name = "testCount";
	id = _pObj->UserGroup_Add(userGroup);
	assert(id > 0);

	count = _pObj->UserGroup_Count();
	assert(count == 1);
}

void CDC_UserGroup_Tests::testJsonAdd()
{
	// success
	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddStringToObject(json, "UserGroup_Name", "testJsonAdd¹þ¹þ");
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_UserGroup_Add(req);

	const string success = "{\n\t\"Result\":\t1,\n\t\"UserGroup_ID\":\t1\n}";
	assert(success == result);

	// failed
	json = cJSON_CreateObject();
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);

	const string failed = FAILED_JSON_RESULT;
	result = _pObj->CDC_UserGroup_Add(req);
	assert(failed == result);
}


void CDC_UserGroup_Tests::testJsonDel()
{
	TCDC_UserGroup userGroup1;
	userGroup1.UserGroup_Name = "testDel1";
	id = _pObj->UserGroup_Add(userGroup1);
	assert(id > 0);

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddNumberToObject(json, "UserGroup_ID", id);
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_UserGroup_Del(req);
	assert(SUCCESS_JSON_RESULT == result);
}

void CDC_UserGroup_Tests::testJsonUpdate()
{
	TCDC_UserGroup userGroup1;
	userGroup1.UserGroup_Name = "testJsonUpdate";
	id = _pObj->UserGroup_Add(userGroup1);
	assert(id > 0);

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddNumberToObject(json, "UserGroup_ID", id);
	cJSON_AddStringToObject(json, "UserGroup_Name", "testJsonUpdate222");
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_UserGroup_Update(req);
	assert(SUCCESS_JSON_RESULT == result);
}

void CDC_UserGroup_Tests::testJsonFind()
{
	TCDC_UserGroup userGroup1;
	userGroup1.UserGroup_Name = "testJsonFind11";
	id = _pObj->UserGroup_Add(userGroup1);
	assert(id > 0);

	TCDC_UserGroup userGroup2;
	userGroup2.UserGroup_Name = "testJsonFind11";
	id = _pObj->UserGroup_Add(userGroup2);
	assert(id > 0);

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddStringToObject(json, "UserGroup_Name", "testJsonFind11");
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_UserGroup_Find(req);
	assert(result != FAILED_JSON_RESULT);
	MDEBUG << result << endl;

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "All");
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_UserGroup_Find(req);
	MDEBUG << result << endl;
	assert(result != FAILED_JSON_RESULT);
}

void CDC_UserGroup_Tests::testJsonFindCount()
{
	TCDC_UserGroup userGroup1;
	userGroup1.UserGroup_Name = "testJsonFindCount11";
	id = _pObj->UserGroup_Add(userGroup1);
	assert(id > 0);

	TCDC_UserGroup userGroup2;
	userGroup2.UserGroup_Name = "testJsonFindCount22";
	id = _pObj->UserGroup_Add(userGroup2);
	assert(id > 0);

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddNumberToObject(json, "UserGroup_ID", id);
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_UserGroup_FindCount(req);
	MDEBUG << result << endl;
	assert(result != FAILED_JSON_RESULT);

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "All");
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_UserGroup_FindCount(req);
	MDEBUG << result << endl;
	assert(result != FAILED_JSON_RESULT);
}


void CDC_UserGroup_Tests::testAdd()
{
	TCDC_UserGroup userGroup;
	userGroup.UserGroup_Name = "testAdd";
	id = _pObj->UserGroup_Add(userGroup);
	assert(id > 0);

	TCDC_UserGroup userGroup2;
	userGroup2.UserGroup_Name = "testAdd2";
	id = _pObj->UserGroup_Add(userGroup2);
	assert(id > 0);

	TCDC_UserGroup t1, t2;
	string name = "testAdd";
	string name2 = "testAdd2";
	ret = _pObj->UserGroup_Find(name, t1);
	assert(ret == success);
	ret = _pObj->UserGroup_Find(name2, t2);
	assert(ret == success);
	assert(t1.UserGroup_ID + 1 == t2.UserGroup_ID);
}



void CDC_UserGroup_Tests::testDel()
{
	TCDC_UserGroup userGroup1;
	userGroup1.UserGroup_ID = 1001;
	userGroup1.UserGroup_Name = "testDel1";
	id = _pObj->UserGroup_Add(userGroup1);
	assert(id > 0);

	TCDC_UserGroup userGroup2;
	userGroup2.UserGroup_ID = 1002;
	userGroup2.UserGroup_Name = "testDel2";
	id = _pObj->UserGroup_Add(userGroup2);
	assert(id > 0);

	ret = _pObj->UserGroup_Del(1002);
	assert(ret == success);
}

void CDC_UserGroup_Tests::testUpdate()
{
	TCDC_UserGroup userGroup1;
	userGroup1.UserGroup_ID = 1001;
	userGroup1.UserGroup_Name = "testUpdate1";
	id = _pObj->UserGroup_Add(userGroup1);
	assert(id > 0);

	TCDC_UserGroup userGroup2;
	userGroup2.UserGroup_ID = 1001;
	userGroup2.UserGroup_Name = "testUpdate2";
	ret = _pObj->UserGroup_Update(userGroup2);
	assert(ret == success);
}

void CDC_UserGroup_Tests::testFind()
{
	TCDC_UserGroup userGroup1;
	userGroup1.UserGroup_ID = 1001;
	userGroup1.UserGroup_Name = "testFind1";
	id = _pObj->UserGroup_Add(userGroup1);
	assert(id > 0);

	TCDC_UserGroup userGroup2;
	userGroup2.UserGroup_ID = 1002;
	userGroup2.UserGroup_Name = "testFind2";
	id = _pObj->UserGroup_Add(userGroup2);
	assert(ret == success);

	bool find = _pObj->UserGroup_Find(1001);
	assert(find == true);

	string name = "testFind2";
	find = _pObj->UserGroup_Find(name);
	assert(find == true);

	TCDC_UserGroup t1;
	ret = _pObj->UserGroup_Find(1001, t1);
	assert(ret == success);
	assert(t1.UserGroup_Name == "testFind1");

	TCDC_UserGroup t2;
	name = "testFind2";
	ret = _pObj->UserGroup_Find(name, t2);
	assert(ret == success);
	assert(t2.UserGroup_ID == 1002);
}



CppUnit::Test* CDC_UserGroup_Tests::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("CDC_UserGroup_Tests");

	
	CppUnit_addTest(pSuite, CDC_UserGroup_Tests, testJsonAdd);
	CppUnit_addTest(pSuite, CDC_UserGroup_Tests, testJsonDel);
	CppUnit_addTest(pSuite, CDC_UserGroup_Tests, testJsonUpdate);
	CppUnit_addTest(pSuite, CDC_UserGroup_Tests, testJsonFind);
	CppUnit_addTest(pSuite, CDC_UserGroup_Tests, testJsonFindCount);
	
	/*CppUnit_addTest(pSuite, CDC_UserGroup_Tests, testCount);
	CppUnit_addTest(pSuite, CDC_UserGroup_Tests, testAdd);
	CppUnit_addTest(pSuite, CDC_UserGroup_Tests, testDel);
	CppUnit_addTest(pSuite, CDC_UserGroup_Tests, testUpdate);
	CppUnit_addTest(pSuite, CDC_UserGroup_Tests, testFind);*/

	return pSuite;
}
