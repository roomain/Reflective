#include "pch.h"
#include <fstream>
#include <format>
#include "ReflectiveData.h"

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
	return std::ranges::find_if(m_reflectProfiles.cbegin(), m_reflectProfiles.cend(), [a_profileName](const auto& a_profile)
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