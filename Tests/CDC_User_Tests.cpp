#include "CDC_User_Tests.h"
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

CDC_User_Tests::CDC_User_Tests(const std::string& name) 
	: CppUnit::TestCase(name)
{
}

CDC_User_Tests::~CDC_User_Tests()
{
}

void CDC_User_Tests::dependsFuc()
{
	try
	{
		string sql = "CREATE TABLE IF NOT EXISTS CDC_UserGroup (\
			UserGroup_ID bigint(20) not null DEFAULT 1,\
			UserGroup_Name Varchar(255) not null,\
			PRIMARY KEY(UserGroup_ID)\
			) ENGINE = InnoDB; ";

		pDB->execDML(sql);

		_pCDC_UserGroupObj = new CDC_UserGroup(pDB);

		TCDC_UserGroup userGroup1;
		userGroup1.UserGroup_ID = 1001;
		userGroup1.UserGroup_Name = "userGroup1";
		id = _pCDC_UserGroupObj->UserGroup_Add(userGroup1);
		assert(id > 0);

		TCDC_UserGroup userGroup2;
		userGroup2.UserGroup_ID = 1002;
		userGroup2.UserGroup_Name = "userGroup2";
		id = _pCDC_UserGroupObj->UserGroup_Add(userGroup2);
		assert(id > 0);
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
	}
}

void CDC_User_Tests::setUp()
{	
	try
	{
		pDB = new CppMySQLDB();
		pDB->setOptions(MYSQL_SET_CHARSET_NAME, "gbk");	
		pDB->connect(HOST, USER, PASSWORD);
		pDB->dropDB(gszDB);
		pDB->createDB(gszDB);
		pDB->open(gszDB);

		dependsFuc();

		string sql = "CREATE TABLE IF NOT EXISTS CDC_User (\
			User_ID bigint(20) not null AUTO_INCREMENT,\
			User_Role INT not null,\
			User_UserGroup_ID bigint(20) not null,\
			User_Name Varchar(255) not null,\
			User_LoginName Varchar(255) not null,\
			User_Password Varchar(255) not null,\
			User_Remarks Varchar(255),\
			PRIMARY KEY(User_ID),\
			FOREIGN KEY(User_UserGroup_ID)\
			REFERENCES CDC_UserGroup(UserGroup_ID)\
			ON DELETE CASCADE\
			) ENGINE = InnoDB; ";
		pDB->execDML(sql);
		_pObj = new CDC_User(pDB);
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
	}
}


void CDC_User_Tests::tearDown()
{
	pDB->dropDB(gszDB);
	pDB->close();
	delete pDB;
	pDB = NULL;
}


void CDC_User_Tests::testJsonAdd()
{
	// success
	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddNumberToObject(json, "User_Role", 1);
	cJSON_AddNumberToObject(json, "User_UserGroup_ID", 1001);
	cJSON_AddStringToObject(json, "User_Name", "testJsonAdd¹þ¹þ");
	cJSON_AddStringToObject(json, "User_LoginName", "testJsonAdd_11");
	cJSON_AddStringToObject(json, "User_Password", "testJsonAdd_222");
	cJSON_AddStringToObject(json, "User_Remarks", "haha¹þ¹þ");
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_User_Add(req);

	const string success = "{\n\t\"Result\":\t1,\n\t\"User_ID\":\t1\n}";
	assert(success == result);

	// failed
	json = cJSON_CreateObject();
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);

	const string failed = FAILED_JSON_RESULT;
	result = _pObj->CDC_User_Add(req);
	assert(failed == result);
}


void CDC_User_Tests::testJsonDel()
{
	TCDC_User User1;
	User1.User_Role = 1;
	User1.User_UserGroup_ID = 1001;
	User1.User_Name = "testJsonDel1";
	User1.User_LoginName = "¹þ¹þ¹þ";
	User1.User_Password = "testJsonDel2222";
	id = _pObj->User_Add(User1);
	assert(id > 0);

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddNumberToObject(json, "User_ID", id);
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_User_Del(req);
	assert(SUCCESS_JSON_RESULT == result);
}

void CDC_User_Tests::testJsonUpdate()
{
	TCDC_User User1;
	User1.User_Role = 1;
	User1.User_UserGroup_ID = 1001;
	User1.User_Name = "testJsonUpdate1";
	User1.User_LoginName = "¹þ¹þ¹þ";
	User1.User_Password = "testJsonUpdate2222";
	id = _pObj->User_Add(User1);
	assert(id > 0);

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddNumberToObject(json, "User_ID", id);
	cJSON_AddNumberToObject(json, "User_Role", 1);
	cJSON_AddNumberToObject(json, "User_UserGroup_ID", 1001);
	cJSON_AddStringToObject(json, "User_Name", "testJsonAdd¹þ¹þ");
	cJSON_AddStringToObject(json, "User_LoginName", "testJsonAdd_11");
	cJSON_AddStringToObject(json, "User_Password", "testJsonAdd_222");
	cJSON_AddStringToObject(json, "User_Remarks", "haha¹þ¹þ");
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_User_Update(req);
	assert(SUCCESS_JSON_RESULT == result);
}

void CDC_User_Tests::testJsonFind()
{
	TCDC_User User1;
	User1.User_Role = 1;
	User1.User_UserGroup_ID = 1001;
	User1.User_Name = "testJsonFind11";
	User1.User_LoginName = "¹þ¹þ¹þ11";
	User1.User_Password = "testJsonFind111";
	User1.User_Remarks = "±¸×¢dsds";
	id = _pObj->User_Add(User1);
	assert(id > 0);

	TCDC_User User2;
	User2.User_Role = 1;
	User2.User_UserGroup_ID = 1001;
	User2.User_Name = "testJsonFind22";
	User2.User_LoginName = "¹þ¹þ¹þ222";
	User2.User_Password = "testJsonFind22";
	id = _pObj->User_Add(User2);
	assert(id > 0);

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddNumberToObject(json, "User_Role", 1);
	cJSON_AddStringToObject(json, "User_Name", "testJsonFind22");
	cJSON_AddStringToObject(json, "User_LoginName", "¹þ¹þ¹þ222");
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_User_Find(req);
	MDEBUG << result << endl;
	assert(result != FAILED_JSON_RESULT);

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "All");
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_User_Find(req);
	MDEBUG << result << endl;
	assert(result != FAILED_JSON_RESULT);
}

void CDC_User_Tests::testJsonFindCount()
{
	TCDC_User User1;
	User1.User_Role = 1;
	User1.User_UserGroup_ID = 1001;
	User1.User_Name = "testJsonFindCount11";
	User1.User_LoginName = "¹þ¹þ¹þ11";
	User1.User_Password = "testJsonFindCount11111";
	id = _pObj->User_Add(User1);
	assert(id > 0);

	TCDC_User User2;
	User2.User_Role = 1;
	User2.User_UserGroup_ID = 1001;
	User2.User_Name = "testJsonFindCount22";
	User2.User_LoginName = "¹þ¹þ¹þ222";
	User2.User_Password = "testJsonFindCount22";
	id = _pObj->User_Add(User2);
	assert(id > 0);

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddNumberToObject(json, "User_Role", 1);
	cJSON_AddNumberToObject(json, "User_UserGroup_ID", 1001);
	cJSON_AddStringToObject(json, "User_Name", "testJsonFindCount11");
	cJSON_AddStringToObject(json, "User_LoginName", "¹þ¹þ¹þ11");
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_User_FindCount(req);
	MDEBUG << result << endl;
	assert(result != FAILED_JSON_RESULT);

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "All");
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_User_FindCount(req);
	MDEBUG << result << endl;
	assert(result != FAILED_JSON_RESULT);
}

void CDC_User_Tests::testCount()
{
	int count = _pObj->User_Count();
	assert(count == 0);

	TCDC_User user;
	user.User_ID = 3003;
	user.User_Role = 0;
	user.User_UserGroup_ID = 1001;
	user.User_Name = "¼ÆÊý";
	user.User_LoginName = "test_Count11";
	user.User_Password = "test_Count1";
	user.User_Remarks = "±¸×¢123_";
	id = _pObj->User_Add(user);
	assert(id > 0);

	count = _pObj->User_Count();
	assert(count == 1);
}

void CDC_User_Tests::testAdd()
{
	TCDC_User user;
	user.User_ID = 10011;
	user.User_Role = 0;
	user.User_UserGroup_ID = 1001;
	user.User_Name = "Ôö¼Ó";
	user.User_LoginName = "test_Add1";
	user.User_Password = "test_Add1";
	user.User_Remarks = "±¸×¢123_";
	id = _pObj->User_Add(user);
	assert(id > 0);
}

void CDC_User_Tests::testDel()
{
	TCDC_User user1;
	user1.User_ID = 10011;
	user1.User_Role = 0;
	user1.User_UserGroup_ID = 1001;
	user1.User_Name = "É¾³ý";
	user1.User_LoginName = "test_Del1";
	user1.User_Password = "test_Del1";
	user1.User_Remarks = "±¸×¢123_";
	id = _pObj->User_Add(user1);
	assert(id > 0);

	TCDC_User user2;
	user2.User_ID = 10012;
	user2.User_Role = 0;
	user2.User_UserGroup_ID = 1002;
	user2.User_Name = "É¾³ý";
	user2.User_LoginName = "test_Del2";
	user2.User_Password = "test_Del2";
	user2.User_Remarks = "±¸×¢123_";
	id = _pObj->User_Add(user2);
	assert(id > 0);

	ret = _pObj->User_Del(1002);
	assert(ret != 0);

	ret = _pObj->User_Del(10012);
	assert(ret == success);
}

void CDC_User_Tests::testUpdate()
{
	TCDC_User user1;
	user1.User_ID = 10011;
	user1.User_Role = 0;
	user1.User_UserGroup_ID = 1001;
	user1.User_Name = "¸üÐÂ";
	user1.User_LoginName = "test_Update2";
	user1.User_Password = "test_Update2";
	user1.User_Remarks = "±¸×¢123_";
	id = _pObj->User_Add(user1);
	assert(id > 0);

	TCDC_User user2;
	user2.User_ID = 10011;
	user2.User_Role = 1;
	user2.User_UserGroup_ID = 1002;
	user2.User_Name = "¸üÐÂ22";
	user2.User_LoginName = "test_Update33";
	user2.User_Password = "test_Update44";
	user2.User_Remarks = "°¡°¡";
	ret = _pObj->User_Update(user2);
	assert(ret == success);
}

void CDC_User_Tests::testFind()
{
	TCDC_User user1;
	user1.User_ID = 10011;
	user1.User_Role = 0;
	user1.User_UserGroup_ID = 1001;
	user1.User_Name = "²éÕÒ";
	user1.User_LoginName = "test_Find1";
	user1.User_Password = "test_Find1";
	user1.User_Remarks = "±¸×¢123_";
	id = _pObj->User_Add(user1);
	assert(id > 0);

	TCDC_User user2;
	user2.User_ID = 10012;
	user2.User_Role = 0;
	user2.User_UserGroup_ID = 1002;
	user2.User_Name = "²éÕÒ";
	user2.User_LoginName = "test_Find2";
	user2.User_Password = "test_Find2";
	user2.User_Remarks = "±¸×¢123_";
	id = _pObj->User_Add(user2);
	assert(id > 0);

	bool find = _pObj->User_Find(10011);
	assert(find == true);

	string name = "test_Find1";
	find = _pObj->User_Find(name);
	assert(find == true);

	TCDC_User t1;
	ret = _pObj->User_Find(10011, t1);
	assert(ret == success);
	assert(t1.User_LoginName == "test_Find1");

	TCDC_User t2;
	name = "test_Find2";
	ret = _pObj->User_Find(name, t2);
	assert(ret == success);
	assert(t2.User_ID == 10012);
}


CppUnit::Test* CDC_User_Tests::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("CDC_User_Tests");

	CppUnit_addTest(pSuite, CDC_User_Tests, testJsonAdd);
	CppUnit_addTest(pSuite, CDC_User_Tests, testJsonDel);
	CppUnit_addTest(pSuite, CDC_User_Tests, testJsonUpdate);
	CppUnit_addTest(pSuite, CDC_User_Tests, testJsonFind);
	CppUnit_addTest(pSuite, CDC_User_Tests, testJsonFindCount);

	/*CppUnit_addTest(pSuite, CDC_User_Tests, testCount);
	CppUnit_addTest(pSuite, CDC_User_Tests, testAdd);
	CppUnit_addTest(pSuite, CDC_User_Tests, testDel);
	CppUnit_addTest(pSuite, CDC_User_Tests, testUpdate);
	CppUnit_addTest(pSuite, CDC_User_Tests, testFind);*/

	return pSuite;
}
