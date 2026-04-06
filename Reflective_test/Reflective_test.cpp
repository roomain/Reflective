#include "pch.h"
#include "CppUnitTest.h"
#include "Reflective.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


struct TestStructIntern
{
	bool m_bool;
	int m_int;
	std::string m_string;
	std::vector<float> m_floatVector;

	static inline bool s_created = false;
	static TestStructIntern s_ref;

	static bool deserialize(TestStructIntern& a_data)
	{
		Reflective::instance().deserialize("TestStructIntern", a_data,
			std::make_pair("m_bool", &TestStructIntern::m_bool),
			std::make_pair("m_int", &TestStructIntern::m_int),
			std::make_pair("m_string", &TestStructIntern::m_string),
			std::make_pair("m_floatVector", &TestStructIntern::m_floatVector));
	}

	TestStructIntern()
	{
		if (!TestStructIntern::s_created)
			TestStructIntern::s_created = TestStructIntern::deserialize(TestStructIntern::s_ref);		
		*this = TestStructIntern::s_ref;
	}
};

TestStructIntern TestStructIntern::s_ref;

struct TestStructNested
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

TestStructNested TestStructNested::s_ref;

namespace Reflectivetest
{
	TEST_CLASS(Reflectivetest)
	{
	public:
		
		TEST_METHOD(Test_deserialization)
		{
			struct TestStruct
			{
				bool m_bool;
				int m_int;
				std::string m_string;
				std::vector<float> m_floatVector;
			};

			Assert::IsTrue(Reflective::instance().loadFile(R"(D:\ProjetsGIT\Reflective\Test_data\test1.json)"));
			Assert::IsTrue(Reflective::instance().hasProfile("Test1"));
			Reflective::instance().setProfile("Test1");
			TestStruct myTest;
			Reflective::instance().deserialize("TestStruct", myTest,
				std::make_pair("m_bool", &TestStruct::m_bool),
				std::make_pair("m_int", &TestStruct::m_int),
				std::make_pair("m_string", &TestStruct::m_string),
				std::make_pair("m_floatVector", &TestStruct::m_floatVector));

			Assert::IsTrue(myTest.m_bool);
			Assert::AreEqual(5, myTest.m_int);
			Assert::AreEqual(std::string("Hello world"), myTest.m_string);
			Assert::AreEqual(size_t(4), myTest.m_floatVector.size());
		}

		TEST_METHOD(Test_deserialization_heritage)
		{
			struct TestStruct
			{
				bool m_bool;
				int m_int;
				std::string m_string;
				std::vector<float> m_floatVector;
			};

			Assert::IsTrue(Reflective::instance().loadFile(R"(D:\ProjetsGIT\Reflective\Test_data\test1.json)"));
			Assert::IsTrue(Reflective::instance().hasProfile("Test1"));
			Reflective::instance().setProfile("Test2");
			TestStruct myTest;
			Reflective::instance().deserialize("TestStruct", myTest,
				std::make_pair("m_bool", &TestStruct::m_bool),
				std::make_pair("m_int", &TestStruct::m_int),
				std::make_pair("m_string", &TestStruct::m_string),
				std::make_pair("m_floatVector", &TestStruct::m_floatVector));

			Assert::IsFalse(myTest.m_bool);
			Assert::AreEqual(8, myTest.m_int);
			Assert::AreEqual(std::string("Hello world"), myTest.m_string);
			Assert::AreEqual(size_t(6), myTest.m_floatVector.size());
		}

		TEST_METHOD(Test_deserialization_intern)
		{
			Assert::IsTrue(Reflective::instance().loadFile(R"(D:\ProjetsGIT\Reflective\Test_data\test1.json)"));
			Assert::IsTrue(Reflective::instance().hasProfile("TestIntern"));
			Reflective::instance().setProfile("TestIntern");
			TestStructIntern myTest;

			Assert::IsTrue(myTest.m_bool);
			Assert::AreEqual(5, myTest.m_int);
			Assert::AreEqual(std::string("Hello world"), myTest.m_string);
			Assert::AreEqual(size_t(4), myTest.m_floatVector.size());
		}
	};
}
