

#ifndef CDC_EDU_VM_Tests_INCLUDED
#define CDC_EDU_VM_Tests_INCLUDED

#include "CppUnit/TestCase.h"
#include "CDC_EDU_VM.h"
#include "CDC_Host.h"
#include "CDC_Template.h"
#include "CDC_EDU_VM.h"
#include "CDC_ThinClient.h"
#include "CDC_ThinClientGroup.h"

class CDC_EDU_VM_Tests: public CppUnit::TestCase
{
public:
	CDC_EDU_VM_Tests(const std::string& name);
	~CDC_EDU_VM_Tests();

	void setUp();
	void tearDown();

	void testJsonAdd();
	void testJsonDel();
	void testJsonUpdate();
	void testJsonFind();
	void testJsonFindCount();

	static CppUnit::Test* suite();

private:
	CDC_EDU_VM* _pObj;
	CDC_Host* _pCDC_HostObj;
	CDC_Template* _pCDC_TemplateObj;
	CDC_ThinClient* _pCDC_ThinClientObj;
	CDC_ThinClientGroup* _pCDC_ThinClientGroupObj;
private:
	void dependsFuc();
	void dependsFuc1();
	void dependsFuc2();
};


#endif // SQLiteTest_INCLUDED
