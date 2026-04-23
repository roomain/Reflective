#pragma once
/***********************************************
* @headerfile Reflective_assign.h
* @date 22 / 04 / 2026
* @author Roomain
************************************************/
#include <boost/json.hpp>
#include "Reflective_traits.h"

template<typename Type>
constexpr bool assign_bool(boost::json::object& a_object, const std::string_view a_memberName, const Type& a_value)
{
	bool bRet = false;
	if constexpr (std::is_same_v<Type, bool>)
	{
		bRet = true;
		a_object[a_memberName] = a_value;
	}
	else if constexpr (std::is_std_optional<bool>)
	{
		bRet = true;
		if (a_value.has_value())
			a_object[a_memberName] = a_value.value();
	}
	return bRet;
}

template<typename Type>
constexpr bool assign_double(boost::json::object& a_object, const std::string_view a_memberName, const Type& a_value)
{
	bool bRet = false;
	if constexpr (std::is_same_v<Type, double>)
	{
		bRet = true;
		a_object[a_memberName] = a_value;
	}
	else if constexpr (std::is_same_v<Type, float>)
	{
		bRet = true;
		a_object[a_memberName] = static_cast<double>(a_value);
	}
	else if constexpr (std::is_std_optional_v<double>)
	{
		bRet = true;
		if (a_value.has_value())
			a_object[a_memberName] = a_value.value();
	}
	else if constexpr (std::is_std_optional<float>)
	{
		bRet = true;
		if (a_value.has_value())
			a_object[a_memberName] = static_cast<double>(a_value.value());
	}
	return bRet;
}

template<typename Type>
constexpr bool assign_uint(boost::json::object& a_object, const std::string_view a_memberName, const Type& a_value)
{
	bool bRet = false;
	if constexpr (std::is_same_v<Type, uint64_t>)
	{
		bRet = true;
		a_object[a_memberName] = a_value;
	}
	else if constexpr (std::is_same_v<Type, unsigned int>)
	{
		bRet = true;
		a_object[a_memberName] = static_cast<uint64_t>(a_value);
	}
	else if constexpr (std::is_same_v<Type, unsigned long>)
	{
		bRet = true;
		a_object[a_memberName] = static_cast<uint64_t>(a_value);
	}
	else if constexpr (std::is_same_v<Type, unsigned long long>)
	{
		bRet = true;
		a_object[a_memberName] = static_cast<uint64_t>(a_value);
	}
	else if constexpr (std::is_std_optional_v<uint64_t>)
	{
		bRet = true;
		if (a_value.has_value())
			a_object[a_memberName] = a_value.value();
	}
	else if constexpr (std::is_std_optional<unsigned int>)
	{
		bRet = true;
		if (a_value.has_value())
			a_object[a_memberName] = static_cast<uint64_t>(a_value.value());
	}
	else if constexpr (std::is_std_optional_v<unsigned long>)
	{
		bRet = true;
		if (a_value.has_value())
			a_object[a_memberName] = static_cast<uint64_t>(a_value.value());
	}
	else if constexpr (std::is_std_optional<unsigned long long>)
	{
		bRet = true;
		if (a_value.has_value())
			a_object[a_memberName] = static_cast<uint64_t>(a_value.value());
	}
	return bRet;
}

template<typename Type>
constexpr bool assign_int(boost::json::object& a_object, const std::string_view a_memberName, const Type& a_value)
{
	bool bRet = false;
	if constexpr (std::is_same_v<Type, int64_t>)
	{
		bRet = true;
		a_object[a_memberName] = a_value;
	}
	else if constexpr (std::is_same_v<Type, int>)
	{
		bRet = true;
		a_object[a_memberName] = static_cast<int64_t>(a_value);
	}
	else if constexpr (std::is_same_v<Type, long>)
	{
		bRet = true;
		a_object[a_memberName] = static_cast<int64_t>(a_value);
	}
	else if constexpr (std::is_same_v<Type, long long>)
	{
		bRet = true;
		a_object[a_memberName] = static_cast<int64_t>(a_value);
	}
	else if constexpr (std::is_std_optional_v<int64_t>)
	{
		bRet = true;
		if (a_value.has_value())
			a_object[a_memberName] = a_value.value();
	}
	else if constexpr (std::is_std_optional<int>)
	{
		bRet = true;
		if (a_value.has_value())
			a_object[a_memberName] = static_cast<int64_t>(a_value.value());
	}
	else if constexpr (std::is_std_optional_v<long>)
	{
		bRet = true;
		if (a_value.has_value())
			a_object[a_memberName] = static_cast<int64_t>(a_value.value());
	}
	else if constexpr (std::is_std_optional<long long>)
	{
		bRet = true;
		if (a_value.has_value())
			a_object[a_memberName] = static_cast<int64_t>(a_value.value());
	}
	return bRet;
}

template<typename Type>
constexpr bool assign_string(boost::json::object& a_object, const std::string_view a_memberName, const Type& a_value)
{
	bool bRet = false;
	if constexpr (is_convertible<std::string, Type>)
	{
		bRet = true;
		a_object[a_memberName] = convert<std::string>(a_value);
	}
	else if constexpr (std::is_same_v<Type, std::string>)
	{
		bRet = true;
		a_object[a_memberName] = a_value;
	}
	else if constexpr (is_std_optional<std::string>)
	{
		bRet = true;
		if(a_value.has_value())
			m_data = a_value.value();
	}
	return bRet;
}

template<typename Type>
constexpr bool assign_object(boost::json::object& a_object, const std::string_view a_memberName, const Type& a_value)
{
	bool bRet = false;
	if constexpr (is_reflective_v<Type>)
	{
		//
	}
	else if (is_optional_reflective_v<Type>)
	{
		//
	}
	return false;
}

template<typename Type>
constexpr bool assign_array(boost::json::object& a_object, const std::string_view a_memberName, const Type& a_value)
{
	bool bRet = false;
	if constexpr (is_std_array_v<Type> || is_std_vector_v<Type> || is_std_list_v<Type>)
	{
		if (a_value.empty())
			return true;

		for (const auto& item : a_value)
		{
			// todo
		}
	}
	return bRet;
}

