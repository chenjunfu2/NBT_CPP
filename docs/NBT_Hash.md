# NBT_Hash.hpp

此文件包含一个用于包装xxhash调用的类

## 类NBT_Hash
- 禁止拷贝
- 允许移动

**类信息**
```cpp
声明：
using HASH_T = XXH64_hash_t;
说明：
包装XXH64_hash_t


声明：
NBT_Hash(const HASH_T &tHashSeed)
说明：
构造函数，创建xxhash对象，并使用一个种子来初始化对象


声明：
~NBT_Hash(void)
说明：
析构函数，释放xxhash对象


声明：
NBT_Hash(NBT_Hash &&_Move) noexcept
NBT_Hash operator=(NBT_Hash &&_Move) noexcept
说明：
赋值与构造函数，从对象移动


声明：
HASH_T Digest(void)
说明：
获取当前已添加数据的哈希值




```