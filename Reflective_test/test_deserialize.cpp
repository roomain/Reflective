#include "pch.h"
#include "CppUnitTest.h"
#include "Reflective.h"
#include "test_structs.h"
#include <filesystem>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace Reflectivetest
{
	TEST_CLASS(Deserialization)
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
		
		TEST_METHOD(Test_simple)
		{

			Assert::IsTrue(Reflective::instance().loadFile(R"(..\..\Test_data\tests_deserialize.json)"));
			Assert::IsTrue(Reflective::instance().hasProfile("Test_simple"));
			Reflective::instance().setProfile("Test_simple");
			TestStructIntern myTest;

			Assert::IsTrue(myTest.m_bool);
			Assert::AreEqual(5, myTest.m_int);
			Assert::AreEqual(std::string("Hello world"), myTest.m_string);
			Assert::AreEqual(size_t(4), myTest.m_floatVector.size());
		}

		TEST_METHOD(Test_profile_legacy)
		{
			Assert::IsTrue(Reflective::instance().loadFile(R"(..\..\Test_data\tests_deserialize.json)"));
			Assert::IsTrue(Reflective::instance().hasProfile("Test_profile_legacy"));
			Reflective::instance().setProfile("Test_profile_legacy");
			TestStructIntern myTest;

			Assert::IsFalse(myTest.m_bool);
			Assert::AreEqual(8, myTest.m_int);
			Assert::AreEqual(std::string("Hello world"), myTest.m_string);
			Assert::AreEqual(size_t(6), myTest.m_floatVector.size());
		}

		TEST_METHOD(Test_profile_override)
		{
			Assert::IsTrue(Reflective::instance().loadFile(R"(..\..\Test_data\tests_deserialize.json)"));
			Assert::IsTrue(Reflective::instance().hasProfile("Test_profile_override"));
			Reflective::instance().setProfile("Test_profile_override");
			TestStructIntern myTest;

			Assert::IsTrue(myTest.m_bool);
			Assert::AreEqual(5, myTest.m_int);
			Assert::AreEqual(std::string("Hello world"), myTest.m_string);
			// no override for arrays
			Assert::AreEqual(size_t(10), myTest.m_floatVector.size());
		}

		TEST_METHOD(Test_enum_as_string)
		{
			Assert::IsTrue(Reflective::instance().loadFile(R"(..\..\Test_data\tests_deserialize.json)"));
			Assert::IsTrue(Reflective::instance().hasProfile("Test_enum_as_string"));
			Reflective::instance().setProfile("Test_enum_as_string");
			TestEnum myTest;

			Assert::AreEqual(4, myTest.m_value);
			Assert::IsTrue(EnumTest::Enum2 == myTest.m_enum);
		}

		TEST_METHOD(Test_nested_struct)
		{
			Assert::IsTrue(Reflective::instance().loadFile(R"(..\..\Test_data\tests_deserialize.json)"));
			Assert::IsTrue(Reflective::instance().hasProfile("Test_nested_struct"));
			Reflective::instance().setProfile("Test_nested_struct");
			//TestEnum tEnum;
			TestNested nested;
			//Assert::IsTrue(TestEnum::s_deserialized);
			Assert::AreEqual(3, nested.m_value);
			Assert::IsTrue(EnumTest::Enum2 == nested.m_enumStruct.m_enum);
			Assert::AreEqual(4, nested.m_enumStruct.m_value);
		}

		TEST_METHOD(Test_nested_struct_overriden)
		{
			Assert::IsTrue(Reflective::instance().loadFile(R"(..\..\Test_data\tests_deserialize.json)"));
			Assert::IsTrue(Reflective::instance().hasProfile("Test_nested_struct_overriden"));
			Reflective::instance().setProfile("Test_nested_struct_overriden");
			TestNested nested;
			
			Assert::AreEqual(3, nested.m_value);
			Assert::IsTrue(EnumTest::Enum1 == nested.m_enumStruct.m_enum);
			Assert::AreEqual(2, nested.m_enumStruct.m_value);
		}

		TEST_METHOD(Test_struct_legacy)
		{
			Assert::IsTrue(Reflective::instance().loadFile(R"(..\..\Test_data\tests_deserialize.json)"));
			Assert::IsTrue(Reflective::instance().hasProfile("Test_struct_legacy"));
			Reflective::instance().setProfile("Test_struct_legacy");
			TestLegacy legacy;

			Assert::AreEqual(3, legacy.m_valueLegacy);
			Assert::IsTrue(EnumTest::Enum2 == legacy.m_enum);
			Assert::AreEqual(4, legacy.m_value);
		}

		TEST_METHOD(Test_optional)
		{
			Assert::IsTrue(Reflective::instance().loadFile(R"(..\..\Test_data\tests_deserialize.json)"));
			Assert::IsTrue(Reflective::instance().hasProfile("Test_optional"));
			Reflective::instance().setProfile("Test_optional");
			TestOptional opt;

			Assert::IsTrue(opt.m_int.has_value());
			Assert::IsTrue(opt.m_str.has_value());
			Assert::AreEqual(4, opt.m_int.value());
			Assert::AreEqual(std::string("Hello world"), opt.m_str.value());
		}

		TEST_METHOD(Test_struct_default_string)
		{
			Assert::IsTrue(Reflective::instance().loadFile(R"(..\..\Test_data\tests_deserialize.json)"));
			Assert::IsTrue(Reflective::instance().hasProfile("Test_struct_default_string"));
			Reflective::instance().setProfile("Test_struct_default_string");
			TestDefault def;

			Assert::AreEqual(2, def.m_int);
			Assert::AreEqual(std::string("default"), def.m_str);
		}

		TEST_METHOD(Test_struct_default_int)
		{
			Assert::IsTrue(Reflective::instance().loadFile(R"(..\..\Test_data\tests_deserialize.json)"));
			Assert::IsTrue(Reflective::instance().hasProfile("Test_struct_default_int"));
			Reflective::instance().setProfile("Test_struct_default_int");
			TestDefault def;

			Assert::AreEqual(5, def.m_int);
			Assert::AreEqual(std::string("Hello world"), def.m_str);
		}

		TEST_METHOD(Test_flags_as_string)
		{
			Assert::IsTrue(Reflective::instance().loadFile(R"(..\..\Test_data\tests_deserialize.json)"));
			Assert::IsTrue(Reflective::instance().hasProfile("Test_flags_as_string"));
			Reflective::instance().setProfile("Test_flags_as_string");
			TestFlag def;

			Assert::AreEqual(static_cast<unsigned int>((1 << 2) | (1 << 4) | (1 << 1)), def.m_flag);
		}

		TEST_METHOD(Test_VulkanFlags_as_string)
		{
			Assert::IsTrue(Reflective::instance().loadFile(R"(..\..\Test_data\tests_deserialize.json)"));
			Assert::IsTrue(Reflective::instance().hasProfile("Test_VulkanFlags_as_string"));
			Reflective::instance().setProfile("Test_VulkanFlags_as_string");
			TestVulkanFlag def;

			Assert::AreEqual(
				static_cast<unsigned int>(VK_QUEUE_COMPUTE_BIT | VK_QUEUE_PROTECTED_BIT | VK_QUEUE_DATA_GRAPH_BIT_ARM),
				static_cast<unsigned int>(def.m_flag));
		}

		TEST_METHOD(Test_optional_VulkanFlags_as_string_no_value)
		{
			Assert::IsTrue(Reflective::instance().loadFile(R"(..\..\Test_data\tests_deserialize.json)"));
			Assert::IsTrue(Reflective::instance().hasProfile("Test_optional_VulkanFlags_as_string_no_value"));
			Reflective::instance().setProfile("Test_optional_VulkanFlags_as_string_no_value");
			TestOptionalVulkanFlag def;

			Assert::IsFalse(def.m_opt.has_value());
		}

		TEST_METHOD(Test_optional_VulkanFlags_as_string_has_value)
		{
			Assert::IsTrue(Reflective::instance().loadFile(R"(..\..\Test_data\tests_deserialize.json)"));
			Assert::IsTrue(Reflective::instance().hasProfile("Test_optional_VulkanFlags_as_string_has_value"));
			Reflective::instance().setProfile("Test_optional_VulkanFlags_as_string_has_value");
			TestOptionalVulkanFlag def;

			Assert::IsTrue(def.m_opt.has_value());
			Assert::IsTrue(def.m_opt.value().m_flag == VK_QUEUE_COMPUTE_BIT);
		}
	};
}
