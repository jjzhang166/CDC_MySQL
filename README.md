## MySQL表建立
表按照如图导入CDC_MySQL.sql
![](importSql1.jpg)
![](importSql2.jpg)

## 接口提供

* 接口在CDC_Host.h 、CDC_User.h和CDC_UserGroup.h 中

CDC_User.h举例，json接口为：
```
    std::string CDC_User_add(const std::string& req);
	std::string CDC_User_del(const std::string& req);
	std::string CDC_User_update(const std::string& req);
	std::string CDC_User_find(const std::string& req);
	std::string CDC_User_FindCount(const std::string& req);
```

## 测试程序运行
测试程序为bin/Tests,运行依赖libCDC_MySQL.so和libCppUnit.so
请将lib中的libCDC_MySQL.so和libCppUnit.so拷贝到bin文件夹
然后执行 ./Tests -all

## 安装
* 首先安装mysql及mysql开发包
- 先安装mysql
- 代码:
-sudo apt-get install mysql-server mysql-client

- 再装开发包
- 代码:
- sudo apt-get install libmysqlclient*-dev

* 给脚本增加可执行权限：
```
chmod +x install_and_compile.sh
./install_and_compile.sh
```

##直接编译 
chmod +x compile.sh
./compile.sh

##类库封装
* 类库以so文件的方式生成，所需头文件在include目录下
* include/MySQL5.7为mysql需要的头文件
* include/CppUnit为单元测试需要的头文件
* lib目标程序为lib/libCDC_MySQL.so


## 测试程序
* 测试程序为bin/Tests
* 使用方法 ./Tests -all
