#pragma once
/***********************************************
* @headerfile ReflectiveData.h
* @date 08 / 04 / 2026
* @author Roomain
************************************************/
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <stack>
#include <tuple>
#include <boost/json.hpp>
#include "ReflectiveVisitor.h"

struct JsonReflectiveProfileData
{
	std::string profile;											/*!< profile */
	std::string parent;												/*!< parent profile */
	std::unordered_map<std::string, boost::json::object> m_classes;	/*!< serialized class*/
};

using LogCallback = std::function<void(const std::source_location&, const std::string_view)>;

class ReflectiveJsonFileData
{	
private:

	static constexpr std::string_view CLASS_ARRAY = "ClassArray";
	static constexpr std::string_view PROFILE_NODE = "Id";
	static constexpr std::string_view PROFILE_NAME_ATT = "name";
	static constexpr std::string_view PROFILE_PARENT_ATT = "parent";

	std::vector<JsonReflectiveProfileData> m_reflectProfiles;
	void readProfile(const boost::json::object& a_profile);

public:

	using const_iterator = std::vector<JsonReflectiveProfileData>::const_iterator;
	constexpr const_iterator cbegin()const { return m_reflectProfiles.cbegin(); }
	constexpr const_iterator cend()const { return m_reflectProfiles.cend(); }

	bool loadFile(const std::string& a_file, LogCallback a_logCallback);
	const_iterator findProfile(const std::string_view a_profileName)const;
	bool hasProfile(const std::string_view a_profileName)const;
	std::stack<const_iterator> parentProfiles(const std::string& a_profile, const std::string& a_classname, LogCallback a_logCallback)const;

	template<typename Type>
	void deserialize(const boost::json::object& a_json, const std::string_view a_memberName, Type& a_data)const
	{
		if (auto iter = a_json.find(a_memberName); iter != a_json.cend())
		{
			boost::json::visit(ReflectiveVisitor<Type>(a_data, *this), iter->value());
		}
	}

	template<typename Object, typename ...Args>
	void deserialize(const boost::json::object& a_jsonObject, Object& a_object, std::tuple<Args...>& a_accessMembers)const
	{
		std::apply(
			[&a_jsonObject, &a_object, this](Args&... tupleArgs)
			{
				(deserialize(a_jsonObject, tupleArgs.first, a_object.*(tupleArgs.second)), ...);
			}, a_accessMembers/*std::forward<std::tuple<Args...>>(a_accessMembers)*/);
	}
};