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
	参数：
		tHashSeed：哈希种子
返回值：构造函数默认


声明：
~NBT_Hash(void)
说明：
析构函数，释放xxhash对象
	参数：
		无
返回值：析构函数默认


声明：
NBT_Hash(NBT_Hash &&_Move) noexcept
NBT_Hash &operator=(NBT_Hash &&_Move) noexcept
说明：
赋值与构造函数，从对象移动
	所有重载参数：
		_Move：从此对象移动并构造或赋值
返回值：移动构造默认或对象的引用


声明：
HASH_T Digest(void)
说明：
获取当前已添加数据的哈希值
	参数：
		无
返回值：当前哈希值


声明：
void Update(const void *pData, size_t szSize)

template<typename T>
requires(std::is_trivially_copyable_v<T>)
void Update(const T &tData)

template<typename T, size_t N>
requires(std::is_trivially_copyable_v<T>)
void Update(const T(&tDataArr)[N])
说明：
使用数据+大小的形式更新hash，提供多个重载
	重载1参数：
		pData：指向数据的无类型指针
		szSize：数据的大小（字节数）

	重载2模板：
		T：任意可平凡复制对象
	重载2参数：
		tData：任意可平凡复制对象的值的const引用

	重载3模板：
		T：任意可平凡复制对象
		N：任意可平凡复制对象组成的数组长度
	重载3参数：
		tDataArr：任意可平凡复制对象组成的数组的const引用
返回值：无


声明：
static HASH_T Hash(const void *pData, size_t szSize, HASH_T tHashSeed)

template<typename T>
requires(std::is_trivially_copyable_v<T>)
static HASH_T Hash(const T &tData, HASH_T tHashSeed)

template<typename T, size_t N>
requires(std::is_trivially_copyable_v<T>)
static HASH_T Hash(const T(&tDataArr)[N], HASH_T tHashSeed)
说明：
静态函数，使用数据+大小的形式直接获得hash，提供多个重载
	重载1参数：
		pData：指向数据的无类型指针
		szSize：数据的大小（字节数）

	重载2模板：
		T：任意可平凡复制对象
	重载2参数：
		tData：任意可平凡复制对象的值的const引用

	重载3模板：
		T：任意可平凡复制对象
		N：任意可平凡复制对象组成的数组长度
	重载3参数：
		tDataArr：任意可平凡复制对象组成的数组的const引用

	所有重载参数：
		tHashSeed：用于计算的哈希种子
返回值：计算的哈希值
```