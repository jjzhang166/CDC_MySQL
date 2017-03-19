## DB接口使用

### Method请求类型表：
```
值	含义
All	代表所有例如在查找中代表查找表中所有数据
Part	代表部分，例如查找中查找符合某些条件的记录
```




1.ADD
函数名例子：string  CDC_User_Add（string  req）;

参数格式：
```
字段	数据类型	含义	备注
Method	string	请求类型	见请求类型表
User_Role	integer	见表结构文档， 下同	
User_UserGroup_ID	integer		
User_Name	string		
User_LoginName	string		
User_Password	string		
User_Remarks	string		
```			

返回值格式：
```
字段	数据类型	含义	备注
Result	integer	执行结果	失败：-1/-2/…. 成功：返回这条记录的ID
```
Demo：

参数：
```
{
“Method”: “Part”,
“User_Role”:1,
“User_UserGroup_ID”:2,
“User_Name”:”xxx”,
“User_LoginName”:”XXX_asd”,
“User_Password”:”111111sdfs”,
“User_Remarks”:”asdasd”
}
```
返回值：
```
{
“Result”: 错误码或者ID
}
```

2.DELETE
函数名例子：string  CDC_User_Delete（string  req）;

```   
字段	数据类型	含义	备注
Method	string	请求类型	见请求类型表
User_ID	integer		
```
服务端反馈：
```
字段	数据类型	含义	备注
Result	integer	执行结果	0：失败  1：成功
```
Demo：
```
参数：
{
“Method”: “Part”,
“User_ID”:1
}
```
返回值：
```
{
“Result”: 1
}
```
3.FIND
函数名例子：string  CDC_User_Find（string  req）;

参数格式：
```
字段	数据类型	含义	备注
Method	string	请求类型	见请求类型表
User_Role	integer		此位置为要查找的条件，如果Method字段是“All”，此处不填，如果Method字段为“Part”时条件最少一个。
User_UserGroup_ID	integer		
User_Name	string		
User_LoginName	string		
User_Password	string		
User_Remarks	string		
```			

返回值格式：
```
字段	数据类型	含义	备注
Result	integer	执行结果	此处为查询结果个数 
Data	json	查询数据结果	
```
Demo：
```
参数：
{
“Method”: “Part”,
“User_Name”:”xxx”,
“User_UserGroup_ID”:2
}
```
或者
```
{
“Method”: “All”
}
```
返回值：
```
{
“Result”: 3，
“Data”:
[
{
“User_ID”:1,
“User_Role”:1,
“User_UserGroup_ID”:2,
“User_Name”:”xxx”,
“User_LoginName”:”XXX_aaa”,
“User_Password”:”111111sdfs”,
“User_Remarks”:”asdasd”
},
{
“User_ID”:2,
“User_Role”:1,
“User_UserGroup_ID”:2,
“User_Name”:”xxx”,
“User_LoginName”:”XXX_bbb”,
“User_Password”:”111111sdfs”,
“User_Remarks”:”asdasd”
},
{
“User_ID”:3,
“User_Role”:1,
“User_UserGroup_ID”:2,
“User_Name”:”xxx”,
“User_LoginName”:”XXX_ccc”,
“User_Password”:”111111sdfs”,
“User_Remarks”:”asdasd”
}
]
}
```
或者
```
{
“Result”: 错误码
}
```
4.FINDCOUNT
```
函数名例子：string  CDC_User_FindCount（string  req）;
```
参数格式：
```
字段	数据类型	含义	备注
Method	string	请求类型	见请求类型表
User_Role	integer		此位置为要查找的条件，如果Method字段是“All”，此处不填，如果Method字段为“Part”时条件最少一个。
User_UserGroup_ID	integer		
User_Name	string		
User_LoginName	string		
User_Password	string		
User_Remarks	string		
```			

返回值格式：
```
字段	数据类型	含义	备注
Result	integer	执行结果	此处为符合条件查询结果的个数 
```
Demo：
```
参数：
{
“Method”: “Part”,
“User_Name”:”xxx”,
“User_UserGroup_ID”:2
}
```
或者
```
{
“Method”: “All”
}
```
返回值：
```
{
“Result”: 3
}
```

5.UPDATE
```
函数名例子：string  CDC_User_Update（string  req）;
```
参数格式：
```
字段	数据类型	含义	备注
Method	string	请求类型	见请求类型表
User_ID	integer		表的主键为必填项
User_Role	integer		此位置为要更新的项，最少一个。
User_UserGroup_ID	integer		
User_Name	string		
User_LoginName	string		
User_Password	string		
User_Remarks	string		
```			

返回值格式：
```
字段	数据类型	含义	备注
Result	integer	执行结果	0：失败  1：成功
```
Demo：
```
参数：
{
“Method”: “Part”,
“User_ID”:1,
“User_Name”:”xxx”,
“User_UserGroup_ID”:2
}
```
返回值：
```
{
“Result”: 1
}
```
注意：
授权表CDC_Authorization中的
Authorization_Deadline		DATE	到期时间

传入的是string格式   
在json字符串中格式： 
 ```
 “Authorization_Deadline” : ”2017-12-31 12:56:30”
```