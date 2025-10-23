# NBT_Endian.hpp

此文件包含类NBT_Endian，用于处理大小端运算，根据实际平台字节序类型进行自动匹配</br>

## 类NBT_Endian
- 禁止构造
- 禁止析构
- 静态成员函数

**类信息**
```cpp
声明：
template<typename T>
requires std::integral<T>
constexpr static T ByteSwapAny(T data) noexcept
说明：
保守实现，仅需平台支持位操作，颠倒字节序，需要整数字节数为2的倍数或字节数为1
	模板：
		T：任意整数类型
	参数：
		data：任意整数类型的值
返回值为字节序的颠倒形式


声明：
static uint16_t ByteSwap16(uint16_t data) noexcept
说明：
颠倒字节序16位特化版，如果平台支持内建指令，则使用平台内建指令，否则落到保守实现ByteSwapAny
	参数：
		data：uint16_t类型的值
返回值为字节序的颠倒形式


声明：
static uint32_t ByteSwap32(uint32_t data) noexcept
说明：
颠倒字节序32位特化版，如果平台支持内建指令，则使用平台内建指令，否则落到保守实现ByteSwapAny
	参数：
		data：uint32_t类型的值
返回值为字节序的颠倒形式


声明：
static uint64_t ByteSwap64(uint64_t data) noexcept
说明：
颠倒字节序64位特化版，如果平台支持内建指令，则使用平台内建指令，否则落到保守实现ByteSwapAny
	参数：
		data：uint64_t类型的值
返回值为字节序的颠倒形式


声明：
template<typename T>
requires std::integral<T>
constexpr static T AutoByteSwap(T data) noexcept
说明：
接受任意整数类型T，颠倒字节序，自动匹配位数，如果没有特化版，则落到保守实现ByteSwapAny
	模板：
		T：任意整数类型
	参数：
		data：任意整数类型的值
返回值为字节序的颠倒形式


声明：
template<typename T>
requires std::integral<T>
static T NativeToBigAny(T data) noexcept
说明：
接受任意整数类型T，自动匹配位数，从当前平台字节序转换到大端字节序，如果平台字节序与大端相同，则不做转换
	模板：
		T：任意整数类型
	参数：
		data：任意整数类型的值
如果进行转换，则返回值为字节序的颠倒形式


声明：
template<typename T>
requires std::integral<T>
static T NativeToLittleAny(T data) noexcept
说明：
接受任意整数类型T，自动匹配位数，从当前平台字节序转换到小端字节序，如果平台字节序与小端相同，则不做转换
	模板：
		T：任意整数类型
	参数：
		data：任意整数类型的值
如果进行转换，则返回值为字节序的颠倒形式


声明：
template<typename T>
requires std::integral<T>
static T BigToNativeAny(T data) noexcept
说明：
接受任意整数类型T，自动匹配位数，从大端字节序转换到当前平台字节序，如果平台字节序与大端相同，则不做转换
	模板：
		T：任意整数类型
	参数：
		data：任意整数类型的值
如果进行转换，则返回值为字节序的颠倒形式


声明：
template<typename T>
requires std::integral<T>
static T LittleToNativeAny(T data) noexcept
说明：
接受任意整数类型T，自动匹配位数，从小端字节序转换到当前平台字节序，如果平台字节序与小端相同，则不做转换
	模板：
		T：任意整数类型
	参数：
		data：任意整数类型的值
如果进行转换，则返回值为字节序的颠倒形式
```