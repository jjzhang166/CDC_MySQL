
#include "CDC_MySQL_TestsSuite.h"
#include "CDC_UserGroup_Tests.h"
#include "CDC_User_Tests.h"
#include "CDC_Host_Tests.h"
#include "CDC_Template_Tests.h"
#include "CDC_ISO_Tests.h"
#include "CDC_ThinClientGroup_Tests.h"
#include "CDC_ThinClient_Tests.h"
#include "CDC_VM_Tests.h"
#include "CDC_EDU_VM_Tests.h"
#include "CDC_Authorization_Tests.h"


CppUnit::Test* CDC_MySQL_TestsSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("CDC_MySQL_TestsSuite");

	pSuite->addTest(CDC_UserGroup_Tests::suite());
	pSuite->addTest(CDC_User_Tests::suite());
	pSuite->addTest(CDC_Host_Tests::suite());
	pSuite->addTest(CDC_Template_Tests::suite());
	pSuite->addTest(CDC_ISO_Tests::suite());
	pSuite->addTest(CDC_ThinClientGroup_Tests::suite());
	pSuite->addTest(CDC_ThinClient_Tests::suite());
	pSuite->addTest(CDC_VM_Tests::suite());
	pSuite->addTest(CDC_EDU_VM_Tests::suite());
	pSuite->addTest(CDC_Authorization_Tests::suite());

	return pSuite;
}
