#include "CDC_EDU_VM_Tests.h"
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

CDC_EDU_VM_Tests::CDC_EDU_VM_Tests(const std::string& name) 
	: CppUnit::TestCase(name)
{
}

CDC_EDU_VM_Tests::~CDC_EDU_VM_Tests()
{
}

void CDC_EDU_VM_Tests::dependsFuc()
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

void CDC_EDU_VM_Tests::dependsFuc1()
{
	try
	{
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
			Template_SpicePwd Varchar(255) not null,        /*Spice密码*/\
			Template_Uuid Varchar(255),                    /*模板内部标识名*/\
			Template_Create_Time DateTime,                  /*模板创建时间*/\
			Template_Update_Time DateTime,                  /*模板最后修改时间*/\
			PRIMARY KEY(Template_ID),\
			FOREIGN KEY(Template_HostID)\
			REFERENCES CDC_Host(Host_ID)\
			ON DELETE CASCADE\
						) ENGINE = InnoDB DEFAULT CHARSET = utf8; ";

		pDB->execDML(sql);

		_pCDC_TemplateObj = new CDC_Template(pDB);

		TCDC_Template t1; 
		t1.Template_ID = 1001;
		t1.Template_HostID = 1001; 
		t1.Template_CoreNum = 1002; 
		t1.Template_MemSize = 1003; 
		t1.Template_SysSize = 1004; 
		t1.Template_Version = 1005; 
		t1.Template_Mode = 1006; 
		t1.Template_State = 1006; 
		t1.Template_Name = "testJson_1"; 
		t1.Template_OS = "testJson_2";
		t1.Template_Path = "testJson_3"; 
		t1.Template_FileName = "testJson_4"; 
		t1.Template_RollBackFile = "testJson_5"; 
		t1.Template_BackUpFile = "testJson_6"; 
		t1.Template_SpicePwd = "testJson_7"; 
		t1.Template_Uuid = "testJson_8"; 
		t1.Template_Create_Time = "2017-12-31 12:56:30"; 
		t1.Template_Update_Time = "2017-12-31 12:56:30";
		id = _pCDC_TemplateObj->Template_Add(t1);
		assert(id > 0);

		TCDC_Template t2;
		t2.Template_ID = 1002;
		t2.Template_HostID = 1002;
		t2.Template_HostID = 1002;
		t2.Template_CoreNum = 10022;
		t2.Template_MemSize = 10033;
		t2.Template_SysSize = 10044;
		t2.Template_Version = 10055;
		t2.Template_Mode = 10066;
		t2.Template_State = 10066;
		t2.Template_Name = "testJson_11";
		t2.Template_OS = "testJson_22";
		t2.Template_Path = "testJson_33";
		t2.Template_FileName = "testJson_44";
		t2.Template_RollBackFile = "testJson_55";
		t2.Template_BackUpFile = "testJson_66";
		t2.Template_SpicePwd = "testJson_77";
		t2.Template_Uuid = "testJson_88";
		t2.Template_Create_Time = "2016-12-31 12:56:30";
		t2.Template_Update_Time = "2016-12-31 12:56:30";
		id = _pCDC_TemplateObj->Template_Add(t2);
		assert(id > 0);
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
	}
}

void CDC_EDU_VM_Tests::dependsFuc2()
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


		sql = "CREATE TABLE IF NOT EXISTS CDC_ThinClient (\
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
			) ENGINE = InnoDB DEFAULT CHARSET = utf8; ";

		pDB->execDML(sql);

		_pCDC_ThinClientObj = new CDC_ThinClient(pDB);

		TCDC_ThinClient t3; 
		t3.ThinClient_ID = 1001;
		t3.ThinClient_ThinGroup_ID = 1001; 
		t3.ThinClient_Mode = 1; 
		t3.ThinClient_Version = "testJson_1"; 
		t3.ThinClient_State = 1; 
		t3.ThinClient_Protocol = 1; 
		t3.ThinClient_Name = "testJson_2"; 
		t3.ThinClient_IP = "testJson_3"; 
		t3.ThinClient_MAC = "testJson_4"; 
		id = _pCDC_ThinClientObj->ThinClient_Add(t3);
		assert(id > 0);

		TCDC_ThinClient t4;
		t4.ThinClient_ID = 1002;
		t4.ThinClient_ThinGroup_ID = 1002;
		t4.ThinClient_Mode = 1;
		t4.ThinClient_Version = "testJson_11";
		t4.ThinClient_State = 1;
		t4.ThinClient_Protocol = 1;
		t4.ThinClient_Name = "testJson_22";
		t4.ThinClient_IP = "testJson_33";
		t4.ThinClient_MAC = "testJson_44";
		id = _pCDC_ThinClientObj->ThinClient_Add(t4);
		assert(id > 0);
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
	}
}

void CDC_EDU_VM_Tests::setUp()
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
		dependsFuc1();
		dependsFuc2();

		string sql = "CREATE TABLE IF NOT EXISTS CDC_EDU_VM (\
			EDU_VM_ID bigint(20) not null AUTO_INCREMENT,   /*虚拟机ID*/\
			EDU_VM_Template_ID bigint(20) not null,         /*模板ID*/\
			EDU_VM_ThinClient_ID bigint(20) not null,       /*瘦终端ID*/\
			EDU_VM_State INT not null,                      /*虚拟机状态（0：离线/1：在线）*/\
			EDU_VM_SysPath Varchar(255) not null,           /*系统盘所在路径*/\
			EDU_VM_SysFilename Varchar(255) not null,       /*系统盘文件名*/\
			EDU_VM_MAC Varchar(255) not null,               /*虚拟机Mac地址*/\
			PRIMARY KEY(EDU_VM_ID),\
			FOREIGN KEY(EDU_VM_Template_ID)\
			REFERENCES CDC_Template(Template_ID)\
			ON DELETE CASCADE,\
			FOREIGN KEY(EDU_VM_ThinClient_ID)\
			REFERENCES CDC_ThinClient(ThinClient_ID)\
			ON DELETE CASCADE\
			) ENGINE = InnoDB DEFAULT CHARSET = utf8; ";
		pDB->execDML(sql);
		_pObj = new CDC_EDU_VM(pDB);
	}
	catch (CppMySQLException& e)
	{
		cerr << e.errorCode() << ":" << e.errorMessage();
	}
}

void CDC_EDU_VM_Tests::tearDown()
{
	pDB->dropDB(gszDB);
	pDB->close();
	delete pDB;
	pDB = NULL;
}


#define ADD_ONE_OBJECT \
		TCDC_EDU_VM t1;\
		t1.EDU_VM_Template_ID = 1001;\
		t1.EDU_VM_ThinClient_ID = 1001;\
		t1.EDU_VM_State = 1;\
		t1.EDU_VM_SysPath = "testJson_4";\
		t1.EDU_VM_SysFilename = "testJson_5";\
		t1.EDU_VM_MAC = "testJson_8";

void CDC_EDU_VM_Tests::testJsonAdd()
{
	// success
	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddNumberToObject(json, "EDU_VM_Template_ID", 1001);
	cJSON_AddNumberToObject(json, "EDU_VM_ThinClient_ID", 1001);
	cJSON_AddNumberToObject(json, "EDU_VM_State", 1);
	cJSON_AddStringToObject(json, "EDU_VM_SysPath", "testJson_4");
	cJSON_AddStringToObject(json, "EDU_VM_SysFilename", "testJson_5");
	cJSON_AddStringToObject(json, "EDU_VM_MAC", "testJson_8");
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_EDU_VM_Add(req);

	const string success = "{\n\t\"Result\":\t1,\n\t\"EDU_VM_ID\":\t1\n}";
	assert(success == result);

	// failed
	json = cJSON_CreateObject();
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);

	const string failed = FAILED_JSON_RESULT;
	result = _pObj->CDC_EDU_VM_Add(req);
	assert(failed == result);
}


void CDC_EDU_VM_Tests::testJsonDel()
{
	ADD_ONE_OBJECT;
	id = _pObj->EDU_VM_Add(t1);
	assert(id > 0);

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddNumberToObject(json, "EDU_VM_ID", id);
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_EDU_VM_Del(req);
	assert(SUCCESS_JSON_RESULT == result);
}

void CDC_EDU_VM_Tests::testJsonUpdate()
{
	ADD_ONE_OBJECT;
	id = _pObj->EDU_VM_Add(t1);
	assert(id > 0);

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddNumberToObject(json, "EDU_VM_ID", id);
	cJSON_AddNumberToObject(json, "EDU_VM_Template_ID", 1002);
	cJSON_AddStringToObject(json, "EDU_VM_SysPath", "XXXXXXX哈哈哈");
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_EDU_VM_Update(req);
	assert(SUCCESS_JSON_RESULT == result);
}

void CDC_EDU_VM_Tests::testJsonFind()
{
	ADD_ONE_OBJECT;
	id = _pObj->EDU_VM_Add(t1);
	assert(id > 0);


	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddNumberToObject(json, "EDU_VM_Template_ID", 1001);
	cJSON_AddNumberToObject(json, "EDU_VM_ThinClient_ID", 1001);
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_EDU_VM_Find(req);
	MDEBUG << result << endl;
	assert(result != FAILED_JSON_RESULT);

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "All");
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_EDU_VM_Find(req);
	MDEBUG << result << endl;
	assert(result != FAILED_JSON_RESULT);
}

void CDC_EDU_VM_Tests::testJsonFindCount()
{
	ADD_ONE_OBJECT;
	id = _pObj->EDU_VM_Add(t1);
	assert(id > 0);

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddNumberToObject(json, "EDU_VM_Template_ID", 1001);
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_EDU_VM_FindCount(req);
	MDEBUG << result << endl;
	assert(result != FAILED_JSON_RESULT);

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "All");
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_EDU_VM_FindCount(req);
	MDEBUG << result << endl;
	assert(result != FAILED_JSON_RESULT);
}


CppUnit::Test* CDC_EDU_VM_Tests::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("CDC_EDU_VM_Tests");

	CppUnit_addTest(pSuite, CDC_EDU_VM_Tests, testJsonAdd);
	CppUnit_addTest(pSuite, CDC_EDU_VM_Tests, testJsonDel);
	CppUnit_addTest(pSuite, CDC_EDU_VM_Tests, testJsonUpdate);
	CppUnit_addTest(pSuite, CDC_EDU_VM_Tests, testJsonFind);
	CppUnit_addTest(pSuite, CDC_EDU_VM_Tests, testJsonFindCount);

	return pSuite;
}
