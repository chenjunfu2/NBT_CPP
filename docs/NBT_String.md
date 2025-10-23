# NBT_String.hpp

此文件包含类MyStringView与类MyString，与它们对应的hash函数在std名称空间中的特化</br>
MyStringView是继承自标准库std::basic_string_view的代理类</br>
MyString是继承自标准库std::basic_string的代理类</br>
用于存储与处理Java的Modified-UTF-8字符串</br>

## 类MyStringView
- 继承所有基类构造
- 允许编译期构造
- 允许字符串或字符数组直接构造
- 允许从MUTF8_Tool返回的std::array直接构造
- 允许从MyString对象显示构造
- 禁止从临时MyString对象构造

**类信息**
```cpp
类声明：
template<typename String, typename StringView>
class MyStringView : public StringView
说明：
继承自StringView
	模板：
		String：当前类对应的String类型
		StringView：当前类的父类


声明：
using StringView::StringView;
说明：
继承所有父类对象的构造


声明：
template<size_t N>
constexpr MyStringView(const typename StringView::value_type(&ltrStr)[N])
说明：
从C风格字符串字面量或字符数组的引用直接构造，如果从非静态字符串构造，则必须确保对象生存期大于等于此对象生存期，否则行为未定义
从C风格字符串字面量或字符数组的引用构造时，会进行末尾\0字符排除
	模板：
		N：数组的长度
	参数：
		ltrStr：数组的const引用
返回值：构造默认返回


声明：
template<size_t N>
constexpr MyStringView(const std::array<typename StringView::value_type, N> &strArray)
说明：
从std::array的引用直接构造，注意std::array的生存期必须大于等于此对象生存期，否则行为未定义
从std::array的引用构造时，不会进行末尾\0字符排除
	模板：
		N：数组的长度
	参数：
		strArray：std::array的const引用
返回值：构造默认返回


声明：
constexpr explicit MyStringView(const MyString<String, StringView> &myString)
说明：
从MyString对象的引用显示构造，注意MyString对象的生存期必须大于等于此对象生存期，否则行为未定义
从MyString对象的引用构造时，不会进行末尾\0字符排除
	参数：
		myString：MyString对象的const引用
返回值：构造默认返回


声明：
std::basic_string_view<char> GetCharTypeView(void) const noexcept
说明：
获取char类型的视图以方便使用（兼容）
	参数：
		无
返回值：char类型的std::basic_string_view
```

## 类MyString
- 继承所有基类构造
- 允许字符串或字符数组直接拷贝构造
- 允许从MUTF8_Tool返回的std::array直接拷贝构造
- 允许从MyString对象直接拷贝构造

**类信息**
```cpp
类声明：
template<typename String, typename StringView>
class MyString :public String
说明：
继承自String
	模板：
		String：当前类的父类
		StringView：当前类对应的View类型
		

声明：
using View = MyStringView<String, StringView>;
说明：
在此处实例化MyStringView模板，与MyStringView类进行关联


声明：
using String::String;
说明：
继承基类所有构造


声明：
template<size_t N>
MyString(const typename String::value_type(&ltrStr)[N])
说明：
从C风格字符串字面量或字符数组的引用直接拷贝构造，会进行末尾\0字符排除
	模板：
		N：数组的长度
	参数：
		ltrStr：数组的const引用
返回值：构造默认返回


声明：
template<size_t N>
MyString(const std::array<typename String::value_type, N> &strArray)
说明：
从std::array的引用直接拷贝构造，不会进行末尾\0字符排除
	模板：
		N：数组的长度
	参数：
		strArray：std::array的const引用
返回值：构造默认返回


声明：
MyString(const View &view)
说明：
从View（MyStringView<String, StringView>）直接拷贝构造string
	参数：
		view：视图（View）对象的const引用
返回值：构造默认返回


声明：
std::basic_string_view<char> GetCharTypeView(void) const noexcept
说明：
获取char类型的视图以方便使用（兼容）
	参数：
		无
返回值：char类型的std::basic_string_view


声明：
auto ToCharTypeUTF8(void) const
说明：
转换到UTF8并获取为char类型的std::string以方便使用（兼容）
	参数：
		无
返回值：char类型的std::basic_string


声明：
auto ToUTF8(void) const
说明：
转换到UTF8的std::string
	参数：
		无
返回值：char8_t类型的std::basic_string


声明：
auto ToWchartTypeUTF16(void) const
说明：
转换到UTF16并获取为wchar_t类型的std::string以方便使用（兼容）
	参数：
		无
返回值：wchar_t类型的std::basic_string


声明：
auto ToUTF16(void) const
说明：
转换到UTF16的std::string
	参数：
		无
返回值：char16_t类型的std::basic_string


声明：
void FromCharTypeUTF8(std::basic_string_view<char> u8String)
说明：
从char类型但是实际编码为utf8的字符串视图替换当前内容
	参数：
		u8String：char类型但是实际为utf8编码的字符串视图
返回值：无


声明：
void FromUTF8(std::basic_string_view<char8_t> u8String)
说明：
从char8_t类型的utf8编码的字符串视图替换当前内容
	参数：
		u8String：char8_t类型的utf8编码的字符串视图
返回值：无


声明：
void FromWchartTypeUTF16(std::basic_string_view<wchar_t> u16String)
说明：
从wchar_t类型但是实际编码为utf16的字符串视图替换当前内容
	参数：
		u16String：wchar_t类型但是实际为utf16编码的字符串视图
返回值：无


声明：
void FromUTF16(std::basic_string_view<char16_t> u16String)
说明：
从char16_t类型的utf16编码的字符串视图替换当前内容
	参数：
		u16String：char16_t类型的utf16编码的字符串视图
返回值：无
```

## 名称空间std
- 新增hash特化MyString
- 新增hash特化MyStringView
- 用于将此类作为map的key的情况

### 类hash特化MyString
```cpp
类声明：
template<typename String, typename StringView>
struct hash<MyString<String, StringView>>
说明：
模板参数与MyString一致，为hash特化


声明：
size_t operator()(const MyString<String, StringView> &s) const noexcept
说明：
仿函数调用，计算类的哈希
	参数：
		s：类的const引用
返回值：类内数据的哈希值
```

### 类hash特化MyStringView
```cpp
类声明：
template<typename String, typename StringView>
struct hash<MyStringView<String, StringView>>
说明：
模板参数与MyStringView一致，为hash特化


声明：
size_t operator()(const MyStringView<String, StringView> &s) const noexcept
说明：
仿函数调用，计算类的哈希
	参数：
		s：类的const引用
返回值：类内数据的哈希值
```