#pragma once

#include "NBT_Node.hpp"
#include "NBT_Print.hpp"

#include <stdint.h>

//提示性实现类（鸭子类型），仅用于模板通过性验证与用户接口提示
class NBT_Visitor
{
public:
	enum class ResultControl : uint8_t
	{
		Continue,	///< 继续处理（继续迭代）
		Break,		///< 跳过剩余值（离开当前结构层级回到父层级）
		Stop,		///< 停止处理（终止解析）
	};

	enum class NestingControl : uint8_t
	{
		Enter,	///< 进入当前值（递归进入嵌套结构或展开值）
		Skip,	///< 跳过当前值（跳过递归进入嵌套结构或展开值）
		Break,	///< 跳过剩余值（离开当前结构层级回到父层级）
		Stop,	///< 停止处理（终止解析）
	};

public:
	template<typename T>
	requires(NBT_Type::IsNumericType_V<T>)
	ResultControl VisitNumericResult(T tNumericResult)
	{
		//do something...
		return ResultControl::Continue;
	}

	template<typename T>
	requires(NBT_Type::IsArrayType_V<T> && !std::is_reference_v<T>)//防止引用折叠
	ResultControl VisitArrayResult(T &&tArrayResult)
	{
		//do something...
		return ResultControl::Continue;
	}

	ResultControl VisitStringResult(NBT_Type::String &&strResult)
	{
		//do something...
		return ResultControl::Continue;
	}

	ResultControl VisitEndResult(void)
	{
		//do something...
		return ResultControl::Continue;
	}

	NestingControl VisitListBegin(NBT_TAG tagElement, size_t szListLength)
	{
		//do something...
		return NestingControl::Enter;
	}

	NestingControl VisitListNextElement(NBT_TAG tagElement, size_t szListIndex)
	{
		//do something...
		return NestingControl::Enter;
	}

	ResultControl VisitListEnd(void)
	{
		//do something...
		return ResultControl::Continue;
	}

	NestingControl VisitCompoundBegin(void)
	{
		//do something...
		return NestingControl::Enter;
	}

	NestingControl VisitCompoundNextEntryType(NBT_TAG tagEntry)
	{
		//do something...
		return NestingControl::Enter;
	}

	NestingControl VisitCompoundNextEntry(NBT_TAG tagEntry, NBT_Type::String &&strKey)
	{
		//do something...
		return NestingControl::Enter;
	}

	ResultControl VisitCompoundEnd(void)
	{
		//do something...
		return ResultControl::Continue;
	}


	void VisitBegin(void)
	{
		//do something...
		return;
	}

	void VisitEnd(void)
	{
		//do something...
		return;
	}

	template<typename... Args>
	void VisitError(NBT_Print_Level lvl, const std::format_string<Args...> fmt, Args&&... args)
	{
		//throw or print error
		return;
	}
};

template <typename T>
concept IsLookLike_NBT_Visitor =
requires(
	T visitor,
	NBT_Visitor nbt_visitor,

	NBT_Type::Byte nbt_byte,
	NBT_Type::Short nbt_short,
	NBT_Type::Int nbt_int,
	NBT_Type::Long nbt_long,
	NBT_Type::Float nbt_float,
	NBT_Type::Double nbt_double,

	NBT_TAG nbt_tag,
	size_t nbt_list_length,
	size_t nbt_list_index,

	NBT_Type::ByteArray nbt_bytearray,
	NBT_Type::IntArray nbt_intarray,
	NBT_Type::LongArray nbt_longarray,
	NBT_Type::String nbt_string,
	NBT_Print_Level nbt_print_level
	)
{
	//数值类型访问方法
	{
		visitor.VisitNumericResult(nbt_byte)
	} -> std::same_as<decltype(nbt_visitor.VisitNumericResult(nbt_byte))>;
	{
		visitor.VisitNumericResult(nbt_short)
	} -> std::same_as<decltype(nbt_visitor.VisitNumericResult(nbt_short))>;
	{
		visitor.VisitNumericResult(nbt_int)
	} -> std::same_as<decltype(nbt_visitor.VisitNumericResult(nbt_int))>;
	{
		visitor.VisitNumericResult(nbt_long)
	} -> std::same_as<decltype(nbt_visitor.VisitNumericResult(nbt_long))>;
	{
		visitor.VisitNumericResult(nbt_float)
	} -> std::same_as<decltype(nbt_visitor.VisitNumericResult(nbt_float))>;
	{
		visitor.VisitNumericResult(nbt_double)
	} -> std::same_as<decltype(nbt_visitor.VisitNumericResult(nbt_double))>;

	//数组类型访问方法
	{
		visitor.VisitArrayResult(std::move(nbt_bytearray))
	} -> std::same_as<decltype(nbt_visitor.VisitArrayResult(std::move(nbt_bytearray)))>;
	{
		visitor.VisitArrayResult(std::move(nbt_intarray))
	} -> std::same_as<decltype(nbt_visitor.VisitArrayResult(std::move(nbt_intarray)))>;
	{
		visitor.VisitArrayResult(std::move(nbt_longarray))
	} -> std::same_as<decltype(nbt_visitor.VisitArrayResult(std::move(nbt_longarray)))>;

	//字符串访问方法
	{
		visitor.VisitStringResult(std::move(nbt_string))
	} -> std::same_as<decltype(nbt_visitor.VisitStringResult(std::move(nbt_string)))>;

	//结束标记访问方法
	{
		visitor.VisitEndResult()
	} -> std::same_as<decltype(nbt_visitor.VisitEndResult())>;


	//List相关方法
	{
		visitor.VisitListBegin(nbt_tag, nbt_list_length)
	} -> std::same_as<decltype(nbt_visitor.VisitListBegin(nbt_tag, nbt_list_length))>;
	{
		visitor.VisitListNextElement(nbt_tag, nbt_list_index)
	} -> std::same_as<decltype(nbt_visitor.VisitListNextElement(nbt_tag, nbt_list_index))>;
	{
		visitor.VisitListEnd()
	} -> std::same_as<decltype(nbt_visitor.VisitListEnd())>;

	//Compound相关方法
	{
		visitor.VisitCompoundBegin()
	} -> std::same_as<decltype(nbt_visitor.VisitCompoundBegin())>;
	{
		visitor.VisitCompoundNextEntryType(nbt_tag)
	} -> std::same_as<decltype(nbt_visitor.VisitCompoundNextEntryType(nbt_tag))>;
	{
		visitor.VisitCompoundNextEntry(nbt_tag, std::move(nbt_string))
	} -> std::same_as<decltype(nbt_visitor.VisitCompoundNextEntry(nbt_tag, std::move(nbt_string)))>;
	{
		visitor.VisitCompoundEnd()
	} -> std::same_as<decltype(nbt_visitor.VisitCompoundEnd())>;

	//开始/结束方法
	{
		visitor.VisitBegin()
	};
	{
		visitor.VisitEnd()
	};

	//错误处理（不完全验证）
	{
		visitor.VisitError(nbt_print_level, "error message")
	};
	{
		visitor.VisitError(nbt_print_level, "error with code: {}", 0)
	};
	{
		visitor.VisitError(nbt_print_level, "error with info: {}", "test info")
	};
};

static_assert(IsLookLike_NBT_Visitor<NBT_Visitor>);
