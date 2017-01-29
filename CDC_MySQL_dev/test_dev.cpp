#include "CppMySQL.h"
#include "CDC_MySQL.h"
#include "cJSON.h"
#include <iostream>
#include <assert.h>
using namespace std;

int main()
{
    CppMySQLDB* pDB;     // ����һ������
	const char* gszDB = "test_dev";
	#define Host		"127.0.0.1"
	#define USER		"root"
	#define PASSWORD	"123456"
	int ret = -1;
	double id = -1;

	cJSON *json = 0;
	char *out = 0;
	string req;
	string result;
	CDC_UserGroup* _pUserGroupObj;
	CDC_User* _pUserObj;

	////////////////////////////////////////
    // ���Ӵ������ݿ⣬�������ݿ��ַ�������ʾ������Ҫ����Ϊgbk
    pDB = new CppMySQLDB();
	pDB->setOptions(MYSQL_SET_CHARSET_NAME, "gbk");
	pDB->connect(Host, USER, PASSWORD);
	pDB->dropDB(gszDB);
	pDB->createDB(gszDB);
	pDB->open(gszDB);
	
	//�����û����
	string sql = "CREATE TABLE IF NOT EXISTS CDC_UserGroup (\
		UserGroup_ID bigint(20) not null AUTO_INCREMENT,\
		UserGroup_Name Varchar(255) not null,\
		PRIMARY KEY(UserGroup_ID)\
		) ENGINE = InnoDB; ";

	pDB->execDML(sql);
	_pUserGroupObj = new CDC_UserGroup(pDB);

	/// �����û���
	sql = "CREATE TABLE IF NOT EXISTS CDC_User (\
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
	_pUserObj = new CDC_User(pDB);

    //////////////////////////////////////
    //���뵽�û����
    json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddStringToObject(json, "UserGroup_Name", "11aa����");
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pUserGroupObj->CDC_UserGroup_Add(req); //���ò���ӿ�

	string success = "{\n\t\"Result\":\t1,\n\t\"UserGroup_ID\":\t1\n}";
	assert(success == result);
    

	//���뵽�û������û����й���
	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddNumberToObject(json, "User_Role", 1);
	cJSON_AddNumberToObject(json, "User_UserGroup_ID", 1); // ����
	cJSON_AddStringToObject(json, "User_Name", "testJsonAdd����");
	cJSON_AddStringToObject(json, "User_LoginName", "testJsonAdd_11");
	cJSON_AddStringToObject(json, "User_Password", "testJsonAdd_222");
	cJSON_AddStringToObject(json, "User_Remarks", "haha����");
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pUserObj->CDC_User_Add(req);  //���ò���ӿ�

	success = "{\n\t\"Result\":\t1,\n\t\"User_ID\":\t1\n}";
	assert(success == result);
	//
	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddNumberToObject(json, "User_ID", 1);
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pUserObj->CDC_User_Del(req); //����ɾ���ӿ�
#define SUCCESS_JSON_RESULT  "{\n\t\"Result\":\t1\n}"
	assert(SUCCESS_JSON_RESULT == result);

    ///////////////////////////////////////////////
	delete _pUserGroupObj;
	_pUserGroupObj = NULL;
	delete _pUserObj;
	_pUserObj = NULL;

    pDB->dropDB(gszDB);
	pDB->close();
	delete pDB;
	pDB = NULL;
    return 0;
}