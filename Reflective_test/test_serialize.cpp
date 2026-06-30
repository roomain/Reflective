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


		TEST_METHOD(Test_vectorWrite)
		{
			TestVector defStruct;
			defStruct.vData.emplace_back("TEST");
			defStruct.vData.emplace_back("TEST");
			defStruct.vData.emplace_back("TEST");
			Reflective::instance().writeProfile("TestVector", defStruct);
			Reflective::instance().writeFile(R"(..\..\Test_data\Test_vectorWrite.json)", true);
		}

		TEST_METHOD(Test_vectorReWrite)
		{
			TestVector defStruct;
			Reflective::instance().writeProfile("TestVector0", defStruct);
			defStruct.vData.emplace_back("TEST");
			defStruct.vData.emplace_back("TEST");
			Reflective::instance().writeProfile("TestVector0", defStruct);
			Reflective::instance().setCurrentProfile("TestVector0");
			TestVector defStruct2;
			Assert::AreEqual(2, static_cast<int>(defStruct2.vData.size()));
		}

		TEST_METHOD(Test_legacyWrite)
		{
			TestLegacy defStruct;
			defStruct.m_enum = EnumTest::Enum1;
			defStruct.m_value = 5;
			defStruct.m_valueLegacy = 3;
			Reflective::instance().writeProfile("TestLegacy", defStruct);
			Reflective::instance().setCurrentProfile("TestLegacy");
			TestLegacy defStruct2;
			Assert::AreEqual(defStruct.m_value, defStruct2.m_value);
		}
	};
}