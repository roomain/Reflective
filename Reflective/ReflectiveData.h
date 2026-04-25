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
#include "Reflective_traits.h"
#include "ReflectiveVisitor.h"
#include "Reflective_assign.h"

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
	void clear() { m_reflectProfiles.clear(); }
	bool loadFile(const std::string& a_file, LogCallback a_logCallback);
	bool writeFile(const std::string& a_file, const bool a_ident = false)const;
	const_iterator findProfile(const std::string_view a_profileName)const;
	bool hasProfile(const std::string_view a_profileName)const;
	void setParentProfile(const std::string_view a_profile, const std::string_view a_parent);
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

	template<typename Type>
	constexpr bool serializeData(boost::json::object& a_json, const std::string_view a_memberName, const Type& a_data)
	{
		return assign_object(a_json, a_memberName, a_data, this)
			|| assign_bool(a_json, a_memberName, a_data)
			|| assign_double(a_json, a_memberName, a_data)
			|| assign_int(a_json, a_memberName, a_data)
			|| assign_uint(a_json, a_memberName, a_data)
			|| assign_string(a_json, a_memberName, a_data)
			|| assign_array(a_json, a_memberName, a_data, this);
	}

	template<typename Object, typename ...Args>
	void serialize(boost::json::object& a_jsonObject, Object& a_object, std::tuple<Args...>& a_accessMembers)
	{
		std::apply(
			[&a_jsonObject, &a_object, this](Args&... tupleArgs)
			{
				(serializeData(a_jsonObject, tupleArgs.first, a_object.*(tupleArgs.second)), ...);
			}, a_accessMembers/*std::forward<std::tuple<Args...>>(a_accessMembers)*/);
	}
	
	template<typename Object> requires is_reflective_v<Object>
	void writeProfile(const std::string_view a_profile, const Object& a_data)
	{
		auto iter = std::ranges::find_if(m_reflectProfiles, [&a_profile](const auto& a_profileData)
			{
				return a_profileData.profile.compare(a_profile) == 0;
			});
		if ( iter == m_reflectProfiles.cend())
		{
			JsonReflectiveProfileData profile{ .profile = std::string(a_profile) };
			m_reflectProfiles.emplace_back(std::move(profile));
			iter = m_reflectProfiles.end() - 1;
		}
		
		auto& object = iter->m_classes[Object::reflectiveName()];
		serialize(object, a_data, Object::s_reflectiveCtx);
	}
};