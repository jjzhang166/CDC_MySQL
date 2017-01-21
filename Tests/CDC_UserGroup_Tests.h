

#ifndef CDC_UserGroup_Tests_INCLUDED
#define CDC_UserGroup_Tests_INCLUDED

#include "CppUnit/TestCase.h"
#include "CDC_UserGroup.h"

class CDC_UserGroup_Tests: public CppUnit::TestCase
{
public:
	CDC_UserGroup_Tests(const std::string& name);
	~CDC_UserGroup_Tests();

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
	CDC_UserGroup* _pObj;
};


#endif // SQLiteTest_INCLUDED
