#pragma once
/***********************************************
* @headerfile test_structs.generated.h
* @date 10/4/2026
* @author reflective.py
************************************************/
#include <utility>
#include <tuple>
#include <string_view>


#define REFLECT_DEF_TestStructIntern \
static inline bool s_deserialized = false; \
static TestStructIntern s_reference; \
static std::tuple<std::pair<std::string_view, bool TestStructIntern::*>, \
			std::pair<std::string_view, int TestStructIntern::*>, \
			std::pair<std::string_view, std::string TestStructIntern::*>, \
			std::pair<std::string_view, std::vector<float> TestStructIntern::*> \
> s_reflectiveCtx; \
 \
inline TestStructIntern() \
{ \
	if(!TestStructIntern::s_deserialized) \
	{ \
		TestStructIntern::s_deserialized = Reflective::instance().deserialize("TestStructIntern",TestStructIntern::s_reference); \
	} \
	*this = s_reference; \
}


#define REFLECT_STATIC_IMPL_TestStructIntern \
TestStructIntern TestStructIntern::s_reference; \
std::tuple<std::pair<std::string_view, bool TestStructIntern::*>, \
			std::pair<std::string_view, int TestStructIntern::*>, \
			std::pair<std::string_view, std::string TestStructIntern::*>, \
			std::pair<std::string_view, std::vector<float> TestStructIntern::*> \
> TestStructIntern::s_reflectiveCtx = std::make_tuple(std::make_pair("m_bool", &TestStructIntern::m_bool),std::make_pair("m_int", &TestStructIntern::m_int),std::make_pair("m_string", &TestStructIntern::m_string),std::make_pair("m_floatVector", &TestStructIntern::m_floatVector));


#define REFLECT_DEF_TestEnum \
static inline bool s_deserialized = false; \
static TestEnum s_reference; \
static std::tuple<std::pair<std::string_view, EnumTest TestEnum::*>, \
			std::pair<std::string_view, int TestEnum::*> \
> s_reflectiveCtx; \
 \
inline TestEnum() \
{ \
	if(!TestEnum::s_deserialized) \
	{ \
		TestEnum::s_deserialized = Reflective::instance().deserialize("TestEnum",TestEnum::s_reference); \
	} \
	*this = s_reference; \
}


#define REFLECT_STATIC_IMPL_TestEnum \
TestEnum TestEnum::s_reference; \
std::tuple<std::pair<std::string_view, EnumTest TestEnum::*>, \
			std::pair<std::string_view, int TestEnum::*> \
> TestEnum::s_reflectiveCtx = std::make_tuple(std::make_pair("m_enum", &TestEnum::m_enum),std::make_pair("m_value", &TestEnum::m_value));
