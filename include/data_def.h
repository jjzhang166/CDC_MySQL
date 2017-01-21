#ifndef _data_def_H_
#define _data_def_H_

#include <string>

#define INVALID_ID -1

/* 用户组表：CDC_UserGroup */
/*
CREATE TABLE IF NOT EXISTS CDC_UserGroup (
UserGroup_ID bigint(20) not null auto_increment(1,1), 
UserGroup_Name Varchar(255) not null,
PRIMARY KEY (UserGroup_ID) AUTO_INCREMENT 1
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
*/

struct TCDC_UserGroup
{
	TCDC_UserGroup()
		:UserGroup_ID(INVALID_ID), UserGroup_Name("")
	{}
	double UserGroup_ID;
	std::string UserGroup_Name;
};

/* 用户表     ：CDC_User */
/*
CREATE TABLE IF NOT EXISTS CDC_User (
User_ID bigint(20) not null DEFAULT 1,
User_Role INT not null,
User_UserGroup_ID bigint(20) not null,
User_Name Varchar(255) not null,
User_LoginName Varchar(255) not null,
User_Password Varchar(255) not null,
User_Remarks Varchar(255),
PRIMARY KEY (User_ID),
FOREIGN KEY (User_UserGroup_ID)
REFERENCES CDC_UserGroup(UserGroup_ID)
ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
*/

struct TCDC_User
{
	TCDC_User()
		:User_ID(INVALID_ID) 
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
/*
CREATE TABLE IF NOT EXISTS CDC_Host (
Host_ID bigint(20) not null DEFAULT 1,
Host_Name Varchar(255) not null,
Host_IP Varchar(255) not null,
Host_VirtNet Varchar(255) not null,
PRIMARY KEY (Host_ID)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
*/
struct TCDC_Host
{
TCDC_Host()
	:Host_ID(INVALID_ID)
	{}
	double Host_ID;
	std::string Host_Name;
	std::string Host_IP;
	std::string Host_VirtNet;
};

/* 模板表：CDC_Template */
/*
CREATE TABLE IF NOT EXISTS CDC_Template (
Template_ID bigint(20) not null DEFAULT 1,
Template_HostID bigint(20) not null,
Template_CoreNum INT not null,
Template_MemSize INT not null,
Template_SysSize INT not null,
Template_Version Varchar(255) not null,
Template_Mode INT not null,
Template_State INT not null,
Template_Name Varchar(255) not null,
Template_OS Varchar(255) not null,
Template_Path Varchar(255) not null,
Template_FileName Varchar(255),
Template_RollBackFile Varchar(255),
Template_BackUpFile Varchar(255),
PRIMARY KEY (Template_ID),
FOREIGN KEY (Template_HostID)
REFERENCES CDC_Host(Host_ID)
ON DELETE CASCADE
) ENGINE=InnoDB;
*/
struct TCDC_Template
{
	double Template_ID;
	double Template_HostID;
	int Template_CoreNum;
	int Template_MemSize;
	int Template_SysSize;
	std::string Template_Version;
	int Template_Mode;
	int Template_State;
	std::string	Template_Name;
	std::string	Template_OS;
	std::string Template_Path;
	std::string	Template_FileName;
	std::string	Template_RollBackFile;
	std::string	Template_BackUpFile;
};
/* ISO表：CDC_ISO */
/*
CREATE TABLE IF NOT EXISTS CDC_ISO (
ISO_ID bigint(20) not null DEFAULT 1,
ISO_HostID bigint(20) not null,
ISO_Name Varchar(255) not null,
ISO_Path Varchar(255) not null,
PRIMARY KEY (ISO_ID)
) ENGINE=InnoDB;
*/
struct CDC_ISO
{
	double ISO_ID;
	double ISO_HostID;
	std::string	ISO_Name;
	std::string	ISO_Path;
};

/* 虚拟机表：CDC_VM */
/*
CREATE TABLE IF NOT EXISTS CDC_VM (
VM_ID bigint(20) not null DEFAULT 1,
VM_Template_ID bigint(20) not null,
VM_Host_ID bigint(20) not null,
VM_ThinClient_ID bigint(20) not null,
VM_CloneMode INT not null,
VM_SysReadOnly INT not null,
VM_DataSize INT not null,
VM_UserGroup_ID bigint(20) not null,
VM_User_ID bigint(20) not null,
VM_State INT not null,
VM_OS Varchar(255) not null,
VM_SysPath Varchar(255) not null,
VM_SysFilename Varchar(255) not null,
VM_DataPath Varchar(255) not null,
VM_DataFilename Varchar(255) not null,
PRIMARY KEY (VM_ID),
FOREIGN KEY (VM_Template_ID)
REFERENCES CDC_Template(Template_ID)
ON DELETE CASCADE,
FOREIGN KEY (VM_Host_ID)
REFERENCES CDC_Host(Host_ID)
ON DELETE CASCADE,
FOREIGN KEY (VM_ThinClient_ID)
REFERENCES CDC_ThinClient(ThinClient_ID),
FOREIGN KEY (VM_UserGroup_ID)
REFERENCES CDC_UserGroup(UserGroup_ID)
ON DELETE CASCADE,
FOREIGN KEY (VM_User_ID)
REFERENCES CDC_User(User_ID)
ON DELETE CASCADE
) ENGINE=InnoDB;
*/
struct TCDC_VM
{
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
};

/* 瘦终端组表：CDC_ThinClientGroup */
/*
CREATE TABLE IF NOT EXISTS CDC_ThinClientGroup (
ThinClientGroup_ID bigint(20) not null DEFAULT 1,
ThinClientGroup_Name Varchar(255) not null,
PRIMARY KEY (ThinClientGroup_ID)
) ENGINE=InnoDB;
*/
struct CDC_ThinClientGroup
{
	double ThinClientGroup_ID;
	std::string	ThinClientGroup_Name;
};

/* 瘦终端表： CDC_ThinClient */
/*
CREATE TABLE IF NOT EXISTS CDC_ThinClient (
ThinClient_ID bigint(20) not null DEFAULT 1,
ThinClient_ThinGroup_ID bigint(20) not null,
ThinClient_Mode INT not null,
ThinClient_Version Varchar(255) not null,
ThinClient_State INT not null,
ThinClient_Protocol INT,
ThinClient_Name Varchar(255),
ThinClient_IP Varchar(255),
ThinClient_MAC Varchar(255),
PRIMARY KEY (ThinClient_ID),
FOREIGN KEY (ThinClient_ThinGroup_ID)
REFERENCES CDC_ThinClientGroup(ThinClientGroup_ID)
ON DELETE CASCADE
) ENGINE=InnoDB;
*/
struct TCDC_ThinClient
{
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
/*
CREATE TABLE IF NOT EXISTS CDC_EDU_VM (
EDU_VM_ID bigint(20) not null DEFAULT 1,
EDU_VM_Template_ID bigint(20) not null,
EDU_VM_ThinClient_ID bigint(20) not null,
EDU_VM_State INT not null,
EDU_VM_SysPath Varchar(255) not null,
EDU_VM_SysFilename Varchar(255) not null,
PRIMARY KEY (EDU_VM_ID),
FOREIGN KEY (EDU_VM_Template_ID)
REFERENCES CDC_Template(Template_ID)
ON DELETE CASCADE,
FOREIGN KEY (EDU_VM_ThinClient_ID)
REFERENCES CDC_ThinClient(ThinClient_ID)
ON DELETE CASCADE
) ENGINE=InnoDB;
*/
struct TCDC_EDU_VM
{
	double EDU_VM_ID;
	double EDU_VM_Template_ID;
	double EDU_VM_ThinClient_ID;
	int	EDU_VM_State;
	std::string	EDU_VM_SysPath;
	std::string	EDU_VM_SysFilename;
};

/* 授权表：CDC_Authorization */
/*
CREATE TABLE CDC_Authorization(
	Authorization_MaxClientNum INT not null,
	Authorization_Deadline DATE not null,
	Authorization_MachineID Varchar(255) not null,
	Authorization_Company Varchar(255) not null,
	primary key(Authorization_MachineID)
	) ENGINE = InnoDB;
*/
struct TCDC_Authorization
{
	double Authorization_MaxClientNum;
	std::string	Authorization_Deadline; // XXX
	std::string	Authorization_MachineID;
	std::string	Authorization_Company;
};
#endif