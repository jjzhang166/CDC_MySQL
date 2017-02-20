#ifndef _data_def_H_
#define _data_def_H_

#include <string>

#define INVALID_NUM -1

/* 用户组表：CDC_UserGroup */
//CREATE TABLE IF NOT EXISTS CDC_UserGroup(
//	UserGroup_ID bigint(20) not null AUTO_INCREMENT,        /*用户组ID*/
//	UserGroup_Name Varchar(255) not null,                   /*用户组名*/
//	PRIMARY KEY(UserGroup_ID)
//	) ENGINE = InnoDB DEFAULT CHARSET = utf8;

struct TCDC_UserGroup
{
	TCDC_UserGroup()
		:UserGroup_ID(INVALID_NUM), UserGroup_Name("")
	{}
	double UserGroup_ID;
	std::string UserGroup_Name;
};

/* 用户表     ：CDC_User */
//CREATE TABLE IF NOT EXISTS CDC_User(
//	User_ID bigint(20) not null AUTO_INCREMENT,     /*用户ID*/
//	User_Role INT not null,                         /*角色（0：管理员:1：用户）*/
//	User_UserGroup_ID bigint(20) not null,          /*用户组ID*/
//	User_Name Varchar(255) not null,                /*用户名*/
//	User_LoginName Varchar(255) not null,           /*登录用户名（英文/数字/下划线）*/
//	User_Password Varchar(255) not null,            /*用户登录密码*/
//	User_Remarks Varchar(255),                      /*备注*/
//	PRIMARY KEY(User_ID),
//	FOREIGN KEY(User_UserGroup_ID)
//	REFERENCES CDC_UserGroup(UserGroup_ID)
//	ON DELETE CASCADE
//	) ENGINE = InnoDB DEFAULT CHARSET = utf8;

struct TCDC_User
{
	TCDC_User()
		:User_ID(INVALID_NUM) 
		,User_Role(1)
		, User_UserGroup_ID(-1)
	{}

	double User_ID; // 用户ID
	int User_Role;	// 角色（0：管理员:1：用户）
	double User_UserGroup_ID;	// 用户组ID
	std::string User_Name;		// 用户名（汉字）
	std::string User_LoginName;	// 登录用户名（英文/数字/下划线）
	std::string User_Password;	// 用户登录密码
	std::string User_Remarks;	// 备注
};


/* 主机表：CDC_Host */
//CREATE TABLE IF NOT EXISTS CDC_Host(
//	Host_ID bigint(20) not null AUTO_INCREMENT,     /*主机ID*/
//	Host_Name Varchar(255) not null,                /*主机名*/
//	Host_IP Varchar(255) not null,                  /*主机IP*/
//	Host_VirtNet Varchar(255) not null,             /*主机虚拟网络*/
//	PRIMARY KEY(Host_ID)
//	) ENGINE = InnoDB DEFAULT CHARSET = utf8;

struct TCDC_Host
{
	TCDC_Host()
	:Host_ID(INVALID_NUM)
	{}
	double Host_ID;
	std::string Host_Name;
	std::string Host_IP;
	std::string Host_VirtNet;
};

/* 模板表：CDC_Template */
//CREATE TABLE IF NOT EXISTS CDC_Template(
//	Template_ID bigint(20) not null AUTO_INCREMENT, /*模板ID*/
//	Template_HostID bigint(20) not null,            /*模板所在主机ID*/
//	Template_CoreNum INT not null,                  /*CPU内核数*/
//	Template_MemSize INT not null,                  /*内存容量（单位：G）*/
//	Template_SysSize INT not null,                  /*系统盘容量（单位：G*/
//	Template_Version INT not null,                  /*版本号*/
//	Template_Mode INT not null,                     /*模式（0：教学/1：办公）*/
//	Template_State INT not null,                    /*模板状态（在线：1/离线：0*/
//	Template_Name Varchar(255) not null,            /*模板名*/
//	Template_OS Varchar(255) not null,              /*系统类型*/
//	Template_Path Varchar(255) not null,            /*模板所在路径*/
//	Template_FileName Varchar(255) not null,        /*模板文件名*/
//	Template_RollBackFile Varchar(255),             /*回滚快照名*/
//	Template_BackUpFile Varchar(255) not null,      /*备份模板名*/
//	Template_SpicePwd Varchar(255) not null,        /*Spice密码*/
//	Template_Uuid Varchar(255),                    /*模板内部标识名*/
//	Template_Create_Time DateTime,                  /*模板创建时间*/
//	Template_Update_Time DateTime,                  /*模板最后修改时间*/
//	PRIMARY KEY(Template_ID),
//	FOREIGN KEY(Template_HostID)
//	REFERENCES CDC_Host(Host_ID)
//	ON DELETE CASCADE
//	) ENGINE = InnoDB DEFAULT CHARSET = utf8;

struct TCDC_Template
{
	TCDC_Template()
		:Template_ID(INVALID_NUM), Template_HostID(INVALID_NUM), 
		Template_CoreNum(INVALID_NUM), Template_MemSize(INVALID_NUM),
		Template_SysSize(INVALID_NUM), Template_Version(INVALID_NUM),
		Template_Mode(INVALID_NUM), Template_State(INVALID_NUM)
	{}

	double Template_ID;
	double Template_HostID;
	int Template_CoreNum;
	int Template_MemSize;
	int Template_SysSize;
	int Template_Version;
	int Template_Mode;
	int Template_State;
	std::string	Template_Name;
	std::string	Template_OS;
	std::string Template_Path;
	std::string	Template_FileName;
	std::string	Template_RollBackFile;
	std::string	Template_BackUpFile;
	std::string	Template_MAC;
	std::string	Template_SpicePwd;
	std::string	Template_SpicePort;
	std::string	Template_Uuid;
	std::string Template_Create_Time;	// DateTime type
	std::string Template_Update_Time;	// DateTime type
};

/* ISO表：CDC_ISO */
//CREATE TABLE IF NOT EXISTS CDC_ISO(
//	ISO_ID bigint(20) not null AUTO_INCREMENT,      /*ISO的ID*/
//	ISO_HostID bigint(20) not null,                 /*ISO所在主机ID*/
//	ISO_Name Varchar(255) not null,                 /*ISO名*/
//	ISO_Path Varchar(255) not null,                 /*ISO文件所在路径*/
//	PRIMARY KEY(ISO_ID),
//	FOREIGN KEY(ISO_HostID)
//	REFERENCES CDC_Host(Host_ID)
//	ON DELETE CASCADE
//	) ENGINE = InnoDB DEFAULT CHARSET = utf8;

struct TCDC_ISO
{
	TCDC_ISO()
		:ISO_ID(INVALID_NUM), ISO_HostID(INVALID_NUM)
	{}
	double ISO_ID;
	double ISO_HostID;
	std::string	ISO_Name;
	std::string	ISO_Path;
};

/* 虚拟机表：CDC_VM */
//CREATE TABLE IF NOT EXISTS CDC_VM(
//	VM_ID bigint(20) not null AUTO_INCREMENT,       /*虚拟机ID*/
//	VM_Template_ID bigint(20) not null,             /*模板ID*/
//	VM_Host_ID bigint(20) not null,                 /*主机ID*/
//	VM_ThinClient_ID bigint(20),                    /*瘦终端ID*/
//	VM_CloneMode INT not null,                      /*克隆模式（0：完整/1：链接）*/
//	VM_SysReadOnly INT not null,                    /*系统盘只读与否（0：否/1：是）*/
//	VM_DataSize INT,                                /*数据盘容量（单位：G）*/
//	VM_UserGroup_ID bigint(20) not null,            /*用户组ID*/
//	VM_User_ID bigint(20) not null,                 /*用户ID*/
//	VM_State INT not null,                          /*虚拟机状态（0：离线/1：在线）*/
//	VM_OS Varchar(255) not null,                    /*系统信息*/
//	VM_SysPath Varchar(255) not null,               /*系统盘所在路径*/
//	VM_SysFilename Varchar(255) not null,           /*系统盘文件名*/
//	VM_DataPath Varchar(255),                       /*数据盘所在路径*/
//	VM_DataFilename Varchar(255),                   /*数据盘文件名*/
//	VM_MAC Varchar(255) not null,                   /*虚拟机Mac地址*/
//	VM_SpicePwd Varchar(255) not null,              /*Spice密码*/
//	VM_Uuid Varchar(255),                           /*虚拟机内部标识*/
//	VM_Create_Time datetime,                        /*虚拟机创建时间*/
//	VM_Update_Time datetime,                        /*虚拟机最后修改时间*/
//	PRIMARY KEY(VM_ID),
//	FOREIGN KEY(VM_Template_ID)
//	REFERENCES CDC_Template(Template_ID)
//	ON DELETE CASCADE,
//	FOREIGN KEY(VM_Host_ID)
//	REFERENCES CDC_Host(Host_ID)
//	ON DELETE CASCADE,
//	FOREIGN KEY(VM_ThinClient_ID)
//	REFERENCES CDC_ThinClient(ThinClient_ID),
//	FOREIGN KEY(VM_UserGroup_ID)
//	REFERENCES CDC_UserGroup(UserGroup_ID)
//	ON DELETE CASCADE,
//	FOREIGN KEY(VM_User_ID)
//	REFERENCES CDC_User(User_ID)
//	ON DELETE CASCADE
//	) ENGINE = InnoDB DEFAULT CHARSET = utf8;
struct TCDC_VM
{
	TCDC_VM()
		:VM_ID(INVALID_NUM), VM_Template_ID(INVALID_NUM),
		VM_Host_ID(INVALID_NUM), VM_ThinClient_ID(INVALID_NUM),
		VM_CloneMode(INVALID_NUM), VM_SysReadOnly(INVALID_NUM),
		VM_DataSize(INVALID_NUM), VM_UserGroup_ID(INVALID_NUM),
		VM_User_ID(INVALID_NUM), VM_State(INVALID_NUM)
	{}

	double VM_ID;
	double VM_Template_ID;
	double VM_Host_ID;
	double VM_ThinClient_ID;
	int VM_CloneMode;
	int VM_SysReadOnly;
	int	VM_DataSize;
	double	VM_UserGroup_ID;
	double	VM_User_ID;
	int	VM_State;
	std::string	VM_OS;
	std::string	VM_SysPath;
	std::string	VM_SysFilename;
	std::string	VM_DataPath;
	std::string	VM_DataFilename;
	std::string	VM_MAC;
	std::string	VM_SpicePwd;
	std::string	VM_SpicePort;
	std::string	VM_Uuid;
	std::string	VM_Create_Time;
	std::string	VM_Update_Time;
};

/* 瘦终端组表：CDC_ThinClientGroup */
//CREATE TABLE IF NOT EXISTS CDC_ThinClientGroup(
//	ThinClientGroup_ID bigint(20) not null AUTO_INCREMENT,  /*瘦终端ID*/
//	ThinClientGroup_Name Varchar(255) not null,             /*瘦终端组ID*/
//	PRIMARY KEY(ThinClientGroup_ID)
//	) ENGINE = InnoDB DEFAULT CHARSET = utf8;
struct TCDC_ThinClientGroup
{
	TCDC_ThinClientGroup()
		:ThinClientGroup_ID(INVALID_NUM)
	{}
	double ThinClientGroup_ID;
	std::string	ThinClientGroup_Name;
};

/* 瘦终端表： CDC_ThinClient */
//CREATE TABLE IF NOT EXISTS CDC_ThinClient(
//	ThinClient_ID bigint(20) not null AUTO_INCREMENT,       /*瘦终端ID*/
//	ThinClient_ThinGroup_ID bigint(20) not null,            /*瘦终端组ID*/
//	ThinClient_Mode INT not null,                           /*模式（0：教学/1：办公）*/
//	ThinClient_Version Varchar(255) not null,               /*版本 */
//	ThinClient_State INT not null,                          /*状态（0：离线/1：在线）*/
//	ThinClient_Protocol INT not null,                       /*协议（0：VOI/1：VDI）*/
//	ThinClient_Name Varchar(255) not null,                  /*瘦终端名*/
//	ThinClient_IP Varchar(255) not null,                    /*IP地址*/
//	ThinClient_MAC Varchar(255) not null,                   /*MAC地址*/
//	PRIMARY KEY(ThinClient_ID),
//	FOREIGN KEY(ThinClient_ThinGroup_ID)
//	REFERENCES CDC_ThinClientGroup(ThinClientGroup_ID)
//	ON DELETE CASCADE
//	) ENGINE = InnoDB DEFAULT CHARSET = utf8;

struct TCDC_ThinClient
{
	TCDC_ThinClient()
		:ThinClient_ID(INVALID_NUM), ThinClient_ThinGroup_ID(INVALID_NUM),
		ThinClient_Mode(INVALID_NUM), ThinClient_State(INVALID_NUM),
		ThinClient_Protocol(INVALID_NUM)
	{}
	double ThinClient_ID;
	double ThinClient_ThinGroup_ID;
	int ThinClient_Mode;
	std::string	ThinClient_Version;
	int	ThinClient_State;
	int	ThinClient_Protocol;
	std::string	ThinClient_Name;
	std::string	ThinClient_IP;
	std::string	ThinClient_MAC;
};

/* 教学虚拟机表：CDC_EDU_VM */
//CREATE TABLE IF NOT EXISTS CDC_EDU_VM(
//	EDU_VM_ID bigint(20) not null AUTO_INCREMENT,   /*虚拟机ID*/
//	EDU_VM_Template_ID bigint(20) not null,         /*模板ID*/
//	EDU_VM_ThinClient_ID bigint(20) not null,       /*瘦终端ID*/
//	EDU_VM_State INT not null,                      /*虚拟机状态（0：离线/1：在线）*/
//	EDU_VM_SysPath Varchar(255) not null,           /*系统盘所在路径*/
//	EDU_VM_SysFilename Varchar(255) not null,       /*系统盘文件名*/
//	EDU_VM_MAC Varchar(255) not null,               /*虚拟机Mac地址*/
//	PRIMARY KEY(EDU_VM_ID),
//	FOREIGN KEY(EDU_VM_Template_ID)
//	REFERENCES CDC_Template(Template_ID)
//	ON DELETE CASCADE,
//	FOREIGN KEY(EDU_VM_ThinClient_ID)
//	REFERENCES CDC_ThinClient(ThinClient_ID)
//	ON DELETE CASCADE
//	) ENGINE = InnoDB DEFAULT CHARSET = utf8;
struct TCDC_EDU_VM
{
	TCDC_EDU_VM()
		:EDU_VM_ID(INVALID_NUM), EDU_VM_Template_ID(INVALID_NUM),
		EDU_VM_ThinClient_ID(INVALID_NUM), EDU_VM_State(INVALID_NUM)
	{}
	double EDU_VM_ID;
	double EDU_VM_Template_ID;
	double EDU_VM_ThinClient_ID;
	int	EDU_VM_State;
	std::string	EDU_VM_SysPath;
	std::string	EDU_VM_SysFilename;
	std::string EDU_VM_MAC;
};

/* 授权表：CDC_Authorization */
/*
CREATE TABLE CDC_Authorization(
	Authorization_MaxClientNum INT not null,
	Authorization_Deadline DateTime not null,
	Authorization_MachineID Varchar(255) not null,
	Authorization_Company Varchar(255) not null,
	primary key(Authorization_MachineID)
	) ENGINE = InnoDB;
*/
struct TCDC_Authorization
{
	TCDC_Authorization()
		:Authorization_MaxClientNum(INVALID_NUM)
	{}
	std::string	Authorization_MachineID;
	double Authorization_MaxClientNum;
	std::string	Authorization_Deadline; // DateTime
	std::string	Authorization_Company;
};
#endif