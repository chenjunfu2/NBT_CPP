#pragma once

#include <vector>

class NBT_Reader;
class NBT_Writer;
class NBT_Helper;

/// @brief 一个公开继承自标准库std::vector（在NBT_Type中给出模板实例化）的代理类。
/// 无特殊成员，构造与使用方式与标准库std::vector一致。
/// @tparam Array 需要继承的父类类型
/// @note 用户不应该独立使用使用此类，请使用在NBT_Type中给出的模板实例化类型别名。
template<typename Array>
class MyArray :public Array//暂时不考虑保护继承
{
	friend class NBT_Reader;
	friend class NBT_Writer;
	friend class NBT_Helper;

public:
	//继承基类构造
	using Array::Array;
};