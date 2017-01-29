#include "CDC_ThinClient_Tests.h"
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
	CppMySQLDB* pDB;
	const char* gszDB = "CDCMySQLTest";
	#define ThinClientGroup		"127.0.0.1"
	#define USER		"root"
	#define PASSWORD	"123456"
	int ret = -1;

	double id = -1;
	cJSON *json = 0;
	char *out = 0;
	string req;
	string result;
}

CDC_ThinClient_Tests::CDC_ThinClient_Tests(const std::string& name) 
	: CppUnit::TestCase(name)
{
}

CDC_ThinClient_Tests::~CDC_ThinClient_Tests()
{
}

void CDC_ThinClient_Tests::dependsFuc()
{
	try
	{
		string sql = "CREATE TABLE IF NOT EXISTS CDC_ThinClientGroup (\
			ThinClientGroup_ID bigint(20) not null AUTO_INCREMENT,  /*瘦终端ID*/\
			ThinClientGroup_Name Varchar(255) not null,             /*瘦终端组ID*/\
			PRIMARY KEY(ThinClientGroup_ID)\
			) ENGINE = InnoDB DEFAULT CHARSET = utf8;";

		pDB->execDML(sql);

		_pCDC_ThinClientGroupObj = new CDC_ThinClientGroup(pDB);

		TCDC_ThinClientGroup t1;
		t1.ThinClientGroup_ID = 1001;
		t1.ThinClientGroup_Name = "testJson1";
		id = _pCDC_ThinClientGroupObj->ThinClientGroup_Add(t1);
		assert(id > 0);

		TCDC_ThinClientGroup t2;
		t2.ThinClientGroup_ID = 1002;
		t2.ThinClientGroup_Name = "testJson2";
		id = _pCDC_ThinClientGroupObj->ThinClientGroup_Add(t2);
		assert(id > 0);
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
	}
}

void CDC_ThinClient_Tests::setUp()
{	
	try
	{
		pDB = new CppMySQLDB();
		pDB->setOptions(MYSQL_SET_CHARSET_NAME, "gbk");	
		pDB->connect(ThinClientGroup, USER, PASSWORD);
		pDB->dropDB(gszDB);
		pDB->createDB(gszDB);
		pDB->open(gszDB);
		dependsFuc();

		string sql = "CREATE TABLE IF NOT EXISTS CDC_ThinClient (\
			ThinClient_ID bigint(20) not null AUTO_INCREMENT,       /*瘦终端ID*/\
			ThinClient_ThinGroup_ID bigint(20) not null,            /*瘦终端组ID*/\
			ThinClient_Mode INT not null,                           /*模式（0：教学/1：办公）*/\
			ThinClient_Version Varchar(255) not null,               /*版本 */\
			ThinClient_State INT not null,                          /*状态（0：离线/1：在线）*/\
			ThinClient_Protocol INT not null,                       /*协议（0：VOI/1：VDI）*/\
			ThinClient_Name Varchar(255) not null,                  /*瘦终端名*/\
			ThinClient_IP Varchar(255) not null,                    /*IP地址*/\
			ThinClient_MAC Varchar(255) not null,                   /*MAC地址*/\
			PRIMARY KEY(ThinClient_ID),\
			FOREIGN KEY(ThinClient_ThinGroup_ID)\
			REFERENCES CDC_ThinClientGroup(ThinClientGroup_ID)\
			ON DELETE CASCADE\
			) ENGINE = InnoDB DEFAULT CHARSET = utf8;";
		pDB->execDML(sql);
		_pObj = new CDC_ThinClient(pDB);
	}
	catch (CppMySQLException& e)
	{
		cerr << e.errorCode() << ":" << e.errorMessage();
	}
}

void CDC_ThinClient_Tests::tearDown()
{
	pDB->dropDB(gszDB);
	pDB->close();
	delete pDB;
	pDB = NULL;
}

void CDC_ThinClient_Tests::testJsonAdd()
{
	// success
	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddNumberToObject(json, "ThinClient_ThinGroup_ID", 1001);
	cJSON_AddNumberToObject(json, "ThinClient_Mode", 1);
	cJSON_AddStringToObject(json, "ThinClient_Version", "testJsonAdd11");
	cJSON_AddNumberToObject(json, "ThinClient_State", 1);
	cJSON_AddNumberToObject(json, "ThinClient_Protocol", 1);
	cJSON_AddStringToObject(json, "ThinClient_Name", "testJsonAdd22");
	cJSON_AddStringToObject(json, "ThinClient_IP", "testJsonAdd_33");
	cJSON_AddStringToObject(json, "ThinClient_MAC", "testJsonAdd_44");
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_ThinClient_Add(req);

	const string success = "{\n\t\"Result\":\t1,\n\t\"ThinClient_ID\":\t1\n}";
	assert(success == result);

	// failed
	json = cJSON_CreateObject();
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);

	const string failed = FAILED_JSON_RESULT;
	result = _pObj->CDC_ThinClient_Add(req);
	assert(failed == result);
}


#define ADD_ONE_OBJECT \
		TCDC_ThinClient t1;\
		t1.ThinClient_ThinGroup_ID = 1001;\
		t1.ThinClient_Mode = 1;\
		t1.ThinClient_Version = "testJson_1";\
		t1.ThinClient_State = 1;\
		t1.ThinClient_Protocol = 1;\
		t1.ThinClient_Name = "testJson_2";\
		t1.ThinClient_IP = "testJson_3";\
		t1.ThinClient_MAC = "testJson_4";\
		id = _pObj->ThinClient_Add(t1);

void CDC_ThinClient_Tests::testJsonDel()
{
	ADD_ONE_OBJECT;
	assert(id > 0);

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddNumberToObject(json, "ThinClient_ID", id);
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_ThinClient_Del(req);
	assert(SUCCESS_JSON_RESULT == result);
}

void CDC_ThinClient_Tests::testJsonUpdate()
{
	ADD_ONE_OBJECT;
	assert(id > 0);

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddNumberToObject(json, "ThinClient_ID", id);
	cJSON_AddNumberToObject(json, "ThinClient_ThinGroup_ID", 1001);
	cJSON_AddStringToObject(json, "ThinClient_Name", "testJsonUpdate哈哈");
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_ThinClient_Update(req);
	assert(SUCCESS_JSON_RESULT == result);
}

void CDC_ThinClient_Tests::testJsonFind()
{
	ADD_ONE_OBJECT;
	assert(id > 0);

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddNumberToObject(json, "ThinClient_ThinGroup_ID", 1001);
	cJSON_AddStringToObject(json, "ThinClient_Name", "testJson_2");
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_ThinClient_Find(req);
	MDEBUG << result << endl;
	assert(result != FAILED_JSON_RESULT);

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "All");
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_ThinClient_Find(req);
	MDEBUG << result << endl;
	assert(result != FAILED_JSON_RESULT);
}

void CDC_ThinClient_Tests::testJsonFindCount()
{
	ADD_ONE_OBJECT;
	id = _pObj->ThinClient_Add(t1);
	assert(id > 0);

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddNumberToObject(json, "ThinClient_ThinGroup_ID", 1001);
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_ThinClient_FindCount(req);
	MDEBUG << result << endl;
	assert(result != FAILED_JSON_RESULT);

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "All");
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_ThinClient_FindCount(req);
	MDEBUG << result << endl;
	assert(result != FAILED_JSON_RESULT);
}




CppUnit::Test* CDC_ThinClient_Tests::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("CDC_ThinClient_Tests");

	CppUnit_addTest(pSuite, CDC_ThinClient_Tests, testJsonAdd);
	CppUnit_addTest(pSuite, CDC_ThinClient_Tests, testJsonDel);
	CppUnit_addTest(pSuite, CDC_ThinClient_Tests, testJsonUpdate);
	CppUnit_addTest(pSuite, CDC_ThinClient_Tests, testJsonFind);
	CppUnit_addTest(pSuite, CDC_ThinClient_Tests, testJsonFindCount);

	return pSuite;
}
