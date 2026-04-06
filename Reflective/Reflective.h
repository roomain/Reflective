#pragma once
/***********************************************
* @headerfile Reflective.h
* @date 05 / 04 / 2026
* @author Roomain
************************************************/
#include <string>
#include <string_view>
#include <functional>
#include <unordered_map>
#include <source_location>
#include <tuple>
#include <stack>
#include <boost/json.hpp>
#include <boost/json/visit.hpp>
#include "ReflectiveVisitor.h"


using LogCallback = std::function<void(const std::source_location&, const std::string_view)>;


struct ProfileData
{
	std::string profile;											/*!< profile */
	std::string parent;												/*!< parent profile */
	std::unordered_map<std::string, boost::json::object> m_classes;		/*!< serialized class*/
};

//template<typename Object, typename ...Args>
//class DeserializeCtx
//{
//private:
//	boost::json::object m_jsonObject;					/*!< json object to deserialize*/
//	std::reference_wrapper<ProfileData> m_data;			/*!< reference to profile*/
//	std::reference_wrapper<Object> m_objToDeserialize;	/*!< reference to profile*/
//};

/*@brief singleton for json deserialisation*/
class Reflective
{
private:
	std::string m_currentProfile;										/*!< current profile*/
	static LogCallback sLogCallback;
	static void internalLog(const std::source_location& a_loc, const std::string_view a_message);
	static constexpr std::string_view CLASS_ARRAY = "ClassArray";
	static constexpr std::string_view PROFILE_NODE = "Id";
	static constexpr std::string_view PROFILE_NAME_ATT = "name";
	static constexpr std::string_view PROFILE_PARENT_ATT = "parent";

	void readProfile(const boost::json::object& a_profile);


	std::vector<ProfileData> m_reflectProfiles;	/*!< all profiles*/
	
	Reflective() = default;

	template<typename Type>
	void deserialize(const boost::json::object& a_object, std::string_view a_memberName ,Type& a_data)const
	{
		if (auto iter = a_object.find(a_memberName); iter != a_object.cend())
		{
			boost::json::visit(ReflectiveVisitor<Type>(a_data), iter->value());
		}
	}
	
	template<typename Object, typename ...Args>
	void deserialize(const boost::json::object& a_jsonObject, Object& a_object, std::tuple<Args...>&& a_accessMembers)const
	{
		
		std::apply(
			[&a_jsonObject, &a_object, this](Args&&... tupleArgs)
			{
				(deserialize(a_jsonObject, tupleArgs.first, a_object.*(tupleArgs.second)), ...);
			}, std::forward<std::tuple<Args...>>(a_accessMembers));
	}

public:
	virtual ~Reflective() = default;
	static Reflective& instance();
	static void setLogCallback(LogCallback a_callback);
	bool loadFile(const std::string& a_file);
	void setProfile(const std::string_view a_profile);
	using const_iterator = std::vector<ProfileData>::const_iterator;
	constexpr const_iterator cbegin()const { return m_reflectProfiles.cbegin(); }
	constexpr const_iterator cend()const { return m_reflectProfiles.cend(); }
	const_iterator findProfile(const std::string_view a_profileName)const;
	bool hasProfile(const std::string_view a_profileName)const;
	template<typename Object, typename ...Args>
	bool deserialize(const std::string& a_classname, Object& a_object, Args&&... a_acessors)const
	{
		auto iter = Reflective::findProfile(m_currentProfile);
		if (iter == m_reflectProfiles.cend())
		{
			Reflective::internalLog(std::source_location::current(),
				std::format("Profile {} not found", m_currentProfile));
			return false;
		}
		else if (!iter->m_classes.contains(a_classname))
		{
			Reflective::internalLog(std::source_location::current(),
				std::format("Class {} in profile {} not found", a_classname, m_currentProfile));
			return false;
		}

		auto accessMembers = std::make_tuple(std::forward<Args>(a_acessors)...);

		// Get stack of parent profile
		auto stackAdd = [&a_classname](std::stack<Reflective::const_iterator>& stack, Reflective::const_iterator a_iter)
			{
				if (auto reflect = a_iter->m_classes.find(a_classname); reflect != a_iter->m_classes.cend())
				{
					stack.push(a_iter);
				}
			};


		std::stack<Reflective::const_iterator> profileStack;
		stackAdd(profileStack, iter);
		iter = Reflective::findProfile(iter->parent);
		while (iter != cend())
		{
			stackAdd(profileStack, iter);
			iter = Reflective::findProfile(iter->parent);
		}

		while(!profileStack.empty())
		{
			deserialize(profileStack.top()->m_classes.at(a_classname), a_object, std::forward<std::tuple<Args...>>(accessMembers));
			profileStack.pop();
		}
	}
};