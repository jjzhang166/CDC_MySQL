
#include "CDC_MySQL_TestsSuite.h"
#include "CDC_UserGroup_Tests.h"
#include "CDC_User_Tests.h"
#include "CDC_Host_Tests.h"


CppUnit::Test* CDC_MySQL_TestsSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("CDC_MySQL_TestsSuite");

	pSuite->addTest(CDC_UserGroup_Tests::suite());
	pSuite->addTest(CDC_User_Tests::suite());
	pSuite->addTest(CDC_Host_Tests::suite());

	return pSuite;
}
