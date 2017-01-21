/* 用户组表：CDC_UserGroup */
CREATE TABLE IF NOT EXISTS CDC_UserGroup (
UserGroup_ID bigint(20) not null AUTO_INCREMENT,        /*用户组ID*/
UserGroup_Name Varchar(255) not null,                   /*用户组名*/ 
PRIMARY KEY (UserGroup_ID)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/* 用户表     ：CDC_User */
CREATE TABLE IF NOT EXISTS CDC_User (
User_ID bigint(20) not null AUTO_INCREMENT,     /*用户ID*/ 
User_Role INT not null,                         /*角色（0：管理员:1：用户）*/ 
User_UserGroup_ID bigint(20) not null,          /*用户组ID*/ 
User_Name Varchar(255) not null,                /*用户名*/ 
User_LoginName Varchar(255) not null,           /*登录用户名（英文/数字/下划线）*/ 
User_Password Varchar(255) not null,            /*用户登录密码*/ 
User_Remarks Varchar(255),                      /*备注*/ 
PRIMARY KEY (User_ID),
FOREIGN KEY (User_UserGroup_ID)
        REFERENCES CDC_UserGroup(UserGroup_ID)
        ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/* 主机表：CDC_Host */
CREATE TABLE IF NOT EXISTS CDC_Host (
Host_ID bigint(20) not null AUTO_INCREMENT,     /*主机ID*/ 
Host_Name Varchar(255) not null,                /*主机名*/ 
Host_IP Varchar(255) not null,                  /*主机IP*/ 
Host_VirtNet Varchar(255) not null,             /*主机虚拟网络*/ 
PRIMARY KEY (Host_ID)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/* 模板表：CDC_Template */
CREATE TABLE IF NOT EXISTS CDC_Template (
Template_ID bigint(20) not null AUTO_INCREMENT, /*模板ID*/ 
Template_HostID bigint(20) not null,            /*模板所在主机ID*/ 
Template_CoreNum INT not null,                  /*CPU内核数*/ 
Template_MemSize INT not null,                  /*内存容量（单位：G）*/ 
Template_SysSize INT not null,                  /*系统盘容量（单位：G*/ 
Template_Version Varchar(255) not null,         /*版本号*/ 
Template_Mode INT not null,                     /*模式（0：教学/1：办公）*/ 
Template_State INT not null,                    /*模板状态（在线：1/离线：0*/ 
Template_Name Varchar(255) not null,            /*模板名*/ 
Template_OS Varchar(255) not null,              /*系统类型*/ 
Template_Path Varchar(255) not null,            /*模板所在路径*/ 
Template_FileName Varchar(255),                 /*模板文件名*/ 
Template_RollBackFile Varchar(255),             /*回滚快照名*/ 
Template_BackUpFile Varchar(255),               /*备份模板名*/ 
PRIMARY KEY (Template_ID),
FOREIGN KEY (Template_HostID)
        REFERENCES CDC_Host(Host_ID)
        ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/* ISO表：CDC_ISO */
CREATE TABLE IF NOT EXISTS CDC_ISO (
ISO_ID bigint(20) not null AUTO_INCREMENT,      /*ISO的ID*/ 
ISO_HostID bigint(20) not null,                 /*ISO所在主机ID*/ 
ISO_Name Varchar(255) not null,                 /*ISO名*/ 
ISO_Path Varchar(255) not null,                 /*ISO文件所在路径*/ 
PRIMARY KEY (ISO_ID)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/* 瘦终端组表：CDC_ThinClientGroup */
CREATE TABLE IF NOT EXISTS CDC_ThinClientGroup (
ThinClientGroup_ID bigint(20) not null AUTO_INCREMENT,  /*瘦终端ID*/ 
ThinClientGroup_Name Varchar(255) not null,             /*瘦终端组ID*/ 
PRIMARY KEY (ThinClientGroup_ID)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/* 瘦终端表： CDC_ThinClient */
CREATE TABLE IF NOT EXISTS CDC_ThinClient (
ThinClient_ID bigint(20) not null AUTO_INCREMENT,       /*瘦终端ID*/ 
ThinClient_ThinGroup_ID bigint(20) not null,            /*瘦终端组ID*/ 
ThinClient_Mode INT not null,                           /*模式（0：教学/1：办公）*/ 
ThinClient_Version Varchar(255) not null,               /*版本 */ 
ThinClient_State INT not null,                          /*状态（0：离线/1：在线）*/ 
ThinClient_Protocol INT,                                /*协议（0：VOI/1：VDI）*/ 
ThinClient_Name Varchar(255),                           /*瘦终端名*/ 
ThinClient_IP Varchar(255),                             /*IP地址*/ 
ThinClient_MAC Varchar(255),                            /*MAC地址*/ 
PRIMARY KEY (ThinClient_ID),
FOREIGN KEY (ThinClient_ThinGroup_ID)
        REFERENCES CDC_ThinClientGroup(ThinClientGroup_ID)
        ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/* 虚拟机表：CDC_VM */
CREATE TABLE IF NOT EXISTS CDC_VM (
VM_ID bigint(20) not null AUTO_INCREMENT,       /*虚拟机ID*/ 
VM_Template_ID bigint(20) not null,             /*模板ID*/ 
VM_Host_ID bigint(20) not null,                 /*主机ID*/ 
VM_ThinClient_ID bigint(20) not null,           /*瘦终端ID*/ 
VM_CloneMode INT not null,                      /*克隆模式（0：完整/1：链接）*/ 
VM_SysReadOnly INT not null,                    /*系统盘只读与否（0：否/1：是）*/ 
VM_DataSize INT not null,                       /*数据盘容量（单位：G）*/ 
VM_UserGroup_ID bigint(20) not null,            /*用户组ID*/ 
VM_User_ID bigint(20) not null,                 /*用户ID*/ 
VM_State INT not null,                          /*虚拟机状态（0：离线/1：在线）*/ 
VM_OS Varchar(255) not null,                    /*系统信息*/ 
VM_SysPath Varchar(255) not null,               /*系统盘所在路径*/ 
VM_SysFilename Varchar(255) not null,           /*系统盘文件名*/ 
VM_DataPath Varchar(255) not null,              /*数据盘所在路径*/ 
VM_DataFilename Varchar(255) not null,          /*数据盘文件名*/ 
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
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/* 教学虚拟机表：CDC_EDU_VM */
CREATE TABLE IF NOT EXISTS CDC_EDU_VM (
EDU_VM_ID bigint(20) not null AUTO_INCREMENT,   /*虚拟机ID*/ 
EDU_VM_Template_ID bigint(20) not null,         /*模板ID*/ 
EDU_VM_ThinClient_ID bigint(20) not null,       /*瘦终端ID*/ 
EDU_VM_State INT not null,                      /*虚拟机状态（0：离线/1：在线）*/ 
EDU_VM_SysPath Varchar(255) not null,           /*系统盘所在路径*/ 
EDU_VM_SysFilename Varchar(255) not null,       /*系统盘文件名*/ 
PRIMARY KEY (EDU_VM_ID),
FOREIGN KEY (EDU_VM_Template_ID)
        REFERENCES CDC_Template(Template_ID)
        ON DELETE CASCADE,
FOREIGN KEY (EDU_VM_ThinClient_ID)
        REFERENCES CDC_ThinClient(ThinClient_ID)
        ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/* 授权表：CDC_Authorization */
CREATE TABLE IF NOT EXISTS CDC_Authorization (
Authorization_MaxClientNum bigint(20) not null, /*设备数上限*/ 
Authorization_Deadline DATE not null,           /*到期时间*/ 
Authorization_MachineID Varchar(255) not null,  /*机器码*/ 
Authorization_Company Varchar(255) not null,    /*单位名称*/ 
PRIMARY KEY (Authorization_MachineID)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;