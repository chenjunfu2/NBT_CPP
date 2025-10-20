# Compiler_Define.h

此文件仅用于判断编译器类型并提供对应宏</br>
首次包含此文件获得定义，重复包含此文件则取消定义</br>
重复包含取消定义的功能，用于防止头文件名称空间污染</br>

## 宏定义
```cpp
声明：
__COMPILER_MSVC__
说明：
为1则代表当前编译器是msvc
为0则代表当前编译器不是msvc


声明：
__COMPILER_GCC__
说明：
为1则代表当前编译器是gcc
为0则代表当前编译器不是gcc


声明：
__COMPILER_CLANG__
说明：
为1则代表当前编译器是clang
为0则代表当前编译器不是clang
```