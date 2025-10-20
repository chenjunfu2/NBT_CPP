# NBT_Helper.hpp

此文件包含类NBT_Helper，用于提供nbt对象操作辅助

## 类NBT_Helper
- 禁止构造
- 禁止析构
- 静态成员函数

**类信息**
```cpp
声明：
template<typename PrintFunc = NBT_Print>
static void Print(const NBT_Node_View<true> nRoot, PrintFunc funcPrint = NBT_Print{ stdout }, bool bPadding = true, bool bNewLine = true)
说明：
打印nbt对象
	参数nRoot为任意NBT_Type或NBT_Node，自动构造为view类型
	参数funcPrint为任意可调用打印对象，需接受cpp20的std::format样式格式化字符串
	参数bPadding为true则进行缩进补白，否则不进行
	参数bNewLine控制打印完成后是否新启一行
返回void


声明：
static std::string Serialize(const NBT_Node_View<true> nRoot)
说明：
直接序列化为类似snbt的格式字符串，无换行，默认格式
	参数nRoot为任意NBT_Type或NBT_Node，自动构造为view类型
返回包含类似snbt的格式字符串std::string


声明：



```