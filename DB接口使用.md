## DB�ӿ�ʹ��

### Method�������ͱ�
```
ֵ	����
All	�������������ڲ����д�����ұ�����������
Part	�����֣���������в��ҷ���ĳЩ�����ļ�¼
```




1.ADD
���������ӣ�string  CDC_User_Add��string  req��;

������ʽ��
```
�ֶ�	��������	����	��ע
Method	string	��������	���������ͱ�
User_Role	integer	����ṹ�ĵ��� ��ͬ	
User_UserGroup_ID	integer		
User_Name	string		
User_LoginName	string		
User_Password	string		
User_Remarks	string		
```			

����ֵ��ʽ��
```
�ֶ�	��������	����	��ע
Result	integer	ִ�н��	ʧ�ܣ�-1/-2/��. �ɹ�������������¼��ID
```
Demo��

������
```
{
��Method��: ��Part��,
��User_Role��:1,
��User_UserGroup_ID��:2,
��User_Name��:��xxx��,
��User_LoginName��:��XXX_asd��,
��User_Password��:��111111sdfs��,
��User_Remarks��:��asdasd��
}
```
����ֵ��
```
{
��Result��: ���������ID
}
```

2.DELETE
���������ӣ�string  CDC_User_Delete��string  req��;

```   
�ֶ�	��������	����	��ע
Method	string	��������	���������ͱ�
User_ID	integer		
```
����˷�����
```
�ֶ�	��������	����	��ע
Result	integer	ִ�н��	0��ʧ��  1���ɹ�
```
Demo��
```
������
{
��Method��: ��Part��,
��User_ID��:1
}
```
����ֵ��
```
{
��Result��: 1
}
```
3.FIND
���������ӣ�string  CDC_User_Find��string  req��;

������ʽ��
```
�ֶ�	��������	����	��ע
Method	string	��������	���������ͱ�
User_Role	integer		��λ��ΪҪ���ҵ����������Method�ֶ��ǡ�All�����˴�������Method�ֶ�Ϊ��Part��ʱ��������һ����
User_UserGroup_ID	integer		
User_Name	string		
User_LoginName	string		
User_Password	string		
User_Remarks	string		
```			

����ֵ��ʽ��
```
�ֶ�	��������	����	��ע
Result	integer	ִ�н��	�˴�Ϊ��ѯ������� 
Data	json	��ѯ���ݽ��	
```
Demo��
```
������
{
��Method��: ��Part��,
��User_Name��:��xxx��,
��User_UserGroup_ID��:2
}
```
����
```
{
��Method��: ��All��
}
```
����ֵ��
```
{
��Result��: 3��
��Data��:
[
{
��User_ID��:1,
��User_Role��:1,
��User_UserGroup_ID��:2,
��User_Name��:��xxx��,
��User_LoginName��:��XXX_aaa��,
��User_Password��:��111111sdfs��,
��User_Remarks��:��asdasd��
},
{
��User_ID��:2,
��User_Role��:1,
��User_UserGroup_ID��:2,
��User_Name��:��xxx��,
��User_LoginName��:��XXX_bbb��,
��User_Password��:��111111sdfs��,
��User_Remarks��:��asdasd��
},
{
��User_ID��:3,
��User_Role��:1,
��User_UserGroup_ID��:2,
��User_Name��:��xxx��,
��User_LoginName��:��XXX_ccc��,
��User_Password��:��111111sdfs��,
��User_Remarks��:��asdasd��
}
]
}
```
����
```
{
��Result��: ������
}
```
4.FINDCOUNT
```
���������ӣ�string  CDC_User_FindCount��string  req��;
```
������ʽ��
```
�ֶ�	��������	����	��ע
Method	string	��������	���������ͱ�
User_Role	integer		��λ��ΪҪ���ҵ����������Method�ֶ��ǡ�All�����˴�������Method�ֶ�Ϊ��Part��ʱ��������һ����
User_UserGroup_ID	integer		
User_Name	string		
User_LoginName	string		
User_Password	string		
User_Remarks	string		
```			

����ֵ��ʽ��
```
�ֶ�	��������	����	��ע
Result	integer	ִ�н��	�˴�Ϊ����������ѯ����ĸ��� 
```
Demo��
```
������
{
��Method��: ��Part��,
��User_Name��:��xxx��,
��User_UserGroup_ID��:2
}
```
����
```
{
��Method��: ��All��
}
```
����ֵ��
```
{
��Result��: 3
}
```

5.UPDATE
```
���������ӣ�string  CDC_User_Update��string  req��;
```
������ʽ��
```
�ֶ�	��������	����	��ע
Method	string	��������	���������ͱ�
User_ID	integer		�������Ϊ������
User_Role	integer		��λ��ΪҪ���µ������һ����
User_UserGroup_ID	integer		
User_Name	string		
User_LoginName	string		
User_Password	string		
User_Remarks	string		
```			

����ֵ��ʽ��
```
�ֶ�	��������	����	��ע
Result	integer	ִ�н��	0��ʧ��  1���ɹ�
```
Demo��
```
������
{
��Method��: ��Part��,
��User_ID��:1,
��User_Name��:��xxx��,
��User_UserGroup_ID��:2
}
```
����ֵ��
```
{
��Result��: 1
}
```
ע�⣺
��Ȩ��CDC_Authorization�е�
Authorization_Deadline		DATE	����ʱ��

�������string��ʽ   
��json�ַ����и�ʽ�� 
 ```
 ��Authorization_Deadline�� : ��2017-12-31 12:56:30��
```