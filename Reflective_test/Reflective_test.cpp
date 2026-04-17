#include "pch.h"
#include "CppUnitTest.h"
#include "Reflective.h"
#include "test_structs.h"
#include <filesystem>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace Reflectivetest
{
	TEST_CLASS(Reflectivetest)
	{
	public:
		/*TEST_METHOD_CLEANUP(cleanup)
		{
			TestStructIntern::s_deserialized = false;
			TestStructIntern::s_reference.m_bool = false;
			TestStructIntern::s_reference.m_string = "";
			TestStructIntern::s_reference.m_int = 0;
			TestStructIntern::s_reference.m_floatVector.clear();

			TestEnum::s_deserialized = false;
			TestEnum::s_reference.m_enum = EnumTest::Enum0;
			TestEnum::s_reference.m_value = 0;

			TestNested::s_deserialized = false;
		}*/
		
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
			//Assert::IsTrue(TestEnum::s_deserialized);
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

		TEST_METHOD(Test_legacy)
		{
			Assert::IsTrue(Reflective::instance().loadFile(R"(..\..\Test_data\test1.json)"));
			Assert::IsTrue(Reflective::instance().hasProfile("LegacyProfile"));
			Reflective::instance().setProfile("LegacyProfile");
			TestLegacy legacy;

			Assert::AreEqual(3, legacy.m_valueLegacy);
			Assert::IsTrue(EnumTest::Enum2 == legacy.m_enum);
			Assert::AreEqual(4, legacy.m_value);
		}

		TEST_METHOD(Test_optional)
		{
			Assert::IsTrue(Reflective::instance().loadFile(R"(..\..\Test_data\test1.json)"));
			Assert::IsTrue(Reflective::instance().hasProfile("OptionalProfile"));
			Reflective::instance().setProfile("OptionalProfile");
			TestOptional opt;

			Assert::IsTrue(opt.m_int.has_value());
			Assert::IsTrue(opt.m_str.has_value());
			Assert::AreEqual(4, opt.m_int.value());
			Assert::AreEqual(std::string("Hello world"), opt.m_str.value());
		}

		TEST_METHOD(Test_default1)
		{
			Assert::IsTrue(Reflective::instance().loadFile(R"(..\..\Test_data\test1.json)"));
			Assert::IsTrue(Reflective::instance().hasProfile("DefaultProfile1"));
			Reflective::instance().setProfile("DefaultProfile1");
			TestDefault def;

			Assert::AreEqual(2, def.m_int);
			Assert::AreEqual(std::string("default"), def.m_str);
		}

		TEST_METHOD(Test_default2)
		{
			Assert::IsTrue(Reflective::instance().loadFile(R"(..\..\Test_data\test1.json)"));
			Assert::IsTrue(Reflective::instance().hasProfile("DefaultProfile2"));
			Reflective::instance().setProfile("DefaultProfile2");
			TestDefault def;

			Assert::AreEqual(5, def.m_int);
			Assert::AreEqual(std::string("Hello world"), def.m_str);
		}

		TEST_METHOD(Test_IsConvertible)
		{
			Assert::IsTrue(is_convertible<EnumTest, std::string_view>);
			Assert::IsTrue(is_convertible<unsigned int, std::string_view>);
		}

		TEST_METHOD(Test_Flags)
		{
			Assert::IsTrue(Reflective::instance().loadFile(R"(..\..\Test_data\test1.json)"));
			Assert::IsTrue(Reflective::instance().hasProfile("FlagProfile"));
			Reflective::instance().setProfile("FlagProfile");
			TestFlag def;

			Assert::AreEqual(static_cast<unsigned int>((1 << 2) | (1 << 4) | 1), def.m_flag);
		}
	};
}
