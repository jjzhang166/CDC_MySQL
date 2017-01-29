

#ifndef CDC_ThinClientGroup_Tests_INCLUDED
#define CDC_ThinClientGroup_Tests_INCLUDED

#include "CppUnit/TestCase.h"
#include "CDC_ThinClientGroup.h"

class CDC_ThinClientGroup_Tests: public CppUnit::TestCase
{
public:
	CDC_ThinClientGroup_Tests(const std::string& name);
	~CDC_ThinClientGroup_Tests();

	void setUp();
	void tearDown();

	void testJsonAdd();
	void testJsonDel();
	void testJsonUpdate();
	void testJsonFind();
	void testJsonFindCount();

	static CppUnit::Test* suite();

private:
	CDC_ThinClientGroup* _pObj;
};


#endif // SQLiteTest_INCLUDED
