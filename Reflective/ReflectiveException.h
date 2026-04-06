#pragma once
/***********************************************
* @headerfile ReflectiveException.h
* @date 06 / 04 / 2026
* @author Roomain
************************************************/
#include <exception>
#include <string>
#include <format>
#include <source_location>
#include <boost/type_index.hpp>


template <typename T>
std::string type_name() 
{
	return boost::typeindex::type_id_with_cvr<T>().pretty_name();
}


/*@brief Reflective parsing exception*/
class ReflectiveException : public std::exception 
{
private:
	std::source_location m_location;	/*!< location of exception*/
	std::string m_message;				/*!< message of exception*/

public:
	ReflectiveException(const std::source_location& a_location, const std::string& a_message) :
		m_location(a_location),
		m_message(a_message)
	{}
	virtual const char* what() const noexcept override
	{
		return m_message.c_str();
	}
	const std::source_location& getLocation() const
	{
		return m_location;
	}

	template<typename Type, typename Expected>
	static ReflectiveException wrongType(const std::source_location& a_location)
	{
		return ReflectiveException(a_location, std::format("Can't assign {} into {}", type_name<Type>(), type_name<Expected>()));
	}

	static ReflectiveException incompatibleSize(const std::source_location& a_location, const size_t& a_size, const size_t& a_expectedSize)
	{
		return ReflectiveException(a_location, std::format("Incompatible array size {} for {}", a_size, a_expectedSize));
	}
};