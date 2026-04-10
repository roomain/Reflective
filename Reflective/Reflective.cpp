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

bool Reflective::loadFile(const std::string& a_file)
{
	return m_fileData.loadFile(a_file, sLogCallback);
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


void Reflective::setProfile(const std::string_view a_profile)
{
	m_currentProfile = a_profile;
}

