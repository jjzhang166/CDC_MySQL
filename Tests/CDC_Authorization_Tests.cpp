#include "CDC_Authorization_Tests.h"
#include "CppMySQL.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "mysql.h"
#include "cJSON.h"
#include "TestsComDef.h"
#include "LogHelper.h"


#include <iostream>
using namespace std;

namespace
{
	const char* gszDB = "CDCMySQLTest";
	#define Authorization		"127.0.0.1"
	#define USER		"root"
	#define PASSWORD	"123456"
	int ret = -1;

	std::string id;
	cJSON *json = 0;
	char *out = 0;
	string req;
	string result;
}

CDC_Authorization_Tests::CDC_Authorization_Tests(const std::string& name) 
	: CppUnit::TestCase(name)
{
}

CDC_Authorization_Tests::~CDC_Authorization_Tests()
{
}

void CDC_Authorization_Tests::setUp()
{	
	try
	{
		CppMySQLDB::Instance().init();
		CppMySQLDB::Instance().setOptions(MYSQL_SET_CHARSET_NAME, "gbk");	
		CppMySQLDB::Instance().connect(Authorization, USER, PASSWORD);
		CppMySQLDB::Instance().dropDB(gszDB);
		CppMySQLDB::Instance().createDB(gszDB);
		CppMySQLDB::Instance().open(gszDB);

		string sql = "CREATE TABLE IF NOT EXISTS CDC_Authorization (\
			Authorization_MachineID Varchar(255) not null,  /*机器码*/\
			Authorization_MaxClientNum bigint(20) not null, /*设备数上限*/\
			Authorization_Deadline datetime not null,           /*到期时间*/\
			Authorization_Company Varchar(255) not null,    /*单位名称*/\
			PRIMARY KEY(Authorization_MachineID)\
			) ENGINE = InnoDB DEFAULT CHARSET = utf8; ";
		execDML(sql);
		_pObj = new CDC_Authorization();
	}
	catch (CppMySQLException& e)
	{
		cerr << e.errorCode() << ":" << e.errorMessage();
	}
}

void CDC_Authorization_Tests::tearDown()
{
	CppMySQLDB::Instance().dropDB(gszDB);
	CppMySQLDB::Instance().close();
}

void CDC_Authorization_Tests::testJsonAdd()
{
	// success
	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddStringToObject(json, "Authorization_MachineID", "XXaa11");
	cJSON_AddNumberToObject(json, "Authorization_MaxClientNum", 11);
	cJSON_AddStringToObject(json, "Authorization_Deadline", "2017-12-31 12:56:30");
	cJSON_AddStringToObject(json, "Authorization_Company", "testJsonAdd_222");
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_Authorization_Add(req);

	const string success = "{\n\t\"Result\":\t1,\n\t\"Authorization_MachineID\":\t\"XXaa11\"\n}";
	assert(success == result);

	// failed
	json = cJSON_CreateObject();
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);

	const string failed = FAILED_JSON_RESULT;
	result = _pObj->CDC_Authorization_Add(req);
	assert(failed == result);
}


#define ADD_ONE_OBJECT \
		TCDC_Authorization t1;\
		t1.Authorization_MachineID = "XXaa11";\
		t1.Authorization_MaxClientNum = 11;\
		t1.Authorization_Deadline = "2017-12-31 12:56:30";\
		t1.Authorization_Company = "testJsonAdd_222";\
		id = _pObj->Authorization_Add(t1);

void CDC_Authorization_Tests::testJsonDel()
{
	ADD_ONE_OBJECT;
	assert(id != "");

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddStringToObject(json, "Authorization_MachineID", id.c_str());
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_Authorization_Del(req);
	assert(SUCCESS_JSON_RESULT == result);
}

void CDC_Authorization_Tests::testJsonUpdate()
{
	ADD_ONE_OBJECT;
	assert(id != "");

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddStringToObject(json, "Authorization_MachineID", id.c_str());
	cJSON_AddNumberToObject(json, "Authorization_MaxClientNum", 22);
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_Authorization_Update(req);
	assert(SUCCESS_JSON_RESULT == result);
}

void CDC_Authorization_Tests::testJsonFind()
{
	ADD_ONE_OBJECT;
	assert(id != "");

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddStringToObject(json, "Authorization_MachineID", id.c_str());
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_Authorization_Find(req);
	MDEBUG << result << endl;
	assert(result != FAILED_JSON_RESULT);

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "All");
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_Authorization_Find(req);
	MDEBUG << result << endl;
	assert(result != FAILED_JSON_RESULT);
}

void CDC_Authorization_Tests::testJsonFindCount()
{
	ADD_ONE_OBJECT;
	assert(id != "");

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddStringToObject(json, "Authorization_MachineID", id.c_str());
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_Authorization_FindCount(req);
	MDEBUG << result << endl;
	assert(result != FAILED_JSON_RESULT);

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "All");
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_Authorization_FindCount(req);
	MDEBUG << result << endl;
	assert(result != FAILED_JSON_RESULT);
}

CppUnit::Test* CDC_Authorization_Tests::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("CDC_Authorization_Tests");

	CppUnit_addTest(pSuite, CDC_Authorization_Tests, testJsonAdd);
	CppUnit_addTest(pSuite, CDC_Authorization_Tests, testJsonDel);
	CppUnit_addTest(pSuite, CDC_Authorization_Tests, testJsonUpdate);
	CppUnit_addTest(pSuite, CDC_Authorization_Tests, testJsonFind);
	CppUnit_addTest(pSuite, CDC_Authorization_Tests, testJsonFindCount);

	return pSuite;
}
