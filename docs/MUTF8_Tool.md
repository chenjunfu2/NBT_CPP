# MUTF8_Tool.hpp

此文件包含类MUTF8_Tool与StringLiteral，用于提供Java的Modified-UTF-8的静态字符串生成与动态转换

## 类StringLiteral
- 通过字符串数组构造
- 默认析构
- 编译期使用

### 类声明
```cpp
template<typename T, size_t N>
class StringLiteral : public std::array<T, N>
```
**解释**</br>
继承自std::array

|模板|说明|
|-|-|
|T|字符串类型|
|N|字符串大小|

### 类型别名
```cpp
using Super = std::array<T, N>;
```
**解释**</br>
继承的父类类型

### 构造函数
```cpp
constexpr StringLiteral(const T(&_tStr)[N]) noexcept
```
**解释**</br>
从字符串字面量、字符串数组构造编译期StringLiteral

|参数|说明|
|-|-|
|_tStr|字符串字面量、字符串数组的const引用|

**返回值**</br>
构造函数默认

### 析构函数
```cpp
constexpr ~StringLiteral(void)
```
**解释**</br>
默认析构

|参数|说明|
|-|-|
|无|无|

**返回值**</br>
析构函数默认

## 类MUTF8_Tool
- 禁止构造
- 禁止析构
- 可编译期运算
- 静态成员函数

### 类声明
```cpp
template<typename MU8T = uint8_t, typename U16T = char16_t, typename U8T = char8_t>
class MUTF8_Tool
```
**解释**</br>

|模板|说明|
|-|-|
|MU8T|modified utf-8的字节类型|
|U16T|utf-16的字节类型|
|U8T|utf-8的字节类型|

### 类型别名
```cpp
using MU8_T = MU8T;
using U16_T = U16T;
using U8_T = U8T;
```
**解释**</br>
模板参数类型别名

### 构造函数
```cpp
MUTF8_Tool(void) = delete;
```
**解释**</br>
不可构造

### 析构函数
```cpp
~MUTF8_Tool(void) = delete;
```
**解释**</br>
不可析构

### 成员函数
```cpp
static constexpr size_t U16ToMU8Length(const std::basic_string_view<U16T> &u16String)
static constexpr size_t U16ToMU8Length(const U16T *u16String, size_t szStringLength)
```
**解释**</br>
动态精确计算utf16转换到mutf8的最终长度，提供两种重载

**重载1**

|参数|说明|
|-|-|
|u16String|utf16字符串类型的std::basic_string_view|

**重载2**

|参数|说明|
|-|-|
|u16String|utf16字符串指针|
|szStringLength|utf16字符串长度（注意是长度而非字节数，且不包含末尾\0）|

**返回值**</br>
计算的长度（注意是长度而非字节数，且不包含末尾\0）

### 成员函数
```cpp
static std::basic_string<MU8T> U16ToMU8(const std::basic_string_view<U16T> &u16String, size_t szReserve = 0)
static std::basic_string<MU8T> U16ToMU8(const U16T *u16String, size_t szStringLength, size_t szReserve = 0)
```
**解释**</br>
动态转换utf16到mutf8，提供两种重载

**重载1**

|参数|说明|
|-|-|
|u16String|utf16字符串类型的std::basic_string_view|

**重载2**

|参数|说明|
|-|-|
|u16String|utf16字符串指针|
|szStringLength|utf16字符串长度（注意是长度而非字节数，且不包含末尾\0）|

**所有重载**

|参数|说明|
|-|-|
|szReserve|需要扩容的字节数，如果通过U16ToMU8Length计算最终长度，可以传递给此参数以避免动态扩容开销|

**返回值**</br>
转换后的mutf8类型string

### 成员函数
```cpp
template<StringLiteral u16String>
requires std::is_same_v<typename decltype(u16String)::value_type, U16T>
static consteval auto U16ToMU8(void)
```
**解释**</br>
静态转换utf16到mutf8

|模板|说明|
|-|-|
|u16String|utf16静态字符串或字符数组（包含长度信息）|

|参数|说明|
|-|-|
|无|无|
		
**返回值**</br>
std::array存储的mutf8静态字符数组

### 成员函数
```cpp
static constexpr size_t U8ToMU8Length(const std::basic_string_view<U8T> &u8String)
static constexpr size_t U8ToMU8Length(const U8T *u8String, size_t szStringLength)
```
**解释**</br>
动态精确计算utf8转换到mutf8的最终长度，提供两种重载

**重载1**

|参数|说明|
|-|-|
|u8String|utf8字符串类型的std::basic_string_view|

**重载2**

|参数|说明|
|-|-|
|u8String|utf8字符串指针|
|szStringLength|utf8字符串长度（注意是长度而非字节数，且不包含末尾\0）|

**返回值**</br>
计算的长度（注意是长度而非字节数，且不包含末尾\0）

### 成员函数
```cpp
static std::basic_string<MU8T> U8ToMU8(const std::basic_string_view<U8T> &u8String, size_t szReserve = 0)
static std::basic_string<MU8T> U8ToMU8(const U8T *u8String, size_t szStringLength, size_t szReserve = 0)
```
**解释**</br>
动态转换utf8到mutf8，提供两种重载

**重载1**

|参数|说明|
|-|-|
|u8String|utf8字符串类型的std::basic_string_view|

**重载2**

|参数|说明|
|-|-|
|u8String|utf8字符串指针|
|szStringLength|utf8字符串长度（注意是长度而非字节数，且不包含末尾\0）|

**所有重载**

|参数|说明|
|-|-|
|szReserve|需要扩容的字节数，如果通过U8ToMU8Length计算最终长度，可以传递给此参数以避免动态扩容开销|

**返回值**</br>
转换后的mutf8类型string

### 成员函数
```cpp
template<StringLiteral u8String>
requires std::is_same_v<typename decltype(u8String)::value_type, U8T>
static consteval auto U8ToMU8(void)
```
**解释**</br>
静态转换utf8到mutf8

|模板|说明|
|-|-|
|u8String|utf8静态字符串或字符数组（包含长度信息）|

|参数|说明|
|-|-|
|无|无|

**返回值**</br>
std::array存储的mutf8静态字符数组


### 成员函数
```cpp
static constexpr size_t MU8ToU16Length(const std::basic_string_view<MU8T> &mu8String)
static constexpr size_t MU8ToU16Length(const MU8T *mu8String, size_t szStringLength)
```
**解释**</br>
动态精确计算mutf8转换到utf16的最终长度

**重载1**

|参数|说明|
|-|-|
|mu8String|mutf8字符串类型的std::basic_string_view|

**重载2**

|参数|说明|
|-|-|
|mu8String|mutf8字符串指针|
|szStringLength|mutf8字符串长度（注意是长度而非字节数，且不包含末尾\0）|

**返回值**</br>
计算的长度（注意是长度而非字节数，且不包含末尾\0）

### 成员函数
```cpp
static std::basic_string<U16T> MU8ToU16(const std::basic_string_view<MU8T> &mu8String, size_t szReserve = 0)
static std::basic_string<U16T> MU8ToU16(const MU8T *mu8String, size_t szStringLength, size_t szReserve = 0)
```
**解释**</br>
动态转换mutf8到utf16，提供两种重载

**重载1**

|参数|说明|
|-|-|
|mu8String|mutf8字符串类型的std::basic_string_view|

**重载2**

|参数|说明|
|-|-|
|mu8String|mutf8字符串指针|
|szStringLength|mutf8字符串长度（注意是长度而非字节数，且不包含末尾\0）|

**所有重载**

|参数|说明|
|-|-|
|szReserve|需要扩容的字节数，如果通过MU8ToU16Length计算最终长度，可以传递给此参数以避免动态扩容开销|

**返回值**</br>
转换后的utf16类型string


### 成员函数
```cpp
static constexpr size_t MU8ToU8Length(const std::basic_string_view<MU8T> &mu8String)
static constexpr size_t MU8ToU8Length(const MU8T *mu8String, size_t szStringLength)
```
**解释**</br>
动态精确计算mutf8转换到utf8的最终长度，提供两种重载

**重载1**

|参数|说明|
|-|-|
|mu8String|mutf8字符串类型的std::basic_string_view|

**重载1**

|参数|说明|
|-|-|
|mu8String|mutf8字符串指针|
|szStringLength|mutf8字符串长度（注意是长度而非字节数，且不包含末尾\0）|

**返回值**</br>
计算的长度（注意是长度而非字节数，且不包含末尾\0）


### 成员函数
```cpp
static std::basic_string<U8T> MU8ToU8(const std::basic_string_view<MU8T> &mu8String, size_t szReserve = 0)
static std::basic_string<U8T> MU8ToU8(const MU8T *mu8String, size_t szStringLength, size_t szReserve = 0)
```
**解释**</br>
动态转换mutf8到utf8，提供两种重载

**重载1**

|参数|说明|
|-|-|
|mu8String|mutf8字符串类型的std::basic_string_view|

**重载2**

|参数|说明|
|-|-|
|mu8String|mutf8字符串指针|
|szStringLength|mutf8字符串长度（注意是长度而非字节数，且不包含末尾\0）|

**所有重载**

|参数|说明|
|-|-|
|szReserve|需要扩容的字节数，如果通过MU8ToU8Length计算最终长度，可以传递给此参数以避免动态扩容开销|

**返回值**</br>
转换后的utf8类型string

## 宏定义

### 定义
```cpp
#define U16CV2MU8(u16String)
```
**解释**</br>
动态转换，代理api：U16ToMU8


### 定义
```cpp
#define MU8CV2U16(mu8String)
```
**解释**</br>
动态转换，代理api：MU8ToU16


### 定义
```cpp
#define U8CV2MU8(u8String)
```
**解释**</br>
动态转换，代理api：U8ToMU8


### 定义
```cpp
#define MU8CV2U8(mu8String)
```
**解释**</br>
动态转换，代理api：MU8ToU8


### 定义
```cpp
#define U16TOMU8STR(u16LiteralString)
```
**解释**</br>
静态转换，代理api：U16ToMU8


### 定义
```cpp
#define U8TOMU8STR(u8LiteralString)
```
**解释**</br>
静态转换，代理api：U8ToMU8
