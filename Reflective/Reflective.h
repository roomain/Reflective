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
#include "ReflectiveData.h"


using LogCallback = std::function<void(const std::source_location&, const std::string_view)>;

/*@brief singleton for json deserialisation*/
class Reflective
{
private:
	std::string m_currentProfile;										/*!< current profile*/
	static LogCallback sLogCallback;
	static void internalLog(const std::source_location& a_loc, const std::string_view a_message);
	
	ReflectiveJsonFileData m_fileData;
	
	Reflective() = default;

public:
	virtual ~Reflective() = default;
	static Reflective& instance();
	static void setLogCallback(LogCallback a_callback);
	bool loadFile(const std::string& a_file);
	bool writeFile(const std::string& a_file, const bool a_ident = false)const;
	void setCurrentProfile(const std::string_view a_profile);
	bool hasProfile(const std::string_view a_profile)const { return m_fileData.hasProfile(a_profile); }
	void clear() { m_fileData.clear(); }
	template<typename Object> requires is_reflective_v<Object>
	bool deserialize(const std::string& a_classname, Object& a_object)const
	{
		auto profileStack = m_fileData.parentProfiles(m_currentProfile, a_classname, sLogCallback);
		if (profileStack.empty())
			return false;

		while(!profileStack.empty())
		{
			m_fileData.deserialize(profileStack.top()->m_classes.at(a_classname), a_object, Object::s_reflectiveCtx);
			profileStack.pop();
		}
		return true;
	}
	bool setParent(const std::string_view a_profile, const std::string_view a_parent);
	
	template<typename Object> requires is_reflective_v<Object>
	void writeProfile(const std::string_view a_profile, const Object& a_data)
	{
		m_fileData.writeProfile(a_profile, a_data);
	}
};