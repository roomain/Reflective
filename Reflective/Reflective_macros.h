#pragma once
/***********************************************
* @headerfile Reflective_macros.h
* @date 09 / 04 / 2026
* @author Roomain
************************************************/
#include <string_view>

// used for parsing file
#define REFLECT_CLASS

// used to define static data used for reflection
#define REFLECT_DEFINE(classname) \
REFLECT_DEF_##classname

// used to implement static data used for reflection
#define REFLECT_IMPL(classname) \
REFLECT_STATIC_IMPL_##classname

// define convert function
template<typename Output, typename Input>
Output convert(const Input& a_data) = delete;