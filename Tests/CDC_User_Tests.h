

#ifndef CDC_User_Tests_INCLUDED
#define CDC_User_Tests_INCLUDED

#include "CppUnit/TestCase.h"
#include "CDC_User.h"
#include "CDC_UserGroup.h"

class CDC_User_Tests: public CppUnit::TestCase
{
public:
	CDC_User_Tests(const std::string& name);
	~CDC_User_Tests();

	void setUp();
	void tearDown();

	void testJsonAdd();
	void testJsonDel();
	void testJsonUpdate();
	void testJsonFind();
	void testJsonFindCount();

	void testCount();
	void testAdd();
	void testDel();
	void testUpdate();
	void testFind();

	static CppUnit::Test* suite();

private:
	CDC_User* _pObj;
	CDC_UserGroup* _pCDC_UserGroupObj;

private:
	void dependsFuc();
};


#endif // SQLiteTest_INCLUDED
