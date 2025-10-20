# NBT_Helper.hpp

此文件包含类NBT_Helper，用于提供nbt对象操作辅助

## 类NBT_Helper
- 禁止构造
- 禁止析构
- 静态成员函数

**类信息**
```cpp
声明：
template<typename PrintFunc = NBT_Print>
static void Print(const NBT_Node_View<true> nRoot, PrintFunc funcPrint = NBT_Print{}, bool bPadding = true, bool bNewLine = true)
说明：
打印nbt对象


```