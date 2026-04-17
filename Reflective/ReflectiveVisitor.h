#pragma once
/***********************************************
* @headerfile ReflectiveVisitor.h
* @date 06 / 04 / 2026
* @author Roomain
************************************************/
#include <type_traits>
#include <functional>
#include <string_view>
#include <boost/json.hpp>
#include <boost/json/visit.hpp>
#include <boost/json/array.hpp>
#include "ReflectiveException.h"
#include "Reflective_traits.h"

class ReflectiveJsonFileData;

template<typename Type>
struct ReflectiveVisitor
{
	Type& m_data;													/*!< reference to data to fill*/
	std::reference_wrapper<const ReflectiveJsonFileData> m_reflectData;

	void operator()(const boost::json::object& a_value) const
	{
		if constexpr (is_reflective_v<Type>)
		{
			m_reflectData.get().deserialize(a_value, m_data, Type::s_reflectiveCtx);
		}
		else
		{
			throw ReflectiveException::typeNotReflective<Type>(std::source_location::current());
		}
	}

	void operator()(const boost::json::array& a_value) const
	{
		if constexpr (is_std_vector_v<Type>)
		{
			for (const auto& item : a_value)
				boost::json::visit(ReflectiveVisitor<std::remove_cvref_t<typename Type::value_type>>(m_data.emplace_back(), m_reflectData), item);

		}
		else if constexpr (is_std_list_v<Type>)
		{
			for (const auto& item : a_value)
				boost::json::visit(ReflectiveVisitor<std::remove_cvref_t<typename Type::value_type>>(m_data.emplace(), m_reflectData), item);
		}
		else if constexpr (is_std_array_v<Type>)
		{
			if (a_value.size() != m_data.size())
			{
				int index = 0;
				for(auto& arrayData : m_data)
				{
					boost::json::visit(ReflectiveVisitor<std::remove_cvref_t<typename Type::value_type>>(arrayData, m_reflectData), a_value.at(index));
					++index;
				}
			}
			else
			{
				throw ReflectiveException::incompatibleSize(std::source_location::current(), a_value.size(), m_data.size());
			}
		}
		else
		{
			throw ReflectiveException::wrongType<Type, boost::json::array>(std::source_location::current());
		}
	}

	void operator()(const boost::json::string& a_value) const
	{
		if constexpr (is_convertible<Type, std::string_view>)
		{
			std::string_view strView(a_value.data(), a_value.size());
			m_data = convert<Type>(strView);
		}
		else if constexpr (std::is_same_v<Type, std::string>)
		{
			// because std::is_trivially_assignable_v<std::string&, std::string> is false
			m_data = a_value;
		}
		else if constexpr (is_std_optional_same_v<Type, std::string>)
		{
			// because std::is_trivially_assignable_v<std::string&, std::string> is false
			m_data = static_cast<Type::value_type>(a_value);
		}
		else if constexpr (std::is_trivially_assignable_v<Type&, boost::json::string>)
		{
			m_data = a_value;
		}
		else
		{
			throw ReflectiveException::wrongType<Type, boost::json::string>(std::source_location::current());
		}
	}

	void operator()(const std::int64_t& a_value) const
	{
		if constexpr (std::is_same_v<Type, unsigned int>)
		{
			if (a_value >= 0)
			{
				m_data = static_cast<unsigned int>(a_value);
			}
			else
			{
				throw ReflectiveException::wrongType<Type, std::int64_t>(std::source_location::current());
			}
		}
		else if constexpr (std::is_trivially_assignable_v<Type&, std::int64_t>)
		{
			m_data = static_cast<Type>(a_value);
		}
		else if constexpr (is_std_optional_assignable_v<Type, std::int64_t>)
		{
			m_data = static_cast<Type::value_type>(a_value);
		}
		else
		{
			throw ReflectiveException::wrongType<Type, std::int64_t>(std::source_location::current());
		}
	}

	void operator()(const std::uint64_t& a_value) const
	{
		if constexpr (std::is_same_v<Type, std::uint64_t>)
		{
			m_data = a_value;
		}
		else if constexpr (std::is_trivially_assignable_v<Type&, std::uint64_t>)
		{
			m_data = static_cast<Type>(a_value);
		}
		else if constexpr (is_std_optional_assignable_v<Type, std::uint64_t>)
		{
			m_data = static_cast<Type::value_type>(a_value);
		}
		else
		{
			throw ReflectiveException::wrongType<Type, std::uint64_t>(std::source_location::current());
		}
	}


	void operator()(const double& a_value) const
	{
		if constexpr (std::is_same_v<Type, double>)
		{
			m_data = a_value;
		}
		else if constexpr (std::is_trivially_assignable_v<Type&, double>)
		{
			m_data = static_cast<Type>(a_value);
		}
		else if constexpr (is_std_optional_assignable_v<Type, double>)
		{
			m_data = static_cast<Type::value_type>(a_value);
		}
		else
		{
			throw ReflectiveException::wrongType<Type, double>(std::source_location::current());
		}
	}

	void operator()(const bool a_value) const
	{
		if constexpr (std::is_same_v<Type, bool>)
		{
			m_data = a_value;
		}
		else if constexpr (is_std_optional_assignable_v<Type, bool>)
		{
			m_data = static_cast<Type::value_type>(a_value);
		}
		else
		{
			throw ReflectiveException::wrongType<Type, bool>(std::source_location::current());
		}
	}

	void operator()(std::nullptr_t) const
	{
		throw ReflectiveException::wrongType<Type, std::nullptr_t>(std::source_location::current());
	}
};