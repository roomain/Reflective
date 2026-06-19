#include "pch.h"
#include "CppUnitTest.h"
#include "Reflective.h"
#include "test_structs.h"
#include <filesystem>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace Reflectivetest
{
	TEST_CLASS(Serialization)
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

		TEST_METHOD(Test_simpleWrite)
		{
			TestDefault defStruct;
			defStruct.m_int = 20;
			Reflective::instance().writeProfile("Test_simpleWrite", defStruct);
			Reflective::instance().writeFile(R"(..\..\Test_data\tests_serialize.json)", true);
		}

		TEST_METHOD(Test_nestedWrite)
		{
			TestNested defStruct;
			Reflective::instance().writeProfile("Test_nested", defStruct);
			Reflective::instance().writeFile(R"(..\..\Test_data\Test_nestedWrite.json)", true);
		}

		TEST_METHOD(Test_nestedArrayWrite)
		{
			TestNestedArray defStruct;
			defStruct.m_enumStructList.emplace_back();
			defStruct.m_enumStructList.emplace_back();
			Reflective::instance().writeProfile("TestNestedArray", defStruct);
			Reflective::instance().writeFile(R"(..\..\Test_data\Test_nestedArrayWrite.json)", true);
		}
	};
}