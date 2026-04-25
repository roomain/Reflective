#include "pch.h"
#include <fstream>
#include <format>
#include "ReflectiveData.h"

void pretty_print(const boost::json::value& jv, std::ostream& os, int indent = 0) {
	const std::string indent_str(indent, ' ');

	if (jv.is_object()) {
		os << "{\n";
		const auto& obj = jv.as_object();
		bool first = true;
		for (const auto& [key, val] : obj) {
			if (!first) os << ",\n";
			os << std::string(indent + 2, ' ') << "\"" << key << "\": ";
			pretty_print(val, os, indent + 2);
			first = false;
		}
		os << "\n" << indent_str << "}";
	}
	else if (jv.is_array()) {
		os << "[\n";
		const auto& arr = jv.as_array();
		for (size_t i = 0; i < arr.size(); ++i) {
			os << std::string(indent + 2, ' ');
			pretty_print(arr[i], os, indent + 2);
			if (i < arr.size() - 1) os << ",";
			os << "\n";
		}
		os << indent_str << "]";
	}
	else {
		os << boost::json::serialize(jv);
	}
}

bool ReflectiveJsonFileData::loadFile(const std::string& a_file, LogCallback a_logCallback)
{
	m_reflectProfiles.clear();
	std::ifstream jsonFile(a_file);
	boost::system::error_code erroCode;
	auto jsonRoot = boost::json::parse(jsonFile, erroCode);
	if (jsonRoot == nullptr)
	{
		if (a_logCallback)
			a_logCallback(std::source_location::current(), erroCode.message());
		return false;
	}

	if (auto classArray = jsonRoot.as_object().find(ReflectiveJsonFileData::CLASS_ARRAY))
	{
		try
		{
			for (const auto& nodeProfile : classArray->value().as_array())
			{
				readProfile(nodeProfile.as_object());
			}
		}
		catch (std::exception& except)
		{
			if(a_logCallback)
				a_logCallback(std::source_location::current(), except.what());
			return false;
		}
	}

	return true;
}

bool ReflectiveJsonFileData::writeFile(const std::string& a_file, const bool a_ident)const
{
	std::ofstream jsonFile(a_file);
	if (!jsonFile)
		return false;

	boost::json::object root;
	root["$shema"] = "not_yet_rated";
	root["title"] = "Reflective";
	root["include"] = boost::json::array();
	boost::json::array profiles;
	for (auto& profile : m_reflectProfiles)
	{
		boost::json::object profileObj;

		boost::json::object idObj;
		idObj[PROFILE_NAME_ATT] = profile.profile;

		if(profile.parent.empty())
			idObj[PROFILE_PARENT_ATT] = nullptr;
		else
			idObj[PROFILE_PARENT_ATT] = profile.parent;

		profileObj[PROFILE_NODE] = idObj;

		for (auto& [name, jsonStruct] : profile.m_classes)
		{
			boost::json::object current;
			profileObj[name] = jsonStruct;
		}
		profiles.emplace_back(profileObj);
	}
	root[CLASS_ARRAY] = profiles;

	if (a_ident)
	{
		pretty_print(root, jsonFile, 4);
	}
	else
	{
		jsonFile << boost::json::serialize(root);
	}
	jsonFile.close();

	return true;
}

void ReflectiveJsonFileData::readProfile(const boost::json::object& a_profile)
{
	JsonReflectiveProfileData reflectProfile;
	for (const auto& node : a_profile)
	{
		if (node.key() == ReflectiveJsonFileData::PROFILE_NODE)
		{
			const auto object = node.value().as_object();
			reflectProfile.profile = object.at(ReflectiveJsonFileData::PROFILE_NAME_ATT).as_string();

			if (auto parentValue = object.at(ReflectiveJsonFileData::PROFILE_PARENT_ATT); !parentValue.is_null())
				reflectProfile.parent = parentValue.as_string();
		}
		else
		{
			reflectProfile.m_classes[node.key()] = node.value().as_object();
		}
	}

	if (!reflectProfile.profile.empty())
	{
		m_reflectProfiles.emplace_back(std::move(reflectProfile));
	}

}

ReflectiveJsonFileData::const_iterator ReflectiveJsonFileData::findProfile(const std::string_view a_profileName)const
{
	return std::ranges::find_if(m_reflectProfiles, [a_profileName](const auto& a_profile)
		{
			return a_profile.profile.compare(a_profileName) == 0;
		});
}

bool ReflectiveJsonFileData::hasProfile(const std::string_view a_profileName)const
{
	return findProfile(a_profileName) != cend();
}

std::stack<ReflectiveJsonFileData::const_iterator> ReflectiveJsonFileData::parentProfiles(const std::string& a_profile, const std::string& a_classname, LogCallback a_logCallback)const
{
	std::stack<ReflectiveJsonFileData::const_iterator> profileStack;
	auto iter = ReflectiveJsonFileData::findProfile(a_profile);
	if (iter == m_reflectProfiles.cend())
	{
		if(a_logCallback)
			a_logCallback(std::source_location::current(),
				std::format("Profile {} not found", a_profile));
		return profileStack;
	}
	else if (!iter->m_classes.contains(a_classname))
	{
		if (a_logCallback)
			a_logCallback(std::source_location::current(),
				std::format("Class {} in profile {} not found", a_classname, a_profile));
		return profileStack;
	}

	// Get stack of parent profile
	auto stackAdd = [&a_classname](std::stack<ReflectiveJsonFileData::const_iterator>& stack, ReflectiveJsonFileData::const_iterator a_iter)
		{
			if (auto reflect = a_iter->m_classes.find(a_classname); reflect != a_iter->m_classes.cend())
			{
				stack.push(a_iter);
			}
		};

	stackAdd(profileStack, iter);
	iter = ReflectiveJsonFileData::findProfile(iter->parent);
	while (iter != cend())
	{
		stackAdd(profileStack, iter);
		iter = ReflectiveJsonFileData::findProfile(iter->parent);
	}
	return profileStack;
}

void ReflectiveJsonFileData::setParentProfile(const std::string_view a_profile, const std::string_view a_parent)
{
	auto iter = std::ranges::find_if(m_reflectProfiles, [a_profile](const auto& a_profileJson)
		{
			return a_profileJson.profile.compare(a_profile) == 0;
		});
	iter->parent = std::string(a_parent);
}