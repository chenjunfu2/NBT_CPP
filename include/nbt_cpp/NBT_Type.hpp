#pragma once

#include <stdint.h>
#include <stddef.h>//size_t
#include <variant>
#include <vector>
#include <unordered_map>
#include <string>
#include <type_traits>

#include "NBT_TAG.hpp"

class NBT_Node;
template <typename Array>
class NBT_Array;
template <typename String, typename StringView>
class NBT_String;
template <typename List>
class NBT_List;
template <typename Compound>
class NBT_Compound;

/// @brief 提供NBT类型定义，包括NBT格式中的所有数据类型，以及部分辅助功能，比如静态类型与Tag映射，类型存在查询，类型列表大小，类型最大小值等
class NBT_Type
{
	/// @brief 禁止构造
	NBT_Type(void) = delete;
	/// @brief 禁止析构
	~NBT_Type(void) = delete;

public:

	/// @name 原始数据类型定义
	/// @{
	using Float_Raw = uint32_t;///< Float 类型的原始表示，用于在平台不支持或需要二进制读写时使用
	using Double_Raw = uint64_t;///< Double 类型的原始表示，用于在平台不支持或需要二进制读写时使用
	/// @}

	/// @name NBT数据类型定义
	/// @{
 
	//内建类型
	//默认为无状态，与std::variant兼容
	using End			= std::monostate;	///< 结束标记类型，无数据
	using Byte			= int8_t;			///<  8位有符号整数
	using Short			= int16_t;			///< 16位有符号整数
	using Int			= int32_t;			///< 32位有符号整数
	using Long			= int64_t;			///< 64位有符号整数

	//浮点类型
	//通过编译期确认类型大小来选择正确的类型，优先浮点类型，如果失败则替换为对应的可用类型
	using Float			= std::conditional_t<(sizeof(float) == sizeof(Float_Raw)), float, Float_Raw>;		///< 单精度浮点类型，如果平台不支持，则替换为同等大小的原始数据类型
	using Double		= std::conditional_t<(sizeof(double) == sizeof(Double_Raw)), double, Double_Raw>;	///< 双精度浮点类型，如果平台不支持，则替换为同等大小的原始数据类型

	//数组类型，不存在SortArray，由于NBT标准不提供，所以此处也不提供
	using ByteArray		= NBT_Array<std::vector<Byte>>;	///< 存储 8位有符号整数的数组类型
	using IntArray		= NBT_Array<std::vector<Int>>;	///< 存储32位有符号整数的数组类型
	using LongArray		= NBT_Array<std::vector<Long>>;	///< 存储64位有符号整数的数组类型

	//字符串类型
	using String		= NBT_String<std::basic_string<uint8_t>, std::basic_string_view<uint8_t>>;	///< 字符串类型，存储Java M-UTF-8字符串

	//列表类型
	//存储一系列同类型标签的有效负载（无标签 ID 或名称），原先为list，因为mc内list也通过下标访问，所以改为vector模拟
	using List			= NBT_List<std::vector<NBT_Node>>;	///< 列表类型，可顺序存储任意相同的NBT类型

	//集合类型
	//挂在序列下的内容都通过map绑定名称
	using Compound		= NBT_Compound<std::unordered_map<String, NBT_Node>>;	///集合类型，可存储任意不同的NBT类型，通过名称映射值

	/// @}

	/// @brief 类型列表模板
	/// @tparam ...Ts 变长参数包
	template<typename... Ts>
	struct _TypeList{};

	/// @brief 完整的NBT类型列表定义
	using TypeList = _TypeList
	<
		End,
		Byte,
		Short,
		Int,
		Long,
		Float,
		Double,
		ByteArray,
		String,
		List,
		Compound,
		IntArray,
		LongArray
	>;

private:
	//通过类型Tag映射到字符串指针数组
	constexpr static inline const char *const cstrTypeName[] =
	{
		"End",
		"Byte",
		"Short",
		"Int",
		"Long",
		"Float",
		"Double",
		"ByteArray",
		"String",
		"List",
		"Compound",
		"IntArray",
		"LongArray",
	};

public:
	/// @brief 通过类型标签获取类型名称
	/// @param tag NBT_TAG枚举的类型标签
	/// @return 类型标签对应的名称的以0结尾的C风格字符串地址，如果类型标签不存在，则返回\"Unknown\"
	/// @note 返回的是全局静态字符串地址，请不要直接修改，需要修改请拷贝
	constexpr static inline const char *GetTypeName(NBT_TAG tag) noexcept//运行时类型判断，允许静态
	{
		if (tag >= NBT_TAG::ENUM_END)
		{
			return "Unknown";
		}

		NBT_TAG_RAW_TYPE tagRaw = (NBT_TAG_RAW_TYPE)tag;
		return cstrTypeName[tagRaw];
	}

	//部分结构的Length类型
	using ArrayLength = int32_t;
	using StringLength = uint16_t;
	using ListLength = int32_t;

	constexpr static inline ArrayLength ArrayLength_Max = INT32_MAX;
	constexpr static inline ArrayLength ArrayLength_Min = INT32_MIN;

	constexpr static inline StringLength StringLength_Max = UINT16_MAX;
	constexpr static inline StringLength StringLength_Min = 0;

	constexpr static inline ListLength ListLength_Max = INT32_MAX;
	constexpr static inline ListLength ListLength_Min = INT32_MIN;

	//内置整数类型上下限
	constexpr static inline Byte Byte_Max = INT8_MAX;
	constexpr static inline Byte Byte_Min = INT8_MIN;

	constexpr static inline Short Short_Max = INT16_MAX;
	constexpr static inline Short Short_Min = INT16_MIN;

	constexpr static inline Int Int_Max = INT32_MAX;
	constexpr static inline Int Int_Min = INT32_MIN;

	constexpr static inline Long Long_Max = INT64_MAX;
	constexpr static inline Long Long_Min = INT64_MIN;

	//类型存在检查
	template <typename T, typename List>
	struct IsValidType;

	template <typename T, typename... Ts>
	struct IsValidType<T, _TypeList<Ts...>> : std::bool_constant<(std::is_same_v<T, Ts> || ...)>
	{};

	template <typename T>
	static constexpr bool IsValidType_V = IsValidType<T, TypeList>::value;

	//类型索引查询
	template <typename T, typename... Ts>
	static consteval NBT_TAG_RAW_TYPE TypeTagHelper()//consteval必须编译期求值
	{
		NBT_TAG_RAW_TYPE tagIndex = 0;
		bool bFound = ((std::is_same_v<T, Ts> ? true : (++tagIndex, false)) || ...);
		return bFound ? tagIndex : (NBT_TAG_RAW_TYPE)-1;
	}

	template <typename T, typename List>
	struct TypeTagImpl;

	template <typename T, typename... Ts>
	struct TypeTagImpl<T, _TypeList<Ts...>>
	{
		static constexpr NBT_TAG_RAW_TYPE value = TypeTagHelper<T, Ts...>();
	};

	template <typename T>
	static constexpr NBT_TAG TypeTag_V = (NBT_TAG)TypeTagImpl<T, TypeList>::value;

	//类型列表大小
	template <typename List>
	struct TypeListSize;

	template <typename... Ts>
	struct TypeListSize<_TypeList<Ts...>>
	{
		static constexpr size_t value = sizeof...(Ts);
	};

	static constexpr size_t TypeListSize_V = TypeListSize<TypeList>::value;

	//enum与索引大小检查
	static_assert(TypeListSize_V == NBT_TAG::ENUM_END, "Enumeration does not match the number of types in the mutator");


	//从NBT_TAG到类型的映射
	template <NBT_TAG Tag>
	struct TagToType;

	template <NBT_TAG_RAW_TYPE I, typename List> struct TypeAt;

	template <NBT_TAG_RAW_TYPE I, typename... Ts>
	struct TypeAt<I, _TypeList<Ts...>>
	{
		using type = std::tuple_element_t<I, std::tuple<Ts...>>;
	};

	template <NBT_TAG Tag>
	struct TagToType
	{
		static_assert((NBT_TAG_RAW_TYPE)Tag < TypeListSize_V, "Invalid NBT_TAG");
		using type = typename TypeAt<(NBT_TAG_RAW_TYPE)Tag, TypeList>::type;
	};

	template <NBT_TAG Tag>
	using TagToType_T = typename TagToType<Tag>::type;

	//映射浮点数到方便读写的raw类型
	template<typename T>
	struct BuiltinRawType
	{
		using Type = T;
		static_assert(IsValidType_V<T> && std::is_integral_v<T>, "Not a legal type!");//抛出编译错误
	};

	template<typename T>
	using BuiltinRawType_T = typename BuiltinRawType<T>::Type;
};

//显示特化
template<>
struct NBT_Type::BuiltinRawType<NBT_Type::Float>//浮点数映射
{
	using Type = Float_Raw;
	static_assert(sizeof(Type) == sizeof(Float), "Type size does not match!");
};

template<>
struct NBT_Type::BuiltinRawType<NBT_Type::Double>//浮点数映射
{
	using Type = Double_Raw;
	static_assert(sizeof(Type) == sizeof(Double), "Type size does not match!");
};