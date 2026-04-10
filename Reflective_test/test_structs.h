#pragma once
#include <string>
#include <vector>
#include <string_view>
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