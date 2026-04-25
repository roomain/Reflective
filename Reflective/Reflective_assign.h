#pragma once
/***********************************************
* @headerfile Reflective_assign.h
* @date 22 / 04 / 2026
* @author Roomain
************************************************/
#include <boost/json.hpp>
#include "Reflective_traits.h"

template<typename Type>
constexpr bool assign_bool(boost::json::value& a_jsonValue, const std::string_view a_memberName, const Type& a_value)
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
constexpr bool assign_double(boost::json::value& a_jsonValue, const std::string_view a_memberName, const Type& a_value)
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
constexpr bool assign_uint(boost::json::value& a_jsonValue, const std::string_view a_memberName, const Type& a_value)
{
	bool bRet = false;
	if constexpr (std::is_same_v<Type, uint64_t>)
	{
		bRet = true;
		a_jsonValue = a_value;
	}
	else if constexpr (std::is_same_v<Type, unsigned int>)
	{
		bRet = true;
		a_jsonValue = static_cast<uint64_t>(a_value);
	}
	else if constexpr (std::is_same_v<Type, unsigned long>)
	{
		bRet = true;
		a_jsonValue = static_cast<uint64_t>(a_value);
	}
	else if constexpr (std::is_same_v<Type, unsigned long long>)
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
	else if constexpr (is_std_optional_v<unsigned int>)
	{
		bRet = true;
		if (a_value.has_value())
			a_jsonValue = static_cast<uint64_t>(a_value.value());
	}
	else if constexpr (is_std_optional_v<unsigned long>)
	{
		bRet = true;
		if (a_value.has_value())
			a_jsonValue = static_cast<uint64_t>(a_value.value());
	}
	else if constexpr (is_std_optional_v<unsigned long long>)
	{
		bRet = true;
		if (a_value.has_value())
			a_jsonValue = static_cast<uint64_t>(a_value.value());
	}
	return bRet;
}

template<typename Type>
constexpr bool assign_int(boost::json::value& a_jsonValue, const std::string_view a_memberName, const Type& a_value)
{
	bool bRet = false;
	if constexpr (std::is_same_v<Type, int64_t>)
	{
		bRet = true;
		a_jsonValue = a_value;
	}
	else if constexpr (std::is_same_v<Type, int>)
	{
		bRet = true;
		a_jsonValue = static_cast<int64_t>(a_value);
	}
	else if constexpr (std::is_same_v<Type, long>)
	{
		bRet = true;
		a_jsonValue = static_cast<int64_t>(a_value);
	}
	else if constexpr (std::is_same_v<Type, long long>)
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
	else if constexpr (is_std_optional_v<int>)
	{
		bRet = true;
		if (a_value.has_value())
			a_jsonValue = static_cast<int64_t>(a_value.value());
	}
	else if constexpr (is_std_optional_v<long>)
	{
		bRet = true;
		if (a_value.has_value())
			a_jsonValue = static_cast<int64_t>(a_value.value());
	}
	else if constexpr (is_std_optional_v<long long>)
	{
		bRet = true;
		if (a_value.has_value())
			a_jsonValue = static_cast<int64_t>(a_value.value());
	}
	return bRet;
}

template<typename Type>
constexpr bool assign_string(boost::json::value& a_jsonValue, const std::string_view a_memberName, const Type& a_value)
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
constexpr bool assign_array(boost::json::value& a_jsonValue, const std::string_view a_memberName, const Type& a_value, Serializer* const a_reflective)
{
	bool bRet = false;
	if constexpr (is_std_array_v<Type> || is_std_vector_v<Type> || is_std_list_v<Type>)
	{
		if (a_value.empty())
			return true;

		boost::json::array array;
		if (is_reflective_v<Type> || is_optional_reflective_v<Type>)
		{
			for (const auto& item : a_value)
			{
				boost::json::object obj;
				a_reflective->serialize(obj, a_value, Type::s_reflectiveCtx);
				array.emplace_back(obj);
			}
			bRet = true;
		}
		else
		{
			for (const auto& item : a_value)
			{
				boost::json::value val;
				if (assign_object(val, a_memberName, item, this)
					|| assign_bool(val, a_memberName, item)
					|| assign_double(val, a_memberName, item)
					|| assign_int(val, a_memberName, item)
					|| assign_uint(val, a_memberName, item)
					|| assign_string(val, a_memberName, item)
					|| assign_array(val, a_memberName, item, this))
				{
					array.emplace_back(val);
				}
				else
				{
					throw ReflectiveException::unsupportedData<Type>(std::source_location::current());
				}
			}
			bRet = true;
		}

		if(bRet)
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
		a_reflective->serialize(a_object[a_memberName], a_value, Type::s_reflectiveCtx);
	}
	else if constexpr(is_optional_reflective_v<Type>)
	{
		bRet = true;
		if (a_value.has_value())
			bRet = assign_object(a_object, a_memberName, a_value.value(), a_reflective);
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
		if constexpr(is_reflective_v<Type> || is_optional_reflective_v<Type>)
		{
			for (const auto& item : a_value)
			{
				boost::json::object obj;
				a_reflective->serialize(obj, item, Type::s_reflectiveCtx);
				array.emplace_back(obj);
			}
			bRet = true;
		}
		else
		{
			for (const auto& item : a_value)
			{
				boost::json::value val;
				if (assign_object(val, a_memberName, item, this)
					|| assign_bool(val, a_memberName, item)
					|| assign_double(val, a_memberName, item)
					|| assign_int(val, a_memberName, item)
					|| assign_uint(val, a_memberName, item)
					|| assign_string(val, a_memberName, item)
					|| assign_array(val, a_memberName, item, this))
				{
					array.emplace_back(val);
				}
				else
				{
					throw ReflectiveException::unsupportedData<Type>(std::source_location::current());
				}
			}
			bRet = true;
		}

		if(bRet)
			a_object[a_memberName] = array;
	}
	return bRet;
}

