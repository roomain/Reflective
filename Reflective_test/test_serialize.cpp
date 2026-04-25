#include "pch.h"
#include "CppUnitTest.h"
#include "Reflective.h"
#include "test_structs.h"
#include <filesystem>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace Reflectivetest
{
	TEST_CLASS(Serialization)
	{
	public:
		TEST_METHOD_INITIALIZE(init)
		{
			Reflective::instance().clear();
		}

		TEST_METHOD_CLEANUP(cleanup)
		{
			Reflective::instance().clear();
		}

		TEST_METHOD(Test_simpleWrite)
		{
			TestDefault defStruct;
			Reflective::instance().writeProfile("Test_simpleWrite", defStruct);
			Reflective::instance().writeFile(R"(..\..\Test_data\tests_serialize.json)", true);
		}
	};
}