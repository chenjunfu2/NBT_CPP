# NBT_Type.hpp

此文件包含类NBT_Type，用于提供nbt类型定义与类型辅助工具

## 类NBT_Type
- 禁止构造
- 禁止析构
- 纯类型、模板声明

**类信息**
```cpp
声明：
using Float_Raw = uint32_t;
using Double_Raw = uint64_t;
说明：
浮点数原始数据大小声明，如果平台未支持
指定位数的浮点数，则使用此原始格式


声明：
using End
using Byte
using Short
using Int
using Long
using Float
using Double
using ByteArray
using IntArray
using LongArray
using String
using List
using Compound
说明：
与NBT标准相同的类型


```