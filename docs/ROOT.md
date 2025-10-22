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

**可选依赖激活方式**
```cpp
#define USE_ZLIB//安装zlib库的情况下
#define USE_XXHASH//安装xxhash库的情况下
```

# 头文件信息
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