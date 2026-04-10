#include "pch.h"
#include "CppUnitTest.h"
#include "Reflective.h"
#include "test_structs.h"
#include <filesystem>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


/*struct TestStructNested
{
	bool m_bool;
	TestStructIntern m_intern;

	static inline bool s_created = false;
	static TestStructNested s_ref;


	static bool deserialize(TestStructNested& a_data)
	{
		Reflective::instance().deserialize("TestStructNested", a_data,
			std::make_pair("m_bool", &TestStructNested::m_bool),
			std::make_pair("m_intern", &TestStructNested::m_intern));
	}

	TestStructNested()
	{
		if (!TestStructNested::s_created)
			TestStructNested::s_created = TestStructNested::deserialize(TestStructNested::s_ref);

		*this = TestStructNested::s_ref;
	}
};

TestStructNested TestStructNested::s_ref;*/

namespace Reflectivetest
{
	TEST_CLASS(Reflectivetest)
	{
	public:
		TEST_METHOD_CLEANUP(cleanup)
		{
			/*reset for test*/
			TestStructIntern::s_deserialized = false;
			TestStructIntern::s_reference.m_bool = false;
			TestStructIntern::s_reference.m_string = "";
			TestStructIntern::s_reference.m_int = 0;
			TestStructIntern::s_reference.m_floatVector.clear();

			TestEnum::s_deserialized = false;
			TestEnum::s_reference.m_enum = EnumTest::Enum0;
			TestEnum::s_reference.m_value = 0;

			TestNested::s_deserialized = false;
		}
		
		TEST_METHOD(Test_deserialization)
		{

			Assert::IsTrue(Reflective::instance().loadFile(R"(..\..\Test_data\test1.json)"));
			Assert::IsTrue(Reflective::instance().hasProfile("Test1"));
			Reflective::instance().setProfile("Test1");
			TestStructIntern myTest;

			Assert::IsTrue(myTest.m_bool);
			Assert::AreEqual(5, myTest.m_int);
			Assert::AreEqual(std::string("Hello world"), myTest.m_string);
			Assert::AreEqual(size_t(4), myTest.m_floatVector.size());
		}

		TEST_METHOD(Test_deserialization_heritage)
		{
			Assert::IsTrue(Reflective::instance().loadFile(R"(..\..\Test_data\test1.json)"));
			Assert::IsTrue(Reflective::instance().hasProfile("Test1"));
			Reflective::instance().setProfile("Test2");
			TestStructIntern myTest;

			Assert::IsFalse(myTest.m_bool);
			Assert::AreEqual(8, myTest.m_int);
			Assert::AreEqual(std::string("Hello world"), myTest.m_string);
			Assert::AreEqual(size_t(6), myTest.m_floatVector.size());
		}

		TEST_METHOD(Test_deserialization_intern)
		{
			Assert::IsTrue(Reflective::instance().loadFile(R"(..\..\Test_data\test1.json)"));
			Assert::IsTrue(Reflective::instance().hasProfile("TestIntern"));
			Reflective::instance().setProfile("TestIntern");
			TestStructIntern myTest;

			Assert::IsTrue(myTest.m_bool);
			Assert::AreEqual(5, myTest.m_int);
			Assert::AreEqual(std::string("Hello world"), myTest.m_string);
			Assert::AreEqual(size_t(4), myTest.m_floatVector.size());
		}

		TEST_METHOD(Test_enumerate)
		{
			Assert::IsTrue(Reflective::instance().loadFile(R"(..\..\Test_data\test1.json)"));
			Assert::IsTrue(Reflective::instance().hasProfile("EnumProfile"));
			Reflective::instance().setProfile("EnumProfile");
			TestEnum myTest;

			Assert::AreEqual(4, myTest.m_value);
			Assert::IsTrue(EnumTest::Enum2 == myTest.m_enum);
		}

		TEST_METHOD(Test_nested)
		{
			Assert::IsTrue(Reflective::instance().loadFile(R"(..\..\Test_data\test1.json)"));
			Assert::IsTrue(Reflective::instance().hasProfile("NestedProfile"));
			Reflective::instance().setProfile("NestedProfile");
			//TestEnum tEnum;
			TestNested nested;
			Assert::IsTrue(TestEnum::s_deserialized);
			Assert::AreEqual(3, nested.m_value);
			Assert::IsTrue(EnumTest::Enum2 == nested.m_enumStruct.m_enum);
			Assert::AreEqual(4, nested.m_enumStruct.m_value);
		}

		TEST_METHOD(Test_nested1)
		{
			Assert::IsTrue(Reflective::instance().loadFile(R"(..\..\Test_data\test1.json)"));
			Assert::IsTrue(Reflective::instance().hasProfile("NestedProfile2"));
			Reflective::instance().setProfile("NestedProfile2");
			TestNested nested;
			
			Assert::AreEqual(3, nested.m_value);
			Assert::IsTrue(EnumTest::Enum1 == nested.m_enumStruct.m_enum);
			Assert::AreEqual(2, nested.m_enumStruct.m_value);
		}
	};
}
