#pragma once
#include <string>
#include <vector>
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