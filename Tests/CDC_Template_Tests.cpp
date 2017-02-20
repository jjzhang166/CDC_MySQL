#include "CDC_Template_Tests.h"
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

CDC_Template_Tests::CDC_Template_Tests(const std::string& name) 
	: CppUnit::TestCase(name)
{
}

CDC_Template_Tests::~CDC_Template_Tests()
{
}

void CDC_Template_Tests::dependsFuc()
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

		pDB->execDML(sql);

		_pCDC_HostObj = new CDC_Host(pDB);

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

void CDC_Template_Tests::setUp()
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

		string sql = "CREATE TABLE IF NOT EXISTS CDC_Template (\
			Template_ID bigint(20) not null AUTO_INCREMENT, /*模板ID*/\
			Template_HostID bigint(20) not null,            /*模板所在主机ID*/\
			Template_CoreNum INT not null,                  /*CPU内核数*/\
			Template_MemSize INT not null,                  /*内存容量（单位：G）*/\
			Template_SysSize INT not null,                  /*系统盘容量（单位：G*/\
			Template_Version INT not null,                  /*版本号*/\
			Template_Mode INT not null,                     /*模式（0：教学/1：办公）*/\
			Template_State INT not null,                    /*模板状态（在线：1/离线：0*/\
			Template_Name Varchar(255) not null,            /*模板名*/\
			Template_OS Varchar(255) not null,              /*系统类型*/\
			Template_Path Varchar(255) not null,            /*模板所在路径*/\
			Template_FileName Varchar(255) not null,        /*模板文件名*/\
			Template_RollBackFile Varchar(255),             /*回滚快照名*/\
			Template_BackUpFile Varchar(255) not null,      /*备份模板名*/\
			Template_MAC Varchar(255) not null,				/*模板Mac地址*/\
			Template_SpicePwd Varchar(255) not null,        /*Spice密码*/\
			Template_SpicePort Varchar(255) not null,       /*Spice端口*/\
			Template_Uuid Varchar(255),                    /*模板内部标识名*/\
			Template_Create_Time DateTime,                  /*模板创建时间*/\
			Template_Update_Time DateTime,                  /*模板最后修改时间*/\
			PRIMARY KEY(Template_ID),\
			FOREIGN KEY(Template_HostID)\
			REFERENCES CDC_Host(Host_ID)\
			ON DELETE CASCADE\
			) ENGINE = InnoDB DEFAULT CHARSET = utf8; ";
		pDB->execDML(sql);
		_pObj = new CDC_Template(pDB);
	}
	catch (CppMySQLException& e)
	{
		cerr << e.errorCode() << ":" << e.errorMessage();
	}
}

void CDC_Template_Tests::tearDown()
{
	pDB->dropDB(gszDB);
	pDB->close();
	delete pDB;
	pDB = NULL;
}


#define ADD_ONE_OBJECT \
		TCDC_Template t1;\
		t1.Template_HostID = 1001;\
		t1.Template_CoreNum = 1002;\
		t1.Template_MemSize = 1003;\
		t1.Template_SysSize = 1004;\
		t1.Template_Version = 1005;\
		t1.Template_Mode = 1006;\
		t1.Template_State = 1006;\
		t1.Template_Name = "testJson_1";\
		t1.Template_OS = "testJson_2";\
		t1.Template_Path = "testJson_3";\
		t1.Template_FileName = "testJson_4";\
		t1.Template_RollBackFile = "testJson_5";\
		t1.Template_BackUpFile = "testJson_6";\
		t1.Template_MAC = "testJson_55"; \
		t1.Template_SpicePwd = "testJson_7";\
		t1.Template_SpicePort = "testJson_72";\
		t1.Template_Uuid = "testJson_8";\
		t1.Template_Create_Time = "2017-12-31 12:56:30";\
		t1.Template_Update_Time = "2018-08-08 08:08:08";

void CDC_Template_Tests::testJsonAdd()
{
	// success
	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddNumberToObject(json, "Template_HostID", 1001);
	cJSON_AddNumberToObject(json, "Template_CoreNum", 1002);
	cJSON_AddNumberToObject(json, "Template_MemSize", 1003);
	cJSON_AddNumberToObject(json, "Template_SysSize", 1004);
	cJSON_AddNumberToObject(json, "Template_Version", 1005);
	cJSON_AddNumberToObject(json, "Template_Mode", 1006);
	cJSON_AddNumberToObject(json, "Template_State", 1006);
	cJSON_AddStringToObject(json, "Template_Name", "testJsonAdd_1");
	cJSON_AddStringToObject(json, "Template_OS", "testJsonAdd_2");
	cJSON_AddStringToObject(json, "Template_Path", "testJsonAdd_3");
	cJSON_AddStringToObject(json, "Template_FileName", "testJsonAdd_4");
	cJSON_AddStringToObject(json, "Template_RollBackFile", "testJsonAdd_5");
	cJSON_AddStringToObject(json, "Template_BackUpFile", "testJsonAdd_6");
	cJSON_AddStringToObject(json, "Template_MAC", "testJsonAdd_66");
	cJSON_AddStringToObject(json, "Template_SpicePwd", "testJsonAdd_7");
	cJSON_AddStringToObject(json, "Template_SpicePort", "testJsonAdd_71");
	cJSON_AddStringToObject(json, "Template_Uuid", "testJsonAdd_8");
	cJSON_AddStringToObject(json, "Template_Create_Time", "2017-12-31 12:56:30");
	cJSON_AddStringToObject(json, "Template_Update_Time", "2018-08-08 08:08:08");
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_Template_Add(req);

	const string success = "{\n\t\"Result\":\t1,\n\t\"Template_ID\":\t1\n}";
	assert(success == result);

	// failed
	json = cJSON_CreateObject();
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);

	const string failed = FAILED_JSON_RESULT;
	result = _pObj->CDC_Template_Add(req);
	assert(failed == result);
}


void CDC_Template_Tests::testJsonDel()
{
	ADD_ONE_OBJECT;
	id = _pObj->Template_Add(t1);
	assert(id > 0);

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddNumberToObject(json, "Template_ID", id);
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_Template_Del(req);
	assert(SUCCESS_JSON_RESULT == result);
}

void CDC_Template_Tests::testJsonUpdate()
{
	ADD_ONE_OBJECT;
	id = _pObj->Template_Add(t1);
	assert(id > 0);

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddNumberToObject(json, "Template_ID", id);
	cJSON_AddStringToObject(json, "Template_Name", "testJsonUpdate哈哈");
	cJSON_AddStringToObject(json, "Template_FileName", "XXXXXXX哈哈哈");
	cJSON_AddStringToObject(json, "Template_Create_Time", "2019-12-31 12:56:30");
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_Template_Update(req);
	assert(SUCCESS_JSON_RESULT == result);
}

void CDC_Template_Tests::testJsonFind()
{
	ADD_ONE_OBJECT;
	id = _pObj->Template_Add(t1);
	assert(id > 0);


	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddNumberToObject(json, "Template_HostID", 1001);
	//cJSON_AddStringToObject(json, "Template_Name", "testJson_1");
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_Template_Find(req);
	MDEBUG << result << endl;
	assert(result != FAILED_JSON_RESULT);

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "All");
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_Template_Find(req);
	MDEBUG << result << endl;
	assert(result != FAILED_JSON_RESULT);
}

void CDC_Template_Tests::testJsonFindCount()
{
	ADD_ONE_OBJECT;
	id = _pObj->Template_Add(t1);
	assert(id > 0);

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddNumberToObject(json, "Template_HostID", 1001);
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_Template_FindCount(req);
	MDEBUG << result << endl;
	assert(result != FAILED_JSON_RESULT);

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "All");
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_Template_FindCount(req);
	MDEBUG << result << endl;
	assert(result != FAILED_JSON_RESULT);
}


CppUnit::Test* CDC_Template_Tests::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("CDC_Template_Tests");

	CppUnit_addTest(pSuite, CDC_Template_Tests, testJsonAdd);
	CppUnit_addTest(pSuite, CDC_Template_Tests, testJsonDel);
	CppUnit_addTest(pSuite, CDC_Template_Tests, testJsonUpdate);
	CppUnit_addTest(pSuite, CDC_Template_Tests, testJsonFind);
	CppUnit_addTest(pSuite, CDC_Template_Tests, testJsonFindCount);

	return pSuite;
}
