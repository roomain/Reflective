#pragma once
#include <string>
#include <vector>
#include <string_view>
#include <optional>
#include <ranges>
#include "Reflective_macros.h"
#include "test_structs.generated.h"

REFLECT_CLASS
struct TestStructIntern
{
	bool m_bool;
	int m_int;
	std::string m_string;
	std::vector<float> m_floatVector;

	REFLECT_DEFINE(TestStructIntern)

};
REFLECT_IMPL(TestStructIntern)

enum class EnumTest
{
	Enum0,
	Enum1,
	Enum2
};

template<>
EnumTest convert<EnumTest, std::string_view>(const std::string_view& data)
{
	EnumTest converted = EnumTest::Enum0;
	if (data == "Enum0")
	{
		return EnumTest::Enum0;
	}
	if (data == "Enum1")
	{
		return EnumTest::Enum1;
	}
	if (data == "Enum2")
	{
		return EnumTest::Enum2;
	}
	return converted;
}

REFLECT_CLASS
struct TestEnum
{
	EnumTest m_enum;
	int m_value;
	REFLECT_DEFINE(TestEnum)
};
REFLECT_IMPL(TestEnum)

REFLECT_CLASS
struct TestNested
{
	TestEnum m_enumStruct;
	int m_value;
	REFLECT_DEFINE(TestNested)
};
REFLECT_IMPL(TestNested)

REFLECT_CLASS
struct TestLegacy : TestEnum
{
	int m_valueLegacy;
	REFLECT_DEFINE(TestLegacy)
};
REFLECT_IMPL(TestLegacy)


REFLECT_CLASS
struct TestOptional
{
	std::optional<int> m_int;
	std::optional<std::string> m_str;
	REFLECT_DEFINE(TestOptional)
};
REFLECT_IMPL(TestOptional)

REFLECT_CLASS
struct TestDefault
{
	int m_int = 5;
	std::string m_str = "default";
	REFLECT_DEFINE(TestDefault)
};
REFLECT_IMPL(TestDefault)


constexpr std::vector<std::string> split(const std::string_view a_entry, const char a_separator)
{
	std::string temp (a_entry);
	std::vector<std::string> splitted;
	while (!temp.empty())
	{
		if (auto iter = std::ranges::find(temp, a_separator); iter != temp.cend())
		{
			const size_t index = iter - temp.cbegin();
			splitted.emplace_back(temp.substr(0, iter - temp.cbegin()));
			temp = temp.substr(index + 1, temp.size() - index - 1);
		}
		else
		{
			splitted.emplace_back(temp);
			temp = "";
		}
	}
	return splitted;
}

template<>
unsigned int convert<unsigned int, std::string_view>(const std::string_view& data)
{
	int value = 0;
	auto flagList = split(data, '|');
	for (const auto& flag : flagList)
	{
		if (flag == "FLAG_1")
		{
			value = value | (1 << 1);
		}
		else if (flag == "FLAG_2")
		{
			value = value | (1 << 2);
		}
		else if (flag == "FLAG_3")
		{
			value = value | (1 << 3);
		}
		else if (flag == "FLAG_4")
		{
			value = value | (1 << 4);
		}
		else if (flag == "FLAG_5")
		{
			value = value | (1 << 5);
		}
	}
	return value;
}

REFLECT_CLASS
struct TestFlag
{
	unsigned int m_flag = 0;
	REFLECT_DEFINE(TestFlag)
};
REFLECT_IMPL(TestFlag)