# NBT_Helper.hpp

此文件包含类NBT_Helper，用于提供nbt对象操作辅助

## 类NBT_Helper
- 禁止构造
- 禁止析构
- 静态成员函数

**类信息**
```cpp
声明：
template<bool bSortCompound = true, typename PrintFunc = NBT_Print>
static void Print(const NBT_Node_View<true> nRoot, PrintFunc funcPrint = NBT_Print{ stdout }, bool bPadding = true, bool bNewLine = true)
说明：
打印nbt对象
	模板：
		bSortCompound：为true则对compound进行排序，否则无顺序保证
		PrintFunc：用于自动推导funcPrint可调用对象类型
	参数：
		nRoot：任意NBT_Type或NBT_Node，自动构造为view类型，具体请参考[NBT_Node_View.hpp](NBT_Node_View.md)
		funcPrint：任意可调用打印对象，需接受cpp20的std::format样式格式化字符串，默认实现为[NBT_Print.hpp](NBT_Print.md)
		bPadding：为true则进行缩进补白，否则不进行
		bNewLine：控制打印完成后是否新启一行
返回值：无


声明：
template<bool bSortCompound = true>
static std::string Serialize(const NBT_Node_View<true> nRoot)
说明：
序列化nbt对象
	模板：
		bSortCompound：为true则对compound进行排序，否则无顺序保证
	参数：
		nRoot：任意NBT_Type或NBT_Node，自动构造为view类型，具体请参考[NBT_Node_View.hpp](NBT_Node_View.md)
返回值：包含类似snbt的格式字符串std::string


声明：
static void DefaultFunc(NBT_Hash &)
说明：
默认示例函数，用于Hash参数默认值
	参数：
		未使用，类型为NBT_Hash对象的引用
返回值：无


声明：
using DefaultFuncType = std::decay_t<decltype(DefaultFunc)>;
说明：
默认示例函数类型，用于Hash模板默认值


声明：
template<bool bSortCompound = true, typename TB = DefaultFuncType, typename TA = DefaultFuncType>
static NBT_Hash::HASH_T Hash(const NBT_Node_View<true> nRoot, NBT_Hash nbtHash, TB funBefore = DefaultFunc, TA funAfter = DefaultFunc)
说明：
计算nbt对象的hash
	模板：
		bSortCompound：为true则对compound进行排序，否则无顺序保证

	参数：
		nRoot：任意NBT_Type或NBT_Node，自动构造为view类型，具体请参考[NBT_Node_View.hpp](NBT_Node_View.md)
		nbtHash：用于计算哈希的对象，一般使用一个seed进行初始化，具体请参考[NBT_Hash.hpp](NBT_Hash.md)
		funBefore：在hash前添加其它数据的可调用对象，默认值请参考前面的DefaultFunc
		funAfter：在hash后添加其它数据的可调用对象，默认值请参考前面的DefaultFunc
返回值：计算的hash
```