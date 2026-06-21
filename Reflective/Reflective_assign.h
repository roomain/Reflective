#pragma once
/***********************************************
* @headerfile Reflective_assign.h
* @date 22 / 04 / 2026
* @author Roomain
************************************************/
#include <boost/json.hpp>
#include "Reflective_traits.h"

template<typename Type>
constexpr bool assign_bool(boost::json::value& a_jsonValue, const Type& a_value)
{
	bool bRet = false;
	if constexpr (std::is_same_v<Type, bool>)
	{
		bRet = true;
		a_jsonValue = a_value;
	}
	else if constexpr (is_std_optional_v<bool>)
	{
		bRet = true;
		if (a_value.has_value())
			a_jsonValue = a_value.value();
	}
	return bRet;
}

template<typename Type>
constexpr bool assign_double(boost::json::value& a_jsonValue, const Type& a_value)
{
	bool bRet = false;
	if constexpr (std::is_same_v<Type, double>)
	{
		bRet = true;
		a_jsonValue = a_value;
	}
	else if constexpr (std::is_same_v<Type, float>)
	{
		bRet = true;
		a_jsonValue = static_cast<double>(a_value);
	}
	else if constexpr (is_std_optional_v<double>)
	{
		bRet = true;
		if (a_value.has_value())
			a_jsonValue = a_value.value();
	}
	else if constexpr (is_std_optional_v<float>)
	{
		bRet = true;
		if (a_value.has_value())
			a_jsonValue = static_cast<double>(a_value.value());
	}
	return bRet;
}

template<typename Type>
constexpr bool assign_uint(boost::json::value& a_jsonValue, const Type& a_value)
{
	bool bRet = false;
	if constexpr (std::is_same_v<Type, uint64_t>)
	{
		bRet = true;
		a_jsonValue = a_value;
	}
	else if constexpr (std::is_same_v<Type, unsigned int> || std::is_same_v<Type, unsigned long> || std::is_same_v<Type, unsigned long long>)
	{
		bRet = true;
		a_jsonValue = static_cast<uint64_t>(a_value);
	}
	else if constexpr (is_std_optional_v<uint64_t>)
	{
		bRet = true;
		if (a_value.has_value())
			a_jsonValue = a_value.value();
	}
	else if constexpr (is_std_optional_v<unsigned int> || is_std_optional_v<unsigned long> || is_std_optional_v<unsigned long long>)
	{
		bRet = true;
		if (a_value.has_value())
			a_jsonValue = static_cast<uint64_t>(a_value.value());
	}
	return bRet;
}

template<typename Type>
constexpr bool assign_int(boost::json::value& a_jsonValue, const Type& a_value)
{
	bool bRet = false;
	if constexpr (std::is_same_v<Type, int64_t>)
	{
		bRet = true;
		a_jsonValue = a_value;
	}
	else if constexpr (std::is_same_v<Type, int> || std::is_same_v<Type, long> || std::is_same_v<Type, long long>)
	{
		bRet = true;
		a_jsonValue = static_cast<int64_t>(a_value);
	}
	else if constexpr (is_std_optional_v<int64_t>)
	{
		bRet = true;
		if (a_value.has_value())
			a_jsonValue = a_value.value();
	}
	else if constexpr (is_std_optional_v<int> || is_std_optional_v<long> || is_std_optional_v<long long>)
	{
		bRet = true;
		if (a_value.has_value())
			a_jsonValue = static_cast<int64_t>(a_value.value());
	}
	return bRet;
}

template<typename Type>
constexpr bool assign_string(boost::json::value& a_jsonValue, const Type& a_value)
{
	bool bRet = false;
	if constexpr (is_convertible<std::string, Type>)
	{
		bRet = true;
		a_jsonValue = convert<std::string>(a_value);
	}
	else if constexpr (std::is_same_v<Type, std::string>)
	{
		bRet = true;
		a_jsonValue = a_value;
	}
	else if constexpr (is_std_optional_v<std::string>)
	{
		bRet = true;
		if (a_value.has_value())
			a_jsonValue = a_value.value();
	}
	return bRet;
}

template<typename Type, typename Serializer>
constexpr bool fillArray(boost::json::array& a_array, const std::string_view a_memberName, const Type& a_value, Serializer* const a_reflective)
{
	if constexpr(is_reflective_v<typename Type::value_type> || is_optional_reflective_v<typename Type::value_type>)
	{
		for (const auto& item : a_value)
		{
			boost::json::object obj;
			a_reflective->serialize(obj, item, Type::value_type::s_reflectiveCtx);
			a_array.emplace_back(obj);
		}
	}
	else
	{
		for (const auto& item : a_value)
		{
			boost::json::value val;
			if (assign_bool(val, item)
				|| assign_double(val, item)
				|| assign_int(val, item)
				|| assign_uint(val, item)
				|| assign_string(val, item)
				|| assign_array(val, a_memberName, item, a_reflective))
			{
				a_array.emplace_back(val);
			}
			else
			{
				throw ReflectiveException::unsupportedData<Type>(std::source_location::current());
			}
		}
	}
	return true;
}

template<typename Type, typename Serializer>
constexpr bool assign_array(boost::json::value& a_jsonValue, const std::string_view a_memberName, const Type& a_value, Serializer* const a_reflective)
{
	bool bRet = false;
	if constexpr (is_std_array_v<Type> || is_std_vector_v<Type> || is_std_list_v<Type>)
	{
		if (a_value.empty())
			return true;

		boost::json::array array;
		if(fillArray(array, a_memberName, a_value,a_reflective))
			a_jsonValue = array;
	}
	return bRet;
}

//-----------------------------------------------------------------------------------------------------------------------

template<typename Type>
constexpr bool assign_bool(boost::json::object& a_object, const std::string_view a_memberName, const Type& a_value)
{
	bool bRet = false;
	if constexpr (std::is_same_v<Type, bool>)
	{
		bRet = true;
		a_object[a_memberName] = a_value;
	}
	else if constexpr (is_std_optional_v<bool>)
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
	else if constexpr (is_std_optional_v<double>)
	{
		bRet = true;
		if (a_value.has_value())
			a_object[a_memberName] = a_value.value();
	}
	else if constexpr (is_std_optional_v<float>)
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
	else if constexpr (std::is_same_v<Type, unsigned int> || std::is_same_v<Type, unsigned long> || std::is_same_v<Type, unsigned long long>)
	{
		bRet = true;
		a_object[a_memberName] = static_cast<uint64_t>(a_value);
	}
	else if constexpr (is_std_optional_v<uint64_t>)
	{
		bRet = true;
		if (a_value.has_value())
			a_object[a_memberName] = a_value.value();
	}
	else if constexpr (is_std_optional_v<unsigned int>)
	{
		bRet = true;
		if (a_value.has_value())
			a_object[a_memberName] = static_cast<uint64_t>(a_value.value());
	}
	else if constexpr (is_std_optional_v<unsigned long>)
	{
		bRet = true;
		if (a_value.has_value())
			a_object[a_memberName] = static_cast<uint64_t>(a_value.value());
	}
	else if constexpr (is_std_optional_v<unsigned long long>)
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
	else if constexpr (std::is_same_v<Type, int> || std::is_same_v<Type, long> || std::is_same_v<Type, long long>)
	{
		bRet = true;
		a_object[a_memberName] = static_cast<int64_t>(a_value);
	}
	else if constexpr (is_std_optional_v<int64_t>)
	{
		bRet = true;
		if (a_value.has_value())
			a_object[a_memberName] = a_value.value();
	}
	else if constexpr (is_std_optional_v<int>)
	{
		bRet = true;
		if (a_value.has_value())
			a_object[a_memberName] = static_cast<int64_t>(a_value.value());
	}
	else if constexpr (is_std_optional_v<long>)
	{
		bRet = true;
		if (a_value.has_value())
			a_object[a_memberName] = static_cast<int64_t>(a_value.value());
	}
	else if constexpr (is_std_optional_v<long long>)
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
	else if constexpr (is_std_optional_v<std::string>)
	{
		bRet = true;
		if(a_value.has_value())
			a_object[a_memberName] = a_value.value();
	}
	return bRet;
}

template<typename Type, typename Serializer>
constexpr bool assign_object(boost::json::object& a_object, const std::string_view a_memberName, const Type& a_value, Serializer* const a_reflective)
{
	bool bRet = false;
	if constexpr (is_reflective_v<Type>)
	{
		bRet = true;
		a_object[a_memberName] = boost::json::object();
		a_reflective->serialize(a_object[a_memberName].as_object(), a_value, Type::s_reflectiveCtx);
	}
	else if constexpr(is_optional_reflective_v<Type>)
	{
		bRet = true;
		if (a_value.has_value())
		{
			a_object[a_memberName] = boost::json::object();
			bRet = assign_object(a_object[a_memberName].as_object(), a_memberName, a_value.value(), a_reflective);
		}
	}
	return bRet;
}

template<typename Type, typename Serializer>
constexpr bool assign_array(boost::json::object& a_object, const std::string_view a_memberName, const Type& a_value, Serializer* const a_reflective)
{
	bool bRet = false;
	if constexpr (is_std_array_v<Type> || is_std_vector_v<Type> || is_std_list_v<Type>)
	{
		if (a_value.empty())
			return true;

		boost::json::array array;
		if (fillArray(array, a_memberName, a_value, a_reflective))
			a_object[a_memberName] = array;
	}
	return bRet;
}

