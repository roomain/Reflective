#include "pch.h"
#include "CppUnitTest.h"
#include "Reflective.h"
#include "test_structs.h"
#include <map>
#include <unordered_map>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace Reflectivetest
{
	TEST_CLASS(Type_traits)
	{
	public:
		TEST_METHOD(Test_IsVector)
		{
			Assert::IsFalse(is_std_vector_v<int>);
			Assert::IsFalse(is_std_vector_v<std::string>);
			Assert::IsFalse(is_std_vector_v<std::array<int,3>>);
			Assert::IsFalse(is_std_vector_v<std::list<int>>);
			Assert::IsFalse(is_std_vector_v<std::map<int, int>>);
			Assert::IsFalse(is_std_vector_v<std::unordered_map<int, int>>);
			Assert::IsTrue(is_std_vector_v<std::vector<std::string>>);
		}

		TEST_METHOD(Test_IsList)
		{
			Assert::IsFalse(is_std_list_v<int>);
			Assert::IsFalse(is_std_list_v<std::string>);
			Assert::IsFalse(is_std_list_v<std::array<int, 3>>);
			Assert::IsFalse(is_std_list_v<std::map<int, int>>);
			Assert::IsFalse(is_std_list_v<std::unordered_map<int, int>>);
			Assert::IsFalse(is_std_list_v<std::vector<std::string>>);
			Assert::IsTrue(is_std_list_v<std::list<int>>);
		}


		TEST_METHOD(Test_IsArray)
		{
			Assert::IsFalse(is_std_array_v<int>);
			Assert::IsFalse(is_std_array_v<std::string>);
			Assert::IsFalse(is_std_array_v<std::list<int>>);
			Assert::IsFalse(is_std_array_v<std::map<int, int>>);
			Assert::IsFalse(is_std_array_v<std::unordered_map<int, int>>);
			Assert::IsFalse(is_std_array_v<std::vector<std::string>>);
			Assert::IsTrue(is_std_array_v<std::array<int, 3>>);
		}

		TEST_METHOD(Test_IsConvertible)
		{
			Assert::IsTrue(is_convertible<EnumTest, std::string_view>);
			Assert::IsTrue(is_convertible<unsigned int, std::string_view>);
			struct TestNotConvertible
			{
				int notConvertible;
			};
			Assert::IsFalse(is_convertible<TestNotConvertible, std::string_view>);
		}

		TEST_METHOD(Test_IsReflective)
		{
			struct TestNotReflective
			{
				int notReflective;
			};
			Assert::IsFalse(is_reflective_v<int>);
			Assert::IsFalse(is_reflective_v<TestNotReflective>);
			Assert::IsTrue(is_reflective_v<TestOptionalVulkanFlag>);
		}

		TEST_METHOD(Test_IsOptionalReflective)
		{
			struct TestNotReflective
			{
				int notReflective;
			};
			Assert::IsFalse(is_optional_reflective_v<int>);
			Assert::IsFalse(is_optional_reflective_v<TestNotReflective>);
			Assert::IsTrue(is_optional_reflective_v<std::optional<TestOptionalVulkanFlag>>);
		}

		TEST_METHOD(Test_IsOptional)
		{
			struct TestNotReflective
			{
				int notReflective;
			};
			Assert::IsFalse(is_std_optional_v<int>);
			Assert::IsFalse(is_std_optional_v<TestNotReflective>);
			Assert::IsTrue(is_std_optional_v<std::optional<TestOptionalVulkanFlag>>);
		}
	};
}