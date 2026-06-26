#include "pch.h"
#include "CppUnitTest.h"
#include "Reflective.h"
#include "test_structs.h"
#include <filesystem>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace Reflectivetest
{
	TEST_CLASS(Profiling)
	{
	public:
		TEST_METHOD_INITIALIZE(init)
		{
			Reflective::instance().clear();
		}

		TEST_METHOD_CLEANUP(cleanup)
		{
			Reflective::instance().clear();
		}

		TEST_METHOD(Test_removeprofile)
		{
			TestDefault defStruct;
			defStruct.m_int = 20;
			Reflective::instance().writeProfile("Test_simpleWrite", defStruct);
			Assert::IsTrue(Reflective::instance().hasProfile("Test_simpleWrite"));
			Reflective::instance().removeProfile("Test_simpleWrite");
			Assert::IsFalse(Reflective::instance().hasProfile("Test_simpleWrite"));
		}

		TEST_METHOD(Test_renameprofile)
		{
			TestDefault defStruct;
			defStruct.m_int = 20;
			Reflective::instance().writeProfile("Test_simpleWrite", defStruct);
			Assert::IsTrue(Reflective::instance().hasProfile("Test_simpleWrite"));
			Reflective::instance().setCurrentProfile("Test_simpleWrite");
			Reflective::instance().renameProfile("Test_simpleWrite", "newName");
			Assert::IsFalse(Reflective::instance().hasProfile("Test_simpleWrite"));
			Assert::IsTrue(Reflective::instance().hasProfile("newName"));
			Assert::AreEqual<std::string>(Reflective::instance().currentProfile(), "newName");
			TestDefault defStructNew;
			Assert::AreEqual(defStructNew.m_int, defStruct.m_int);
		}

		TEST_METHOD(Test_hasClass)
		{
			TestDefault defStruct;
			Reflective::instance().writeProfile("Test_simpleWrite", defStruct);
			Assert::IsTrue(Reflective::instance().hasProfile("Test_simpleWrite"));
			Assert::IsTrue(Reflective::instance().hasClass("Test_simpleWrite", "TestDefault"));
		}
	};
}