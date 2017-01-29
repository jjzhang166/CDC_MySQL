

#ifndef CDC_Authorization_Tests_INCLUDED
#define CDC_Authorization_Tests_INCLUDED

#include "CppUnit/TestCase.h"
#include "CDC_Authorization.h"

class CDC_Authorization_Tests: public CppUnit::TestCase
{
public:
	CDC_Authorization_Tests(const std::string& name);
	~CDC_Authorization_Tests();

	void setUp();
	void tearDown();

	void testJsonAdd();
	void testJsonDel();
	void testJsonUpdate();
	void testJsonFind();
	void testJsonFindCount();

	static CppUnit::Test* suite();

private:
	CDC_Authorization* _pObj;
};


#endif // SQLiteTest_INCLUDED
