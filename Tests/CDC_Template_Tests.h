

#ifndef CDC_Template_Tests_INCLUDED
#define CDC_Template_Tests_INCLUDED

#include "CppUnit/TestCase.h"
#include "CDC_Template.h"
#include "CDC_Host.h"

class CDC_Template_Tests: public CppUnit::TestCase
{
public:
	CDC_Template_Tests(const std::string& name);
	~CDC_Template_Tests();

	void setUp();
	void tearDown();

	void testJsonAdd();
	void testJsonDel();
	void testJsonUpdate();
	void testJsonFind();
	void testJsonFindCount();

	static CppUnit::Test* suite();

private:
	CDC_Template* _pObj;
	CDC_Host* _pCDC_HostObj;
private:
	void dependsFuc();
};


#endif // SQLiteTest_INCLUDED
