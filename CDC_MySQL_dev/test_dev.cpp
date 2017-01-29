#include "CppMySQL.h"
#include "CDC_MySQL.h"
#include "cJSON.h"
#include <iostream>
#include <assert.h>
using namespace std;

int main()
{
    CppMySQLDB* pDB;     // 必须一个对象
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
    // 连接创建数据库，设置数据库字符集，显示汉字需要设置为utf8
    pDB = new CppMySQLDB();
	pDB->setOptions(MYSQL_SET_CHARSET_NAME, "utf8");
	pDB->connect(Host, USER, PASSWORD);
	pDB->dropDB(gszDB);
	pDB->createDB(gszDB);
	pDB->open(gszDB);
	
	//创建用户组表
	string sql = "CREATE TABLE IF NOT EXISTS CDC_UserGroup (\
		UserGroup_ID bigint(20) not null AUTO_INCREMENT,\
		UserGroup_Name Varchar(255) not null,\
		PRIMARY KEY(UserGroup_ID)\
		) ENGINE = InnoDB; ";

	pDB->execDML(sql);
	_pUserGroupObj = new CDC_UserGroup(pDB);

	/// 创建用户表
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
    //插入到用户组表
    json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddStringToObject(json, "UserGroup_Name", "11aa哈哈");
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pUserGroupObj->CDC_UserGroup_Add(req); //调用插入接口

	string success = "{\n\t\"Result\":\t1,\n\t\"UserGroup_ID\":\t1\n}";
	assert(success == result);
    

	//插入到用户表，和用户组有关联
	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddNumberToObject(json, "User_Role", 1);
	cJSON_AddNumberToObject(json, "User_UserGroup_ID", 1); // 关联
	cJSON_AddStringToObject(json, "User_Name", "testJsonAdd哈哈");
	cJSON_AddStringToObject(json, "User_LoginName", "testJsonAdd_11");
	cJSON_AddStringToObject(json, "User_Password", "testJsonAdd_222");
	cJSON_AddStringToObject(json, "User_Remarks", "haha哈哈");
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pUserObj->CDC_User_Add(req);  //调用插入接口

	success = "{\n\t\"Result\":\t1,\n\t\"User_ID\":\t1\n}";
	assert(success == result);
	//
	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Method", "Part");
	cJSON_AddNumberToObject(json, "User_ID", 1);
	out = cJSON_Print(json);
	req = out;
	cJSON_Delete(json);
	result = _pUserObj->CDC_User_Del(req); //调用删除接口
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