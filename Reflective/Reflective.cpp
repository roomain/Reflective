// Reflective.cpp : Définit les fonctions de la bibliothèque statique.
//

#include "pch.h"
#include <fstream>
#include <exception>
#include "Reflective.h"

LogCallback Reflective::sLogCallback;

void Reflective::setLogCallback(LogCallback a_callback)
{
	sLogCallback = a_callback;
}

void Reflective::internalLog(const std::source_location& a_loc, const std::string_view a_message)
{
	if (sLogCallback)
		sLogCallback(a_loc, a_message);
}

Reflective& Reflective::instance()
{
	static Reflective instance;
	return instance;
}


bool Reflective::loadFile(const std::string& a_file)
{
	m_reflectProfiles.clear();
	std::ifstream jsonFile(a_file);
	boost::system::error_code erroCode;
	auto jsonRoot = boost::json::parse(jsonFile, erroCode);
	if (jsonRoot == nullptr)
	{
		Reflective::internalLog(std::source_location::current(), erroCode.message());
		return false;
	}

	if (auto classArray = jsonRoot.as_object().find(Reflective::CLASS_ARRAY))
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
			Reflective::internalLog(std::source_location::current(), except.what());
			return false;
		}
	}

	return true;
}

void Reflective::setProfile(const std::string_view a_profile)
{
	m_currentProfile = a_profile;
}

Reflective::const_iterator Reflective::findProfile(const std::string_view a_profileName)const
{
	return std::ranges::find_if(m_reflectProfiles.cbegin(), m_reflectProfiles.cend(), [a_profileName](const auto& a_profile)
		{
			return a_profile.profile.compare(a_profileName) == 0;
		});
}


bool Reflective::hasProfile(const std::string_view a_profileName)const
{
	return findProfile(a_profileName) != cend();
}


void Reflective::readProfile(const boost::json::object& a_profile)
{
	ProfileData reflectProfile;
	for (const auto& node : a_profile)
	{
		if (node.key() == Reflective::PROFILE_NODE)
		{
			const auto object = node.value().as_object();
			reflectProfile.profile = object.at(Reflective::PROFILE_NAME_ATT).as_string();

			if(auto parentValue = object.at(Reflective::PROFILE_PARENT_ATT); !parentValue.is_null())
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