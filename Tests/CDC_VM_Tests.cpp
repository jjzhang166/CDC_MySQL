#include "CDC_VM_Tests.h"
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

CDC_VM_Tests::CDC_VM_Tests(const std::string& name) 
	: CppUnit::TestCase(name)
{
}

CDC_VM_Tests::~CDC_VM_Tests()
{
}

void CDC_VM_Tests::dependsFuc()
{
	try
	{
		string sql = "CREATE TABLE IF NOT EXISTS CDC_Host (\
			Host_ID bigint(20) not null AUTO_INCREMENT,     /*����ID*/\
			Host_Name Varchar(255) not null,                /*������*/\
			Host_IP Varchar(255) not null,                  /*����IP*/\
			Host_VirtNet Varchar(255) not null,             /*������������*/\
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

void CDC_VM_Tests::dependsFuc1()
{
	try
	{
		string sql = "CREATE TABLE IF NOT EXISTS CDC_Template (\
			Template_ID bigint(20) not null AUTO_INCREMENT, /*ģ��ID*/\
			Template_HostID bigint(20) not null,            /*ģ����������ID*/\
			Template_CoreNum INT not null,                  /*CPU�ں���*/\
			Template_MemSize INT not null,                  /*�ڴ���������λ��G��*/\
			Template_SysSize INT not null,                  /*ϵͳ����������λ��G*/\
			Template_Version INT not null,                  /*�汾��*/\
			Template_Mode INT not null,                     /*ģʽ��0����ѧ/1���칫��*/\
			Template_State INT not null,                    /*ģ��״̬�����ߣ�1/���ߣ�0*/\
			Template_Name Varchar(255) not null,            /*ģ����*/\
			Template_OS Varchar(255) not null,              /*ϵͳ����*/\
			Template_Path Varchar(255) not null,            /*ģ������·��*/\
			Template_FileName Varchar(255) not null,        /*ģ���ļ���*/\
			Template_RollBackFile Varchar(255),             /*�ع�������*/\
			Template_BackUpFile Varchar(255) not null,      /*����ģ����*/\
			Template_MAC Varchar(255) not null,				/*ģ��Mac��ַ*/\
			Template_SpicePwd Varchar(255) not null,        /*Spice����*/\
			Template_SpicePort Varchar(255) not null,        /*Spice�˿�*/\
			Template_Uuid Varchar(255),                    /*ģ���ڲ���ʶ��*/\
			Template_Create_Time DateTime,                  /*ģ�崴��ʱ��*/\
			Template_Update_Time DateTime,                  /*ģ������޸�ʱ��*/\
			PRIMARY KEY(Template_ID),\
			FOREIGN KEY(Template_HostID)\
			REFERENCES CDC_Host(Host_ID)\
			ON DELETE CASCADE\
						) ENGINE = InnoDB DEFAULT CHARSET = utf8; ";

		execDML(sql);

		_pCDC_TemplateObj = new CDC_Template();

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
		t1.Template_MAC = "testJson_67";
		t1.Template_SpicePwd = "testJson_7"; 
		t1.Template_SpicePort = "testJson_71";
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
		t2.Template_MAC = "testJson_662";
		t2.Template_SpicePwd = "testJson_77";
		t2.Template_SpicePort = "testJson_72";
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

void CDC_VM_Tests::dependsFuc2()
{
	try
	{
		string sql = "CREATE TABLE IF NOT EXISTS CDC_ThinClientGroup (\
					ThinClientGroup_ID bigint(20) not null AUTO_INCREMENT,  /*���ն�ID*/\
					ThinClientGroup_Name Varchar(255) not null,             /*���ն���ID*/\
					PRIMARY KEY(ThinClientGroup_ID)\
					) ENGINE = InnoDB DEFAULT CHARSET = utf8;";

		execDML(sql);

		_pCDC_ThinClientGroupObj = new CDC_ThinClientGroup();

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
			ThinClient_ID bigint(20) not null AUTO_INCREMENT,       /*���ն�ID*/\
			ThinClient_ThinGroup_ID bigint(20) not null,            /*���ն���ID*/\
			ThinClient_Mode INT not null,                           /*ģʽ��0����ѧ/1���칫��*/\
			ThinClient_Version Varchar(255) not null,               /*�汾 */\
			ThinClient_State INT not null,                          /*״̬��0������/1�����ߣ�*/\
			ThinClient_Protocol INT not null,                       /*Э�飨0��VOI/1��VDI��*/\
			ThinClient_Name Varchar(255) not null,                  /*���ն���*/\
			ThinClient_IP Varchar(255) not null,                    /*IP��ַ*/\
			ThinClient_MAC Varchar(255) not null,                   /*MAC��ַ*/\
			PRIMARY KEY(ThinClient_ID),\
			FOREIGN KEY(ThinClient_ThinGroup_ID)\
			REFERENCES CDC_ThinClientGroup(ThinClientGroup_ID)\
			ON DELETE CASCADE\
			) ENGINE = InnoDB DEFAULT CHARSET = utf8; ";

		execDML(sql);

		_pCDC_ThinClientObj = new CDC_ThinClient();

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

void CDC_VM_Tests::dependsFuc3()
{
	try
	{
		string sql = "CREATE TABLE IF NOT EXISTS CDC_UserGroup (\
			UserGroup_ID bigint(20) not null DEFAULT 1,\
			UserGroup_Name Varchar(255) not null,\
			PRIMARY KEY(UserGroup_ID)\
			) ENGINE = InnoDB; ";

		execDML(sql);

		_pCDC_UserGroupObj = new CDC_UserGroup();

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


void CDC_VM_Tests::dependsFuc4()
{
	try
	{
		string sql = "CREATE TABLE IF NOT EXISTS CDC_User (\
			User_ID bigint(20) not null AUTO_INCREMENT,     /*�û�ID*/\
			User_Role INT not null,                         /*��ɫ��0������Ա:1���û���*/\
			User_UserGroup_ID bigint(20) not null,          /*�û���ID*/\
			User_Name Varchar(255) not null,                /*�û���*/\
			User_LoginName Varchar(255) not null,           /*��¼�û�����Ӣ��/����/�»��ߣ�*/\
			User_Password Varchar(255) not null,            /*�û���¼����*/\
			User_Remarks Varchar(255),                      /*��ע*/\
			PRIMARY KEY(User_ID),\
			FOREIGN KEY(User_UserGroup_ID)\
			REFERENCES CDC_UserGroup(UserGroup_ID)\
			ON DELETE CASCADE\
			) ENGINE = InnoDB DEFAULT CHARSET = utf8;";

		execDML(sql);

		_pCDC_UserObj = new CDC_User();

		TCDC_User User1;
		User1.User_ID = 1001;
		User1.User_Role = 1;
		User1.User_UserGroup_ID = 1001;
		User1.User_Name = "testJson1";
		User1.User_LoginName = "������";
		User1.User_Password = "testJson11";
		id = _pCDC_UserObj->User_Add(User1);
		assert(id > 0);

		TCDC_User User2;
		User2.User_ID = 1002;
		User2.User_Role = 1;
		User2.User_UserGroup_ID = 1002;
		User2.User_Name = "testJson2";
		User2.User_LoginName = "������";
		User2.User_Password = "testJson22";
		id = _pCDC_UserObj->User_Add(User2);
		assert(id > 0);
	}
	catch (CppMySQLException& e)
	{
		MERR << e.errorCode() << ":" << e.errorMessage();
	}

}

void CDC_VM_Tests::setUp()
{	
	try
	{
		CppMySQLDB::Instance().init();
		CppMySQLDB::Instance().setOptions(MYSQL_SET_CHARSET_NAME, "gbk");	
		CppMySQLDB::Instance().connect(HOST, USER, PASSWORD);
		CppMySQLDB::Instance().dropDB(gszDB);
		CppMySQLDB::Instance().createDB(gszDB);
		CppMySQLDB::Instance().open(gszDB);
		dependsFuc();
		dependsFuc1();
		dependsFuc2();
		dependsFuc3();
		dependsFuc4();

		string sql = "CREATE TABLE IF NOT EXISTS CDC_VM (\
			VM_ID bigint(20) not null AUTO_INCREMENT,       /*�����ID*/\
			VM_Template_ID bigint(20) not null,             /*ģ��ID*/\
			VM_Host_ID bigint(20) not null,                 /*����ID*/\
			VM_ThinClient_ID bigint(20),                    /*���ն�ID*/\
			VM_CloneMode INT not null,                      /*��¡ģʽ��0������/1�����ӣ�*/\
			VM_SysReadOnly INT not null,                    /*ϵͳ��ֻ�����0����/1���ǣ�*/\
			VM_DataSize INT,                                /*��������������λ��G��*/\
			VM_UserGroup_ID bigint(20) not null,            /*�û���ID*/\
			VM_User_ID bigint(20) not null,                 /*�û�ID*/\
			VM_State INT not null,                          /*�����״̬��0������/1�����ߣ�*/\
			VM_OS Varchar(255) not null,                    /*ϵͳ��Ϣ*/\
			VM_SysPath Varchar(255) not null,               /*ϵͳ������·��*/\
			VM_SysFilename Varchar(255) not null,           /*ϵͳ���ļ���*/\
			VM_DataPath Varchar(255),                       /*����������·��*/\
			VM_DataFilename Varchar(255),                   /*�������ļ���*/\
			VM_MAC Varchar(255) not null,                   /*�����Mac��ַ*/\
			VM_SpicePwd Varchar(255) not null,              /*Spice����*/\
			VM_SpicePort Varchar(255) not null,             /*Spice�˿�*/\
			VM_Uuid Varchar(255),                           /*������ڲ���ʶ*/\
			VM_Create_Time datetime,                        /*���������ʱ��*/\
			VM_Update_Time datetime,                        /*���������޸�ʱ��*/\
			PRIMARY KEY(VM_ID),\
			FOREIGN KEY(VM_Template_ID)\
			REFERENCES CDC_Template(Template_ID)\
			ON DELETE CASCADE,\
			FOREIGN KEY(VM_Host_ID)\
			REFERENCES CDC_Host(Host_ID)\
			ON DELETE CASCADE,\
			FOREIGN KEY(VM_ThinClient_ID)\
			REFERENCES CDC_ThinClient(ThinClient_ID),\
			FOREIGN KEY(VM_UserGroup_ID)\
			REFERENCES CDC_UserGroup(UserGroup_ID)\
			ON DELETE CASCADE,\
			FOREIGN KEY(VM_User_ID)\
			REFERENCES CDC_User(User_ID)\
			ON DELETE CASCADE\
			) ENGINE = InnoDB DEFAULT CHARSET = utf8; ";
		execDML(sql);
		_pObj = new CDC_VM();
	}
	catch (CppMySQLException& e)
	{
		cerr << e.errorCode() << ":" << e.errorMessage();
	}
}

void CDC_VM_Tests::tearDown()
{
	CppMySQLDB::Instance().dropDB(gszDB);
	CppMySQLDB::Instance().close();
}


#define ADD_ONE_OBJECT \
		TCDC_VM t1;\
		t1.VM_Template_ID = 1001;\
		t1.VM_Host_ID = 1001;\
		t1.VM_ThinClient_ID = 1001;\
		t1.VM_CloneMode = 1;\
		t1.VM_SysReadOnly = 1;\
		t1.VM_DataSize = 1024;\
		t1.VM_UserGroup_ID = 1001;\
		t1.VM_User_ID = 1001;\
		t1.VM_State = 1;\
		t1.VM_OS = "testJson_3";\
		t1.VM_SysPath = "testJson_4";\
		t1.VM_SysFilename = "testJson_5";\
		t1.VM_DataPath = "testJson_6";\
		t1.VM_DataFilename = "testJson_7";\
		t1.VM_MAC = "testJson_8";\
		t1.VM_SpicePwd = "testJson_8";\
		t1.VM_SpicePort = "testJson_83";\
		t1.VM_Uuid = "testJson_8";\
		t1.VM_Create_Time = "2017-12-31 12:56:30";\
		t1.VM_Update_Time = "2017-12-31 12:56:30";

void CDC_VM_Tests::testJsonAdd()
{
	// success
	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddNumberToObject(json, "VM_Template_ID", 1001);
	cJSON_AddNumberToObject(json, "VM_Host_ID", 1001);
	cJSON_AddNumberToObject(json, "VM_ThinClient_ID", 1001);
	cJSON_AddNumberToObject(json, "VM_CloneMode", 1);
	cJSON_AddNumberToObject(json, "VM_SysReadOnly", 1);
	cJSON_AddNumberToObject(json, "VM_DataSize", 1024);
	cJSON_AddNumberToObject(json, "VM_UserGroup_ID", 1001);
	cJSON_AddNumberToObject(json, "VM_User_ID", 1001);
	cJSON_AddStringToObject(json, "VM_Name", "testJsonAdd_1");
	cJSON_AddNumberToObject(json, "VM_State", 1);
	cJSON_AddStringToObject(json, "VM_OS", "testJson_3");
	cJSON_AddStringToObject(json, "VM_SysPath", "testJson_4");
	cJSON_AddStringToObject(json, "VM_SysFilename", "testJson_5");
	cJSON_AddStringToObject(json, "VM_DataPath", "testJson_6");
	cJSON_AddStringToObject(json, "VM_DataFilename", "testJson_7");
	cJSON_AddStringToObject(json, "VM_SpicePwd", "testJson_8");
	cJSON_AddStringToObject(json, "VM_SpicePort", "testJson_18");
	cJSON_AddStringToObject(json, "VM_MAC", "testJson_8");
	cJSON_AddStringToObject(json, "VM_Uuid", "testJsonAdd_8");
	/*cJSON_AddStringToObject(json, "VM_Create_Time", "2017-12-31 12:56:30");
	cJSON_AddStringToObject(json, "VM_Update_Time", "2017-12-31 12:56:30");*/
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_VM_Add(req);

	const string success = "{\n\t\"Result\":\t1,\n\t\"VM_ID\":\t1\n}";
	assert(success == result);

	// failed
	json = cJSON_CreateObject();
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);

	const string failed = FAILED_JSON_RESULT;
	result = _pObj->CDC_VM_Add(req);
	assert(failed == result);
}


void CDC_VM_Tests::testJsonDel()
{
	ADD_ONE_OBJECT;
	id = _pObj->VM_Add(t1);
	assert(id > 0);

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddNumberToObject(json, "VM_ID", id);
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_VM_Del(req);
	assert(SUCCESS_JSON_RESULT == result);
}

void CDC_VM_Tests::testJsonUpdate()
{
	ADD_ONE_OBJECT;
	id = _pObj->VM_Add(t1);
	assert(id > 0);

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddNumberToObject(json, "VM_ID", id);
	cJSON_AddNumberToObject(json, "VM_Host_ID", 1002);
	cJSON_AddStringToObject(json, "VM_SysPath", "XXXXXXX������");
	cJSON_AddStringToObject(json, "VM_Create_Time", "2019-12-31 12:56:30");
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_VM_Update(req);
	assert(SUCCESS_JSON_RESULT == result);
}

void CDC_VM_Tests::testJsonFind()
{
	ADD_ONE_OBJECT;
	id = _pObj->VM_Add(t1);
	assert(id > 0);


	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddNumberToObject(json, "VM_Host_ID", 1001);
	cJSON_AddStringToObject(json, "VM_SpicePwd", "testJson_8");
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_VM_Find(req);
	MDEBUG << result << endl;
	assert(result != FAILED_JSON_RESULT);

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "All");
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_VM_Find(req);
	MDEBUG << result << endl;
	assert(result != FAILED_JSON_RESULT);
}

void CDC_VM_Tests::testJsonFindCount()
{
	ADD_ONE_OBJECT;
	id = _pObj->VM_Add(t1);
	assert(id > 0);

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddNumberToObject(json, "VM_Host_ID", 1001);
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_VM_FindCount(req);
	MDEBUG << result << endl;
	assert(result != FAILED_JSON_RESULT);

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "All");
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pObj->CDC_VM_FindCount(req);
	MDEBUG << result << endl;
	assert(result != FAILED_JSON_RESULT);
}


CppUnit::Test* CDC_VM_Tests::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("CDC_VM_Tests");

	CppUnit_addTest(pSuite, CDC_VM_Tests, testJsonAdd);
	CppUnit_addTest(pSuite, CDC_VM_Tests, testJsonDel);
	CppUnit_addTest(pSuite, CDC_VM_Tests, testJsonUpdate);
	CppUnit_addTest(pSuite, CDC_VM_Tests, testJsonFind);
	CppUnit_addTest(pSuite, CDC_VM_Tests, testJsonFindCount);

	return pSuite;
}
