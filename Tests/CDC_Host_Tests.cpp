#include "CDC_Host_Tests.h"
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
	#define HOST		"127.0.0.1"
	#define USER		"root"
	#define PASSWORD	"123456"
	int ret = -1;

	double id = -1;
	cJSON *json = 0;
	char *out = 0;
	string req;
	string result;
}

CDC_Host_Tests::CDC_Host_Tests(const std::string& name) 
	: CppUnit::TestCase(name)
{
}

CDC_Host_Tests::~CDC_Host_Tests()
{
}

void CDC_Host_Tests::setUp()
{	
	try
	{
		pDB = new CppMySQLDB();
		pDB->setOptions(MYSQL_SET_CHARSET_NAME, "gbk");	
		pDB->connect(HOST, USER, PASSWORD);
		pDB->dropDB(gszDB);
		pDB->createDB(gszDB);
		pDB->open(gszDB);

		string sql = "CREATE TABLE IF NOT EXISTS CDC_Host (\
			Host_ID bigint(20) not null DEFAULT 1,\
			Host_Name Varchar(255) not null,\
			Host_IP Varchar(255) not null,\
			Host_VirtNet Varchar(255) not null,\
			PRIMARY KEY(Host_ID)\
			) ENGINE = InnoDB DEFAULT CHARSET = utf8;";
		pDB->execDML(sql);
		_pObj = new CDC_Host(*pDB);
	}
	catch (CppMySQLException& e)
	{
		cerr << e.errorCode() << ":" << e.errorMessage();
	}
}

void CDC_Host_Tests::tearDown()
{
	pDB->dropDB(gszDB);
	pDB->close();
	delete pDB;
	pDB = NULL;
}

void CDC_Host_Tests::testJsonAdd()
{
	// success
	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddStringToObject(json, "Host_Name", "testJsonAdd哈哈");
	cJSON_AddStringToObject(json, "Host_IP", "127.0.0.1");
	cJSON_AddStringToObject(json, "Host_VirtNet", "testJsonAdd_222");
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_Host_Add(req);

	const string success = "{\n\t\"Result\":\t1,\n\t\"Host_ID\":\t1\n}";
	assert(success == result);

	// failed
	json = cJSON_CreateObject();
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);

	const string failed = FAILED_JSON_RESULT;
	result = _pObj->CDC_Host_Add(req);
	assert(failed == result);
}


void CDC_Host_Tests::testJsonDel()
{
	TCDC_Host t1;
	t1.Host_Name = "testJsonDel1";
	t1.Host_IP = "127.0.0.1";
	t1.Host_VirtNet = "testJsonDel2222";
	id = _pObj->Host_Add(t1);
	assert(id > 0);

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddNumberToObject(json, "Host_ID", id);
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_Host_Del(req);
	assert(SUCCESS_JSON_RESULT == result);
}

void CDC_Host_Tests::testJsonUpdate()
{
	TCDC_Host t1;
	t1.Host_Name = "testJsonUpdate1";
	t1.Host_IP = "127.0.0.1";
	t1.Host_VirtNet = "testJsonUpdate2222";
	id = _pObj->Host_Add(t1);
	assert(id > 0);

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddNumberToObject(json, "Host_ID", id);
	cJSON_AddStringToObject(json, "Host_Name", "testJsonUpdate哈哈");
	cJSON_AddStringToObject(json, "Host_IP", "127.0.0.1");
	cJSON_AddStringToObject(json, "Host_VirtNet", "testJsonUpdate22");
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_Host_Update(req);
	assert(SUCCESS_JSON_RESULT == result);
}

void CDC_Host_Tests::testJsonFind()
{
	TCDC_Host t1;
	t1.Host_Name = "testJsonFind11";
	t1.Host_IP = "127.0.0.1";
	t1.Host_VirtNet = "testJsonFind11aa";
	id = _pObj->Host_Add(t1);
	assert(id > 0);

	TCDC_Host t2;
	t2.Host_Name = "testJsonFind22";
	t2.Host_IP = "127.0.0.1";
	t2.Host_VirtNet = "testJsonFind22bb";
	id = _pObj->Host_Add(t2);
	assert(id > 0);

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddStringToObject(json, "Host_Name", "testJsonFind22");
	cJSON_AddStringToObject(json, "Host_IP", "127.0.0.1");
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_Host_Find(req);
	MDEBUG << result << endl;
	assert(result.length() > 0);

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "All");
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_Host_Find(req);
	MDEBUG << result << endl;
	assert(result.length() > 0);
}

void CDC_Host_Tests::testJsonFindCount()
{
	TCDC_Host t1;
	t1.Host_Name = "testJsonFindCount11";
	t1.Host_IP = "127.0.0.1";
	t1.Host_VirtNet = "testJsonFindCount11aa";
	id = _pObj->Host_Add(t1);
	assert(id > 0);

	TCDC_Host t2;
	t2.Host_Name = "testJsonFindCount22";
	t2.Host_IP = "127.0.0.1";
	t2.Host_VirtNet = "testJsonFindCount22bb";
	id = _pObj->Host_Add(t2);
	assert(id > 0);

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddStringToObject(json, "Host_IP", "127.0.0.1");
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_Host_FindCount(req);
	MDEBUG << result << endl;
	assert(result.length() > 0);

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "All");
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_Host_FindCount(req);
	MDEBUG << result << endl;
	assert(result.length() > 0);
}


void CDC_Host_Tests::testCount()
{
	int count = _pObj->Host_Count();
	assert(count == 0);

	TCDC_Host host;
	host.Host_ID = 3003;
	host.Host_Name = "计数";
	host.Host_IP = "test_Count11";
	host.Host_VirtNet = "test_Count1";
	id = _pObj->Host_Add(host);
	assert(id > 0);

	count = _pObj->Host_Count();
	assert(count == 1);
}

void CDC_Host_Tests::testAdd()
{
	TCDC_Host host;
	host.Host_ID = 10011;
	host.Host_Name = "增加";
	host.Host_IP = "test_Add1";
	host.Host_VirtNet = "test_Add1";
	id = _pObj->Host_Add(host);
	assert(id > 0);
}

void CDC_Host_Tests::testDel()
{
	TCDC_Host host1;
	host1.Host_ID = 10011;
	host1.Host_Name = "删除";
	host1.Host_IP = "test_Del1";
	host1.Host_VirtNet = "test_Del1";
	id = _pObj->Host_Add(host1);
	assert(id > 0);

	TCDC_Host host2;
	host2.Host_ID = 10012;
	host2.Host_Name = "删除";
	host2.Host_IP = "test_Del2";
	host2.Host_VirtNet = "test_Del2";
	id = _pObj->Host_Add(host2);
	assert(id > 0);

	ret = _pObj->Host_Del(1002);
	assert(ret != 0);

	ret = _pObj->Host_Del(10012);
	assert(ret == success);
}

void CDC_Host_Tests::testUpdate()
{
	TCDC_Host host1;
	host1.Host_ID = 10011;
	host1.Host_Name = "更新";
	host1.Host_IP = "test_Update2";
	host1.Host_VirtNet = "test_Update2";
	id = _pObj->Host_Add(host1);
	assert(id > 0);

	TCDC_Host host2;
	host2.Host_ID = 10011;
	host2.Host_Name = "更新22";
	host2.Host_IP = "test_Update33";
	host2.Host_VirtNet = "test_Update44";
	ret = _pObj->Host_Update(host2);
	assert(ret == success);
}

void CDC_Host_Tests::testFind()
{
	TCDC_Host host1;
	host1.Host_ID = 10011;
	host1.Host_Name = "查找1";
	host1.Host_IP = "test_Find1";
	host1.Host_VirtNet = "test_Find1";
	id = _pObj->Host_Add(host1);
	assert(id > 0);

	TCDC_Host host2;
	host2.Host_ID = 10012;
	host2.Host_Name = "查找2";
	host2.Host_IP = "test_Find2";
	host2.Host_VirtNet = "test_Find2";
	id = _pObj->Host_Add(host2);
	assert(id > 0);

	bool find = _pObj->Host_Find(10011);
	assert(find == true);

	string name = "查找1";
	find = _pObj->Host_Find(name);
	assert(find == true);

	TCDC_Host t1;
	ret = _pObj->Host_Find(10011, t1);
	assert(ret == success);
	assert(t1.Host_Name == "查找1");

	TCDC_Host t2;
	name = "查找2";
	ret = _pObj->Host_Find(name, t2);
	assert(ret == success);
	assert(t2.Host_ID == 10012);
}


CppUnit::Test* CDC_Host_Tests::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("CDC_Host_Tests");

	CppUnit_addTest(pSuite, CDC_Host_Tests, testJsonAdd);
	CppUnit_addTest(pSuite, CDC_Host_Tests, testJsonDel);
	CppUnit_addTest(pSuite, CDC_Host_Tests, testJsonUpdate);
	CppUnit_addTest(pSuite, CDC_Host_Tests, testJsonFind);
	CppUnit_addTest(pSuite, CDC_Host_Tests, testJsonFindCount);

	CppUnit_addTest(pSuite, CDC_Host_Tests, testCount);
	CppUnit_addTest(pSuite, CDC_Host_Tests, testAdd);
	CppUnit_addTest(pSuite, CDC_Host_Tests, testDel);
	CppUnit_addTest(pSuite, CDC_Host_Tests, testUpdate);
	CppUnit_addTest(pSuite, CDC_Host_Tests, testFind);

	return pSuite;
}
