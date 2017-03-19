#include "CDC_ISO_Tests.h"
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

CDC_ISO_Tests::CDC_ISO_Tests(const std::string& name) 
	: CppUnit::TestCase(name)
{
}

CDC_ISO_Tests::~CDC_ISO_Tests()
{
}

void CDC_ISO_Tests::dependsFuc()
{
	try
	{
		string sql = "CREATE TABLE IF NOT EXISTS CDC_Host (\
			Host_ID bigint(20) not null AUTO_INCREMENT,     /*主机ID*/\
			Host_Name Varchar(255) not null,                /*主机名*/\
			Host_IP Varchar(255) not null,                  /*主机IP*/\
			Host_VirtNet Varchar(255) not null,             /*主机虚拟网络*/\
			PRIMARY KEY(Host_ID)\
			) ENGINE = InnoDB DEFAULT CHARSET = utf8; ";

		execDML(sql);

		_pCDC_HostObj = new CDC_Host();

		TCDC_Host t1;
		t1.Host_ID = 1001;
		t1.Host_Name = "testJson1";
		t1.Host_IP = "127.0.0.1";
		t1.Host_VirtNet = "testJson1111";
		id = _pCDC_HostObj->Host_Add(t1);
		assert(id > 0);

		TCDC_Host t2;
		t2.Host_ID = 1002;
		t2.Host_Name = "testJson2";
		t2.Host_IP = "127.0.0.1";
		t2.Host_VirtNet = "testJson2222";
		id = _pCDC_HostObj->Host_Add(t2);
		assert(id > 0);
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
	}
}

void CDC_ISO_Tests::setUp()
{	
	try
	{
		CppMySQLDB::Instance().init();
		CppMySQLDB::Instance().setOptions(MYSQL_SET_CHARSET_NAME, "gbk");	
		CppMySQLDB::Instance().connect(Host, USER, PASSWORD);
		CppMySQLDB::Instance().dropDB(gszDB);
		CppMySQLDB::Instance().createDB(gszDB);
		CppMySQLDB::Instance().open(gszDB);
		dependsFuc();

		string sql = "CREATE TABLE IF NOT EXISTS CDC_ISO (\
			ISO_ID bigint(20) not null AUTO_INCREMENT,      /*ISO的ID*/\
			ISO_HostID bigint(20) not null,                 /*ISO所在主机ID*/\
			ISO_Name Varchar(255) not null,                 /*ISO名*/\
			ISO_Path Varchar(255) not null,                 /*ISO文件所在路径*/\
			PRIMARY KEY(ISO_ID),\
			FOREIGN KEY(ISO_HostID)\
			REFERENCES CDC_Host(Host_ID)\
			ON DELETE CASCADE\
			) ENGINE = InnoDB DEFAULT CHARSET = utf8; ";
		execDML(sql);
		_pObj = new CDC_ISO();
	}
	catch (CppMySQLException& e)
	{
		cerr << e.errorCode() << ":" << e.errorMessage();
	}
}

void CDC_ISO_Tests::tearDown()
{
	CppMySQLDB::Instance().dropDB(gszDB);
	CppMySQLDB::Instance().close();
}

void CDC_ISO_Tests::testJsonAdd()
{
	// success
	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddNumberToObject(json, "ISO_HostID", 1001);
	cJSON_AddStringToObject(json, "ISO_Name", "testJsonAdd11");
	cJSON_AddStringToObject(json, "ISO_Path", "testJsonAdd_222");
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_ISO_Add(req);

	const string success = "{\n\t\"Result\":\t1,\n\t\"ISO_ID\":\t1\n}";
	assert(success == result);

	// failed
	json = cJSON_CreateObject();
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);

	const string failed = FAILED_JSON_RESULT;
	result = _pObj->CDC_ISO_Add(req);
	assert(failed == result);
}


#define ADD_ONE_OBJECT \
		TCDC_ISO t1;\
		t1.ISO_HostID = 1001;\
		t1.ISO_Name = "testJson_1";\
		t1.ISO_Path = "testJson_2";\
		id = _pObj->ISO_Add(t1);

void CDC_ISO_Tests::testJsonDel()
{
	ADD_ONE_OBJECT;
	assert(id > 0);

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddNumberToObject(json, "ISO_ID", id);
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_ISO_Del(req);
	assert(SUCCESS_JSON_RESULT == result);
}

void CDC_ISO_Tests::testJsonUpdate()
{
	ADD_ONE_OBJECT;
	assert(id > 0);

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddNumberToObject(json, "ISO_ID", id);
	cJSON_AddNumberToObject(json, "ISO_HostID", 1001);
	cJSON_AddStringToObject(json, "ISO_Name", "testJsonUpdate哈哈");
	cJSON_AddStringToObject(json, "ISO_Path", "testJsonUpdate22");
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_ISO_Update(req);
	assert(SUCCESS_JSON_RESULT == result);
}

void CDC_ISO_Tests::testJsonFind()
{
	ADD_ONE_OBJECT;
	assert(id > 0);

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddNumberToObject(json, "ISO_HostID", 1001);
	cJSON_AddStringToObject(json, "Template_Name", "testJson_1");
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_ISO_Find(req);
	MDEBUG << result << endl;
	assert(result != FAILED_JSON_RESULT);

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "All");
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_ISO_Find(req);
	MDEBUG << result << endl;
	assert(result != FAILED_JSON_RESULT);
}

void CDC_ISO_Tests::testJsonFindCount()
{
	ADD_ONE_OBJECT;
	id = _pObj->ISO_Add(t1);
	assert(id > 0);

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddNumberToObject(json, "ISO_HostID", 1001);
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_ISO_FindCount(req);
	MDEBUG << result << endl;
	assert(result != FAILED_JSON_RESULT);

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "All");
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_ISO_FindCount(req);
	MDEBUG << result << endl;
	assert(result != FAILED_JSON_RESULT);
}




CppUnit::Test* CDC_ISO_Tests::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("CDC_ISO_Tests");

	CppUnit_addTest(pSuite, CDC_ISO_Tests, testJsonAdd);
	CppUnit_addTest(pSuite, CDC_ISO_Tests, testJsonDel);
	CppUnit_addTest(pSuite, CDC_ISO_Tests, testJsonUpdate);
	CppUnit_addTest(pSuite, CDC_ISO_Tests, testJsonFind);
	CppUnit_addTest(pSuite, CDC_ISO_Tests, testJsonFindCount);

	return pSuite;
}
