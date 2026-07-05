#pragma once

#ifdef REFLECTIVE_EXPORTS
#define REFLECTIVE_EXPORT __declspec(dllexport)
#else
#define REFLECTIVE_EXPORT __declspec(dllimport)
#endif