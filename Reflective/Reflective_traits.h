#pragma once
/***********************************************
* @headerfile reflection_traits.h
* @date 26 / 02 / 2026
* @author Roomain
************************************************/
#include <type_traits>
#include <vector>
#include <array>
#include <list>

// Type traits check if type is vector
template<typename Type>
struct is_std_vector : std::false_type {};


template<typename Type, typename Alloc>
struct is_std_vector<std::vector<Type, Alloc>> : std::true_type {};

template<typename T>
constexpr bool is_std_vector_v = is_std_vector<T>::value;

//-------------------------------------------------------------------------------
// Type traits check if type is list
template<typename Type>
struct is_std_list : std::false_type {};


template<typename Type, typename Alloc>
struct is_std_list<std::list<Type, Alloc>> : std::true_type {};

template<typename T>
constexpr bool is_std_list_v = is_std_list<T>::value;
//-------------------------------------------------------------------------------
// Type traits check if type is array
template<typename Type>
struct is_std_array : std::false_type {};


template<typename Type, std::size_t Size>
struct is_std_array<std::array<Type, Size>> : std::true_type {};

template<typename T>
constexpr bool is_std_array_v = is_std_array<T>::value;
//-------------------------------------------------------------------------------

template<typename, typename = std::void_t<>>
struct is_reflective : std::false_type {};

template<typename T>
struct is_reflective<T, std::void_t<decltype(&T::s_reflectiveCtx)> > : std::true_type {};

template<typename T>
constexpr bool is_reflective_v = is_reflective<T>::value;

//-------------------------------------------------------------------------------

template<typename T>
concept is_convertible = requires(const std::string_view s) {
    { convert<T>(s) } -> std::same_as<T>;
};