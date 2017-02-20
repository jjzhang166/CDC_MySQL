#ifndef _data_def_H_
#define _data_def_H_

#include <string>

#define INVALID_NUM -1

/* �û����CDC_UserGroup */
//CREATE TABLE IF NOT EXISTS CDC_UserGroup(
//	UserGroup_ID bigint(20) not null AUTO_INCREMENT,        /*�û���ID*/
//	UserGroup_Name Varchar(255) not null,                   /*�û�����*/
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

/* �û���     ��CDC_User */
//CREATE TABLE IF NOT EXISTS CDC_User(
//	User_ID bigint(20) not null AUTO_INCREMENT,     /*�û�ID*/
//	User_Role INT not null,                         /*��ɫ��0������Ա:1���û���*/
//	User_UserGroup_ID bigint(20) not null,          /*�û���ID*/
//	User_Name Varchar(255) not null,                /*�û���*/
//	User_LoginName Varchar(255) not null,           /*��¼�û�����Ӣ��/����/�»��ߣ�*/
//	User_Password Varchar(255) not null,            /*�û���¼����*/
//	User_Remarks Varchar(255),                      /*��ע*/
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

	double User_ID; // �û�ID
	int User_Role;	// ��ɫ��0������Ա:1���û���
	double User_UserGroup_ID;	// �û���ID
	std::string User_Name;		// �û��������֣�
	std::string User_LoginName;	// ��¼�û�����Ӣ��/����/�»��ߣ�
	std::string User_Password;	// �û���¼����
	std::string User_Remarks;	// ��ע
};


/* ������CDC_Host */
//CREATE TABLE IF NOT EXISTS CDC_Host(
//	Host_ID bigint(20) not null AUTO_INCREMENT,     /*����ID*/
//	Host_Name Varchar(255) not null,                /*������*/
//	Host_IP Varchar(255) not null,                  /*����IP*/
//	Host_VirtNet Varchar(255) not null,             /*������������*/
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

/* ģ���CDC_Template */
//CREATE TABLE IF NOT EXISTS CDC_Template(
//	Template_ID bigint(20) not null AUTO_INCREMENT, /*ģ��ID*/
//	Template_HostID bigint(20) not null,            /*ģ����������ID*/
//	Template_CoreNum INT not null,                  /*CPU�ں���*/
//	Template_MemSize INT not null,                  /*�ڴ���������λ��G��*/
//	Template_SysSize INT not null,                  /*ϵͳ����������λ��G*/
//	Template_Version INT not null,                  /*�汾��*/
//	Template_Mode INT not null,                     /*ģʽ��0����ѧ/1���칫��*/
//	Template_State INT not null,                    /*ģ��״̬�����ߣ�1/���ߣ�0*/
//	Template_Name Varchar(255) not null,            /*ģ����*/
//	Template_OS Varchar(255) not null,              /*ϵͳ����*/
//	Template_Path Varchar(255) not null,            /*ģ������·��*/
//	Template_FileName Varchar(255) not null,        /*ģ���ļ���*/
//	Template_RollBackFile Varchar(255),             /*�ع�������*/
//	Template_BackUpFile Varchar(255) not null,      /*����ģ����*/
//	Template_SpicePwd Varchar(255) not null,        /*Spice����*/
//	Template_Uuid Varchar(255),                    /*ģ���ڲ���ʶ��*/
//	Template_Create_Time DateTime,                  /*ģ�崴��ʱ��*/
//	Template_Update_Time DateTime,                  /*ģ������޸�ʱ��*/
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

/* ISO��CDC_ISO */
//CREATE TABLE IF NOT EXISTS CDC_ISO(
//	ISO_ID bigint(20) not null AUTO_INCREMENT,      /*ISO��ID*/
//	ISO_HostID bigint(20) not null,                 /*ISO��������ID*/
//	ISO_Name Varchar(255) not null,                 /*ISO��*/
//	ISO_Path Varchar(255) not null,                 /*ISO�ļ�����·��*/
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

/* �������CDC_VM */
//CREATE TABLE IF NOT EXISTS CDC_VM(
//	VM_ID bigint(20) not null AUTO_INCREMENT,       /*�����ID*/
//	VM_Template_ID bigint(20) not null,             /*ģ��ID*/
//	VM_Host_ID bigint(20) not null,                 /*����ID*/
//	VM_ThinClient_ID bigint(20),                    /*���ն�ID*/
//	VM_CloneMode INT not null,                      /*��¡ģʽ��0������/1�����ӣ�*/
//	VM_SysReadOnly INT not null,                    /*ϵͳ��ֻ�����0����/1���ǣ�*/
//	VM_DataSize INT,                                /*��������������λ��G��*/
//	VM_UserGroup_ID bigint(20) not null,            /*�û���ID*/
//	VM_User_ID bigint(20) not null,                 /*�û�ID*/
//	VM_State INT not null,                          /*�����״̬��0������/1�����ߣ�*/
//	VM_OS Varchar(255) not null,                    /*ϵͳ��Ϣ*/
//	VM_SysPath Varchar(255) not null,               /*ϵͳ������·��*/
//	VM_SysFilename Varchar(255) not null,           /*ϵͳ���ļ���*/
//	VM_DataPath Varchar(255),                       /*����������·��*/
//	VM_DataFilename Varchar(255),                   /*�������ļ���*/
//	VM_MAC Varchar(255) not null,                   /*�����Mac��ַ*/
//	VM_SpicePwd Varchar(255) not null,              /*Spice����*/
//	VM_Uuid Varchar(255),                           /*������ڲ���ʶ*/
//	VM_Create_Time datetime,                        /*���������ʱ��*/
//	VM_Update_Time datetime,                        /*���������޸�ʱ��*/
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

/* ���ն����CDC_ThinClientGroup */
//CREATE TABLE IF NOT EXISTS CDC_ThinClientGroup(
//	ThinClientGroup_ID bigint(20) not null AUTO_INCREMENT,  /*���ն�ID*/
//	ThinClientGroup_Name Varchar(255) not null,             /*���ն���ID*/
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

/* ���ն˱� CDC_ThinClient */
//CREATE TABLE IF NOT EXISTS CDC_ThinClient(
//	ThinClient_ID bigint(20) not null AUTO_INCREMENT,       /*���ն�ID*/
//	ThinClient_ThinGroup_ID bigint(20) not null,            /*���ն���ID*/
//	ThinClient_Mode INT not null,                           /*ģʽ��0����ѧ/1���칫��*/
//	ThinClient_Version Varchar(255) not null,               /*�汾 */
//	ThinClient_State INT not null,                          /*״̬��0������/1�����ߣ�*/
//	ThinClient_Protocol INT not null,                       /*Э�飨0��VOI/1��VDI��*/
//	ThinClient_Name Varchar(255) not null,                  /*���ն���*/
//	ThinClient_IP Varchar(255) not null,                    /*IP��ַ*/
//	ThinClient_MAC Varchar(255) not null,                   /*MAC��ַ*/
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

/* ��ѧ�������CDC_EDU_VM */
//CREATE TABLE IF NOT EXISTS CDC_EDU_VM(
//	EDU_VM_ID bigint(20) not null AUTO_INCREMENT,   /*�����ID*/
//	EDU_VM_Template_ID bigint(20) not null,         /*ģ��ID*/
//	EDU_VM_ThinClient_ID bigint(20) not null,       /*���ն�ID*/
//	EDU_VM_State INT not null,                      /*�����״̬��0������/1�����ߣ�*/
//	EDU_VM_SysPath Varchar(255) not null,           /*ϵͳ������·��*/
//	EDU_VM_SysFilename Varchar(255) not null,       /*ϵͳ���ļ���*/
//	EDU_VM_MAC Varchar(255) not null,               /*�����Mac��ַ*/
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

/* ��Ȩ��CDC_Authorization */
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