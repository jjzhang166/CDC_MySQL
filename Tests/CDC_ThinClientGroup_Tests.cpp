#include "CDC_ThinClientGroup_Tests.h"
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

CDC_ThinClientGroup_Tests::CDC_ThinClientGroup_Tests(const std::string& name) 
	: CppUnit::TestCase(name)
{
}

CDC_ThinClientGroup_Tests::~CDC_ThinClientGroup_Tests()
{
}

void CDC_ThinClientGroup_Tests::setUp()
{	
	pDB = new CppMySQLDB();
	pDB->setOptions(MYSQL_SET_CHARSET_NAME, "gbk");	
	pDB->connect(Host, USER, PASSWORD);
	pDB->dropDB(gszDB);
	pDB->createDB(gszDB);
	pDB->open(gszDB);

	string sql = "CREATE TABLE IF NOT EXISTS CDC_ThinClientGroup (\
		ThinClientGroup_ID bigint(20) not null AUTO_INCREMENT,  /*ÊÝÖÕ¶ËID*/\
		ThinClientGroup_Name Varchar(255) not null,             /*ÊÝÖÕ¶Ë×éID*/\
		PRIMARY KEY(ThinClientGroup_ID)\
		) ENGINE = InnoDB DEFAULT CHARSET = utf8;";

	pDB->execDML(sql);
	_pObj = new CDC_ThinClientGroup(pDB);
}


void CDC_ThinClientGroup_Tests::tearDown()
{
	pDB->dropDB(gszDB);
	pDB->close();
	delete pDB;
	pDB = NULL;
}

void CDC_ThinClientGroup_Tests::testJsonAdd()
{
	// success
	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddStringToObject(json, "ThinClientGroup_Name", "testJsonAdd");
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_ThinClientGroup_Add(req);

	const string success = "{\n\t\"Result\":\t1,\n\t\"ThinClientGroup_ID\":\t1\n}";
	assert(success == result);

	// failed
	json = cJSON_CreateObject();
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);

	const string failed = FAILED_JSON_RESULT;
	result = _pObj->CDC_ThinClientGroup_Add(req);
	assert(failed == result);
}


void CDC_ThinClientGroup_Tests::testJsonDel()
{
	TCDC_ThinClientGroup ThinClientGroup1;
	ThinClientGroup1.ThinClientGroup_Name = "testDel1";
	id = _pObj->ThinClientGroup_Add(ThinClientGroup1);
	assert(id > 0);

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddNumberToObject(json, "ThinClientGroup_ID", id);
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_ThinClientGroup_Del(req);
	assert(SUCCESS_JSON_RESULT == result);
}

void CDC_ThinClientGroup_Tests::testJsonUpdate()
{
	TCDC_ThinClientGroup ThinClientGroup1;
	ThinClientGroup1.ThinClientGroup_Name = "testJsonUpdate";
	id = _pObj->ThinClientGroup_Add(ThinClientGroup1);
	assert(id > 0);

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddNumberToObject(json, "ThinClientGroup_ID", id);
	cJSON_AddStringToObject(json, "ThinClientGroup_Name", "testJsonUpdate222");
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_ThinClientGroup_Update(req);
	assert(SUCCESS_JSON_RESULT == result);
}

void CDC_ThinClientGroup_Tests::testJsonFind()
{
	TCDC_ThinClientGroup ThinClientGroup1;
	ThinClientGroup1.ThinClientGroup_Name = "testJsonFind11";
	id = _pObj->ThinClientGroup_Add(ThinClientGroup1);
	assert(id > 0);

	TCDC_ThinClientGroup ThinClientGroup2;
	ThinClientGroup2.ThinClientGroup_Name = "testJsonFind22";
	id = _pObj->ThinClientGroup_Add(ThinClientGroup2);
	assert(id > 0);

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddNumberToObject(json, "ThinClientGroup_ID", id);
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_ThinClientGroup_Find(req);
	assert(result != FAILED_JSON_RESULT);
	MDEBUG << result << endl;

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "All");
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_ThinClientGroup_Find(req);
	MDEBUG << result << endl;
	assert(result != FAILED_JSON_RESULT);
}

void CDC_ThinClientGroup_Tests::testJsonFindCount()
{
	TCDC_ThinClientGroup ThinClientGroup1;
	ThinClientGroup1.ThinClientGroup_Name = "testJsonFindCount11";
	id = _pObj->ThinClientGroup_Add(ThinClientGroup1);
	assert(id > 0);

	TCDC_ThinClientGroup ThinClientGroup2;
	ThinClientGroup2.ThinClientGroup_Name = "testJsonFindCount22";
	id = _pObj->ThinClientGroup_Add(ThinClientGroup2);
	assert(id > 0);

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddNumberToObject(json, "ThinClientGroup_ID", id);
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_ThinClientGroup_FindCount(req);
	MDEBUG << result << endl;
	assert(result != FAILED_JSON_RESULT);

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "All");
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_ThinClientGroup_FindCount(req);
	MDEBUG << result << endl;
	assert(result != FAILED_JSON_RESULT);
}


CppUnit::Test* CDC_ThinClientGroup_Tests::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("CDC_ThinClientGroup_Tests");

	
	CppUnit_addTest(pSuite, CDC_ThinClientGroup_Tests, testJsonAdd);
	CppUnit_addTest(pSuite, CDC_ThinClientGroup_Tests, testJsonDel);
	CppUnit_addTest(pSuite, CDC_ThinClientGroup_Tests, testJsonUpdate);
	CppUnit_addTest(pSuite, CDC_ThinClientGroup_Tests, testJsonFind);
	CppUnit_addTest(pSuite, CDC_ThinClientGroup_Tests, testJsonFindCount);

	return pSuite;
}
