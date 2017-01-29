

#ifndef CDC_ISO_Tests_INCLUDED
#define CDC_ISO_Tests_INCLUDED

#include "CppUnit/TestCase.h"
#include "CDC_ISO.h"
#include "CDC_ISO.h"
#include "CDC_Host.h"

class CDC_ISO_Tests: public CppUnit::TestCase
{
public:
	CDC_ISO_Tests(const std::string& name);
	~CDC_ISO_Tests();

	void setUp();
	void tearDown();

	void testJsonAdd();
	void testJsonDel();
	void testJsonUpdate();
	void testJsonFind();
	void testJsonFindCount();

	static CppUnit::Test* suite();

private:
	CDC_ISO* _pObj;
	CDC_Host* _pCDC_HostObj;
private:
	void dependsFuc();
};


#endif // SQLiteTest_INCLUDED
