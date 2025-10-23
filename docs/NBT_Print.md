# NBT_Print.hpp

此文件包含类NBT_Print，用于提供std::format格式化输出，是许多类使用的输出方式的默认值

## 类NBT_Print
- 通过c文件对象构造
- 此类为默认打印实现，实际可被使用此类为默认值参数的函数的调用方以类似此类的仿函数参数重写的其它类型替换，比如调用方实现了一个My_Print，只要重载了仿函数调用运算符且参数与此类的仿函数调用运算符一致，则可以直接替换此类并传递给目标参数

**类信息**
```cpp
宏定义：
#define FMT_STR _Fmt_string
说明：
在旧版本msvc中使用内部的format_string（_Fmt_string）


宏定义：
#define FMT_STR format_string
说明：
在其它编译器或新版msvc中使用标准的format_string


声明：
NBT_Print(FILE *_pfOutput)
说明：
使用c文件对象指针构造，类不持有此指针，析构不销毁对象
	参数：
		_pfOutput：指向FILE的文件对象指针
返回值：构造函数默认


声明：
NBT_Print(void)
说明：
默认析构
	参数：
		无
返回值：析构函数默认


声明：
template<typename... Args>
void operator()(const std::FMT_STR<Args...> fmt, Args&&... args) noexcept
说明：
仿函数调用重载运算符，使用std::format格式化字符串输出到类构造时指定的c文件对象中
函数不能也不应该抛出任何异常，因为函数可能用于异常信息打印，不能出现二次异常，
实现中如果std::format出现任何二次异常，则放弃打印自定义信息，
从c标准io的printf输出新抛出的异常信息
	模板：
		Args：变长参数模板，接受任意类型
	参数：
		fmt：接受cpp20的format_string
		args：用于输出的值变长参数
返回值：无
```