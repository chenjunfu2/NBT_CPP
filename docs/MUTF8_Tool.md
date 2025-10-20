# MUTF8_Tool.hpp

此文件包含类MUTF8_Tool与StringLiteral，用于提供Java的Modified-UTF-8的静态字符串生成与动态转换

## 类StringLiteral
- 通过字符串数组构造
- 默认析构
- 编译期使用

**类信息**
'''cpp
类模板：
template<typename T, size_t N>
class StringLiteral : public std::array<T, N>
说明：
T为字符串类型，N为字符串大小


类型：
using Super = std::array<T, N>;
说明：
继承的父类类型


声明：
constexpr StringLiteral(const T(&_tStr)[N]) noexcept
说明：
从字符串字面量、字符串数组构造编译期StringLiteral


声明：
constexpr ~StringLiteral(void)
说明：
默认析构
'''

## 类MUTF8_Tool
- 禁止构造
- 禁止析构
- 可编译期运算
- 静态成员函数

**类信息**
'''
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
动态精确计算utf16转换到mutf8的最终长度，提供两种重载，支持string_view或字符串指针+长度
返回值为计算的长度


声明：
static std::basic_string<MU8T> U16ToMU8(const std::basic_string_view<U16T> &u16String, size_t szReserve = 0)
static std::basic_string<MU8T> U16ToMU8(const U16T *u16String, size_t szStringLength, size_t szReserve = 0)
说明：
动态转换utf16到mutf8，提供两种重载，支持string_view或字符串指针+长度
szReserve代表需要扩容的字节数，如果通过U16ToMU8Length计算最终长度，可以传递给此参数以避免动态扩容开销
返回值为转换后的string


声明：
template<StringLiteral u16String>
requires std::is_same_v<typename decltype(u16String)::value_type, U16T>
static consteval auto U16ToMU8(void)
说明：
静态转换utf16到mutf8，字符串通过模板传入并构造为StringLiteral，以实现静态计算
返回值为std::array存储的静态字符数组


声明：
static constexpr size_t U8ToMU8Length(const std::basic_string_view<U8T> &u8String)
static constexpr size_t U8ToMU8Length(const U8T *u8String, size_t szStringLength)
说明：
动态精确计算utf8转换到mutf8的最终长度，提供两种重载，支持string_view或字符串指针+长度
返回值为计算的长度


声明：
static std::basic_string<MU8T> U8ToMU8(const std::basic_string_view<U8T> &u8String, size_t szReserve = 0)
static std::basic_string<MU8T> U8ToMU8(const U8T *u8String, size_t szStringLength, size_t szReserve = 0)
说明：
动态转换utf8到mutf8，提供两种重载，支持string_view或字符串指针+长度
szReserve代表需要扩容的字节数，如果通过U8ToMU8Length计算最终长度，可以传递给此参数以避免动态扩容开销
返回值为转换后的string


声明：
template<StringLiteral u8String>
requires std::is_same_v<typename decltype(u8String)::value_type, U8T>
static consteval auto U8ToMU8(void)
说明：
静态转换utf8到mutf8，字符串通过模板传入并构造为StringLiteral，以实现静态计算
返回值为std::array存储的静态字符数组


声明：
static constexpr size_t MU8ToU16Size(const std::basic_string_view<MU8T> &mu8String)
static constexpr size_t MU8ToU16Size(const MU8T *mu8String, size_t szStringLength)
说明：
动态精确计算mutf8转换到utf16的最终长度，提供两种重载，支持string_view或字符串指针+长度
返回值为计算的长度


声明：
static std::basic_string<U16T> MU8ToU16(const std::basic_string_view<MU8T> &mu8String, size_t szReserve = 0)
static std::basic_string<U16T> MU8ToU16(const MU8T *mu8String, size_t szStringLength, size_t szReserve = 0)
说明：
动态转换mutf8到utf16，提供两种重载，支持string_view或字符串指针+长度
szReserve代表需要扩容的字节数，如果通过MU8ToU16Size计算最终长度，可以传递给此参数以避免动态扩容开销
返回值为转换后的string


声明：
static constexpr size_t MU8ToU8Length(const std::basic_string_view<MU8T> &mu8String)
static constexpr size_t MU8ToU8Length(const MU8T *mu8String, size_t szStringLength)
说明：
动态精确计算mutf8转换到utf8的最终长度，提供两种重载，支持string_view或字符串指针+长度
返回值为计算的长度


声明：
static std::basic_string<U8T> MU8ToU8(const std::basic_string_view<MU8T> &mu8String, size_t szReserve = 0)
static std::basic_string<U8T> MU8ToU8(const MU8T *mu8String, size_t szStringLength, size_t szReserve = 0)
说明：
动态转换mutf8到utf8，提供两种重载，支持string_view或字符串指针+长度
szReserve代表需要扩容的字节数，如果通过MU8ToU16Size计算最终长度，可以传递给此参数以避免动态扩容开销
返回值为转换后的string
'''

## 宏定义
'''cpp
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
'''