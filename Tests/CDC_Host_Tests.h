

#ifndef CDC_Host_Tests_INCLUDED
#define CDC_Host_Tests_INCLUDED

#include "CppUnit/TestCase.h"
#include "CDC_Host.h"

class CDC_Host_Tests: public CppUnit::TestCase
{
public:
	CDC_Host_Tests(const std::string& name);
	~CDC_Host_Tests();

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
	CDC_Host* _pObj;
};


#endif // SQLiteTest_INCLUDED
