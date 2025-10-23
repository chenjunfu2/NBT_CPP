# MUTF8_Tool.hpp

此文件包含类MUTF8_Tool与StringLiteral，用于提供Java的Modified-UTF-8的静态字符串生成与动态转换

## 类StringLiteral
- 通过字符串数组构造
- 默认析构
- 编译期使用

**类信息**
```cpp
类声明：
template<typename T, size_t N>
class StringLiteral : public std::array<T, N>
说明：
继承自std::array
	模板：
		T：字符串类型
		N：字符串大小


类型：
using Super = std::array<T, N>;
说明：
继承的父类类型


声明：
constexpr StringLiteral(const T(&_tStr)[N]) noexcept
说明：
从字符串字面量、字符串数组构造编译期StringLiteral
	参数：
		_tStr：字符串字面量、字符串数组的const引用
返回值：构造函数默认


声明：
constexpr ~StringLiteral(void)
说明：
默认析构
	参数：
		无
返回值：析构函数默认
```

## 类MUTF8_Tool
- 禁止构造
- 禁止析构
- 可编译期运算
- 静态成员函数

**类信息**
```cpp
类模板：
template<typename MU8T = uint8_t, typename U16T = char16_t, typename U8T = char8_t>
class MUTF8_Tool
说明：
MU8T为modified utf-8的字节类型
U16T为utf-16的字节类型
U8T为utf-8的字节类型


类型：
using MU8_T = MU8T;
using U16_T = U16T;
using U8_T = U8T;
说明：
模板参数类型别名


声明：
static constexpr size_t U16ToMU8Length(const std::basic_string_view<U16T> &u16String)
static constexpr size_t U16ToMU8Length(const U16T *u16String, size_t szStringLength)
说明：
动态精确计算utf16转换到mutf8的最终长度，提供两种重载
	重载1参数：
		u16String：utf16字符串类型的std::basic_string_view

	重载2参数：
		u16String：utf16字符串指针
		szStringLength：utf16字符串长度（注意是长度而非字节数，且不包含末尾\0）
返回值：计算的长度（注意是长度而非字节数，且不包含末尾\0）


声明：
static std::basic_string<MU8T> U16ToMU8(const std::basic_string_view<U16T> &u16String, size_t szReserve = 0)
static std::basic_string<MU8T> U16ToMU8(const U16T *u16String, size_t szStringLength, size_t szReserve = 0)
说明：
动态转换utf16到mutf8，提供两种重载
	重载1参数：
		u16String：utf16字符串类型的std::basic_string_view

	重载2参数：
		u16String：utf16字符串指针
		szStringLength：utf16字符串长度（注意是长度而非字节数，且不包含末尾\0）

	所有重载参数：
		szReserve：需要扩容的字节数，如果通过U16ToMU8Length计算最终长度，可以传递给此参数以避免动态扩容开销
返回值：转换后的mutf8类型string


声明：
template<StringLiteral u16String>
requires std::is_same_v<typename decltype(u16String)::value_type, U16T>
static consteval auto U16ToMU8(void)
说明：
静态转换utf16到mutf8
	模板：
		u16String：utf16静态字符串或字符数组（包含长度信息）
	参数：
		无
返回值：std::array存储的mutf8静态字符数组


声明：
static constexpr size_t U8ToMU8Length(const std::basic_string_view<U8T> &u8String)
static constexpr size_t U8ToMU8Length(const U8T *u8String, size_t szStringLength)
说明：
动态精确计算utf8转换到mutf8的最终长度，提供两种重载
	重载1参数：
		u8String：utf8字符串类型的std::basic_string_view

	重载2参数：
		u8String：utf8字符串指针
		szStringLength：utf8字符串长度（注意是长度而非字节数，且不包含末尾\0）
返回值：计算的长度（注意是长度而非字节数，且不包含末尾\0）


声明：
static std::basic_string<MU8T> U8ToMU8(const std::basic_string_view<U8T> &u8String, size_t szReserve = 0)
static std::basic_string<MU8T> U8ToMU8(const U8T *u8String, size_t szStringLength, size_t szReserve = 0)
说明：
动态转换utf8到mutf8，提供两种重载
	重载1参数：
		u8String：utf8字符串类型的std::basic_string_view

	重载2参数：
		u8String：utf8字符串指针
		szStringLength：utf8字符串长度（注意是长度而非字节数，且不包含末尾\0）

	所有重载参数：
		szReserve：需要扩容的字节数，如果通过U8ToMU8Length计算最终长度，可以传递给此参数以避免动态扩容开销
返回值：转换后的mutf8类型string


声明：
template<StringLiteral u8String>
requires std::is_same_v<typename decltype(u8String)::value_type, U8T>
static consteval auto U8ToMU8(void)
说明：
静态转换utf8到mutf8
	模板：
		u8String：utf8静态字符串或字符数组（包含长度信息）
	参数：
		无
返回值：std::array存储的mutf8静态字符数组


声明：
static constexpr size_t MU8ToU16Length(const std::basic_string_view<MU8T> &mu8String)
static constexpr size_t MU8ToU16Length(const MU8T *mu8String, size_t szStringLength)
说明：
动态精确计算mutf8转换到utf16的最终长度
	重载1参数：
		mu8String：mutf8字符串类型的std::basic_string_view

	重载2参数：
		mu8String：mutf8字符串指针
		szStringLength：mutf8字符串长度（注意是长度而非字节数，且不包含末尾\0）
返回值：计算的长度（注意是长度而非字节数，且不包含末尾\0）


声明：
static std::basic_string<U16T> MU8ToU16(const std::basic_string_view<MU8T> &mu8String, size_t szReserve = 0)
static std::basic_string<U16T> MU8ToU16(const MU8T *mu8String, size_t szStringLength, size_t szReserve = 0)
说明：
动态转换mutf8到utf16，提供两种重载
	重载1参数：
		mu8String：mutf8字符串类型的std::basic_string_view

	重载2参数：
		mu8String：mutf8字符串指针
		szStringLength：mutf8字符串长度（注意是长度而非字节数，且不包含末尾\0）

	所有重载参数：
		szReserve：需要扩容的字节数，如果通过MU8ToU16Length计算最终长度，可以传递给此参数以避免动态扩容开销
返回值：转换后的utf16类型string


声明：
static constexpr size_t MU8ToU8Length(const std::basic_string_view<MU8T> &mu8String)
static constexpr size_t MU8ToU8Length(const MU8T *mu8String, size_t szStringLength)
说明：
动态精确计算mutf8转换到utf8的最终长度，提供两种重载
	重载1参数：
		mu8String：mutf8字符串类型的std::basic_string_view

	重载2参数：
		mu8String：mutf8字符串指针
		szStringLength：mutf8字符串长度（注意是长度而非字节数，且不包含末尾\0）
返回值：计算的长度（注意是长度而非字节数，且不包含末尾\0）


声明：
static std::basic_string<U8T> MU8ToU8(const std::basic_string_view<MU8T> &mu8String, size_t szReserve = 0)
static std::basic_string<U8T> MU8ToU8(const MU8T *mu8String, size_t szStringLength, size_t szReserve = 0)
说明：
动态转换mutf8到utf8，提供两种重载
	重载1参数：
		mu8String：mutf8字符串类型的std::basic_string_view

	重载2参数：
		mu8String：mutf8字符串指针
		szStringLength：mutf8字符串长度（注意是长度而非字节数，且不包含末尾\0）

	所有重载参数：
		szReserve：需要扩容的字节数，如果通过MU8ToU8Length计算最终长度，可以传递给此参数以避免动态扩容开销
返回值：转换后的utf8类型string
```

## 宏定义
```cpp
声明：
U16CV2MU8(u16String)
说明：
动态转换，代理api：U16ToMU8


声明：
MU8CV2U16(mu8String)
说明：
动态转换，代理api：MU8ToU16


声明：
U8CV2MU8(u8String)
说明：
动态转换，代理api：U8ToMU8


声明：
MU8CV2U8(mu8String)
说明：
动态转换，代理api：MU8ToU8


声明：
U16TOMU8STR(u16LiteralString)
说明：
静态转换，代理api：U16ToMU8


声明：
U8TOMU8STR(u8LiteralString)
说明：
静态转换，代理api：U8ToMU8
```