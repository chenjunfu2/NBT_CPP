# NBT_IO.hpp

此文件包含类NBT_IO，用于提供nbt文件读写，解压与压缩功能

## 类NBT_IO
- 禁止构造
- 禁止析构
- 静态成员函数

**类信息**
```cpp
声明：
template<typename T = std::vector<uint8_t>>
static bool WriteFile(const std::string &strFileName, const T &tData)
说明：
写出数据到指定文件
	模板：
		T：任意顺序容器类型
	参数：
		strFileName：写出的文件的路径
		tData：写出数据的对象
返回值：写出是否成功


声明：
template<typename T = std::vector<uint8_t>>
static bool ReadFile(const std::string &strFileName, T &tData)
说明：
读取数据从指定文件
	模板：
		T：任意顺序容器类型
	参数：
		strFileName：读取的文件的路径
		tData：读取数据的的对象
返回值：读取是否成功


声明：
static bool IsFileExist(const std::string &sFileName)
说明：
判断文件是否存在
	参数：
		sFileName：判断的文件的路径
返回值：文件是否存在（注意如果判断出错也会返回不存在）


声明：
static bool IsZlib(uint8_t u8DataFirst, uint8_t u8DataSecond)
说明：
判断当前数据是否为Zlib压缩（不严谨但快速）
	参数：
		u8DataFirst：数据的第一个字节
		u8DataSecond：数据的第二个字节
返回值：是否为Zlib压缩


声明：
static bool IsGzip(uint8_t u8DataFirst, uint8_t u8DataSecond)
说明：
判断当前数据是否为Gzip压缩（不严谨但快速）
	参数：
		u8DataFirst：数据的第一个字节
		u8DataSecond：数据的第二个字节
返回值：是否为Gzip压缩


声明：
template<typename T>
requires (sizeof(typename T::value_type) == 1 && std::is_trivially_copyable_v<typename T::value_type>)
static bool IsDataZipped(T &tData)
说明：
判断当前数据是否被压缩（不论Gzip还是Zlib）
	模板：
		T：任意顺序容器类型，要求容器值类型必须大小为1且可平凡拷贝
	参数：
		tData：需要判断数据的对象
返回值：数据是否被压缩


声明：
template<typename I, typename O>
requires (sizeof(typename I::value_type) == 1 && std::is_trivially_copyable_v<typename I::value_type> &&
		  sizeof(typename O::value_type) == 1 && std::is_trivially_copyable_v<typename O::value_type>)
static void DecompressData(O &oData, const I &iData)
说明：
解压数据，注意输出容器不能是输入容器，该函数在失败时会抛出异常
	模板：
		I：任意顺序容器类型，要求容器值类型必须大小为1且可平凡拷贝
		O：任意顺序容器类型，要求容器值类型必须大小为1且可平凡拷贝
	参数：
		oData：输出原始数据的容器对象
		iData：输入压缩数据的容器对象
返回值：无


声明：
template<typename I, typename O>
requires (sizeof(typename I::value_type) == 1 && std::is_trivially_copyable_v<typename I::value_type> &&
		  sizeof(typename O::value_type) == 1 && std::is_trivially_copyable_v<typename O::value_type>)
static void CompressData(O &oData, const I &iData, int iLevel = Z_DEFAULT_COMPRESSION)
说明：
压缩数据，注意输出容器不能是输入容器，该函数在失败时会抛出异常
	模板：
		I：任意顺序容器类型，要求容器值类型必须大小为1且可平凡拷贝
		O：任意顺序容器类型，要求容器值类型必须大小为1且可平凡拷贝
	参数：
		oData：输出压缩数据的容器对象
		iData：输入原始数据的容器对象
		iLevel：压缩等级
返回值：无


声明：
template<typename I, typename O, typename ErrInfoFunc = NBT_Print>
requires (sizeof(typename I::value_type) == 1 && std::is_trivially_copyable_v<typename I::value_type> &&
		  sizeof(typename O::value_type) == 1 && std::is_trivially_copyable_v<typename O::value_type>)
static bool DecompressDataNoThrow(O &oData, const I &iData, ErrInfoFunc funcErrInfo = NBT_Print{ stderr }) noexcept
说明：
解压数据，注意输出容器不能是输入容器，该函数在失败时不会抛出异常，而是输出异常信息到funcErrInfo，并返回成功与否
	模板：
		I：任意顺序容器类型，要求容器值类型必须大小为1且可平凡拷贝
		O：任意顺序容器类型，要求容器值类型必须大小为1且可平凡拷贝
	参数：
		oData：输出原始数据的容器对象
		iData：输入压缩数据的容器对象
		funcErrInfo：打印异常信息的函数，需要接受std::format参数格式，默认值实现请参见[NBT_Print.hpp](NBT_Print.md)
返回值：是否成功执行


声明：
template<typename I, typename O, typename ErrInfoFunc = NBT_Print>
requires (sizeof(typename I::value_type) == 1 && std::is_trivially_copyable_v<typename I::value_type> &&
		  sizeof(typename O::value_type) == 1 && std::is_trivially_copyable_v<typename O::value_type>)
static bool CompressDataNoThrow(O &oData, const I &iData, int iLevel = Z_DEFAULT_COMPRESSION, ErrInfoFunc funcErrInfo = NBT_Print{ stderr }) noexcept
说明：
压缩数据，注意输出容器不能是输入容器，该函数在失败时不会抛出异常，而是输出异常信息到funcErrInfo，并返回成功与否
	模板：
		I：任意顺序容器类型，要求容器值类型必须大小为1且可平凡拷贝
		O：任意顺序容器类型，要求容器值类型必须大小为1且可平凡拷贝
	参数：
		oData：输出压缩数据的容器对象
		iData：输入原始数据的容器对象
		iLevel：压缩等级
		funcErrInfo：打印异常信息的函数，需要接受std::format参数格式，默认值实现请参见[NBT_Print.hpp](NBT_Print.md)
返回值：是否成功执行
```