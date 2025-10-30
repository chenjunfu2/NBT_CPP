# 使用方式
一般情况下，直接使用\"#include<nbt_cpp\/NBT_All.hpp>\"即可包含所有有头文件</br>
具体使用示例请参见项目根目录下的\"usage\"文件夹内的项目</br>

# 注意事项
本库仅头文件，无源文件</br>
本库仅针对nbt对象处理与nbt二进制文件处理，不涉及snbt转换与处理，不提供snbt相关功能</br>
本库仅单线程，请不要多线程同时访问同一个NBT对象，需要多线程请加锁，或每个线程访问独立的NBT对象</br>

# 可选依赖
- xxhash（解锁NBT_Helper中的Hash功能）
- zlib（解锁NBT_IO中的压缩解压相关功能）

## 可选依赖激活方式
**如果你是通过vcpkg安装的，会自动激活依赖，请直接忽略下面内容，同时也请不要编辑vcpkg_config.h**</br>
</br>
**如果你不是通过vcpkg安装的，那么在头文件vcpkg_config.h中：**</br>
### zlib配置
安装zlib库则存在下面内容，否则删除下面内容</br>
```cpp
//use zlib
#define CJF2_NBT_CPP_USE_ZLIB
```
### xxhash配置
安装xxhash库则存在下面内容，否则删除下面内容</br>
```cpp
//use xxhash
#define CJF2_NBT_CPP_USE_XXHASH
```

# 库内容主要内容介绍
正常情况下，所有单独的可用模块都只在**NBT_All.hpp**中包含，
如果你需要单独的模块而非要求全部模块，可以在**NBT_All.hpp**中查找。</br>
</br>
任何时候，其它的头文件都有可能存在额外的依赖或互相依赖，
直接包含它们可能导致错误，除非你知道你在干什么。</br>
</br>
但是单独的模块可能会引入其它带有依赖的模块，而非只存在模块自身，
比如**NBT_Node.hpp**中就包含了其它的NBT类型，而这些类型又和NBT_Node存在相互依赖，
但是其它的NBT类型在看到NBT_Node之前只存在前向声明，单独包含它们则会导致问题。</br>
</br>
下面的说明，解释了**NBT_All.hpp**中的头文件中的额外依赖关系，以便更好的使用此库。</br>
</br>
### NBT_Node.hpp
- NBT_TAG.hpp
- NBT_Type.hpp
- NBT_Array.hpp
- NBT_String.hpp
- NBT_List.hpp
- NBT_Compound.hpp

这个头文件基本上包含了所有可用的NBT类型，只要包含此头文件，即可使用所有的NBT类型。
同时也包含了所有NBT类型对应的Tag（也就是enum），tag与类型可以在编译期互相转化。
具体的类型都在**NBT_Type.hpp**中定义，其它的类型则是未实例化的模板形式，请不要直接使用它们。
因为这些类型需要和NBT_Node互相引用，所以在看到NBT_Node类型前，它们不应被实例化。

### NBT_Node_View.hpp
- NBT_Node.hpp

这个头文件仅包含NBT_Node.hpp，类似于std::string_view与std::string的关系，
它只是NBT_Node类型的View，不持有对象，仅为对象的视图，方便在一些不需要持有对象，
但是需要处理任意NBT_Node类型访问的地方使用，比如NBT_Helper中，可以处理任意对象，
但是为了处理任意对象，导致需要把当前对象拿去构造一个临时的NBT_Node（不论是拷贝还是移动），
都太不优雅，这时候直接构造为NBT_Node_View就能以虚拟的NBT_Node（接口基本一致）形式访问。

### NBT_Helper.hpp
- NBT_Print.hpp
- NBT_Node.hpp
- NBT_Node_View.hpp
- NBT_Hash.hpp（仅在安装xxhash库的情况下）

这个头文件基本上是处理nbt对象打印、序列化、哈希的帮助函数，
不使用重载而是单独提供的原因是，cpp不方便去包装任何builtin类型并给这些类型提供各种重载，
反而是给访问做到一起才更方便，所以这样设计，而在MC中因为用的是Java，本身提供builtin类型的包装器，
所以使用重载更为方便，此为设计模式的差异。

### NBT_Reader.hpp 与 NBT_Writer.hpp
- NBT_Print.hpp
- NBT_Node.hpp
- NBT_Endian.hpp

这两个头文件包含了NBT_Print（打印功能的默认实现，可被替换），
同时也包含了NBT_Node，因为它们必须看见所有类型才能知道怎么读写，
最后包含了NBT_Endian，以使得不同平台能处理大小端转换，
它们基本上做的内容是差不多的，所以包含的模块也一致，
一个是从字节流中读取到NBT类型，另一个是从NBT类型中写出到字节流，
有点像std::ifstream和std::ofstream的功能

### NBT_IO.hpp
- NBT_Print.hpp

这个头文件基本上是完成从文件中读写NBT字节流与压缩解压功能的，
是否提供压缩解压功能取决于是否安装zlib库
基本上不存在额外的NBT库依赖，它只处理文件与字节流


# 头文件详细信息
- [Compiler_Define.h](Compiler_Define.md)
- [MUTF8_Tool.hpp](MUTF8_Tool.md)
- [NBT_All.hpp](NBT_All.md)
- [NBT_Array.hpp](NBT_Array.md)
- [NBT_Compound.hpp](NBT_Compound.md)
- [NBT_Endian.hpp](NBT_Endian.md)
- [NBT_Hash.hpp](NBT_Hash.md)
- [NBT_Helper.hpp](NBT_Helper.md)
- [NBT_IO.hpp](NBT_IO.md)
- [NBT_List.hpp](NBT_List.md)
- [NBT_Node.hpp](NBT_Node.md)
- [NBT_Node_View.hpp](NBT_Node_View.md)
- [NBT_Print.hpp](NBT_Print.md)
- [NBT_Reader.hpp](NBT_Reader.md)
- [NBT_String.hpp](NBT_String.md)
- [NBT_TAG.hpp](NBT_TAG.md)
- [NBT_Type.hpp](NBT_Type.md)
- [NBT_Writer.hpp](NBT_Writer.md)
- [vcpkg_config.h](vcpkg_config.md)