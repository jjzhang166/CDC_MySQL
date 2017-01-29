

#ifndef CDC_ThinClient_Tests_INCLUDED
#define CDC_ThinClient_Tests_INCLUDED

#include "CppUnit/TestCase.h"
#include "CDC_ThinClient.h"
#include "CDC_ThinClient.h"
#include "CDC_ThinClientGroup.h"

class CDC_ThinClient_Tests: public CppUnit::TestCase
{
public:
	CDC_ThinClient_Tests(const std::string& name);
	~CDC_ThinClient_Tests();

	void setUp();
	void tearDown();

	void testJsonAdd();
	void testJsonDel();
	void testJsonUpdate();
	void testJsonFind();
	void testJsonFindCount();

	static CppUnit::Test* suite();

private:
	CDC_ThinClient* _pObj;
	CDC_ThinClientGroup* _pCDC_ThinClientGroupObj;
private:
	void dependsFuc();
};


#endif // SQLiteTest_INCLUDED
