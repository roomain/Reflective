#pragma once
/***********************************************
* @headerfile Reflective_macros.h
* @date 09 / 04 / 2026
* @author Roomain
************************************************/

// used for parsing file
#define REFLECT_CLASS

// used to define static data used for reflection
#define REFLECT_DEFINE(classname) \
REFLECT_DEF_##classname

// used to implement static data used for reflection
#define REFLECT_IMPL(classname) \
REFLECT_STATIC_IMPL_##classname