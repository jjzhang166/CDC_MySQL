

#ifndef CDC_VM_Tests_INCLUDED
#define CDC_VM_Tests_INCLUDED

#include "CppUnit/TestCase.h"
#include "CDC_VM.h"
#include "CDC_Host.h"
#include "CDC_Template.h"
#include "CDC_VM.h"
#include "CDC_UserGroup.h"
#include "CDC_User.h"
#include "CDC_ThinClient.h"
#include "CDC_ThinClientGroup.h"

class CDC_VM_Tests: public CppUnit::TestCase
{
public:
	CDC_VM_Tests(const std::string& name);
	~CDC_VM_Tests();

	void setUp();
	void tearDown();

	void testJsonAdd();
	void testJsonDel();
	void testJsonUpdate();
	void testJsonFind();
	void testJsonFindCount();

	static CppUnit::Test* suite();

private:
	CDC_VM* _pObj;
	CDC_Host* _pCDC_HostObj;
	CDC_Template* _pCDC_TemplateObj;
	CDC_UserGroup* _pCDC_UserGroupObj;
	CDC_User* _pCDC_UserObj;
	CDC_ThinClient* _pCDC_ThinClientObj;
	CDC_ThinClientGroup* _pCDC_ThinClientGroupObj;
private:
	void dependsFuc();
	void dependsFuc1();
	void dependsFuc2();
	void dependsFuc3();
	void dependsFuc4();
};


#endif // SQLiteTest_INCLUDED
