#pragma once
#include <string>
#include <vector>
#include <string_view>
#include <optional>
#include <variant>
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
EnumTest convert<EnumTest>(const std::string_view data)
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

