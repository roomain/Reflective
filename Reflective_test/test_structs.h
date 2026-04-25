#pragma once
#include <string>
#include <vector>
#include <string_view>
#include <optional>
#include <ranges>
#include "Reflective_macros.h"
#include "test_structs.generated.h"

REFLECT_CLASS
struct TestStructIntern
{
	bool m_bool;
	int m_int;
	std::string m_string;
	std::vector<float> m_floatVector;

	REFLECT_DEFINE(TestStructIntern)

};

enum class EnumTest
{
	Enum0,
	Enum1,
	Enum2
};

template<>
constexpr EnumTest convert<EnumTest, std::string_view>(const std::string_view& data)
{
	EnumTest converted = EnumTest::Enum0;
	if (data == "Enum0")
	{
		return EnumTest::Enum0;
	}
	if (data == "Enum1")
	{
		return EnumTest::Enum1;
	}
	if (data == "Enum2")
	{
		return EnumTest::Enum2;
	}
	return converted;
}

REFLECT_CLASS
struct TestEnum
{
	EnumTest m_enum;
	int m_value;
	REFLECT_DEFINE(TestEnum)
};

REFLECT_CLASS
struct TestNested
{
	TestEnum m_enumStruct;
	int m_value;
	REFLECT_DEFINE(TestNested)
};

REFLECT_CLASS
struct TestLegacy : TestEnum
{
	int m_valueLegacy;
	REFLECT_DEFINE(TestLegacy)
};


REFLECT_CLASS
struct TestOptional
{
	std::optional<int> m_int;
	std::optional<std::string> m_str;
	REFLECT_DEFINE(TestOptional)
};

REFLECT_CLASS
struct TestDefault
{
	int m_int = 5;
	std::string m_str = "default";
	REFLECT_DEFINE(TestDefault)
};


constexpr std::vector<std::string> split(const std::string_view a_entry, const char a_separator)
{
	std::string temp (a_entry);
	std::vector<std::string> splitted;
	while (!temp.empty())
	{
		if (auto iter = std::ranges::find(temp, a_separator); iter != temp.cend())
		{
			const size_t index = iter - temp.cbegin();
			splitted.emplace_back(temp.substr(0, iter - temp.cbegin()));
			temp = temp.substr(index + 1, temp.size() - index - 1);
		}
		else
		{
			splitted.emplace_back(temp);
			temp = "";
		}
	}
	return splitted;
}

template<>
constexpr unsigned int convert<unsigned int, std::string_view>(const std::string_view& data)
{
	int value = 0;
	auto flagList = split(data, '|');
	for (const auto& flag : flagList)
	{
		if (flag == "FLAG_1")
		{
			value = value | (1 << 1);
		}
		else if (flag == "FLAG_2")
		{
			value = value | (1 << 2);
		}
		else if (flag == "FLAG_3")
		{
			value = value | (1 << 3);
		}
		else if (flag == "FLAG_4")
		{
			value = value | (1 << 4);
		}
		else if (flag == "FLAG_5")
		{
			value = value | (1 << 5);
		}
		else
		{
			//todo throw exception
		}
	}
	return value;
}

REFLECT_CLASS
struct TestFlag
{
	unsigned int m_flag = 0;
	REFLECT_DEFINE(TestFlag)
};

// test for vulkan flag
typedef enum VkQueueFlagBits {
	VK_QUEUE_GRAPHICS_BIT = 0x00000001,
	VK_QUEUE_COMPUTE_BIT = 0x00000002,
	VK_QUEUE_TRANSFER_BIT = 0x00000004,
	VK_QUEUE_SPARSE_BINDING_BIT = 0x00000008,
	VK_QUEUE_PROTECTED_BIT = 0x00000010,
	VK_QUEUE_VIDEO_DECODE_BIT_KHR = 0x00000020,
	VK_QUEUE_VIDEO_ENCODE_BIT_KHR = 0x00000040,
	VK_QUEUE_OPTICAL_FLOW_BIT_NV = 0x00000100,
	VK_QUEUE_DATA_GRAPH_BIT_ARM = 0x00000400,
	VK_QUEUE_FLAG_BITS_MAX_ENUM = 0x7FFFFFFF
} VkQueueFlagBits;

constexpr VkQueueFlagBits operator | (const  VkQueueFlagBits a_flag0, const  VkQueueFlagBits a_flag1)
{
	return static_cast<VkQueueFlagBits>(static_cast<unsigned int>(a_flag0) | static_cast<unsigned int>(a_flag1));
}

template<>
constexpr VkQueueFlagBits convert<VkQueueFlagBits, std::string_view>(const std::string_view& data)
{
	VkQueueFlagBits value = static_cast<VkQueueFlagBits>(0);
	auto flagList = split(data, '|');
	for (const auto& flag : flagList)
	{
		if (flag == "VK_QUEUE_GRAPHICS_BIT")
		{
			value = value | VK_QUEUE_GRAPHICS_BIT;
		}
		else if (flag == "VK_QUEUE_COMPUTE_BIT")
		{
			value = value | VK_QUEUE_COMPUTE_BIT;
		}
		else if (flag == "VK_QUEUE_TRANSFER_BIT")
		{
			value = value | VK_QUEUE_TRANSFER_BIT;
		}
		else if (flag == "VK_QUEUE_SPARSE_BINDING_BIT")
		{
			value = value | VK_QUEUE_SPARSE_BINDING_BIT;
		}
		else if (flag == "VK_QUEUE_PROTECTED_BIT")
		{
			value = value | VK_QUEUE_PROTECTED_BIT;
		}
		else if (flag == "VK_QUEUE_VIDEO_DECODE_BIT_KHR")
		{
			value = value | VK_QUEUE_VIDEO_DECODE_BIT_KHR;
		}
		else if (flag == "VK_QUEUE_VIDEO_ENCODE_BIT_KHR")
		{
			value = value | VK_QUEUE_VIDEO_ENCODE_BIT_KHR;
		}
		else if (flag == "VK_QUEUE_OPTICAL_FLOW_BIT_NV")
		{
			value = value | VK_QUEUE_OPTICAL_FLOW_BIT_NV;
		}
		else if (flag == "VK_QUEUE_DATA_GRAPH_BIT_ARM")
		{
			value = value | VK_QUEUE_DATA_GRAPH_BIT_ARM;
		}
		else
		{
			//todo throw exception
		}
	}
	return value;
}

REFLECT_CLASS
struct TestVulkanFlag
{
	VkQueueFlagBits m_flag = static_cast<VkQueueFlagBits>(0);
	REFLECT_DEFINE(TestVulkanFlag)
};

REFLECT_CLASS
struct TestOptionalVulkanFlag
{
	std::optional<TestVulkanFlag> m_opt;
	REFLECT_DEFINE(TestOptionalVulkanFlag)
};
