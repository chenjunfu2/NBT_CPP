#pragma once

#include "NBT_Node.hpp"

/// @file

//用法和NBT_Node一致，但是只持有构造时传入对象的指针，且不持有对象
//对象随时可销毁，如果销毁后使用持有销毁对象的view则行为未定义，用户自行负责
//构造传入nullptr指针且进行使用则后果自负

/// @class NBT_Node_View
/// @brief NBT节点的视图，用于引用而不持有对象，类似于标准库的std::string与std::string_view的关系
/// @note 该类使用std::variant变体来存储指向不同类型NBT数据的指针，提供类型安全的访问接口
/// @warning 视图类不持有实际数据对象，只持有指向外部数据的指针，它也不会延长外部对象或临时对象的生命周期。如果外部数据被销毁后继续使用视图则行为未定义，用户需自行负责数据生命周期管理
/// @tparam bIsConst 模板参数，指定视图是否持有数据的只读引用，为true则持有对象的const指针，为false持有对象的非const指针。
/// 这里的模板值控制的状态类似于指向const对象的指针与指向对象的const指针的区别。
/// @note 与NBT_Node强制持有对象所有权不同，NBT_Node_View提供轻量级访问方式，适用于：
/// - 函数参数传递：避免拷贝开销，或者移动的持有权转移，可以直接引用现有NBT数据对象本身
/// - 临时数据访问：在明确数据生命周期时提供零拷贝访问
/// - 统一接口处理：以变体方式兼容任意NBT类型，同时保持对原始数据的直接操作能力
template <bool bIsConst>
class NBT_Node_View
{
	template <bool _bIsConst>
	friend class NBT_Node_View;//需要设置自己为友元，这样不同模板的类实例之间才能相互访问

private:
	//类型列表展开，声明std::variant
	template <typename T>
	struct AddConstIf
	{
		using type = std::conditional<bIsConst, const T, T>::type;
	};

	template <typename T>
	using PtrType = typename AddConstIf<T>::type *;

	template <typename T>
	struct TypeListPointerToVariant;

	template <typename... Ts>
	struct TypeListPointerToVariant<NBT_Type::_TypeList<Ts...>>
	{
		using type = std::variant<PtrType<Ts>...>;//展开成指针类型
	};

	using VariantData = TypeListPointerToVariant<NBT_Type::TypeList>::type;

	//数据对象（仅持有数据的指针）
	VariantData data;
public:
	/// @brief 静态常量，表示当前视图引用的数据是否只读
	static inline constexpr bool is_const = bIsConst;

	/// @brief 通用构造函数（适用于const与非const的情况）
	/// @tparam T 要引用的数据类型
	/// @param value 要引用的数据对象的引用
	/// @note 要求类型T必须是NBT_Type类型列表中的任意一个，且不是NBT_Node类型
	/// @note 传入的引用对象必须在使用期间保持有效，否则行为未定义
	template <typename T>
	requires(!std::is_same_v<std::decay_t<T>, NBT_Node>)//此构造任何时候都生效，const的情况下可以引用非const对象的指针
	NBT_Node_View(T &value) : data(&value)
	{
		static_assert(NBT_Type::IsValidType_V<std::decay_t<T>>, "Invalid type for NBT node view");
	}

	/// @brief 通用构造函数（仅适用于const）
	/// @tparam T 要引用的数据类型
	/// @param value 要引用的数据对象的常量引用
	/// @note 要求类型T必须是NBT_Type类型列表中的任意一个，且不是NBT_Node类型
	/// @warning 传入的引用对象必须在使用期间保持有效，否则行为未定义
	template <typename T>
	requires(!std::is_same_v<std::decay_t<T>, NBT_Node> && bIsConst)//此构造只在const的情况下生效，因为非const的情况下不能通过const对象初始化非const指针
	NBT_Node_View(const T &value) : data(&value)
	{
		static_assert(NBT_Type::IsValidType_V<std::decay_t<T>>, "Invalid type for NBT node view");
	}

	/// @brief 用于一定程度防止用户直接使用临时对象初始化视图的防御手段
	/// @tparam T 任意类型
	/// @param value 任意类型的临时对象
	/// @note 这是一个删除的构造函数
	template <typename T>
	requires(!std::is_same_v<std::decay_t<T>, NBT_Node>)//这里的requires是为了保证不要让任意类型的右值全部重载到这里
	NBT_Node_View(T &&value) = delete;

	/// @brief 从NBT_Node构造视图（适用于const与非const的情况）
	/// @param node 要创建视图的NBT_Node对象
	/// @note 创建指向NBT_Node内部数据的视图
	/// @warning 传入的NBT_Node对象必须在使用期间保持有效，否则行为未定义
	NBT_Node_View(NBT_Node &node)
	{
		std::visit([this](auto &arg)
			{
				this->data = &arg;
			}, node.data);
	}

	/// @brief 从NBT_Node构造视图（仅适用于const）
	/// @param node 要创建视图的NBT_Node对象
	/// @note 创建指向NBT_Node内部数据的只读视图
	/// @warning 传入的NBT_Node对象必须在使用期间保持有效，否则行为未定义
	template <typename = void>//requires占位模板
	requires(bIsConst)//此构造只在const的情况下生效，因为非const的情况下不能通过const对象初始化非const指针
	NBT_Node_View(const NBT_Node &node)
	{
		std::visit([this](auto &arg)
			{
				this->data = &arg;
			}, node.data);
	}

	/// @brief 用于一定程度防止用户直接使用临时对象初始化视图的防御手段
	/// @param node NBT_Node类型的临时对象
	/// @note 这是一个删除的构造函数
	NBT_Node_View(NBT_Node &&node) = delete;

	/// @brief 从非const视图隐式构造const视图
	/// @tparam void 占位模板参数
	/// @param other 用于构造的非const视图对象
	/// @note 从NBT_Node_View<false>隐式构造为NBT_Node_View<true>
	template <typename = void>//requires占位模板
	requires(bIsConst)//此构造只在const的情况下生效，不能从const视图转换到非const视图
	NBT_Node_View(const NBT_Node_View<false> &_Other)
	{
		std::visit([this](auto &arg)
			{
				this->data = arg;//此处arg为非const指针
			}, _Other.data);
	}

private:
	// 隐藏默认构造（TAG_End）
	// 它应该存在，并在内部使用，但用户不应该使用，所以它是private的，但不是delete的
	NBT_Node_View() = default;
public:

	/// @brief 默认析构函数
	/// @note 视图析构不会影响实际数据对象，只释放内部指针引用
	~NBT_Node_View() = default;

	//拷贝构造
	NBT_Node_View(const NBT_Node_View &_NBT_Node_View) : data(_NBT_Node_View.data)
	{}

	//移动构造
	NBT_Node_View(NBT_Node_View &&_NBT_Node_View) noexcept : data(std::move(_NBT_Node_View.data))
	{}

	NBT_Node_View &operator=(const NBT_Node_View &_NBT_Node_View)
	{
		data = _NBT_Node_View.data;
		return *this;
	}

	NBT_Node_View &operator=(NBT_Node_View &&_NBT_Node_View) noexcept
	{
		data = std::move(_NBT_Node_View.data);
		return *this;
	}


	bool operator==(const NBT_Node_View &_Right) const noexcept
	{
		if (GetTag() != _Right.GetTag())
		{
			return false;
		}

		return std::visit([this](const auto *argL, const auto *argR)-> bool
			{
				using TL = const std::decay_t<decltype(argL)>;
				return *argL == *(TL)argR;
			}, this->data, _Right.data);
	}

	bool operator!=(const NBT_Node_View &_Right) const noexcept
	{
		if (GetTag() != _Right.GetTag())
		{
			return true;
		}

		return std::visit([this](const auto *argL, const auto *argR)-> bool
			{
				using TL = const std::decay_t<decltype(argL)>;
				return *argL != *(TL)argR;
			}, this->data, _Right.data);
	}

	std::partial_ordering operator<=>(const NBT_Node_View &_Right) const noexcept
	{
		if (GetTag() != _Right.GetTag())
		{
			return std::partial_ordering::unordered;
		}

		return std::visit([this](const auto *argL, const auto *argR)-> std::partial_ordering
			{
				using TL = const std::decay_t<decltype(argL)>;
				return *argL <=> *(TL)argR;
			}, this->data, _Right.data);
	}

	//获取标签类型
	NBT_TAG GetTag() const noexcept
	{
		return (NBT_TAG)data.index();//返回当前存储类型的index（0基索引，与NBT_TAG enum一一对应）
	}

	//类型安全访问
	template<typename T>
	const T &GetData() const
	{
		return *std::get<PtrType<T>>(data);
	}

	template<typename T>
	requires(!bIsConst)
	T &GetData()
	{
		return *std::get<PtrType<T>>(data);
	}

	// 类型检查
	template<typename T>
	bool TypeHolds() const
	{
		return std::holds_alternative<PtrType<T>>(data);
	}

	//针对每种类型重载一个方便的函数
	/*
		纯类型名函数：直接获取此类型，不做任何检查，由标准库std::get具体实现决定
		Is开头的类型名函数：判断当前NBT_Node是否为此类型
		纯类型名函数带参数版本：查找当前Compound指定的Name并转换到类型引用返回，不做检查，具体由标准库实现定义
		Has开头的类型名函数带参数版本：查找当前Compound是否有特定Name的Tag，并返回此Name的Tag（转换到指定类型）的指针
	*/
#define TYPE_GET_FUNC(type)\
const NBT_Type::type &Get##type() const\
{\
	return *std::get<PtrType<NBT_Type::type>>(data);\
}\
\
template <typename = void>\
requires(!bIsConst)\
NBT_Type::type &Get##type()\
{\
	return *std::get<PtrType<NBT_Type::type>>(data);\
}\
\
bool Is##type() const\
{\
	return std::holds_alternative<PtrType<NBT_Type::type>>(data);\
}\
friend std::conditional_t<bIsConst, const NBT_Type::type &, NBT_Type::type &> Get##type(NBT_Node_View & node)\
{\
	return node.Get##type();\
}\
\
friend const NBT_Type::type &Get##type(const NBT_Node_View & node)\
{\
	return node.Get##type();\
}

	TYPE_GET_FUNC(End);
	TYPE_GET_FUNC(Byte);
	TYPE_GET_FUNC(Short);
	TYPE_GET_FUNC(Int);
	TYPE_GET_FUNC(Long);
	TYPE_GET_FUNC(Float);
	TYPE_GET_FUNC(Double);
	TYPE_GET_FUNC(ByteArray);
	TYPE_GET_FUNC(IntArray);
	TYPE_GET_FUNC(LongArray);
	TYPE_GET_FUNC(String);
	TYPE_GET_FUNC(List);
	TYPE_GET_FUNC(Compound);

#undef TYPE_GET_FUNC
};