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
#include <optional>
#include <variant>

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

template<typename Output, typename Input>
concept is_convertible = requires(const Input& s) {
    { convert<Output, Input>(s) } -> std::same_as<Output>;
};

//-------------------------------------------------------------------------------
// Type traits check if type is optional
template<typename Type>
struct is_std_optional : std::false_type {};

template<typename Type>
struct is_std_optional<std::optional<Type>> : std::true_type {};

template<typename T>
constexpr bool is_std_optional_v = is_std_optional<T>::value;

template<typename T, typename U>
concept is_std_optional_assignable_v = is_std_optional_v<T> &&
std::is_trivially_assignable_v<typename T::value_type&, U>;

template<typename T, typename U>
concept is_std_optional_same_v = is_std_optional_v<T> &&
std::is_same_v<typename T::value_type, U>;
//-------------------------------------------------------------------------------
// Type traits check if type is variant
// not use in visitor because ex std::variant<int, double, bool> = unsigned int cause trouble
template<typename Type>
struct is_std_variant : std::false_type {};

template<typename ...Type>
struct is_std_variant<std::variant<Type...>> : std::true_type {};

template<typename T>
constexpr bool is_std_variant_v = is_std_variant<T>::value;

template<typename ValueType, typename... Ts>
constexpr bool is_variant_assignable(...) {
    bool bIsAssignable = false;
    ((bIsAssignable = bIsAssignable || std::is_same_v<Ts, ValueType> || std::is_trivially_assignable_v<Ts&, ValueType>), ...);
    return bIsAssignable;
}

template<typename ValueType, typename... Ts>
constexpr bool is_variant_assignable(const std::variant<Ts...>&) {
    return is_variant_assignable<ValueType, Ts...>();
}

//-------------------------------------------------------------------------------